rm -rf build
mkdir build
cd build
mpicc -O3 -std=gnu11 ../deepflow2_mpi.c ../src/*.c -lm -ljpeg -lpng -o deepflow2 
cd ..
