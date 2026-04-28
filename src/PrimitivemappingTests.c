//PrimitivemappingTests.c

// Copyright (C) 2026 Jiri Fajta
// License: MIT License.

/* PSn00bSDK */
#include <psxgpu.h>

/* PSYQ */
// #include <libgpu.h>


#include <stdio.h>
#include "Structs/PolyX.h"


void initPOLY_X_Test(POLY_X *polyX){
    //polyX->tag = 0x00;// both needs to be set by setPoly... anyway
    //polyX->code = 0x00;

    // set colors
    polyX->r0 = 0xA1;
    polyX->g0 = 0xA2;
    polyX->b0 = 0xA3;

    polyX->r1 = 0xA4;
    polyX->g1 = 0xA5;
    polyX->b1 = 0xA6;

    polyX->r2 = 0xA7;
    polyX->g2 = 0xA8;
    polyX->b2 = 0xA9;

    polyX->r3 = 0xAA;
    polyX->g3 = 0xAB;
    polyX->b3 = 0xAC;

    // set xy
    polyX->x0 = 0xB1;
    polyX->y0 = 0xB2;

    polyX->x1 = 0xB3;
    polyX->y1 = 0xB4;

    polyX->x2 = 0xB5;
    polyX->y2 = 0xB6;

    polyX->x3 = 0xB7;
    polyX->y3 = 0xB8;

    // set uv
    polyX->u0 = 0xD1;
    polyX->v0 = 0xD2;

    polyX->u1 = 0xD3;
    polyX->v1 = 0xD4;

    polyX->u2 = 0xD5;
    polyX->v2 = 0xD6;

    polyX->u3 = 0xD7;
    polyX->v3 = 0xD8;

    // set clut tpage and pads
    polyX->clut = 0xE1;
    polyX->tpage = 0xE2;

    polyX->p0 = 0xE3;
    polyX->p1 = 0xE4;
    polyX->p2 = 0xE5;
    polyX->p3 = 0xE6;
    polyX->p4 = 0xE7;

    // set wh
    polyX->w = 0xF1;
    polyX->h = 0xF2;
}


//SPRT_XX
void copySPRT_X(void *poly, POLY_X *polyX){
    *((long*)poly) = polyX->tag;
    *((long*)(poly) + 1) = *((long*)&polyX->r0);
    *((long*)(poly) + 2) = *((long*)&polyX->x0);
    *((long*)(poly) + 3) = *((long*)&polyX->u0);

    if((polyX->code & 0x64) == 0x64){
        *(((long*)poly) + 4) = *((long*)&polyX->w);
    }
}

//TILE_XX
void copyTILE_X(void *poly, POLY_X *polyX){
    *((long*)poly) = polyX->tag;
    *((long*)(poly) + 1) = *((long*)&polyX->r0);
    *((long*)(poly) + 2) = *((long*)&polyX->x0);

    if((polyX->code & 0x60) == 0x60){
        *(((long*)poly) + 3) = *((long*)&polyX->w);
    }
}

//LINE_XX
void copyLINE_F2(LINE_F2 *poly, POLY_X *polyX){
    poly->tag = polyX->tag;
    *((long*)&poly->r0) = *((long*)&polyX->r0);

    *((long*)&poly->x0) = *((long*)&polyX->x0);
    *((long*)&poly->x1) = *((long*)&polyX->x1);
}
void copyLINE_G2(LINE_G2 *poly, POLY_X *polyX){
    poly->tag = polyX->tag;
    *((long*)&poly->r0) = *((long*)&polyX->r0);
    *((long*)&poly->r1) = *((long*)&polyX->r1);

    *((long*)&poly->x0) = *((long*)&polyX->x0);
    *((long*)&poly->x1) = *((long*)&polyX->x1);
}

//FXX
void copyPOLY_F3(POLY_F3 *poly, POLY_X *polyX){
    poly->tag = polyX->tag;
    *((long*)&poly->r0) = *((long*)&polyX->r0);

    *((long*)&poly->x0) = *((long*)&polyX->x0);
    *((long*)&poly->x1) = *((long*)&polyX->x1);
    *((long*)&poly->x2) = *((long*)&polyX->x2);
}

void copyPOLY_F4(POLY_F4 *poly, POLY_X *polyX){
    poly->tag = polyX->tag;
    *((long*)&poly->r0) = *((long*)&polyX->r0);

    *((long*)&poly->x0) = *((long*)&polyX->x0);
    *((long*)&poly->x1) = *((long*)&polyX->x1);
    *((long*)&poly->x2) = *((long*)&polyX->x2);
    *((long*)&poly->x3) = *((long*)&polyX->x3);
}

