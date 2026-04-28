# Copyright (C) 2025 Jiri Fajta
# License: MIT License.

# nop; # delay slot, instruction after beq, beq is already executed before actual jump. Note that compiler adds nop automatically.

.section .text.swapEndian32
.global swapEndian32

/* void unalignword(void *value_ptr)
 32bit: $a0 = $4 = value_ptr of int, u_int, (PSX long u_long)
 or
 16bit: $a0 = $4 = value_ptr of short, u_short

 other registers if used by function:
 $v0	: used to return value for this function.
 */


swapEndian32:
	lw $t0,0($a0);
	sll $t1,$t0,24;
	andi $t2,$t0,0xFF00;
	sll $t2,$t2,8;

	lui $t4, 0x00FF
	ori $t4, $t4, 0x0000
	and $t3, $t0, $t4
	#andi $t3,$t0,0xFF0000 # not supported 32bit immediate value
	
	srl $t3,$t3,8
	srl $t0,$t0,24
	or $t0,$t0,$t1
	or $t0,$t0,$t2
	or $t0,$t0,$t3
	sw $t0,0($a0)
	jr $ra
	#nop


.section .text.swapEndian32Ret
.global swapEndian32Ret

swapEndian32Ret:
	lw $t0,0($a0)
	sll $t1,$t0,24
	andi $t2,$t0,0xFF00
	sll $t2,$t2,8

	lui $t4, 0x00FF
	ori $t4, $t4, 0x0000
	and $t3, $t0, $t4
	#andi $t3,$t0,0xFF0000

	srl $t3,$t3,8
	srl $v0,$t0,24
	or $v0,$v0,$t1
	or $v0,$v0,$t2
	or $v0,$v0,$t3
	jr $ra
	#nop


.section .text.swapEndian16
.global swapEndian16


swapEndian16:
	lh $t0,0($a0)
	andi $t1,$t0,0xFF
	sll $t1,$t1,8
	andi $t0,$t0,0xFF00
	srl $t0,$t0,8
	or $t0,$t0,$t1
	sh $t0,0($a0)
	jr $ra
	#nop


.section .text.swapEndian16Ret
.global swapEndian16Ret

swapEndian16Ret:
	lh $t0,0($a0)
	andi $t1,$t0,0xFF
	sll $t1,$t1,8
	andi $t0,$t0,0xFF00
	srl $t0,$t0,8
	or $v0,$t0,$t1
	jr $ra
	#nop
