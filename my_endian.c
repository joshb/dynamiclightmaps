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
#include "my_endian.h"

#ifndef UNKNOWN_ENDIAN
#define UNKNOWN_ENDIAN	0
#endif
#ifndef LITTLE_ENDIAN
#define LITTLE_ENDIAN	1
#endif
#ifndef BIG_ENDIAN
#define BIG_ENDIAN		2
#endif

static uint32_t endian = UNKNOWN_ENDIAN;

static uint32_t
set_endian()
{
	if(endian == UNKNOWN_ENDIAN) {
		unsigned char c[] = { 1, 0 };
		unsigned short *p = (unsigned short *)c;

		if(*p == 1) {
			endian = LITTLE_ENDIAN;
			printf("Using little-endian byte ordering\n");
		} else {
			endian = BIG_ENDIAN;
			printf("Using big-endian byte ordering\n");
		}
	}

	return endian;
}

static float
swap_float(float f)
{
	float retval;
	uint8_t *p1 = (uint8_t *)&f;
	uint8_t *p2 = (uint8_t *)&retval;

	p2[0] = p1[3];
	p2[1] = p1[2];
	p2[2] = p1[1];
	p2[3] = p1[0];

	return retval;
}

static int32_t
swap_int(int32_t i)
{
	int32_t retval;
	uint8_t *p1 = (uint8_t *)&i;
	uint8_t *p2 = (uint8_t *)&retval;

	p2[0] = p1[3];
	p2[1] = p1[2];
	p2[2] = p1[1];
	p2[3] = p1[0];

	return retval;
}

static uint32_t
swap_uint(uint32_t i)
{
	uint32_t retval;
	uint8_t *p1 = (uint8_t *)&i;
	uint8_t *p2 = (uint8_t *)&retval;

	p2[0] = p1[3];
	p2[1] = p1[2];
	p2[2] = p1[1];
	p2[3] = p1[0];

	return retval;
}

static int16_t
swap_short(int16_t s)
{
	int16_t retval;
	uint8_t *p1 = (uint8_t *)&s;
	uint8_t *p2 = (uint8_t *)&retval;

	p2[0] = p1[1];
	p2[1] = p1[0];

	return retval;
}

static uint16_t
swap_ushort(uint16_t s)
{
	uint16_t retval;
	uint8_t *p1 = (uint8_t *)&s;
	uint8_t *p2 = (uint8_t *)&retval;

	p2[0] = p1[1];
	p2[1] = p1[0];

	return retval;
}

float
le_to_native_float(float f)
{
	if(set_endian() == BIG_ENDIAN)
		return swap_float(f);

	return f;
}

int32_t
le_to_native_int(int32_t i)
{
	if(set_endian() == BIG_ENDIAN)
		return swap_int(i);

	return i;
}

uint32_t
le_to_native_uint(uint32_t i)
{
	if(set_endian() == BIG_ENDIAN)
		return swap_uint(i);

	return i;
}

int16_t
le_to_native_short(int16_t s)
{
	if(set_endian() == BIG_ENDIAN)
		return swap_short(s);

	return s;
}

uint16_t
le_to_native_ushort(uint16_t s)
{
	if(set_endian() == BIG_ENDIAN)
		return swap_ushort(s);

	return s;
}
