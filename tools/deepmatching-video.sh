#!/bin/bash
#SBATCH --job-name=deepmatch
#SBATCH --output=deepmatch.txt
#SBATCH --ntasks=10
#SBATCH --mem 40960M
#SBATCH --time=71:59:59

export OMP_NUM_THREADS=10

filePattern="out/frame_%06d.ppm"
startFrame=1
endFrame=9999

i=$[$startFrame]
j=$[$startFrame + 1]

mkdir -p match

while true; do
  file1=$(printf "$filePattern" "$i")
  file2=$(printf "$filePattern" "$j")
  if [ -a $file2 ] && [ "$j" -le "$endFrame" ]; then
    if [ ! -f match/forward_${i}_${j}.match ]; then
      ./deepmatching "$file1" "$file2" -nt 10 > "match/forward_${i}_${j}.match"
    fi
    if [ ! -f match/backward_${j}_${i}.match ]; then
      ./deepmatching "$file2" "$file1" -nt 10 > "match/backward_${j}_${i}.match"
    fi
    echo "Completed match between ${i} and ${j}"
  else
    break
  fi
  i=$[$i + 1]
  j=$[$j + 1]
done
