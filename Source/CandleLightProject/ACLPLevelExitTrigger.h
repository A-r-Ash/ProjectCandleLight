#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ACLPLevelExitTrigger.generated.h"

class UBoxComponent;
class UPrimitiveComponent;
struct FHitResult;

/** Visible trigger at the far-right end of a side. Overlap loads the next map. */
UCLASS(Blueprintable)
class ACLPLevelExitTrigger : public AActor
{
	GENERATED_BODY()

public:
	ACLPLevelExitTrigger();

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Exit")
	FName NextLevelName = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Exit")
	FVector BoxExtent = FVector(120.f, 220.f, 180.f);

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

private:
	UPROPERTY(VisibleAnywhere, Category = "Level Exit")
	UBoxComponent* TriggerBox;

	bool bTriggered = false;
};
