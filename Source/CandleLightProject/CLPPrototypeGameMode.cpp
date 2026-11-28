#include "CLPPrototypeGameMode.h"

#include "ACLGoalCorner.h"
#include "ACLPLevelExitTrigger.h"
#include "ACLPPausePlayerController.h"
#include "ACLPVesselSpawnPoint.h"
#include "CLPVessel.h"
#include "CLPVesselCandle.h"
#include "CLPVesselConfig.h"
#include "Camera/PlayerCameraManager.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/DirectionalLight.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/World.h"
#include "GameFramework/Controller.h"
#include "GameFramework/HUD.h"
#include "GameFramework/Info.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"

namespace
{
	struct FVesselDef
	{
		ECLPVesselKind Kind;
		const TCHAR* Label;
		float Speed;
		float Jump;
		float LightRadius;
		FColor Colour;
		float Mass;
	};

	static const FVesselDef& GetVesselDef(int32 Index)
	{
		static const FVesselDef Defs[10] = {
			{ ECLPVesselKind::Candle,       TEXT("CANDLE"),        300.f, 720.f, 900.f, FColor(240, 220, 180), 120.f },
			{ ECLPVesselKind::Match,        TEXT("MATCH"),         430.f, 470.f, 260.f, FColor(190, 120, 60),  8.f   },
			{ ECLPVesselKind::Lantern,      TEXT("LANTERN"),       280.f, 420.f, 620.f, FColor(140, 170, 200), 45.f  },
			{ ECLPVesselKind::Torch,        TEXT("TORCH"),         300.f, 450.f, 700.f, FColor(150, 90, 50),   35.f  },
			{ ECLPVesselKind::Wisp,         TEXT("WISP"),          470.f, 530.f, 320.f, FColor(230, 240, 255), 1.f   },
			{ ECLPVesselKind::WaxPuddle,    TEXT("WAX PUDDLE"),    240.f, 330.f, 140.f, FColor(230, 170, 80),  30.f  },
			{ ECLPVesselKind::Beacon,       TEXT("BEACON"),        160.f, 330.f, 800.f, FColor(110, 130, 160), 200.f },
			{ ECLPVesselKind::EmberStone,   TEXT("EMBER STONE"),   220.f, 350.f, 240.f, FColor(130, 65, 50),   150.f },
			{ ECLPVesselKind::MirrorShard,  TEXT("MIRROR SHARD"),  360.f, 450.f, 380.f, FColor(190, 210, 225), 12.f  },
			{ ECLPVesselKind::IncenseStick, TEXT("INCENSE STICK"), 330.f, 430.f, 300.f, FColor(110, 90, 70),   4.f   }
		};
		return Defs[FMath::Clamp(Index, 0, 9)];
	}

	static int32 FindDefIndex(ECLPVesselKind Kind)
	{
		for (int32 Index = 0; Index < 10; ++Index)
		{
			if (GetVesselDef(Index).Kind == Kind)
			{
				return Index;
			}
		}
		return 0;
	}

	static UMaterial* PrototypeMaterial()
	{
		UMaterial* Mat = LoadObject<UMaterial>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
		return Mat ? Mat : UMaterial::GetDefaultMaterial(MD_Surface);
	}

