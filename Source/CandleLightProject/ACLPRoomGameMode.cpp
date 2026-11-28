#include "ACLPRoomGameMode.h"

#include "ACLPTransitionHUD.h"

ACLPRoomGameMode::ACLPRoomGameMode()
{
	bBuildRuntimeTestLane = false;
	bTeleportOnSwap = true;
	HUDClass = ACLPTransitionHUD::StaticClass();
}
