time ./build/DeepFlow_acc ./samples/1.ppm ./samples/2.ppm ./samples/my_output_acc.flo -match ./samples/forward.match
../flow-code/color_flow ./samples/my_output_acc.flo ./samples/my_output_acc.png
diff -b ./samples/my_output_acc.flo ./samples/correct_omp_jacobi.flo
