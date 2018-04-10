time ./cmake-build-debug/DeepFlow_mpi ./samples/1.ppm ./samples/2.ppm ./samples/my_output_omp.flo -match ./samples/forward.match
../tools/flo_visualization/color_flow  ./samples/my_output_omp.flo ./samples/my_output_omp.png
diff -b ./samples/my_output_omp.flo ./samples/correct_omp_2.flo
