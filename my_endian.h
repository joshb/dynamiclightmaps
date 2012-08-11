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

#ifndef __MY_ENDIAN_H__
#define __MY_ENDIAN_H__

#ifdef __GLIBC__
#include <stdint.h>
#else
#include <sys/types.h>
#endif

#ifndef int8_t
 #define int8_t char
#endif
#ifndef uint8_t
 #define uint8_t unsigned char
#endif
#ifndef int16_t
 #define int16_t short
#endif
#ifndef uint16_t
 #define uint16_t unsigned short
#endif
#ifndef int32_t
 #define int32_t long
#endif
#ifndef uint32_t
 #define uint32_t unsigned long
#endif

#define native_to_le_float le_to_native_float
#define native_to_le_int le_to_native_int
#define native_to_le_uint le_to_native_uint
#define native_to_le_short le_to_native_short
#define native_to_le_ushort le_to_native_ushort

float le_to_native_float(float f);
int32_t le_to_native_int(int32_t i);
uint32_t le_to_native_uint(uint32_t i);
int16_t le_to_native_short(int16_t s);
uint16_t le_to_native_ushort(uint16_t s);

#endif /* __MY_ENDIAN_H__ */
