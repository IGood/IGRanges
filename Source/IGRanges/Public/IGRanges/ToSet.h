// Copyright Ian Good

#pragma once

#include "Containers/Set.h"
#include <ranges>

namespace IG::Ranges
{
namespace Private
{
struct ToSetToken
{
};

template <typename RangeType>
[[nodiscard]] auto MakeSet(const RangeType& Range)
{
	using T = std::ranges::range_value_t<RangeType>;
	TSet<T> Set;

	if constexpr (std::ranges::sized_range<RangeType>)
	{
		Set.Reserve(std::ranges::distance(Range));
	}

	return Set;
}

} // namespace Private

template <typename RangeType>
[[nodiscard]] auto ToSet(RangeType&& Range)
{
	auto Set = Private::MakeSet(Range);
	for (auto&& X : Range)
	{
		Set.Emplace(X);
	}

	return Set;
}

template <typename RangeType, typename ProjectionType>
[[nodiscard]] auto ToSet(RangeType&& Range, ProjectionType&& Proj)
{
	auto Set = Private::MakeSet(Range);
	for (auto&& X : Range)
	{
		Set.Emplace(std::invoke(Proj, X));
	}

	return Set;
}

[[nodiscard]] inline constexpr Private::ToSetToken ToSet()
{
	return {};
}

} // namespace IG::Ranges

template <typename RangeType>
[[nodiscard]] auto operator|(RangeType&& Range, IG::Ranges::Private::ToSetToken)
{
	return IG::Ranges::ToSet(std::forward<RangeType>(Range));
}
