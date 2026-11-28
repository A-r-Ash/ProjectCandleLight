#include "ACLCollectible.h"

#include "ACLPRoomManager.h"
#include "CLPVessel.h"
#include "Components/PointLightComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/Material.h"
#include "Materials/MaterialInterface.h"
#include "UObject/ConstructorHelpers.h"

ACLCollectible::ACLCollectible()
{
	PrimaryActorTick.bCanEverTick = true;

	TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerSphere"));
	RootComponent = TriggerSphere;
	TriggerSphere->SetSphereRadius(48.f);
	TriggerSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerSphere->SetCollisionResponseToAllChannels(ECR_Overlap);
	TriggerSphere->SetGenerateOverlapEvents(true);

	FragmentMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FragmentMesh"));
	FragmentMesh->SetupAttachment(RootComponent);
	FragmentMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FragmentMesh->SetRelativeScale3D(FVector(0.22f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereObj(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (SphereObj.Succeeded())
	{
		FragmentMesh->SetStaticMesh(SphereObj.Object);
	}

	FragmentMesh->SetMaterial(0, UMaterial::GetDefaultMaterial(MD_Surface));

	Glow = CreateDefaultSubobject<UPointLightComponent>(TEXT("Glow"));
	Glow->SetupAttachment(RootComponent);
	Glow->SetLightColor(FLinearColor(1.f, 0.85f, 0.45f));
	Glow->SetAttenuationRadius(220.f);
	Glow->SetIntensity(1800.f);

	TriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &ACLCollectible::HandleBeginOverlap);
}

void ACLCollectible::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (FragmentMesh)
	{
		if (UMaterialInterface* CollectibleMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Candlelight/Materials/MI_CLP_Collectible.MI_CLP_Collectible")))
		{
			FragmentMesh->SetMaterial(0, CollectibleMaterial);
		}
	}
}

void ACLCollectible::BeginPlay()
{
	Super::BeginPlay();
	BaseLocation = GetActorLocation();

	if (!RoomManager)
	{
		TArray<AActor*> Managers;
		UGameplayStatics::GetAllActorsOfClass(this, ACLPRoomManager::StaticClass(), Managers);
		RoomManager = Managers.Num() > 0 ? Cast<ACLPRoomManager>(Managers[0]) : nullptr;
	}
}

void ACLCollectible::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bCollected)
	{
		return;
	}

	BobTime += DeltaSeconds;
	FragmentMesh->AddRelativeRotation(FRotator(0.f, 60.f * DeltaSeconds, 0.f));
	SetActorLocation(BaseLocation + FVector(0.f, 0.f, FMath::Sin(BobTime * 2.f) * 8.f));
}

void ACLCollectible::HandleBeginOverlap(UPrimitiveComponent*, AActor* OtherActor, UPrimitiveComponent*, int32, bool, const FHitResult&)
{
	ACLPVessel* Vessel = Cast<ACLPVessel>(OtherActor);
	if (Vessel && Vessel->IsSparkActive())
	{
		Collect();
	}
}

void ACLCollectible::Collect()
{
	if (bCollected)
	{
		return;
	}

	bCollected = true;
	TriggerSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FragmentMesh->SetVisibility(false);
	Glow->SetIntensity(0.f);

	if (RoomManager)
	{
		RoomManager->NotifyCollectibleCollected(this);
	}
	OnCollected.Broadcast();
	UE_LOG(LogTemp, Display, TEXT("CLP Collectible collected"));
}
