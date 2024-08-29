// Copyright Ian Good

#pragma once

#include <functional>
#include <ranges>

namespace IG::Ranges
{
template <class _Pr>
[[nodiscard]] constexpr auto Where(_Pr&& _Pred)
{
	return std::views::filter(std::forward<_Pr>(_Pred));
}

template <class _Rx, class _Ty>
[[nodiscard]] constexpr auto Where(_Rx _Ty::*_Pm)
{
	return std::views::filter(std::mem_fn(_Pm));
}

template <class _Pr>
[[nodiscard]] constexpr auto WhereNot(_Pr&& _Pred)
{
	return std::views::filter(std::not_fn(std::forward<_Pr>(_Pred)));
}

template <class _Rx, class _Ty>
[[nodiscard]] constexpr auto WhereNot(_Rx _Ty::*_Pm)
{
	return std::views::filter(std::not_fn(std::mem_fn(_Pm)));
}

} // namespace IG::Ranges
