# Copyright (C) 2026 Jiri Fajta
# License: MIT License.

# nop; # delay slot, instruction after beq, beq is already executed before actual jump. Note that compiler adds nop automatically.

###### VERTEX ######

### VertexOnly TileSptrX_FX & GX
.section .text.primitivemappingVertexOnlyTileSptrX
.global primitivemappingVertexOnlyTileSptrX

primitivemappingVertexOnlyTileSptrX:
	lw $t0,8($a1); # load x0,y0
	sw $t0,8($a0); # load x1,y1
	jr $ra;
	#nop;


### VertexOnly LINE_FX & GX
.section .text.primitivemappingVertexOnlyPolyF2
.global primitivemappingVertexOnlyPolyF2

primitivemappingVertexOnlyPolyF2:
	lw $t0,8($a1);  # load x0,y0
	lw $t1,20($a1); # load x1,y1
	sw $t0,8($a0); 	# store x0,y0
	sw $t1,12($a0); # store x1,y1
	jr $ra;
	#nop;

.section .text.primitivemappingVertexOnlyPolyG2
.global primitivemappingVertexOnlyPolyG2

primitivemappingVertexOnlyPolyG2:
	lw $t0,8($a1);  # load x0,y0
	lw $t1,20($a1); # load x1,y1
	sw $t0,8($a0); 	# store x0,y0
	sw $t1,16($a0); # store x1,y1
	jr $ra;
	#nop;


### VertexOnly POLY_FX & GX

.section .text.primitivemappingVertexOnlyPolyF3
.global primitivemappingVertexOnlyPolyF3

primitivemappingVertexOnlyPolyF3:
	lw $t0,8($a1);  # load x0,y0
	lw $t1,20($a1); # load x1,y1
	lw $t2,32($a1); # load x2,y2
	sw $t0,8($a0); 	# store x0,y0
	sw $t1,12($a0); # store x1,y1
	sw $t2,16($a0); # store x2,y2
	jr $ra;
	#nop;


.section .text.primitivemappingVertexOnlyPolyF4
.global primitivemappingVertexOnlyPolyF4

primitivemappingVertexOnlyPolyF4:
	lw $t0,8($a1);  # load x0,y0
	lw $t1,20($a1); # load x1,y1
	lw $t2,32($a1); # load x2,y2
	lw $t3,44($a1);
	sw $t0,8($a0);  # store x0,y0
	sw $t1,12($a0);
	sw $t2,16($a0);
	sw $t3,20($a0);
	jr $ra;
	#nop;


### VertexOnly POLY_G3 WORKS

.section .text.primitivemappingVertexOnlyPolyG3
.global primitivemappingVertexOnlyPolyG3

primitivemappingVertexOnlyPolyG3:
	lw $t0,8($a1);  # load x0,y0
	lw $t1,20($a1); # load x1,y1
	lw $t2,32($a1); # load x2,y2
	sw $t0,8($a0);  # store x0,y0
	sw $t1,16($a0);
	sw $t2,24($a0);
	jr $ra;
	#nop;

.section .text.primitivemappingVertexOnlyPolyG4
.global primitivemappingVertexOnlyPolyG4

primitivemappingVertexOnlyPolyG4:
	lw $t0,8($a1);  # load x0,y0
	lw $t1,20($a1);
	lw $t2,32($a1);
	lw $t3,44($a1);
	sw $t0,8($a0);  # store x0,y0
	sw $t1,16($a0);
	sw $t2,24($a0);
	sw $t3,32($a0);
	jr $ra;
	#nop;


### VertexOnly POLY_FTX & GTX

.section .text.primitivemappingVertexOnlyPolyFT3
.global primitivemappingVertexOnlyPolyFT3

primitivemappingVertexOnlyPolyFT3:
	lw $t0,8($a1);  # load x0,y0
	lw $t1,20($a1); # load x1,y1
	lw $t2,32($a1); # load x2,y2
	sw $t0,8($a0);  # store x0,y0
	sw $t1,16($a0);
	sw $t2,24($a0);			
	jr $ra;
	#nop;

.section .text.primitivemappingVertexOnlyPolyFT4
.global primitivemappingVertexOnlyPolyFT4

primitivemappingVertexOnlyPolyFT4:
	lw $t0,8($a1);  # load x0,y0
	lw $t1,20($a1); # load x1,y1
	lw $t2,32($a1); # load x2,y2
	lw $t3,44($a1);
	sw $t0,8($a0);  # store x0,y0
	sw $t1,16($a0);
	sw $t2,24($a0);
	sw $t3,32($a0);
	jr $ra;
	#nop;


.section .text.primitivemappingVertexOnlyPolyGT3
.global primitivemappingVertexOnlyPolyGT3

primitivemappingVertexOnlyPolyGT3:
	lw $t0,8($a1);  # load x0,y0
	lw $t1,20($a1); # load x1,y1
	lw $t2,32($a1); # load x2,y2
	sw $t0,8($a0);  # store x0,y0
	sw $t1,20($a0);
	sw $t2,32($a0);			
	jr $ra;
	#nop;


.section .text.primitivemappingVertexOnlyPolyGT4
.global primitivemappingVertexOnlyPolyGT4

