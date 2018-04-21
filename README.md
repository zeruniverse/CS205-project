# Parallelized Deepflow

Team: Shiyu Huang, Hongxiang Qiu, Zeyu Zhao, Zongren Zou

**Table of Contents** 

+ [Introduction](README.md#introduction)
+ [Prepare Data](README.md#prepare-data)
  + [Two Images](README.md#two-images)
  + [Video](README.md#video)
+ [Implementations](README.md#implementations)
+ [Applications](README.md#applications)
  + [Video Stylization](README.md#video-stylization)
  + [Slow Motion Video](README.md#slow-motion-video)
+ [References](README.md#references)

## Introduction

[Optical Flow](https://en.wikipedia.org/wiki/Optical_flow) describes the motion of objects between two consecutive images. There're sparse optical flow (flow for feature sets) and dense optical flow (flow for all pixels). [DeepFlow](https://thoth.inrialpes.fr/src/deepflow/) is a dense optical flow algorithm based on [DeepMatching](https://thoth.inrialpes.fr/src/deepmatching/), which calculates dense correspondences between two images (so the flow is more accurate). For images without large displacement, it's actually OK to run deepflow without deep matching. But in our project, we run deep matching for each pair of frames.

Below is a visualization example of the optical flow.

|Images             |  Flow|
|:-------------------------:|:-------------------------:|
|![opt](https://people.csail.mit.edu/celiu/OpticalFlow/car_input.gif)  |  ![flo](https://people.csail.mit.edu/celiu/OpticalFlow/car_flow.jpg)|

(source: https://people.csail.mit.edu/celiu/OpticalFlow/)


## Prepare Data

As mentioned above, Deepmatching result is a part of the input of the deepflow.
### Two Images




## Implementations

## Applications

### Video Stylization

### Slow Motion Video

## References
