rm -rf build
mkdir build
cd build
pgcc -acc -Minfo -Mlarge_arrays ../deepflow2_acc.c ../src_acc/*.c -lm -ljpeg -lpng -ta=tesla:cc50 -o deepflow2 