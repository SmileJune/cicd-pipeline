cmake_minimum_required(VERSION 3.27)
project(cicd_pipeline C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(cicd_pipeline
        cal.c
        cal.h
        main.c)
