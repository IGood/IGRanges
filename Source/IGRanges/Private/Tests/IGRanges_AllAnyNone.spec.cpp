// Copyright Ian Good

#include "IGRanges/AllAnyNone.h"
#include "IGRanges/Impl/Common.h"
#include "IGRangesInternal.h"
#include "Misc/AutomationTest.h"
#include <functional>
#include <ranges>

#if WITH_DEV_AUTOMATION_TESTS

BEGIN_DEFINE_SPEC(FIGRangesAllAnyNoneSpec, "IG.Ranges.AllAnyNone", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

static constexpr auto AlwaysTrue = [](auto&&) {
	return true;
};

static constexpr auto AlwaysFalse = [](auto&&) {
	return false;
};

static bool IsThree(int32 X)
{
	return X == 3;
}

static bool IsFour(const TSharedPtr<int32>& X)
{
	return X != nullptr && *X == 4;
}

template <typename RangeType>
void TestRange(const RangeType& Range)
{
	using namespace IG::Ranges;

	const bool bExpectedAll = std::ranges::all_of(Range, [](auto&& x) { return !!x; });
	const bool bActualAll = Range | All();
	TestEqual("all", bActualAll, bExpectedAll);

	const bool bExpectedAny = std::ranges::any_of(Range, AlwaysTrue);
	const bool bActualAny = Range | Any();
	TestEqual("any", bExpectedAny, bExpectedAny);

	const bool bExpectedNone = std::ranges::none_of(Range, [](auto&& x) { return !!x; });
	const bool bActualNone = Range | None();
	TestEqual("none", bActualNone, bActualNone);
}

template <typename RangeType, typename PredicateT>
void TestRange(const RangeType& Range, const PredicateT& Predicate)
{
	using namespace IG::Ranges;

	const bool bExpectedAll = std::ranges::all_of(Range, Predicate);
	const bool bActualAll = Range | All(Predicate);
	TestEqual("all", bActualAll, bExpectedAll);

	const bool bExpectedAny = std::ranges::any_of(Range, Predicate);
	const bool bActualAny = Range | Any(Predicate);
	TestEqual("any", bActualAny, bExpectedAny);

	const bool bExpectedNone = std::ranges::none_of(Range, Predicate);
	const bool bActualNone = Range | None(Predicate);
	TestEqual("none", bActualNone, bActualNone);
}

END_DEFINE_SPEC(FIGRangesAllAnyNoneSpec)

void FIGRangesAllAnyNoneSpec::Define()
{
	using namespace IG::Ranges;

	static constexpr int32 SomeValues[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

	const TSharedPtr<int32> A = MakeShared<int32>(1);
	const TSharedPtr<int32> B = MakeShared<int32>(2);
	const TSharedPtr<int32> C = MakeShared<int32>(3);
	const TSharedPtr<int32> D = MakeShared<int32>(4);
	const TSharedPtr<int32> SomePointers[] = {nullptr, A, B, C, D, nullptr, nullptr, D, A, D};

	It("empty (int32)", [this]() {
		auto Empty = std::ranges::empty_view<int32>();
		TestRange(Empty);
		TestRange(Empty, AlwaysTrue);
		TestRange(Empty, AlwaysFalse);
		TestRange(Empty, &IsThree);
		TestRange(Empty, [](int32 X) { return X == 7; });
	});

	It("empty (shared ptr)", [this]() {
		auto Empty = std::ranges::empty_view<TSharedPtr<int32>>();
		TestRange(Empty);
		TestRange(Empty, AlwaysTrue);
		TestRange(Empty, AlwaysFalse);
		TestRange(Empty, std::mem_fn(&TSharedPtr<int32>::IsValid));
		TestRange(Empty, &IsFour);
		TestRange(Empty, [](const TSharedPtr<int32>& X) { return X != nullptr && *X == 7; });
	});

	It("many (int32)", [this]() {
		TestRange(SomeValues);
		TestRange(SomeValues, AlwaysTrue);
		TestRange(SomeValues, AlwaysFalse);
		TestRange(SomeValues, &IsThree);
		TestRange(SomeValues, [](int32 X) { return X == 7; });
	});

	It("many (shared ptr)", [this, SomePointers]() {
		TestRange(SomePointers);
		TestRange(SomePointers, AlwaysTrue);
		TestRange(SomePointers, AlwaysFalse);
		TestRange(SomePointers, std::mem_fn(&TSharedPtr<int32>::IsValid));
		TestRange(SomePointers, &IsFour);
		TestRange(SomePointers, [](const TSharedPtr<int32>& X) { return X != nullptr && *X == 7; });
	});
}

#endif // WITH_DEV_AUTOMATION_TESTS
