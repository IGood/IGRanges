// Copyright Ian Good

#include "IGRanges/Sum.h"
#include "IGRangesInternal.h"
#include "Misc/AutomationTest.h"
#include <functional>
#include <numeric>
#include <ranges>

#if WITH_DEV_AUTOMATION_TESTS

DEFINE_SPEC(FIGRangesSumSpec, "IG.Ranges.Sum", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter);

void FIGRangesSumSpec::Define()
{
	using namespace IG::Ranges;

	It("empty", [this]() {
		{
			const int32 ActualSum = std::ranges::empty_view<int32>() | Sum();
			TestEqual("sum", ActualSum, 0);
		}
		{
			const FVector ActualSum = std::ranges::empty_view<FVector>() | Sum();
			TestEqual("sum", ActualSum, FVector::ZeroVector);
		}
		{
			const FString ActualSum = std::ranges::empty_view<FString>() | Sum();
			TestEqual("sum", ActualSum, FString{});
		}
	});

	It("single", [this]() {
		{
			const int32 Single[] = {123};
			const int32 ActualSum = Single | Sum();
			TestEqual("sum", ActualSum, Single[0]);
		}
		{
			const FVector Single[] = {FVector(1, 2, 3)};
			const FVector ActualSum = Single | Sum();
			TestEqual("sum", ActualSum, Single[0]);
		}
		{
			const FString Single[] = {TEXT("123")};
			const FString ActualSum = Single | Sum();
			TestEqual("sum", ActualSum, Single[0]);
		}
	});

	It("many", [this]() {
		{
			const int32 SomeValues[] = {1, 2, 3, 4, 5};
			const int32 ExpectedSum = std::accumulate(SomeValues, SomeValues + UE_ARRAY_COUNT(SomeValues), 0);
			const int32 ActualSum = SomeValues | Sum();
			TestEqual("sum", ActualSum, ExpectedSum);
		}
		{
			const FVector SomeValues[] = {FVector{1}, FVector{2}, FVector{3}, FVector{4}, FVector{5}};
			const FVector ExpectedSum = std::accumulate(SomeValues, SomeValues + UE_ARRAY_COUNT(SomeValues), FVector::ZeroVector);
			const FVector ActualSum = SomeValues | Sum();
			TestEqual("sum", ActualSum, ExpectedSum);
		}
		{
			const FString SomeValues[] = {TEXT("1"), TEXT("2"), TEXT("3"), TEXT("4"), TEXT("5")};
			const FString ExpectedSum = std::accumulate(SomeValues, SomeValues + UE_ARRAY_COUNT(SomeValues), FString{});
			const FString ActualSum = SomeValues | Sum();
			TestEqual("sum", ActualSum, ExpectedSum);
		}
	});

	It("many_transformed", [this]() {
		const FString SomeValues[] = {TEXT("1"), TEXT("22"), TEXT("333"), TEXT("4444"), TEXT("55555")};
		const auto AddLen = [](int32 Acc, const FString& Elem) {
			return Acc + Elem.Len();
		};
		const int32 ExpectedSum = std::accumulate(SomeValues, SomeValues + UE_ARRAY_COUNT(SomeValues), int32{}, AddLen);
		const int32 ActualSum = SomeValues | Sum(&FString::Len);
		TestEqual("sum", ActualSum, ExpectedSum);
	});
}

#endif // WITH_DEV_AUTOMATION_TESTS
