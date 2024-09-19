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
[[nodiscard]] constexpr auto ExactCast()
{
	return std::views::transform([](auto&& x) { return ::ExactCast<T>(x); });
}

template <class T>
[[nodiscard]] constexpr auto CastChecked()
{
	return std::views::transform([](auto&& x) { return ::CastChecked<T>(x); });
}

template <class T>
[[nodiscard]] constexpr auto CastCheckedRef()
{
	return std::views::transform([](auto&& x) -> T& { return *::CastChecked<T>(x); });
}

template <class T>
[[nodiscard]] constexpr auto CastChecked(ECastCheckedType::Type CheckType)
{
	return std::views::transform([](auto&& x) { return ::CastChecked<T>(x, CheckType); });
}

// Note: There is no `CastCheckedRef(ECastCheckedType::Type)` because it allows for null-in / null-out.

} // namespace IG::Ranges
