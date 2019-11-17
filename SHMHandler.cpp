#include "SHMHandler.h"
#include "Mediator.h"

SHMHandler::SHMHandler(Mediator* mediator)
{
	p_mediator = mediator;
	//Is_SHM_Enabled = false;
	//pkWebcamParamsSHM = NULL;
	cropparams = NULL;
}

SHMHandler::~SHMHandler()
{
	//����player->cropparams->ExeFileopen�]��0;
	cropparams->ExeFileopen = 0;

	if (cropparams != NULL)
	{
		//delete cropparams;
		cropparams = NULL;
	}
	if (pkWebcamParamsSHM != NULL)
	{
		delete pkWebcamParamsSHM;
		pkWebcamParamsSHM = NULL;
	}
	
}