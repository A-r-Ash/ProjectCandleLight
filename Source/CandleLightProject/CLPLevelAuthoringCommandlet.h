#pragma once

#include "CoreMinimal.h"
#include "Commandlets/Commandlet.h"
#include "CLPLevelAuthoringCommandlet.generated.h"

/** Creates the reusable Candle first-pass assets: tuning asset, materials and CL_Room_01_Candle. */
UCLASS()
class UCLPLevelAuthoringCommandlet : public UCommandlet
{
	GENERATED_BODY()

public:
	virtual int32 Main(const FString& Params) override;
};
