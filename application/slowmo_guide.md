# How to produce a slow motion video

There is an existing software to produce slow motion video based on optical flow. However, it uses the Farneback algorithm in OpenCV (old algorithm) and the flow file is stored in different format.

We wrote a tool to convert deepflow result into sVflow format (used by the slow motion software).

Below is how to use deepflow to generate slow motion videos.
