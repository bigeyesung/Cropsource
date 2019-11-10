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

void CropHandler::SetParam()
{
	CropParams* cropparams = p_mediator->GetSHMhandler()->LoadFromShm();
	m_Videoname = cropparams->path;
	cv::VideoCapture cap(m_Videoname);
	int w_reset = 0;
	int h_reset = 0;
	int new_w = 0;
	int new_h = 0;
	m_delta_h = 0;
	m_delta_w = 0;

    //changing what cropdata is pointing to
	cropparams->Height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	cropparams->Width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	cropparams->Fps = cap.get(CV_CAP_PROP_FPS);
	cropparams->Frames = cap.get(CV_CAP_PROP_FRAME_COUNT);

	//h264規定 h,w 必須被2整除
	//if temp_h,w 偶數:直接取小於自己的最大整數
	//if temp_h,w 奇數:直接取大於自己的最小整數
	double crop_h = cropparams->Height / (cropparams->H_n - (cropparams->H_n - 1)*cropparams->H_ptx);
	double crop_w = cropparams->Width / (cropparams->W_n - (cropparams->W_n - 1)*cropparams->W_ptx);
	int temp_h = crop_h;
	int temp_w = crop_w;
    if (temp_h%2 == 0)
	{
		//cropparams->C_h = floor(cropparams->Height /
		//			   (cropparams->H_n - (cropparams->H_n - 1)*cropparams->H_ptx));
		cropparams->C_h = temp_h;
	}
	else
	{
		cropparams->C_h = temp_h + 1;
		h_reset = 1;
	}
	if (temp_w %2 ==0)
	{
		/*cropparams->C_w = floor(cropparams->Width /
					   (cropparams->W_n - (cropparams->W_n - 1)*cropparams->W_ptx));*/
		cropparams->C_w = temp_w;
	}
    else
	{
		cropparams->C_w = temp_w + 1;
		w_reset = 1;
	}
	
	//
	//重新設定來源影片寬高(四捨五入)
	if (h_reset == 1 || w_reset == 1)
	{
		int integer = 0;
		double float_num = 0;
		double ori = 0.0f;
		if (h_reset == 1)
        	{
			//ex: 新高 = n*切割高-(n-1)*切割高*重疊比
			//切割高*重疊比=浮點數->要先四捨五入避免誤差
			int temp1 = cropparams->H_n*cropparams->C_h;
			int temp2 = (cropparams->H_n - 1)*Rounding(cropparams->C_h*cropparams->H_ptx);
			new_h = temp1 - temp2;
			//ori = cropparams->C_h*(cropparams->H_n - (cropparams->H_n - 1)*cropparams->H_ptx);
			//new_h = Rounding(ori); 
		}
		if (w_reset == 1)
		{
			
			int temp1 = cropparams->W_n*cropparams->C_w;
			int temp2 = (cropparams->W_n- 1)*Rounding(cropparams->C_w*cropparams->W_ptx);
			new_w = temp1 - temp2;
			//ori = cropparams->C_w*(cropparams->W_n - (cropparams->W_n - 1)*cropparams->W_ptx);
			//new_w = Rounding(ori);
		}

		if (new_w != 0)
		{
			m_delta_w = (new_w - cropparams->Width);
			cropparams->Width = new_w;
		}
		if (new_h != 0)
		{
			m_delta_h = (new_h - cropparams->Height);
			cropparams->Height = new_h;
		}

	SetcopyMakeBorderParams(m_delta_w, m_delta_h);

	}

	m_FPS = cropparams->Fps;
	m_Totalframes = cropparams->Frames;
	if (cropparams->Codec == En_H264)
	{
		m_Codec = AV_CODEC_ID_H264;
	}
	else if (cropparams->Codec == En_H265)
	{
		m_Codec = AV_CODEC_ID_H265;
	}

	m_CRF = to_string(cropparams->CRF);
	cropparams = NULL;
}

