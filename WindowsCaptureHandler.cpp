#include "WindowsCaptureHandler.h"
#include "Mediator.h"
WindowsCaptureHandler::WindowsCaptureHandler(Mediator* mediator)
{
	p_mediator = mediator;
	IsSelectWindow = false;
	TitleButtonPressed = false;
	m_Step = 0;
}