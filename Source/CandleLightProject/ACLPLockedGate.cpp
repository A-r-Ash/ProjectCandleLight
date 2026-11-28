#include "ACLPLockedGate.h"

#include "Components/StaticMeshComponent.h"
#include "Materials/Material.h"
#include "Materials/MaterialInterface.h"
#include "UObject/ConstructorHelpers.h"

ACLPLockedGate::ACLPLockedGate()
{
	PrimaryActorTick.bCanEverTick = true;

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	RootComponent = DoorMesh;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeObj(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeObj.Succeeded())
	{
		DoorMesh->SetStaticMesh(CubeObj.Object);
	}
	DoorMesh->SetRelativeScale3D(FVector(1.f, 4.f, 6.f));

	DoorMesh->SetMaterial(0, UMaterial::GetDefaultMaterial(MD_Surface));
}

void ACLPLockedGate::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (DoorMesh)
	{
		if (UMaterialInterface* GateMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Candlelight/Materials/MI_CLP_Gate.MI_CLP_Gate")))
		{
			DoorMesh->SetMaterial(0, GateMaterial);
		}
	}
}

void ACLPLockedGate::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const FVector Target = bIsOpen ? OpenOffset : ClosedOffset;
	CurrentOffset = FMath::VInterpTo(CurrentOffset, Target, DeltaSeconds, MoveSpeed);
	DoorMesh->SetRelativeLocation(CurrentOffset);
}

void ACLPLockedGate::SetOpen(bool bShouldOpen)
{
	if (bIsOpen == bShouldOpen)
	{
		return;
	}

	bIsOpen = bShouldOpen;
	OnStateChanged.Broadcast(bIsOpen);
	UE_LOG(LogTemp, Display, TEXT("CLP Gate %s"), bIsOpen ? TEXT("OPEN") : TEXT("CLOSED"));
}
