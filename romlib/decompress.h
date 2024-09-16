/***************************************************************************
 *  GBAGI: The Game Boy Advance Adventure Game Interpreter
 *  Copyright (C) 2003,2004 Brian Provinciano, http://www.bripro.com
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
#ifndef _DECOMPRESS_H
#define _DECOMPRESS_H
/******************************************************************************/

void initLZW(void);
void closeLZW(void);
bool setBITS(int32_t value);
uint8_t *decode_string(uint8_t*buffer, uint32_t code);
uint32_t input_code(uint8_t**input);
bool LZW_expand(uint8_t*in, uint8_t*out, int32_t len);

void PIC_expand(uint8_t*in, uint8_t*out, int32_t len);
/******************************************************************************/
#endif
/******************************************************************************/
