i=1
j=2
convertCommandLine="./generator"

while true; do
  file1="input/forward_${i}_${j}.flo"
  file2="input/backward_${j}_${i}.flo"
  if [ -a $file2 ]; then
    ii=$[$i - 1]
    jj=$[$j - 1]
    eval $convertCommandLine "$file1" "output/ocv-forward-$ii-$jj.sVflow"
    eval $convertCommandLine "$file2" "output/ocv-backward-$jj-$ii.sVflow"
    echo "Finish Processing flow between ${i} and ${j}"
  else
    break
  fi
  i=$[$i + 1]
  j=$[$j + 1]
done
