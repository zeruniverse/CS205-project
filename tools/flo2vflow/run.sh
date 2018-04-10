i=1
j=2
convertCommandLine="./converter"

while true; do
  file1="input/forward_${i}_${j}.flo"
  file2="input/backward_${j}_${i}.flo"
  if [ -a $file2 ]; then
    eval $convertCommandLine "$file1" "output/ocv_${i}_${j}.vflo"
    eval $convertCommandLine "$file2" "output/ocv_${j}_${i}.vflo"
    echo "Finish Processing flow between ${i} and ${j}"
  else
    break
  fi
  i=$[$i + 1]
  j=$[$j + 1]
done
