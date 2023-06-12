cmake_minimum_required(VERSION 3.10)

project(cpu_usage_tracker)
set(CMAKE_C_STANDARD 99)

set(INC_DIRS
	inc/	
)

set(SRCS
	src/main.c	
)

if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
	add_compile_options(-Wall -Wextra)
	message("GCC IS USING")
elseif(CMAKE_C_COMPILER_ID STREQUAL "Clang")
	add_compile_options(-Weverything)
	message("CLANG IS USING")
endif()

add_executable(${CMAKE_PROJECT_NAME} ${SRCS})
include_directories(${CMAKE_PROJECT_NAME} ${INC_DIRS})


