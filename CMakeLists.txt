cmake_minimum_required(VERSION 3.27)
project(cicd_pipeline C)

set(CMAKE_C_STANDARD 11)

# CUnit 헤더 파일 위치를 직접 지정
include_directories(/opt/homebrew/Cellar/cunit/2.1-3/include)

add_executable(cicd_pipeline
        cal.c
        cal.h
        main.c)

# CUnit 라이브러리 위치 및 헤더 파일을 설정
set(CUNIT_INCLUDE_DIR "/opt/homebrew/Cellar/cunit/2.1-3/include")
set(CUNIT_LIBRARY "/opt/homebrew/Cellar/cunit/2.1-3/lib/libcunit.a")

# 테스트 실행 파일에 대한 설정
add_executable(cicd_pipeline_test
        cal.c
        cal.h
        test_cal.c) # 테스트 코드 파일 추가

# 테스트 실행 파일에 CUnit 라이브러리 링크
target_link_libraries(cicd_pipeline_test ${CUNIT_LIBRARY})
