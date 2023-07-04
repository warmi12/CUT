cmake_minimum_required(VERSION 3.10)

project(cpu_usage_tracker)

set(CMAKE_C_STANDARD 99)

add_subdirectory(test/ring_buffer)

add_executable(${CMAKE_PROJECT_NAME}
        src/main.c
        src/ring_buffer/ring_buffer.c
        src/reader/reader.c
        src/analyzer/analyzer.c
        src/printer/printer.c
	src/threads_info/threads_info.c
)


target_include_directories(
	${CMAKE_PROJECT_NAME} PRIVATE
        inc/ring_buffer/
        inc/reader/
        inc/analyzer/
        inc/printer/
	inc/threads_info/
)

if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
        target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -Wall -Wextra)
        message("GCC IS USING")
elseif(CMAKE_C_COMPILER_ID STREQUAL "Clang")
       	target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -Weverything -Wno-disabled-macro-expansion) 
	message("CLANG IS USING")
endif()


enable_testing()
add_test(ring_buffer_test test/ring_buffer/ring_buffer_test)

