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