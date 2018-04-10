time ./cmake-build-debug/DeepFlow ./samples/1.ppm ./samples/2.ppm ./samples/my_output.flo -match ./samples/forward.match
diff -b ./samples/my_output.flo ./samples/forward.flo
