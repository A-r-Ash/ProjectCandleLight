#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ACLCollectible.generated.h"

class ACLPRoomManager;
class ACLPVessel;
class UPointLightComponent;
class UPrimitiveComponent;
class USphereComponent;
class UStaticMeshComponent;
struct FHitResult;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCLPCollectibleCollected);

/** A light fragment. Only the vessel holding the spark can collect it. */
UCLASS(Blueprintable)
class ACLCollectible : public AActor
{
	GENERATED_BODY()

public:
	ACLCollectible();

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collectible")
	ACLPRoomManager* RoomManager = nullptr;

	UPROPERTY(BlueprintAssignable, Category = "Collectible")
	FCLPCollectibleCollected OnCollected;

	UFUNCTION(BlueprintCallable, Category = "Collectible")
	void Collect();

	UFUNCTION(BlueprintPure, Category = "Collectible")
	bool IsCollected() const { return bCollected; }

protected:
	UFUNCTION()
	void HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, Category = "Collectible")
	USphereComponent* TriggerSphere;

	UPROPERTY(VisibleAnywhere, Category = "Collectible")
	UStaticMeshComponent* FragmentMesh;

	UPROPERTY(VisibleAnywhere, Category = "Collectible")
	UPointLightComponent* Glow;

	FVector BaseLocation = FVector::ZeroVector;
	float BobTime = 0.f;
	bool bCollected = false;
};
