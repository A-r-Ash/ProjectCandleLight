#include "CLPVessel.h"

#include "CLPPrototypeGameMode.h"
#include "CLPVesselConfig.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputCoreTypes.h"
#include "InputMappingContext.h"
#include "InputModifiers.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "UObject/ConstructorHelpers.h"

namespace CLPShapes
{
	static UStaticMesh* Cube()
	{
		return LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
	}
	static UStaticMesh* Cylinder()
	{
		return LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	}
	static UStaticMesh* Sphere()
	{
		return LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	}
	static UStaticMesh* Cone()
	{
		return LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cone.Cone"));
	}
	static UMaterial* BasicMaterial()
	{
		UMaterial* Mat = LoadObject<UMaterial>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
		return Mat ? Mat : UMaterial::GetDefaultMaterial(MD_Surface);
	}
}

ACLPVessel::ACLPVessel()
{
	PrimaryActorTick.bCanEverTick = false;
	AutoPossessPlayer = EAutoReceiveInput::Disabled;
	bUseControllerRotationYaw = false;

	GetCapsuleComponent()->InitCapsuleSize(20.f, 44.f);
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->MaxWalkSpeed = 320.f;
	GetCharacterMovement()->JumpZVelocity = 460.f;
	GetCharacterMovement()->GravityScale = 1.8f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxStepHeight = 35.f;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(RootComponent);
	BodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	DetailMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DetailMesh"));
	DetailMesh->SetupAttachment(RootComponent);
	DetailMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DetailMesh->SetVisibility(false);

	FlameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlameMesh"));
	FlameMesh->SetupAttachment(RootComponent);
	FlameMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FlameMesh->SetVisibility(false);

	// Editor-visible capsule preview. Hidden in game, shown in Blueprint viewports.
	CollisionPreview = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CollisionPreview"));
	CollisionPreview->SetupAttachment(RootComponent);
	CollisionPreview->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CollisionPreview->SetCastShadow(false);
	CollisionPreview->SetVisibility(true);
	CollisionPreview->SetHiddenInGame(true);
	if (UStaticMesh* CapsuleMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Capsule.Capsule")))
	{
		CollisionPreview->SetStaticMesh(CapsuleMesh);
	}
	if (UMaterialInterface* PreviewMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Engine/EngineDebugMaterials/M_SimpleUnlitTranslucent.M_SimpleUnlitTranslucent")))
	{
		CollisionPreview->SetMaterial(0, PreviewMaterial);
	}

	FlameLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("FlameLight"));
	FlameLight->SetupAttachment(RootComponent);
	FlameLight->SetLightColor(FLinearColor(1.f, 0.55f, 0.18f));
	FlameLight->SetSourceRadius(8.f);
	FlameLight->SetIntensity(0.f);

	NameLabel = CreateDefaultSubobject<UTextRenderComponent>(TEXT("NameLabel"));
	NameLabel->SetupAttachment(RootComponent);
	NameLabel->SetWorldSize(18.f);
	NameLabel->SetTextRenderColor(FColor(120, 140, 165));
	NameLabel->SetHorizontalAlignment(EHTA_Center);
	NameLabel->SetVerticalAlignment(EVRTA_TextBottom);
	NameLabel->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
	NameLabel->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	NameLabel->SetCastShadow(false);

	SideCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("SideCamera"));
	SideCamera->SetupAttachment(RootComponent);
	// Orthographic side camera in front of the level shell. Looking along -Y
	// keeps world +X (level progress) on screen right. The back wall sits behind
	// this camera, so it can never block the view.
	SideCamera->SetRelativeLocation(FVector(0.f, 1500.f, 330.f));
	SideCamera->SetRelativeRotation(FRotator(-6.f, -90.f, 0.f));
	SideCamera->SetProjectionMode(ECameraProjectionMode::Orthographic);
	SideCamera->OrthoWidth = 1800.f;
	SideCamera->AspectRatio = 16.f / 9.f;
	SideCamera->bConstrainAspectRatio = true;
	SideCamera->bAutoActivate = true;

	SwapAction = CreateDefaultSubobject<UInputAction>(TEXT("SwapAction"));
	SwapAction->ValueType = EInputActionValueType::Boolean;

	PrototypeContext = CreateDefaultSubobject<UInputMappingContext>(TEXT("PrototypeContext"));

	static ConstructorHelpers::FObjectFinder<UInputAction> MoveObj(TEXT("/Game/Input/Actions/IA_Move.IA_Move"));
	static ConstructorHelpers::FObjectFinder<UInputAction> JumpObj(TEXT("/Game/Input/Actions/IA_Jump.IA_Jump"));
	MoveAction = MoveObj.Succeeded() ? MoveObj.Object : nullptr;
	JumpAction = JumpObj.Succeeded() ? JumpObj.Object : nullptr;

	// Explicit bindings so the prototype does not depend on the template mapping context.
	if (PrototypeContext && MoveAction)
	{
		PrototypeContext->MapKey(MoveAction, EKeys::A).Modifiers.Add(NewObject<UInputModifierNegate>(PrototypeContext));
		PrototypeContext->MapKey(MoveAction, EKeys::Left).Modifiers.Add(NewObject<UInputModifierNegate>(PrototypeContext));
		PrototypeContext->MapKey(MoveAction, EKeys::D);
		PrototypeContext->MapKey(MoveAction, EKeys::Right);
	}
	if (PrototypeContext && JumpAction)
	{
		PrototypeContext->MapKey(JumpAction, EKeys::SpaceBar);
	}
	if (PrototypeContext && SwapAction)
	{
		PrototypeContext->MapKey(SwapAction, EKeys::E);
	}
}

