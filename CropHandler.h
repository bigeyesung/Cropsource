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
    double m_cur_frames;					//���e�B�z��frame index
	int m_Width;
	int m_Height;
	int m_Width_ptx;
	int m_Height_ptx;
	int m_Width_num;
	int m_Height_num;
	int m_FPS;
	int m_Totalframes;
	int m_Crop_status;						//0: �ǳƤ���,1: ���b����, 2: ���Χ���
	int m_delta_w;							//left, right�W�[pixel ��
	int m_delta_h;							//top, bottom�W�[pixel ��
	int m_left, m_right, m_top, m_bottom;	//openCV::copyMakeBorder �Ѽ�
	AVCodecID m_Codec;