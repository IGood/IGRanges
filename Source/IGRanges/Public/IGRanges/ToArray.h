// Copyright Ian Good

#pragma once

#include "Containers/Array.h"
#include <ranges>

#include "IGRanges/Impl/Prologue.inl"

namespace IG::Ranges
{
namespace Private
{
struct ToArray_fn
{
	template <typename RangeType>
	[[nodiscard]] constexpr auto operator()(RangeType&& Range) const
	{
		using T = std::ranges::range_value_t<RangeType>;
		TArray<T> Array;

		if constexpr (std::ranges::sized_range<RangeType>)
		{
			Array.Reserve(std::ranges::distance(Range));
		}

		for (auto&& X : Range)
		{
			Array.Emplace(X);
		}

		return Array;
	}
};

} // namespace Private

/**
 * Creates a `TArray` from a range.
 *
 * @usage
 * TArray<int32> SquaredNumbers = SomeNumbers | Select([](int32 N) { return N * N; }) | ToArray();
 */
[[nodiscard]] inline constexpr auto ToArray()
{
	return std::ranges::_Range_closure<_IGRP ToArray_fn>{};
}

/**
 * Same as `ToArray` (no parameters) but first applies a projection to elements.
 * Equivalent to `Select(proj) | ToArray()`.
 *
 * @usage
 * TArray<int32> SquaredNumbers = SomeNumbers | ToArray([](int32 N) { return N * N; });
 * TArray<FString> Names = SomeObjects | ToArray([](auto&& Obj) { return GetNameSafe(Obj); });
 */
template <typename TransformT>
[[nodiscard]] constexpr auto ToArray(TransformT&& Trans)
{
	return std::views::transform(std::forward<TransformT>(Trans))
		 | _IGR ToArray();
}

} // namespace IG::Ranges

#include "IGRanges/Impl/Epilogue.inl"
