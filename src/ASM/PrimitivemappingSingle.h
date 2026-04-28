//PrimitivemappingSingle.h

// Copyright (C) 2026 Jiri Fajta
// License: MIT License.

#ifndef PrimitivemappingSingle_H_INCLUDED
#define PrimitivemappingSingle_H_INCLUDED

#include "../Structs/PolyX.h"

#if defined(c_plusplus) || defined(__cplusplus)
extern "C" {
#endif

/*
 * Function:
 * Copy specific attributes only from [POLY_X] struct -> supported primitive data structs.
 * Definition of struct [POLY_X] is found in PolyX.c.
 * 
 * Supported primitive data structs:
 *  POLY_XXX [F3, F4, FT3, FT4, G3, G4, GT3, GT4]
 *  LINE_XX [F2, G2]
 *  SPRT, SPRT_8, SPRT_16, TILE, TILE_1, TILE_8, TILE_16
 * 
 * Setting primitive type and [abr] (known as texshade), [semi-trans] and so on in polyX is fine. primitivemapping(...) function can handle this as well.
 * 
 * Parameter:
 * ptr_primitive_out    : Overrides given primitive to output pointer with updated attributes values as selected values function type.
 * ptr_primitive_in     : POLY_X pointer with relevent attributes set. 
 *
 * Return:
 * None.
 */

/*VertexOnly*/
extern void primitivemappingVertexOnlyTileSptrX(void *ptr_primitive_out, POLY_X *ptr_primitive_in);
extern void primitivemappingVertexOnlyPolyF2(void *ptr_primitive_out, POLY_X *ptr_primitive_in);
extern void primitivemappingVertexOnlyPolyG2(void *ptr_primitive_out, POLY_X *ptr_primitive_in);

extern void primitivemappingVertexOnlyPolyF3(void *ptr_primitive_out, POLY_X *ptr_primitive_in);
extern void primitivemappingVertexOnlyPolyFT3(void *ptr_primitive_out, POLY_X *ptr_primitive_in);
extern void primitivemappingVertexOnlyPolyG3(void *ptr_primitive_out, POLY_X *ptr_primitive_in);
extern void primitivemappingVertexOnlyPolyGT3(void *ptr_primitive_out, POLY_X *ptr_primitive_in);

extern void primitivemappingVertexOnlyPolyF4(void *ptr_primitive_out, POLY_X *ptr_primitive_in);
extern void primitivemappingVertexOnlyPolyFT4(void *ptr_primitive_out, POLY_X *ptr_primitive_in);
extern void primitivemappingVertexOnlyPolyG4(void *ptr_primitive_out, POLY_X *ptr_primitive_in);
extern void primitivemappingVertexOnlyPolyGT4(void *ptr_primitive_out, POLY_X *ptr_primitive_in);

/*ColorOnly*/
extern void primitivemappingColorOnlyPolyFXXLineFXTileSptrX(void *ptr_primitive_out, POLY_X *ptr_primitive_in);
extern void primitivemappingColorOnlyLineG2(void *ptr_primitive_out, POLY_X *ptr_primitive_in);
extern void primitivemappingColorOnlyPolyG3(void *ptr_primitive_out, POLY_X *ptr_primitive_in);
extern void primitivemappingColorOnlyPolyGT3(void *ptr_primitive_out, POLY_X *ptr_primitive_in);
extern void primitivemappingColorOnlyPolyG4(void *ptr_primitive_out, POLY_X *ptr_primitive_in);
extern void primitivemappingColorOnlyPolyGT4(void *ptr_primitive_out, POLY_X *ptr_primitive_in);

/*UvOnly*/
extern void primitivemappingUvOnlyTileSptrX(void *ptr_primitive_out, POLY_X *ptr_primitive_in);
extern void primitivemappingUvOnlyPolyFT3(void *ptr_primitive_out, POLY_X *ptr_primitive_in);
extern void primitivemappingUvOnlyPolyFT4(void *ptr_primitive_out, POLY_X *ptr_primitive_in);
extern void primitivemappingUvOnlyPolyGT3(void *ptr_primitive_out, POLY_X *ptr_primitive_in);
extern void primitivemappingUvOnlyPolyGT4(void *ptr_primitive_out, POLY_X *ptr_primitive_in);


#if defined(c_plusplus) || defined(__cplusplus)
}
#endif

#endif