void CropHandler::CropVideo()
{
	ofstream myfile;
	myfile.open("time.txt");
	clock_t begin1 = clock();
	clock_t begin = clock();

	//確認SHM資料存在
	if (!CheckCropData())
	 return;
	CropParams* cropparams = p_mediator->GetSHMhandler()->LoadFromShm();

	//設定切割Thread狀態:未完成
	m_mtx_.lock();
	m_Thread_finish = false;
	m_mtx_.unlock();

    vector<vector<Pos>> f_pos;
	f_pos.resize(cropparams->H_n);
	for (int i = 0; i < f_pos.size(); i++)
	{
		f_pos[i].resize(cropparams->W_n);
	}

	//Pos** pos = new Pos*[cropparams->H_n];
	//for (int k = 0; k < cropparams->H_n; k++)
	//	pos[k] = new Pos[cropparams->W_n];
	
	//初始化 FFmpeg library
	av_register_all();

    double Totalframes = cropparams->H_n*cropparams->W_n*cropparams->Frames;
	int w_cover = Rounding(cropparams->W_ptx*cropparams->C_w);
	int h_cover = Rounding(cropparams->H_ptx*cropparams->C_h);
	
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	myfile << "init time: " << elapsed_secs<<endl;
	begin = 0;
	end = 0;

	Mat image;
	Mat ROIimage;
	VideoCapture video_capture;
	video_capture = GetVideo();
	int w_range, h_range;

    vector<AVFormatContext*> ofmt_ctx;
	ofmt_ctx.resize(cropparams->H_n*cropparams->W_n);
	vector<AVCodec*> out_codec;
	out_codec.resize(cropparams->H_n*cropparams->W_n);
	vector<AVStream*> out_stream;
	out_stream.resize(cropparams->H_n*cropparams->W_n);
	vector<AVCodecContext*> out_codec_ctx;
	out_codec_ctx.resize(cropparams->H_n*cropparams->W_n);
	vector<SwsContext*> swsctx;
	swsctx.resize(cropparams->H_n*cropparams->W_n);
	vector<AVFrame*> frame;
	frame.resize(cropparams->H_n*cropparams->W_n);

    int idx = 0;
	//切割loop 開始
	for (int i = 0; i < f_pos.size(); i++)
	{
		if (m_is_stoped)
			break;
		for (int j = 0; j < f_pos[i].size(); j++)
		{
			begin = clock();
			if (m_is_stoped)
				break;
			//設定ROI 位置
			//M = (開始位置+length*幾段 -重疊段 -1)
			//pos = M+1-重疊段
			if (j == 0)
				f_pos[i][j].x = 0;
			else
			{
				int M = 0 + j*cropparams->C_w - (j - 1)*(w_cover)-1;
				f_pos[i][j].x = M + 1 - w_cover;
			}

			if (i == 0)
				f_pos[i][j].y = 0;
			else
			{
				int M = 0 + i*cropparams->C_h - (i - 1)*(h_cover)-1;
				f_pos[i][j].y = M + 1 - h_cover;
			}
			//
			end = clock();
			elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
			myfile << "set pos done: " << elapsed_secs << endl;
			begin = 0;
			end = 0;

			begin = clock();
			//判斷切割位置在原始/放大影片W,H範圍內
			//int w_range, h_range;
			//W
			if (f_pos[i][j].x + cropparams->C_w - 1 > cropparams->Width)
			{
				w_range = cropparams->Width - f_pos[i][j].x;
			}
			else
				w_range = cropparams->C_w;
			//H
			if (f_pos[i][j].y + cropparams->C_h - 1 > cropparams->Height)
			{
				h_range = cropparams->Height - f_pos[i][j].y;
			}
			else
				h_range = cropparams->C_h;

			end = clock();
			elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
			myfile << "check range done: " << elapsed_secs << endl;
			begin = 0;
			end = 0;

			begin = clock();
			const int output_w = w_range;
			const int output_h = h_range;
			av_register_all();
			int ret;
			//auto video_capture1 = GetVideo();
			if (m_is_stoped)
			{
				cropparams->ErrIndex = Fail_open_video_capture;
				break;
			}

            end = clock();
			elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
			myfile << "set mat done:: " << elapsed_secs << endl;
			begin = 0;
			end = 0;

			//設定ffmpeg參數
			begin = clock();

			std::string output_path = m_paths[cropparams->W_n* i + j];
            if (!ffmpegInitParam(ofmt_ctx[idx], out_codec[idx], out_stream[idx], out_codec_ctx[idx], output_path,
				swsctx[idx], frame[idx], output_w, output_h))
			{
				break;
			}

			bool end_of_stream = false;

			end = clock();
			elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
			myfile << "set ffmpeg done:: " << elapsed_secs << endl;
			begin = 0;
			end = 0;

			idx++;
		}
	}//forLoop done
	begin = clock();
	//讀取frame並寫入
	for (int m = 0; m < m_Totalframes; m++)
	{
		if (m_is_stoped)
			break;
		
		//set ROI	
		video_capture >> image;
		//video_capture1 >> Ori_image;
		//如果要放大影片->邊界放大
		if (m_delta_h != 0 || m_delta_w != 0)
		{
			//copyMakeBorder(Ori_image, Changed_image, 0, m_delta_h,
			//	m_delta_w, m_delta_w, BORDER_REPLICATE);
			//image = Changed_image;
			copyMakeBorder(image, image, m_top, m_bottom,
				m_left, m_right, BORDER_REPLICATE);
			//image = Changed_image;
		}
		int idx = 0;
        for (int i = 0; i < f_pos.size(); i++)
		{
			if (m_is_stoped)
				break;
			
			for (int j = 0; j < f_pos[i].size(); j++)
			{
				if (m_is_stoped)
					break;

				cv::Rect Rec(f_pos[i][j].x, f_pos[i][j].y, w_range, h_range);
				ROIimage = image(Rec);

				const int stride[] = { static_cast<int>(ROIimage.step[0]) };
				sws_scale(swsctx[idx], &ROIimage.data, stride, 0, ROIimage.rows,
					frame[idx]->data, frame[idx]->linesize);
				frame[idx]->pts += av_rescale_q(1, out_codec_ctx[idx]->time_base,
					out_stream[idx]->time_base);

				//寫入
				if (!write_frame(out_codec_ctx[idx], ofmt_ctx[idx], frame[idx]))
				{
					break;
				}
				idx++;

                //取得當前進度
				double cur_progress = m_cur_frames / Totalframes;
				cropparams->TotalProgress = cur_progress * 100;
				m_cur_frames++;
				if (m_is_stoped)
					break;

			}
		}
		////取得當前進度
		//double cur_progress = m_cur_frames / Totalframes;
		//cropparams->TotalProgress = cur_progress * 100;
		//m_cur_frames++;
		//if (m_is_stoped)
		//	break;

	}

	end = clock();
	elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	myfile << "crop 1 video done: " << elapsed_secs << endl;
	begin = 0;
	end = 0;

	//清除ffmpeg ptr
	begin = clock();
	for (int i = 0; i < ofmt_ctx.size(); i++)
	{
		av_write_trailer(ofmt_ctx[i]);
		av_frame_free(&frame[i]);
		avcodec_close(out_codec_ctx[i]);
		avio_close(ofmt_ctx[i]->pb);
		if (out_stream[i]->codecpar->extradata != NULL)
		{
			out_stream[i]->codecpar->extradata = NULL;
		}
		avformat_free_context(ofmt_ctx[i]);
	}

	//opencv clear
	image.release();


	end = clock();
	elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	myfile << "clear ffmpeg ptr and opencv done: " << elapsed_secs << endl;
	myfile << "======next=========" << endl;
	begin = 0;
	end = 0;

    	begin = clock();
	 //檔案產生需要等待時間
	Sleep(2000);
	//確認切割檔案都存在
	bool all_exist = true;
	for (int i = 0; i < m_paths.size(); i++)
	{
		if (!is_file_exist(m_paths[i].c_str()))
		{
			all_exist = false;
			break;
		}
	}
	if (!all_exist)
	{
		cropparams->ErrIndex = File_not_all_exist;
	}
	end = clock();
	elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	myfile << "check files exist: " << elapsed_secs << endl;
	begin = 0;
	end = 0;
	begin = clock();

	m_paths.clear();
	m_cur_frames = 1;

    //設定切割thread狀態:完成
	m_mtx_.lock();
	m_Thread_finish = true;
	m_mtx_.unlock();
	cropparams = NULL;

	end = clock();
	elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	myfile << "crop done: " << elapsed_secs << endl;
	begin = 0;
	end = 0;

	
	clock_t end1 = clock();
	elapsed_secs = double(end1 - begin1) / CLOCKS_PER_SEC;
	myfile << "total time: " << elapsed_secs << endl;
	myfile.close();
}

