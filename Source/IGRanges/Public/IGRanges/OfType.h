// Copyright Ian Good

#pragma once

#include "Templates/Casts.h"
#include <ranges>

namespace IG::Ranges
{
namespace Private
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

[[nodiscard]] constexpr auto NonNull()
{
	return std::views::filter([](auto* x) { return x != nullptr; });
}

[[nodiscard]] constexpr auto NonNullRef()
{
	return NonNull() | std::views::transform([](auto* x) -> decltype(*x)& { return *x; });
}

} // namespace Private

template <class T>
[[nodiscard]] constexpr auto OfType()
{
	return Private::Cast<T>() | Private::NonNull();
}

template <class T>
[[nodiscard]] constexpr auto OfTypeRef()
{
	return Private::Cast<T>() | Private::NonNullRef();
}

template <class T>
[[nodiscard]] constexpr auto OfTypeExact()
{
	return Private::CastExact<T>() | Private::NonNull();
}

template <class T>
[[nodiscard]] constexpr auto OfTypeExactRef()
{
	return Private::CastExact<T>() | Private::NonNullRef();
}

} // namespace IG::Ranges
