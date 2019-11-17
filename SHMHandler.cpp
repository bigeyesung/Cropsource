#include "SHMHandler.h"
#include "Mediator.h"

SHMHandler::SHMHandler(Mediator* mediator)
{
	p_mediator = mediator;
	//Is_SHM_Enabled = false;
	//pkWebcamParamsSHM = NULL;
	cropparams = NULL;
}