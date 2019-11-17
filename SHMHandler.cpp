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

void SHMHandler::SetSHMStatus(bool is_enabled)
{
}

bool SHMHandler::GetSHMStatus()
{
	
	if (cropparams->NotifytoPlayer == 0)
		return false;
	else
		return true;
}

bool SHMHandler::init()
{
	pkWebcamParamsSHM = NULL;
	cropparams = NULL;
	//WinAPISharedMem<CropParams>* pkWebcamParamsSHM;
	if (!pkWebcamParamsSHM)
	{
		pkWebcamParamsSHM = new WinAPISharedMem<CropParams>();
		pkWebcamParamsSHM->createSharedMem(_T("CropParams"), sizeof(CropParams), cropparams);
	}
	if (cropparams == NULL)
	{
		//printf("Failed to create ExchangeInfo shared memory\n");
		//system("pause");
		return false;
	}

	cropparams->ExeFileopen = 1;
	std::memset(cropparams->path, '\0', sizeof(char) * 60000);
	std::memset(cropparams->path, '\0', sizeof(char) * 60000);
	return true;
}

void SHMHandler::Clear()
{
	if (pkWebcamParamsSHM != NULL)
	{
		delete pkWebcamParamsSHM;
		pkWebcamParamsSHM = NULL;
	}

	if (cropparams != NULL)
	{
		//delete cropparams;
		cropparams = NULL;
	}



}

void SHMHandler::WriteToShm(const CropParams & CropData)
{
	*cropparams = CropData;
	
}