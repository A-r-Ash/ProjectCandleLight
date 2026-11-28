#include "CLPVesselPresets.h"

// Each preset owns its Blueprint Class Defaults. The GameMode does not overwrite
// these values when a spawn point uses the generated Blueprint class.

ACLPVessel_Candle::ACLPVessel_Candle()
{
	ConfigureVessel(ECLPVesselKind::Candle, TEXT("CANDLE"), 300.f, 720.f, 900.f, FColor(240, 220, 180));
	SetVesselMass(120.f);
	SetVesselSize(20.f, 44.f,
		FVector(0.38f, 0.38f, 1.15f), FVector(0.f, 0.f, -4.f),
		FVector(0.f, 0.f, 62.f), FVector(0.18f, 0.18f, 0.26f));
}

ACLPVessel_Match::ACLPVessel_Match()
{
	ConfigureVessel(ECLPVesselKind::Match, TEXT("MATCH"), 430.f, 470.f, 260.f, FColor(190, 120, 60));
	SetVesselMass(8.f);
	SetVesselSize(5.f, 20.f,
		FVector(0.12f, 0.12f, 0.72f), FVector(0.f, 0.f, -4.f),
		FVector(0.f, 0.f, 42.f), FVector(0.11f, 0.11f, 0.16f));
}

ACLPVessel_Lantern::ACLPVessel_Lantern()
{
	ConfigureVessel(ECLPVesselKind::Lantern, TEXT("LANTERN"), 280.f, 420.f, 620.f, FColor(140, 170, 200));
	SetVesselMass(90.f);
	SetVesselSize(18.f, 36.f,
		FVector(0.62f, 0.50f, 0.90f), FVector(0.f, 0.f, -4.f),
		FVector(0.f, 0.f, 12.f), FVector(0.14f, 0.14f, 0.20f));
}

ACLPVessel_Torch::ACLPVessel_Torch()
{
	ConfigureVessel(ECLPVesselKind::Torch, TEXT("TORCH"), 300.f, 450.f, 700.f, FColor(150, 90, 50));
	SetVesselMass(70.f);
	SetVesselSize(9.f, 44.f,
		FVector(0.12f, 0.12f, 1.10f), FVector(0.f, 0.f, -4.f),
		FVector(0.f, 0.f, 72.f), FVector(0.22f, 0.22f, 0.32f));
}

ACLPVessel_Wisp::ACLPVessel_Wisp()
{
	ConfigureVessel(ECLPVesselKind::Wisp, TEXT("WISP"), 470.f, 530.f, 320.f, FColor(230, 240, 255));
	SetVesselMass(5.f);
	SetVesselSize(10.f, 20.f,
		FVector(0.22f), FVector(0.f, 0.f, 8.f),
		FVector(0.f, 0.f, 8.f), FVector(0.30f));
}

ACLPVessel_WaxPuddle::ACLPVessel_WaxPuddle()
{
	ConfigureVessel(ECLPVesselKind::WaxPuddle, TEXT("WAX PUDDLE"), 240.f, 330.f, 140.f, FColor(230, 170, 80));
	SetVesselMass(60.f);
	SetVesselSize(26.f, 12.f,
		FVector(0.95f, 0.95f, 0.12f), FVector(0.f, 0.f, -38.f),
		FVector(0.f, 0.f, -28.f), FVector(0.14f, 0.14f, 0.10f));
}

ACLPVessel_Beacon::ACLPVessel_Beacon()
{
	ConfigureVessel(ECLPVesselKind::Beacon, TEXT("BEACON"), 160.f, 330.f, 800.f, FColor(110, 130, 160));
	SetVesselMass(200.f);
	SetVesselSize(24.f, 40.f,
		FVector(0.80f, 0.80f, 0.40f), FVector(0.f, 0.f, -26.f),
		FVector(0.f, 0.f, 58.f), FVector(0.28f, 0.28f, 0.28f));
}

ACLPVessel_EmberStone::ACLPVessel_EmberStone()
{
	ConfigureVessel(ECLPVesselKind::EmberStone, TEXT("EMBER STONE"), 220.f, 350.f, 240.f, FColor(130, 65, 50));
	SetVesselMass(240.f);
	SetVesselSize(22.f, 26.f,
		FVector(0.46f), FVector(0.f, 0.f, -2.f),
		FVector(0.f, 0.f, 10.f), FVector(0.12f));
}

ACLPVessel_MirrorShard::ACLPVessel_MirrorShard()
{
	ConfigureVessel(ECLPVesselKind::MirrorShard, TEXT("MIRROR SHARD"), 360.f, 450.f, 380.f, FColor(190, 210, 225));
	SetVesselMass(30.f);
	SetVesselSize(6.f, 40.f,
		FVector(0.12f, 0.60f, 0.90f), FVector(0.f, 0.f, 2.f),
		FVector(0.f, 0.f, 46.f), FVector(0.12f));
}

ACLPVessel_IncenseStick::ACLPVessel_IncenseStick()
{
	ConfigureVessel(ECLPVesselKind::IncenseStick, TEXT("INCENSE STICK"), 330.f, 430.f, 300.f, FColor(110, 90, 70));
	SetVesselMass(10.f);
	SetVesselSize(5.f, 44.f,
		FVector(0.08f, 0.08f, 1.f), FVector(0.f, 0.f, -4.f),
		FVector(0.f, 0.f, 48.f), FVector(0.10f, 0.10f, 0.14f));
}
