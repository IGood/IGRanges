// Copyright Ian Good

#include "IGRanges/NonNull.h"
#include "IGRangesInternal.h"
#include "Misc/AutomationTest.h"
#include <ranges>

#if WITH_DEV_AUTOMATION_TESTS

BEGIN_DEFINE_SPEC(FIGRangesNonNullSpec, "IG.Ranges.NonNull", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

/**
 * Given a range of pointer-like elements, tests that `NonNull` behaves the same as a null-check.
 */
template <typename RangeType>
bool TestPointers(const RangeType& Range)
{
	using namespace IG::Ranges;
	using T = std::ranges::range_value_t<RangeType>;

	TArray<T> ExpectedPointers;
	for (auto&& X : Range)
	{
		if (X != nullptr)
		{
			ExpectedPointers.Emplace(X);
		}
	}

	int32 i = -1;
	for (auto&& X : Range | NonNull())
	{
		++i;
		UTEST_TRUE_EXPR(ExpectedPointers.IsValidIndex(i));
		UTEST_EQUAL("non-null element", X, ExpectedPointers[i]);
	}

	return true;
}

/**
 * Given a range of pointer-like elements, tests that `NonNullRef` behaves the same as a null-check.
 */
template <typename RangeType>
bool TestRefs(const RangeType& Range)
{
	using namespace IG::Ranges;

	using T = std::ranges::range_value_t<RangeType>;

	TArray<T> ExpectedPointers;
	for (auto&& X : Range)
	{
		if (X != nullptr)
		{
			ExpectedPointers.Emplace(X);
		}
	}

	int32 i = -1;
	for (auto& X : Range | NonNullRef())
	{
		++i;
		UTEST_TRUE_EXPR(ExpectedPointers.IsValidIndex(i));

		// Use `TestSame` (not `TestEqual`) because these are supposed to be references to the same object.
		if constexpr (TIsTWeakPtr_V<T>) // Support for `TWeakPtr`
		{
			UTEST_SAME("non-null element", X, *ExpectedPointers[i].Pin().Get());
		}
		else
		{
			UTEST_SAME("non-null element", X, *ExpectedPointers[i]);
		}
	}

	return true;
}

END_DEFINE_SPEC(FIGRangesNonNullSpec)

void FIGRangesNonNullSpec::Define()
{
	// `NonNull` yields only non-null pointers.
	It("removes_null_pointers", [this]() {
		int32 A = 1;
		int32 B = 2;
		int32 C = 3;
		int32 D = 4;
		const int32* SomePointers[] = {nullptr, &A, &B, &C, &D, nullptr, nullptr, &D, &A, &D};

		TestPointers(SomePointers);
	});

	// `NonNull` yields only non-null shared pointers.
	It("removes_null_shared_pointers", [this]() {
		TSharedPtr<int32> A = MakeShared<int32>(1);
		TSharedPtr<int32> B = MakeShared<int32>(2);
		TSharedPtr<int32> C = MakeShared<int32>(3);
		TSharedPtr<int32> D = MakeShared<int32>(4);
		const TSharedPtr<int32> SomePointers[] = {nullptr, A, B, C, D, nullptr, nullptr, D, A, D};

		TestPointers(SomePointers);
	});

	// `NonNull` yields only non-null weak pointers.
	It("removes_null_weak_pointers", [this]() {
		TSharedPtr<int32> A = MakeShared<int32>(1);
		TSharedPtr<int32> B = MakeShared<int32>(2);
		TSharedPtr<int32> C = MakeShared<int32>(3);
		TSharedPtr<int32> D = MakeShared<int32>(4);
		const TSharedPtr<int32> SomePointers[] = {nullptr, A, B, C, D, nullptr, nullptr, D, A, D};

		auto SomeWeakPointers = SomePointers | std::views::transform([](auto&& x) { return x.ToWeakPtr(); });
		TestPointers(SomeWeakPointers);
	});

	// `NonNullRef` yields references to non-null elements' data.
	It("removes_null_pointers_yields_refs", [this]() {
		int32 A = 1;
		int32 B = 2;
		int32 C = 3;
		int32 D = 4;
		const int32* SomePointers[] = {nullptr, &A, &B, &C, &D, nullptr, nullptr, &D, &A, &D};

		TestRefs(SomePointers);
	});

	// `NonNullRef` yields references to shared pointers' data.
	It("removes_null_shared_pointers_yields_refs", [this]() {
		TSharedPtr<int32> A = MakeShared<int32>(1);
		TSharedPtr<int32> B = MakeShared<int32>(2);
		TSharedPtr<int32> C = MakeShared<int32>(3);
		TSharedPtr<int32> D = MakeShared<int32>(4);
		const TSharedPtr<int32> SomePointers[] = {nullptr, A, B, C, D, nullptr, nullptr, D, A, D};

		TestRefs(SomePointers);
	});

	// `NonNullRef` yields references to weak pointers' data.
	It("removes_null_weak_pointers_yields_refs", [this]() {
		TSharedPtr<int32> A = MakeShared<int32>(1);
		TSharedPtr<int32> B = MakeShared<int32>(2);
		TSharedPtr<int32> C = MakeShared<int32>(3);
		TSharedPtr<int32> D = MakeShared<int32>(4);
		const TSharedPtr<int32> SomePointers[] = {nullptr, A, B, C, D, nullptr, nullptr, D, A, D};
		auto SomeWeakPointers = SomePointers | std::views::transform([](auto&& x) { return x.ToWeakPtr(); });

		TestRefs(SomeWeakPointers);
	});
}

#endif // WITH_DEV_AUTOMATION_TESTS
