cmake_minimum_required(VERSION 3.0)
project(parallel_cpp CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-microsoft-enum-value")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

# google benchmark
set(BENCHMARK_ENABLE_TESTING OFF)
add_subdirectory(depend/benchmark)

add_executable(sharing_memory sharing_memory.cpp)
add_executable(aba_problem aba_problem.cpp)

find_package(OpenMP 5.0 REQUIRED)

add_executable(openmp_parallel_for openmp_parallel_for.cpp)
target_link_libraries(openmp_parallel_for PRIVATE OpenMP::OpenMP_CXX)

add_executable(openmp_simd openmp_simd.cpp)
target_compile_options(openmp_simd PRIVATE -fno-slp-vectorize)
target_link_libraries(openmp_simd PRIVATE OpenMP::OpenMP_CXX benchmark::benchmark)
