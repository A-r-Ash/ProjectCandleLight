#include "CLPLevelAuthoringCommandlet.h"

#if WITH_EDITOR

#include "ACLGoalCorner.h"
#include "ACLPLevelExitTrigger.h"
#include "ACLPRoomGameMode.h"
#include "ACLSparkReturn.h"
#include "ACLPVesselSpawnPoint.h"
#include "CLPVessel.h"
#include "CLPVesselConfig.h"
#include "CLPVesselPresets.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/Blueprint.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "Engine/DirectionalLight.h"
#include "Engine/PointLight.h"
#include "Engine/PostProcessVolume.h"
#include "Engine/SkyLight.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/TextRenderActor.h"
#include "Engine/World.h"
#include "Editor.h"
#include "EngineUtils.h"
#include "FileHelpers.h"
#include "GameFramework/WorldSettings.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Misc/PackageName.h"
#include "UObject/SavePackage.h"
#include "UObject/UObjectGlobals.h"

namespace
{
	static bool SaveAuthoredPackage(UPackage* Package, UObject* Asset, const FString& LongPackageName, const FString& Extension)
	{
		const FString Filename = FPackageName::LongPackageNameToFilename(LongPackageName, Extension);
		if (Filename.IsEmpty())
		{
			UE_LOG(LogTemp, Warning, TEXT("CLP Authoring: no filename for %s"), *LongPackageName);
			return false;
		}

		FSavePackageArgs SaveArgs;
		SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
		SaveArgs.SaveFlags = SAVE_None;
		SaveArgs.bSlowTask = false;

		const bool bSaved = UPackage::SavePackage(Package, Asset, *Filename, SaveArgs);
		UE_LOG(LogTemp, Display, TEXT("CLP Authoring: saved %s (%d)"), *LongPackageName, bSaved ? 1 : 0);
		return bSaved;
	}

	static UMaterial* BasicParentMaterial()
	{
		UMaterial* Mat = LoadObject<UMaterial>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
		return Mat ? Mat : UMaterial::GetDefaultMaterial(MD_Surface);
	}

	static void CreateMaterialInstance(const TCHAR* AssetName, FLinearColor Colour)
	{
		const FString PackageName = FString::Printf(TEXT("/Game/Candlelight/Materials/%s"), AssetName);
		const FString ObjectPath = PackageName + TEXT(".") + AssetName;

		UMaterialInstanceConstant* Material = LoadObject<UMaterialInstanceConstant>(nullptr, *ObjectPath);
		UPackage* Package = Material ? Cast<UPackage>(Material->GetOutermost()) : CreatePackage(*PackageName);
		if (!Material)
		{
			Material = NewObject<UMaterialInstanceConstant>(
				Package, UMaterialInstanceConstant::StaticClass(), FName(AssetName),
				RF_Public | RF_Standalone);
		}
		Package->MarkPackageDirty();

		Material->SetParentEditorOnly(BasicParentMaterial());
		Material->SetVectorParameterValueEditorOnly(FMaterialParameterInfo(TEXT("BaseColor")), Colour);

		SaveAuthoredPackage(Package, Material, PackageName, FPackageName::GetAssetPackageExtension());
	}

	static void CreateVesselConfig(
		const TCHAR* AssetName,
		ECLPVesselKind Kind,
		const TCHAR* DisplayName,
		float MoveSpeed,
		float JumpVelocity,
		float Mass,
		float LightRadius,
		bool bCanFitSmallGaps,
		FColor BodyColor,
		float CapsuleRadius,
		float CapsuleHalfHeight,
		FVector BodyMeshScale,
		FVector BodyMeshOffset,
		FVector FlameMeshOffset,
		FVector FlameMeshScale)
	{
		const FString PackageName = FString::Printf(TEXT("/Game/Candlelight/Data/%s"), AssetName);
		const FString ObjectPath = PackageName + TEXT(".") + AssetName;

		UCLPVesselConfig* Config = LoadObject<UCLPVesselConfig>(nullptr, *ObjectPath);
		UPackage* Package = Config ? Cast<UPackage>(Config->GetOutermost()) : CreatePackage(*PackageName);
		if (!Config)
		{
			Config = NewObject<UCLPVesselConfig>(
				Package, UCLPVesselConfig::StaticClass(), FName(AssetName),
				RF_Public | RF_Standalone);
		}
		Package->MarkPackageDirty();

		Config->VesselKind = Kind;
		Config->DisplayName = DisplayName;
		Config->MoveSpeed = MoveSpeed;
		Config->JumpVelocity = JumpVelocity;
		Config->GravityScale = 1.8f;
		Config->LightRadius = LightRadius;
		Config->LightIntensity = 8000.f;
		Config->Mass = Mass;
		Config->bCanFitSmallGaps = bCanFitSmallGaps;
		Config->BurnDurationSeconds = 180.f;
		Config->LowWaxThreshold = 0.25f;
		Config->BodyColor = BodyColor;
		Config->CapsuleRadius = CapsuleRadius;
		Config->CapsuleHalfHeight = CapsuleHalfHeight;
		Config->BodyMeshScale = BodyMeshScale;
		Config->BodyMeshOffset = BodyMeshOffset;
		Config->FlameMeshOffset = FlameMeshOffset;
		Config->FlameMeshScale = FlameMeshScale;

		SaveAuthoredPackage(Package, Config, PackageName, FPackageName::GetAssetPackageExtension());
	}

#if WITH_EDITOR
	static TSubclassOf<ACLPVessel> CreateVesselBlueprint(const TCHAR* BlueprintName, UClass* ParentClass)
	{
		const FString PackageName = FString::Printf(TEXT("/Game/Candlelight/Blueprints/Vessels/%s"), BlueprintName);
		const FString ObjectPath = PackageName + TEXT(".") + BlueprintName;

		// Never overwrite a designer-edited Blueprint. Recreating is a full wipe operation.
		if (UBlueprint* Existing = LoadObject<UBlueprint>(nullptr, *ObjectPath))
		{
			UClass* ExistingClass = Existing->GeneratedClass;
			UE_LOG(LogTemp, Display, TEXT("CLP Authoring: kept existing %s"), *PackageName);
			return (ExistingClass && ExistingClass->IsChildOf(ACLPVessel::StaticClass())) ? ExistingClass : nullptr;
		}

		UPackage* Package = CreatePackage(*PackageName);
		Package->MarkPackageDirty();

		UBlueprint* Blueprint = FKismetEditorUtilities::CreateBlueprint(
			ParentClass,
			Package,
			FName(BlueprintName),
			BPTYPE_Normal,
			UBlueprint::StaticClass(),
			UBlueprintGeneratedClass::StaticClass(),
			FName(TEXT("CLPLevelAuthoring")));

		if (!Blueprint)
		{
			UE_LOG(LogTemp, Error, TEXT("CLP Authoring: failed to create Blueprint %s"), *PackageName);
			return nullptr;
		}

		FKismetEditorUtilities::CompileBlueprint(Blueprint);
		SaveAuthoredPackage(Package, Blueprint, PackageName, FPackageName::GetAssetPackageExtension());
		UE_LOG(LogTemp, Display, TEXT("CLP Authoring: created vessel Blueprint %s"), *PackageName);

		UClass* GeneratedClass = Blueprint->GeneratedClass;
		return (GeneratedClass && GeneratedClass->IsChildOf(ACLPVessel::StaticClass())) ? GeneratedClass : nullptr;
	}

