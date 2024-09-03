// Copyright Ian Good

#pragma once

#include "Templates/Casts.h"
#include <ranges>

#define _IGRP IG::Ranges::Private::

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

[[nodiscard]] inline constexpr auto NonNull()
{
	return std::views::filter([](auto* x) { return x != nullptr; });
}

[[nodiscard]] inline constexpr auto NonNullRef()
{
	return _IGRP NonNull() | std::views::transform([](auto* x) -> decltype(*x)& { return *x; });
}

} // namespace Private

template <class T>
[[nodiscard]] constexpr auto OfType()
{
	return _IGRP Cast<T>() | _IGRP NonNull();
}

template <class T>
[[nodiscard]] constexpr auto OfTypeRef()
{
	return _IGRP Cast<T>() | _IGRP NonNullRef();
}

template <class T>
[[nodiscard]] constexpr auto OfTypeExact()
{
	return _IGRP CastExact<T>() | _IGRP NonNull();
}

template <class T>
[[nodiscard]] constexpr auto OfTypeExactRef()
{
	return _IGRP CastExact<T>() | _IGRP NonNullRef();
}

} // namespace IG::Ranges

#undef _IGRP