	static TSubclassOf<ACLPVessel> LoadVesselBlueprintClass(ECLPVesselKind Kind)
	{
		const TCHAR* Path = nullptr;
		switch (Kind)
		{
		case ECLPVesselKind::Candle:       Path = TEXT("/Game/Candlelight/Blueprints/Vessels/BP_Vessel_Candle.BP_Vessel_Candle_C"); break;
		case ECLPVesselKind::Match:        Path = TEXT("/Game/Candlelight/Blueprints/Vessels/BP_Vessel_Match.BP_Vessel_Match_C"); break;
		case ECLPVesselKind::Lantern:      Path = TEXT("/Game/Candlelight/Blueprints/Vessels/BP_Vessel_Lantern.BP_Vessel_Lantern_C"); break;
		case ECLPVesselKind::Torch:        Path = TEXT("/Game/Candlelight/Blueprints/Vessels/BP_Vessel_Torch.BP_Vessel_Torch_C"); break;
		case ECLPVesselKind::Wisp:         Path = TEXT("/Game/Candlelight/Blueprints/Vessels/BP_Vessel_Wisp.BP_Vessel_Wisp_C"); break;
		case ECLPVesselKind::WaxPuddle:    Path = TEXT("/Game/Candlelight/Blueprints/Vessels/BP_Vessel_WaxPuddle.BP_Vessel_WaxPuddle_C"); break;
		case ECLPVesselKind::Beacon:       Path = TEXT("/Game/Candlelight/Blueprints/Vessels/BP_Vessel_Beacon.BP_Vessel_Beacon_C"); break;
		case ECLPVesselKind::EmberStone:   Path = TEXT("/Game/Candlelight/Blueprints/Vessels/BP_Vessel_EmberStone.BP_Vessel_EmberStone_C"); break;
		case ECLPVesselKind::MirrorShard:  Path = TEXT("/Game/Candlelight/Blueprints/Vessels/BP_Vessel_MirrorShard.BP_Vessel_MirrorShard_C"); break;
		case ECLPVesselKind::IncenseStick: Path = TEXT("/Game/Candlelight/Blueprints/Vessels/BP_Vessel_IncenseStick.BP_Vessel_IncenseStick_C"); break;
		}

		if (!Path)
		{
			return nullptr;
		}

		UClass* BlueprintClass = LoadObject<UClass>(nullptr, Path);
		return (BlueprintClass && BlueprintClass->IsChildOf(ACLPVessel::StaticClass())) ? BlueprintClass : nullptr;
	}
}

ACLPPrototypeGameMode::ACLPPrototypeGameMode()
{
	DefaultPawnClass = nullptr;
	PlayerControllerClass = ACLPPausePlayerController::StaticClass();
}

APawn* ACLPPrototypeGameMode::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	return nullptr;
}

void ACLPPrototypeGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (!CandleConfig)
	{
		CandleConfig = LoadObject<UCLPVesselConfig>(nullptr, TEXT("/Game/Candlelight/Data/DA_Candle_Vessel.DA_Candle_Vessel"));
	}
	if (!MatchConfig)
	{
		MatchConfig = LoadObject<UCLPVesselConfig>(nullptr, TEXT("/Game/Candlelight/Data/DA_Match_Vessel.DA_Match_Vessel"));
	}

	if (bBuildRuntimeTestLane)
	{
		ClearTemplateActors();
		BuildWorld();
	}
	else
	{
		SpawnVesselsFromLevel();
		EnsureLevelExitTrigger();
	}

	// In PIE the PlayerController/PlayerState are not safe to possess during GameMode
	// BeginPlay. Wait one tick so framework delegates are fully bound.
	GetWorldTimerManager().SetTimerForNextTick(this, &ACLPPrototypeGameMode::PossessActive);
}

void ACLPPrototypeGameMode::ClearTemplateActors()
{
	TArray<AActor*> ExistingActors;
	UGameplayStatics::GetAllActorsOfClass(this, AActor::StaticClass(), ExistingActors);
	for (AActor* Actor : ExistingActors)
	{
		// Keep engine/framework actors alive. AInfo covers PlayerState and GameStateBase;
		// destroying PlayerState breaks AController::Possess on the next tick.
		if (!Actor || Actor == this || Actor->IsA<AGameModeBase>() || Actor->IsA<AController>() ||
			Actor->IsA<AHUD>() || Actor->IsA<AInfo>() || Actor->IsA<APlayerCameraManager>())
		{
			continue;
		}
		Actor->Destroy();
	}
}

