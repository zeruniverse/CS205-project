rm -rf build
mkdir build
cd build
cmake .. -DCMAKE_C_COMPILER=pgcc
make DeepFlow_acc