void copyPOLY_FT3(POLY_FT3 *poly, POLY_X *polyX){
    poly->tag = polyX->tag;

    *((long*)&poly->r0) = *((long*)&polyX->r0);

    *((long*)&poly->x0) = *((long*)&polyX->x0);
    *((long*)&poly->x1) = *((long*)&polyX->x1);
    *((long*)&poly->x2) = *((long*)&polyX->x2);

    *((long*)&poly->u0) = *((long*)&polyX->u0);
    *((long*)&poly->u1) = *((long*)&polyX->u1);
    *((long*)&poly->u2) = *((long*)&polyX->u2);
}

void copyPOLY_FT4(POLY_FT4 *poly, POLY_X *polyX){
    poly->tag = polyX->tag;

    *((long*)&poly->r0) = *((long*)&polyX->r0);

    *((long*)&poly->x0) = *((long*)&polyX->x0);
    *((long*)&poly->x1) = *((long*)&polyX->x1);
    *((long*)&poly->x2) = *((long*)&polyX->x2);
    *((long*)&poly->x3) = *((long*)&polyX->x3);

    *((long*)&poly->u0) = *((long*)&polyX->u0);
    *((long*)&poly->u1) = *((long*)&polyX->u1);
    *((long*)&poly->u2) = *((long*)&polyX->u2);
    *((long*)&poly->u3) = *((long*)&polyX->u3);
}


// GXX
void copyPOLY_G3(POLY_G3 *poly, POLY_X *polyX){
    poly->tag = polyX->tag;
    *((long*)&poly->r0) = *((long*)&polyX->r0);
    *((long*)&poly->r1) = *((long*)&polyX->r1);
    *((long*)&poly->r2) = *((long*)&polyX->r2);

    *((long*)&poly->x0) = *((long*)&polyX->x0);
    *((long*)&poly->x1) = *((long*)&polyX->x1);
    *((long*)&poly->x2) = *((long*)&polyX->x2);
}

void copyPOLY_G4(POLY_G4 *poly, POLY_X *polyX){
    poly->tag = polyX->tag;
    *((long*)&poly->r0) = *((long*)&polyX->r0);
    *((long*)&poly->r1) = *((long*)&polyX->r1);
    *((long*)&poly->r2) = *((long*)&polyX->r2);
    *((long*)&poly->r3) = *((long*)&polyX->r3);

    *((long*)&poly->x0) = *((long*)&polyX->x0);
    *((long*)&poly->x1) = *((long*)&polyX->x1);
    *((long*)&poly->x2) = *((long*)&polyX->x2);
    *((long*)&poly->x3) = *((long*)&polyX->x3);

}

void copyPOLY_GT3(POLY_GT3 *poly, POLY_X *polyX){
    poly->tag = polyX->tag;
    //poly->code = polyX->code;
    *((long*)&poly->r0) = *((long*)&polyX->r0);
    *((long*)&poly->r1) = *((long*)&polyX->r1);
    *((long*)&poly->r2) = *((long*)&polyX->r2);

    *((long*)&poly->x0) = *((long*)&polyX->x0);
    *((long*)&poly->x1) = *((long*)&polyX->x1);
    *((long*)&poly->x2) = *((long*)&polyX->x2);


    *((long*)&poly->u0) = *((long*)&polyX->u0);
    *((long*)&poly->u1) = *((long*)&polyX->u1);
    *((long*)&poly->u2) = *((long*)&polyX->u2);
}

void copyPOLY_GT4(POLY_GT4 *poly, POLY_X *polyX){
    poly->tag = polyX->tag;

    *((long*)&poly->r0) = *((long*)&polyX->r0);
    *((long*)&poly->r1) = *((long*)&polyX->r1);
    *((long*)&poly->r2) = *((long*)&polyX->r2);
    *((long*)&poly->r3) = *((long*)&polyX->r3);

    *((long*)&poly->x0) = *((long*)&polyX->x0);
    *((long*)&poly->x1) = *((long*)&polyX->x1);
    *((long*)&poly->x2) = *((long*)&polyX->x2);
    *((long*)&poly->x3) = *((long*)&polyX->x3);

    *((long*)&poly->u0) = *((long*)&polyX->u0);
    *((long*)&poly->u1) = *((long*)&polyX->u1);
    *((long*)&poly->u2) = *((long*)&polyX->u2);
    *((long*)&poly->u3) = *((long*)&polyX->u3);
    
    //*((long*)&poly->w) = *((long*)&polyX->w);
}

void keepVertexData(void *poly, unsigned long size){
    int i=0;
    for(i = 0 ; i < size ; i++){
        switch( ((*((long*)poly + i)) & 0xFFFF) ){
            case 0x00B1:
            case 0x00B3:
            case 0x00B5:
            case 0x00B7:
            //keep all B entries.
            break;
            default:
                *((long*)poly + i) = 0x00;
            break;
        }
    }
}
