cmake_minimum_required(VERSION 3.10)

project(cpu_usage_tracker)

set(INC_DIRS
	inc/	
)

set(SRCS
	src/main.c	
)

add_executable(${CMAKE_PROJECT_NAME} ${SRCS})
include_directories(${CMAKE_PROJECT_NAME} ${INC_DIRS})


