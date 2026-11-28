#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CLPVessel.h"
#include "CLPVesselConfig.generated.h"

/** Tuning values for one vessel. Level designers edit this asset instead of magic numbers in code. */
UCLASS(BlueprintType)
class UCLPVesselConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vessel")
	ECLPVesselKind VesselKind = ECLPVesselKind::Candle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vessel")
	FString DisplayName = TEXT("CANDLE");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (ClampMin = "1.0"))
	float MoveSpeed = 210.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (ClampMin = "1.0"))
	float JumpVelocity = 390.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (ClampMin = "0.1"))
	float GravityScale = 1.8f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Light", meta = (ClampMin = "0.0"))
	float LightRadius = 900.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Light", meta = (ClampMin = "0.0"))
	float LightIntensity = 8000.f;

	/** Body mass used by pressure plates. Inactive vessels still apply this mass. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle", meta = (ClampMin = "1.0"))
	float Mass = 120.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle")
	bool bCanFitSmallGaps = false;

	/** Collision capsule. Edit these to change what gaps a vessel can fit through. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision", meta = (ClampMin = "1.0"))
	float CapsuleRadius = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision", meta = (ClampMin = "1.0"))
	float CapsuleHalfHeight = 44.f;

	/** Visual body size and placement. These do not change collision. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visual Size")
	FVector BodyMeshScale = FVector(1.f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visual Size")
	FVector BodyMeshOffset = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visual Size")
	FVector FlameMeshOffset = FVector(0.f, 0.f, 42.f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visual Size")
	FVector FlameMeshScale = FVector(0.18f, 0.18f, 0.26f);

	/** Candle-specific clock. Other vessels currently ignore this. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Candle", meta = (ClampMin = "1.0"))
	float BurnDurationSeconds = 180.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Candle", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float LowWaxThreshold = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visual")
	FColor BodyColor = FColor(240, 220, 180);
};
