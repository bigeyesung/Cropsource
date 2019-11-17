#include "TCPHandler.h"
#include "Mediator.h"

//#include "cinder/app/App.h"  //for test 
using namespace ci;
using namespace ci::app;
using namespace std;
TCPHandler::TCPHandler(Mediator* mediator)
{
	p_mediator = mediator;
	m_IsConnect = false;
	m_Step = 0;
	FrameNum = 0;
    	m_DrawState = State_GetServerIP;
	TCPCmd = TCP_StopDraw;
	EndStateMessage = 1;
	m_ProjectorNum = 1;
	m_WinWidth = 1920;
	m_WinHigh = 1080;
	//���� �n�ιw�]�ɦW
	m_ProjectName = "ProjectorConfig2.xml";
}