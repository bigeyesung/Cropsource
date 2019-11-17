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