#include "ACLPLevelExitTrigger.h"

#include "ACLPTransitionHUD.h"
#include "CLPVessel.h"
#include "Components/BoxComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInterface.h"

ACLPLevelExitTrigger::ACLPLevelExitTrigger()
{
	PrimaryActorTick.bCanEverTick = false;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;
	TriggerBox->SetBoxExtent(BoxExtent);
	TriggerBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	TriggerBox->SetGenerateOverlapEvents(true);
	TriggerBox->SetHiddenInGame(false);

	if (UMaterialInterface* ExitMaterial = LoadObject<UMaterialInterface>(
		nullptr, TEXT("/Game/Candlelight/Materials/MI_CLP_Return.MI_CLP_Return")))
	{
		TriggerBox->SetMaterial(0, ExitMaterial);
	}
}

void ACLPLevelExitTrigger::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (TriggerBox)
	{
		TriggerBox->SetBoxExtent(BoxExtent);
	}
}

void ACLPLevelExitTrigger::BeginPlay()
{
	Super::BeginPlay();
	if (TriggerBox)
	{
		TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ACLPLevelExitTrigger::HandleBeginOverlap);
	}
}

void ACLPLevelExitTrigger::HandleBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (bTriggered || !Cast<ACLPVessel>(OtherActor) || NextLevelName.IsNone())
	{
		return;
	}

	bTriggered = true;
	UE_LOG(LogTemp, Display, TEXT("CLP Level exit: transitioning to %s"), *NextLevelName.ToString());

	APlayerController* PlayerController = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr;
	ACLPTransitionHUD* TransitionHUD = PlayerController ? Cast<ACLPTransitionHUD>(PlayerController->GetHUD()) : nullptr;
	if (TransitionHUD)
	{
		TransitionHUD->BeginLevelTransition(NextLevelName);
	}
	else
	{
		UGameplayStatics::OpenLevel(this, NextLevelName);
	}
}
