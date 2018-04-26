rm -rf build
mkdir build
cd build
mpicc ../deepflow2_mpi.c ../src/*.c -lm -ljpeg -lpng -o deepflow2 
cd ..
