// Copyright Ian Good

#pragma once

#include "IGRanges/Cast.h"
#include <ranges>

#define _IGR IG::Ranges::
#define _IGRP IG::Ranges::Private::

namespace IG::Ranges
{
namespace Private
{
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
	return _IGR Cast<T>() | _IGRP NonNull();
}

template <class T>
[[nodiscard]] constexpr auto OfTypeRef()
{
	return _IGR Cast<T>() | _IGRP NonNullRef();
}

template <class T>
[[nodiscard]] constexpr auto OfTypeExact()
{
	return _IGR CastExact<T>() | _IGRP NonNull();
}

template <class T>
[[nodiscard]] constexpr auto OfTypeExactRef()
{
	return _IGR CastExact<T>() | _IGRP NonNullRef();
}

} // namespace IG::Ranges

#undef _IGR
#undef _IGRP
