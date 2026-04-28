# Copyright (C) 2025 Jiri Fajta
# License: MIT License.

# Note: values to compare are in little endian mode. i.e. value 0x08000000 is in C code visable as 0x0000008 = RRGGBBCC. CC stands for code in a PSX primiteve.

# nop; # delay slot, instruction after beq, beq is already executed before actual jump. Note that compiler adds nop automatically.

.section .text.primitivemappinginvert
.global primitivemappinginvert

primitivemappinginvert:
	lw $t0,0($a1);
	lw $t1,4($a1);
	lw $t2,8($a1);
	sw $t0,0($a0);
	sw $t1,4($a0);
	sw $t2,8($a0);
	lui   $t4, 0x6000
	ori   $t4, $t4, 0x0000
	and   $t3, $t1, $t4
	beq $t3, $t4, sprtTileB;
	#nop;
	lui   $t4, 0x4000
	ori   $t4, $t4, 0x0000
	and   $t3, $t1, $t4
	bgtz $t3,line2B;
	#nop;
	lui   $t4, 0x0800
	ori   $t4, $t4, 0x0000
	and   $t3, $t1, $t4
	bgtz $t3,poly4B;
	#nop;
	poly3B:
		lui   $t4, 0x0400
		ori   $t4, $t4, 0x0000
		and   $t3, $t1, $t4
		bgtz $t3,tex3B;
		#nop;
		nontex3B:
			lui   $t4, 0x1000
			ori   $t4, $t4, 0x0000
			and   $t3, $t1, $t4
			bgtz $t3,nontexG3B;
				nontexF3B:
					lw $t0,12($a1);
					lw $t1,16($a1);
					sw $t0,20($a0);
					sw $t1,32($a0);
					j primitivemappinginvert_exit;
					#nop;
				nontexG3B:
					lw $t0,12($a1);
					lw $t1,16($a1);
					lw $t2,20($a1);
					sw $t0,16($a0);
					sw $t1,20($a0);
					sw $t2,28($a0);
					lw $t0,24($a1);
					sw $t0,32($a0);
					j primitivemappinginvert_exit;
					#nop;
		tex3B:
			lui   $t4, 0x1000
			ori   $t4, $t4, 0x0000
			and   $t3, $t1, $t4
			bgtz $t3,texG3B;
				texF3B:
					lw $t0,12($a1);
					lw $t1,16($a1);
					lw $t2,20($a1);
					sw $t0,12($a0);
					sw $t1,20($a0);
					sw $t2,24($a0);
					lw $t0,24($a1);
					lw $t1,28($a1);
					sw $t0,32($a0);
					sw $t1,36($a0);
					j primitivemappinginvert_exit;
					#nop;
				texG3B:
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
					j primitivemappinginvert_exit;
					#nop;
	poly4B:
		lui   $t4, 0x0400
		ori   $t4, $t4, 0x0000
		and   $t3, $t1, $t4
		bgtz $t3,tex4B;
		#nop;
		nontex4B:
			lui   $t4, 0x1000
			ori   $t4, $t4, 0x0000
			and   $t3, $t1, $t4
			bgtz $t3,nontexG4B;
				nontexF4B:
					lw $t0,12($a1);
					lw $t1,16($a1);
					lw $t2,20($a1);
					sw $t0,20($a0);
					sw $t1,32($a0);
					sw $t2,44($a0);
					j primitivemappinginvert_exit;
					#nop;
				nontexG4B:
					lw $t0,12($a1);
					lw $t1,16($a1);
					lw $t2,20($a1);
					sw $t0,16($a0);
					sw $t1,20($a0);
					sw $t2,28($a0);
					lw $t0,24($a1);
					lw $t1,28($a1);
					lw $t2,32($a1);
					sw $t0,32($a0);
					sw $t1,40($a0);
					sw $t2,44($a0);
					j primitivemappinginvert_exit;
					#nop;
		tex4B:
			lui   $t4, 0x1000
			ori   $t4, $t4, 0x0000
			and   $t3, $t1, $t4
			bgtz $t3,texG4B;
				texF4B:
					lw $t0,12($a1);
					lw $t1,16($a1);
					lw $t2,20($a1);
					sw $t0,12($a0);
					sw $t1,20($a0);
					sw $t2,24($a0);
					lw $t0,24($a1);
					lw $t1,28($a1);
					lw $t2,32($a1);
					sw $t0,32($a0);
					sw $t1,36($a0);
					sw $t2,44($a0);
					lw $t0,36($a1);
					sw $t0,48($a0);
					j primitivemappinginvert_exit;
					#nop;
				texG4B:
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
					j primitivemappinginvert_exit;
					#nop;
	line2B:
		lui   $t4, 0x0800
		ori   $t4, $t4, 0x0000
		and   $t3, $t1, $t4
		bgtz $t3,primitivemappinginvert_exit;
		#nop;
		lui   $t4, 0x1000
		ori   $t4, $t4, 0x0000
		and   $t3, $t1, $t4
		bgtz $t3,lineG2B;
		#nop;
			lineF2B:
				lw $t0,12($a1);
				sw $t0,20($a0);
				j primitivemappinginvert_exit;
				#nop;
			lineG2B:
				lw $t0,12($a1);
				lw $t1,16($a1);
				sw $t0,16($a0);
				sw $t1,20($a0);
				j primitivemappinginvert_exit;
				#nop;
	sprtTileB:
		lui   $t4, 0x0400
		ori   $t4, $t4, 0x0000
		and   $t3, $t1, $t4
		bgtz $t3,sprtB;
		#nop;
			tileB:
				lui   $t4, 0x1800
				ori   $t4, $t4, 0x0000
				and   $t3, $t1, $t4
				bgtz $t3,primitivemappinginvert_exit;
				#nop;
					tilewhB:
						lw $t0,12($a1);
						sw $t0,52($a0);
						j primitivemappinginvert_exit;
						#nop;
			sprtB:
				lw $t0,12($a1);
				sw $t0,12($a0);
				lui   $t4, 0x1800
				ori   $t4, $t4, 0x0000
				and   $t3, $t1, $t4
				bgtz $t3,primitivemappinginvert_exit;
					sprtwhB:
						lw $t0,12($a1);
						lw $t1,16($a1);
						sw $t0,12($a0);
						sw $t1,52($a0);
						j primitivemappinginvert_exit;
						#nop;
	primitivemappinginvert_exit:
		jr $ra
		#nop;

