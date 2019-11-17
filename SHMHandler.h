#pragma once
#include <vector>
//#include "WinAPISharedMem_CType.h"
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include "boost\asio.hpp"  //for winsocket conflict
#include "../../Include/CropParams.h"
#include "../../Include/WinAPISharedMem_CType.h"

class Mediator;
class SHMHandler
{
private:
	Mediator* p_mediator;
	bool Is_SHM_Enabled;                                   //decide SHM state 
	CropParams* cropparams;                                //crop parameter 
	WinAPISharedMem<CropParams>* pkWebcamParamsSHM;