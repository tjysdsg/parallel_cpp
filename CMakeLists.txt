cmake_minimum_required(VERSION 3.0)
project(thran)

set(CMAKE_CXX_STANDARD 17)

add_executable(sharing_memory sharing_memory.cpp)
add_executable(aba_problem aba_problem.cpp)

find_package(OpenMP REQUIRED)
add_executable(openmp_parallel_for openmp_parallel_for.cpp)
target_link_libraries(openmp_parallel_for PRIVATE OpenMP::OpenMP_CXX)
