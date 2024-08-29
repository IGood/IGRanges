// Copyright Ian Good

#include "IGRanges/CPOs.h"
#include "IGRanges/ToSet.h"
#include "IGRangesInternal.h"
#include "Misc/AutomationTest.h"
#include <ranges>

#if WITH_DEV_AUTOMATION_TESTS

DEFINE_SPEC(FIGRangesToSetSpec, "IG.Ranges.ToSet", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter);

void FIGRangesToSetSpec::Define()
{
	using namespace IG::Ranges;

	static constexpr int32 SomeValues[] = {10, 8, 6, 4, 2, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10};
	static constexpr int32 NumSomeValues = UE_ARRAY_COUNT(SomeValues);

	// `ToSet` with an empty range produces a set with zero count.
	It("empty", [this]() {
		const TSet<int32> TestMe = std::ranges::empty_view<int32>() | ToSet();

		TestEqual("count", TestMe.Num(), 0);
	});

	// `ToSet` produces a set equal to traditional `TSet` usage.
	It("many", [this]() {
		TSet<int32> ExpectedSet;
		ExpectedSet.Reserve(NumSomeValues);
		for (auto&& X : SomeValues)
		{
			ExpectedSet.Emplace(X);
		}

		const TSet<int32> TestMe = SomeValues | ToSet();

		TestEqual("count", TestMe.Num(), ExpectedSet.Num());
		TestTrue("equal sets", TestMe.Difference(ExpectedSet).IsEmpty());
	});
}

#endif // WITH_DEV_AUTOMATION_TESTS
