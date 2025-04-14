// Copyright Ian Good

#include "IGRanges/Cast.h"
#include "IGRangesInternal.h"
#include "IGRanges/OfType.h"
#include "IGRanges/ToArray.h"
#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

namespace
{

enum class ECastType
{
	Cast,
	ExactCast,
	CastChecked,
	CastCheckedWithArg,
};

template <class T>
[[nodiscard]] constexpr auto CastCheckedNoArg()
{
	return IG::Ranges::CastChecked<T>();
}

template <class T>
[[nodiscard]] constexpr auto CastCheckedNullAllowed()
{
	return IG::Ranges::CastChecked<T>(ECastCheckedType::NullAllowed);
}

}
BEGIN_DEFINE_SPEC(FIGRangesCastSpec, "IG.Ranges.Cast", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

/**
 * Given a range of pointer-like elements, tests that `IG::Ranges::Cast` behaves the same as `::Cast`.
 */
template <class T, ECastType CastType, auto RangeCastFunctor, typename RangeType>
bool TestPointers(const RangeType& Range)
{
	TArray<const T*> ExpectedPointers;
	for (auto&& X : Range)
	{
		if constexpr (CastType == ECastType::Cast)
		{
			ExpectedPointers.Emplace(Cast<T>(X));
		}
		else if constexpr (CastType == ECastType::ExactCast)
		{
			ExpectedPointers.Emplace(ExactCast<T>(X));
		}
		else if constexpr (CastType == ECastType::CastChecked)
		{
			ExpectedPointers.Emplace(CastChecked<T>(X));
		}
		else if constexpr (CastType == ECastType::CastCheckedWithArg)
		{
			ExpectedPointers.Emplace(CastChecked<T>(X, ECastCheckedType::NullAllowed));
		}
	}

	int32 i = -1;
	for (auto* X : Range | RangeCastFunctor())
	{
		++i;
		UTEST_TRUE_EXPR(ExpectedPointers.IsValidIndex(i));
		UTEST_EQUAL("casted element", X, ExpectedPointers[i]);
	}

	return true;
}

/**
 * Gets a few CDOs and puts them into an array of type `PointerType[]`.
 * `PointerType` is a raw pointer or a smart pointer type (e.g. UObject*, TObjectPtr, TWeakObjectPtr).
 * This array of pointer-like things is then tested with `Cast`.
 */
template <typename PointerType>
void TestPointers()
{
	PointerType O = GetDefault<UObject>();
	PointerType F = GetDefault<UField>();  // is a UObject
	PointerType E = GetDefault<UEnum>();   // is a UField
	PointerType S = GetDefault<UStruct>(); // is a UField
	PointerType C = GetDefault<UClass>();  // is a UStruct
	PointerType SomePointers[] = {nullptr, O, F, E, S, C, nullptr, nullptr, C, O, F, F, E, E};

	using ExpectedPointerType = const UField;

	TestPointers<ExpectedPointerType, ECastType::Cast, &IG::Ranges::Cast<ExpectedPointerType>>(SomePointers);
	TestPointers<ExpectedPointerType, ECastType::ExactCast, &IG::Ranges::ExactCast<ExpectedPointerType>>(SomePointers);

	using namespace IG::Ranges;
	auto CheckedPointers = SomePointers
		| OfType<ExpectedPointerType>()
		| ToArray();

	TestPointers<ExpectedPointerType, ECastType::CastChecked, &CastCheckedNoArg<ExpectedPointerType>>(CheckedPointers);
	TestPointers<ExpectedPointerType, ECastType::CastCheckedWithArg, &CastCheckedNullAllowed<ExpectedPointerType>>(CheckedPointers);
}

END_DEFINE_SPEC(FIGRangesCastSpec)

void FIGRangesCastSpec::Define()
{
	// `Cast` yields pointers to objects of the specified type or null.
	It("yields_objects_of_specified_type_or_null (raw pointer)", [this]() {
		TestPointers<const UObject*>();
	});

	// `Cast` yields pointers to objects of the specified type or null.
	It("yields_objects_of_specified_type_or_null (TObjectPtr)", [this]() {
		TestPointers<TObjectPtr<const UObject>>();
	});

	// `Cast` yields pointers to objects of the specified type or null.
	It("yields_objects_of_specified_type_or_null (TWeakObjectPtr)", [this]() {
		TestPointers<TWeakObjectPtr<const UObject>>();
	});
}

#endif // WITH_DEV_AUTOMATION_TESTS
