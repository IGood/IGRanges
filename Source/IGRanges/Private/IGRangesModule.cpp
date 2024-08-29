// Copyright Ian Good

#include "CoreMinimal.h"
#include "IGRangesInternal.h"
#include "Modules/ModuleManager.h"

DEFINE_LOG_CATEGORY(LogIGRanges);
DEFINE_LOG_CATEGORY(LogIGRangesTests);

namespace IG::Ranges
{
class FModule : public IModuleInterface
{
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

void FModule::StartupModule()
{
	UE_LOG(LogIGRanges, Log, TEXT("Hello, World!"));
}

void FModule::ShutdownModule()
{
	UE_LOG(LogIGRanges, Log, TEXT("Goodbye, World!"));
}

} // namespace IG::Ranges

IMPLEMENT_MODULE(IG::Ranges::FModule, IGRanges)
