## CI/CD 프로세스 설계 과제

간단한 C 프로그램을 이용하여 전체 프로세스와 내부 파이프 라인을 구축하라.

**************************구현 예정 리스트**************************

1. 브랜치 관리
    1. main - 운영 서버 코드
    2. develop - 개발 서버 코드
    3. feature - 개발 중인 코드
2. PR시 Lint 체크
3. PR시 Unit Test 및 Test Report
4. MERGE시 브랜치에 맞는 서버에 배포
5. 이슈 발생 시 이전 버전으로 다시 배포하는 페일오버 전략 구축
---

## 작업 로그

**cal.c**

```c
#include "cal.h"

int add(int a, int b) {
    return a + b;
}
```

해당 함수에 해당하는 테스트 코드를 작성했다.

간단한 덧셈 함수에 대한 테스트만 작성할 것이라 간단한 assert 함수를 포함하는 가벼운 C 테스트 프레임워크인 **CUnit**을 사용했다.

**test_cal.c**

```yaml
#include <CUnit/Basic.h>
#include "cal.h"

void test_add(void) {
    CU_ASSERT(add(0, 0) == 0);
    CU_ASSERT(add(2, 2) == 4);
    CU_ASSERT(add(-1, 1) == 0);
}

int main() {
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("Suite_1", 0, 0);

    CU_add_test(suite, "test of add()", test_add);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return CU_get_error();
}
```

## PR Test [main, develop]

**test.yml**

```yaml
name: PR Test

on:
  pull_request:
    branches: [ main, develop ]

jobs:
  build-and-test:
    runs-on: ubuntu-latest

    steps:
      - uses: actions/checkout@v4

      - name: Install dependencies
        run: sudo apt-get install -y libcunit1-dev gcc lcov

      - name: Build and Test
        run: |
          chmod +x build.sh
          ./build.sh
          cd build
          ./cicd_pipeline_test

      - name: Generate Coverage info
        run: |
          lcov --capture --directory build --output-file coverage.info
          lcov --list coverage.info

      - name: Coverage Report
        uses: romeovs/lcov-reporter-action@v0.2.16
        with:
          lcov-file: coverage.info
          github-token: ${{ secrets.PAT }}
```

**build.sh**

```bash
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
```

**CMakeLists.txt**

```
cmake_minimum_required(VERSION 3.27)
project(cicd_pipeline C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "--coverage")

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
```
<img width="932" alt="스크린샷 2024-03-06 오전 12 36 34" src="https://github.com/SmileJune/cicd-pipeline/assets/91049936/4366da78-04d1-445d-82ef-431ef721ef67">

**더 생각해봐야 할 점**

<img width="650" alt="스크린샷 2024-03-06 오전 12 22 41" src="https://github.com/SmileJune/cicd-pipeline/assets/91049936/58d120c9-9688-4eae-ac81-1a1dc87935b1">

[Caching dependencies to speed up workflows - GitHub Docs](https://docs.github.com/en/actions/using-workflows/caching-dependencies-to-speed-up-workflows)

## Lint

https://github.com/wearerequired/lint-action

```yaml
name: Lint

on:
  pull_request:
    branches: [ main, develop ]

permissions:
  checks: write
  contents: write

jobs:
  run-linters:
    name: Run linters
    runs-on: ubuntu-latest

    steps:
      - name: Check out Git repository
        uses: actions/checkout@v3

      - name: Install ClangFormat
        run: sudo apt-get install -y clang-format

      - name: Run linters
        uses: wearerequired/lint-action@v2
        with:
          clang_format: true
```

## **Release Versioning**

main 브랜츠로 머지가 됐을 때 올바르게 배포가 됐다면 해당 버전을 Github Release로 버저닝한다.

**main_deploy.yml**

```yaml
name: main deploy

on:
  push:
    branches: [ main ]

jobs:
  build-and-deploy:
    runs-on: ubuntu-latest

    steps:
      - uses: actions/checkout@v4

      - name: Install dependencies
        run: sudo apt-get install -y libcunit1-dev gcc

      - name: Build
        run: |
          chmod +x build.sh
          ./build.sh
          cd build

      - name: deploy
        run: |
          cd build
          ./cicd_pipeline

  create-release-version:
    needs: build-and-deploy
    name: Create Release
    runs-on: ubuntu-latest
    steps:
      - name: Checkout code
        uses: actions/checkout@v3

      - name: Create Release
        id: create_release
        uses: actions/create-release@v1
        env:
          GITHUB_TOKEN: ${{ secrets.PAT }}
        with:
          tag_name: ${{ github.run_number }}
          release_name: Release ${{ github.run_number }}
```
<img width="682" alt="스크린샷 2024-03-06 오전 12 59 27" src="https://github.com/SmileJune/cicd-pipeline/assets/91049936/3f4fd71c-eefd-4099-a41f-426f295b3268">
