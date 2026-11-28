#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ACLPTransitionHUD.generated.h"

/** Full-screen black fade used when an exit trigger loads the next level. */
UCLASS()
class ACLPTransitionHUD : public AHUD
{
	GENERATED_BODY()

public:
	ACLPTransitionHUD();

	/** Start fading to black, then open NextLevel when the screen is covered. */
	void BeginLevelTransition(FName NextLevel);

	/** Shows or hides the pause menu overlay. */
	void SetPaused(bool bInPaused) { bPaused = bInPaused; }

protected:
	virtual void BeginPlay() override;
	virtual void DrawHUD() override;

private:
	bool bTransitioning = false;
	bool bPaused = false;
	float FadeAlpha = 0.f;
	float FadeDuration = 0.45f;
	FName PendingLevel = NAME_None;
};
