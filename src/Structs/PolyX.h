//PolyX.h

// Copyright (C) 2025 Jiri Fajta
// License: MIT License.

#ifndef PolyX_H_INCLUDED
#define PolyX_H_INCLUDED

typedef struct {
	/* Universal GPU primitie struct */

	/* offset        */
	/* word  | bytes */
	/*---------------*/
	/* POS 0 | 0     */
	unsigned long tag;
	/* POS 1 | 4*/
	unsigned char r0, g0, b0, code;
	/* POS 2 | 8*/
	short x0,  y0;
	/* POS 3 | 12*/
	unsigned char u0, v0;
	unsigned short clut;
	/* POS 4 | 16*/
	unsigned char r1, g1, b1, p0;
	/* POS 5 | 20*/
	short x1, y1;
	/* POS 6 | 24*/
	unsigned char u1, v1;
	unsigned short tpage;
	/* POS 7 | 28*/
	unsigned char r2, g2, b2, p1;
	/* POS 8 | 32*/
	short x2, y2;
	/* POS 9 | 36*/
	unsigned char u2, v2;
	unsigned short p2;
	/* POS 10 | 40*/
	unsigned char r3, g3, b3, p3;
	/* POS 11 | 44*/
	short x3, y3;
	/* POS 12 | 48*/
	unsigned char u3, v3;
	unsigned short p4;
	/* POS 13 | 52*/
	short w, h;
} POLY_X;

#endif
