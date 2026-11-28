#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ACLPLockedGate.generated.h"

class UStaticMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCLPGateStateChanged, bool, bIsOpen);

/** A placeable door that plates, light locks and the room manager can drive. */
UCLASS(Blueprintable)
class ACLPLockedGate : public AActor
{
	GENERATED_BODY()

public:
	ACLPLockedGate();

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = "Gate")
	void SetOpen(bool bShouldOpen);

	UFUNCTION(BlueprintPure, Category = "Gate")
	bool IsOpen() const { return bIsOpen; }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gate")
	FVector ClosedOffset = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gate")
	FVector OpenOffset = FVector(0.f, 0.f, 220.f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gate", meta = (ClampMin = "0.1"))
	float MoveSpeed = 6.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gate")
	FColor GateColor = FColor(70, 95, 130);

	UPROPERTY(BlueprintAssignable, Category = "Gate")
	FCLPGateStateChanged OnStateChanged;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Gate")
	UStaticMeshComponent* DoorMesh;

	bool bIsOpen = false;
	FVector CurrentOffset = FVector::ZeroVector;
};
