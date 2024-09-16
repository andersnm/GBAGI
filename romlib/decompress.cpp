/***************************************************************************
 *  GBAGI: The Game Boy Advance Adventure Game Interpreter
 *  Copyright (C) 2003,2004 Brian Provinciano, http://www.bripro.com
 *
 *  lzw code by Lance Ewing
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software Foundation,
 *  Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 ***************************************************************************/

/******************************************************************************/
#include <stdint.h>
#include <stdlib.h>
#include "decompress.h"
/******************************************************************************/
#define  NORMAL     0
#define  ALTERNATE  1

#define MAXBITS		12
#define TABLE_SIZE	18041		/* strange number */
#define START_BITS	9

int32_t BITS, MAX_VALUE, MAX_CODE;
uint32_t* prefix_code;
uint8_t *append_character, *decode_stack;
int32_t input_bit_count;	/* Number of bits in input bit buffer */
uint32_t input_bit_buffer;

/******************************************************************************/
void initLZW(void)
{
	decode_stack 		= (uint8_t*) calloc(8192,sizeof(uint8_t));
	prefix_code 		= (uint32_t*) calloc(TABLE_SIZE,sizeof(uint32_t));
	append_character 	= (uint8_t*) calloc(TABLE_SIZE,sizeof(uint8_t));

	input_bit_count=0;			/* Number of bits in input bit buffer */
	input_bit_buffer=0L;
}
/******************************************************************************/
void closeLZW(void)
{
	free(decode_stack);
	free(prefix_code);
	free(append_character);
}

/******************************************************************************/
//Purpose: To adjust the number of bits used to store codes to the value
//passed in.
/******************************************************************************/
bool setBITS(int32_t value)
{
	if(value==MAXBITS)
		return true;

	BITS = value;
	MAX_VALUE = (1 << BITS) - 1;
	MAX_CODE = MAX_VALUE - 1;
	return false;
}

/******************************************************************************/
// Purpose: To return the string that the code taken from the input buffer
// represents. The string is returned as a stack, i.e. the characters are
// in reverse order.
/******************************************************************************/
uint8_t *decode_string(uint8_t *buffer, uint32_t code)
{
	uint32_t i;

	for (i = 0; code > 255; )
	{
		*buffer++=append_character[code];
		code=prefix_code[code];
		if (i++>=4000)
		{
			// ErrorMessage(_T("lzw: error in code expansion.\n"));
            return NULL;
		}
	}
	*buffer=(uint8_t)code;

	return buffer;
}

/******************************************************************************/
//Purpose: To return the next code from the input buffer.
/******************************************************************************/
uint32_t input_code(uint8_t **input)
{
	uint32_t r;

	while (input_bit_count <= 24)
	{
		input_bit_buffer |= (uint32_t) *(*input)++ << input_bit_count;
		input_bit_count += 8;
	}
	r = (input_bit_buffer & 0x7FFF) % (1 << BITS);
	input_bit_buffer >>= BITS;
	input_bit_count -= BITS;

	return r;
}

/******************************************************************************/
// Purpose: To uncompress the data contained in the input buffer and store
// the result in the output buffer. The fileLength parameter says how
// many bytes to uncompress. The compression itself is a form of LZW that
// adjusts the number of bits that it represents its codes in as it fills
// up the available codes. Two codes have special meaning:
//
//  code 256 = start over
//  code 257 = end of data
/******************************************************************************/
bool LZW_expand(uint8_t *in, uint8_t *out, int32_t len)
{
	int32_t lzwnext, lzwnew, lzwold;
	int32_t c;
	uint8_t *s, *end;

	initLZW();

	setBITS(START_BITS);	/* Starts at 9-bits */
	lzwnext = 257;			/* Next available code to define */

	end = (unsigned char *)((long)out + (long)len);

	lzwold = input_code(&in);	/* Read in the first code */
	c = lzwold;
	lzwnew = input_code(&in);

	while ((out < end) && (lzwnew != 0x101))
	{
		if (lzwnew == 0x100)
		{
			/* Code to "start over" */
			lzwnext = 258;
			setBITS(START_BITS);
			lzwold = input_code(&in);
			c = lzwold;
			*out++ = (char)c;
			lzwnew = input_code(&in);
		}
		else
		{
			if (lzwnew >= lzwnext)
			{
				/* Handles special LZW scenario */
				*decode_stack = (uint8_t)c;
				s = decode_string(decode_stack+1, lzwold);
			}
			else
				s = decode_string(decode_stack, lzwnew);
      		if(!s) return false;
			/* Reverse order of decoded string and
			 * store in out buffer
			 */
			c = *s;
			while (s >= decode_stack)
				*out++ = *s--;

			if (lzwnext > MAX_CODE)
				setBITS(BITS + 1);

			prefix_code[lzwnext] = lzwold;
			append_character[lzwnext] = (uint8_t)c;
			lzwnext++;
			lzwold = lzwnew;

			lzwnew = input_code(&in);
		}
	}
	closeLZW();
    return true;
}
/******************************************************************************/
void PIC_expand(uint8_t *in, uint8_t *out, int32_t len)
{
    unsigned char data, oldData=0, outData;
    int mode = NORMAL;
	uint8_t *end=out+len;

	while(out<end) {

      data = (*in++);

      if (mode == ALTERNATE)
	 outData = ((data & 0xF0) >> 4) + ((oldData & 0x0F) << 4);
      else
	 outData = data;

      if ((outData == 0xF0) || (outData == 0xF2)) {
	 *out++ = (outData);
	 if (mode == NORMAL) {
	    data = (*in++);
	    *out++ = ((data & 0xF0) >> 4);
	    mode = ALTERNATE;
	 }
	 else {
	    *out++ = ((data & 0x0F));
	    mode = NORMAL;
	 }
      }
      else
	 *out++ = (outData);

      oldData = data;
   }
}
/******************************************************************************/