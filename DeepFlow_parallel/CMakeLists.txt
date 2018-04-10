cmake_minimum_required(VERSION 3.5)
project(DeepFlow)

set(CMAKE_CXX_STANDARD 11)

set(PG FALSE)
set(USE_MPI FALSE)

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

add_executable(DeepFlow
        deepflow2.c
        src/image.c
        src/io.c
        src/opticalflow.c
        src/opticalflow_aux.c
        src/solver.c)
target_link_libraries(DeepFlow ${JPEG_LIBRARY})
target_link_libraries(DeepFlow ${PNG_LIBRARY})
target_link_libraries(DeepFlow m)

find_package(OpenMP REQUIRED)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")


add_executable(DeepFlow_omp
        deepflow2_mpi.c
        src_omp/image_omp.c
        src/io.c
        src_omp/opticalflow_omp.c
        src_omp/opticalflow_aux_omp.c
        src_omp/solver_omp.c
        src_omp/omp.h)
target_link_libraries(DeepFlow_omp ${JPEG_LIBRARY})
target_link_libraries(DeepFlow_omp ${PNG_LIBRARY})
target_link_libraries(DeepFlow_omp m)
if (USE_MPI)
    find_package(MPI REQUIRED)
    include_directories(${MPI_INCLUDE_PATH})
    set_target_properties(DeepFlow_mpi PROPERTIES
            COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
    set_target_properties(DeepFlow_mpi PROPERTIES
            LINK_FLAGS "${MPI_LINK_FLAGS}")
endif ()