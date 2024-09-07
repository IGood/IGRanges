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
			TestEqual("sum int32", ActualSum, 0);
		}
		{
			const FVector ActualSum = std::ranges::empty_view<FVector>() | Sum();
			TestEqual("sum FVector", ActualSum, FVector::ZeroVector);
		}
		{
			const FQuat ActualSum = std::ranges::empty_view<FQuat>() | Sum();
			TestEqual("sum FQuat", ActualSum, FQuat::Identity);
		}
		{
			const FString ActualSum = std::ranges::empty_view<FString>() | Sum();
			TestEqual("sum FString", ActualSum, FString{});
		}
	});

	It("single", [this]() {
		{
			const int32 Single[] = {123};
			const int32 ActualSum = Single | Sum();
			TestEqual("sum int32", ActualSum, Single[0]);
		}
		{
			const FVector Single[] = {FVector(1, 2, 3)};
			const FVector ActualSum = Single | Sum();
			TestEqual("sum FVector", ActualSum, Single[0]);
		}
		{
			const FQuat Single[] = {FQuat(1, 2, 3, 4)};
			const FQuat ActualSum = Single | Sum();
			TestEqual("sum FQuat", ActualSum, Single[0]);
		}
		{
			const FString Single[] = {TEXT("123")};
			const FString ActualSum = Single | Sum();
			TestEqual("sum FString", ActualSum, Single[0]);
		}
	});

	It("many", [this]() {
		{
			const int32 SomeValues[] = {1, 2, 3, 4, 5};
			const int32 ExpectedSum = std::accumulate(SomeValues, SomeValues + UE_ARRAY_COUNT(SomeValues), 0);
			const int32 ActualSum = SomeValues | Sum();
			TestEqual("sum int32", ActualSum, ExpectedSum);
		}
		{
			const FVector SomeValues[] = {FVector(1.0), FVector(2.0), FVector(3.0), FVector(4.0), FVector(5.0)};
			const FVector ExpectedSum = std::accumulate(SomeValues, SomeValues + UE_ARRAY_COUNT(SomeValues), FVector::ZeroVector);
			const FVector ActualSum = SomeValues | Sum();
			TestEqual("sum FVector", ActualSum, ExpectedSum);
		}
		{
			const FQuat SomeValues[] = {FQuat(1.0), FQuat(2.0), FQuat(3.0), FQuat(4.0), FQuat(5.0)};
			const FQuat ExpectedSum = std::accumulate(SomeValues, SomeValues + UE_ARRAY_COUNT(SomeValues), FQuat(EForceInit::ForceInitToZero));
			const FQuat ActualSum = SomeValues | Sum();
		}
		{
			const FString SomeValues[] = {TEXT("1"), TEXT("2"), TEXT("3"), TEXT("4"), TEXT("5")};
			const FString ExpectedSum = std::accumulate(SomeValues, SomeValues + UE_ARRAY_COUNT(SomeValues), FString{});
			const FString ActualSum = SomeValues | Sum();
			TestEqual("sum FString", ActualSum, ExpectedSum);
		}
	});

	It("many_transformed", [this]() {
		const FString SomeValues[] = {TEXT("1"), TEXT("22"), TEXT("333"), TEXT("4444"), TEXT("55555")};
		const auto AddLen = [](int32 Acc, const FString& Elem) {
			return Acc + Elem.Len();
		};
		const int32 ExpectedSum = std::accumulate(SomeValues, SomeValues + UE_ARRAY_COUNT(SomeValues), int32{}, AddLen);
		const int32 ActualSum = SomeValues | Sum(&FString::Len);
		TestEqual("sum int32", ActualSum, ExpectedSum);
	});
}

#endif // WITH_DEV_AUTOMATION_TESTS
