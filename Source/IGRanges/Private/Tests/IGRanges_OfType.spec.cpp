// Copyright Ian Good

#include "IGRanges/OfType.h"
#include "IGRangesInternal.h"
#include "Misc/AutomationTest.h"
#include "UObject/MetaData.h"
#include "UObject/Package.h"

#if WITH_DEV_AUTOMATION_TESTS

BEGIN_DEFINE_SPEC(FIGRangesOfTypeSpec, "IG.Ranges.OfType", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

/**
 * Given a range of pointer-like elements, tests that `OfType` behaves the same as `Cast`.
 */
template <class T, typename RangeType>
bool TestPointersOfType(const RangeType& Range)
{
	using namespace IG::Ranges;

	TArray<const T*> ExpectedPointers;
	for (auto&& X : Range)
	{
		if (auto* Casted = Cast<T>(X))
		{
			ExpectedPointers.Emplace(Casted);
		}
	}

	int32 i = -1;
	for (auto* X : Range | OfType<T>())
	{
		++i;
		UTEST_TRUE_EXPR(ExpectedPointers.IsValidIndex(i));
		UTEST_EQUAL("casted element", X, ExpectedPointers[i]);
	}

	UTEST_EQUAL("count", i + 1, ExpectedPointers.Num());

	return true;
}

/**
 * Gets a few CDOs and puts them into an array of type `PointerType[]`.
 * `PointerType` is a raw pointer or a smart pointer type (e.g. UObject*, TObjectPtr, TWeakObjectPtr).
 * This array of pointer-like things is then tested with `OfType`.
 */
template <typename PointerType>
void TestPointers()
{
	PointerType A = GetDefault<UObject>();
	PointerType B = GetDefault<UClass>();
	PointerType C = GetDefault<UPackage>();
	PointerType D = GetDefault<UMetaData>();
	PointerType SomePointers[] = {nullptr, A, B, C, D, nullptr, nullptr, D, A, D};

	TestPointersOfType<const UMetaData>(SomePointers);
}

/**
 * Given a range of pointer-like elements, tests that `OfTypeRef` behaves the same as `Cast`.
 */
template <class T, typename RangeType>
bool TestRefsOfType(const RangeType& Range)
{
	using namespace IG::Ranges;

	TArray<const T*> ExpectedPointers;
	for (auto&& X : Range)
	{
		if (auto* Casted = Cast<T>(X))
		{
			ExpectedPointers.Emplace(Casted);
		}
	}

	int32 i = -1;
	for (auto& X : Range | OfTypeRef<T>())
	{
		++i;
		UTEST_TRUE_EXPR(ExpectedPointers.IsValidIndex(i));

		// Use `TestSame` (not `TestEqual`) because these are supposed to be references to the same object.
		UTEST_SAME("casted element", X, *ExpectedPointers[i]);
	}

	UTEST_EQUAL("count", i + 1, ExpectedPointers.Num());

	return true;
}

/**
 * Same as `TestPointers` but tests `OfTypeRef`.
 */
template <typename PointerType>
void TestRefs()
{
	PointerType A = GetDefault<UObject>();
	PointerType B = GetDefault<UClass>();
	PointerType C = GetDefault<UPackage>();
	PointerType D = GetDefault<UMetaData>();
	PointerType SomePointers[] = {nullptr, A, B, C, D, nullptr, nullptr, D, A, D};

	TestRefsOfType<const UMetaData>(SomePointers);
}

/**
 * Given a range of pointer-like elements, tests that `OfType` behaves the same as `IsA`.
 */
template <typename RangeType>
bool TestPointersIsA(const RangeType& Range, const UClass* Class)
{
	using namespace IG::Ranges;

	TArray<const UObject*> ExpectedPointers;
	for (auto&& X : Range)
	{
		if (X != nullptr && X->IsA(Class))
		{
			ExpectedPointers.Emplace(&*X);
		}
	}

	int32 i = -1;
	for (auto&& X : Range | OfType(Class))
	{
		++i;
		UTEST_TRUE_EXPR(ExpectedPointers.IsValidIndex(i));
		UTEST_SAME("element", *X, *ExpectedPointers[i]);
	}

	UTEST_EQUAL("count", i + 1, ExpectedPointers.Num());

	return true;
}

/**
 * Gets a few CDOs and puts them into an array of type `PointerType[]`.
 * `PointerType` is a raw pointer or a smart pointer type (e.g. UObject*, TObjectPtr, TWeakObjectPtr).
 * This array of pointer-like things is then tested with `OfType`.
 */
template <typename PointerType>
void TestPointersIsA()
{
	PointerType A = GetDefault<UObject>();
	PointerType B = GetDefault<UClass>();
	PointerType C = GetDefault<UPackage>();
	PointerType D = GetDefault<UMetaData>();
	PointerType SomePointers[] = {nullptr, A, B, C, D, nullptr, nullptr, D, A, D};

	TestPointersIsA(SomePointers, UMetaData::StaticClass());
}

END_DEFINE_SPEC(FIGRangesOfTypeSpec)

void FIGRangesOfTypeSpec::Define()
{
	// `OfType` yields only pointers to objects of the specified type.
	It("yields_objects_of_specified_type (raw pointer)", [this]() {
		TestPointers<const UObject*>();
	});

	// `OfType` yields only pointers to objects of the specified type.
	It("yields_objects_of_specified_type (TObjectPtr)", [this]() {
		TestPointers<TObjectPtr<const UObject>>();
	});

	// `OfType` yields only pointers to objects of the specified type.
	It("yields_objects_of_specified_type (TWeakObjectPtr)", [this]() {
		TestPointers<TWeakObjectPtr<const UObject>>();
	});

	// `OfType` yields only references to objects of the specified type.
	It("yields_references_to_objects_of_specified_type (raw pointer)", [this]() {
		TestRefs<const UObject*>();
	});

	// `OfType` yields only references to objects of the specified type.
	It("yields_references_to_objects_of_specified_type (TObjectPtr)", [this]() {
		TestRefs<TObjectPtr<const UObject>>();
	});

	// `OfType` yields only references to objects of the specified type.
	It("yields_references_to_objects_of_specified_type (TWeakObjectPtr)", [this]() {
		TestRefs<TWeakObjectPtr<const UObject>>();
	});

	// `OfType` yields only pointers to objects of the specified class.
	It("yields_objects_of_specified_class (raw pointer)", [this]() {
		TestPointersIsA<const UObject*>();
	});

	// `OfType` yields only pointers to objects of the specified class.
	It("yields_objects_of_specified_class (TObjectPtr)", [this]() {
		TestPointersIsA<TObjectPtr<const UObject>>();
	});

	// `OfType` yields only pointers to objects of the specified class.
	It("yields_objects_of_specified_class (TWeakObjectPtr)", [this]() {
		TestPointersIsA<TWeakObjectPtr<const UObject>>();
	});
}

#endif // WITH_DEV_AUTOMATION_TESTS
