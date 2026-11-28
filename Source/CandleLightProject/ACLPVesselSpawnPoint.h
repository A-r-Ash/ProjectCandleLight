#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CLPVessel.h"
#include "ACLPVesselSpawnPoint.generated.h"

class UTextRenderComponent;

/** Place one of these per vessel in a level. ACLPRoomGameMode spawns the body on it. */
UCLASS(Blueprintable)
class ACLPVesselSpawnPoint : public AActor
{
	GENERATED_BODY()

public:
	ACLPVesselSpawnPoint();

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	ECLPVesselKind VesselKind = ECLPVesselKind::Candle;

	/** Use a vessel Blueprint here to make that Blueprint's Class Defaults authoritative. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	TSubclassOf<ACLPVessel> VesselClassOverride;

	/** Enable to tune this placed vessel without editing its shared data asset. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn|Size Override")
	bool bOverrideCollisionAndSize = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn|Size Override", meta = (ClampMin = "1.0", EditCondition = "bOverrideCollisionAndSize"))
	float CapsuleRadius = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn|Size Override", meta = (ClampMin = "1.0", EditCondition = "bOverrideCollisionAndSize"))
	float CapsuleHalfHeight = 44.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn|Size Override", meta = (EditCondition = "bOverrideCollisionAndSize"))
	FVector BodyMeshScale = FVector(1.f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn|Size Override", meta = (EditCondition = "bOverrideCollisionAndSize"))
	FVector BodyMeshOffset = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn|Size Override", meta = (EditCondition = "bOverrideCollisionAndSize"))
	FVector FlameMeshOffset = FVector(0.f, 0.f, 42.f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn|Size Override", meta = (EditCondition = "bOverrideCollisionAndSize"))
	FVector FlameMeshScale = FVector(0.18f, 0.18f, 0.26f);

protected:
	UPROPERTY(VisibleAnywhere, Category = "Spawn")
	UTextRenderComponent* MarkerLabel;
};
