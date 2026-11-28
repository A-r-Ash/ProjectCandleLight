#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CLPVessel.h"
#include "CLPPrototypeGameMode.generated.h"

class ACLPVessel;
class UCLPVesselConfig;

/**
 * Candlelight GameMode. Two modes:
 * - Runtime test lane: clears the map and builds a basic-shape lane.
 * - Level mode: preserves placed actors and spawns vessels on ACLPVesselSpawnPoint actors.
 */
UCLASS()
class ACLPPrototypeGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACLPPrototypeGameMode();

	virtual void BeginPlay() override;
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) override;

	/** E input: move the spark to the next vessel in the cycle. */
	UFUNCTION(BlueprintCallable, Category = "Vessel")
	void AdvanceSpark();

	/** Directly place the spark in a chosen vessel. Used by return-to-Candle volumes. */
	UFUNCTION(BlueprintCallable, Category = "Vessel")
	void SetActiveVesselKind(ECLPVesselKind Kind);

	UFUNCTION(BlueprintPure, Category = "Vessel")
	ECLPVesselKind GetActiveVesselKind() const;

protected:
	void ActivateVessel(int32 NewIndex, bool bTeleportTargetToCurrent);
	void ClearTemplateActors();
	void BuildWorld();
	void SpawnVesselsFromLevel();
	void EnsureLevelExitTrigger();
	ACLPVessel* SpawnVesselFromDef(int32 DefIndex, const FVector& Location, TSubclassOf<ACLPVessel> ClassOverride = nullptr);
	void PossessActive();
	void SpawnBlock(const FVector& Location, const FVector& Scale, const FColor& Colour);
	void SpawnHelpText();

	/** True for the quick test lane. ACLPRoomGameMode sets this false. */
	UPROPERTY(EditDefaultsOnly, Category = "Prototype")
	bool bBuildRuntimeTestLane = true;

	/** Authored levels teleport the next body to the spark's position. The test lane keeps bodies spread out. */
	UPROPERTY(EditDefaultsOnly, Category = "Prototype")
	bool bTeleportOnSwap = false;

	/** Tuning asset used by the Candle vessel. */
	UPROPERTY(EditDefaultsOnly, Category = "Candle")
	UCLPVesselConfig* CandleConfig = nullptr;

	/** Tuning asset used by the Match vessel. */
	UPROPERTY(EditDefaultsOnly, Category = "Match")
	UCLPVesselConfig* MatchConfig = nullptr;

	UPROPERTY()
	TArray<ACLPVessel*> Vessels;

	int32 ActiveVesselIndex = 0;
};
