#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CLPVessel.h"
#include "ACLGoalCorner.generated.h"

class ACLPLockedGate;
class UBoxComponent;
class UPointLightComponent;
class UStaticMeshComponent;
class UTextRenderComponent;
class UPrimitiveComponent;
struct FHitResult;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCLPGoalReached);

/**
 * End-of-side marker. In Level 1 it simply reports that the player reached the
 * corner. Later levels can link it to a gate that opens the next side.
 */
UCLASS(Blueprintable)
class ACLGoalCorner : public AActor
{
	GENERATED_BODY()

public:
	ACLGoalCorner();

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Goal")
	FString GoalName = TEXT("CORNER");

	/** Optional door to open when the goal is reached. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Goal")
	ACLPLockedGate* LinkedGate = nullptr;

	/** Which vessel must be holding the spark to finish this side. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Goal")
	ECLPVesselKind RequiredVesselKind = ECLPVesselKind::Candle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Goal")
	bool bTriggerOnce = true;

	UPROPERTY(BlueprintAssignable, Category = "Goal")
	FCLPGoalReached OnGoalReached;

	UFUNCTION(BlueprintPure, Category = "Goal")
	bool IsReached() const { return bReached; }

protected:
	UFUNCTION()
	void HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, Category = "Goal")
	UBoxComponent* TriggerBox;

	UPROPERTY(VisibleAnywhere, Category = "Goal")
	UStaticMeshComponent* MarkerMesh;

	UPROPERTY(VisibleAnywhere, Category = "Goal")
	UTextRenderComponent* Label;

	UPROPERTY(VisibleAnywhere, Category = "Goal")
	UPointLightComponent* Glow;

	bool bReached = false;
};
