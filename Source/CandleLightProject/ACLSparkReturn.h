#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CLPVessel.h"
#include "ACLSparkReturn.generated.h"

class UBoxComponent;
class UPointLightComponent;
class UStaticMeshComponent;
class UTextRenderComponent;
class UPrimitiveComponent;
struct FHitResult;

/**
 * Trigger volume that sends the spark back into a chosen body.
 * Level 2 uses it to return Match -> Candle at the far side of the crawl tunnel.
 */
UCLASS(Blueprintable)
class ACLSparkReturn : public AActor
{
	GENERATED_BODY()

public:
	ACLSparkReturn();

	virtual void OnConstruction(const FTransform& Transform) override;

	/** Only triggers when this vessel is holding the spark. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spark Return")
	ECLPVesselKind FromVesselKind = ECLPVesselKind::Match;

	/** Vessel the spark returns to. Level 2 uses Candle. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spark Return")
	ECLPVesselKind ReturnVesselKind = ECLPVesselKind::Candle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spark Return")
	bool bTriggerOnce = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spark Return")
	FString ReturnLabel = TEXT("RETURN TO CANDLE");

protected:
	UFUNCTION()
	void HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, Category = "Spark Return")
	UBoxComponent* TriggerBox;

	UPROPERTY(VisibleAnywhere, Category = "Spark Return")
	UStaticMeshComponent* MarkerMesh;

	UPROPERTY(VisibleAnywhere, Category = "Spark Return")
	UTextRenderComponent* Label;

	UPROPERTY(VisibleAnywhere, Category = "Spark Return")
	UPointLightComponent* Glow;

	bool bTriggered = false;
};
