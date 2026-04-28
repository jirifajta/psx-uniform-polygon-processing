
cmake_minimum_required(VERSION 3.21)

project(
	NAME_MAIN
	LANGUAGES    C
	VERSION      1.0.0
	DESCRIPTION  "PSX"
)

enable_language(ASM)

file(GLOB  _sources *.c ASM/Mipsel/*.s)

psn00bsdk_add_executable(main GPREL ${_sources})

install(FILES ${PROJECT_BINARY_DIR}/main.exe TYPE BIN)
