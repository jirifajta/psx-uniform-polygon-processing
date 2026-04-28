//main.c

// Copyright (C) 2026 Jiri Fajta
// License: MIT License.

/* PSn00bSDK */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <psxetc.h>
#include <psxgte.h>
#include <psxgpu.h>

/* PSYQ */
// #include <stdio.h>
// #include <sys/types.h>
// #include <stdlib.h>
// #include <memory.h>
// #include <libetc.h>
// #include <libgte.h>
// #include <libgpu.h>


#include "Structs/PolyX.h"
#include "PrimitivemappingTests.h"
#include "ASM/Primitivemapping.h"
#include "ASM/PrimitivemappingVertexOnly.h"
#include "ASM/PrimitivemappingSingle.h"
#include "ASM/UnAlignWord.h"
#include "ASM/Memcpyc.h"
#include "ASM/SwapEndian.h"

//PROTO
int main(void);

/// Macros //////////////////////////////////////////////////////////
#define ARRAY_SIZE(array_ptr) \
    (sizeof(array_ptr) / sizeof(*(array_ptr)))

/// Compare bytes functions //////////////////////////////////////////////////////////

// return -1: Array is same. return >-1: is at position of word i where first missmatch was encountered. i.e. at 4*i <-> 4*i+3 bytes.
// all checks after i-th word where skipped.
long check_same_words(unsigned long *ptr0, unsigned long *ptr1, long num_words){
    long i;
    for(i=0; i<num_words; i++){
        if(*(ptr0 + i) != *(ptr1 + i)){
            return i;
        }
    }
    return -1;
}

long check_same_words_print(unsigned long *ptr0, unsigned long *ptr1, long num_words){
    long i, count_missmatch = 0;
    for(i=0; i<num_words; i++){
        if(*(ptr0 + i) == *(ptr1 + i)){
            printf("check[%d]: %08X %08X\r\n", i, *(ptr0 + i) , *(ptr1 + i)  );
        }else{
            printf("check[%d]: %08X %08X X\r\n", i, *(ptr0 + i) , *(ptr1 + i)  );
            count_missmatch++;
        }
    }
    return count_missmatch;
}

void printPoltCompairStats(unsigned long *packet, unsigned long *poly, long len_words, long ret, char *str){
    printf("Titel: %s\r\n", str);
    printf("packet  addr: %08X\r\n", packet);
    printf("poly addr: %08X\r\n", poly);
    printf("number of words: %d, %d bytes\r\n",  len_words, len_words * 4 ); // (((P_TAG *)&polyX)->len + 1) 
    printf("ret: %d\r\n\r\n", ret);
}

// returns primitive size in number of words excluding tag size.
unsigned char getPrimitivePayloadSize(void *primitive){
    /* Both functions below do the same thing*/
    //return (long) (((P_TAG*)primitive)->len);
    return ((unsigned char*)primitive)[3];// Get 3rd byte of the first word.
}
// returns primitive size in number of bytes including tag size. That is why +1 is used in tis function.
unsigned char getPrimitiveSizeBytes(void *primitive){
    return (getPrimitivePayloadSize(primitive) + 1) * 4;
}
// returns primitive size in number of words including tag size. That is why +1 is used in tis function.
unsigned char getPrimitiveSize(void *primitive){
    return getPrimitivePayloadSize(primitive) + 1;
}



/// swapEndianXXC //////////////////////////////////////////////////////////
long swapEndian32C(long* value){
    return 
    (((unsigned char*)value)[0])<<24 |
    (((unsigned char*)value)[1])<<16 |
    (((unsigned char*)value)[2])<<8  |
    (((unsigned char*)value)[3])     ;
}

short swapEndian16C(short* value){
    return 
    (((unsigned char*)value)[0])<<8  |
    (((unsigned char*)value)[1])     ;
}

