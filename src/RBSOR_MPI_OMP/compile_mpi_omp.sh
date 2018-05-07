rm -rf build
mkdir build
cd build
mpicc -fopenmp ../deepflow2_mpi.c ../src/*.c -lm -ljpeg -lpng -o deepflow2 
cd ..
cp build/deepflow2 deepflow
