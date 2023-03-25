cmake_minimum_required(VERSION 3.21)
project(traceroute C)

set(CMAKE_C_STANDARD 17)

add_executable(traceroute traceroute.c send.c receive.c traceroute.h send.h receive.h)
