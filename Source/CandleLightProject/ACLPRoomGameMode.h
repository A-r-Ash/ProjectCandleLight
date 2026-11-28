#pragma once

#include "CoreMinimal.h"
#include "CLPPrototypeGameMode.h"
#include "ACLPRoomGameMode.generated.h"

/** GameMode for authored levels. It preserves placed actors and spawns vessels on spawn points. */
UCLASS()
class ACLPRoomGameMode : public ACLPPrototypeGameMode
{
	GENERATED_BODY()

public:
	ACLPRoomGameMode();
};
