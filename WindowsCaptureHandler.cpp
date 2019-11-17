#include "WindowsCaptureHandler.h"
#include "Mediator.h"
WindowsCaptureHandler::WindowsCaptureHandler(Mediator* mediator)
{
	p_mediator = mediator;
	IsSelectWindow = false;
	TitleButtonPressed = false;
	m_Step = 0;
}

WindowsCaptureHandler::~WindowsCaptureHandler()
{
	//if (p_mediator != NULL)
	//{
	//	p_mediator = NULL;
	//}
}

bool WindowsCaptureHandler::init()
{
	HRESULT hr = S_OK;
	hr = m_DirectXWindowCapture.init();
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}