primitivemappingVertexOnlyPolyGT4:
	lw $t0,8($a1);  # load x0,y0
	lw $t1,20($a1); # load x1,y1
	lw $t2,32($a1); # load x2,y2
	lw $t3,44($a1);
	sw $t0,8($a0);  # store x0,y0
	sw $t1,20($a0);
	sw $t2,32($a0);
	sw $t3,44($a0);
	jr $ra;
	#nop;
	

###### COLOR ######

### ColorOnly POLY_FXX, LineFX, TileSptrX_FX & GX
.section .text.primitivemappingColorOnlyPolyFXXLineFXTileSptrX
.global primitivemappingColorOnlyPolyFXXLineFXTileSptrX

primitivemappingColorOnlyPolyFXXLineFXTileSptrX:
	lw $t0,4($a1);  # load RRGGBBCC
	sw $t0,4($a0); # load RRGGBBCC
	jr $ra;
	#nop;


### ColorOnly LINEG2
.section .text.primitivemappingColorOnlyLineG2
.global primitivemappingColorOnlyLineG2

primitivemappingColorOnlyLineG2:
	lw $t0,4($a1);
	lw $t1,16($a1);
	sw $t0,4($a0);
	sw $t1,12($a0);
	jr $ra;
	#nop;

		
### ColorOnly POLYG3
.section .text.primitivemappingColorOnlyPolyG3
.global primitivemappingColorOnlyPolyG3

primitivemappingColorOnlyPolyG3:
	lw $t0,4($a1);
	lw $t1,16($a1);
	lw $t2,28($a1);
	sw $t0,4($a0);
	sw $t1,12($a0);
	sw $t2,20($a0);		
	jr $ra;
	#nop;


### ColorOnly POLYGT3
.section .text.primitivemappingColorOnlyPolyGT3
.global primitivemappingColorOnlyPolyGT3

primitivemappingColorOnlyPolyGT3:
	lw $t0,4($a1);
	lw $t1,16($a1);
	lw $t2,28($a1);
	sw $t0,4($a0);
	sw $t1,16($a0);
	sw $t2,28($a0);
	jr $ra;
	#nop;


### ColorOnly POLYG4
.section .text.primitivemappingColorOnlyPolyG4
.global primitivemappingColorOnlyPolyG4

primitivemappingColorOnlyPolyG4:
	lw $t0,4($a1);
	lw $t1,16($a1);
	lw $t2,28($a1);
	lw $t3,40($a1);
	sw $t0,4($a0);
	sw $t1,12($a0);
	sw $t2,20($a0);		
	sw $t3,28($a0);
	jr $ra;
	#nop;


### ColorOnly POLYGT4
.section .text.primitivemappingColorOnlyPolyGT4
.global primitivemappingColorOnlyPolyGT4

primitivemappingColorOnlyPolyGT4:
	lw $t0,4($a1);
	lw $t1,16($a1);
	lw $t2,28($a1);
	lw $t3,40($a1);
	sw $t0,4($a0);
	sw $t1,16($a0);				
	sw $t2,28($a0);
	sw $t3,40($a0);
	jr $ra;
	#nop;
	
###### UV ######

### UvOnly TileSptrX_FX & GX

### SptrX_FX & GX
.section .text.primitivemappingUvOnlyTileSptrX
.global primitivemappingUvOnlyTileSptrX

primitivemappingUvOnlyTileSptrX:
	lh $t0,12($a1);
	sh $t0,12($a0);
	jr $ra;
	#nop;


### UvOnly POLYFT3
.section .text.primitivemappingUvOnlyPolyFT3
.global primitivemappingUvOnlyPolyFT3

primitivemappingUvOnlyPolyFT3:
	lh $t0,12($a1);
	lh $t1,24($a1);
	lh $t2,36($a1);
	sh $t0,12($a0);
	sh $t1,20($a0);
	sh $t2,28($a0);
	jr $ra;
	#nop;


### UvOnly POLYFT4
.section .text.primitivemappingUvOnlyPolyFT4
.global primitivemappingUvOnlyPolyFT4

primitivemappingUvOnlyPolyFT4:
	lh $t0,12($a1);
	lh $t1,24($a1);
	lh $t2,36($a1);
	lh $t3,48($a1);
	sh $t0,12($a0);
	sh $t1,20($a0);
	sh $t2,28($a0);
	sh $t3,36($a0);
	jr $ra;
	#nop;


### UvOnly POLYGT3
.section .text.primitivemappingUvOnlyPolyGT3
.global primitivemappingUvOnlyPolyGT3

primitivemappingUvOnlyPolyGT3:
	lh $t0,12($a1);
	lh $t1,24($a1);
	lh $t2,36($a1);
	sh $t0,12($a0);
	sh $t1,24($a0);
	sh $t2,36($a0);
	jr $ra;
	#nop;


### UvOnly POLYGT4
.section .text.primitivemappingUvOnlyPolyGT4
.global primitivemappingUvOnlyPolyGT4

primitivemappingUvOnlyPolyGT4:
	lh $t0,12($a1);
	lh $t1,24($a1);
	lh $t2,36($a1);
	lh $t3,48($a1);
	sh $t0,12($a0);
	sh $t1,24($a0);
	sh $t2,36($a0);
	sh $t3,48($a0);
	jr $ra;
	#nop;
