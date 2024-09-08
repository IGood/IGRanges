// Copyright Ian Good

#pragma once

#include "IGRanges/Cast.h"
#include <ranges>

#include "IGRanges/Impl/Prologue.inl"

namespace IG::Ranges
{
namespace Private
{
[[nodiscard]] inline constexpr auto Deref()
{
	return std::views::transform([](auto&& x) -> decltype(*x)& { return *x; });
}

[[nodiscard]] inline constexpr auto NonNull()
{
	return std::views::filter([](auto* x) { return x != nullptr; });
}

[[nodiscard]] inline constexpr auto NonNullRef()
{
	return _IGRP NonNull() | _IGRP Deref();
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

template <typename OtherClassType>
[[nodiscard]] constexpr auto OfType(OtherClassType Class)
{
	return std::views::filter([Class](auto&& x) { return x != nullptr && x->IsA(Class); });
}

template <typename OtherClassType>
[[nodiscard]] constexpr auto OfTypeRef(OtherClassType Class)
{
	return _IGR OfType(Class) | _IGRP Deref();
}

} // namespace IG::Ranges

#include "IGRanges/Impl/Epilogue.inl"