void ACLPVessel::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	ApplyPresetSize();
	ApplyMovementPreset();
}

void ACLPVessel::BeginPlay()
{
	Super::BeginPlay();
	ApplyPresetSize();
	ApplyMovementPreset();
	ApplyRuntimeMaterials();
}

void ACLPVessel::ApplyPresetSize()
{
	SetVesselSize(
		PresetCapsuleRadius,
		PresetCapsuleHeight * 0.5f,
		PresetBodyMeshScale,
		PresetBodyMeshOffset,
		PresetFlameMeshOffset,
		PresetFlameMeshScale);
}

void ACLPVessel::ApplyMovementPreset()
{
	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		Movement->MaxWalkSpeed = FMath::Max(1.f, PresetMoveSpeed);
	}
}

void ACLPVessel::ApplyRuntimeMaterials()
{
	UMaterial* Parent = CLPShapes::BasicMaterial();
	if (!Parent)
	{
		return;
	}

	if (!BodyMaterial)
	{
		BodyMaterial = UMaterialInstanceDynamic::Create(Parent, this);
	}
	if (!FlameMaterial)
	{
		FlameMaterial = UMaterialInstanceDynamic::Create(Parent, this);
	}

	BodyMaterial->SetVectorParameterValue(TEXT("BaseColor"), FLinearColor(DesiredBodyColor));
	FlameMaterial->SetVectorParameterValue(TEXT("BaseColor"), FLinearColor(1.f, 0.55f, 0.12f));

	BodyMesh->SetMaterial(0, BodyMaterial);
	DetailMesh->SetMaterial(0, BodyMaterial);
	FlameMesh->SetMaterial(0, FlameMaterial);
}

float ACLPVessel::GetCapsuleRadius() const
{
	return GetCapsuleComponent()->GetScaledCapsuleRadius();
}

float ACLPVessel::GetCapsuleHalfHeight() const
{
	return GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
}

