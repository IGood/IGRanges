// Copyright Ian Good

#pragma once

#include "Templates/Casts.h"
#include <ranges>

namespace IG::Ranges
{
/**
 * Casts the values of a range of UObjects to a specified type using UE's `Cast<T>` function.
 * Safe to accept null values; may yield null results.
 *
 * @see `OfType<T>`
 */
template <class T>
[[nodiscard]] constexpr auto Cast()
{
	return std::views::transform([](auto&& x) { return ::Cast<T>(x); });
}

/**
 * Casts the values of a range of UObjects to a specified type using UE's `ExactCast<T>` function.
 * Safe to accept null values; may yield null results.
 *
 * @see `OfExactType<T>`
 */
template <class T>
[[nodiscard]] constexpr auto ExactCast()
{
	return std::views::transform([](auto&& x) { return ::ExactCast<T>(x); });
}

/**
 * Casts the values of a range of UObjects to a specified type using UE's `CastChecked<T>` function.
 * Not safe to accept null values; never yields null results.
 */
template <class T>
[[nodiscard]] constexpr auto CastChecked()
{
	return std::views::transform([](auto&& x) { return ::CastChecked<T>(x); });
}

/**
 * Same as `CastChecked<T>` but yields references to values instead of pointers.
 */
template <class T>
[[nodiscard]] constexpr auto CastCheckedRef()
{
	return std::views::transform([](auto&& x) -> T& { return *::CastChecked<T>(x); });
}

/**
 * Similar to `CastChecked<T>` (no parameters) but null-check behavior is parameterized.
 */
template <class T>
[[nodiscard]] constexpr auto CastChecked(ECastCheckedType::Type CheckType)
{
	return std::views::transform([](auto&& x) { return ::CastChecked<T>(x, CheckType); });
}

// Note: There is no `CastCheckedRef(ECastCheckedType::Type)` because it allows for null-in / null-out.

} // namespace IG::Ranges
