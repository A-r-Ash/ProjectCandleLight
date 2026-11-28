#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ACLLightLock.generated.h"

class ACLPLockedGate;
class UMaterialInstanceDynamic;
class UStaticMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCLPLightLockStateChanged, bool, bIsLit);

/** Opens a linked gate while an active Candle's light reaches the lock. */
UCLASS(Blueprintable)
class ACLLightLock : public AActor
{
	GENERATED_BODY()

public:
	ACLLightLock();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Light Lock")
	ACLPLockedGate* LinkedGate = nullptr;

	/** Minimum light radius needed to satisfy the lock. Match fails; Candle passes. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Light Lock", meta = (ClampMin = "0.0"))
	float MinLightRadius = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Light Lock", meta = (ClampMin = "0.05"))
	float CheckInterval = 0.15f;

	UPROPERTY(BlueprintAssignable, Category = "Light Lock")
	FCLPLightLockStateChanged OnStateChanged;

	UFUNCTION(BlueprintPure, Category = "Light Lock")
	bool IsLit() const { return bIsLit; }

protected:
	void EvaluateLight();

	UPROPERTY(VisibleAnywhere, Category = "Light Lock")
	UStaticMeshComponent* EyeMesh;

	UPROPERTY()
	UMaterialInstanceDynamic* EyeMaterial = nullptr;

	float CheckTimer = 0.f;
	bool bIsLit = false;
};