void ACLPVessel::SetVesselSize(float InCapsuleRadius, float InCapsuleHalfHeight, const FVector& InBodyScale, const FVector& InBodyOffset, const FVector& InFlameOffset, const FVector& InFlameScale)
{
	const float Radius = FMath::Max(1.f, InCapsuleRadius);
	const float HalfHeight = FMath::Max(1.f, InCapsuleHalfHeight);

	GetCapsuleComponent()->SetCapsuleSize(Radius, HalfHeight);

	// Keep the Blueprint-editable defaults in sync so the Class Defaults panel,
	// the construction script preview and runtime all show the same numbers.
	PresetCapsuleRadius = Radius;
	PresetCapsuleHalfHeight = HalfHeight;
	PresetCapsuleHeight = HalfHeight * 2.f;
	PresetBodyMeshScale = InBodyScale;
	PresetBodyMeshOffset = InBodyOffset;
	PresetFlameMeshOffset = InFlameOffset;
	PresetFlameMeshScale = InFlameScale;

	if (CollisionPreview)
	{
		if (UStaticMesh* CapsuleMesh = CollisionPreview->GetStaticMesh())
		{
			const FVector Extent = CapsuleMesh->GetBounds().BoxExtent;
			CollisionPreview->SetRelativeLocation(FVector::ZeroVector);
			CollisionPreview->SetRelativeScale3D(FVector(
				Radius / FMath::Max(Extent.X, 0.01f),
				Radius / FMath::Max(Extent.Y, 0.01f),
				HalfHeight / FMath::Max(Extent.Z, 0.01f)));
		}
	}

	if (BodyMesh)
	{
		BodyMesh->SetRelativeLocation(InBodyOffset);
		BodyMesh->SetRelativeScale3D(InBodyScale);
	}

	if (FlameMesh)
	{
		FlameMesh->SetRelativeLocation(InFlameOffset);
		FlameMesh->SetRelativeScale3D(InFlameScale);
	}
}

void ACLPVessel::ApplyConfigData(UCLPVesselConfig* Config)
{
	if (!Config)
	{
		return;
	}

	ConfigureVessel(
		Config->VesselKind,
		Config->DisplayName,
		Config->MoveSpeed,
		Config->JumpVelocity,
		Config->LightRadius,
		Config->BodyColor);

	GetCharacterMovement()->GravityScale = Config->GravityScale;
	SetVesselMass(Config->Mass);
	SetVesselSize(
		Config->CapsuleRadius,
		Config->CapsuleHalfHeight,
		Config->BodyMeshScale,
		Config->BodyMeshOffset,
		Config->FlameMeshOffset,
		Config->FlameMeshScale);
	ApplyRuntimeMaterials();
}

void ACLPVessel::ApplyCapsuleForKind(ECLPVesselKind InKind)
{
	float Radius = 20.f;
	float HalfHeight = 44.f;

	switch (InKind)
	{
	case ECLPVesselKind::Candle:       Radius = 20.f; HalfHeight = 44.f; break;
	case ECLPVesselKind::Match:        Radius = 5.f;  HalfHeight = 20.f; break;
	case ECLPVesselKind::Lantern:      Radius = 18.f; HalfHeight = 36.f; break;
	case ECLPVesselKind::Torch:        Radius = 9.f;  HalfHeight = 44.f; break;
	case ECLPVesselKind::Wisp:         Radius = 10.f; HalfHeight = 20.f; break;
	case ECLPVesselKind::WaxPuddle:    Radius = 26.f; HalfHeight = 12.f; break;
	case ECLPVesselKind::Beacon:       Radius = 24.f; HalfHeight = 40.f; break;
	case ECLPVesselKind::EmberStone:   Radius = 22.f; HalfHeight = 26.f; break;
	case ECLPVesselKind::MirrorShard:  Radius = 6.f;  HalfHeight = 40.f; break;
	case ECLPVesselKind::IncenseStick: Radius = 5.f;  HalfHeight = 44.f; break;
	}

	GetCapsuleComponent()->InitCapsuleSize(Radius, HalfHeight);
}

