// Copyright Ian Good

#include "IGRanges/CPOs.h"
#include "IGRanges/Where.h"
#include "IGRangesInternal.h"
#include "Misc/AutomationTest.h"
#include <functional>

#if WITH_DEV_AUTOMATION_TESTS

BEGIN_DEFINE_SPEC(FIGRangesWhereSpec, "IG.Ranges.Where", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

constexpr struct FMyNumber
{
	constexpr FMyNumber(int32 InN)
		: N(InN)
		, bIsEven(InN % 2 == 0)
	{
	}

	int32 N;

	bool bIsEven;

	constexpr bool IsEven() const { return N % 2 == 0; }
};

static bool StaticIsEven(const FMyNumber& X)
{
	return X.N % 2 == 0;
};

/**
 * Tests that a `Where` or `WhereNot` filter behaves the same as using a predicate directly.
 */
template <typename PredicateT, typename WhereFilterT>
bool TestCallable(PredicateT&& Predicate, WhereFilterT&& WhereFilter)
{
	using namespace IG::Ranges;

	static constexpr FMyNumber SomeNumbers[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	static constexpr int32 NumSomeNumbers = UE_ARRAY_COUNT(SomeNumbers);

	TArray<int32> ExpectedValues;
	for (auto&& X : SomeNumbers)
	{
		if (std::invoke(Predicate, X))
		{
			ExpectedValues.Emplace(X.N);
		}
	}

	int i = -1;
	for (auto&& X : SomeNumbers | std::forward<WhereFilterT>(WhereFilter))
	{
		++i;
		UTEST_TRUE_EXPR(ExpectedValues.IsValidIndex(i));
		UTEST_EQUAL("filtered element", X.N, ExpectedValues[i]);
	}

	UTEST_EQUAL("count", i + 1, ExpectedValues.Num());

	return true;
}

END_DEFINE_SPEC(FIGRangesWhereSpec)

void FIGRangesWhereSpec::Define()
{
	using namespace IG::Ranges;

	// `Where` accepts function pointers as a predicate.
	It("function_pointer", [this]() {
		TestCallable(&StaticIsEven, Where(&StaticIsEven));
	});

	// `Where` accepts function objects as a predicate.
	It("function_object", [this]() {
		const auto IsEven = [](auto&& x) {
			return x.N % 2 == 0;
		};

		TestCallable(IsEven, Where(IsEven));
	});

	// `Where` accepts member field pointers as a predicate.
	It("member_field_pointer", [this]() {
		TestCallable(std::mem_fn(&FMyNumber::bIsEven), Where(&FMyNumber::bIsEven));
	});

	// `Where` accepts member function pointers as a predicate.
	It("member_function_pointer", [this]() {
		TestCallable(std::mem_fn(&FMyNumber::IsEven), Where(&FMyNumber::IsEven));
	});

	// `WhereNot` accepts function pointers as a predicate.
	It("function_pointer", [this]() {
		TestCallable(std::not_fn(&StaticIsEven), WhereNot(&StaticIsEven));
	});

	// `WhereNot` accepts function objects as a predicate.
	It("function_object (not)", [this]() {
		const auto IsEven = [](auto&& x) {
			return x.N % 2 == 0;
		};

		TestCallable(std::not_fn(IsEven), WhereNot(IsEven));
	});

	// `Where` accepts member field pointers as a predicate.
	It("member_field_pointer (not)", [this]() {
		TestCallable(std::not_fn(std::mem_fn(&FMyNumber::bIsEven)), WhereNot(&FMyNumber::bIsEven));
	});

	// `Where` accepts member function pointers as a predicate.
	It("member_function_pointer (not)", [this]() {
		TestCallable(std::not_fn(std::mem_fn(&FMyNumber::IsEven)), WhereNot(&FMyNumber::IsEven));
	});
}

#endif // WITH_DEV_AUTOMATION_TESTS
