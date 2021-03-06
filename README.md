# Parallelized DeepFlow

[![Build Status](https://travis-ci.org/zeruniverse/CS205-project.svg?branch=master)](https://travis-ci.org/zeruniverse/CS205-project)
![Environment](https://img.shields.io/badge/gcc-4.8+-blue.svg)
![Environment](https://img.shields.io/badge/g++-4.8+-blue.svg)

Team: Shiyu Huang, Hongxiang Qiu, Zeyu Zhao, Zongren Zou

## Project Website

**This README focuses on how to run our code and reproduce our results. For background, analysis, detailed results, and etc. (i.e. the report) see our [project website](https://zeruniverse.github.io/CS205-project/).**

**Table of Contents** 

+ [Introduction](README.md#introduction)
+ [Prepare Data](README.md#prepare-data)
  + [Two Images](README.md#two-images)
  + [Video](README.md#video)
+ [Generate Flow](README.md#generate-flow)
  + [Serial and OpenMP](README.md#serial-and-openmp)
  + [OpenACC](README.md#openacc)
  + [MPI (with or without OpenMP)](README.md#mpi)
  + [MapReduce + OpenMP](https://github.com/zeruniverse/CS205-project/blob/master/src/MapReduce/README.md)
  + [Flow Visualization](README.md#flow-visualization)
  + [Test Case](README.md#test-case)
+ [Applications](README.md#applications)
  + [Video Stylization](README.md#video-stylization)
  + [Slow Motion Video](README.md#slow-motion-video)
+ [References](README.md#references)

## Introduction

[Optical Flow](https://en.wikipedia.org/wiki/Optical_flow) describes the motion of objects between two consecutive images. There're sparse optical flow (flow for feature sets) and dense optical flow (flow for all pixels). [DeepFlow](https://thoth.inrialpes.fr/src/deepflow/) is a dense optical flow algorithm based on [DeepMatching](https://thoth.inrialpes.fr/src/deepmatching/), which calculates dense correspondences between two images (so the flow is more accurate). For images without large displacement, it's actually OK to run deepflow without deep matching. But in our project, we run deep matching for each pair of frames.

Below is a visualization example of the optical flow.

|Images             |  Flow|
|:-------------------------:|:-------------------------:|
|![opt](https://zeruniverse.github.io/CS205-project/img/optflow.gif)  |  ![flo](https://zeruniverse.github.io/CS205-project/img/optflow.jpg)|

(source: https://people.csail.mit.edu/celiu/OpticalFlow/)

For more details, see our [project website](https://zeruniverse.github.io/CS205-project/).

## Prepare Data

As mentioned above, DeepMatching result is a part of the input of DeepFlow. You should download [DeepMatching](https://thoth.inrialpes.fr/src/deepmatching/) and compile it on your machine. The following guide assumes the compiled executable has name `deepmatching` and assumes you are using the OpenMP implementation provided in the official website. For the GPU implementation, the execution command should be similar.

**If you just want to test the software, you can skip this section and use our [sample data](https://github.com/zeruniverse/CS205-project/releases/tag/0.01).**

### Two Images

Suppose two images have name `1.jpg` and `2.jpg`. To generate `forward_1_2.match`, you should run command:

```bash
./deepmatching 1.jpg 2.jpg -nt 10 > forward_1_2.match
```

`-nt 10` means running with 10 threads.

For your convenience, we provide sample images [ak1.ppm](https://github.com/zeruniverse/CS205-project/releases/download/0.01/ak1.ppm), [ak2.ppm](https://github.com/zeruniverse/CS205-project/releases/download/0.01/ak2.ppm) and the `ak1` to `ak2` match [ak_forward.match](https://github.com/zeruniverse/CS205-project/releases/download/0.01/ak_forward.match).

### Video

You need [ffmpeg](https://en.wikipedia.org/wiki/FFmpeg) to extract frames from a video. You can install `ffmpeg` in Ubuntu with `apt-get` command, similar in other Unix-like systems.

Suppose you have video `example.mp4`, and you want to extract frames into `out` folder. 

```bash
mkdir out
ffmpeg -i example.mp4 out/frame_%06d.ppm
```

You can also extract the first 180 seconds and change the output resolution to `640x360`. The command is:

```bash
mkdir out
ffmpeg -i bunny.mp4 -t 180 -vf scale=640:360 out/frame_%06d.ppm
```

Then you can use [our bash script](tools/deepmatching-video.sh) to generate all forward and backward matches for all pairs of frames. This script is HPC-ready, you can `sbatch deepmatching-video.sh` in Slurm HPC. The matches will be in `match` folder.

For your convenience, we provide sample video frames ([360p](https://github.com/zeruniverse/CS205-project/releases/download/0.01/bunny_640x360_frame.zip), [720p](https://github.com/zeruniverse/CS205-project/releases/download/0.01/bunny_1280x720_first999_frame.zip), [1080p](https://github.com/zeruniverse/CS205-project/releases/download/0.01/nyc_1920x1080_frame.zip)) and corresponding pairwise matchings ([360p](https://github.com/zeruniverse/CS205-project/releases/download/0.01/bunny_640x360_match.zip), [720p](https://github.com/zeruniverse/CS205-project/releases/download/0.01/bunny_1280x720_match.zip), [1080p](https://github.com/zeruniverse/CS205-project/releases/download/0.01/nyc_1920x1080_match.zip)). In addition, we have a very short 720p bullet video sample ([frames](https://github.com/zeruniverse/CS205-project/releases/download/0.01/new_bullet_frame.zip) and [matches](https://github.com/zeruniverse/CS205-project/releases/download/0.01/new_bullet_match.zip)), which is used to generate our [application examples](README.md#applications).

## Generate Flow

This is `how to run our model` part. The guide assumes you use AWS m4.2xlarge node (AWS g3.4xlarge for OpenACC) with Ubuntu Server 16.04 LTS (HVM).

We have a seperate guide to run MapReduce, see [here](https://github.com/zeruniverse/CS205-project/blob/master/src/MapReduce/README.md)

If you followed the previous section or downloaded our samples, you should now have:

+ For a pair of image
  + Two image files (our sample is `ak1.ppm`, `ak2.ppm`)
  + One match file (our sample is `ak_forward.match`)
+ For a video
  + A folder named `out` containing all frames
  + A folder named `match` containing all matches
  
The command to download our sample data is:

```bash
wget https://github.com/zeruniverse/CS205-project/releases/download/0.01/ak1.ppm
wget https://github.com/zeruniverse/CS205-project/releases/download/0.01/ak2.ppm
wget https://github.com/zeruniverse/CS205-project/releases/download/0.01/ak_forward.match
wget https://github.com/zeruniverse/CS205-project/releases/download/0.01/bunny_640x360_frame.zip
wget https://github.com/zeruniverse/CS205-project/releases/download/0.01/bunny_640x360_match.zip
unzip bunny_640x360_frame.zip
unzip bunny_640x360_match.zip
```

First, you should install required packages:

```bash
sudo apt-get update
sudo apt-get install -y cmake
sudo apt-get install -y build-essential
sudo apt-get install -y libjpeg-dev
sudo apt-get install -y libpng-dev
sudo apt-get install -y unzip
```

If you are using other (old) systems, please ensure you have new `gcc` and `g++`, otherwise, you might get compile error of `error: ‘for’ loop initial declarations are only allowed in C99 mode`.

### Serial and OpenMP

After completing above steps, you can compile DeepFlow. Here, we use `RBSOR_OMP` implementation as example, all `*_OMP` and `*_serial` implementations under `src` can be compiled and executed in the same manner.

Compile it:

```bash
git clone https://github.com/zeruniverse/CS205-project.git
cd CS205-project/src/RBSOR_OMP
mkdir build
cd build
cmake ..
make

# Now you have the executable deepflow2, move it to the place of your data
mv deepflow2 ../../../..
cd ../../../..
```

#### Generate Flow for Two images

The command below generates the flow `flow.flo` and output the execution time.

```bash
export OMP_NUM_THREADS=4
time ./deepflow2 ak1.ppm ak2.ppm flow.flo -match ak_forward.match
```

#### Generate Flow for Video

You can use the `video_flow.sh` in each implementation folder. Please note you should have `out` and `match` folder containing the video data.

```bash
# copy video_flow.sh here
cp CS205-project/src/RBSOR_OMP/video_flow.sh .
bash video_flow.sh
```
In the terminal, you can see accumulated time cost after the completion of each pair of frames. The flow results are stored in the `flow` folder.

### OpenACC

First follow the course guide to install `pgcc` in g3.4xlarge node and then install libraries described above.

We use `RBSOR_ACC` implementation as example, all `*_ACC` can be compiled and executed in the same manner.

Compile it:

```bash
cd CS205-project/src/RBSOR_ACC
bash compile_acc.sh
```

Copy the executable (under `build`) and `video_flow.sh` to the place of your data, running commands for images and videos are same with `[Serial and OpenMP](README.md#generate-flow-for-two-images)`.

### MPI

First follor the course guide to install `mpicc` and then install libraries described above.

We use `RBSOR_MPI` implementation as example. `RBSOR_MPI_OMP` can be compiled and executed similarly.

Compile:
```
cd CS205-project/src/RBSOR_MPI
bash compile_mpi.sh
```

Copy the executable (under `build`) and `video_flow.sh` to the place of your data, and use commands `mpirun -np 4` to run the program, while 4 is the number of threads you want to use (for video, simply run `video_flow.sh`). Notice that it is a guide for running this program on a single instance.

### Flow Visualization

To check the correctness of the generated flows, you might want to use [this tool](tools/flo_visualization) to visualize them.

```bash
cd tools/flo_visualization/imageLib
make
cd ..
make
./color_flow path/to/flo vis.png
```

`vis.png` is the visualization file.

### Test Case

Below is a complete test case for 2 images (OpenMP). For videos, the command should be similar (actually easier) because you just run bash file we provided (see above).

Before running the following commands in your terminal, make sure you have latest `cmake`, `gcc` and `g++` with libraries at the beginning of this section.

```bash
git clone https://github.com/zeruniverse/CS205-project.git
cd CS205-project/tools/flo_visualization/imageLib
make
cd ..
make
cp color_flow ../../src/RBSOR_OMP/
cd ../../src/RBSOR_OMP/
mkdir build
cd build
cmake ..
make
cp deepflow2 ../
cd ..
export OMP_NUM_THREADS=4
wget https://github.com/zeruniverse/CS205-project/releases/download/0.01/ak1.ppm
wget https://github.com/zeruniverse/CS205-project/releases/download/0.01/ak2.ppm
wget https://github.com/zeruniverse/CS205-project/releases/download/0.01/ak_forward.match
time ./deepflow2 ak1.ppm ak2.ppm flow.flo -match ak_forward.match
./color_flow flow.flo flow.png
ls
```
the `ls` command should give you `flow.png` (`CS205-project/src/RBSOR_OMP/flow.png`). This is the visualization of the flow between `ak1.ppm` and `ak2.ppm`. Check it with any image viewer of your choice. It should look the same with the following:

![flow](https://user-images.githubusercontent.com/4648756/39793174-4d05bf58-5312-11e8-835b-9eb4d83ef35e.png)

## Applications

We present two applications of deepflow on video which will benefit from the parallelization.

### Video Stylization

Video stylization tries to apply the style of a paint to a video and DeepFlow is used to keep temporal consistency. The [existing implementation](https://github.com/manuelruder/artistic-videos) uses DeepFlow by default so simply replacing the deepflow executable with our parallelized executable will work. A short guide is [here](application/stylize_guide.md).

We provide a [sample video](https://youtu.be/UIuqtqeNiU8) and its [stylized result](https://youtu.be/-d9m3iaYs7A) with the paint [Starry Night](https://en.wikipedia.org/wiki/The_Starry_Night#/media/File:Van_Gogh_-_Starry_Night_-_Google_Art_Project.jpg).

### Slow Motion Video

Fake slow motion video can be generated by interpolating in-between frames using optical flow. [SlowmoVideo](https://github.com/slowmoVideo/slowmoVideo) is a software to generate such videos. However, it uses Farneback algorithm. To use DeepFlow, you need our [tool](tools/flo2svflow) and then follow [this guide](application/slowmo_guide.md).

We provide a [sample video](https://youtu.be/UIuqtqeNiU8) and its [slowed-down version](https://youtu.be/v-gCcF4bAHo) generated by SlomoVideo and DeepFlow.

## References

[DeepMatching: Hierarchical Deformable Dense Matching (IJCV 2016)](https://hal.inria.fr/hal-01148432v2/file/paper.pdf)

[DeepFlow: Large displacement optical flow with deep matching (ICCV 2013)](http://hal.inria.fr/hal-00873592)

[DeepMatching](https://thoth.inrialpes.fr/src/deepmatching/) and [DeepFlow](https://thoth.inrialpes.fr/src/deepflow/) code

[SlowmoVideo](https://github.com/slowmoVideo/slowmoVideo)

[Artistic Style Transfer for Videos](https://github.com/manuelruder/artistic-videos)

[Iterative Methods for Solving Ax = b - The SOR Method](https://www.maa.org/press/periodicals/loci/joma/iterative-methods-for-solving-iaxi-ibi-the-sor-method)

[Parallel SOR Iterative Algorithms and Performance Evaluation on a Linux Cluster](http://www.dtic.mil/dtic/tr/fulltext/u2/a449212.pdf)
