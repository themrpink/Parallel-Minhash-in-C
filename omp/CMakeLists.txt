cmake_minimum_required(VERSION 3.17)
project(omp C)

find_package(OpenMP)

if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()


add_executable(omp main.c documents_getters.c get_similarities.c hash_FNV_1.c tokenizer.c  shingle_extract.c time_test.c)