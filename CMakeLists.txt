cmake_minimum_required(VERSION 3.10)

project(cpu_usage_tracker)
set(CMAKE_C_STANDARD 99)

add_subdirectory(test/ring_buffer)

if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
	add_compile_options(-Wall -Wextra)
	message("GCC IS USING")
elseif(CMAKE_C_COMPILER_ID STREQUAL "Clang")
	add_compile_options(-Weverything)
	message("CLANG IS USING")
endif()

add_executable(${CMAKE_PROJECT_NAME}
	src/main.c
	src/ring_buffer/ring_buffer.c
	src/reader/reader.c
)


include_directories(
   inc/ring_buffer/
   inc/reader/
)

enable_testing()
add_test(ring_buffer_test test/ring_buffer/ring_buffer_test)

