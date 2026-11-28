#pragma once

#include "CoreMinimal.h"
#include "CLPVessel.h"
#include "CLPVesselCandle.h"
#include "CLPVesselPresets.generated.h"

/**
 * Native preset classes. Each one applies its vessel's default mesh, capsule and
 * light values in the constructor, so a Blueprint child shows the correct setup
 * immediately when opened in the Blueprint editor.
 */

UCLASS(Blueprintable)
class ACLPVessel_Candle : public ACLPVesselCandle
{
	GENERATED_BODY()
public:
	ACLPVessel_Candle();
};

UCLASS(Blueprintable)
class ACLPVessel_Match : public ACLPVessel
{
	GENERATED_BODY()
public:
	ACLPVessel_Match();
};

UCLASS(Blueprintable)
class ACLPVessel_Lantern : public ACLPVessel
{
	GENERATED_BODY()
public:
	ACLPVessel_Lantern();
};

UCLASS(Blueprintable)
class ACLPVessel_Torch : public ACLPVessel
{
	GENERATED_BODY()
public:
	ACLPVessel_Torch();
};

UCLASS(Blueprintable)
class ACLPVessel_Wisp : public ACLPVessel
{
	GENERATED_BODY()
public:
	ACLPVessel_Wisp();
};

UCLASS(Blueprintable)
class ACLPVessel_WaxPuddle : public ACLPVessel
{
	GENERATED_BODY()
public:
	ACLPVessel_WaxPuddle();
};

UCLASS(Blueprintable)
class ACLPVessel_Beacon : public ACLPVessel
{
	GENERATED_BODY()
public:
	ACLPVessel_Beacon();
};

UCLASS(Blueprintable)
class ACLPVessel_EmberStone : public ACLPVessel
{
	GENERATED_BODY()
public:
	ACLPVessel_EmberStone();
};

UCLASS(Blueprintable)
class ACLPVessel_MirrorShard : public ACLPVessel
{
	GENERATED_BODY()
public:
	ACLPVessel_MirrorShard();
};

UCLASS(Blueprintable)
class ACLPVessel_IncenseStick : public ACLPVessel
{
	GENERATED_BODY()
public:
	ACLPVessel_IncenseStick();
};