	static TSubclassOf<ACLPVessel> GetVesselBlueprintClass(ECLPVesselKind Kind)
	{
		switch (Kind)
		{
		case ECLPVesselKind::Candle:       return CreateVesselBlueprint(TEXT("BP_Vessel_Candle"),       ACLPVessel_Candle::StaticClass());
		case ECLPVesselKind::Match:        return CreateVesselBlueprint(TEXT("BP_Vessel_Match"),        ACLPVessel_Match::StaticClass());
		case ECLPVesselKind::Lantern:      return CreateVesselBlueprint(TEXT("BP_Vessel_Lantern"),      ACLPVessel_Lantern::StaticClass());
		case ECLPVesselKind::Torch:        return CreateVesselBlueprint(TEXT("BP_Vessel_Torch"),        ACLPVessel_Torch::StaticClass());
		case ECLPVesselKind::Wisp:         return CreateVesselBlueprint(TEXT("BP_Vessel_Wisp"),         ACLPVessel_Wisp::StaticClass());
		case ECLPVesselKind::WaxPuddle:    return CreateVesselBlueprint(TEXT("BP_Vessel_WaxPuddle"),    ACLPVessel_WaxPuddle::StaticClass());
		case ECLPVesselKind::Beacon:       return CreateVesselBlueprint(TEXT("BP_Vessel_Beacon"),       ACLPVessel_Beacon::StaticClass());
		case ECLPVesselKind::EmberStone:   return CreateVesselBlueprint(TEXT("BP_Vessel_EmberStone"),   ACLPVessel_EmberStone::StaticClass());
		case ECLPVesselKind::MirrorShard:  return CreateVesselBlueprint(TEXT("BP_Vessel_MirrorShard"),  ACLPVessel_MirrorShard::StaticClass());
		case ECLPVesselKind::IncenseStick: return CreateVesselBlueprint(TEXT("BP_Vessel_IncenseStick"), ACLPVessel_IncenseStick::StaticClass());
		}
		return nullptr;
	}

	static void PatchMapSpawnPoints(const TCHAR* PackageName)
	{
		UPackage* Package = LoadPackage(nullptr, PackageName, LOAD_None);
		UWorld* World = Package ? UWorld::FindWorldInPackage(Package) : nullptr;
		if (!World)
		{
			UE_LOG(LogTemp, Error, TEXT("CLP Authoring: could not load map for blueprint patch: %s"), PackageName);
			return;
		}

		TArray<AActor*> SpawnActors;
		UGameplayStatics::GetAllActorsOfClass(World, ACLPVesselSpawnPoint::StaticClass(), SpawnActors);

		int32 Patched = 0;
		for (AActor* Actor : SpawnActors)
		{
			if (ACLPVesselSpawnPoint* Point = Cast<ACLPVesselSpawnPoint>(Actor))
			{
				Point->VesselClassOverride = GetVesselBlueprintClass(Point->VesselKind);
				++Patched;
			}
		}

		Package->MarkPackageDirty();
		SaveAuthoredPackage(Package, World, PackageName, FPackageName::GetMapPackageExtension());
		UE_LOG(LogTemp, Display, TEXT("CLP Authoring: patched %d spawn point(s) in %s"), Patched, PackageName);
		World->DestroyWorld(false, nullptr);
	}

