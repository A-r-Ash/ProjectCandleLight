#include "ACLPPressurePlate.h"

#include "ACLPLockedGate.h"
#include "CLPVessel.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "Materials/Material.h"
#include "Materials/MaterialInterface.h"
#include "UObject/ConstructorHelpers.h"

ACLPPressurePlate::ACLPPressurePlate()
{
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;
	TriggerBox->SetBoxExtent(FVector(55.f, 55.f, 18.f));
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerBox->SetCollisionResponseToAllChannels(ECR_Overlap);
	TriggerBox->SetGenerateOverlapEvents(true);

	PlateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlateMesh"));
	PlateMesh->SetupAttachment(RootComponent);
	PlateMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PlateMesh->SetRelativeScale3D(FVector(1.1f, 1.1f, 0.12f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeObj(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeObj.Succeeded())
	{
		PlateMesh->SetStaticMesh(CubeObj.Object);
	}

	PlateMesh->SetMaterial(0, UMaterial::GetDefaultMaterial(MD_Surface));

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ACLPPressurePlate::HandleBeginOverlap);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ACLPPressurePlate::HandleEndOverlap);
}

void ACLPPressurePlate::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (PlateMesh)
	{
		if (UMaterialInterface* PlateMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Candlelight/Materials/MI_CLP_Plate.MI_CLP_Plate")))
		{
			PlateMesh->SetMaterial(0, PlateMaterial);
		}
	}
}

void ACLPPressurePlate::HandleBeginOverlap(UPrimitiveComponent*, AActor* OtherActor, UPrimitiveComponent*, int32, bool, const FHitResult&)
{
	if (OtherActor && OtherActor != this)
	{
		OverlappingActors.Add(OtherActor);
		RecomputePressedState();
	}
}

void ACLPPressurePlate::HandleEndOverlap(UPrimitiveComponent*, AActor* OtherActor, UPrimitiveComponent*, int32)
{
	if (OtherActor)
	{
		OverlappingActors.Remove(OtherActor);
		RecomputePressedState();
	}
}

void ACLPPressurePlate::RecomputePressedState()
{
	float TotalMass = 0.f;
	for (const TWeakObjectPtr<AActor>& WeakActor : OverlappingActors)
	{
		AActor* Actor = WeakActor.Get();
		if (!Actor)
		{
			continue;
		}

		if (ACLPVessel* Vessel = Cast<ACLPVessel>(Actor))
		{
			TotalMass += Vessel->GetVesselMass();
		}
		else if (Cast<ACharacter>(Actor))
		{
			TotalMass += 40.f;
		}
		else
		{
			TotalMass += 10.f;
		}
	}

	const bool bNewPressed = TotalMass >= RequiredMass;
	if (bNewPressed == bIsPressed)
	{
		return;
	}

	bIsPressed = bNewPressed;
	if (PlateMesh)
	{
		PlateMesh->SetRelativeLocation(FVector(0.f, 0.f, bIsPressed ? -8.f : 0.f));
	}

	if (LinkedGate)
	{
		LinkedGate->SetOpen(bIsPressed);
	}

	OnStateChanged.Broadcast(bIsPressed);
	UE_LOG(LogTemp, Display, TEXT("CLP Plate %s (mass %.0f/%.0f)"), bIsPressed ? TEXT("PRESSED") : TEXT("RELEASED"), TotalMass, RequiredMass);
}
