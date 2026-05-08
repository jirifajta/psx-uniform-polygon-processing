# Copyright (C) 2025 Jiri Fajta
# License: MIT License.

# nop; # delay slot, instruction after beq, beq is already executed before actual jump. Note that compiler adds nop automatically.

# supports to read 4 unaligned bytes to return 4 aligned bytes i.e. 1 word
.section .text.unalignword
.global unalignword

unalignword:
	lb   $v0, 0($a0)
    lb   $t0, 1($a0)
    lb   $t1, 2($a0)
    lb   $t2, 3($a0)
    sll  $t0, $t0, 8
    sll  $t1, $t1, 16
    sll  $t2, $t2, 24
    or   $v0, $v0, $t0
    or   $v0, $v0, $t1
    or   $v0, $v0, $t2
	jr   $ra
	#nop
