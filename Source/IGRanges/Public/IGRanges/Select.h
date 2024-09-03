// Copyright Ian Good

#pragma once

#include <ranges>

namespace IG::Ranges
{
template <class _Fn>
[[nodiscard]] constexpr auto Select(_Fn&& _Fun)
{
	return std::views::transform(std::forward<_Fn>(_Fun));
}

} // namespace IG::Ranges
