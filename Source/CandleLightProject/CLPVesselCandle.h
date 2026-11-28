#pragma once

#include "CoreMinimal.h"
#include "CLPVessel.h"
#include "CLPVesselCandle.generated.h"

class UCLPVesselConfig;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCLPCandleWaxEvent);

/**
 * Candle is the clock. Its light shrinks while the spark lives in it.
 * The body is heavy, slow and tall: it holds plates but cannot fit crawl gaps.
 */
UCLASS(Blueprintable)
class ACLPVesselCandle : public ACLPVessel
{
	GENERATED_BODY()

public:
	ACLPVesselCandle();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetSparkActive(bool bActive) override;

	/** Re-reads the tuning asset. Safe to call after spawning and assigning CandleConfig. */
	UFUNCTION(BlueprintCallable, Category = "Candle")
	void ApplyConfig();

	UFUNCTION(BlueprintCallable, Category = "Candle")
	float GetWaxRatio() const { return FMath::Clamp(RemainingWaxSeconds / FMath::Max(1.f, MaxWaxSeconds), 0.f, 1.f); }

	UFUNCTION(BlueprintCallable, Category = "Candle")
	void RefillWax();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Candle")
	UCLPVesselConfig* CandleConfig = nullptr;

	UPROPERTY(BlueprintAssignable, Category = "Candle")
	FCLPCandleWaxEvent OnLowWax;

	UPROPERTY(BlueprintAssignable, Category = "Candle")
	FCLPCandleWaxEvent OnExpired;

protected:
	void UpdateLightFromWax();

	float MaxWaxSeconds = 180.f;
	float RemainingWaxSeconds = 180.f;
	float BaseLightRadius = 900.f;
	float BaseLightIntensity = 8000.f;
	float LowWaxThreshold = 0.25f;
	bool bLowWaxFired = false;
	bool bExpiredFired = false;
};