	static void PatchMapLevelExit(const TCHAR* PackageName, FName NextLevelName)
	{
		// Load through the editor so placed actors are fully available. Plain
		// LoadPackage + FindWorldInPackage does not populate actors in a commandlet.
		if (!FEditorFileUtils::LoadMap(PackageName, false, false))
		{
			UE_LOG(LogTemp, Error, TEXT("CLP Authoring: could not load map for exit patch: %s"), PackageName);
			return;
		}

		UWorld* World = GEditor ? GEditor->GetEditorWorldContext().World() : nullptr;
		if (!World)
		{
			UE_LOG(LogTemp, Error, TEXT("CLP Authoring: no editor world for exit patch: %s"), PackageName);
			return;
		}

		TArray<AActor*> ExitActors;
		UGameplayStatics::GetAllActorsOfClass(World, ACLPLevelExitTrigger::StaticClass(), ExitActors);
		if (ExitActors.Num() > 0)
		{
			UE_LOG(LogTemp, Display, TEXT("CLP Authoring: kept existing level exit in %s"), PackageName);
			return;
		}

		TArray<AActor*> GoalActors;
		UGameplayStatics::GetAllActorsOfClass(World, ACLGoalCorner::StaticClass(), GoalActors);
		if (GoalActors.Num() == 0)
		{
			UE_LOG(LogTemp, Error, TEXT("CLP Authoring: no goal actor found for exit patch: %s"), PackageName);
			return;
		}

		const FVector ExitLocation = GoalActors[0]->GetActorLocation() + FVector(250.f, 0.f, -20.f);
		ACLPLevelExitTrigger* Exit = World->SpawnActor<ACLPLevelExitTrigger>(
			ACLPLevelExitTrigger::StaticClass(), ExitLocation, FRotator::ZeroRotator);
		if (Exit)
		{
			Exit->NextLevelName = NextLevelName;
			Exit->SetActorLabel(TEXT("CL_Exit_End"));
			World->GetOutermost()->MarkPackageDirty();
			const FString Filename = FPackageName::LongPackageNameToFilename(
				PackageName, FPackageName::GetMapPackageExtension());
			if (FEditorFileUtils::SaveMap(World, Filename))
			{
				UE_LOG(LogTemp, Display, TEXT("CLP Authoring: added level exit to %s -> %s"),
					PackageName, *NextLevelName.ToString());
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("CLP Authoring: failed to save exit patch: %s"), PackageName);
			}
		}
	}
#else
	static TSubclassOf<ACLPVessel> GetVesselBlueprintClass(ECLPVesselKind)
	{
		return nullptr;
	}
