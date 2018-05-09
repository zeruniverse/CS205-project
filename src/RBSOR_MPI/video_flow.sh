filePattern="out/frame_%06d.ppm"
startFrame=1
endFrame=250

i=$[$startFrame]
j=$[$startFrame + 1]

mkdir -p flow

START=$(date +%s)

while true; do
  file1=$(printf "$filePattern" "$i")
  file2=$(printf "$filePattern" "$j")
  if [ -a $file2 ] && [ "$j" -le "$endFrame" ]; then
    if [ ! -f flow/forward_${i}_${j}.flo ]; then
      mpirun -np 4 ./deepflow2 "$file1" "$file2" "flow/forward_${i}_${j}.flo" -match "match/forward_${i}_${j}.match"
    fi
    if [ ! -f flow/backward_${j}_${i}.flo ]; then
      mpirun -np 4 ./deepflow2 "$file2" "$file1" "flow/backward_${j}_${i}.flo" -match "match/backward_${j}_${i}.match"
    fi
    echo "Finish Processing flow between ${i} and ${j}"
    NEWT=$(date +%s)
    DIFF=$(( $NEWT - $START ))
    echo "$DIFF seconds (accumulated)"
  else
    break
  fi
  i=$[$i + 1]
  j=$[$j + 1]
done