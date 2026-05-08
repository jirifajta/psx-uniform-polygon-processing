//SwapEndian.h

// Copyright (C) 2025 Jiri Fajta
// License: MIT License.

#ifndef SwapEndian_H_INCLUDED
#define SwapEndian_H_INCLUDED


#if defined(c_plusplus) || defined(__cplusplus)
extern "C" {
#endif

/**
 * @details Swap bytes such that:
 *  little-endian -> big-endian
 *  big-endian -> little-endian
 * 
 * 32 bit version: int, u_int, (PSX long, u_long)
 *
 * @param value_ptr Any pointer that points to the first byte of interest. Pointer must be stored as 4 bytes aligned. Input will be overwritten.
 */
extern void swapEndian32(void *value_ptr);

/**
 * @details Swap bytes such that:
 *  little-endian -> big-endian
 *  big-endian -> little-endian
 * 
 * 32 bit version: int, u_int, (PSX long, u_long)
 *
 * @param value_ptr Any pointer that points to the first byte of interest. Pointer must be stored as 4 bytes aligned.
 * 
 * @return 4 byte counter endian format.
 */
extern long swapEndian32Ret(void *value_ptr);

/**
 * @details Swap bytes such that:
 *  little-endian -> big-endian
 *  big-endian -> little-endian
 * 
 * 16 bit version: short, u_short
 *
 * @param value_ptr Any pointer that points to the first byte of interest. Pointer must be stored as 2 bytes aligned. Input will be overwritten.
 */
extern void swapEndian16(void *value_ptr);

/**
 * @details Swap bytes such that:
 *  little-endian -> big-endian
 *  big-endian -> little-endian
 * 
 * 16 bit version: short, u_short
 *
 * @param value_ptr Any pointer that points to the first byte of interest. Pointer must be stored as 2 bytes aligned.
 * 
 * @return 2 byte counter endian format.
 */
extern short swapEndian16Ret(void *value_ptr);

#if defined(c_plusplus) || defined(__cplusplus)
}
#endif

#endif
