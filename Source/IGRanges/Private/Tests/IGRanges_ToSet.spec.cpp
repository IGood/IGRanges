// Copyright Ian Good

#include "IGRanges/ToSet.h"
#include "IGRangesInternal.h"
#include "Misc/AutomationTest.h"
#include <ranges>

#if WITH_DEV_AUTOMATION_TESTS

BEGIN_DEFINE_SPEC(FIGRangesToSetSpec, "IG.Ranges.ToSet", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

void TestSet(const TSet<int32>& Actual, const TSet<int32>& Expected)
{
	TestEqual("count", Actual.Num(), Expected.Num());
	TestTrue("equal sets", Actual.Difference(Expected).IsEmpty());
};

END_DEFINE_SPEC(FIGRangesToSetSpec)

void FIGRangesToSetSpec::Define()
{
	using namespace IG::Ranges;

	static constexpr int32 SomeValues[] = {10, 8, 6, 4, 2, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10};
	static constexpr int32 NumSomeValues = UE_ARRAY_COUNT(SomeValues);

	// `ToSet` with an empty range produces a set with zero count.
	It("empty", [this]() {
		const TSet<int32> TestMe = std::ranges::empty_view<int32>() | ToSet();
		TestSet(TestMe, {});
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
		TestSet(TestMe, ExpectedSet);
	});

	// `ToSet` produces a set equal to traditional `TSet` usage.
	// (works even with transformations)
	It("many_transformed", [this]() {
		const auto Square = [](auto&& x) {
			return x * x;
		};

		TSet<int32> ExpectedSet;
		ExpectedSet.Reserve(NumSomeValues);
		for (auto&& X : SomeValues)
		{
			ExpectedSet.Emplace(Square(X));
		}

		TSet<int32> TestMe = SomeValues | std::views::transform(Square) | ToSet();
		TestSet(TestMe, ExpectedSet);

		TestMe = SomeValues | ToSet(Square);
		TestSet(TestMe, ExpectedSet);
	});
}

#endif // WITH_DEV_AUTOMATION_TESTS
