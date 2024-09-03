// Copyright Ian Good

#include "IGRanges/CPOs.h"
#include "IGRanges/Select.h"
#include "IGRangesInternal.h"
#include "Misc/AutomationTest.h"
#include <functional>

#if WITH_DEV_AUTOMATION_TESTS

BEGIN_DEFINE_SPEC(FIGRangesSelectSpec, "IG.Ranges.Select", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

static constexpr double SquareD(const int32 X)
{
	return static_cast<double>(X) * static_cast<double>(X);
}

constexpr struct FMyNumber
{
	constexpr FMyNumber(int32 InN)
		: N(InN)
		, Squared(SquareD(InN))
	{
	}

	const int32 N = 0;

	const double Squared = 0.0;

	constexpr double Square() const { return SquareD(N); }
};

static double StaticSquare(const FMyNumber& X)
{
	return SquareD(X.N);
};

/**
 * Tests that a `Select` tranformation behaves the same as using a transformation directly.
 */
template <typename TransformT>
bool TestCallable(TransformT&& Transform)
{
	using namespace IG::Ranges;

	static constexpr FMyNumber SomeNumbers[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	static constexpr int32 NumSomeNumbers = UE_ARRAY_COUNT(SomeNumbers);

	TArray<double> ExpectedValues;
	ExpectedValues.Reserve(NumSomeNumbers);
	for (auto&& X : SomeNumbers)
	{
		ExpectedValues.Emplace(std::invoke(Transform, X));
	}

	int i = -1;
	for (auto&& X : SomeNumbers | Select(Transform))
	{
		++i;
		UTEST_TRUE_EXPR(ExpectedValues.IsValidIndex(i));
		UTEST_EQUAL("transformed element", X, ExpectedValues[i]);
	}

	UTEST_EQUAL("count", i + 1, ExpectedValues.Num());

	return true;
}

END_DEFINE_SPEC(FIGRangesSelectSpec)

void FIGRangesSelectSpec::Define()
{
	// `Select` accepts function pointers as a transformation.
	It("function_pointer", [this]() {
		TestCallable(&StaticSquare);
	});

	// `Select` accepts function objects as a transformation.
	It("function_object", [this]() {
		const auto Square = [](auto&& x) {
			return SquareD(x.N);
		};

		TestCallable(Square);
	});

	// `Select` accepts member field pointers as a transformation.
	It("member_field_pointer", [this]() {
		TestCallable(&FMyNumber::Squared);
	});

	// `Select` accepts member function pointers as a transformation.
	It("member_function_pointer", [this]() {
		TestCallable(&FMyNumber::Square);
	});
}

#endif // WITH_DEV_AUTOMATION_TESTS
