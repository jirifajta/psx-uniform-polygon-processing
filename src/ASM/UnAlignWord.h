//UnAlignWord.h

// Copyright (C) 2025 Jiri Fajta
// License: MIT License.

#ifndef UnAlignWord_H_INCLUDED
#define UnAlignWord_H_INCLUDED


#if defined(c_plusplus) || defined(__cplusplus)
extern "C" {
#endif

/*
 * Function:
 * Supports to read 4 unaligned bytes to return 4 aligned bytes i.e. 1 aligned word.
 *
 * Parameter:
 * Any pointer that points to the first byte of interest.
 * 
 * Return:
 * Returns 4 byte long value that has to be stored as 4 byte aligned value.
 */

extern long unalignword(void *value_ptr);

#if defined(c_plusplus) || defined(__cplusplus)
}
#endif

#endif
