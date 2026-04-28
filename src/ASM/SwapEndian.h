//SwapEndian.h

// Copyright (C) 2025 Jiri Fajta
// License: MIT License.

#ifndef SwapEndian_H_INCLUDED
#define SwapEndian_H_INCLUDED


#if defined(c_plusplus) || defined(__cplusplus)
extern "C" {
#endif

/*
 * Function:
 * Swap bytes such that:
 *  little-endian -> big-endian
 *  big-endian -> little-endian
 * 
 * 32 bit version: int, u_int, (PSX long, u_long)
 * 16 bit version: short, u_short
 * 
 * Parameter:
 * Any pointer that points to the first byte of interest. Use 16bit or 32bit function. Pointer must be stored as 2 or 4 bytes aligned respectively.
 * 
 * Return (if Ret contained in function name):
 * Returns 2 or 4 byte endian opposite endian format. Based on [swapEndian16(...)] [swapEndian32(...)].
 */

extern void swapEndian32(void *value_ptr);
extern long swapEndian32Ret(void *value_ptr);
extern void swapEndian16(void *value_ptr);
extern short swapEndian16Ret(void *value_ptr);

#if defined(c_plusplus) || defined(__cplusplus)
}
#endif

#endif
