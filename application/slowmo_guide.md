# How to produce a slow motion video

There is an existing software to produce slow motion video based on optical flow. However, it uses the Farneback algorithm in OpenCV (old algorithm) and the flow file is stored in different format.

We wrote a [tool](https://github.com/zeruniverse/CS205-project/tree/master/tools/flo2svflow) to convert deepflow result into sVflow format (used by the slow motion software).

Below is how to use deepflow to generate slow motion videos.

1. Download the open source slowmotion software from their website and follow their instruction to install the software. 
https://github.com/slowmoVideo/slowmoVideo

2. Create a project in slowmotion project. Use their user interface to set the slow rate of the video.

3. Use our [tool](https://github.com/zeruniverse/CS205-project/tree/master/tools/flo2svflow) to convert the .flo files produced by our algorithm to sVflow format.

4. Go to the folder of the slowmotion project. 

5. Add the .sVflow files produced in step 3 to cache/oFlowOrig folder. The slowmoVideo also parses the original video. In most cases it is consisitent with the parsing in our software. To guarantee that frame files match the flow files, you can also replace the frames in frames/orig with the frame files used to produce flow files. 

6. Click "render" in the slowmotion software. It will use our .flo files to produce slow motion videos. The software calls ffmpeg library to produce video. On some platforms the default ffmpeg does not perform well. In this case, you can go to rendered/ folder in the project folder to retrieve the rendered frames and produce the video seperately. 