void ACLPPrototypeGameMode::BuildWorld()
{
	Vessels.Empty();

	// Low fill light so the basic shapes are visible even after clearing the template map.
	ADirectionalLight* Light = GetWorld()->SpawnActor<ADirectionalLight>(
		ADirectionalLight::StaticClass(), FVector::ZeroVector, FRotator(-50.f, 90.f, 0.f));
	if (Light)
	{
		if (UDirectionalLightComponent* DirLight = Light->FindComponentByClass<UDirectionalLightComponent>())
		{
			DirLight->SetIntensity(2.f);
			DirLight->SetLightColor(FLinearColor(0.65f, 0.75f, 1.f));
		}
	}

	// Test lane: floor, back wall, end stops, a few jump platforms, plate and gate.
	SpawnBlock(FVector(1200.f, 0.f, -50.f), FVector(64.f, 6.f, 1.f), FColor(70, 82, 100));
	SpawnBlock(FVector(1200.f, 320.f, 320.f), FVector(64.f, 1.f, 6.4f), FColor(30, 38, 52));
	SpawnBlock(FVector(-600.f, 0.f, 300.f), FVector(1.f, 6.f, 6.f), FColor(30, 38, 52));
	SpawnBlock(FVector(3000.f, 0.f, 300.f), FVector(1.f, 6.f, 6.f), FColor(30, 38, 52));

	SpawnBlock(FVector(650.f, 0.f, 120.f), FVector(4.f, 4.f, 0.5f), FColor(90, 105, 130));
	SpawnBlock(FVector(1200.f, 0.f, 220.f), FVector(4.f, 4.f, 0.5f), FColor(90, 105, 130));
	SpawnBlock(FVector(1750.f, 0.f, 140.f), FVector(5.f, 4.f, 0.5f), FColor(90, 105, 130));
	SpawnBlock(FVector(2250.f, 0.f, 260.f), FVector(4.f, 4.f, 0.5f), FColor(90, 105, 130));

	SpawnBlock(FVector(950.f, 0.f, 20.f), FVector(2.f, 2.f, 0.2f), FColor(210, 175, 80));
	SpawnBlock(FVector(1500.f, 0.f, 200.f), FVector(1.f, 2.f, 4.f), FColor(70, 95, 130));

	for (int32 Index = 0; Index < 10; ++Index)
	{
		SpawnVesselFromDef(Index, FVector(static_cast<float>(Index) * 260.f, 0.f, 100.f));
	}

	if (Vessels.Num() > 0)
	{
		ActiveVesselIndex = 0;
		Vessels[ActiveVesselIndex]->SetSparkActive(true);
		UE_LOG(LogTemp, Display, TEXT("CLP Prototype ready: %d vessels, active=%s"),
			Vessels.Num(), *Vessels[ActiveVesselIndex]->GetVesselLabel());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("CLP Prototype failed to spawn vessels"));
	}

	SpawnHelpText();
}

void ACLPPrototypeGameMode::SpawnVesselsFromLevel()
{
	Vessels.Empty();

	TArray<AActor*> SpawnActors;
	UGameplayStatics::GetAllActorsOfClass(this, ACLPVesselSpawnPoint::StaticClass(), SpawnActors);

	ACLPVesselSpawnPoint* Points[10] = { nullptr };
	for (AActor* Actor : SpawnActors)
	{
		if (ACLPVesselSpawnPoint* Point = Cast<ACLPVesselSpawnPoint>(Actor))
		{
			Points[FindDefIndex(Point->VesselKind)] = Point;
		}
	}

	int32 SpawnedCount = 0;
	for (int32 Index = 0; Index < 10; ++Index)
	{
		if (!Points[Index])
		{
			continue;
		}
		ACLPVessel* Vessel = SpawnVesselFromDef(
			Index, Points[Index]->GetActorLocation(), Points[Index]->VesselClassOverride);
		if (Vessel && Points[Index]->bOverrideCollisionAndSize)
		{
			Vessel->SetVesselSize(
				Points[Index]->CapsuleRadius,
				Points[Index]->CapsuleHalfHeight,
				Points[Index]->BodyMeshScale,
				Points[Index]->BodyMeshOffset,
				Points[Index]->FlameMeshOffset,
				Points[Index]->FlameMeshScale);
		}
		++SpawnedCount;
	}

	if (Vessels.Num() > 0)
	{
		ActiveVesselIndex = 0;
		Vessels[0]->SetSparkActive(true);
		UE_LOG(LogTemp, Display, TEXT("CLP Level vessels spawned: %d from %d spawn points, active=%s"),
			Vessels.Num(), SpawnedCount, *Vessels[0]->GetVesselLabel());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("CLP Level has no ACLPVesselSpawnPoint actors"));
	}
}

