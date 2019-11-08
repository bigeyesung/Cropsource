#pragma once
#include "SHMHandler.h"
#include <fstream>
//#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h> 
#include <iostream>
#include <string>
#include <ctime> //count time

// FFmpeg
extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavutil/pixdesc.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}

// OpenCV
#include <opencv2/opencv.hpp>
#include <opencv/cv.h>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace std;
using namespace cv;

class Mediator;
class CropHandler
{
private:
	
	// 2D array for opencv ROI funcs index
	struct Pos
	{
		int x;
		int y;
	};