//Memcpyc.h

// Copyright (C) 2025 Jiri Fajta
// License: MIT License.

#ifndef Memcpyc_H_INCLUDED
#define Memcpyc_H_INCLUDED


#if defined(c_plusplus) || defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief memcpyc() copies up to 16 bytes in sequence which is faster then default memcpy() function, memcpy() copies byte for byte.
 *
 * @param src_ptr: Any pointer that points to the first byte of interest to copy from.
 * @param dst_ptr: Any pointer that points to the first byte of interest to copy to.
 * @param len_bytes: Number of bytes to copy.
 * 
 * NOTE: Pointer must be stored as 4 bytes aligned if len_bytes is more then 3. Otherwise use memcpy().
 */
extern void memcpyc(void *dst_ptr, void *src_ptr, unsigned long len_bytes);

/**
 * @brief memcpyw() copies per 4 bytes in sequence which is faster then default memcpy() function, memcpy() copies byte for byte.
 *
 * @param src_ptr: Any pointer that points to the first byte of interest to copy from.
 * @param dst_ptr: Any pointer that points to the first byte of interest to copy to.
 * @param number_of_words: Number of words to copy. I.e. 4*number_of_words bytes to copy.
 * 
 * NOTE: Pointer must be stored as 4 bytes aligned.
 */
extern void memcpyw(void *dst_ptr, void *src_ptr, unsigned long number_of_words);

#if defined(c_plusplus) || defined(__cplusplus)
}
#endif

#endif
