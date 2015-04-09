#include "gspipechannel.h"
#include "gsprocessor.h"
#include <framework/gsapi.h>

VOID CGSPipeChannel::OnEstablish()
{	
	CGSProcessor::Instance()->StartSyncPlayer(gsapi::GetServerStartTime());
}


