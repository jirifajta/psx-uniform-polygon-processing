# Copyright (C) 2025 Jiri Fajta
# License: MIT License.

# Note: values to compare are in little endian mode. i.e. value 0x08000000 is in C code visable as 0x0000008 = RRGGBBCC. CC stands for code in a PSX primiteve.
# nop; # delay slot, instruction after beq, beq is already executed before actual jump. Note that compiler adds nop automatically.

.section .text.primitivemapping
.global primitivemapping


primitivemapping:
	lw $t0,0($a1); # load TAG
	lw $t1,4($a1); # load RRGGBBCC
	lw $t2,8($a1); # load x0,y0
	sw $t0,0($a0); # store TAG
	sw $t1,4($a0); # store RRGGBBCC
	sw $t2,8($a0); # store x0,y0
	lui   $t4, 0xE800		# Poly4 with 3rd bit on.
	ori   $t4, $t4, 0x0000
	and   $t3, $t1, $t4
	lui   $t4, 0x2800		# Poly4 with texture
	ori   $t4, $t4, 0x0000
	beq   $t3, $t4, poly4A; # (t1 & 0xE800000) == 0x28000000 jump to label poly4A. Using instructions: [lui, ori, and] (use [lui], [ori] to create value 0x28000000, next use [and] do compare) else move on below for next instruction.
	# nop; # delay slot, instruction after beq, beq is already executed before actual jump. Note that compiler adds nop automatically.
	lui   $t4, 0xE000
	ori   $t4, $t4, 0x0000
	and   $t3, $t1, $t4
	lui   $t4, 0x2000
	ori   $t4, $t4, 0x0000
	beq   $t3, $t4, poly3A;
	#nop;
	lui   $t4, 0x6000
	ori   $t4, $t4, 0x0000
	beq   $t3, $t4, sprtTileA; # jump to label sprtTileA. It is a tile or sprt.
	#nop;
	lui   $t4, 0x4000
	ori   $t4, $t4, 0x0000
	beq   $t3, $t4, line2A;
	#nop;
	j primitivemapping_exit; # unsupported code, exit.
	#nop;
	poly3A: # it is a poly3A. A form of polygon with 3 vertices.
		lui   $t4, 0x0400
		ori   $t4, $t4, 0x0000
		and   $t3, $t1, $t4
		bgtz $t3,tex3A; # check if nontex3A or tex3A.
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
					lw $t0,16($a1);
					lw $t1,20($a1);
					lw $t2,28($a1);
					sw $t0,12($a0);
					sw $t1,16($a0);
					sw $t2,20($a0);
					lw $t0,32($a1);
					sw $t0,24($a0);
					j primitivemapping_exit;
					#nop;
		tex3A:
			lui   $t4, 0x1000
			ori   $t4, $t4, 0x0000
			and   $t3, $t1, $t4
			bgtz $t3,texG3A;
				texF3A:
					lw $t0,12($a1);
					lw $t1,20($a1);
					lw $t2,24($a1);
					sw $t0,12($a0);
					sw $t1,16($a0);
					sw $t2,20($a0);
					lw $t0,32($a1);
					lw $t1,36($a1);
					sw $t0,24($a0);
					sw $t1,28($a0);
					j primitivemapping_exit;
					#nop;
				texG3A:
					lw $t0,12($a1);
					lw $t1,16($a1);
					lw $t2,20($a1);
					sw $t0,12($a0);
					sw $t1,16($a0);
					sw $t2,20($a0);
					lw $t0,24($a1);
					lw $t1,28($a1);
					lw $t2,32($a1);
					sw $t0,24($a0);
					sw $t1,28($a0);
					sw $t2,32($a0);
					lw $t0,36($a1);
					sw $t0,36($a0);
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
					lw $t0,16($a1);
					lw $t1,20($a1);
					lw $t2,28($a1);
					sw $t0,12($a0);
					sw $t1,16($a0);
					sw $t2,20($a0);
					lw $t0,32($a1);
					lw $t1,40($a1);
					lw $t2,44($a1);
					sw $t0,24($a0);
					sw $t1,28($a0);
					sw $t2,32($a0);
					j primitivemapping_exit;
					#nop;
		tex4A:
			lui   $t4, 0x1000
			ori   $t4, $t4, 0x0000
			and   $t3, $t1, $t4
			bgtz $t3,texG4A;
				texF4A:
					lw $t0,12($a1);
					lw $t1,20($a1);
					lw $t2,24($a1);
					sw $t0,12($a0);
					sw $t1,16($a0);
					sw $t2,20($a0);
					lw $t0,32($a1);
					lw $t1,36($a1);
					lw $t2,44($a1);
					sw $t0,24($a0);
					sw $t1,28($a0);
					sw $t2,32($a0);
					lw $t0,48($a1);
					sw $t0,36($a0);
					j primitivemapping_exit;
					#nop;
				texG4A:
					lw $t0,12($a1);
					lw $t1,16($a1);
					lw $t2,20($a1);
					sw $t0,12($a0);
					sw $t1,16($a0);
					sw $t2,20($a0);
					lw $t0,24($a1);
					lw $t1,28($a1);
					lw $t2,32($a1);
					sw $t0,24($a0);
					sw $t1,28($a0);
					sw $t2,32($a0);
					lw $t0,36($a1);
					lw $t1,40($a1);
					lw $t2,44($a1);
					sw $t0,36($a0);
					sw $t1,40($a0);
					sw $t2,44($a0);
					lw $t0,48($a1);
					sw $t0,48($a0);
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
				lw $t0,16($a1);
				lw $t1,20($a1);
				sw $t0,12($a0);
				sw $t1,16($a0);
				j primitivemapping_exit;
				#nop;
	sprtTileA:
		lui   $t4, 0x0400
		ori   $t4, $t4, 0x0000
		and   $t3, $t1, $t4
		bgtz $t3,sprtA;
		#nop;
			tileA:
				lui   $t4, 0x1800
				ori   $t4, $t4, 0x0000
				and   $t3, $t1, $t4
				bgtz $t3,primitivemapping_exit;
				#nop;
					tilewhA:
						lw $t0,52($a1);
						sw $t0,12($a0);
						j primitivemapping_exit;
						#nop;
			sprtA:
				lw $t0,12($a1);
				sw $t0,12($a0);
				lui   $t4, 0x1800
				ori   $t4, $t4, 0x0000
				and   $t3, $t1, $t4
				bgtz $t3,primitivemapping_exit;
					sprtwhA:
						lw $t1,52($a1);
						sw $t1,16($a0);
						j primitivemapping_exit;
						#nop;
	primitivemapping_exit:
		jr $ra
		#nop;
