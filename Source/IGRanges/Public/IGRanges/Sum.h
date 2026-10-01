// Copyright Ian Good

#pragma once

#include "IGRanges/Impl/Common.h"
#include <ranges>

#include "IGRanges/Impl/Prologue.inl"

namespace IG::Ranges
{
namespace Private
{
struct Sum_fn
{
	template <typename RangeType>
	[[nodiscard]] constexpr auto operator()(RangeType&& Range) const
	{
		using T = std::ranges::range_value_t<RangeType>;

		auto It = std::begin(Range);
		const auto& End = std::end(Range);

		// If the range is empty, then return a default-initialized value.
		if (It == End)
		{
			return _IGRP Construct<T>();
		}

		T Result = *It;

		while (++It != End)
		{
			Result = std::move(Result) + *It;
		}

		return Result;
	}
};

} // namespace Private

/**
 * Computes the sum of a sequence of values by applying `operator+`.
 * Empty ranges return a default-initialized value.
 *
 * @usage
 * int32 Total = SomeNumbers | Sum();
 * FVector Offset = SomeVectors | Sum();
 * FString Concatenated = SomeStrings | Sum();
 */
[[nodiscard]] inline constexpr auto Sum()
{
	return std::ranges::_Range_closure<_IGRP Sum_fn>{};
}

/**
 * Same as `Sum` (no parameters) but first applies a projection to elements.
 * Equivalent to `Select(proj) | Sum()`.
 *
 * @usage
 * float TotalWeight = SomeStructs | Sum([](const FBar& B) { return B.Weight; });
 * float TotalWeight = SomeStructs | Sum(&FBar::Weight);
 */
template <typename TransformT>
[[nodiscard]] constexpr auto Sum(TransformT&& Trans)
{
	return std::views::transform(std::forward<TransformT>(Trans))
		 | _IGR Sum();
}

} // namespace IG::Ranges

#include "IGRanges/Impl/Epilogue.inl"
