// Copyright Ian Good

#include "IGRanges/Cast.h"
#include "IGRangesInternal.h"
#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

BEGIN_DEFINE_SPEC(FIGRangesCastSpec, "IG.Ranges.Cast", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

/**
 * Given a range of pointer-like elements, tests that `IG::Ranges::Cast` behaves the same as `::Cast`.
 */
template <class T, typename RangeType>
bool TestPointers(const RangeType& Range)
{
	TArray<const T*> ExpectedPointers;
	for (auto&& X : Range)
	{
		ExpectedPointers.Emplace(::Cast<T>(X));
	}

	int32 i = -1;
	for (auto* X : Range | IG::Ranges::Cast<T>())
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

	TestPointers<const UField>(SomePointers);
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
