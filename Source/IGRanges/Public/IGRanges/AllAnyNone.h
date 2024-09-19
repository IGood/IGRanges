// Copyright Ian Good

#pragma once

#include "IGRanges/Impl/Common.h"
#include <algorithm>
#include <ranges>

#include "IGRanges/Impl/Prologue.inl"

namespace IG::Ranges
{
namespace Private
{
enum class EAlgoChoice
{
	AllOf,
	AnyOf,
	NoneOf,
};

template <EAlgoChoice _Choice>
struct Algo_fn
{
	template <typename RangeType, class _Pr>
	[[nodiscard]] constexpr bool operator()(RangeType&& Range, _Pr _Pred) const
	{
		if constexpr (_Choice == EAlgoChoice::AllOf)
		{
			return std::all_of(std::begin(Range), std::end(Range), std::move(_Pred));
		}
		else if constexpr (_Choice == EAlgoChoice::AnyOf)
		{
			return std::any_of(std::begin(Range), std::end(Range), std::move(_Pred));
		}
		else if constexpr (_Choice == EAlgoChoice::NoneOf)
		{
			return std::none_of(std::begin(Range), std::end(Range), std::move(_Pred));
		}
	}
};

template <EAlgoChoice _Choice, class _Pr>
[[nodiscard]] constexpr auto ChooseAlgo(_Pr&& _Pred)
{
	return std::ranges::_Range_closure<Algo_fn<_Choice>, std::decay_t<_Pr>>{std::forward<_Pr>(_Pred)};
}

} // namespace Private

/**
 * Returns True if all elements in the range satisfy the predicate (or the range is empty); otherwise, False.
 * If no predicate is specified, then elements themselves are tested for "truthiness".
 *
 * Alias for `std::all_of`:
 * Checks if a unary predicate returns True for all elements in the range.
 *
 * @usage
 * bool bAllEven = SomeNumbers | All([](int32 N) { return N % 2 == 0; });
 * bool bAllGood = SomeStructs | All(&FBar::IsGood);
 */
template <class _Pr = std::identity>
[[nodiscard]] constexpr auto All(_Pr&& _Pred = {})
{
	return _IGRP ChooseAlgo<_IGRP EAlgoChoice::AllOf>(std::forward<_Pr>(_Pred));
}

/**
 * Returns True if any element in the range satisfies the predicate; otherwise, False.
 * If no predicate is specified, then returns whether there are any elements in the range at all.
 *
 * Alias for `std::any_of`:
 * Checks if a unary predicate returns True for at least one element in the range.
 *
 * @usage
 * bool bSomeGood = SomeStructs | Any(&FBar::IsGood);
 */
template <class _Pr = _IGRP AlwaysTrue>
[[nodiscard]] constexpr auto Any(_Pr&& _Pred = {})
{
	return _IGRP ChooseAlgo<_IGRP EAlgoChoice::AnyOf>(std::forward<_Pr>(_Pred));
}

/**
 * Returns True if no element in the range satisfies the predicate (or the range is empty); otherwise, False.
 * If no predicate is specified, then elements themselves are tested for "truthiness".
 *
 * Alias for `std::none_of`:
 * Checks if a unary predicate returns True for no elements in the range.
 *
 * @usage
 * bool bNoGood = SomeStructs | None(&FBar::IsGood);
 */
template <class _Pr = std::identity>
[[nodiscard]] constexpr auto None(_Pr&& _Pred = {})
{
	return _IGRP ChooseAlgo<_IGRP EAlgoChoice::NoneOf>(std::forward<_Pr>(_Pred));
}

} // namespace IG::Ranges

#include "IGRanges/Impl/Epilogue.inl"
