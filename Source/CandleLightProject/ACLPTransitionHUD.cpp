#include "ACLPTransitionHUD.h"

#include "Engine/Canvas.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

ACLPTransitionHUD::ACLPTransitionHUD()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACLPTransitionHUD::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Display, TEXT("CLP Transition HUD ready"));
}

void ACLPTransitionHUD::BeginLevelTransition(FName NextLevel)
{
	if (bTransitioning || NextLevel.IsNone())
	{
		return;
	}

	PendingLevel = NextLevel;
	FadeAlpha = 0.f;
	bTransitioning = true;
}

void ACLPTransitionHUD::DrawHUD()
{
	Super::DrawHUD();

	if (!Canvas)
	{
		return;
	}

	if (bTransitioning && GetWorld())
	{
		FadeAlpha = FMath::Clamp(FadeAlpha + GetWorld()->GetDeltaSeconds() / FadeDuration, 0.f, 1.f);
		DrawRect(FLinearColor(0.f, 0.f, 0.f, FadeAlpha), 0.f, 0.f, Canvas->SizeX, Canvas->SizeY);

		if (FadeAlpha >= 1.f)
		{
			const FName LevelToLoad = PendingLevel;
			bTransitioning = false;
			PendingLevel = NAME_None;
			UE_LOG(LogTemp, Display, TEXT("CLP Transition fade complete: loading %s"), *LevelToLoad.ToString());
			UGameplayStatics::OpenLevel(this, LevelToLoad);
		}
		return;
	}

	if (!bPaused)
	{
		return;
	}

	DrawRect(FLinearColor(0.f, 0.f, 0.f, 0.78f), 0.f, 0.f, Canvas->SizeX, Canvas->SizeY);

	const float CenterX = Canvas->SizeX * 0.5f;
	const float StartY = Canvas->SizeY * 0.34f;
	const FLinearColor TitleColour(1.f, 0.80f, 0.36f);
	const FLinearColor OptionColour(0.90f, 0.92f, 0.96f);

	DrawText(TEXT("PAUSED"), TitleColour, CenterX - 92.f, StartY, nullptr, 2.0f);
	DrawText(TEXT("ESC / P   RESUME"), OptionColour, CenterX - 150.f, StartY + 90.f, nullptr, 1.25f);
	DrawText(TEXT("R         RESTART LEVEL"), OptionColour, CenterX - 180.f, StartY + 140.f, nullptr, 1.25f);
	DrawText(TEXT("Q         EXIT GAME"), OptionColour, CenterX - 150.f, StartY + 190.f, nullptr, 1.25f);
}
