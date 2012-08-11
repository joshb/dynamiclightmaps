/*
 * Copyright (C) 2003 Josh A. Beam
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include "my_endian.h"

struct pcx_header {
	uint8_t manufacturer;
	uint8_t version;
	uint8_t encoding;
	uint8_t bitsperpixel;
	int16_t xmin;
	int16_t ymin;
	int16_t xmax;
	int16_t ymax;
	uint16_t horizdpi;
	uint16_t vertdpi;
	uint8_t palette[48];
	uint8_t reserved;
	uint8_t colorplanes;
	uint16_t bytesperline;
	uint16_t palettetype;
	uint16_t hscrsize;
	uint16_t vscrsize;
	uint8_t filler[54];
};

static int
read_scanline(FILE *fp, unsigned char *planes[], unsigned int num_planes,
              unsigned int bytesperline)
{
	unsigned int i, j;
	unsigned int p, count;
	uint8_t byte;

	for(p = 0; p < num_planes; p++) {
		for(i = 0; i < bytesperline;) {
			if(fread(&byte, 1, 1, fp) != 1)
				return 0;

			if(byte >> 6 == 0x3) {
				count = byte & ~(0x3 << 6);
				if(count == 0)
					return 0;
				if(fread(&byte, 1, 1, fp) != 1)
					return 0;
			} else {
				count = 1;
			}

			for(j = 0; j < count; j++)
				planes[p][i++] = byte;

			if(i >= bytesperline) {
				p++;
				if(p >= num_planes)
					return 1;
				i = 0;
			}
		}
	}

	return 1;
}

static unsigned char *
load_pcx_data_8(FILE *fp, int width, int height,
                unsigned int bytesperline)
{
	int i, j;
	int max;
	unsigned char *data, *p_data;
	unsigned char *line, *planes[1];
	unsigned int current_line = 0;
	uint8_t byte;
	unsigned char palette[256][3];

	p_data = (unsigned char *)malloc(sizeof(unsigned char) * width * height);
	if(!p_data)
		return NULL;

	line = (unsigned char *)malloc(sizeof(unsigned char) * bytesperline);
	if(!line) {
		free(p_data);
		return NULL;
	}
	planes[0] = line;

	while(current_line < (unsigned int)height) {
		if(read_scanline(fp, planes, 1, bytesperline) == 0) {
			free(p_data);
			free(line);
			return NULL;
		}

		for(i = 0; i < width; i++)
			p_data[width * current_line + i] = planes[0][i];

		current_line++;
	}

	/* read palette */
	fseek(fp, -769, SEEK_END);
	fread(&byte, 1, 1, fp);
	if(byte != 12) {
		fprintf(stderr, "Error: This ain't a palette\n");
		free(p_data);
		free(line);
		return NULL;
	}
	for(i = 0; i < 256; i++)
		fread(palette[i], 3, 1, fp);

	data = (unsigned char *)malloc(sizeof(unsigned char) * width * height * 3);
	if(!data) {
		free(p_data);
		free(line);
		return NULL;
	}

	max = width * height;
	j = 0;
	for(i = 0; i < max; i++) {
		data[j++] = palette[(p_data[i])][0];
		data[j++] = palette[(p_data[i])][1];
		data[j++] = palette[(p_data[i])][2];
	}

	free(p_data);
	free(line);
	return data;
}

static unsigned char *
load_pcx_data_24(FILE *fp, int width, int height,
                 unsigned int bytesperline)
{
	int i;
	unsigned char *data;
	unsigned char *line, *planes[3];
	unsigned int current_line = 0;

	data = (unsigned char *)malloc(sizeof(unsigned char) * width * height * 3);
	if(!data)
		return NULL;

	line = (unsigned char *)malloc(sizeof(unsigned char) * bytesperline * 3);
	if(!line) {
		free(data);
		return NULL;
	}
	planes[0] = line;
	planes[1] = planes[0] + bytesperline;
	planes[2] = planes[1] + bytesperline;

	while(current_line < (unsigned int)height) {
		if(read_scanline(fp, planes, 3, bytesperline) == 0) {
			free(data);
			free(line);
			return NULL;
		}

		for(i = 0; i < width; i++) {
			data[width * current_line * 3 + i * 3 + 0] = planes[0][i];
			data[width * current_line * 3 + i * 3 + 1] = planes[1][i];
			data[width * current_line * 3 + i * 3 + 2] = planes[2][i];
		}

		current_line++;
	}

	free(line);
	return data;
}

unsigned char *
read_pcx(const char *filename, unsigned int *widthp, unsigned int *heightp)
{
	FILE *fp;
	struct pcx_header header;
	unsigned char *data;
	int width, height;

	fp = fopen(filename, "rb");
	if(!fp) {
		fprintf(stderr, "pcx Error: Couldn't open %s for reading\n", filename);
		return NULL;
	}

	fread(&header, sizeof(struct pcx_header), 1, fp);
	header.xmin = le_to_native_short(header.xmin);
	header.ymin = le_to_native_short(header.ymin);
	header.xmax = le_to_native_short(header.xmax);
	header.ymax = le_to_native_short(header.ymax);
	header.bytesperline = le_to_native_ushort(header.bytesperline);

	if(header.bitsperpixel != 8) {
		fprintf(stderr, "Error: %s has unsupported number of bits per pixel\n", filename);
		return NULL;
	}
	if(header.colorplanes != 1 && header.colorplanes != 3) {
		fprintf(stderr, "Error: %s has unsupported number of color planes\n", filename);
		return NULL;
	}

	width = header.xmax - header.xmin + 1;
	height = header.ymax - header.ymin + 1;

	if(width < 1 || height < 1) {
		fprintf(stderr, "Error: %s has bad dimensions (%dx%d)\n", filename, width, height);
		return NULL;
	}

	switch(header.colorplanes) {
		default:
			data = NULL;
			break;
		case 1:
			data = load_pcx_data_8(fp, width, height, header.bytesperline);
			break;
		case 3:
			data = load_pcx_data_24(fp, width, height, header.bytesperline);
			break;
	}

	fclose(fp);
	*widthp = width;
	*heightp = height;

	if(!data)
		fprintf(stderr, "Error: Unable to load %s\n", filename);

	return data;
}