void ACLPVessel::ConfigureVessel(ECLPVesselKind InKind, const FString& InLabel, float InMoveSpeed, float InJumpVelocity, float InLightRadius, const FColor& InColour)
{
	Kind = InKind;
	VesselLabel = InLabel;
	LightRadius = InLightRadius;
	PresetMoveSpeed = InMoveSpeed;
	ApplyCapsuleForKind(InKind);

	GetCharacterMovement()->MaxWalkSpeed = InMoveSpeed;
	GetCharacterMovement()->JumpZVelocity = InJumpVelocity;
	FlameLight->SetAttenuationRadius(LightRadius);

	NameLabel->SetText(FText::FromString(VesselLabel));

	UStaticMesh* Cube = CLPShapes::Cube();
	UStaticMesh* Cylinder = CLPShapes::Cylinder();
	UStaticMesh* Sphere = CLPShapes::Sphere();
	UStaticMesh* Cone = CLPShapes::Cone();

	BodyMesh->SetStaticMesh(Cube);
	BodyMesh->SetRelativeLocation(FVector::ZeroVector);
	BodyMesh->SetRelativeRotation(FRotator::ZeroRotator);
	BodyMesh->SetRelativeScale3D(FVector(1.f));

	DetailMesh->SetStaticMesh(nullptr);
	DetailMesh->SetRelativeLocation(FVector::ZeroVector);
	DetailMesh->SetRelativeRotation(FRotator::ZeroRotator);
	DetailMesh->SetRelativeScale3D(FVector(1.f));
	DetailMesh->SetVisibility(false);

	FlameMesh->SetStaticMesh(Sphere);
	FlameMesh->SetRelativeLocation(FVector(0.f, 0.f, 42.f));
	FlameMesh->SetRelativeScale3D(FVector(0.18f, 0.18f, 0.26f));

	// Constructor-safe: assign a static engine material so Blueprint previews and
	// serialized classes never own private dynamic instances. Runtime colour is
	// applied in ApplyRuntimeMaterials().
	DesiredBodyColor = InColour;
	UMaterial* PreviewMaterial = CLPShapes::BasicMaterial();
	BodyMesh->SetMaterial(0, PreviewMaterial);
	DetailMesh->SetMaterial(0, PreviewMaterial);
	FlameMesh->SetMaterial(0, PreviewMaterial);

	switch (Kind)
	{
	case ECLPVesselKind::Candle:
		BodyMesh->SetStaticMesh(Cylinder);
		BodyMesh->SetRelativeLocation(FVector(0.f, 0.f, -4.f));
		BodyMesh->SetRelativeScale3D(FVector(0.38f, 0.38f, 1.15f));
		FlameMesh->SetRelativeLocation(FVector(0.f, 0.f, 62.f));
		break;

	case ECLPVesselKind::Match:
		BodyMesh->SetRelativeLocation(FVector(0.f, 0.f, -4.f));
		BodyMesh->SetRelativeScale3D(FVector(0.12f, 0.12f, 0.72f));
		DetailMesh->SetStaticMesh(Sphere);
		DetailMesh->SetRelativeLocation(FVector(0.f, 0.f, 30.f));
		DetailMesh->SetRelativeScale3D(FVector(0.10f));
		DetailMesh->SetVisibility(true);
		FlameMesh->SetRelativeLocation(FVector(0.f, 0.f, 42.f));
		FlameMesh->SetRelativeScale3D(FVector(0.11f, 0.11f, 0.16f));
		break;

	case ECLPVesselKind::Lantern:
		BodyMesh->SetRelativeLocation(FVector(0.f, 0.f, -4.f));
		BodyMesh->SetRelativeScale3D(FVector(0.62f, 0.50f, 0.90f));
		DetailMesh->SetStaticMesh(Cylinder);
		DetailMesh->SetRelativeLocation(FVector(0.f, 0.f, 42.f));
		DetailMesh->SetRelativeScale3D(FVector(0.70f, 0.55f, 0.12f));
		DetailMesh->SetVisibility(true);
		FlameMesh->SetRelativeLocation(FVector(0.f, 0.f, 12.f));
		FlameMesh->SetRelativeScale3D(FVector(0.14f, 0.14f, 0.20f));
		break;

	case ECLPVesselKind::Torch:
		BodyMesh->SetStaticMesh(Cylinder);
		BodyMesh->SetRelativeLocation(FVector(0.f, 0.f, -4.f));
		BodyMesh->SetRelativeScale3D(FVector(0.12f, 0.12f, 1.10f));
		DetailMesh->SetStaticMesh(Cone);
		DetailMesh->SetRelativeLocation(FVector(0.f, 0.f, 50.f));
		DetailMesh->SetRelativeScale3D(FVector(0.34f, 0.34f, 0.30f));
		DetailMesh->SetVisibility(true);
		FlameMesh->SetRelativeLocation(FVector(0.f, 0.f, 72.f));
		FlameMesh->SetRelativeScale3D(FVector(0.22f, 0.22f, 0.32f));
		break;

	case ECLPVesselKind::Wisp:
		BodyMesh->SetStaticMesh(Sphere);
		BodyMesh->SetRelativeLocation(FVector(0.f, 0.f, 8.f));
		BodyMesh->SetRelativeScale3D(FVector(0.22f));
		FlameMesh->SetRelativeLocation(FVector(0.f, 0.f, 8.f));
		FlameMesh->SetRelativeScale3D(FVector(0.30f));
		break;

	case ECLPVesselKind::WaxPuddle:
		BodyMesh->SetStaticMesh(Cylinder);
		BodyMesh->SetRelativeLocation(FVector(0.f, 0.f, -38.f));
		BodyMesh->SetRelativeScale3D(FVector(0.95f, 0.95f, 0.12f));
		FlameMesh->SetRelativeLocation(FVector(0.f, 0.f, -28.f));
		FlameMesh->SetRelativeScale3D(FVector(0.14f, 0.14f, 0.10f));
		break;

	case ECLPVesselKind::Beacon:
		BodyMesh->SetRelativeLocation(FVector(0.f, 0.f, -26.f));
		BodyMesh->SetRelativeScale3D(FVector(0.80f, 0.80f, 0.40f));
		DetailMesh->SetStaticMesh(Cylinder);
		DetailMesh->SetRelativeLocation(FVector(0.f, 0.f, 12.f));
		DetailMesh->SetRelativeScale3D(FVector(0.16f, 0.16f, 0.80f));
		DetailMesh->SetVisibility(true);
		FlameMesh->SetRelativeLocation(FVector(0.f, 0.f, 58.f));
		FlameMesh->SetRelativeScale3D(FVector(0.28f, 0.28f, 0.28f));
		break;

	case ECLPVesselKind::EmberStone:
		BodyMesh->SetStaticMesh(Sphere);
		BodyMesh->SetRelativeLocation(FVector(0.f, 0.f, -2.f));
		BodyMesh->SetRelativeScale3D(FVector(0.46f));
		DetailMesh->SetStaticMesh(Cube);
		DetailMesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
		DetailMesh->SetRelativeRotation(FRotator(0.f, 45.f, 20.f));
		DetailMesh->SetRelativeScale3D(FVector(0.70f, 0.10f, 0.10f));
		DetailMesh->SetVisibility(true);
		FlameMesh->SetRelativeLocation(FVector(0.f, 0.f, 10.f));
		FlameMesh->SetRelativeScale3D(FVector(0.12f));
		break;

	case ECLPVesselKind::MirrorShard:
		BodyMesh->SetRelativeLocation(FVector(0.f, 0.f, 2.f));
		BodyMesh->SetRelativeRotation(FRotator(0.f, 18.f, 16.f));
		BodyMesh->SetRelativeScale3D(FVector(0.12f, 0.60f, 0.90f));
		DetailMesh->SetStaticMesh(Cube);
		DetailMesh->SetRelativeLocation(FVector(0.f, 0.f, -42.f));
		DetailMesh->SetRelativeScale3D(FVector(0.50f, 0.30f, 0.10f));
		DetailMesh->SetVisibility(true);
		FlameMesh->SetRelativeLocation(FVector(0.f, 0.f, 46.f));
		FlameMesh->SetRelativeScale3D(FVector(0.12f));
		break;

	case ECLPVesselKind::IncenseStick:
		BodyMesh->SetStaticMesh(Cylinder);
		BodyMesh->SetRelativeLocation(FVector(0.f, 0.f, -4.f));
		BodyMesh->SetRelativeScale3D(FVector(0.08f, 0.08f, 1.f));
		FlameMesh->SetRelativeLocation(FVector(0.f, 0.f, 48.f));
		FlameMesh->SetRelativeScale3D(FVector(0.10f, 0.10f, 0.14f));
		break;
	}

	// Mirror this kind's shape into the Blueprint-editable defaults. OnConstruction
	// then re-applies the same values instead of resetting them to the base cube.
	PresetCapsuleRadius = GetCapsuleComponent()->GetUnscaledCapsuleRadius();
	PresetCapsuleHalfHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	PresetCapsuleHeight = PresetCapsuleHalfHeight * 2.f;
	PresetBodyMeshScale = BodyMesh->GetRelativeScale3D();
	PresetBodyMeshOffset = BodyMesh->GetRelativeLocation();
	PresetFlameMeshOffset = FlameMesh->GetRelativeLocation();
	PresetFlameMeshScale = FlameMesh->GetRelativeScale3D();
}

