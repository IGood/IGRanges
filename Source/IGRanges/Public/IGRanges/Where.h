// Copyright Ian Good

#pragma once

#include <functional>
#include <ranges>

#include "IGRanges/Impl/Prologue.inl"

namespace IG::Ranges
{
template <class _Pr>
[[nodiscard]] constexpr auto Where(_Pr&& _Pred)
{
	return std::views::filter(std::forward<_Pr>(_Pred));
}

template <class _Pr>
[[nodiscard]] constexpr auto SafeWhere(_Pr _Pred)
{
	return std::views::filter([Pred = std::move(_Pred)]<typename T>(T&& x) {
		return x != nullptr && std::invoke(Pred, std::forward<T>(x));
	});
}

template <class _Pr>
[[nodiscard]] constexpr auto WhereNot(_Pr&& _Pred)
{
	return std::views::filter(std::not_fn(std::forward<_Pr>(_Pred)));
}

template <class _Pr>
[[nodiscard]] constexpr auto SafeWhereNot(_Pr&& _Pred)
{
	return _IGR SafeWhere(std::not_fn(std::forward<_Pr>(_Pred)));
}

} // namespace IG::Ranges

#include "IGRanges/Impl/Epilogue.inl"