bool CropHandler::ffmpegInitParam(
	AVFormatContext *&ofmt_ctx, AVCodec *&out_codec, AVStream *&out_stream,
	AVCodecContext *&out_codec_ctx, string& output_path,
	SwsContext *&swsctx, AVFrame *&frame, 
	int output_w, int output_h)
{
	CropParams* cropparams = p_mediator->GetSHMhandler()->LoadFromShm();
	int f_ret;
	//設定ffmpeg參數
	const char *output_filename = output_path.c_str();
	f_ret = initialize_avformat_context(ofmt_ctx, output_filename);
	if (f_ret < 0)
	{

    cropparams->ErrIndex = Fail_allocate_output_format_context;
		m_is_stoped = true;
		return false;
	}
	f_ret = initialize_io_context(ofmt_ctx, output_filename);
	if (f_ret < 0)
	{
		cropparams->ErrIndex = Fail_open_output_IO_context;
		m_is_stoped = true;
		return false;
	}

	out_codec = avcodec_find_encoder(m_Codec);
	out_stream = avformat_new_stream(ofmt_ctx, out_codec);
	out_codec_ctx = avcodec_alloc_context3(out_codec);
	set_codec_params(ofmt_ctx, out_codec_ctx, output_w, output_h, m_FPS);
	f_ret = initialize_codec_stream(out_stream, out_codec_ctx, out_codec);
	if (f_ret < 0)
	{
		m_is_stoped = true;
		return false;
	}

	out_stream->codecpar->extradata = out_codec_ctx->extradata;
	out_stream->codecpar->extradata_size = out_codec_ctx->extradata_size;
	av_dump_format(ofmt_ctx, 0, output_filename, 1);

    swsctx = initialize_sample_scaler(out_codec_ctx, output_w, output_h);
	frame = allocate_frame_buffer(out_codec_ctx, output_w, output_h);
	f_ret = avformat_write_header(ofmt_ctx, nullptr);
	if (f_ret < 0)
	{
		cropparams->ErrIndex = Fail_write_header;
		m_is_stoped = true;
		return false;
	}

	if (output_filename != NULL)
	{
		output_filename = NULL;
	}
	return true;
}

