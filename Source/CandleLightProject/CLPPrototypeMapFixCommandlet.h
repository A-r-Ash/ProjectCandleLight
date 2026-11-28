#pragma once

#include "CoreMinimal.h"
#include "Commandlets/Commandlet.h"
#include "CLPPrototypeMapFixCommandlet.generated.h"

/** One-shot editor commandlet: clears the template GameMode overrides on the bundled maps. */
UCLASS()
class UCLPPrototypeMapFixCommandlet : public UCommandlet
{
	GENERATED_BODY()

public:
	virtual int32 Main(const FString& Params) override;
};
