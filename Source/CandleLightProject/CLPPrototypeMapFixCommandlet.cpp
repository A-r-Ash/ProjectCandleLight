#include "CLPPrototypeMapFixCommandlet.h"

#include "Engine/World.h"
#include "GameFramework/WorldSettings.h"
#include "Misc/PackageName.h"
#include "UObject/SavePackage.h"
#include "UObject/UObjectGlobals.h"

int32 UCLPPrototypeMapFixCommandlet::Main(const FString& Params)
{
	static const TCHAR* MapPaths[] = {
		TEXT("/Game/ThirdPerson/Lvl_ThirdPerson"),
		TEXT("/Game/Variant_Platforming/Lvl_Platforming"),
		TEXT("/Game/Variant_SideScrolling/Lvl_SideScrolling"),
		TEXT("/Game/Variant_Combat/Lvl_Combat")
	};

	for (const TCHAR* MapPath : MapPaths)
	{
		UPackage* Package = LoadPackage(nullptr, MapPath, LOAD_None);
		if (!Package)
		{
			UE_LOG(LogTemp, Warning, TEXT("CLP MapFix: could not load %s"), MapPath);
			continue;
		}

		UWorld* World = UWorld::FindWorldInPackage(Package);
		AWorldSettings* Settings = World ? World->GetWorldSettings(false, false) : nullptr;
		if (!Settings)
		{
			UE_LOG(LogTemp, Warning, TEXT("CLP MapFix: no WorldSettings in %s"), MapPath);
			continue;
		}

		Settings->Modify();
		Settings->DefaultGameMode = nullptr;
		Package->MarkPackageDirty();

		const FString Filename = FPackageName::LongPackageNameToFilename(MapPath, FPackageName::GetMapPackageExtension());
		if (Filename.IsEmpty())
		{
			UE_LOG(LogTemp, Warning, TEXT("CLP MapFix: no filename for %s"), MapPath);
			continue;
		}

		FSavePackageArgs SaveArgs;
		SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
		SaveArgs.SaveFlags = SAVE_None;
		SaveArgs.bSlowTask = false;

		const bool bSaved = UPackage::SavePackage(Package, World, *Filename, SaveArgs);
		UE_LOG(LogTemp, Display, TEXT("CLP MapFix: %s -> DefaultGameMode cleared, saved=%d"), MapPath, bSaved ? 1 : 0);
	}

	return 0;
}