void CropHandler::CropVideo2()
{
	ofstream myfile;
	myfile.open("time.txt");
	clock_t begin1 = clock();
	clock_t begin = clock();

	//確認SHM資料存在
	if (!CheckCropData())
		return;
	CropParams* cropparams = p_mediator->GetSHMhandler()->LoadFromShm();

	//設定切割Thread狀態:未完成
	m_mtx_.lock();
	m_Thread_finish = false;
	m_mtx_.unlock();

    Pos** pos = new Pos*[cropparams->H_n];
	for (int k = 0; k < cropparams->H_n; k++)
		pos[k] = new Pos[cropparams->W_n];

	//初始化 FFmpeg library
	av_register_all();

	double Totalframes = cropparams->H_n*cropparams->W_n*cropparams->Frames;
	int w_cover = Rounding(cropparams->W_ptx*cropparams->C_w);
	int h_cover = Rounding(cropparams->H_ptx*cropparams->C_h);

	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	myfile << "init time: " << elapsed_secs << endl;
	begin = 0;
	end = 0;

    	//切割loop 開始
	for (int i = 0; i < cropparams->H_n; i++)
	{
		if (m_is_stoped)
			break;
		for (int j = 0; j < cropparams->W_n; j++)
		{
			begin = clock();
			if (m_is_stoped)
				break;
			//設定ROI 位置
			//M = (開始位置+length*幾段 -重疊段 -1)
			//pos = M+1-重疊段
			if (j == 0)
				pos[i][j].x = 0;
			else
			{
				int M = 0 + j*cropparams->C_w - (j - 1)*(w_cover)-1;
				pos[i][j].x = M + 1 - w_cover;
			}

			if (i == 0)
				pos[i][j].y = 0;
			else
			{
				int M = 0 + i*cropparams->C_h - (i - 1)*(h_cover)-1;
				pos[i][j].y = M + 1 - h_cover;
			}
			//
			end = clock();
			elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
			myfile << "set pos done: " << elapsed_secs << endl;
			begin = 0;
			end = 0;