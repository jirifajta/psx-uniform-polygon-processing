//PrimitivemappingVertexOnly.h

#ifndef PrimitivemappingVertexOnly_H_INCLUDED
#define PrimitivemappingVertexOnly_H_INCLUDED

#include "../Structs/PolyX.h"

#if defined(c_plusplus) || defined(__cplusplus)
extern "C" {
#endif

/*
 * Function:
 * Copy vertex only from [POLY_X] struct -> supported primitive data structs.
 * Definition of struct [POLY_X] is found in PolyX.c.
 * 
 * Supported primitive data structs:
 *  LINE_XX [F2, G2]
 *  POLY_XXX [F3, F4, FT3, FT4, G3, G4, GT3, GT4]
 *  SPRT, SPRT_8, SPRT_16, TILE, TILE_1, TILE_8, TILE_16
 *
 * Requirements:
 * Any type of primitive must be initialized (i.e. polyX.code and polyX.tag) in polyX variable. Set these manually or use functions like setPolyGT3(polyX). Tip: Use macro functions to ignore struct data type.
 * With this, variable polyX.code is needed to be recognize what type of primitive to process.
 * When polyX.code is not set or unsupported primitive code is used, then the output struct is unpredictable.
 * 
 * Setting primitive type and [abr] (known as texshade), [semi-trans] and so on in polyX is fine. primitivemapping(...) function can handle this as well.
 * 
 * Parameter:
 * ptr_primitive_out    : Overrides given primitive as output pointer with updated x's and y's values  that can be processed by PSX GPU like POLY_GT3.
 *                          Struct type that is returned is based on value set in polyX.code used as input.
 * ptr_primitive_in     : POLY_X pointer where [code], [tag] data (i.e. poly.code and poly.tag) is set. See Requirements above. 
 *
 * Return:
 * None.
 */

extern void primitivemappingVertexOnly(void *ptr_primitive_out, POLY_X *ptr_primitive_in);

#if defined(c_plusplus) || defined(__cplusplus)
}
#endif

#endif
