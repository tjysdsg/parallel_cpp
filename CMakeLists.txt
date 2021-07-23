cmake_minimum_required(VERSION 3.0)
project(thran)

set(CMAKE_CXX_STANDARD 17)

add_executable(sharing_memory sharing_memory.cpp)
add_executable(aba_problem aba_problem.cpp)

find_package(OpenMP REQUIRED)
add_executable(openmp openmp.cpp)
target_link_libraries(openmp PRIVATE OpenMP::OpenMP_CXX)
