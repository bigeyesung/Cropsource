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

TCPHandler::~TCPHandler()
{
	ProjClient.CloseSocket();
	m_VideoLoad.EndThread();

	if (m_UDPBroadcastclient != NULL)
	{
		delete m_UDPBroadcastclient;
		m_UDPBroadcastclient = NULL;
	}

	//if (p_mediator != NULL)
	//{
	//	p_mediator = NULL;
	//}
}

void TCPHandler::init()
{
	//����serverIP
	m_UDPBroadcastclient = new UDPBroadcastClient(UDP_IP_Port);
	//TCP Client
	ProjClient.InitSocket();
	//���]�w���A���ܼƫ��е�TCP
	ProjClient.SetSataePtr(&m_DrawState);
	//set udp broadcast
	//�ե��ݤ���}�ҦP�˪�port �|�X��
	m_udpClient.init(true, UDP_Play_Port, "");

    	////���ճ�i��
	m_ImgTexture = gl::Texture::create(800, 600);

	DrawInit("ProjectorConfig2");
}

void TCPHandler::CreateShader(string VertexShaderPath, string FragShaderPath)
{
	try
	{
		for (int i = 0; i < m_Shader.size(); i++)
		{
			m_Shader[i] = (gl::GlslProg::create(loadFile(VertexShaderPath)
				, loadFile(FragShaderPath)));
		}
	}
	catch (gl::GlslProgCompileExc &exc)
	{
		console() << "shader error";
		exit(EXIT_FAILURE);
	}
}

void TCPHandler::Update()
{
	ProjClient.Async_Update();
	m_udpClient.update();
}

void TCPHandler::Draw()
{
	if (m_DrawState == State_PlayVideo)
	{
		if (m_udpClient.getNumber() != -10)
		{
			TCPCmd = TCP_DrawTexture;
		}
		m_VideoLoad.BindVideoTexture(m_udpClient.getNumber());
		DrawTexture();
    	}
	else if(m_DrawState == State_ReloadVideo)
	{
		TCPCmd = TCP_StopDraw;
		m_udpClient.setNumber(-10);
		m_VideoLoad.BindVideoTexture(m_udpClient.getNumber());
		DrawTexture();
		m_DrawState = State_PlayVideo;
		SendToServer(1);
		ProjClient.Async_ReadStateString();
	}
	else if (m_DrawState == State_ReloadVideo_Reconnect)
	{
		TCPCmd = TCP_StopDraw;
		//�_�u���p
		m_udpClient.setNumber(-10);
		m_VideoLoad.BindVideoTexture(m_udpClient.getNumber());
		DrawTexture();
		m_DrawState = State_WaitServerInstruction;
		m_VideoLoad.ReleaseVideo();
		m_IsConnect = false;
	}
	else if (m_DrawState == State_PreparePlayVideo)
	{
		//�p�G���L�ɮצW �ݭn���sŪ��
		if (true)//�C��PrePlay�����sŪ���v�� shader mesh
		{

			//��ƥi���L ���sŪ���ɮ�
			DrawInit(m_ProjectName, true);
			setWindowPos(m_WinOffset, 0);
			setWindowSize(m_ProjectorNum * m_WinWidth, m_WinHigh);
		}
        	TCPCmd = TCP_DrawTexture;
		//TCP ���B���� �����������O
		ProjClient.Async_ReadStateString();
		m_DrawState = State_PlayVideo;
		//�o�e�T����server�A�w�i�H�}�l����
		SendToServer(1);
	}
    else if (m_DrawState == State_DrawWindowCapture)
	{
		//��sDX texture
		p_mediator->GetWChandler()->getDX_GLobject().UpdateHDC();
		p_mediator->GetWChandler()->getDX_GLobject().LockDX();
		DrawTexture();
		p_mediator->GetWChandler()->getDX_GLobject().UnLockDX();
	}
    else if (m_DrawState == State_preDrawWindowCapture)
	{
		TCPCmd = TCP_WindowCapture;
		m_Step = 0;
		int CinderBindIndex = 0;
		SetShaderPara_WindowCapture(CinderBindIndex);
		//��ܫ��w����
		p_mediator->GetWChandler()->SelectWindow(m_StateData[StateSetData_Count]);
		p_mediator->GetWChandler()->SetWCStatus(true);
		//�j�wDX GL texture
		p_mediator->GetWChandler()->getDX_GLobject().InitGLDX_ShareTexture(CinderBindIndex);
        	m_DrawState = State_DrawWindowCapture;
		//TCP ���B���� ���ݤ����������O
		ProjClient.Async_ReadStateString();
		//�^�ǽT�{�T�� �w�}�l�e
		SendToServer(1);
	}
    else if (m_DrawState == State_GetAllWindowTitle)
	{
		if (m_Step == 0)//���o�e�����W�ٵ�Server
		{
			//TCPCmd = TCP_GetAllWindowTitle;
			vector<string> AllTitle;
			p_mediator->GetWChandler()->FindAllWindowTitle(AllTitle);
			string tempS = "";
			tempS = to_string(AllTitle.size()) + " ";
			for (int i = 0; i < AllTitle.size(); i++)
			{
				tempS = tempS + AllTitle[i] + " ";
			}   
            		EndStateMessage = -1;
			//�����W�٥N���T�{�T��
			SendToServer(tempS);
			m_DrawState = State_WaitServerInstruction;
		}
	}
else if (m_DrawState == State_WaitServerInstruction)
	{
		if (m_Step == 0)
		{
			//m_VideoLoad.ReleaseVideo();
			//����state���o�e�T���� server
			SendToServer(EndStateMessage);
			TCPCmd = TCP_StopDraw;
			//���BTCPŪ��
			ProjClient.Async_ReadString();
			m_Step++;
		}   