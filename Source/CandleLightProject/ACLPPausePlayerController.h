#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ACLPPausePlayerController.generated.h"

class ACLPTransitionHUD;

/** Adds Escape/P pause, restart and quit controls for the prototype build. */
UCLASS()
class ACLPPausePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACLPPausePlayerController();

protected:
	virtual void SetupInputComponent() override;

private:
	void TogglePause();
	void RestartLevel();
	void ExitGame();
	bool IsPaused() const;
};
