#pragma once


#include "../Modules/DirectXWindowCapture/DirectXWindowCapture.h"

#include "cinder/app/App.h"  //for test
using namespace ci;
using namespace ci::app;
using namespace std;

class Mediator;
class WindowsCaptureHandler
{
public:
	explicit WindowsCaptureHandler(Mediator* mediator);
	~WindowsCaptureHandler();

	bool GetWCStatus() { return TitleButtonPressed; }                        
	void SetWCStatus(bool WindowCapture) { TitleButtonPressed = WindowCapture; }

	bool init();
	
	void Draw();