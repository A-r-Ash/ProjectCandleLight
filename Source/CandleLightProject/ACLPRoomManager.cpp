#include "ACLPRoomManager.h"

#include "ACLCollectible.h"
#include "ACLPLockedGate.h"
#include "CLPVesselCandle.h"
#include "Kismet/GameplayStatics.h"

ACLPRoomManager::ACLPRoomManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACLPRoomManager::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> Candles;
	UGameplayStatics::GetAllActorsOfClass(this, ACLPVesselCandle::StaticClass(), Candles);
	if (Candles.Num() > 0)
	{
		ACLPVesselCandle* Candle = Cast<ACLPVesselCandle>(Candles[0]);
		if (Candle)
		{
			Candle->OnExpired.AddDynamic(this, &ACLPRoomManager::HandleCandleExpired);
		}
	}
}

void ACLPRoomManager::NotifyCollectibleCollected(ACLCollectible* Collectible)
{
	if (!Collectible || bRoomComplete)
	{
		return;
	}

	++CollectedCount;
	UE_LOG(LogTemp, Display, TEXT("CLP Room collectibles %d/%d"), CollectedCount, RequiredCollectibles);

	if (CollectedCount >= RequiredCollectibles)
	{
		bRoomComplete = true;
		if (LinkedGate)
		{
			LinkedGate->SetOpen(true);
		}
		OnRoomComplete.Broadcast();
		UE_LOG(LogTemp, Display, TEXT("CLP Room complete - exit open"));
	}
}

void ACLPRoomManager::HandleCandleExpired()
{
	UE_LOG(LogTemp, Display, TEXT("CLP Room heard candle expiry - room reset hook reserved"));
	// Retry behaviour is D02 in the GDD. Hook goes here when that decision is locked.
}
