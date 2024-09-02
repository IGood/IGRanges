// Copyright Ian Good

#pragma once

#include "Containers/Array.h"
#include "IGRanges/Select.h"
#include <ranges>

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
	return std::ranges::_Range_closure<IG::Ranges::Private::ToArray_fn>{};
}
template <class _Fn>
[[nodiscard]] constexpr auto ToArray(_Fn&& _Fun)
{
	// Use `Select` to take advantage of overloads for "callable" or "member pointer".
	return IG::Ranges::Select(std::forward<_Fn>(_Fun))
		 | std::ranges::_Range_closure<IG::Ranges::Private::ToArray_fn>{};
}

} // namespace IG::Ranges
