// Copyright Ian Good

#pragma once

#include "Containers/Set.h"
#include <ranges>

#define _IGRP IG::Ranges::Private::

namespace IG::Ranges
{
namespace Private
{
struct ToSet_fn
{
	template <typename RangeType>
	[[nodiscard]] constexpr auto operator()(RangeType&& Range) const
	{
		using T = std::ranges::range_value_t<RangeType>;
		TSet<T> Set;

		if constexpr (std::ranges::sized_range<RangeType>)
		{
			Set.Reserve(std::ranges::distance(Range));
		}

		for (auto&& X : Range)
		{
			Set.Emplace(X);
		}

		return Set;
	}
};

} // namespace Private

[[nodiscard]] inline constexpr auto ToSet()
{
	return std::ranges::_Range_closure<_IGRP ToSet_fn>{};
}

template <typename TransformT>
[[nodiscard]] constexpr auto ToSet(TransformT&& Trans)
{
	return std::views::transform(std::forward<TransformT>(Trans))
		 | IG::Ranges::ToSet();
}

} // namespace IG::Ranges

#undef _IGRP
