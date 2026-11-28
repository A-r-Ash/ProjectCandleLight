#include "ACLSparkReturn.h"

#include "ACLPRoomGameMode.h"
#include "CLPVessel.h"
#include "Components/BoxComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/Material.h"
#include "Materials/MaterialInterface.h"
#include "UObject/ConstructorHelpers.h"

namespace
{
	static const TCHAR* VesselShortName(ECLPVesselKind Kind)
	{
		switch (Kind)
		{
		case ECLPVesselKind::Candle:       return TEXT("CANDLE");
		case ECLPVesselKind::Match:        return TEXT("MATCH");
		case ECLPVesselKind::Lantern:      return TEXT("LANTERN");
		case ECLPVesselKind::Torch:        return TEXT("TORCH");
		case ECLPVesselKind::Wisp:         return TEXT("WISP");
		case ECLPVesselKind::WaxPuddle:    return TEXT("WAX PUDDLE");
		case ECLPVesselKind::Beacon:       return TEXT("BEACON");
		case ECLPVesselKind::EmberStone:   return TEXT("EMBER STONE");
		case ECLPVesselKind::MirrorShard:  return TEXT("MIRROR SHARD");
		case ECLPVesselKind::IncenseStick: return TEXT("INCENSE STICK");
		default:                           return TEXT("VESSEL");
		}
	}
}

ACLSparkReturn::ACLSparkReturn()
{
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;
	TriggerBox->SetBoxExtent(FVector(70.f, 70.f, 90.f));
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerBox->SetCollisionResponseToAllChannels(ECR_Overlap);
	TriggerBox->SetGenerateOverlapEvents(true);

	MarkerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MarkerMesh"));
	MarkerMesh->SetupAttachment(RootComponent);
	MarkerMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MarkerMesh->SetRelativeScale3D(FVector(0.8f, 0.8f, 1.6f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderObj(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	if (CylinderObj.Succeeded())
	{
		MarkerMesh->SetStaticMesh(CylinderObj.Object);
	}

	// Static default here; OnConstruction loads the authored MI_CLP_Return material.
	MarkerMesh->SetMaterial(0, UMaterial::GetDefaultMaterial(MD_Surface));

	Label = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Label"));
	Label->SetupAttachment(RootComponent);
	Label->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
	Label->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	Label->SetWorldSize(18.f);
	Label->SetTextRenderColor(FColor(150, 220, 255));
	Label->SetHorizontalAlignment(EHTA_Center);

	Glow = CreateDefaultSubobject<UPointLightComponent>(TEXT("Glow"));
	Glow->SetupAttachment(RootComponent);
	Glow->SetRelativeLocation(FVector(0.f, 0.f, 30.f));
	Glow->SetLightColor(FLinearColor(0.3f, 0.65f, 1.f));
	Glow->SetAttenuationRadius(340.f);
	Glow->SetIntensity(1800.f);

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ACLSparkReturn::HandleBeginOverlap);
}

void ACLSparkReturn::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (Label)
	{
		Label->SetText(FText::FromString(ReturnLabel));
	}

	if (MarkerMesh)
	{
		if (UMaterialInterface* ReturnMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Candlelight/Materials/MI_CLP_Return.MI_CLP_Return")))
		{
			MarkerMesh->SetMaterial(0, ReturnMaterial);
		}
	}
}

void ACLSparkReturn::HandleBeginOverlap(UPrimitiveComponent*, AActor* OtherActor, UPrimitiveComponent*, int32, bool, const FHitResult&)
{
	if (bTriggered && bTriggerOnce)
	{
		return;
	}

	ACLPVessel* Vessel = Cast<ACLPVessel>(OtherActor);
	if (!Vessel || !Vessel->IsSparkActive() || Vessel->GetVesselKind() != FromVesselKind)
	{
		return;
	}

	ACLPRoomGameMode* RoomGameMode = Cast<ACLPRoomGameMode>(UGameplayStatics::GetGameMode(this));
	if (!RoomGameMode)
	{
		return;
	}

	bTriggered = true;
	UE_LOG(LogTemp, Display, TEXT("CLP Spark return: %s -> %s"),
		VesselShortName(FromVesselKind), VesselShortName(ReturnVesselKind));
	RoomGameMode->SetActiveVesselKind(ReturnVesselKind);
}
