#include "CropHandler.h"
#include "Mediator.h"

CropHandler::CropHandler(Mediator* mediator)
{
	m_cur_frames = 1;
	p_mediator = mediator;
	m_need_init = false;
	m_is_stoped = false;
	m_crop_thread = NULL;
	
}

CropHandler::~CropHandler()
{
	//if (p_mediator != NULL)
	//{
	//	p_mediator = NULL;
	//}

	if (m_crop_thread != NULL)
	{
		delete m_crop_thread;
		m_crop_thread = NULL;
	}
	
}

void CropHandler::SetParam(CropParams & cropref)
{
	m_Videoname = cropref.path;
	cv::VideoCapture cap(m_Videoname);

	//test
	myFile_player << m_Videoname.c_str() << endl;
	//test

	int w_reset = 0;
	int h_reset = 0;
	int new_w = 0;
	int new_h = 0;
	m_delta_h = 0;
	m_delta_w = 0;

	//changing what cropdata is pointing to
	cropref.Height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	cropref.Width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	cropref.Fps = cap.get(CV_CAP_PROP_FPS);
	cropref.Frames = cap.get(CV_CAP_PROP_FRAME_COUNT);
	//h264規定 h,w 必須被2整除
	//h,w偶數:直接取小於自己的最大整數
	//h,w奇數"直接取大於自己的最小整數
	double video_h = cropref.Height / (cropref.H_n - (cropref.H_n - 1)*cropref.H_ptx);
	double video_w = cropref.Width / (cropref.W_n - (cropref.W_n - 1)*cropref.W_ptx);
	int temp_h = video_h;
	int temp_w = video_w;

	if (temp_h % 2 == 0)
	{
		cropref.C_h = floor(cropref.Height /
			(cropref.H_n - (cropref.H_n - 1)*cropref.H_ptx));
	}
	else
	{
		cropref.C_h = temp_h + 1;
		h_reset = 1;
	}

	if (temp_w % 2 == 0)
	{
		cropref.C_w = floor(cropref.Width /
			(cropref.W_n - (cropref.W_n - 1)*cropref.W_ptx));
	}
	else
	{
		cropref.C_w = temp_w + 1;
		w_reset = 1;
	}

	//resize original video width,height->四捨五入
	if (h_reset == 1 || w_reset == 1)
	{
		int integer = 0;
		double float_num = 0;
		double ori = 0.0f;
		if (h_reset == 1)
		{
			ori = cropref.C_h*(cropref.H_n - (cropref.H_n - 1)*cropref.H_ptx);
			new_h = Rounding(ori);
		}
		if (w_reset == 1)
		{
			ori = cropref.C_w*(cropref.W_n - (cropref.W_n - 1)*cropref.W_ptx);
			new_w = Rounding(ori);
		}

		if (new_w != 0)
		{
			m_delta_w = (new_w - cropref.Width) / 2;
			cropref.Width = new_w;
		}
		if (new_h != 0)
		{
			m_delta_h = (new_h - cropref.Height);
			cropref.Height = new_h;
		}

	}

	m_Croppath = cropref.Croppath;
	m_FPS = cropref.Fps;
	m_Totalframes = cropref.Frames;
	m_Codec = "libx264";
	m_Container = ".mp4";
	m_CRF = "20";
	ReadPath();
}

