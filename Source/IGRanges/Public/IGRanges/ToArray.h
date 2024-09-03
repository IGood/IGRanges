// Copyright Ian Good

#pragma once

#include "Containers/Array.h"
#include <ranges>

#define _IGRP IG::Ranges::Private::

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

[[nodiscard]] constexpr auto ToArray()
{
	return std::ranges::_Range_closure<_IGRP ToArray_fn>{};
}

template <typename TransformT>
[[nodiscard]] constexpr auto ToArray(TransformT&& Trans)
{
	return std::views::transform(std::forward<TransformT>(Trans))
		 | IG::Ranges::ToArray();
}

} // namespace IG::Ranges

#undef _IGRP
