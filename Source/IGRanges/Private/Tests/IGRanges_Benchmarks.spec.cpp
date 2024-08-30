// Copyright Ian Good

#include "IGRanges.h"
#include "IGRangesInternal.h"
#include "Misc/AutomationTest.h"
#include "Tests/Benchmark.h"
#include "UObject/MetaData.h"
#include "UObject/Package.h"

#if WITH_DEV_AUTOMATION_TESTS

DEFINE_SPEC(FIGRangesBenchmarksSpec, "IG.Ranges.Benchmarks", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter);

static TArray<const UObject*> MakeObjectsArray()
{
	const UObject* ObjectCDO = GetDefault<UObject>();
	const UObject* ClassCDO = GetDefault<UClass>();
	const UObject* PackageCDO = GetDefault<UPackage>();
	const UObject* MetaDataCDO = GetDefault<UMetaData>();

	const UObject* SomeObjects[] = {
		ObjectCDO,
		ClassCDO,
		PackageCDO,
		MetaDataCDO,
		nullptr,
		ObjectCDO->GetClass(),
		ClassCDO->GetClass(),
		PackageCDO->GetClass(),
		MetaDataCDO->GetClass(),
		nullptr,
		ObjectCDO->GetPackage(),
		ClassCDO->GetPackage(),
		PackageCDO->GetPackage(),
		MetaDataCDO->GetPackage(),
		nullptr,
	};

	constexpr int32 NumCopies = 500'000;

	TArray<const UObject*> SomeObjectsManyTimes;
	SomeObjectsManyTimes.Reserve(UE_ARRAY_COUNT(SomeObjects) * NumCopies);

	for (int32 i = 0; i < NumCopies; ++i)
	{
		SomeObjectsManyTimes.Append(SomeObjects);
	}

	return SomeObjectsManyTimes;
}

void FIGRangesBenchmarksSpec::Define()
{
	It("complex_chain", [this]() {
		const TArray<const UObject*> MyObjects = MakeObjectsArray();

		// Discard every 3rd thing.
		int32 Skip3sValue = 0;
		const auto DiscardMe = [&Skip3sValue](auto&&) {
			++Skip3sValue;
			return Skip3sValue % 3 == 0;
		};

		// Later, keep every other thing.
		bool bFlipFlop = false;
		const auto FlipFlop = [&bFlipFlop](auto&&) {
			bFlipFlop = !bFlipFlop;
			return bFlipFlop;
		};

		const auto BaselineVersion = [&]() {
			Skip3sValue = 0;
			bFlipFlop = false;
			int32 Results = 0;
			TArray<FString> Names;
			for (const UObject* Obj : MyObjects)
			{
				if (DiscardMe(Obj))
				{
					continue;
				}

				if (const UMetaData* MetaData = Cast<UMetaData>(Obj))
				{
					if (FlipFlop(MetaData))
					{
						const FName Name = MetaData->GetFName();
						FString NameStr = Name.ToString();
						NameStr.AppendInt(++Results);
						Names.Emplace(MoveTemp(NameStr));
					}
				}
			}

			return Names;
		};

		const auto IGRangesVersion = [&]() {
			Skip3sValue = 0;
			bFlipFlop = false;
			int32 Results = 0;
			return MyObjects
				 | WhereNot(DiscardMe)
				 | OfType<UMetaData>()
				 | Where(FlipFlop)
				 | Select(&UMetaData::GetFName)
				 | Select([&Results](auto&& Name) {
					   FString NameStr = Name.ToString();
					   NameStr.AppendInt(++Results);
					   return NameStr;
				   })
				 | ToArray();
		};

		// Sanity check that these versions produce the same results.
		{
			const TArray<FString> ExpectedNames = BaselineVersion();
			const TArray<FString> ActualNames = IGRangesVersion();
			if (!TestEqual("version results", ActualNames, ExpectedNames))
			{
				return;
			}

			UE_LOG(LogIGRangesTests, Log, TEXT("%d elements were filtered & transformed into %d elements."), MyObjects.Num(), ActualNames.Num());
		}

		constexpr int32 NumRuns = 7;
		UE_BENCHMARK(NumRuns, BaselineVersion);
		UE_BENCHMARK(NumRuns, IGRangesVersion);
	});
}

#endif // WITH_DEV_AUTOMATION_TESTS
