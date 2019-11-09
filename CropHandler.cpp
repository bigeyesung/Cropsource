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