// Copyright Ian Good

#pragma once

#include <functional>
#include <ranges>

namespace IG::Ranges
{
template <class _Fn>
[[nodiscard]] constexpr auto Select(_Fn&& _Fun)
{
	return std::views::transform(std::forward<_Fn>(_Fun));
}

template <class _Rx, class _Ty>
[[nodiscard]] constexpr auto Select(_Rx _Ty::*_Pm)
{
	return std::views::transform(std::mem_fn(_Pm));
}

} // namespace IG::Ranges