#endif

	static AActor* FindActorByLabel(UWorld* World, const TCHAR* Label)
	{
		if (!World)
		{
			return nullptr;
		}
		for (TActorIterator<AActor> It(World); It; ++It)
		{
			if ((*It)->GetActorLabel() == Label)
			{
				return *It;
			}
		}
		return nullptr;
	}

	static void ConfigureDirectionalLight(ADirectionalLight* Light)
	{
		if (!Light)
		{
			return;
		}

		// Key light comes from the camera side (+Y) and above, so the side-view
		// faces are lit instead of silhouetted against the back wall.
		Light->SetActorRotation(FRotator(-42.f, -90.f, 0.f));
		if (UDirectionalLightComponent* Component = Light->FindComponentByClass<UDirectionalLightComponent>())
		{
			Component->SetMobility(EComponentMobility::Movable);
			Component->SetIntensity(3.2f);
			Component->SetLightColor(FLinearColor(1.f, 0.88f, 0.74f));
			Component->SetCastShadows(true);
		}
	}

	static void AddSideViewLighting(UWorld* World, float FloorLength, float FloorCenterX)
	{
		if (!World)
		{
			return;
		}

		if (!FindActorByLabel(World, TEXT("CL_SkyLight")))
		{
			ASkyLight* Sky = World->SpawnActor<ASkyLight>(
				ASkyLight::StaticClass(), FVector(FloorCenterX, 0.f, 700.f), FRotator::ZeroRotator);
			if (Sky)
			{
				if (USkyLightComponent* Component = Sky->GetLightComponent())
				{
					Component->SetMobility(EComponentMobility::Movable);
					Component->SetIntensity(1.15f);
					Component->bRealTimeCapture = true;
				}
				Sky->SetActorLabel(TEXT("CL_SkyLight"));
			}
		}

		if (!FindActorByLabel(World, TEXT("CL_Exposure")))
		{
			APostProcessVolume* Exposure = World->SpawnActor<APostProcessVolume>(
				APostProcessVolume::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			if (Exposure)
			{
				Exposure->bUnbound = true;
				Exposure->Settings.bOverride_AutoExposureMinBrightness = true;
				Exposure->Settings.bOverride_AutoExposureMaxBrightness = true;
				Exposure->Settings.AutoExposureMinBrightness = 1.f;
				Exposure->Settings.AutoExposureMaxBrightness = 1.f;
				Exposure->SetActorLabel(TEXT("CL_Exposure"));
			}
		}

		const float StartX = FloorCenterX - FloorLength * 0.32f;
		const float StepX = FloorLength * 0.32f;
		for (int32 Index = 1; Index <= 3; ++Index)
		{
			const FString Label = FString::Printf(TEXT("CL_FillLight_%d"), Index);
			if (FindActorByLabel(World, *Label))
			{
				continue;
			}

			APointLight* Fill = World->SpawnActor<APointLight>(
				APointLight::StaticClass(),
				FVector(StartX + StepX * (Index - 1), 850.f, 560.f),
				FRotator::ZeroRotator);
			if (Fill)
			{
				if (UPointLightComponent* Component = Fill->FindComponentByClass<UPointLightComponent>())
				{
					Component->SetMobility(EComponentMobility::Movable);
					Component->SetIntensity(14000.f);
					Component->SetAttenuationRadius(2600.f);
					Component->SetLightColor(FLinearColor(1.f, 0.78f, 0.55f));
					Component->SetCastShadows(false);
				}
				Fill->SetActorLabel(Label);
			}
		}
	}

	static void PatchMapLightingAndCamera(const TCHAR* PackageName)
	{
		if (!FEditorFileUtils::LoadMap(PackageName, false, false))
		{
			UE_LOG(LogTemp, Error, TEXT("CLP Authoring: could not load map for lighting patch: %s"), PackageName);
			return;
		}

		UWorld* World = GEditor ? GEditor->GetEditorWorldContext().World() : nullptr;
		if (!World)
		{
			UE_LOG(LogTemp, Error, TEXT("CLP Authoring: no editor world for lighting patch: %s"), PackageName);
			return;
		}

		TArray<AActor*> LightActors;
		UGameplayStatics::GetAllActorsOfClass(World, ADirectionalLight::StaticClass(), LightActors);
		for (AActor* Actor : LightActors)
		{
			ConfigureDirectionalLight(Cast<ADirectionalLight>(Actor));
		}

		float FloorLength = 4000.f;
		float FloorCenterX = 1300.f;
		if (AActor* Floor = FindActorByLabel(World, TEXT("CL_Floor")))
		{
			FloorLength = Floor->GetActorScale3D().X * 100.f;
			FloorCenterX = Floor->GetActorLocation().X;
		}

		if (AActor* BackWall = FindActorByLabel(World, TEXT("CL_BackWall")))
		{
			FVector Location = BackWall->GetActorLocation();
			Location.Y = 2200.f;
			BackWall->SetActorLocation(Location);
		}

		AddSideViewLighting(World, FloorLength, FloorCenterX);

		World->GetOutermost()->MarkPackageDirty();
		const FString Filename = FPackageName::LongPackageNameToFilename(
			PackageName, FPackageName::GetMapPackageExtension());
		if (FEditorFileUtils::SaveMap(World, Filename))
		{
			UE_LOG(LogTemp, Display, TEXT("CLP Authoring: patched lighting and camera wall in %s"), PackageName);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("CLP Authoring: failed to save lighting patch: %s"), PackageName);
		}
	}

	static void AddNextLevelPreview(
		UWorld* World, float FloorLength, float FloorCenterX, const TCHAR* Heading, ECLPVesselKind NextKind);

	static void PatchMapNextLevelPreview(
		const TCHAR* PackageName, const TCHAR* Heading, ECLPVesselKind NextKind)
	{
		if (!FEditorFileUtils::LoadMap(PackageName, false, false))
		{
			UE_LOG(LogTemp, Error, TEXT("CLP Authoring: could not load map for preview patch: %s"), PackageName);
			return;
		}

		UWorld* World = GEditor ? GEditor->GetEditorWorldContext().World() : nullptr;
		if (!World)
		{
			UE_LOG(LogTemp, Error, TEXT("CLP Authoring: no editor world for preview patch: %s"), PackageName);
			return;
		}

		float FloorLength = 4000.f;
		float FloorCenterX = 1300.f;
		if (AActor* Floor = FindActorByLabel(World, TEXT("CL_Floor")))
		{
			FloorLength = Floor->GetActorScale3D().X * 100.f;
			FloorCenterX = Floor->GetActorLocation().X;
		}

		if (AActor* EndWall = FindActorByLabel(World, TEXT("CL_EndWall")))
		{
			FVector Location = EndWall->GetActorLocation();
			Location.X = FloorCenterX + FloorLength * 0.5f + 2300.f;
			EndWall->SetActorLocation(Location);
		}

		if (!FindActorByLabel(World, TEXT("CL_NextPreview_Floor")))
		{
			AddNextLevelPreview(World, FloorLength, FloorCenterX, Heading, NextKind);
		}

		World->GetOutermost()->MarkPackageDirty();
		const FString Filename = FPackageName::LongPackageNameToFilename(
			PackageName, FPackageName::GetMapPackageExtension());
		if (FEditorFileUtils::SaveMap(World, Filename))
		{
			UE_LOG(LogTemp, Display, TEXT("CLP Authoring: patched next-level preview in %s"), PackageName);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("CLP Authoring: failed to save preview patch: %s"), PackageName);
		}
	}

	static UWorld* CreateRoomWorld(const TCHAR* PackageName, const TCHAR* WorldName)
	{
		UPackage* Package = CreatePackage(PackageName);
		Package->MarkPackageDirty();

		UWorld* World = UWorld::CreateWorld(
			EWorldType::Editor, false, FName(WorldName), Package);
		if (!World)
		{
			UE_LOG(LogTemp, Error, TEXT("CLP Authoring: failed to create world %s"), WorldName);
			return nullptr;
		}

		World->SetFlags(RF_Public | RF_Standalone);
		if (AWorldSettings* Settings = World->GetWorldSettings())
		{
			Settings->DefaultGameMode = ACLPRoomGameMode::StaticClass();
		}

		ADirectionalLight* Light = World->SpawnActor<ADirectionalLight>(
			ADirectionalLight::StaticClass(), FVector::ZeroVector, FRotator(-42.f, -90.f, 0.f));
		if (Light)
		{
			ConfigureDirectionalLight(Light);
			Light->SetActorLabel(TEXT("CL_KeyLight"));
		}

		return World;
	}

	static AStaticMeshActor* SpawnBlock(UWorld* World, const FVector& Location, const FVector& Scale, const TCHAR* MaterialPath, const TCHAR* Label)
	{
		AStaticMeshActor* Block = World->SpawnActor<AStaticMeshActor>(
			AStaticMeshActor::StaticClass(), Location, FRotator::ZeroRotator);
		if (!Block)
		{
			return nullptr;
		}

		static UStaticMesh* Cube = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
		Block->GetStaticMeshComponent()->SetStaticMesh(Cube);
		Block->SetActorScale3D(Scale);

		if (UMaterialInterface* Material = LoadObject<UMaterialInterface>(nullptr, MaterialPath))
		{
			Block->GetStaticMeshComponent()->SetMaterial(0, Material);
		}

#if WITH_EDITOR
		Block->SetActorLabel(Label);
#endif
		return Block;
	}

	static void AddSpawnPoint(UWorld* World, ECLPVesselKind Kind, const FVector& Location, const TCHAR* Label, TSubclassOf<ACLPVessel> ClassOverride = nullptr)
	{
		ACLPVesselSpawnPoint* Point = World->SpawnActor<ACLPVesselSpawnPoint>(
			ACLPVesselSpawnPoint::StaticClass(), Location, FRotator::ZeroRotator);
		if (!Point)
		{
			return;
		}

		Point->VesselKind = Kind;
		Point->VesselClassOverride = ClassOverride;
#if WITH_EDITOR
		Point->SetActorLabel(Label);
#endif
	}

	static void AddGoal(UWorld* World, const FVector& Location, const TCHAR* Label, ECLPVesselKind RequiredKind)
	{
		ACLGoalCorner* Goal = World->SpawnActor<ACLGoalCorner>(
			ACLGoalCorner::StaticClass(), Location, FRotator::ZeroRotator);
		if (!Goal)
		{
			return;
		}

		Goal->GoalName = Label;
		Goal->RequiredVesselKind = RequiredKind;
#if WITH_EDITOR
		Goal->SetActorLabel(Label);
#endif
	}

	static void AddLevelExit(UWorld* World, const FVector& Location, const TCHAR* Label, FName NextLevelName)
	{
		ACLPLevelExitTrigger* Exit = World->SpawnActor<ACLPLevelExitTrigger>(
			ACLPLevelExitTrigger::StaticClass(), Location, FRotator::ZeroRotator);
		if (!Exit)
		{
			return;
		}

		Exit->NextLevelName = NextLevelName;
#if WITH_EDITOR
		Exit->SetActorLabel(Label);
#endif
	}

	static void AddNextLevelPreview(
		UWorld* World, float FloorLength, float FloorCenterX, const TCHAR* Heading, ECLPVesselKind NextKind)
	{
		const float EndX = FloorCenterX + FloorLength * 0.5f;
		const float StartX = EndX + 250.f;
		const float PreviewLength = 1800.f;
		const float CenterX = StartX + PreviewLength * 0.5f;

		SpawnBlock(World, FVector(CenterX, 0.f, -50.f), FVector(PreviewLength / 100.f, 6.f, 1.f),
			TEXT("/Game/Candlelight/Materials/MI_CLP_Floor.MI_CLP_Floor"), TEXT("CL_NextPreview_Floor"));

		if (NextKind == ECLPVesselKind::Match)
		{
			SpawnBlock(World, FVector(CenterX, 0.f, 170.f), FVector(14.f, 4.f, 2.4f),
				TEXT("/Game/Candlelight/Materials/MI_CLP_Wall.MI_CLP_Wall"), TEXT("CL_NextPreview_Tunnel"));
		}
		else
		{
			SpawnBlock(World, FVector(StartX + 400.f, 0.f, 100.f), FVector(4.f, 4.f, 0.4f),
				TEXT("/Game/Candlelight/Materials/MI_CLP_Platform.MI_CLP_Platform"), TEXT("CL_NextPreview_Platform_1"));
			SpawnBlock(World, FVector(StartX + 900.f, 0.f, 170.f), FVector(4.f, 4.f, 0.4f),
				TEXT("/Game/Candlelight/Materials/MI_CLP_Platform.MI_CLP_Platform"), TEXT("CL_NextPreview_Platform_2"));
			SpawnBlock(World, FVector(StartX + 1400.f, 0.f, 100.f), FVector(4.f, 4.f, 0.4f),
				TEXT("/Game/Candlelight/Materials/MI_CLP_Platform.MI_CLP_Platform"), TEXT("CL_NextPreview_Platform_3"));
		}

		ATextRenderActor* TextActor = World->SpawnActor<ATextRenderActor>(
			ATextRenderActor::StaticClass(), FVector(EndX + 760.f, 0.f, 460.f), FRotator(0.f, -90.f, 0.f));
		if (TextActor)
		{
			if (UTextRenderComponent* Text = TextActor->FindComponentByClass<UTextRenderComponent>())
			{
				Text->SetText(FText::FromString(Heading));
				Text->SetWorldSize(44.f);
				Text->SetTextRenderColor(FColor(255, 216, 140));
				Text->SetHorizontalAlignment(EHTA_Center);
				Text->SetVerticalAlignment(EVRTA_TextBottom);
				Text->SetCastShadow(false);
			}
#if WITH_EDITOR
			TextActor->SetActorLabel(TEXT("CL_NextPreview_Label"));
#endif
		}
	}

	static void BuildSharedShell(UWorld* World, float FloorLength, float FloorCenterX)
	{
		SpawnBlock(World, FVector(FloorCenterX, 0.f, -50.f), FVector(FloorLength / 100.f, 6.f, 1.f),
			TEXT("/Game/Candlelight/Materials/MI_CLP_Floor.MI_CLP_Floor"), TEXT("CL_Floor"));
		// Back wall is behind the orthographic camera (Y=1500), so it cannot
		// block the side view.
		SpawnBlock(World, FVector(FloorCenterX, 2200.f, 300.f), FVector(FloorLength / 100.f, 1.f, 6.f),
			TEXT("/Game/Candlelight/Materials/MI_CLP_Wall.MI_CLP_Wall"), TEXT("CL_BackWall"));
		SpawnBlock(World, FVector(FloorCenterX - FloorLength / 2.f - 50.f, 0.f, 300.f), FVector(1.f, 6.f, 6.f),
			TEXT("/Game/Candlelight/Materials/MI_CLP_Wall.MI_CLP_Wall"), TEXT("CL_StartWall"));
		SpawnBlock(World, FVector(FloorCenterX + FloorLength / 2.f + 2300.f, 0.f, 300.f), FVector(1.f, 6.f, 6.f),
			TEXT("/Game/Candlelight/Materials/MI_CLP_Wall.MI_CLP_Wall"), TEXT("CL_EndWall"));

		AddSideViewLighting(World, FloorLength, FloorCenterX);
	}

	static void SaveWorld(UWorld* World, const TCHAR* PackageName);

	static void CreateLevel3Placeholder()
	{
		const TCHAR* MapName = TEXT("/Game/Candlelight/Maps/Levels/CL_Level_03_LanternSide");
		if (FPackageName::DoesPackageExist(MapName))
		{
			UE_LOG(LogTemp, Display, TEXT("CLP Authoring: kept existing %s"), MapName);
			return;
		}

		UWorld* World = CreateRoomWorld(MapName, TEXT("CL_Level_03_LanternSide"));
		if (!World)
		{
			return;
		}

		BuildSharedShell(World, 4200.f, 1400.f);
		AddNextLevelPreview(World, 4200.f, 1400.f, TEXT("MORE TO COME"), ECLPVesselKind::Candle);
		SpawnBlock(World, FVector(850.f, 0.f, 100.f), FVector(4.f, 4.f, 0.4f),
			TEXT("/Game/Candlelight/Materials/MI_CLP_Platform.MI_CLP_Platform"), TEXT("CL_Platform_01"));
		SpawnBlock(World, FVector(1600.f, 0.f, 170.f), FVector(4.f, 4.f, 0.4f),
			TEXT("/Game/Candlelight/Materials/MI_CLP_Platform.MI_CLP_Platform"), TEXT("CL_Platform_02"));
		SpawnBlock(World, FVector(2350.f, 0.f, 100.f), FVector(4.f, 4.f, 0.4f),
			TEXT("/Game/Candlelight/Materials/MI_CLP_Platform.MI_CLP_Platform"), TEXT("CL_Platform_03"));

		AddSpawnPoint(World, ECLPVesselKind::Lantern, FVector(0.f, 0.f, 60.f), TEXT("CL_Spawn_Lantern"),
			GetVesselBlueprintClass(ECLPVesselKind::Lantern));
		AddGoal(World, FVector(3200.f, 0.f, 100.f), TEXT("LEVEL 3 CORNER"), ECLPVesselKind::Lantern);
		AddLevelExit(World, FVector(3450.f, 0.f, 80.f), TEXT("CL_Exit_End"), NAME_None);

		SaveWorld(World, MapName);
	}

	static void SaveWorld(UWorld* World, const TCHAR* PackageName)
	{
		SaveAuthoredPackage(World->GetOutermost(), World, PackageName, FPackageName::GetMapPackageExtension());
		// Explicitly tear down subsystems so the commandlet shuts down cleanly.
		World->DestroyWorld(false, nullptr);
	}
}

