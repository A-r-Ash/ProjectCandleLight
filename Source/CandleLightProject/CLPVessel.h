#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CLPVessel.generated.h"

class UCameraComponent;
class UCLPVesselConfig;
class UEnhancedInputLocalPlayerSubsystem;
class UInputAction;
class UInputMappingContext;
class UMaterialInstanceDynamic;
class UPointLightComponent;
class UStaticMeshComponent;
class UTextRenderComponent;
struct FInputActionValue;

UENUM(BlueprintType)
enum class ECLPVesselKind : uint8
{
	Candle,
	Match,
	Lantern,
	Torch,
	Wisp,
	WaxPuddle,
	Beacon,
	EmberStone,
	MirrorShard,
	IncenseStick
};

/**
 * One physical vessel body. The spark lives in exactly one of these at a time.
 * Built from engine basic shapes so it can be tested and blocked out without art assets.
 */
UCLASS()
class ACLPVessel : public ACharacter
{
	GENERATED_BODY()

public:
	ACLPVessel();

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void UnPossessed() override;

	/** Shape, label, mass feel, light radius and colour for this vessel. */
	void ConfigureVessel(ECLPVesselKind InKind, const FString& InLabel, float InMoveSpeed, float InJumpVelocity, float InLightRadius, const FColor& InColour);

	/** Applies every value from an editable tuning asset, including collision and visual size. */
	void ApplyConfigData(UCLPVesselConfig* Config);

	/** Editor-driven collision and visual size override. */
	void SetVesselSize(float InCapsuleRadius, float InCapsuleHalfHeight, const FVector& InBodyScale, const FVector& InBodyOffset, const FVector& InFlameOffset, const FVector& InFlameScale);

	/** Re-applies the Blueprint-editable size defaults to the live components. */
	void ApplyPresetSize();

	/** Re-applies the Blueprint-editable movement defaults to the live movement component. */
	void ApplyMovementPreset();

	bool ShouldUseTuningAsset() const { return bUseTuningAsset; }
	UCLPVesselConfig* GetTuningAsset() const { return TuningAsset; }

	/** Turns the flame and halo on/off when the spark enters/leaves this body. */
	virtual void SetSparkActive(bool bActive);

	bool IsSparkActive() const { return bSparkActive; }
	ECLPVesselKind GetVesselKind() const { return Kind; }
	FString GetVesselLabel() const { return VesselLabel; }
	float GetVesselMass() const { return VesselMass; }
	float GetMoveSpeed() const { return PresetMoveSpeed; }
	float GetLightRadius() const { return LightRadius; }
	float GetCapsuleRadius() const;
	float GetCapsuleHalfHeight() const;

	UFUNCTION(BlueprintCallable, Category = "Vessel")
	void SetVesselMass(float InMass) { VesselMass = FMath::Max(1.f, InMass); }

protected:
	void MoveEnhanced(const FInputActionValue& Value);
	void MoveX(float Value);
	void JumpPressed();
	void JumpReleased();
	void SwapPressed();
	UEnhancedInputLocalPlayerSubsystem* GetInputSubsystem() const;
	void ApplyCapsuleForKind(ECLPVesselKind InKind);
	void ApplyRuntimeMaterials();

	UPROPERTY(VisibleAnywhere, Category = "Vessel")
	UStaticMeshComponent* BodyMesh;

	UPROPERTY(VisibleAnywhere, Category = "Vessel")
	UStaticMeshComponent* DetailMesh;

	UPROPERTY(VisibleAnywhere, Category = "Vessel")
	UStaticMeshComponent* FlameMesh;

	/** Editor-only wireframe preview that matches the collision capsule. */
	UPROPERTY(VisibleAnywhere, Category = "Vessel|Debug")
	UStaticMeshComponent* CollisionPreview;

	UPROPERTY(VisibleAnywhere, Category = "Vessel")
	UPointLightComponent* FlameLight;

	UPROPERTY(VisibleAnywhere, Category = "Vessel")
	UTextRenderComponent* NameLabel;

	UPROPERTY(VisibleAnywhere, Category = "Vessel")
	UCameraComponent* SideCamera;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY()
	UInputAction* SwapAction;

	UPROPERTY()
	UInputMappingContext* PrototypeContext;

	UPROPERTY()
	UEnhancedInputLocalPlayerSubsystem* CachedInputSubsystem = nullptr;

	UPROPERTY()
	UMaterialInstanceDynamic* BodyMaterial;

	UPROPERTY()
	UMaterialInstanceDynamic* FlameMaterial;

	UPROPERTY()
	ECLPVesselKind Kind = ECLPVesselKind::Candle;

	UPROPERTY()
	FString VesselLabel;

	/** Body-based mass. Inactive vessels still press plates. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vessel")
	float VesselMass = 100.f;

	/** Blueprint-editable movement speed. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Vessel|Movement", meta = (ClampMin = "1.0"))
	float PresetMoveSpeed = 300.f;

	/** Enable to let a tuning data asset override the Blueprint defaults at runtime. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Vessel|Tuning")
	bool bUseTuningAsset = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Vessel|Tuning", meta = (EditCondition = "bUseTuningAsset"))
	UCLPVesselConfig* TuningAsset = nullptr;

	/** Blueprint-editable collision. Change these when opening a vessel Blueprint. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Vessel|Collision", meta = (ClampMin = "1.0"))
	float PresetCapsuleRadius = 20.f;

	/** Full collision height. Capsule Half Height is derived from this value. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Vessel|Collision", meta = (ClampMin = "2.0"))
	float PresetCapsuleHeight = 88.f;

	/** Derived: Preset Capsule Height / 2. */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Vessel|Collision")
	float PresetCapsuleHalfHeight = 44.f;

	/** Blueprint-editable visual size. These do not change the capsule. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Vessel|Visual Size")
	FVector PresetBodyMeshScale = FVector(1.f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Vessel|Visual Size")
	FVector PresetBodyMeshOffset = FVector::ZeroVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Vessel|Visual Size")
	FVector PresetFlameMeshOffset = FVector(0.f, 0.f, 42.f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Vessel|Visual Size")
	FVector PresetFlameMeshScale = FVector(0.18f, 0.18f, 0.26f);

	float LightRadius = 600.f;
	FColor DesiredBodyColor = FColor::White;
	bool bSparkActive = false;
	bool bHasLoggedMovement = false;
};
