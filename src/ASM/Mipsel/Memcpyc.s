# Copyright (C) 2025 Jiri Fajta
# License: MIT License.

# nop; # delay slot, instruction after beq, beq is already executed before actual jump. Note that compiler adds nop automatically.

.section .text.memcpyc
.global memcpyc

/* void memcpyc(dst_ptr, src_ptr, len_bytes)
 $a0 = $4 = dst_ptr
 $a1 = $5 = src_ptr
 $a2 = $6 = len_bytes

 other registers:
 $t0 - $t3	: used to load and store words
 $t4		: used temporarily to subtract a value from $a2 for branching 
 */

memcpyc:
	beq $a2,0,memcpyexit
	move $t4,$a2
	addiu $t4,$a2,-15
	bgtz $t4,memcpyloop4w # copy per 16 bytes
	#nop
	addiu $t4,$a2,-3
	bgtz $t4,memcpyloop1w # copy per 4 bytes
	#nop
	move $t4,$a2
	bgtz $t4,memcpyloop1b # copy per byte
	#nop
	memcpyloop4w:
		lw $t0,0($a1)
		lw $t1,4($a1)
		lw $t2,8($a1)
		lw $t3,12($a1)
		sw $t0,0($a0)
		sw $t1,4($a0)
		sw $t2,8($a0)
		sw $t3,12($a0)
		addiu $a1,$a1,16
		addiu $a0,$a0,16
		addiu $a2,$a2,-16
		beq $a2,0,memcpyexit
		addiu $t4,$a2,-15
		bgtz $t4,memcpyloop4w
		#nop
		addiu $t4,$a2,-3
		bgtz $t4,memcpyloop1w
		#nop
		move $t4,$a2
		bgtz $t4,memcpyloop1b
		#nop
	memcpyloop1w:
		lw $t0,0($a1)
		sw $t0,0($a0)
		addiu $a1,$a1,4
		addiu $a0,$a0,4
		addiu $a2,$a2,-4
		beq $a2,0,memcpyexit
		addiu $t4,$a2,-3
		bgtz $t4,memcpyloop1w
		#nop
	memcpyloop1b:
		lbu $t0,0($a1)
		sb $t0,0($a0)
		addiu $a1,$a1,1
		addiu $a0,$a0,1
		addiu $a2,$a2,-1
		bgtz $a2,memcpyloop1b
		#nop
	memcpyexit:
		jr $ra
		#nop


.section .text.memcpyw
.global memcpyw

/* void memcpyw(dst_ptr, src_ptr, len_bytes)
 $a0 = $4 = dst_ptr
 $a1 = $5 = src_ptr
 $a2 = $6 = len_words i.e. multiple of 4 bytes. len_words = 2 words is 8bytes
 */

memcpyw:
	lw $t0,0($a1)
	addiu $a1,$a1,4
	addiu $a2,$a2,-1
	sw $t0,0($a0)
	addiu $a0,$a0,4
	bgtz $a2,memcpyw
	#nop
	jr $ra
	#nop
