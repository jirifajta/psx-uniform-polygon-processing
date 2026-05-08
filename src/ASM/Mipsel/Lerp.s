#Lerp.s

# Copyright (C) 2026 Jiri Fajta
# License: MIT License.

# lerphalf: R = A + ((B - A) >> 1)
#
# Inputs:
#   $a0 = A
#   $a1 = B
#
# Output:
#   $v0 = result

.section .text.lerpMidpoints
.global lerpMidpoints

lerpMidpoints:
    subu    $t0, $a1, $a0    # t0 = B - A
    sra     $t0, $t0, 1      # t0 = (B - A) >> 1 => t0 = (t0) >> 1
    addu    $v0, $a0, $t0    # v0 = A + ((B - A) >> 1) => v0 = A + (t0)
    jr      $ra
    nop


# lerpFixed12: R = A + (((B - A) * C) >> 12)
#
# NOTE: a + c(b-a) = a + bc - ac = a(1-c) + bc
#
# Inputs:
#   $a0 = A
#   $a1 = B
#   $a2 = C = {0-4096}
#
# Output:
#   $v0 = result
#
# SAFE ONLY if abs(B-A) <= 524287

.section .text.lerpFixed12
.global lerpFixed12

lerpFixed12:
    subu   $t0, $a1, $a0     # delta: B-A
    mul    $t1, $t0, $a2     # 32-bit product, (B-A)*C
    sra    $t1, $t1, 12		 # ((B-A)*C) >> 12
    addu   $v0, $a0, $t1	 # A + ((B-A)*C) >> 12
    jr     $ra
    nop


# Alternative: lerpFixed12
#
# r = (A*C + B*(4096-C)) >> 12
#
# Inputs:
#   $a0 = A
#   $a1 = B
#   $a2 = C = {0-4096}
#
# Output:
#   $v0 = result
#
# Pure 32-bit version.
# Correct only if intermediate multiplies stay in signed 32-bit range.

#lerpFixed12:
#    li      $t0, 4096
#    subu    $t0, $t0, $a2        # t0 = (4096 - C)
#
#    mul     $t1, $a0, $a2        # A * C
#    mul     $t2, $a1, $t0        # B * (4096 - C)
#
#    addu    $t1, $t1, $t2        # A * C + B * (4096 - C)
#
#    sra     $v0, $t1, 12         # A * C + B * (4096 - C)/4096
#
#    jr      $ra
#    nop

