time ./cmake-build-debug/DeepFlow_rbsor ./samples/1.ppm ./samples/2.ppm ./samples/my_output_rbsor.flo -match ./samples/forward.match
../tools/flo_visualization/color_flow  ./samples/my_output_rbsor.flo ./samples/my_output_rbsor.png
diff -b ./samples/my_output_rbsor.flo ./samples/correct_rbsor_2.flo
