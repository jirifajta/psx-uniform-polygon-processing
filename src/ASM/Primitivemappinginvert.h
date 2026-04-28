//Primitivemappinginvert.h

// Copyright (C) 2025 Jiri Fajta
// License: MIT License.

#ifndef Primitivemappinginvert_H_INCLUDED
#define Primitivemappinginvert_H_INCLUDED

#include "../Structs/PolyX.h"

#if defined(c_plusplus) || defined(__cplusplus)
extern "C" {
#endif

/*
 * Function:
 * Reverts proces of the function in [Primitivemapping.H], primitivemapping(...). values in primitive data struct like POLY_GT4 will e converted to POLY_X.
 *
 * Requirements:
 * See [Primitivemapping.H] for Requirements and supported primitive data structs. Note that [code] and [tag] needs to be set to the primitive struct like setPolyGT3(polyGt3).
 * So, setPolyGT3(polyX) is not needed.
 * 
 * ptr_primitive_out    : Returns POLY_X pointer. 
 *                          Struct type that is returned is based on value set in polyX.code used as input.
 * ptr_primitive_in     : Pointer to supported primitive data struct like POLY_GT4 where [code], [tag] data (i.e. poly.code and poly.tag) is set. See Requirements above. 
 */

extern void primitivemappinginvert(void *ptr_primitive_out, POLY_X *ptr_primitive_in);

#if defined(c_plusplus) || defined(__cplusplus)
}
#endif

#endif