int32 UCLPLevelAuthoringCommandlet::Main(const FString& Params)
{
	// -SkipMaps refreshes assets only. -PatchMaps keeps map geometry but points
	// every vessel spawn at its new Blueprint class.
	const bool bSkipMaps = Params.Contains(TEXT("SkipMaps"));
	const bool bPatchMaps = Params.Contains(TEXT("PatchMaps"));
	const bool bAddLevel3 = Params.Contains(TEXT("AddLevel3"));

	// 1. Tuning assets. Designers can edit these later without changing C++.
	CreateVesselConfig(
		TEXT("DA_Candle_Vessel"), ECLPVesselKind::Candle, TEXT("CANDLE"),
		300.f, 720.f, 120.f, 900.f, false, FColor(240, 220, 180),
		20.f, 44.f,
		FVector(0.38f, 0.38f, 1.15f), FVector(0.f, 0.f, -4.f),
		FVector(0.f, 0.f, 62.f), FVector(0.18f, 0.18f, 0.26f));

	CreateVesselConfig(
		TEXT("DA_Match_Vessel"), ECLPVesselKind::Match, TEXT("MATCH"),
		430.f, 470.f, 8.f, 260.f, true, FColor(190, 120, 60),
		5.f, 20.f,
		FVector(0.12f, 0.12f, 0.72f), FVector(0.f, 0.f, -4.f),
		FVector(0.f, 0.f, 42.f), FVector(0.11f, 0.11f, 0.16f));

	// 2. Blockout materials. These are the reusable level-kit colours.
	CreateMaterialInstance(TEXT("MI_CLP_Floor"),       FLinearColor(0.27f, 0.32f, 0.39f));
	CreateMaterialInstance(TEXT("MI_CLP_Wall"),        FLinearColor(0.12f, 0.15f, 0.21f));
	CreateMaterialInstance(TEXT("MI_CLP_Platform"),    FLinearColor(0.35f, 0.41f, 0.51f));
	CreateMaterialInstance(TEXT("MI_CLP_Plate"),       FLinearColor(0.82f, 0.68f, 0.32f));
	CreateMaterialInstance(TEXT("MI_CLP_Gate"),        FLinearColor(0.27f, 0.37f, 0.51f));
	CreateMaterialInstance(TEXT("MI_CLP_CandleWax"),   FLinearColor(0.94f, 0.86f, 0.71f));
	CreateMaterialInstance(TEXT("MI_CLP_Flame"),       FLinearColor(1.00f, 0.55f, 0.12f));
	CreateMaterialInstance(TEXT("MI_CLP_Collectible"), FLinearColor(1.00f, 0.92f, 0.45f));
	CreateMaterialInstance(TEXT("MI_CLP_Lock"),        FLinearColor(0.18f, 0.22f, 0.28f));
	CreateMaterialInstance(TEXT("MI_CLP_Goal"),        FLinearColor(0.95f, 0.72f, 0.20f));
	CreateMaterialInstance(TEXT("MI_CLP_Return"),      FLinearColor(0.35f, 0.75f, 1.00f));

#if WITH_EDITOR
	// 2b. One openable Blueprint per vessel. Each has its parent preset's mesh,
	// capsule and size already visible in the Components/Class Defaults panels.
	GetVesselBlueprintClass(ECLPVesselKind::Candle);
	GetVesselBlueprintClass(ECLPVesselKind::Match);
	GetVesselBlueprintClass(ECLPVesselKind::Lantern);
	GetVesselBlueprintClass(ECLPVesselKind::Torch);
	GetVesselBlueprintClass(ECLPVesselKind::Wisp);
	GetVesselBlueprintClass(ECLPVesselKind::WaxPuddle);
	GetVesselBlueprintClass(ECLPVesselKind::Beacon);
	GetVesselBlueprintClass(ECLPVesselKind::EmberStone);
	GetVesselBlueprintClass(ECLPVesselKind::MirrorShard);
	GetVesselBlueprintClass(ECLPVesselKind::IncenseStick);
#endif

	if (bAddLevel3)
	{
		CreateLevel3Placeholder();
	}

	if (bPatchMaps)
	{
#if WITH_EDITOR
		PatchMapSpawnPoints(TEXT("/Game/Candlelight/Maps/Levels/CL_Level_01_CandleSide"));
		PatchMapSpawnPoints(TEXT("/Game/Candlelight/Maps/Levels/CL_Level_02_MatchSide"));
		PatchMapLevelExit(TEXT("/Game/Candlelight/Maps/Levels/CL_Level_01_CandleSide"), TEXT("CL_Level_02_MatchSide"));
		PatchMapLevelExit(TEXT("/Game/Candlelight/Maps/Levels/CL_Level_02_MatchSide"), TEXT("CL_Level_03_LanternSide"));
		PatchMapLightingAndCamera(TEXT("/Game/Candlelight/Maps/Levels/CL_Level_01_CandleSide"));
		PatchMapLightingAndCamera(TEXT("/Game/Candlelight/Maps/Levels/CL_Level_02_MatchSide"));
		PatchMapLightingAndCamera(TEXT("/Game/Candlelight/Maps/Levels/CL_Level_03_LanternSide"));
		PatchMapNextLevelPreview(TEXT("/Game/Candlelight/Maps/Levels/CL_Level_01_CandleSide"),
			TEXT("NEXT: MATCH SIDE"), ECLPVesselKind::Match);
		PatchMapNextLevelPreview(TEXT("/Game/Candlelight/Maps/Levels/CL_Level_02_MatchSide"),
			TEXT("NEXT: LANTERN SIDE"), ECLPVesselKind::Lantern);
		PatchMapNextLevelPreview(TEXT("/Game/Candlelight/Maps/Levels/CL_Level_03_LanternSide"),
			TEXT("MORE TO COME"), ECLPVesselKind::Candle);
#endif
	}
	else if (!bSkipMaps)
	{
	// 3. Level 1 - Candle side. Basic platformer, no puzzle systems yet.
	{
		const TCHAR* MapName = TEXT("/Game/Candlelight/Maps/Levels/CL_Level_01_CandleSide");
		UWorld* World = CreateRoomWorld(MapName, TEXT("CL_Level_01_CandleSide"));
		if (World)
		{
			BuildSharedShell(World, 4000.f, 1300.f);
			AddNextLevelPreview(World, 4000.f, 1300.f, TEXT("NEXT: MATCH SIDE"), ECLPVesselKind::Match);

			SpawnBlock(World, FVector(550.f, 0.f, 90.f), FVector(5.f, 4.f, 0.4f),
				TEXT("/Game/Candlelight/Materials/MI_CLP_Platform.MI_CLP_Platform"), TEXT("CL_Platform_01"));
			SpawnBlock(World, FVector(1150.f, 0.f, 170.f), FVector(5.f, 4.f, 0.4f),
				TEXT("/Game/Candlelight/Materials/MI_CLP_Platform.MI_CLP_Platform"), TEXT("CL_Platform_02"));
			SpawnBlock(World, FVector(1750.f, 0.f, 100.f), FVector(5.f, 4.f, 0.4f),
				TEXT("/Game/Candlelight/Materials/MI_CLP_Platform.MI_CLP_Platform"), TEXT("CL_Platform_03"));
			SpawnBlock(World, FVector(2350.f, 0.f, 180.f), FVector(5.f, 4.f, 0.4f),
				TEXT("/Game/Candlelight/Materials/MI_CLP_Platform.MI_CLP_Platform"), TEXT("CL_Platform_04"));

			AddSpawnPoint(World, ECLPVesselKind::Candle, FVector(0.f, 0.f, 60.f), TEXT("CL_Spawn_Candle"),
				GetVesselBlueprintClass(ECLPVesselKind::Candle));
			AddGoal(World, FVector(2950.f, 0.f, 100.f), TEXT("LEVEL 1 CORNER"), ECLPVesselKind::Candle);
			AddLevelExit(World, FVector(3200.f, 0.f, 80.f), TEXT("CL_Exit_To_Level02"),
				TEXT("CL_Level_02_MatchSide"));

			SaveWorld(World, MapName);
		}
	}

	// 4. Level 2 - Match side. Candle walks to the low tunnel, swaps to Match,
	//    crawls through, then a return volume swaps the spark back to Candle.
	{
		const TCHAR* MapName = TEXT("/Game/Candlelight/Maps/Levels/CL_Level_02_MatchSide");
		UWorld* World = CreateRoomWorld(MapName, TEXT("CL_Level_02_MatchSide"));
		if (World)
		{
			BuildSharedShell(World, 4400.f, 1300.f);
			AddNextLevelPreview(World, 4400.f, 1300.f, TEXT("NEXT: LANTERN SIDE"), ECLPVesselKind::Lantern);

			// Low ceiling: underside at Z=50. Candle needs ~88, Match needs ~40.
			SpawnBlock(World, FVector(1550.f, 0.f, 210.f), FVector(7.f, 4.f, 3.2f),
				TEXT("/Game/Candlelight/Materials/MI_CLP_Wall.MI_CLP_Wall"), TEXT("CL_CrawlTunnel_Ceiling"));
			SpawnBlock(World, FVector(1550.f, -170.f, 60.f), FVector(7.f, 0.4f, 1.2f),
				TEXT("/Game/Candlelight/Materials/MI_CLP_Wall.MI_CLP_Wall"), TEXT("CL_CrawlTunnel_RailA"));
			SpawnBlock(World, FVector(1550.f, 170.f, 60.f), FVector(7.f, 0.4f, 1.2f),
				TEXT("/Game/Candlelight/Materials/MI_CLP_Wall.MI_CLP_Wall"), TEXT("CL_CrawlTunnel_RailB"));

			// Short exit platform sequence for Candle after the swap-back.
			SpawnBlock(World, FVector(2350.f, 0.f, 100.f), FVector(4.f, 4.f, 0.4f),
				TEXT("/Game/Candlelight/Materials/MI_CLP_Platform.MI_CLP_Platform"), TEXT("CL_Platform_01"));
			SpawnBlock(World, FVector(2700.f, 0.f, 170.f), FVector(4.f, 4.f, 0.4f),
				TEXT("/Game/Candlelight/Materials/MI_CLP_Platform.MI_CLP_Platform"), TEXT("CL_Platform_02"));

			AddSpawnPoint(World, ECLPVesselKind::Candle, FVector(0.f, 0.f, 60.f), TEXT("CL_Spawn_Candle"),
				GetVesselBlueprintClass(ECLPVesselKind::Candle));
			AddSpawnPoint(World, ECLPVesselKind::Match, FVector(1000.f, -250.f, 40.f), TEXT("CL_Spawn_Match"),
				GetVesselBlueprintClass(ECLPVesselKind::Match));

			ACLSparkReturn* ReturnVolume = World->SpawnActor<ACLSparkReturn>(
				ACLSparkReturn::StaticClass(), FVector(2050.f, 0.f, 60.f), FRotator::ZeroRotator);
			if (ReturnVolume)
			{
				ReturnVolume->FromVesselKind = ECLPVesselKind::Match;
				ReturnVolume->ReturnVesselKind = ECLPVesselKind::Candle;
				ReturnVolume->ReturnLabel = TEXT("SWAP BACK TO CANDLE");
#if WITH_EDITOR
				ReturnVolume->SetActorLabel(TEXT("CL_ReturnToCandle"));
#endif
			}

			AddGoal(World, FVector(3050.f, 0.f, 100.f), TEXT("LEVEL 2 CORNER"), ECLPVesselKind::Candle);
			AddLevelExit(World, FVector(3300.f, 0.f, 80.f), TEXT("CL_Exit_To_Level03"),
				TEXT("CL_Level_03_LanternSide"));

			SaveWorld(World, MapName);
		}
	}
	CreateLevel3Placeholder();
	}

	UE_LOG(LogTemp, Display, TEXT("CLP Authoring complete: 2 vessel configs, 10 materials, 3 levels"));
	return 0;
}

#else

int32 UCLPLevelAuthoringCommandlet::Main(const FString& Params)
{
	(void)Params;
	return 0;
}

#endif // WITH_EDITOR
