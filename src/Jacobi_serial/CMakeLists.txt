cmake_minimum_required(VERSION 3.5)
project(DeepFlow)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 11)
set(PG FALSE)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O3 -fPIC -Wall -msse4")

if (PG)
    message("Profiling mode")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pg")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")
endif ()

find_package(JPEG REQUIRED)
find_package(PNG REQUIRED)
include_directories(${JPEG_INCLUDE_DIR})
include_directories(${PNG_INCLUDE_DIR})

add_executable(deepflow2
        deepflow2.c
        src/image.c
        src/io.c
        src/opticalflow.c
        src/opticalflow_aux.c
        src/solver.c
        src/malloc_impl.c)
target_link_libraries(deepflow2 ${JPEG_LIBRARY})
target_link_libraries(deepflow2 ${PNG_LIBRARY})
target_link_libraries(deepflow2 m)
