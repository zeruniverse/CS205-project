/*
This file is part of slowmoVideo.
Copyright (C) 2012  Lucas Walter
              2012  Simon A. Eugster (Granjow)  <simon.eu@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
*/

#include <opencv2/opencv.hpp>
#include "flowRW_sV.h"
#include "flowField_sV.h"

#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;
#define TAG_FLOAT 202021.25

/**
 *  create a optical flow file
 *
 *  @param flow     optical flow to save
 *  @param flowname file name for optical flow
 */
void drawOptFlowMap(const Mat& flow, std::string flowname)
{
  //cout<<"draw"<<flow.cols<<'a'<<flow.rows<<endl;
  FlowField_sV flowField(flow.cols, flow.rows);
  //qDebug() << "flow is : " << flow.cols << " by " << flow.rows;
  //cout<<flow.rows<<"\t"<<flow.cols<<endl;
  for(int y = 0; y < flow.rows; y++)
        for(int x = 0; x < flow.cols; x++) {
            //cout<<"x="<<x<<"   y="<<y<<endl;
            const Point2f& fxyo = flow.at<Point2f>(y, x);
            flowField.setX(x, y, fxyo.x);
            flowField.setY(x, y, fxyo.y);
        }
  
  FlowRW_sV::save(flowname, &flowField);
}

struct CError : public exception
{
    CError(const char* msg)                 { strcpy(message, msg); }
    CError(const char* fmt, int d)          { sprintf(message, fmt, d); }
    CError(const char* fmt, float f)        { sprintf(message, fmt, f); }
    CError(const char* fmt, const char *s)  { sprintf(message, fmt, s); }
    CError(const char* fmt, const char *s,
            int d)                          { sprintf(message, fmt, s, d); }
    char message[1024];         // longest allowable message
};

void ReadFlowFile(Mat **img, const char* filename)
{
    if (filename == NULL)
  throw CError("ReadFlowFile: empty filename");

    const char *dot = strrchr(filename, '.');
    if (strcmp(dot, ".flo") != 0)
  throw CError("ReadFlowFile (%s): extension .flo expected", filename);

    FILE *stream = fopen(filename, "rb");
    if (stream == 0)
        throw CError("ReadFlowFile: could not open %s", filename);
    
    int width, height;
    float tag;

    if ((int)fread(&tag,    sizeof(float), 1, stream) != 1 ||
  (int)fread(&width,  sizeof(int),   1, stream) != 1 ||
  (int)fread(&height, sizeof(int),   1, stream) != 1)
  throw CError("ReadFlowFile: problem reading file %s", filename);

    if (tag != TAG_FLOAT) // simple test for correct endian-ness
  throw CError("ReadFlowFile(%s): wrong tag (possibly due to big-endian machine?)", filename);

    // another sanity check to see that integers were read correctly (99999 should do the trick...)
    if (width < 1 || width > 99999)
  throw CError("ReadFlowFile(%s): illegal width %d", filename, width);

    if (height < 1 || height > 99999)
  throw CError("ReadFlowFile(%s): illegal height %d", filename, height);
    *img = new cv::Mat(height,width, CV_64FC2);

    //printf("reading %d x %d x 2 = %d floats\n", width, height, width*height*2);
    int n = 2 * width;
    for (int y = 0; y < height; y++) {
      for (int x=0; x<width;x++) {
  if ((int)fread(&((*img)->at<Point2f>(y, x)), sizeof(float), 2, stream) != 2)
      throw CError("ReadFlowFile(%s): file is too short", filename);
    }
}
    if (fgetc(stream) != EOF)
  throw CError("ReadFlowFile(%s): file is too long", filename);
    fclose(stream);
    
}

int main(int argc, char** argv){
 
    Mat *input_flow;
    ReadFlowFile(&input_flow, argv[1]);
    //namedWindow( "Display window", WINDOW_AUTOSIZE );
    //imshow("hello", input_flow);
    //cout<<"main"<<input_flow->cols<<'a'<<input_flow->rows<<endl;
    drawOptFlowMap(*input_flow, argv[2]);
    //waitKey(0);             // Wait for a keystroke in the window
    return 0;
}
