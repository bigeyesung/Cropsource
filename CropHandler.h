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

    Mediator* p_mediator;
	std::string m_CRF;
	std::string m_Videoname;
	std::vector<std::string> m_paths;		 //�s����θ��|
	boost::thread* m_crop_thread;			//����thread
	boost::mutex m_mtx_;					//mutex lock unlock
	bool m_Thread_finish;
	bool m_is_stoped;						//���_flag default = false;
	bool m_need_init;						//true: init the class ,false: class is running/ no need to init

public:
	explicit CropHandler(Mediator* mediator);
	~CropHandler();
	//void SetParam(CropParams& cropref);
	bool CheckInit();                               //check class working 
	bool CheckCropData();							//
	bool is_file_exist(const char * fileName);		//
	void SetParam();								//
	void SetCropStatus(int status);					//
	void SetInit(bool needinit);
	void SetcopyMakeBorderParams(int& delta_w,int& delta_h);//openCV mat�e����j��
	void GetCropfilepath();							//
	void CropVideo();								//�(��thread)

	//
	bool ffmpegInitParam(AVFormatContext *& ofmt_ctx, AVCodec *& out_codec, AVStream *& out_stream,
		AVCodecContext *& out_codec_ctx, string & output_path, SwsContext *&swsctx, AVFrame *&frame,
		int output_w, int output_h);
	void CropVideo2();
	void StartCrop();								//(�Dthread)
	void init();
	void clear();
	int GetCropStatus();							//crop
	int Rounding(double ori);						//
	cv::VideoCapture GetVideo();

    //-----FFmpeg funcs--------//
	//�]�wAVformat
	int initialize_avformat_context(AVFormatContext *&fctx, const char * output);
	int initialize_io_context(AVFormatContext *&fctx, const char *output);

    //
	void set_codec_params(AVFormatContext *&fctx, AVCodecContext *&codec_ctx,
		 double width, double height, int fps);
	int initialize_codec_stream(AVStream *&stream, AVCodecContext *&codec_ctx,
		 AVCodec *&codec);
	SwsContext *initialize_sample_scaler(AVCodecContext *codec_ctx,
		double width, double height);

    	// AVFrame��buffer
	AVFrame *allocate_frame_buffer(AVCodecContext *codec_ctx,
		double width, double height);
