// Copyright Ian Good

#include "IGRanges/FirstOrDefault.h"
#include "IGRangesInternal.h"
#include "Misc/AutomationTest.h"
#include <ranges>

#if WITH_DEV_AUTOMATION_TESTS

BEGIN_DEFINE_SPEC(FIGRangesFirstOrDefaultSpec, "IG.Ranges.FirstOrDefault", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

constexpr struct FMyNumber
{
	constexpr FMyNumber() = default;

	constexpr FMyNumber(int32 InN)
		: N(InN)
		, bIsEven(InN % 2 == 0)
	{
	}

	FMyNumber& operator=(const FMyNumber& Other)
	{
		N = Other.N;
		bIsEven = Other.bIsEven;
		return *this;
	}

	bool operator==(const FMyNumber& Other) const
	{
		return N == Other.N && bIsEven == Other.bIsEven;
	}

	int32 N = 0;

	bool bIsEven = true;

	constexpr bool IsEven() const { return N % 2 == 0; }
};

static constexpr auto AlwaysFalse = [](auto&&) {
	return false;
};

template <typename RangeType, typename T>
void TestRange(const FString& What, RangeType&& Range, T Default)
{
	using namespace IG::Ranges;

	T Expected = std::move(Default);
	for (auto&& X : Range)
	{
		if (true) // add this to silence warning C4702: unreachable code
		{
			Expected = X;
			break;
		}
	}

	const T Actual = Range | FirstOrDefault();
	TestEqual(What, Actual, Expected);
}

template <typename RangeType, class _Pr, typename T>
void TestFilteredRange(const FString& What, RangeType&& Range, _Pr&& _Pred, T Default)
{
	using namespace IG::Ranges;

	T Expected = std::move(Default);
	for (auto&& X : Range)
	{
		if (std::invoke(_Pred, X))
		{
			Expected = X;
			break;
		}
	}

	const T Actual = Range | FirstOrDefault(_Pred);
	TestEqual(What, Actual, Expected);
}

END_DEFINE_SPEC(FIGRangesFirstOrDefaultSpec)

void FIGRangesFirstOrDefaultSpec::Define()
{
	static constexpr int32 SomeValues[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

	static constexpr FMyNumber SomeNumbers[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

	const TSharedPtr<int32> A = MakeShared<int32>(1);
	const TSharedPtr<int32> B = MakeShared<int32>(2);
	const TSharedPtr<int32> C = MakeShared<int32>(3);
	const TSharedPtr<int32> D = MakeShared<int32>(4);
	const TSharedPtr<int32> SomePointers[] = {nullptr, A, B, C, D, nullptr, nullptr, D, A, D};

	It("empty (int32)", [this]() {
		TestRange("default", std::ranges::empty_view<int32>(), 0);
	});

	It("empty (struct)", [this]() {
		TestRange("default FMyNumber", std::ranges::empty_view<FMyNumber>(), FMyNumber{});
		// Test that `FVector` returns "zero" instead of uninitialized.
		TestRange("default FVector", std::ranges::empty_view<FVector>(), FVector::ZeroVector);
		// Test that `FQuat` returns "identity" instead of uninitialized.
		TestRange("default FQuat", std::ranges::empty_view<FQuat>(), FQuat::Identity);
	});

	It("empty (shared ptr)", [this]() {
		TestRange("default", std::ranges::empty_view<TSharedPtr<int32>>(), TSharedPtr<int32>{});
	});

	It("many (int32)", [this]() {
		TestRange("first", SomeValues, 0);
	});

	It("many (struct)", [this]() {
		TestRange("first", SomeNumbers, FMyNumber{});
	});

	It("many (shared ptr)", [this, SomePointers]() {
		TestRange("first", SomePointers, TSharedPtr<int32>{});
	});

	It("many_filtered (int32)", [this]() {
		const auto IsEven = [](auto&& x) {
			return x % 2 == 0;
		};

		TestFilteredRange("default", SomeValues, AlwaysFalse, 0);
		TestFilteredRange("filter+first", SomeValues, IsEven, 0);
	});

	It("many_filtered (struct)", [this]() {
		TestFilteredRange("default", SomeNumbers, AlwaysFalse, FMyNumber{});
		TestFilteredRange("filter+first", SomeNumbers, &FMyNumber::bIsEven, FMyNumber{});
		TestFilteredRange("filter+first", SomeNumbers, &FMyNumber::IsEven, FMyNumber{});
	});

	It("many_filtered (shared ptr)", [this, SomePointers]() {
		const auto IsEven = [](auto&& x) {
			return x.IsValid() && (*x % 2 == 0);
		};

		TestFilteredRange("default", SomePointers, AlwaysFalse, TSharedPtr<int32>{});
		TestFilteredRange("filter+first", SomePointers, IsEven, TSharedPtr<int32>{});
	});
}

#endif // WITH_DEV_AUTOMATION_TESTS
