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

void WindowsCaptureHandler::DrawInit()
{
	//�q xml ����W�����M�צW��
	//�w�]����
	p_mediator->GetTCPhandler()->DrawInit("ProjectorConfig2.xml");
	//�]�wshader Ū1�i texture UV ����
	p_mediator->GetTCPhandler()->SetShaderPara_WindowCapture(0);
	////����� �w�]�eindex 0
	//vector<string> title;
	//m_DirectXWindowCapture.FindAllWindowTitle(title);
}

bool WindowsCaptureHandler::FindAllWindowTitle(vector<string>& AllTitle)
{
	m_DirectXWindowCapture.FindAllWindowTitle(AllTitle);	
	return true;
}