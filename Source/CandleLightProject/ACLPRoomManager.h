#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ACLPRoomManager.generated.h"

class ACLCollectible;
class ACLPLockedGate;
class ACLPVesselCandle;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCLPRoomComplete);

/** Room-scoped rules: collectible count, exit unlock, candle-expiry hook. */
UCLASS(Blueprintable)
class ACLPRoomManager : public AActor
{
	GENERATED_BODY()

public:
	ACLPRoomManager();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Room")
	void NotifyCollectibleCollected(ACLCollectible* Collectible);

	UFUNCTION(BlueprintPure, Category = "Room")
	int32 GetCollectedCount() const { return CollectedCount; }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Room", meta = (ClampMin = "1"))
	int32 RequiredCollectibles = 3;

	/** Opened when all required collectibles are collected. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Room")
	ACLPLockedGate* LinkedGate = nullptr;

	UPROPERTY(BlueprintAssignable, Category = "Room")
	FCLPRoomComplete OnRoomComplete;

protected:
	UFUNCTION()
	void HandleCandleExpired();

	int32 CollectedCount = 0;
	bool bRoomComplete = false;
};
