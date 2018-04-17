We present two applications of deepflow on videos. Our parallelization of deepflow can make those tasks faster.

For both applications, we use [a bullet video](https://vimeo.com/265083949) as example.

## Slow Motion

With slow motion cameras, we can track objects with high speed (such as bullet). However, we might want the captured video to go even slower so we can see the details more clearly.

An intuitive way to further slow down the video is simply reducing the FPS, which has negative impact on the video quality. Another way is to interpolate fake frames between each pair of original frames using optical flow, and the resulting video will look much more smooth.

We use deepflow to generate optical flow and [here](https://vimeo.com/265087981) is the interpolation result (1 fake frame between each pair so it's 2x slow down).

We also tried more aggressive interpolation, and [here](https://vimeo.com/265113420) is the 10x slower video (if you simply reduce FPS, the resulting video will look very uncontinuous, although the interpolation has some problems as well).

You can try your own video or reproduce our results by following [this guide](slowmo_guide.md)

## Style Transfer for Video

We can use deep learning to apply the style of one picture to another. For example, by appling the style of Vincent van Gogh's paint to your selfie, you will get your portrait painted by him.

If we want to transfer the style of a video, we can simply process frame by frame. However, the resulting video will be very noisy due to the loss of temporal consistency. Incorporating temporal loss in the loss function will fix the noisiness and the temporal loss can be calculated using optical flow.

There's an [existing implementation](https://github.com/manuelruder/artistic-videos) of video style transfer and this implementation uses deepflow to calculate the optical flow between frames. The stylization of videos is very slow and companies like [deepart](https://deepart.io/video/) charges 99 euros (~US$123) for stylizing a 10-second video.

We present a [stylized bullet video](https://vimeo.com/265086962) using the paint [Starry Night](https://en.wikipedia.org/wiki/The_Starry_Night#/media/File:Van_Gogh_-_Starry_Night_-_Google_Art_Project.jpg).

You can try your own video or reproduce our results by following [this guide](stylize_guide.md)
