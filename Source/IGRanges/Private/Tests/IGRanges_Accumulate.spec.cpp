// Copyright Ian Good

#include "IGRanges/Accumulate.h"
#include "IGRangesInternal.h"
#include "Misc/AutomationTest.h"
#include <numeric>
#include <ranges>

#if WITH_DEV_AUTOMATION_TESTS

DEFINE_SPEC(FIGRangesAccumulateSpec, "IG.Ranges.Accumulate", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter);

void FIGRangesAccumulateSpec::Define()
{
	using namespace IG::Ranges;

	static const FString Seed = TEXT("🌱");

	static const auto Fold = [](FString Acc, int32 Elem) {
		Acc
			.AppendChar(TEXT(','))
			.AppendInt(Elem);
		return Acc;
	};

	It("empty", [this]() {
		const int32* Empty = nullptr;
		const FString ExpectedAccumulate = std::accumulate(Empty, Empty, Seed, Fold);
		const FString ActualAccumulate = std::ranges::empty_view<int32>() | Accumulate(Seed, Fold);
		TestEqual("accumulate", ActualAccumulate, ExpectedAccumulate);
	});

	It("single", [this]() {
		const int32 Single[] = {123};
		const FString ExpectedAccumulate = std::accumulate(Single, Single + UE_ARRAY_COUNT(Single), Seed, Fold);
		const FString ActualAccumulate = Single | Accumulate(Seed, Fold);
		TestEqual("accumulate", ActualAccumulate, ExpectedAccumulate);
	});

	It("many", [this]() {
		const int32 SomeValues[] = {1, 2, 3, 4, 5};
		const FString ExpectedAccumulate = std::accumulate(SomeValues, SomeValues + UE_ARRAY_COUNT(SomeValues), Seed, Fold);
		const FString ActualAccumulate = SomeValues | Accumulate(Seed, Fold);
		TestEqual("accumulate", ActualAccumulate, ExpectedAccumulate);
	});
}

#endif // WITH_DEV_AUTOMATION_TESTS
