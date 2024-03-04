cmake_minimum_required(VERSION 3.27)
project(cicd_pipeline C)

set(CMAKE_C_STANDARD 11)

# pkg-config를 사용하여 CUnit 설정을 찾습니다.
find_package(PkgConfig REQUIRED)
pkg_check_modules(CUNIT REQUIRED cunit)

# CUnit 헤더 파일 위치
include_directories(${CUNIT_INCLUDE_DIRS})

# 실행 파일
add_executable(cicd_pipeline
        cal.c
        cal.h
        main.c)

# 테스트 실행 파일
add_executable(cicd_pipeline_test
        cal.c
        cal.h
        test_cal.c)

# 테스트 실행 파일에 CUnit 라이브러리 링크
target_link_libraries(cicd_pipeline_test ${CUNIT_LIBRARIES})

# 링커에게 CUnit 라이브러리 경로를 알려줍니다.
link_directories(${CUNIT_LIBRARY_DIRS})