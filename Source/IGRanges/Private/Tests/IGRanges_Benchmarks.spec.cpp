// Copyright Ian Good

#include "IGRanges.h"
#include "IGRangesInternal.h"
#include "Misc/AutomationTest.h"
#include "Tests/Benchmark.h"
#include "UObject/MetaData.h"
#include "UObject/Package.h"
#include <numeric>

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

	It("accumulate", [this]() {
		const TArray<const UObject*> MyObjects = MakeObjectsArray();

		const auto BaselineVersion = [&]() {
			uint64 Result = 0;
			for (const UObject* Elem : MyObjects)
			{
				Result += (Elem != nullptr) ? Elem->GetName().Len() : 0;
			}

			return Result;
		};

		const auto Fold = [](uint64 Acc, const UObject* Elem) {
			return Acc + ((Elem != nullptr) ? Elem->GetName().Len() : 0);
		};

		const auto StdVersion = [&]() {
			return std::accumulate(MyObjects.begin(), MyObjects.end(), uint64{}, Fold);
		};

		const auto IGRangesVersion = [&]() {
			return MyObjects | Accumulate(uint64{}, Fold);
		};

		// Sanity check that these versions produce the same results.
		{
			const int32 Expected = BaselineVersion();
			const int32 StdActual = StdVersion();
			const int32 IgrActual = IGRangesVersion();
			const bool bSuccess =
				TestEqual("std version results", StdActual, Expected)
				&& TestEqual("igr version results", IgrActual, Expected);
			if (!bSuccess)
			{
				return;
			}

			UE_LOG(LogIGRangesTests, Log, TEXT("%d elements were accumulated."), MyObjects.Num());
		}

		constexpr int32 NumRuns = 7;
		UE_BENCHMARK(NumRuns, BaselineVersion);
		UE_BENCHMARK(NumRuns, StdVersion);
		UE_BENCHMARK(NumRuns, IGRangesVersion);
	});

	It("sum", [this]() {
		const TArray<const UObject*> MyObjects = MakeObjectsArray();

		const auto BaselineVersion = [&]() {
			int32 Result = 0;
			for (const UObject* Elem : MyObjects)
			{
				Result += (Elem != nullptr) ? Elem->GetName().Len() : 0;
			}

			return Result;
		};

		const auto Fold = [](int32 Acc, const UObject* Elem) {
			return Acc + ((Elem != nullptr) ? Elem->GetName().Len() : 0);
		};

		const auto StdVersion = [&]() {
			return std::accumulate(MyObjects.begin(), MyObjects.end(), int32{}, Fold);
		};

		const auto SumSelector = [](const UObject* Elem) {
			return (Elem != nullptr) ? Elem->GetName().Len() : 0;
		};

		const auto IGRangesVersion = [&]() {
			return MyObjects | Sum(SumSelector);
		};

		// Sanity check that these versions produce the same results.
		{
			const int32 Expected = BaselineVersion();
			const int32 StdActual = StdVersion();
			const int32 IgrActual = IGRangesVersion();
			const bool bSuccess =
				TestEqual("std version results", StdActual, Expected)
				&& TestEqual("igr version results", IgrActual, Expected);
			if (!bSuccess)
			{
				return;
			}

			UE_LOG(LogIGRangesTests, Log, TEXT("%d elements were summed."), MyObjects.Num());
		}

		constexpr int32 NumRuns = 7;
		UE_BENCHMARK(NumRuns, BaselineVersion);
		UE_BENCHMARK(NumRuns, StdVersion);
		UE_BENCHMARK(NumRuns, IGRangesVersion);
	});

	It("lookup", [this]() {
		const TArray<const UObject*> MyObjects = MakeObjectsArray();

		using FMapType = TMap<const UObject*, int32>;

		const auto GetMap = [&]() {
			FMapType Counts;

			for (const UObject* Elem : MyObjects)
			{
				Counts.Emplace(Elem, 0);
			}

			return Counts;
		};

		auto BaselineVersion = [&, Counts = GetMap()]() mutable {
			for (const UObject* Elem : MyObjects)
			{
				if (int32* Found = (Elem != nullptr) ? Counts.Find(Elem) : nullptr)
				{
					++(*Found);
				}
			}

			return Counts;
		};

		auto IGRangesVersion = [&, Counts = GetMap()]() mutable {
			for (int32& Count : MyObjects | LookupRef(Counts))
			{
				++Count;
			}

			return Counts;
		};

		// Sanity check that these versions produce the same results.
		{
			const FMapType Expected = BaselineVersion();
			const FMapType IgrActual = IGRangesVersion();
			const bool bSuccess = TestTrue("igr version results", IgrActual.OrderIndependentCompareEqual(Expected));
			if (!bSuccess)
			{
				return;
			}

			UE_LOG(LogIGRangesTests, Log, TEXT("%d elements were looked up."), MyObjects.Num());
		}

		constexpr int32 NumRuns = 7;
		UE_BENCHMARK(NumRuns, BaselineVersion);
		UE_BENCHMARK(NumRuns, IGRangesVersion);
	});
}

#endif // WITH_DEV_AUTOMATION_TESTS
