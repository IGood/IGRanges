// Copyright Ian Good

#pragma once

#include "Templates/Casts.h"
#include <ranges>

namespace IG::Ranges
{
template <class T>
[[nodiscard]] constexpr auto Cast()
{
	return std::views::transform([](auto&& x) { return ::Cast<T>(x); });
}

template <class T>
[[nodiscard]] constexpr auto CastExact()
{
	return std::views::transform([](auto&& x) { return ::CastExact<T>(x); });
}

template <class T>
[[nodiscard]] constexpr auto CastChecked()
{
	return std::views::transform([](auto&& x) { return ::CastChecked<T>(x); });
}

template <class T>
[[nodiscard]] constexpr auto CastChecked(ECastCheckedType::Type CheckType)
{
	return std::views::transform([](auto&& x) { return ::CastChecked<T>(x, CheckType); });
}

} // namespace IG::Ranges
