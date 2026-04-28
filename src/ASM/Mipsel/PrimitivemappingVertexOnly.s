# Copyright (C) 2026 Jiri Fajta
# License: MIT License.

# Note: values to compare are in little endian mode. i.e. value 0x08000000 is in C code visable as 0x0000008 = RRGGBBCC. CC stands for code in a PSX primiteve.

# nop; # delay slot, instruction after beq, beq is already executed before actual jump. Note that compiler adds nop automatically.

.section .text.primitivemappingVertexOnly
.global primitivemappingVertexOnly

primitivemappingVertexOnly:
	lw $t1,4($a1); # load RRGGBBCC
	lw $t2,8($a1); # load x0,y0
	sw $t2,8($a0); # store x0,y0
	lui   $t4, 0x6000
	ori   $t4, $t4, 0x0000
	and   $t3, $t1, $t4
	beq $t3, $t4, primitivemapping_exit; # jump to label primitivemapping_exit. It is a tile or sprt.
	#nop;
	lui   $t4, 0x4000
	ori   $t4, $t4, 0x0000
	and   $t3, $t1, $t4
	bgtz $t3,line2A;
	#nop;
	lui   $t4, 0x0800
	ori   $t4, $t4, 0x0000
	and   $t3, $t1, $t4
	bgtz $t3,poly4A; # jump to label poly4A if value is 0x08000000 using instructions: [lui, ori, and] (use [lui], [ori] to create value 0x08000000, next use [and] do compare) else move on below for next instruction.
	#nop; # delay slot, instruction is already executed before actual jump by bgtz.
	poly3A: # it is a poly3A. A form of polygon with 3 vertices.
		lui   $t4, 0x0400
		ori   $t4, $t4, 0x0000
		and   $t3, $t1, $t4
		bgtz $t3,tex3A; # check if nontex3A or tex3A. nontex means primitive with no texture.
		#nop;
		nontex3A:
			lui   $t4, 0x1000
			ori   $t4, $t4, 0x0000
			and   $t3, $t1, $t4
			bgtz $t3,nontexG3A; # check if nontexF3A or nontexG3A
				nontexF3A:
					lw $t0,20($a1); # copy 4 bytes from position 20 (x1,y1) in POLY_X into register t0.
					lw $t1,32($a1);
					sw $t0,12($a0); # store 4 bytes from register t0 and store into struct at position 12.
					sw $t1,16($a0);
					j primitivemapping_exit; # done, exit.
					#nop;
				nontexG3A:
					lw $t0,20($a1);
					lw $t1,32($a1);
					sw $t0,16($a0);
					sw $t1,24($a0);
					j primitivemapping_exit;
					#nop;
		tex3A:
			lui   $t4, 0x1000
			ori   $t4, $t4, 0x0000
			and   $t3, $t1, $t4
			bgtz $t3,texG3A;
				texF3A:
					lw $t0,20($a1);
					lw $t1,32($a1);
					sw $t0,16($a0);
					sw $t1,24($a0);
					j primitivemapping_exit;
					#nop;
				texG3A:
					lw $t0,20($a1);
					lw $t1,32($a1);
					sw $t0,20($a0);
					sw $t1,32($a0);
					j primitivemapping_exit;
					#nop;
	poly4A:
		lui   $t4, 0x0400
		ori   $t4, $t4, 0x0000
		and   $t3, $t1, $t4
		bgtz $t3,tex4A;
		#nop;
		nontex4A:
			lui   $t4, 0x1000
			ori   $t4, $t4, 0x0000
			and   $t3, $t1, $t4
			bgtz $t3,nontexG4A;
				nontexF4A:
					lw $t0,20($a1);
					lw $t1,32($a1);
					lw $t2,44($a1);
					sw $t0,12($a0);
					sw $t1,16($a0);
					sw $t2,20($a0);
					j primitivemapping_exit;
					#nop;
				nontexG4A:
					lw $t0,20($a1);
					lw $t1,32($a1);
					lw $t2,44($a1);
					sw $t0,16($a0);
					sw $t1,24($a0);
					sw $t2,32($a0);
					j primitivemapping_exit;
					#nop;
		tex4A:
			lui   $t4, 0x1000
			ori   $t4, $t4, 0x0000
			and   $t3, $t1, $t4
			bgtz $t3,texG4A;
				texF4A:
					lw $t0,20($a1);
					lw $t1,32($a1);
					lw $t2,44($a1);
					sw $t0,16($a0);
					sw $t1,24($a0);
					sw $t2,32($a0);
					j primitivemapping_exit;
					#nop;
				texG4A:
					lw $t0,20($a1);
					lw $t1,32($a1);
					lw $t2,44($a1);
					sw $t0,20($a0);
					sw $t1,32($a0);
					sw $t2,44($a0);
					j primitivemapping_exit;
					#nop;
	line2A:
		lui   $t4, 0x0800
		ori   $t4, $t4, 0x0000
		and   $t3, $t1, $t4
		bgtz $t3,primitivemapping_exit;
		#nop;
		lui   $t4, 0x1000
		ori   $t4, $t4, 0x0000
		and   $t3, $t1, $t4
		bgtz $t3,lineG2A;
			lineF2A:
				lw $t0,20($a1);
				sw $t0,12($a0);
				j primitivemapping_exit;
				#nop;
			lineG2A:
				lw $t1,20($a1);
				sw $t1,16($a0);
				j primitivemapping_exit;
				#nop;
	primitivemapping_exit:
		jr $ra
		#nop;
