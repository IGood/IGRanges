// Copyright Ian Good

#pragma once

#include "HAL/Platform.h" // `int32`
#include <ranges>

#include "IGRanges/Impl/Prologue.inl"

namespace IG::Ranges
{
namespace Private
{
struct Count_fn
{
	template <typename RangeType>
	[[nodiscard]] constexpr auto operator()(RangeType&& Range) const
	{
		return static_cast<int32>(std::ranges::distance(std::forward<RangeType>(Range)));
	}
};

} // namespace Private

/**
 * Returns the number of elements in a range.
 *
 * @usage
 * int32 NumVulerableActors = SomeActors | Where(&AActor::CanBeDamaged) | Count();
 */
[[nodiscard]] inline constexpr auto Count()
{
	return std::ranges::_Range_closure<_IGRP Count_fn>{};
}

/**
 * Returns the number of elements in a range that satisfy a predicate.
 * Equivalent to `Where(pred) | Count()`.
 *
 * @usage
 * int32 NumVulerableActors = SomeActors | Count(&AActor::CanBeDamaged);
 */
template <class _Pr>
[[nodiscard]] constexpr auto Count(_Pr&& _Pred)
{
	return std::views::filter(std::forward<_Pr>(_Pred))
		 | _IGR Count();
}

} // namespace IG::Ranges

#include "IGRanges/Impl/Epilogue.inl"