void ACLPPrototypeGameMode::EnsureLevelExitTrigger()
{
	TArray<AActor*> ExistingExits;
	UGameplayStatics::GetAllActorsOfClass(this, ACLPLevelExitTrigger::StaticClass(), ExistingExits);
	if (ExistingExits.Num() > 0)
	{
		return;
	}

	TArray<AActor*> GoalActors;
	UGameplayStatics::GetAllActorsOfClass(this, ACLGoalCorner::StaticClass(), GoalActors);
	if (GoalActors.Num() == 0)
	{
		return;
	}

	const FString MapName = GetWorld()->GetMapName();
	FName NextLevel = NAME_None;
	if (MapName.Contains(TEXT("Level_01")))
	{
		NextLevel = TEXT("CL_Level_02_MatchSide");
	}
	else if (MapName.Contains(TEXT("Level_02")))
	{
		NextLevel = TEXT("CL_Level_03_LanternSide");
	}

	if (NextLevel.IsNone())
	{
		return;
	}

	ACLGoalCorner* Goal = Cast<ACLGoalCorner>(GoalActors[0]);
	const FVector ExitLocation = Goal->GetActorLocation() + FVector(250.f, 0.f, -20.f);
	ACLPLevelExitTrigger* Exit = GetWorld()->SpawnActor<ACLPLevelExitTrigger>(
		ACLPLevelExitTrigger::StaticClass(), ExitLocation, FRotator::ZeroRotator);
	if (Exit)
	{
		Exit->NextLevelName = NextLevel;
		UE_LOG(LogTemp, Display, TEXT("CLP Level exit spawned at %s -> %s"),
			*ExitLocation.ToString(), *NextLevel.ToString());
	}
}

ACLPVessel* ACLPPrototypeGameMode::SpawnVesselFromDef(int32 DefIndex, const FVector& Location, TSubclassOf<ACLPVessel> ClassOverride)
{
	const FVesselDef& Def = GetVesselDef(DefIndex);

	TSubclassOf<ACLPVessel> VesselClass = ClassOverride;
	if (!VesselClass)
	{
		// Authored maps do not need hand-patched spawn points: every vessel kind
		// resolves to its generated Blueprint automatically.
		VesselClass = LoadVesselBlueprintClass(Def.Kind);
	}

	const bool bUsingBlueprint = VesselClass != nullptr;
	if (!VesselClass)
	{
		VesselClass = ACLPVessel::StaticClass();
		if (Def.Kind == ECLPVesselKind::Candle)
		{
			VesselClass = ACLPVesselCandle::StaticClass();
		}
	}

	ACLPVessel* Vessel = GetWorld()->SpawnActor<ACLPVessel>(VesselClass, Location, FRotator::ZeroRotator);
	if (!Vessel)
	{
		return nullptr;
	}

	// A Blueprint class owns its own Class Defaults. A native class still uses the
	// tuning data assets. Set bUseTuningAsset on a Blueprint to opt back in.
	UCLPVesselConfig* Config = nullptr;
	if (Vessel->ShouldUseTuningAsset())
	{
		Config = Vessel->GetTuningAsset();
		if (!Config)
		{
			Config = (Def.Kind == ECLPVesselKind::Candle) ? CandleConfig
				: (Def.Kind == ECLPVesselKind::Match) ? MatchConfig : nullptr;
		}
	}
	else if (!bUsingBlueprint)
	{
		Config = (Def.Kind == ECLPVesselKind::Candle) ? CandleConfig
			: (Def.Kind == ECLPVesselKind::Match) ? MatchConfig : nullptr;
	}

	if (Config)
	{
		Vessel->ApplyConfigData(Config);
	}
	else if (!bUsingBlueprint)
	{
		Vessel->ConfigureVessel(Def.Kind, Def.Label, Def.Speed, Def.Jump, Def.LightRadius, Def.Colour);
		Vessel->SetVesselMass(Def.Mass);
	}

	if (ACLPVesselCandle* Candle = Cast<ACLPVesselCandle>(Vessel))
	{
		if (CandleConfig)
		{
			Candle->CandleConfig = CandleConfig;
			Candle->ApplyConfig();
			Candle->RefillWax();
		}
	}

	Vessel->SetSparkActive(false);
	Vessels.Add(Vessel);

	UE_LOG(LogTemp, Display,
		TEXT("CLP Vessel %s class=%s source=%s moveSpeed=%.1f collision: radius=%.1f halfHeight=%.1f (height=%.1f)"),
		*Vessel->GetVesselLabel(),
		*Vessel->GetClass()->GetName(),
		bUsingBlueprint ? TEXT("BlueprintDefaults") : (Vessel->ShouldUseTuningAsset() ? TEXT("TuningAsset") : TEXT("NativePreset")),
		Vessel->GetMoveSpeed(),
		Vessel->GetCapsuleRadius(),
		Vessel->GetCapsuleHalfHeight(),
		Vessel->GetCapsuleHalfHeight() * 2.f);

	return Vessel;
}

