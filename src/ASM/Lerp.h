//Lerp.h

// Copyright (C) 2026 Jiri Fajta
// License: MIT License.

#ifndef Lerp_H_INCLUDED
#define Lerp_H_INCLUDED


#if defined(c_plusplus) || defined(__cplusplus)
extern "C" {
#endif

/**
 * @details Calculates midpoint between two points.
 *
 * @param a pointer to value A.
 * @param b pointer to value B.
 * @return A + (B - A) >> 1 as long format.
 */

extern long lerpMidpoints(long a, long b);

/**
 * @details Calculates linear interpolation between two points. R = A + (((B - A) * C) >> 12).
 *
 * @param a pointer to value A.
 * @param b pointer to value B.
 * @param c pointer to factor value C. C = {0-4096}.
 * @return A + (((B - A) * C) >> 12) as long format. Note: a + c(b-a) = a + bc - ac = a(1-c) + bc
 */

extern long lerpFixed12(long a, long b, long c);
#if defined(c_plusplus) || defined(__cplusplus)
}
#endif

#endif
