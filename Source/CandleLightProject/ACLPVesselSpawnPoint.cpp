#include "ACLPVesselSpawnPoint.h"

#include "Components/TextRenderComponent.h"
#include "Components/SceneComponent.h"

ACLPVesselSpawnPoint::ACLPVesselSpawnPoint()
{
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	MarkerLabel = CreateDefaultSubobject<UTextRenderComponent>(TEXT("MarkerLabel"));
	MarkerLabel->SetupAttachment(RootComponent);
	MarkerLabel->SetWorldSize(16.f);
	MarkerLabel->SetTextRenderColor(FColor(120, 200, 255));
	MarkerLabel->SetHorizontalAlignment(EHTA_Center);
	MarkerLabel->SetRelativeLocation(FVector(0.f, 0.f, 20.f));
	MarkerLabel->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	MarkerLabel->SetHiddenInGame(true);
}

void ACLPVesselSpawnPoint::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (!MarkerLabel)
	{
		return;
	}

	const TCHAR* Name = TEXT("CANDLE");
	switch (VesselKind)
	{
	case ECLPVesselKind::Match:        Name = TEXT("MATCH"); break;
	case ECLPVesselKind::Lantern:      Name = TEXT("LANTERN"); break;
	case ECLPVesselKind::Torch:        Name = TEXT("TORCH"); break;
	case ECLPVesselKind::Wisp:         Name = TEXT("WISP"); break;
	case ECLPVesselKind::WaxPuddle:    Name = TEXT("WAX PUDDLE"); break;
	case ECLPVesselKind::Beacon:       Name = TEXT("BEACON"); break;
	case ECLPVesselKind::EmberStone:   Name = TEXT("EMBER STONE"); break;
	case ECLPVesselKind::MirrorShard:  Name = TEXT("MIRROR SHARD"); break;
	case ECLPVesselKind::IncenseStick: Name = TEXT("INCENSE STICK"); break;
	default: break;
	}

	MarkerLabel->SetText(FText::Format(FText::FromString(TEXT("SPAWN: {0}")), FText::FromString(Name)));
}