void ACLPPrototypeGameMode::SpawnBlock(const FVector& Location, const FVector& Scale, const FColor& Colour)
{
	AStaticMeshActor* Block = GetWorld()->SpawnActor<AStaticMeshActor>(
		AStaticMeshActor::StaticClass(), Location, FRotator::ZeroRotator);
	if (!Block)
	{
		return;
	}

	UStaticMesh* Cube = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
	Block->GetStaticMeshComponent()->SetStaticMesh(Cube);
	Block->SetActorScale3D(Scale);

	UMaterialInstanceDynamic* Mat = UMaterialInstanceDynamic::Create(PrototypeMaterial(), Block);
	Mat->SetVectorParameterValue(TEXT("BaseColor"), FLinearColor(Colour));
	Block->GetStaticMeshComponent()->SetMaterial(0, Mat);
}

void ACLPPrototypeGameMode::SpawnHelpText()
{
	AStaticMeshActor* Anchor = GetWorld()->SpawnActor<AStaticMeshActor>(
		AStaticMeshActor::StaticClass(), FVector(1200.f, 0.f, 540.f), FRotator::ZeroRotator);
	if (!Anchor)
	{
		return;
	}

	UStaticMesh* Cube = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
	Anchor->GetStaticMeshComponent()->SetStaticMesh(Cube);
	Anchor->SetActorScale3D(FVector(0.01f));

	UTextRenderComponent* Text = NewObject<UTextRenderComponent>(Anchor, TEXT("HelpText"));
	Text->SetupAttachment(Anchor->GetStaticMeshComponent());
	Text->SetRelativeLocation(FVector(0.f, -60.f, 0.f));
	Text->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	Text->SetWorldSize(26.f);
	Text->SetTextRenderColor(FColor(255, 220, 160));
	Text->SetHorizontalAlignment(EHTA_Center);
	Text->SetText(FText::FromString(TEXT("A/D or LEFT/RIGHT : MOVE X    SPACE : JUMP    E : SWAP SPARK -> NEXT VESSEL")));
	Text->RegisterComponent();
}

void ACLPPrototypeGameMode::PossessActive()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (!PC)
	{
		GetWorldTimerManager().SetTimerForNextTick(this, &ACLPPrototypeGameMode::PossessActive);
		return;
	}

	if (Vessels.IsValidIndex(ActiveVesselIndex))
	{
		PC->Possess(Vessels[ActiveVesselIndex]);
	}
}

ECLPVesselKind ACLPPrototypeGameMode::GetActiveVesselKind() const
{
	return Vessels.IsValidIndex(ActiveVesselIndex) ? Vessels[ActiveVesselIndex]->GetVesselKind() : ECLPVesselKind::Candle;
}

void ACLPPrototypeGameMode::ActivateVessel(int32 NewIndex, bool bTeleportTargetToCurrent)
{
	if (!Vessels.IsValidIndex(NewIndex) || NewIndex == ActiveVesselIndex)
	{
		return;
	}

	ACLPVessel* Current = Vessels[ActiveVesselIndex];
	ACLPVessel* Next = Vessels[NewIndex];

	if (bTeleportTargetToCurrent && Current && Next)
	{
		FVector TargetLocation = Current->GetActorLocation();
		TargetLocation.Z += Next->GetCapsuleHalfHeight() - Current->GetCapsuleHalfHeight();
		Next->SetActorLocation(TargetLocation, false, nullptr, ETeleportType::TeleportPhysics);
	}

	Current->SetSparkActive(false);
	ActiveVesselIndex = NewIndex;
	Next->SetSparkActive(true);
	PossessActive();

	UE_LOG(LogTemp, Display, TEXT("CLP Spark swapped to %s (%d/%d)"),
		*Next->GetVesselLabel(), ActiveVesselIndex + 1, Vessels.Num());
}

void ACLPPrototypeGameMode::AdvanceSpark()
{
	if (Vessels.Num() == 0)
	{
		return;
	}

	ActivateVessel((ActiveVesselIndex + 1) % Vessels.Num(), bTeleportOnSwap);
}

void ACLPPrototypeGameMode::SetActiveVesselKind(ECLPVesselKind Kind)
{
	for (int32 Index = 0; Index < Vessels.Num(); ++Index)
	{
		if (Vessels[Index] && Vessels[Index]->GetVesselKind() == Kind)
		{
			ActivateVessel(Index, true);
			return;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("CLP Could not find requested vessel in this level"));
}
