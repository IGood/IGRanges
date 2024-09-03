// Copyright Ian Good

#pragma once

#include "Containers/Set.h"
#include "IGRanges/Select.h"
#include <ranges>

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

[[nodiscard]] constexpr auto ToSet()
{
	return std::ranges::_Range_closure<IG::Ranges::Private::ToSet_fn>{};
}

template <typename TransformT>
[[nodiscard]] constexpr auto ToSet(TransformT&& Trans)
{
	// Use `Select` to take advantage of overloads for "callable" or "member pointer".
	return IG::Ranges::Select(std::forward<TransformT>(Trans)) | IG::Ranges::ToSet();
}

} // namespace IG::Ranges