/// TEST SCENARIOS //////////////////////////////////////////////////////////
void test_asm_prims(){
    short count_correct = 0, count_failed = 0;

    /*
    Store polyX in scratchpad CPU Cache at address 0x1F800000 for faster processing. 
    POLY_X is a universal struct for primitives.
        Some primitives are not supported, see documentation for more details.
    */
    POLY_X *polyX = (POLY_X*) 0x1F800000;

    /*Variables to check with if polyX was correctly mapped to corresponding data struct*/
    TILE tile;
    TILE_1 tile1;
    TILE_8 tile8;
    TILE_16 tile16;
    SPRT sprt;
    SPRT_8 sprt8;
    SPRT_16 sprt16;
    LINE_F2 lineF2;
    LINE_G2 lineG2;
    POLY_F3 polyF3;
    POLY_F4 polyF4;
    POLY_FT3 polyFT3;
    POLY_FT4 polyFT4;
    POLY_G3 polyG3;
    POLY_G4 polyG4;
    POLY_GT3 polyGT3;
    POLY_GT4 polyGT4;

    /*
    Packet buffer for GPU must be in MAIN RAM. POLY_X will be coppied as valid primitive struct that GPU expects.
    In practice, you will create a large buffer for all packages concatenated.
    Below is only one for now.
    */
    unsigned char packet[sizeof(POLY_X)];
    long ret;// result of a test case.

    printf("test_asm_prims:\r\n");

    initPOLY_X_Test(polyX);

    //TILEX
    setTile(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyTILE_X(&tile, polyX);
    primitivemapping(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&tile , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&tile , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&tile , getPrimitiveSize(polyX) , ret , "tile");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    setTile1(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyTILE_X(&tile1, polyX);
    primitivemapping(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&tile1 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&tile1 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&tile1 , getPrimitiveSize(polyX) , ret , "tile1");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    setTile8(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyTILE_X(&tile8, polyX);
    primitivemapping(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&tile8 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&tile8 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&tile8 , getPrimitiveSize(polyX) , ret , "tile8");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    setTile16(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyTILE_X(&tile16, polyX);
    primitivemapping(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&tile16 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&tile16 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&tile16 , getPrimitiveSize(polyX) , ret , "tile16");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    //SPRTX
    setSprt(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copySPRT_X(&sprt, polyX);
    primitivemapping(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&sprt , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&sprt , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&sprt , getPrimitiveSize(polyX) , ret , "sprt");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    setSprt8(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copySPRT_X(&sprt8, polyX);
    primitivemapping(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&sprt8 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&sprt8 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&sprt8 , getPrimitiveSize(polyX) , ret , "sprt8");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    setSprt16(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copySPRT_X(&sprt16, polyX);
    primitivemapping(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&sprt16 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&sprt16 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&sprt16 , getPrimitiveSize(polyX) , ret , "sprt16");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    //LINEXX
    setLineF2(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyLINE_F2(&lineF2, polyX);
    primitivemapping(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&lineF2 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&lineF2 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&lineF2 , getPrimitiveSize(polyX) , ret , "lineF2");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    setLineG2(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyLINE_G2(&lineG2, polyX);
    primitivemapping(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&lineG2 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&lineG2 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&lineG2 , getPrimitiveSize(polyX) , ret , "lineG2");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    //FXX
    setPolyF3(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyPOLY_F3(&polyF3, polyX);
    primitivemapping(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&polyF3 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&polyF3 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&polyF3 , getPrimitiveSize(polyX) , ret , "polyF3");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    setPolyF4(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyPOLY_F4(&polyF4, polyX);
    primitivemapping(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&polyF4 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&polyF4 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&polyF4 , getPrimitiveSize(polyX) , ret , "polyF4");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    setPolyFT3(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyPOLY_FT3(&polyFT3, polyX);
    primitivemapping(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&polyFT3 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&polyFT3 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&polyFT3 , getPrimitiveSize(polyX) , ret , "polyFT3");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    setPolyFT4(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyPOLY_FT4(&polyFT4, polyX);
    primitivemapping(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&polyFT4 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&polyFT4 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&polyFT4 , getPrimitiveSize(polyX) , ret , "polyFT4");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    //GXX
    setPolyG3(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyPOLY_G3(&polyG3, polyX);
    primitivemapping(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&polyG3 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&polyG3 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&polyG3 , getPrimitiveSize(polyX) , ret , "polyG3");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    setPolyG4(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyPOLY_G4(&polyG4, polyX);
    primitivemapping(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&polyG4 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&polyG4 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&polyG4 , getPrimitiveSize(polyX) , ret , "polyG4");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    setPolyGT3(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyPOLY_GT3(&polyGT3, polyX);
    primitivemapping(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&polyGT3 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&polyGT3 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&polyGT3 , getPrimitiveSize(polyX) , ret , "polyGT3");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    setPolyGT4(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyPOLY_GT4(&polyGT4, polyX);
    primitivemapping(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&polyGT4 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&polyGT4 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&polyGT4 , getPrimitiveSize(polyX) , ret , "polyGT4");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    printf("Stats: correct: %d failed: %d total: %d\r\n", count_correct, count_failed , (count_correct + count_failed) );

}

void test_asm_memcpyc(long len){
    long i, count_missmatch = 0;;
    unsigned char src[(abs(len)+15)/16*16];// round up to multiple of 16
    unsigned char dst[(abs(len)+15)/16*16];
    
    printf("test_asm_memcpyc:\r\n");

    memset(src, 0, ARRAY_SIZE(src));
    memset(dst, 0, ARRAY_SIZE(dst));

    printf("len: %d\r\n", len);
    printf("src  addr: %08X\r\n", src);
    printf("dst addr: %08X\r\n", dst);

    for(i=0; i<abs(len); i++){
        src[i] = i;
    }

    //memcpyc(void *dst_ptr, void *src_ptr, unsigned long len_bytes);
    memcpyc(dst, src,  len);


    for(i=0; i<abs(len); i++){
        if(*(dst + i) == *(src + i)){
            printf("check[%d]: %08X %08X\r\n", i, *(dst + i) , *(src + i)  );
        }else{
            printf("check[%d]: %08X %08X X\r\n", i, *(dst + i) , *(src + i)  );
            count_missmatch++;
        }
    }
    printf("Stats: failed: %d total: %d\r\n", count_missmatch , len );
}

void test_asm_memcpyword(long number_of_words){
    long i, count_missmatch = 0;
    unsigned char src[(abs(number_of_words) * 4 + 15)/16*16];// round up to multiple of 16
    unsigned char dst[(abs(number_of_words) * 4 + 15)/16*16];
    
    printf("test_asm_memcpyword:\r\n");

    memset(src, 0, ARRAY_SIZE(src));
    memset(dst, 0, ARRAY_SIZE(dst));

    printf("number_of_words: %d\r\n", number_of_words);
    printf("src addr: %08X\r\n", src);
    printf("dst addr: %08X\r\n", dst);

    for(i=0; i<abs(number_of_words) * 4; i++){
        src[i] = i;
    }

    //memcpyc(void *dst_ptr, void *src_ptr, unsigned long len_bytes);
    memcpyw(dst, src,  number_of_words);


    for(i=0; i<abs(number_of_words) * 4; i++){
        if(*(dst + i) == *(src + i)){
            printf("check[%d]: %08X %08X\r\n", i, *(dst + i) , *(src + i)  );
        }else{
            printf("check[%d]: %08X %08X X\r\n", i, *(dst + i) , *(src + i)  );
            count_missmatch++;
        }
    }
    printf("Stats: failed: %d total: %d\r\n", count_missmatch , (number_of_words * 4) );
}



void test_asm_unaligned_to_aligned(){
    long i, out;
    unsigned char src[16];
    unsigned char dst[16];
    
    printf("test_asm_unaligned_to_aligned:\r\n");

    memset(src, 0, ARRAY_SIZE(src));
    memset(dst, 0, ARRAY_SIZE(dst));

    for(i=0; i<16; i++){
        src[i] = i;
    }

    out = unalignword(src + 5);

    printf("out: %08X\r\n", out);
    printf("out addr: %08X\r\n", &out);
    printf("src addr: %08X\r\n", src);
    printf("dst addr: %08X\r\n", dst);
}


void test_asm_swapEndian(){
    long i,ret , previous_value, expected_value;
    short expected_value16 , reserve0;
    short count_correct = 0, count_failed = 0;
    unsigned char src[16];
    unsigned char dst[16];

    printf("test_asm_swapEndian:\r\n");

    memset(src, 0, ARRAY_SIZE(src));
    memset(dst, 0, ARRAY_SIZE(dst));

    // init values
    for(i=0; i<16; i++){
        src[i] = i + 1;// do not start at value zero due to 0x00 values in short w.r.t. long.
    }

    /*TEST: swapEndian16Ret(...) */
    printf("swapEndian16Ret:\r\n");
    *((short*)&dst[0]) = swapEndian16Ret(src);

    printf("src value as long for debugging: %08X\r\n", *((long*)&src[0]) );
    printf("dst value as long for debugging: %08X\r\n", *((long*)&dst[0]) );
    printf("src value short: %04X\r\n", *((short*)&src[0]) );
    printf("dst value short: %04X\r\n", *((short*)&dst[0]) );

    // swap Endian in C.
    expected_value16 = swapEndian16C( ((short*)&src[0]) );
    ret = *((short*)&dst[0]) == expected_value16 ? -1 : 0;
    printf("swapEndian16Ret: correct: %d\r\n", ret == -1 );
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    /*TEST: swapEndian16(...) */
    printf("swapEndian16:\r\n");

    // swap Endian in C.
    expected_value16 = swapEndian16C( ((short*)&src[0]) );
    swapEndian16(src);
    ret = *((short*)&dst[0]) == expected_value16 ? -1 : 0;
    printf("swapEndian16: correct: %d\r\n", ret == -1 );
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;


    /*Init values */
    for(i=0; i<16; i++){
        src[i] = i + 1;
    }

    /*TEST: swapEndian32Ret(...) */
    printf("swapEndian32Ret:\r\n");
    *((long*)&dst[0]) = swapEndian32Ret(src);
    // swap Endian in C.
    expected_value = swapEndian32C( ((long*)&src[0]) );
    ret = check_same_words( ((long*)&dst[0]) , &expected_value , 1);
    printf("swapEndian32Ret: correct: %d\r\n", ret == -1 );
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    /*TEST: swapEndian32(...) */
    printf("swapEndian32:\r\n");
    previous_value = *((long*)&src[0]) ;
    printf("src value before: %08X\r\n", *((long*)&src[0]) );
    printf("src addr: %08X\r\n", src);
    // Swap Endian in place memory. i.e. location at pointer is changed. 
    swapEndian32(src);
    printf("src value after: %08X\r\n", *((long*)&src[0]) );

    // swap Endian in C.
    expected_value = swapEndian32C(&previous_value);

    printf("expected_value value: %08X\r\n",  ( expected_value ) );
    ret = check_same_words( ((long*)&src[0]) , &expected_value , 1);
    printf("swapEndian32: correct: %d\r\n", ret == -1 );
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    printf("Stats: correct: %d failed: %d total: %d\r\n", count_correct, count_failed , (count_correct + count_failed) );
}


void test_asm_primitivemappingVertexOnly(){
    short count_correct = 0, count_failed = 0;

    /*
    Store polyX in scratchpad CPU Cache at address 0x1F800000 for faster processing. 
    POLY_X is a universal struct for primitives.
        Some primitives are not supported, see documentation for more details.
    */
    POLY_X *polyX = (POLY_X*) 0x1F800000;

    /*Variables to check with if polyX was correctly mapped to corresponding data struct*/
    TILE tile;
    TILE_1 tile1;
    TILE_8 tile8;
    TILE_16 tile16;
    SPRT sprt;
    SPRT_8 sprt8;
    SPRT_16 sprt16;
    LINE_F2 lineF2;
    LINE_G2 lineG2;
    POLY_F3 polyF3;
    POLY_F4 polyF4;
    POLY_FT3 polyFT3;
    POLY_FT4 polyFT4;
    POLY_G3 polyG3;
    POLY_G4 polyG4;
    POLY_GT3 polyGT3;
    POLY_GT4 polyGT4;

    /*
    Packet buffer for GPU must be in MAIN RAM. POLY_X will be coppied as valid primitive struct that GPU expects.
    In practice, you will create a large buffer for all packages concatenated.
    Below is only one for now.
    */
    unsigned char packet[sizeof(POLY_X)];
    long ret;// result of a test case.

    printf("test_asm_primitivemappingVertexOnly:\r\n");

    initPOLY_X_Test(polyX);

    printf("test_asm_primitivemappingVertexOnly packet ADDR: %08X:\r\n",packet);

    //TILEX
    setTile(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyTILE_X(&tile, polyX);
    keepVertexData(&tile, getPrimitiveSize(polyX));
    primitivemappingVertexOnly(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&tile , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&tile , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&tile , getPrimitiveSize(polyX) , ret , "tile");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;



    setTile1(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyTILE_X(&tile1, polyX);
    keepVertexData(&tile1, getPrimitiveSize(polyX));
    primitivemappingVertexOnly(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&tile1 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&tile1 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&tile1 , getPrimitiveSize(polyX) , ret , "tile1");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    setTile8(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyTILE_X(&tile8, polyX);
    keepVertexData(&tile8, getPrimitiveSize(polyX));
    primitivemappingVertexOnly(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&tile8 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&tile8 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&tile8 , getPrimitiveSize(polyX) , ret , "tile8");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    setTile16(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyTILE_X(&tile16, polyX);
    keepVertexData(&tile16, getPrimitiveSize(polyX));
    primitivemappingVertexOnly(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&tile16 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&tile16 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&tile16 , getPrimitiveSize(polyX) , ret , "tile16");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    //SPRTX
    setSprt(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copySPRT_X(&sprt, polyX);
    keepVertexData(&sprt, getPrimitiveSize(polyX));
    primitivemappingVertexOnly(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&sprt , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&sprt , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&sprt , getPrimitiveSize(polyX) , ret , "sprt");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    setSprt8(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copySPRT_X(&sprt8, polyX);
    keepVertexData(&sprt8, getPrimitiveSize(polyX));
    primitivemappingVertexOnly(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&sprt8 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&sprt8 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&sprt8 , getPrimitiveSize(polyX) , ret , "sprt8");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    setSprt16(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copySPRT_X(&sprt16, polyX);
    keepVertexData(&sprt16, getPrimitiveSize(polyX));
    primitivemappingVertexOnly(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&sprt16 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&sprt16 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&sprt16 , getPrimitiveSize(polyX) , ret , "sprt16");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    //LINEXX
    setLineF2(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyLINE_F2(&lineF2, polyX);
    keepVertexData(&lineF2, getPrimitiveSize(polyX));
    primitivemappingVertexOnly(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&lineF2 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&lineF2 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&lineF2 , getPrimitiveSize(polyX) , ret , "lineF2");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    setLineG2(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyLINE_G2(&lineG2, polyX);
    keepVertexData(&lineG2, getPrimitiveSize(polyX));
    primitivemappingVertexOnly(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&lineG2 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&lineG2 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&lineG2 , getPrimitiveSize(polyX) , ret , "lineG2");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    //FXX
    setPolyF3(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyPOLY_F3(&polyF3, polyX);
    keepVertexData(&polyF3, getPrimitiveSize(polyX));
    primitivemappingVertexOnly(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&polyF3 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&polyF3 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&polyF3 , getPrimitiveSize(polyX) , ret , "polyF3");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    setPolyF4(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyPOLY_F4(&polyF4, polyX);
    keepVertexData(&polyF4, getPrimitiveSize(polyX));
    primitivemappingVertexOnly(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&polyF4 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&polyF4 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&polyF4 , getPrimitiveSize(polyX) , ret , "polyF4");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    setPolyFT3(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyPOLY_FT3(&polyFT3, polyX);
    keepVertexData(&polyFT3, getPrimitiveSize(polyX));
    primitivemappingVertexOnly(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&polyFT3 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&polyFT3 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&polyFT3 , getPrimitiveSize(polyX) , ret , "polyFT3");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    setPolyFT4(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyPOLY_FT4(&polyFT4, polyX);
    keepVertexData(&polyFT4, getPrimitiveSize(polyX));
    primitivemappingVertexOnly(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&polyFT4 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&polyFT4 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&polyFT4 , getPrimitiveSize(polyX) , ret , "polyFT4");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    //GXX
    setPolyG3(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyPOLY_G3(&polyG3, polyX);
    keepVertexData(&polyG3, getPrimitiveSize(polyX));
    primitivemappingVertexOnly(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&polyG3 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&polyG3 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&polyG3 , getPrimitiveSize(polyX) , ret , "polyG3");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    setPolyG4(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyPOLY_G4(&polyG4, polyX);
    keepVertexData(&polyG4, getPrimitiveSize(polyX));
    primitivemappingVertexOnly(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&polyG4 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&polyG4 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&polyG4 , getPrimitiveSize(polyX) , ret , "polyG4");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    setPolyGT3(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyPOLY_GT3(&polyGT3, polyX);
    keepVertexData(&polyGT3, getPrimitiveSize(polyX));
    primitivemappingVertexOnly(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&polyGT3 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&polyGT3 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&polyGT3 , getPrimitiveSize(polyX) , ret , "polyGT3");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    setPolyGT4(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyPOLY_GT4(&polyGT4, polyX);
    keepVertexData(&polyGT4, getPrimitiveSize(polyX));
    primitivemappingVertexOnly(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&polyGT4 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&polyGT4 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&polyGT4 , getPrimitiveSize(polyX) , ret , "polyGT4");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    printf("Stats: correct: %d failed: %d total: %d\r\n", count_correct, count_failed , (count_correct + count_failed) );

}


/* This does not log failed or success, manual check is needed.*/
void test_asm_PrimitivemappingSingle_vertexOnly(){
    short count_correct = 0, count_failed = 0;

    /*
    Store polyX in scratchpad CPU Cache at address 0x1F800000 for faster processing. 
    POLY_X is a universal struct for primitives.
        Some primitives are not supported, see documentation for more details.
    */
    POLY_X *polyX = (POLY_X*) 0x1F800000;

    /*Variables to check with if polyX was correctly mapped to corresponding data struct*/
    TILE tile;
    TILE_1 tile1;
    TILE_8 tile8;
    TILE_16 tile16;
    SPRT sprt;
    SPRT_8 sprt8;
    SPRT_16 sprt16;
    LINE_F2 lineF2;
    LINE_G2 lineG2;
    POLY_F3 polyF3;
    POLY_F4 polyF4;
    POLY_FT3 polyFT3;
    POLY_FT4 polyFT4;
    POLY_G3 polyG3;
    POLY_G4 polyG4;
    POLY_GT3 polyGT3;
    POLY_GT4 polyGT4;

    /*
    Packet buffer for GPU must be in MAIN RAM. POLY_X will be coppied as valid primitive struct that GPU expects.
    In practice, you will create a large buffer for all packages concatenated.
    Below is only one for now.
    */
    unsigned char packet[sizeof(POLY_X)];
    long ret;// result of a test case.

    printf("test_asm_prims:\r\n");

    initPOLY_X_Test(polyX);

    //TILEX
    setTile(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyTILE_X(&tile, polyX);
    primitivemappingVertexOnlyTileSptrX(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&tile , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&tile , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&tile , getPrimitiveSize(polyX) , ret , "tile");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    setTile1(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyTILE_X(&tile1, polyX);
    primitivemappingVertexOnlyTileSptrX(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&tile1 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&tile1 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&tile1 , getPrimitiveSize(polyX) , ret , "tile1");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    setTile8(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyTILE_X(&tile8, polyX);
    primitivemappingVertexOnlyTileSptrX(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&tile8 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&tile8 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&tile8 , getPrimitiveSize(polyX) , ret , "tile8");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    setTile16(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyTILE_X(&tile16, polyX);
    primitivemappingVertexOnlyTileSptrX(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&tile16 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&tile16 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&tile16 , getPrimitiveSize(polyX) , ret , "tile16");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    //SPRTX
    setSprt(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copySPRT_X(&sprt, polyX);
    primitivemappingVertexOnlyTileSptrX(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&sprt , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&sprt , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&sprt , getPrimitiveSize(polyX) , ret , "sprt");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    setSprt8(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copySPRT_X(&sprt8, polyX);
    primitivemappingVertexOnlyTileSptrX(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&sprt8 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&sprt8 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&sprt8 , getPrimitiveSize(polyX) , ret , "sprt8");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    setSprt16(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copySPRT_X(&sprt16, polyX);
    primitivemappingVertexOnlyTileSptrX(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&sprt16 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&sprt16 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&sprt16 , getPrimitiveSize(polyX) , ret , "sprt16");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    //LINEXX
    setLineF2(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyLINE_F2(&lineF2, polyX);
    primitivemappingVertexOnlyPolyF2(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&lineF2 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&lineF2 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&lineF2 , getPrimitiveSize(polyX) , ret , "lineF2");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    setLineG2(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyLINE_G2(&lineG2, polyX);
    primitivemappingVertexOnlyPolyG2(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&lineG2 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&lineG2 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&lineG2 , getPrimitiveSize(polyX) , ret , "lineG2");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    //FXX
    setPolyF3(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyPOLY_F3(&polyF3, polyX);
    primitivemappingVertexOnlyPolyF3(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&polyF3 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&polyF3 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&polyF3 , getPrimitiveSize(polyX) , ret , "polyF3");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    setPolyF4(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyPOLY_F4(&polyF4, polyX);
    primitivemappingVertexOnlyPolyF4(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&polyF4 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&polyF4 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&polyF4 , getPrimitiveSize(polyX) , ret , "polyF4");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    setPolyFT3(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyPOLY_FT3(&polyFT3, polyX);
    primitivemappingVertexOnlyPolyFT3(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&polyFT3 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&polyFT3 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&polyFT3 , getPrimitiveSize(polyX) , ret , "polyFT3");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    setPolyFT4(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyPOLY_FT4(&polyFT4, polyX);
    primitivemappingVertexOnlyPolyFT4(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&polyFT4 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&polyFT4 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&polyFT4 , getPrimitiveSize(polyX) , ret , "polyFT4");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    //GXX
    setPolyG3(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyPOLY_G3(&polyG3, polyX);
    primitivemappingVertexOnlyPolyG3(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&polyG3 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&polyG3 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&polyG3 , getPrimitiveSize(polyX) , ret , "polyG3");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    setPolyG4(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyPOLY_G4(&polyG4, polyX);
    primitivemappingVertexOnlyPolyG4(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&polyG4 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&polyG4 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&polyG4 , getPrimitiveSize(polyX) , ret , "polyG4");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    setPolyGT3(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyPOLY_GT3(&polyGT3, polyX);
    primitivemappingVertexOnlyPolyGT3(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&polyGT3 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&polyGT3 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&polyGT3 , getPrimitiveSize(polyX) , ret , "polyGT3");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    setPolyGT4(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyPOLY_GT4(&polyGT4, polyX);
    primitivemappingVertexOnlyPolyGT4(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&polyGT4 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&polyGT4 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&polyGT4 , getPrimitiveSize(polyX) , ret , "polyGT4");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    //printf("Stats: correct: %d failed: %d total: %d\r\n", count_correct, count_failed , (count_correct + count_failed) );
    printf("Stats: No stats, check manually for correctness.\r\n");

}

/* This does not log failed or success, manual check is needed.*/
void test_asm_PrimitivemappingSingle_colorOnly(){
    short count_correct = 0, count_failed = 0;

    /*
    Store polyX in scratchpad CPU Cache at address 0x1F800000 for faster processing. 
    POLY_X is a universal struct for primitives.
        Some primitives are not supported, see documentation for more details.
    */
    POLY_X *polyX = (POLY_X*) 0x1F800000;

    /*Variables to check with if polyX was correctly mapped to corresponding data struct*/
    TILE tile;
    TILE_1 tile1;
    TILE_8 tile8;
    TILE_16 tile16;
    SPRT sprt;
    SPRT_8 sprt8;
    SPRT_16 sprt16;
    LINE_F2 lineF2;
    LINE_G2 lineG2;
    POLY_F3 polyF3;
    POLY_F4 polyF4;
    POLY_FT3 polyFT3;
    POLY_FT4 polyFT4;
    POLY_G3 polyG3;
    POLY_G4 polyG4;
    POLY_GT3 polyGT3;
    POLY_GT4 polyGT4;

    /*
    Packet buffer for GPU must be in MAIN RAM. POLY_X will be coppied as valid primitive struct that GPU expects.
    In practice, you will create a large buffer for all packages concatenated.
    Below is only one for now.
    */
    unsigned char packet[sizeof(POLY_X)];
    long ret;// result of a test case.

    printf("test_asm_prims:\r\n");

    initPOLY_X_Test(polyX);

    //TILEX
    setTile(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyTILE_X(&tile, polyX);
    primitivemappingColorOnlyPolyFXXLineFXTileSptrX(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&tile , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&tile , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&tile , getPrimitiveSize(polyX) , ret , "tile");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    setTile1(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyTILE_X(&tile1, polyX);
    primitivemappingColorOnlyPolyFXXLineFXTileSptrX(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&tile1 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&tile1 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&tile1 , getPrimitiveSize(polyX) , ret , "tile1");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    setTile8(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyTILE_X(&tile8, polyX);
    primitivemappingColorOnlyPolyFXXLineFXTileSptrX(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&tile8 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&tile8 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&tile8 , getPrimitiveSize(polyX) , ret , "tile8");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    setTile16(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyTILE_X(&tile16, polyX);
    primitivemappingColorOnlyPolyFXXLineFXTileSptrX(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&tile16 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&tile16 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&tile16 , getPrimitiveSize(polyX) , ret , "tile16");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    //SPRTX
    setSprt(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copySPRT_X(&sprt, polyX);
    primitivemappingColorOnlyPolyFXXLineFXTileSptrX(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&sprt , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&sprt , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&sprt , getPrimitiveSize(polyX) , ret , "sprt");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    setSprt8(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copySPRT_X(&sprt8, polyX);
    primitivemappingColorOnlyPolyFXXLineFXTileSptrX(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&sprt8 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&sprt8 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&sprt8 , getPrimitiveSize(polyX) , ret , "sprt8");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    setSprt16(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copySPRT_X(&sprt16, polyX);
    primitivemappingColorOnlyPolyFXXLineFXTileSptrX(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&sprt16 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&sprt16 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&sprt16 , getPrimitiveSize(polyX) , ret , "sprt16");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    //LINEXX
    setLineF2(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyLINE_F2(&lineF2, polyX);
    primitivemappingColorOnlyPolyFXXLineFXTileSptrX(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&lineF2 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&lineF2 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&lineF2 , getPrimitiveSize(polyX) , ret , "lineF2");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    setLineG2(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyLINE_G2(&lineG2, polyX);
    primitivemappingColorOnlyLineG2(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&lineG2 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&lineG2 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&lineG2 , getPrimitiveSize(polyX) , ret , "lineG2");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    //FXX
    setPolyF3(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyPOLY_F3(&polyF3, polyX);
    primitivemappingColorOnlyPolyFXXLineFXTileSptrX(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&polyF3 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&polyF3 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&polyF3 , getPrimitiveSize(polyX) , ret , "polyF3");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    setPolyF4(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyPOLY_F4(&polyF4, polyX);
    primitivemappingColorOnlyPolyFXXLineFXTileSptrX(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&polyF4 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&polyF4 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&polyF4 , getPrimitiveSize(polyX) , ret , "polyF4");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    setPolyFT3(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyPOLY_FT3(&polyFT3, polyX);
    primitivemappingColorOnlyPolyFXXLineFXTileSptrX(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&polyFT3 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&polyFT3 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&polyFT3 , getPrimitiveSize(polyX) , ret , "polyFT3");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    setPolyFT4(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyPOLY_FT4(&polyFT4, polyX);
    primitivemappingColorOnlyPolyFXXLineFXTileSptrX(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&polyFT4 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&polyFT4 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&polyFT4 , getPrimitiveSize(polyX) , ret , "polyFT4");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    //GXX
    setPolyG3(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyPOLY_G3(&polyG3, polyX);
    primitivemappingColorOnlyPolyG3(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&polyG3 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&polyG3 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&polyG3 , getPrimitiveSize(polyX) , ret , "polyG3");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    setPolyG4(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyPOLY_G4(&polyG4, polyX);
    primitivemappingColorOnlyPolyG4(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&polyG4 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&polyG4 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&polyG4 , getPrimitiveSize(polyX) , ret , "polyG4");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    setPolyGT3(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyPOLY_GT3(&polyGT3, polyX);
    primitivemappingColorOnlyPolyGT3(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&polyGT3 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&polyGT3 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&polyGT3 , getPrimitiveSize(polyX) , ret , "polyGT3");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    setPolyGT4(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyPOLY_GT4(&polyGT4, polyX);
    primitivemappingColorOnlyPolyGT4(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&polyGT4 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&polyGT4 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&polyGT4 , getPrimitiveSize(polyX) , ret , "polyGT4");
    count_correct += ret ==-1 ? 1 : 0;
    count_failed  += ret !=-1 ? 1 : 0;

    //printf("Stats: correct: %d failed: %d total: %d\r\n", count_correct, count_failed , (count_correct + count_failed) );
    printf("Stats: No stats, check manually for correctness.\r\n");

}

/* This does not log failed or success, manual check is needed.*/
void test_asm_PrimitivemappingSingle_uvOnly(){
    short count_correct = 0, count_failed = 0;

    /*
    Store polyX in scratchpad CPU Cache at address 0x1F800000 for faster processing. 
    POLY_X is a universal struct for primitives.
        Some primitives are not supported, see documentation for more details.
    */
    POLY_X *polyX = (POLY_X*) 0x1F800000;

    /*Variables to check with if polyX was correctly mapped to corresponding data struct*/
    TILE tile;
    TILE_1 tile1;
    TILE_8 tile8;
    TILE_16 tile16;
    SPRT sprt;
    SPRT_8 sprt8;
    SPRT_16 sprt16;
    LINE_F2 lineF2;
    LINE_G2 lineG2;
    POLY_F3 polyF3;
    POLY_F4 polyF4;
    POLY_FT3 polyFT3;
    POLY_FT4 polyFT4;
    POLY_G3 polyG3;
    POLY_G4 polyG4;
    POLY_GT3 polyGT3;
    POLY_GT4 polyGT4;

    /*
    Packet buffer for GPU must be in MAIN RAM. POLY_X will be coppied as valid primitive struct that GPU expects.
    In practice, you will create a large buffer for all packages concatenated.
    Below is only one for now.
    */
    unsigned char packet[sizeof(POLY_X)];
    long ret;// result of a test case.

    printf("test_asm_prims:\r\n");

    initPOLY_X_Test(polyX);

    //TILEX NOT SUPPORTED HAS NO TEXTURE. SO, THIS MUST FAIL.
    setTile(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyTILE_X(&tile, polyX);
    primitivemappingUvOnlyTileSptrX(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&tile , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&tile , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&tile , getPrimitiveSize(polyX) , ret , "tile");
    // count_correct += ret ==-1 ? 1 : 0;
    // count_failed  += ret !=-1 ? 1 : 0;

    //SPRTX
    setSprt(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copySPRT_X(&sprt, polyX);
    primitivemappingUvOnlyTileSptrX(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&sprt , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&sprt , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&sprt , getPrimitiveSize(polyX) , ret , "sprt");
    // count_correct += ret ==-1 ? 1 : 0;
    // count_failed  += ret !=-1 ? 1 : 0;

    setSprt8(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copySPRT_X(&sprt8, polyX);
    primitivemappingUvOnlyTileSptrX(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&sprt8 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&sprt8 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&sprt8 , getPrimitiveSize(polyX) , ret , "sprt8");
    // count_correct += ret ==-1 ? 1 : 0;
    // count_failed  += ret !=-1 ? 1 : 0;

    setSprt16(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copySPRT_X(&sprt16, polyX);
    primitivemappingUvOnlyTileSptrX(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&sprt16 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&sprt16 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&sprt16 , getPrimitiveSize(polyX) , ret , "sprt16");
    // count_correct += ret ==-1 ? 1 : 0;
    // count_failed  += ret !=-1 ? 1 : 0;

    //FXX
    setPolyFT3(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyPOLY_FT3(&polyFT3, polyX);
    primitivemappingUvOnlyPolyFT3(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&polyFT3 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&polyFT3 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&polyFT3 , getPrimitiveSize(polyX) , ret , "polyFT3");
    // count_correct += ret ==-1 ? 1 : 0;
    // count_failed  += ret !=-1 ? 1 : 0;

    setPolyFT4(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyPOLY_FT4(&polyFT4, polyX);
    primitivemappingUvOnlyPolyFT4(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&polyFT4 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&polyFT4 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&polyFT4 , getPrimitiveSize(polyX) , ret , "polyFT4");
    // count_correct += ret ==-1 ? 1 : 0;
    // count_failed  += ret !=-1 ? 1 : 0;

    //GXX
    setPolyGT3(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyPOLY_GT3(&polyGT3, polyX);
    primitivemappingUvOnlyPolyGT3(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&polyGT3 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&polyGT3 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&polyGT3 , getPrimitiveSize(polyX) , ret , "polyGT3");
    // count_correct += ret ==-1 ? 1 : 0;
    // count_failed  += ret !=-1 ? 1 : 0;

    setPolyGT4(polyX);
    memset(packet, 0, sizeof(POLY_X));
    copyPOLY_GT4(&polyGT4, polyX);
    primitivemappingUvOnlyPolyGT4(&packet, polyX);
    ret = check_same_words((unsigned long*)packet , (unsigned long*)&polyGT4 , getPrimitiveSize(polyX) );
    check_same_words_print((unsigned long*)packet , (unsigned long*)&polyGT4 , getPrimitiveSize(polyX) );
    printPoltCompairStats( (unsigned long*)packet , (unsigned long*)&polyGT4 , getPrimitiveSize(polyX) , ret , "polyGT4");
    // count_correct += ret ==-1 ? 1 : 0;
    // count_failed  += ret !=-1 ? 1 : 0;

    //printf("Stats: correct: %d failed: %d total: %d\r\n", count_correct, count_failed , (count_correct + count_failed) );
    printf("Stats: No stats, check manually for correctness.\r\n");

}



int main(void) {
    /* RUN TEST SCENARIOS */
    test_asm_prims();

    test_asm_primitivemappingVertexOnly();

    test_asm_memcpyc(96); // copy 96 bytes
    test_asm_memcpyc(21);
    test_asm_memcpyc(17);
    test_asm_memcpyc(16);
    test_asm_memcpyc(5);
    test_asm_memcpyc(4);
    test_asm_memcpyc(3);
    test_asm_memcpyc(0);
    //test_asm_memcpyc(-1); // try negative length. Note that ASM processes 0 bytes, but the test fails while comparing because no data was copied from src array to dst array.

    test_asm_memcpyword(6); // copy 6 words i.e. 24 bytes
    test_asm_memcpyword(3); // copy 3 words
    test_asm_memcpyword(0); // copy 0 words
    //test_asm_memcpyword(-1);  // try negative length. Note that ASM processes 0 bytes, but the test fails while comparing because no data was copied from src array to dst array.

    test_asm_unaligned_to_aligned();

    test_asm_swapEndian();

    test_asm_PrimitivemappingSingle_vertexOnly();
    test_asm_PrimitivemappingSingle_colorOnly();
    test_asm_PrimitivemappingSingle_uvOnly();

    return 0;
}
