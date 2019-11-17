#pragma once
//#include <boost\asio.hpp>

#include "cinder/gl/gl.h"

#include "../Modules/TCPclient/TCPClientClass.h"
#include "../Modules/UDPBroadcast/UDPBroadcastClient.h"
#include "../Modules/DataBreakDown/DataBreakDown.h"
#include "../Modules/Mesh/MeshModule.h"
#include "../Modules/VideoModule/VideoLoad.h"
#include "../Modules/UDPProcess/UdpProcess.h"
#include "../3rdParty/OpenCloseEXE/OpenCloseEXE.h"


enum e_Port
{
	TCP_Port		= 1500,		//���Oport
	UDP_Play_Port	= 2000,		//����port
	UDP_IP_Port		= 8883,		//����IP
};  

enum TCPStatus
{
	TCP_DrawTexture,		//�v������ �i�H�P�_shm
	TCP_WindowCapture,
	TCP_StopDraw,			//����
	TCP_GetAllWindowTitle,	//���o�Ҧ������W��
};

class Mediator;
class TCPHandler
{  
public:
	explicit TCPHandler(Mediator* mediator);
	~TCPHandler();
	
	void init();
	//TCP �O�_�s�u
	bool GetTCPStatus() { return m_IsConnect; }
	//���e���O
	int  GetTCPCmd() { return TCPCmd; }
	//���s�u�b����
	void Update();