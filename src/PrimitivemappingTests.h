//PrimitivemappingTests.h

// Copyright (C) 2026 Jiri Fajta
// License: MIT License.

#ifndef PrimitivemappingTests_H_INCLUDED
#define PrimitivemappingTests_H_INCLUDED

/* PSn00bSDK */
#include <psxgpu.h>

/* PSYQ */
// #include <libgpu.h>


#include <stdio.h>
#include "Structs/PolyX.h"


void initPOLY_X_Test(POLY_X *polyX);


//SPRT_XX
void copySPRT_X(void *poly, POLY_X *polyX);

//TILE_XX
void copyTILE_X(void *poly, POLY_X *polyX);

//LINE_XX
void copyLINE_F2(LINE_F2 *poly, POLY_X *polyX);
void copyLINE_G2(LINE_G2 *poly, POLY_X *polyX);

//FXX
void copyPOLY_F3(POLY_F3 *poly, POLY_X *polyX);
void copyPOLY_F4(POLY_F4 *poly, POLY_X *polyX);
void copyPOLY_FT3(POLY_FT3 *poly, POLY_X *polyX);
void copyPOLY_FT4(POLY_FT4 *poly, POLY_X *polyX);

// GXX
void copyPOLY_G3(POLY_G3 *poly, POLY_X *polyX);
void copyPOLY_G4(POLY_G4 *poly, POLY_X *polyX);
void copyPOLY_GT3(POLY_GT3 *poly, POLY_X *polyX);
void copyPOLY_GT4(POLY_GT4 *poly, POLY_X *polyX);
void keepVertexData(void *poly, unsigned long size);

#endif