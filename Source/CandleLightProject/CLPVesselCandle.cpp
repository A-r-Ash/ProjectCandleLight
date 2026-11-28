#include "CLPVesselCandle.h"

#include "CLPVesselConfig.h"
#include "Components/PointLightComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ACLPVesselCandle::ACLPVesselCandle()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACLPVesselCandle::BeginPlay()
{
	Super::BeginPlay();
	ApplyConfig();
	RefillWax();
}

void ACLPVesselCandle::ApplyConfig()
{
	if (!CandleConfig)
	{
		return;
	}

	MaxWaxSeconds = FMath::Max(1.f, CandleConfig->BurnDurationSeconds);
	LowWaxThreshold = FMath::Clamp(CandleConfig->LowWaxThreshold, 0.f, 1.f);
	BaseLightRadius = FMath::Max(1.f, CandleConfig->LightRadius);
	BaseLightIntensity = FMath::Max(1.f, CandleConfig->LightIntensity);

	GetCharacterMovement()->MaxWalkSpeed = CandleConfig->MoveSpeed;
	GetCharacterMovement()->JumpZVelocity = CandleConfig->JumpVelocity;
	GetCharacterMovement()->GravityScale = CandleConfig->GravityScale;
	SetVesselMass(CandleConfig->Mass);

	if (FlameLight)
	{
		FlameLight->SetAttenuationRadius(BaseLightRadius);
	}

	UpdateLightFromWax();
}

void ACLPVesselCandle::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!IsSparkActive() || bExpiredFired)
	{
		return;
	}

	RemainingWaxSeconds = FMath::Max(0.f, RemainingWaxSeconds - DeltaSeconds);
	UpdateLightFromWax();

	if (!bLowWaxFired && GetWaxRatio() <= LowWaxThreshold)
	{
		bLowWaxFired = true;
		OnLowWax.Broadcast();
		UE_LOG(LogTemp, Display, TEXT("CLP Candle low wax: %.0f%%"), GetWaxRatio() * 100.f);
	}

	if (RemainingWaxSeconds <= 0.f && !bExpiredFired)
	{
		bExpiredFired = true;
		OnExpired.Broadcast();
		UE_LOG(LogTemp, Display, TEXT("CLP Candle expired"));
	}
}

void ACLPVesselCandle::SetSparkActive(bool bActive)
{
	Super::SetSparkActive(bActive);
	UpdateLightFromWax();
}

void ACLPVesselCandle::RefillWax()
{
	RemainingWaxSeconds = MaxWaxSeconds;
	bLowWaxFired = false;
	bExpiredFired = false;
	UpdateLightFromWax();
}

void ACLPVesselCandle::UpdateLightFromWax()
{
	if (!FlameLight)
	{
		return;
	}

	const float Ratio = GetWaxRatio();
	const bool bLit = IsSparkActive() && !bExpiredFired;

	FlameLight->SetAttenuationRadius(bLit ? BaseLightRadius * FMath::Lerp(0.35f, 1.f, Ratio) : 0.f);
	FlameLight->SetIntensity(bLit ? BaseLightIntensity * FMath::Lerp(0.25f, 1.f, Ratio) : 0.f);
}
