cmake_minimum_required(VERSION 3.10)

add_subdirectory(../Unity unity)

set(INC_DIRS
	../../inc/ring_buffer	
)

set(SRCS
	../../src/ring_buffer/ring_buffer.c
)

set(TEST_SRCS
	ring_buffer_main.c
	ring_buffer_runner.c
	ring_buffer_test.c
)

add_executable(ring_buffer_test ${SRCS} ${TEST_SRCS})
target_include_directories(ring_buffer_test PRIVATE ${INC_DIRS})
target_compile_options(ring_buffer_test PRIVATE -Wall -Wextra)
target_link_libraries(ring_buffer_test PRIVATE unity)
