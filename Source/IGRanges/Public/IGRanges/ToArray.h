// Copyright Ian Good

#pragma once

#include "Containers/Array.h"
#include <ranges>

namespace IG::Ranges
{
namespace Private
{
struct ToArrayToken
{
};

template <typename RangeType>
[[nodiscard]] auto MakeArray(const RangeType& Range)
{
	using T = std::ranges::range_value_t<RangeType>;
	TArray<T> Array;

	if constexpr (std::ranges::sized_range<RangeType>)
	{
		Array.Reserve(std::ranges::distance(Range));
	}

	return Array;
}

} // namespace Private

template <typename RangeType>
[[nodiscard]] auto ToArray(RangeType&& Range)
{
	auto Array = Private::MakeArray(Range);
	for (auto&& X : Range)
	{
		Array.Emplace(X);
	}

	return Array;
}

template <typename RangeType, typename ProjectionType>
[[nodiscard]] auto ToArray(RangeType&& Range, ProjectionType&& Proj)
{
	auto Array = Private::MakeArray(Range);
	for (auto&& X : Range)
	{
		Array.Emplace(std::invoke(Proj, X));
	}

	return Array;
}

[[nodiscard]] inline constexpr Private::ToArrayToken ToArray()
{
	return {};
}

} // namespace IG::Ranges

template <typename RangeType>
[[nodiscard]] auto operator|(RangeType&& Range, IG::Ranges::Private::ToArrayToken)
{
	return IG::Ranges::ToArray(std::forward<RangeType>(Range));
}
