// Copyright Ian Good

#include "IGRanges/Filters/IsChildOf.h"
#include "IGRanges/Where.h"
#include "IGRangesInternal.h"
#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

BEGIN_DEFINE_SPEC(FIGRangesIsChildOfSpec, "IG.Ranges.IsChildOf", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

/**
 * Given a range of pointer-like elements, tests that `IG::Ranges::Filters::IsChildOf` behaves the same as `UStruct::IsChildOf`.
 */
template <typename RangeType>
bool TestPointers(const RangeType& Range)
{
	using namespace IG::Ranges;

	TArray<const UStruct*> ExpectedPointers;
	for (auto&& X : Range)
	{
		if (X != nullptr && X->IsChildOf<UField>())
		{
			ExpectedPointers.Emplace(IG::Ranges::Private::AsStructPointer(X));
		}
	}

	int32 i = -1;
	for (auto&& X : Range | Where(Filters::IsChildOf<UField>()))
	{
		++i;
		UTEST_TRUE_EXPR(ExpectedPointers.IsValidIndex(i));

		const UStruct* Actual = IG::Ranges::Private::AsStructPointer(X);
		UTEST_EQUAL("casted element", Actual, ExpectedPointers[i]);
	}

	UTEST_EQUAL("count", i + 1, ExpectedPointers.Num());

	return true;
}

/**
 * Gets a few UClasses and puts them into an array of type `PointerType[]`.
 * `PointerType` is a raw pointer or a smart pointer type (e.g. UClass*, TObjectPtr, TWeakObjectPtr).
 * This array of pointer-like things is then tested with `IsChildOf`.
 */
template <typename PointerType>
void TestPointers()
{
	UClass* O = UObject::StaticClass();
	UClass* F = UField::StaticClass();  // is a UObject
	UClass* E = UEnum::StaticClass();   // is a UField
	UClass* S = UStruct::StaticClass(); // is a UField
	UClass* C = UClass::StaticClass();  // is a UStruct
	PointerType SomePointers[] = {nullptr, O, F, E, S, C, nullptr, nullptr, C, O, F, F, E, E};

	// For smart pointer types like `TSubclassOf<T>` & `TSoftClassPtr<T>`, Unreal doesn't protect us from creating bad
	// instances like this where the UClasses involved are not correct:
	// /* ohno */ TSubclassOf<UEnum> = UStruct::StaticClass();
	// But Unreal does protect us from using bad instances by returning Null from `Get`, so we'll weed those out here
	// so our tests don't break.
	if constexpr (IG::Ranges::Private::HasGet<PointerType>)
	{
		for (PointerType& P : SomePointers)
		{
			if (P.Get() == nullptr)
			{
				P = nullptr;
			}
		}
	}

	TestPointers(SomePointers);
}

END_DEFINE_SPEC(FIGRangesIsChildOfSpec)

void FIGRangesIsChildOfSpec::Define()
{
	It("yields_classes_of_specified_type (raw pointer)", [this]() {
		TestPointers<const UClass*>();
	});

	It("yields_classes_of_specified_type (TObjectPtr)", [this]() {
		TestPointers<TObjectPtr<const UClass>>();
	});

	It("yields_classes_of_specified_type (TWeakObjectPtr)", [this]() {
		TestPointers<TWeakObjectPtr<const UClass>>();
	});

	It("yields_classes_of_specified_type (TSubclassOf)", [this]() {
		TestPointers<TSubclassOf<const UStruct>>();
	});

	It("yields_classes_of_specified_type (TSoftClassPtr)", [this]() {
		TestPointers<TSoftClassPtr<const UStruct>>();
	});
}

#endif // WITH_DEV_AUTOMATION_TESTS
