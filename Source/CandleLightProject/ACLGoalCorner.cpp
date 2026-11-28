#include "ACLGoalCorner.h"

#include "ACLPLockedGate.h"
#include "CLPVessel.h"
#include "Components/BoxComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Materials/Material.h"
#include "Materials/MaterialInterface.h"
#include "UObject/ConstructorHelpers.h"

ACLGoalCorner::ACLGoalCorner()
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
	MarkerMesh->SetRelativeScale3D(FVector(0.9f, 0.9f, 1.8f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeObj(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeObj.Succeeded())
	{
		MarkerMesh->SetStaticMesh(CubeObj.Object);
	}

	// Use a serializable material. Dynamic instances are created at runtime only,
	// never in a constructor that gets saved into a map.
	MarkerMesh->SetMaterial(0, UMaterial::GetDefaultMaterial(MD_Surface));

	Label = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Label"));
	Label->SetupAttachment(RootComponent);
	Label->SetRelativeLocation(FVector(0.f, 0.f, 110.f));
	Label->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	Label->SetWorldSize(20.f);
	Label->SetTextRenderColor(FColor(255, 220, 140));
	Label->SetHorizontalAlignment(EHTA_Center);

	Glow = CreateDefaultSubobject<UPointLightComponent>(TEXT("Glow"));
	Glow->SetupAttachment(RootComponent);
	Glow->SetRelativeLocation(FVector(0.f, 0.f, 40.f));
	Glow->SetLightColor(FLinearColor(1.f, 0.65f, 0.2f));
	Glow->SetAttenuationRadius(400.f);
	Glow->SetIntensity(2500.f);

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ACLGoalCorner::HandleBeginOverlap);
}

void ACLGoalCorner::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (Label)
	{
		Label->SetText(FText::Format(FText::FromString(TEXT("{0}\nCORNER")), FText::FromString(GoalName)));
	}

	if (MarkerMesh)
	{
		if (UMaterialInterface* GoalMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Candlelight/Materials/MI_CLP_Goal.MI_CLP_Goal")))
		{
			MarkerMesh->SetMaterial(0, GoalMaterial);
		}
	}
}

void ACLGoalCorner::HandleBeginOverlap(UPrimitiveComponent*, AActor* OtherActor, UPrimitiveComponent*, int32, bool, const FHitResult&)
{
	if (bReached && bTriggerOnce)
	{
		return;
	}

	ACLPVessel* Vessel = Cast<ACLPVessel>(OtherActor);
	if (!Vessel || !Vessel->IsSparkActive())
	{
		return;
	}

	if (Vessel->GetVesselKind() != RequiredVesselKind)
	{
		UE_LOG(LogTemp, Display, TEXT("CLP GOAL rejected %s - wrong vessel for this corner"), *Vessel->GetVesselLabel());
		return;
	}

	bReached = true;
	if (LinkedGate)
	{
		LinkedGate->SetOpen(true);
	}
	OnGoalReached.Broadcast();

	UE_LOG(LogTemp, Display, TEXT("CLP GOAL reached: %s"), *GoalName);
}