void ACLPVessel::SetSparkActive(bool bActive)
{
	bSparkActive = bActive;
	FlameMesh->SetVisibility(bActive);
	FlameLight->SetIntensity(bActive ? 8000.f : 0.f);
	NameLabel->SetTextRenderColor(bActive ? FColor(255, 216, 140) : FColor(120, 140, 165));
}

void ACLPVessel::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (!PlayerInputComponent)
	{
		return;
	}

	// Use the project's config-driven legacy bindings only. Enhanced Input assets
	// were inconsistent between PIE and the packaged build (A/D and E).
	PlayerInputComponent->BindAxis(TEXT("CLP_MoveX"), this, &ACLPVessel::MoveX);
	PlayerInputComponent->BindAction(TEXT("CLP_Jump"), IE_Pressed, this, &ACLPVessel::JumpPressed);
	PlayerInputComponent->BindAction(TEXT("CLP_Jump"), IE_Released, this, &ACLPVessel::JumpReleased);
	PlayerInputComponent->BindAction(TEXT("CLP_Swap"), IE_Pressed, this, &ACLPVessel::SwapPressed);
}

void ACLPVessel::UnPossessed()
{
	if (CachedInputSubsystem && PrototypeContext)
	{
		CachedInputSubsystem->RemoveMappingContext(PrototypeContext);
	}
	CachedInputSubsystem = nullptr;

	Super::UnPossessed();
}

