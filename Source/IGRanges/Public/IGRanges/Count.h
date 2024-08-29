// Copyright Ian Good

#pragma once

#include <ranges>

namespace IG::Ranges
{
namespace Private
{
struct CountToken
{
};

} // namespace Private

[[nodiscard]] inline constexpr Private::CountToken Count()
{
	return {};
}

} // namespace IG::Ranges

template <typename RangeType>
[[nodiscard]] constexpr int32 operator|(RangeType&& Range, IG::Ranges::Private::CountToken)
{
	return static_cast<int32>(std::ranges::distance(std::forward<RangeType>(Range)));
}
