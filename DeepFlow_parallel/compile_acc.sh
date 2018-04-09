rm -rf build
mkdir build
cd build
cmake .. -DCMAKE_C_COMPILER=gcc-7
make DeepFlow_acc
