time ./cmake-build-debug/DeepFlow_mpi ./samples/1.ppm ./samples/2.ppm ./samples/my_output_mpi.flo -match ./samples/forward.match
../flow-code/color_flow ./samples/my_output_mpi.flo ./samples/my_output_mpi.png
diff -b ./samples/my_output_mpi.flo ./samples/correct_mpi_2.flo
