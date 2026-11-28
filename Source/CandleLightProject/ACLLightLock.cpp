#include "ACLLightLock.h"

#include "ACLPLockedGate.h"
#include "CLPVessel.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "UObject/ConstructorHelpers.h"

ACLLightLock::ACLLightLock()
{
	PrimaryActorTick.bCanEverTick = true;

	EyeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EyeMesh"));
	RootComponent = EyeMesh;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereObj(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (SphereObj.Succeeded())
	{
		EyeMesh->SetStaticMesh(SphereObj.Object);
	}
	EyeMesh->SetRelativeScale3D(FVector(0.45f));
	EyeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	EyeMesh->SetMaterial(0, UMaterial::GetDefaultMaterial(MD_Surface));
}

void ACLLightLock::BeginPlay()
{
	Super::BeginPlay();

	// Runtime-only dynamic instance: changing colour is safe and never saved into a map.
	UMaterialInterface* Parent = nullptr;
	if (UMaterialInterface* Authored = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Candlelight/Materials/MI_CLP_Lock.MI_CLP_Lock")))
	{
		Parent = Authored;
	}
	else
	{
		Parent = UMaterial::GetDefaultMaterial(MD_Surface);
	}

	EyeMaterial = UMaterialInstanceDynamic::Create(Parent, this);
	if (EyeMesh)
	{
		EyeMesh->SetMaterial(0, EyeMaterial);
	}

	EvaluateLight();
}

void ACLLightLock::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CheckTimer -= DeltaSeconds;
	if (CheckTimer <= 0.f)
	{
		CheckTimer = CheckInterval;
		EvaluateLight();
	}
}

void ACLLightLock::EvaluateLight()
{
	bool bNewLit = false;

	TArray<AActor*> Vessels;
	UGameplayStatics::GetAllActorsOfClass(this, ACLPVessel::StaticClass(), Vessels);
	for (AActor* Actor : Vessels)
	{
		ACLPVessel* Vessel = Cast<ACLPVessel>(Actor);
		if (!Vessel || !Vessel->IsSparkActive() || Vessel->GetVesselKind() != ECLPVesselKind::Candle)
		{
			continue;
		}

		const float Radius = Vessel->GetLightRadius();
		if (Radius < MinLightRadius)
		{
			continue;
		}

		if (FVector::Dist(Vessel->GetActorLocation(), GetActorLocation()) <= Radius)
		{
			bNewLit = true;
			break;
		}
	}

	if (bNewLit == bIsLit)
	{
		return;
	}

	bIsLit = bNewLit;
	if (EyeMaterial)
	{
		EyeMaterial->SetVectorParameterValue(TEXT("BaseColor"), bIsLit ? FLinearColor(1.f, 0.72f, 0.25f) : FLinearColor(0.18f, 0.22f, 0.28f));
	}
	if (LinkedGate)
	{
		LinkedGate->SetOpen(bIsLit);
	}
	OnStateChanged.Broadcast(bIsLit);
	UE_LOG(LogTemp, Display, TEXT("CLP Light Lock %s"), bIsLit ? TEXT("LIT") : TEXT("DARK"));
}
