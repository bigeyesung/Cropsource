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
//���s�u�b����
	void Draw();
	void ConnectServer();
	//bool IsConnect() { return m_IsConnect; }

	//�����^����
	void SetShaderPara_WindowCapture(int BindIndex);
	void Draw_WindowCapture();
	//Ū�� ����shader mesh ��� , ���i���Ƨ��� �ҥH���sŪ���P���ɮ�
	void DrawInit(string ReceiveName, bool ReInit = false);
	bool IsWindowCapture() { return m_DrawState == State_DrawWindowCapture; }//�^�ǬO�_�b�����^��

private:
	//�o�T���� server
	void SendToServer(int in);
	void SendToServer(string in);

	//�]�wshade �Ѽ�
	void SetShaderPara();
	//Ū��glsl shader
	void CreateShader(string VertexShaderPath, string FragShaderPath);
	//�إ�mesh
	void SetMeshData();
	void generateMonitorPoint(vector<vector<joints>>& PointArr, int aliquots_w, int aliquots_h);
	//�etexture
	void DrawTexture();
	//Ū���v��
	void LoadVideoFirst(const vector<string>& VideoPath);
	vector<int> m_StateData;
	bool m_IsConnect;
	//���e�M�צW��
	string m_ProjectName;
	int TCPCmd;
	int FrameNum;//�p�ɥ�
	int m_DrawState;
	int m_Step;
	//�T�{�T�� 1:���A���� -1:���o�e ��L:����
	int EndStateMessage;
    //xml���
	s_InitData m_InitData;
	//�s�u��������A
	int m_WinOffset;
	int m_WinWidth;
	int m_WinHigh;

	//���ե� ��v���ƶq
	int m_ProjectorNum;
	//���զsŪ��
	//vector<vector<joints>> f_Pos; 
    	Mediator* p_mediator;
	//TCP ���q
	TCPClientClass ProjClient;
	//��������Ƥ��R
	DataBreakDown m_DataBreakDown;
	//����serverIP
	UDPBroadcastClient* m_UDPBroadcastclient;
	//�v���P�B �������e����frame��
	UdpProcess m_udpClient;

    	//shader
	vector<gl::GlslProgRef> m_Shader;
	//Mesh
	vector<CustomMesh> m_CustomMesh;
	//Ū���v��
	VideoLoad m_VideoLoad;
	//��i��
	gl::TextureRef	m_ImgTexture;
	//�}����L�{��
	OpenCloseExe m_OpenCloseExe;
};