UEnhancedInputLocalPlayerSubsystem* ACLPVessel::GetInputSubsystem() const
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	ULocalPlayer* LocalPlayer = PC ? PC->GetLocalPlayer() : nullptr;
	return LocalPlayer ? ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer) : nullptr;
}

void ACLPVessel::MoveEnhanced(const FInputActionValue& Value)
{
	MoveX(Value.Get<FVector2D>().X);
}

void ACLPVessel::MoveX(float Value)
{
	if (UGameplayStatics::IsGamePaused(this) || FMath::Abs(Value) <= KINDA_SMALL_NUMBER)
	{
		return;
	}

	if (FMath::Abs(Value) > KINDA_SMALL_NUMBER)
	{
		if (!bHasLoggedMovement)
		{
			bHasLoggedMovement = true;
			UE_LOG(LogTemp, Display, TEXT("CLP Move input received: %s X=%.2f"), *VesselLabel, Value);
		}
		AddMovementInput(FVector::XAxisVector, Value);
	}
}

void ACLPVessel::JumpPressed()
{
	if (!UGameplayStatics::IsGamePaused(this))
	{
		Jump();
	}
}

void ACLPVessel::JumpReleased()
{
	if (!UGameplayStatics::IsGamePaused(this))
	{
		StopJumping();
	}
}

void ACLPVessel::SwapPressed()
{
	if (UGameplayStatics::IsGamePaused(this))
	{
		return;
	}

	UE_LOG(LogTemp, Display, TEXT("CLP Swap input received: %s"), *VesselLabel);
	if (ACLPPrototypeGameMode* GM = Cast<ACLPPrototypeGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		GM->AdvanceSpark();
	}
}
