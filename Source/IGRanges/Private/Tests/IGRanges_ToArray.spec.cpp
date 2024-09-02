// Copyright Ian Good

#include "IGRanges/CPOs.h"
#include "IGRanges/ToArray.h"
#include "IGRangesInternal.h"
#include "Misc/AutomationTest.h"
#include <ranges>

#if WITH_DEV_AUTOMATION_TESTS

DEFINE_SPEC(FIGRangesToArraySpec, "IG.Ranges.ToArray", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter);

void FIGRangesToArraySpec::Define()
{
	using namespace IG::Ranges;

	// These values represent our "expected array".
	// This number of elements has been chosen because we expect the reallocation strategy for `TArray` will give us
	// different values for `Num` & `Max` when adding them one at a time.
	static constexpr int32 SomeValues[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	static constexpr int32 NumSomeValues = UE_ARRAY_COUNT(SomeValues);

	// `ToArray` with an empty range produces an array with zero count & capacity.
	It("empty", [this]() {
		const TArray<int32> TestMe = std::ranges::empty_view<int32>() | ToArray();

		TestEqual("count", TestMe.Num(), 0);
		TestEqual("capacity", TestMe.Max(), 0);
	});

	// `ToArray` with a range whose size is known produces an array with equal count & capacity.
	It("many", [this]() {
		TArray<int32> ExpectedArray;
		ExpectedArray.Append(SomeValues);

		const TArray<int32> TestMe = SomeValues | ToArray();

		TestEqual("count", TestMe.Num(), NumSomeValues);
		TestEqual("capacity", TestMe.Max(), NumSomeValues);
		TestEqual("contents", TestMe, ExpectedArray);
	});

	// `ToArray` with a range whose size is known produces an array with equal count & capacity.
	// (works even with transformations)
	It("many_transformed", [this]() {
		const auto Square = [](auto&& x) {
			return x * x;
		};

		TArray<int32> ExpectedArray;
		for (auto&& X : SomeValues)
		{
			ExpectedArray.Emplace(Square(X));
		}

		const auto TestArray = [&, this](const auto& A) {
			TestEqual("count", A.Num(), NumSomeValues);
			TestEqual("capacity", A.Max(), NumSomeValues);
			TestEqual("contents", A, ExpectedArray);
		};

		// `ToArray` with 0 arguments (just creates an array from the view).
		TArray<int32> TestMe = SomeValues | std::views::transform(Square) | ToArray();
		TestArray(TestMe);

		// `ToArray` with 1 transformation argument (applies the transformation).
		TestMe = SomeValues | ToArray(Square);
		TestArray(TestMe);
	});

	// `ToArray` with a range whose size is unknown produces an array with count & capacity consistent with traditional
	// `TArray` usage.
	It("many_filtered", [this]() {
		const auto IsEven = [](auto&& x) {
			return x % 2 == 0;
		};

		TArray<int32> ExpectedArray;
		for (auto&& X : SomeValues)
		{
			if (IsEven(X))
			{
				ExpectedArray.Emplace(X);
			}
		}

		const TArray<int32> TestMe = SomeValues | std::views::filter(IsEven) | ToArray();

		TestEqual("count", TestMe.Num(), ExpectedArray.Num());
		TestEqual("capacity", TestMe.Max(), ExpectedArray.Max());
		TestEqual("contents", TestMe, ExpectedArray);
	});
}

#endif // WITH_DEV_AUTOMATION_TESTS
