# PSX uniform polygon processing

PSX graphical primitives makes use of multiple type of data structs to define its property like _POLY_F3_, _POLY_GT4_ and _LINE_F2_.
During programming functions to process mesh’s vertices/faces to primitives, I did end up with various functions to process these various type of primitive structs. Which was not ideal. My new approach was to find a solution to make this more uniform.
Therefore, a struct type and assembly function were created to process all polygons in the same fashion. 

## So, what does assembly function actually do?<br/>
You process all updates in _POLY_X_ which is great to store in scratchpad memory in CPU to process data fast. Once all relevant variables are set (including GTE functions returning values like sx, sy), the assembly function copy data from _POLY_X_ into RAM as the targeted polygon type would be, for example _POLY_GT3_.
Assembly function recognize which POLY type it has to convert to, because _POLY_X_ _[code]_ (and evantually _[tag]_) where set by functions such as _setPolyGT3(&polyx)_. Be aware to set these manually or use macro function such as _setPoly...(&polyx)_ to ignore datatype.

## What about performance rather then using C code?<br/>
Well this assembly was not tested in an isolated environment to test the real performance. I have used this in my more complex program and just this change did perform 15% faster. Executing GTE functons with _POLY_X_, assembly and loading vertecies in the right order can perform even up to 100% faster.
<br/>
<br/>
**Author:** MSc Jiří Fajta<br/>
**Code implementation date:** 2025-2026<br/>

# MIPS assembly codes
Function description, arguments and requirements are found in [.h](/ASM/) files.

> [!NOTE]
> This repository only contains my own source code written from scratch which can be used for **Mipsel GCC** (like ![PSn00bSDK](https://github.com/Lameguy64/PSn00bSDK/)) and **PsyQ** compilers.


## Functions
![Primitivemapping.h](src/ASM/Primitivemapping.h)
* Copy _POLY_X_ struct -> supported primitive data structs for PSX GPU.
* Supported primitive data structs:<br/>
  - POLY_XXX [F3, F4, FT3, FT4, G3, G4, GT3, GT4]<br/>
  - LINE_XX [F2, G2]<br/>
  - SPRT, SPRT_8, SPRT_16, TILE, TILE_1, TILE_8, TILE_16<br/>
* NOT supported primitive data structs:<br/>
  - LINE_F3, LINE_F4, LINE_G3, LINE_G4
  - All types of DR_X (like DR_MODE, DR_TWIN and so on).

> [!NOTE]
> One variable is missing in _POLY_X_ struct which eliminates the compatibility with _LINE_F3_-_LINE_G4_. This decision was made to reduce space usage in scratchpad. _DR_X_ (like _DR_MODE_) are not supported due to lack of check in [code]-variable for their ID. This is a design choice to reduce compare operations in assembly.

> [!TIP]
> You can always copy data for 4 vertecies into _POLY_X_ dispite only three are needed like in case of FT3. _PrimitivemappingXXX.h_ will ingore garbage data copied into 4th vertex _POLY_X_, because it will copy only data that is needed due to [code].

![PrimitivemappingVertexOnly.h](src/ASM/PrimitivemappingVertexOnly.h)
* While _Primitivemapping.h_ copies all relevent variables, _PrimitivemappingVertexOnly.h_ copies only x's and y's. This performs slightly faster.

![PrimitivemappingSingle.h](src/ASM/PrimitivemappingSingle.h)
* Copies only specific attributes from _POLY_X_. This performs slightly faster.

> [!TIP]
> _PrimitivemappingSingle.h_ provides opperations for specific type of primitive, therefore use function pointers to these functions to speed up loops by detecting correct primitive only once. 6% improvement over _Primitivemapping.h_.

![Primitivemappinginvert.h](src/ASM/Primitivemappinginvert.h)
* Inverts the process of _Primitivemapping.h_.
* Copy supported primitive data structs for PSX GPU -> _POLY_X_ struct.
> [!WARNING]
> Primitivemapping.h was never tested, therefore it might produce incorrect behaviour.

![Memcpyc.h](src/ASM/Memcpyc.h)
* _Memcpyc_ copies up to 16 bytes in sequence which is faster then default memcpy() function. _Memcpy()_ copies byte for byte.
* _Memcpyword_ is also included, this copies in multiple of 4 bytes only.

> [!TIP]
> _Memcpyc_ copies up to 16 bytes sequentially (16 bytes read then 16 bytes write). This function also supports any length to copy (i.e. it does not has to be multiple of 4 or 16 bytes). Hence that if remaining copy length is less then 16 bytes then it copies per 4 bytes. Hence also that if remaining copy length is less then 4 bytes then last up to 3 bytes are copied byte by byte.<br/> Use _Memcpy_ for shorter copy opperations. Note that data to be copied from and to must be 4 bytes aligned.


![UnAlignWord.h](src/ASM/UnAlignWord.h)
* Supports to read 4 unaligned bytes to return 4 aligned bytes i.e. 1 aligned word.

![SwapEndian.h](src/ASM/SwapEndian.h)
* Converts 16-bit and 32-bit values between _Little-endian_ <-> _Big-endian_.
* Two flavers: With and without return value.


# What about union struct?
Moreover _uniform_ _polygon_,I was investigating how union struct could solve this problem. But the challenge remains the same. Because the variables of various structs where not on the same position in memory and more importantly, the access in C code was not stored in same variable. Hence this example:<br/>

**typedef union**{<br/>
. POLY_G3 g3;<br/>
. POLY_F3 f3;<br/>
. POLY_FT3 ft3;<br/>
. POLY_GT3 gt3;<br/>
**} POLY_3**;<br/><br/>
**POLY_3** poly_3;<br/>
poly_3.**g3**.x2 = poly_3.**ft3**.x2 = 100.

Still, data needs to be accessed by using variable _**g3**_ and _**ft3**_. This still makes access not uniform.

# Additional codes
[main.c](src/main.c) tests if assembly code results in correct output.<br/>
[PrimitivemappingSnipped.c](src/PrimitivemappingSnipped.c) provides high level idea about how to use Primitivemapping.h.