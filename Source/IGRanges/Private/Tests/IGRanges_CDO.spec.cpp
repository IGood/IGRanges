// Copyright Ian Good

#include "IGRanges/Select.h"
#include "IGRanges/Selectors/CDO.h"
#include "IGRangesInternal.h"
#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

BEGIN_DEFINE_SPEC(FIGRangesCDOSpec, "IG.Ranges.CDO", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

/**
 * Given a range of pointer-like elements, tests that `CDO` behaves the same as `GetDefaultObject`.
 */
template <class ExpectedCDOType, typename RangeType>
bool TestPointers(const RangeType& Range)
{
	using namespace IG::Ranges;

	TArray<ExpectedCDOType> ExpectedPointers;
	for (auto&& X : Range)
	{
		ExpectedPointers.Emplace(
			(X != nullptr)
				? static_cast<ExpectedCDOType>(X->GetDefaultObject())
				: nullptr);
	}

	int32 i = -1;
	for (ExpectedCDOType X : Range | Select(Selectors::CDO))
	{
		++i;
		UTEST_TRUE_EXPR(ExpectedPointers.IsValidIndex(i));
		UTEST_EQUAL("casted element", X, ExpectedPointers[i]);
	}

	UTEST_EQUAL("count", i + 1, ExpectedPointers.Num());

	return true;
}

/**
 * Gets a few UClasses and puts them into an array of type `PointerType[]`.
 * `PointerType` is a raw pointer or a smart pointer type (e.g. UClass*, TObjectPtr, TWeakObjectPtr).
 * This array of pointer-like things is then tested with `CDO`.
 */
template <typename PointerType, typename ExpectedCDOType>
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

	TestPointers<ExpectedCDOType>(SomePointers);
}

END_DEFINE_SPEC(FIGRangesCDOSpec)

void FIGRangesCDOSpec::Define()
{
	It("yields_CDOs_from_classes (raw pointer)", [this]() {
		TestPointers<const UClass*, UObject*>();
	});

	It("yields_CDOs_from_classes (TObjectPtr)", [this]() {
		TestPointers<TObjectPtr<const UClass>, UObject*>();
	});

	It("yields_CDOs_from_classes (TWeakObjectPtr)", [this]() {
		TestPointers<TWeakObjectPtr<const UClass>, UObject*>();
	});

	It("yields_CDOs_from_classes (TSubclassOf)", [this]() {
		TestPointers<TSubclassOf<const UStruct>, const UStruct*>();
	});

	It("yields_CDOs_from_classes (TSoftClassPtr)", [this]() {
		TestPointers<TSoftClassPtr<const UStruct>, const UStruct*>();
	});
}

#endif // WITH_DEV_AUTOMATION_TESTS
