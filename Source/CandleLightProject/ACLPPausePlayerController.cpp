#include "ACLPPausePlayerController.h"

#include "ACLPTransitionHUD.h"
#include "Components/InputComponent.h"
#include "InputCoreTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

ACLPPausePlayerController::ACLPPausePlayerController()
{
	bShowMouseCursor = false;
}

void ACLPPausePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (!InputComponent)
	{
		return;
	}

	InputComponent->BindKey(EKeys::Escape, IE_Pressed, this, &ACLPPausePlayerController::TogglePause).bExecuteWhenPaused = true;
	InputComponent->BindKey(EKeys::P, IE_Pressed, this, &ACLPPausePlayerController::TogglePause).bExecuteWhenPaused = true;
	InputComponent->BindKey(EKeys::R, IE_Pressed, this, &ACLPPausePlayerController::RestartLevel).bExecuteWhenPaused = true;
	InputComponent->BindKey(EKeys::Q, IE_Pressed, this, &ACLPPausePlayerController::ExitGame).bExecuteWhenPaused = true;
}

bool ACLPPausePlayerController::IsPaused() const
{
	return UGameplayStatics::IsGamePaused(this);
}

void ACLPPausePlayerController::TogglePause()
{
	const bool bNewPaused = !IsPaused();
	UGameplayStatics::SetGamePaused(this, bNewPaused);
	SetShowMouseCursor(bNewPaused);

	if (ACLPTransitionHUD* PauseHUD = Cast<ACLPTransitionHUD>(GetHUD()))
	{
		PauseHUD->SetPaused(bNewPaused);
	}

	UE_LOG(LogTemp, Display, TEXT("CLP Pause: %s"), bNewPaused ? TEXT("ON") : TEXT("OFF"));
}

void ACLPPausePlayerController::RestartLevel()
{
	if (!IsPaused())
	{
		return;
	}

	UGameplayStatics::SetGamePaused(this, false);
	SetShowMouseCursor(false);

	const FString LevelName = UGameplayStatics::GetCurrentLevelName(this, true);
	UE_LOG(LogTemp, Display, TEXT("CLP Pause: restarting %s"), *LevelName);
	UGameplayStatics::OpenLevel(this, FName(*LevelName));
}

void ACLPPausePlayerController::ExitGame()
{
	if (!IsPaused())
	{
		return;
	}

	UE_LOG(LogTemp, Display, TEXT("CLP Pause: exiting game"));
	UKismetSystemLibrary::QuitGame(this, this, EQuitPreference::Quit, true);
}
