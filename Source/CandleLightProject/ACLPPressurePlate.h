#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ACLPPressurePlate.generated.h"

class ACLPLockedGate;
class UBoxComponent;
class UPrimitiveComponent;
struct FHitResult;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCLPPlateStateChanged, bool, bIsPressed);

/** Weight-based plate. Dark vessels still press it, because mass belongs to the body. */
UCLASS(Blueprintable)
class ACLPPressurePlate : public AActor
{
	GENERATED_BODY()

public:
	ACLPPressurePlate();

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Plate")
	float RequiredMass = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Plate")
	ACLPLockedGate* LinkedGate = nullptr;

	UPROPERTY(BlueprintAssignable, Category = "Plate")
	FCLPPlateStateChanged OnStateChanged;

	UFUNCTION(BlueprintPure, Category = "Plate")
	bool IsPressed() const { return bIsPressed; }

protected:
	UFUNCTION()
	void HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void HandleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void RecomputePressedState();

	UPROPERTY(VisibleAnywhere, Category = "Plate")
	UBoxComponent* TriggerBox;

	UPROPERTY(VisibleAnywhere, Category = "Plate")
	UStaticMeshComponent* PlateMesh;

	TSet<TWeakObjectPtr<AActor>> OverlappingActors;
	bool bIsPressed = false;
};
