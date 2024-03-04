#!/bin/bash

# build 디렉토리의 존재 여부를 확인하고, 존재하면 삭제
if [ -d "build" ]; then
    echo "Removing existing build directory..."
    rm -rf build
fi

# 새로운 build 디렉토리 생성
echo "Creating new build directory..."
mkdir build
cd build

# CMake를 사용하여 프로젝트 구성 및 빌드
echo "Configuring and building project with CMake..."
cmake ..
make

echo "Build completed."
