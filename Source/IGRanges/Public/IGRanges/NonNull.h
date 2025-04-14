// Copyright Ian Good

#pragma once

#include "Templates/SharedPointer.h"
#include <ranges>

#include "IGRanges/Impl/Prologue.inl"

namespace IG::Ranges
{
namespace Private
{
template<bool bValidate>
struct NonNullRef_fn
{
	template <typename RangeType>
	[[nodiscard]] constexpr auto operator()(RangeType&& Range) const
	{
		using T = std::ranges::range_value_t<RangeType>;

		if constexpr (TIsTWeakPtr_V<T>) // Support for `TWeakPtr`
		{
			if constexpr (bValidate)
			{
				return std::forward<RangeType>(Range)
					 | std::views::filter([](auto&& x) { return x.IsValid(); })
					 | std::views::transform([](auto&& x) -> decltype(auto) { return std::ref(*x.Pin().Get()); });
			}
			else
			{
				return std::forward<RangeType>(Range)
					 | std::views::transform([](auto&& x) -> decltype(auto) { return std::ref(*x.Pin().Get()); });
			}
		}
		else
		{
			if constexpr (bValidate)
			{
				return std::forward<RangeType>(Range)
					 | std::views::filter([](auto&& x) { return x != nullptr; })
					 | std::views::transform([](auto&& x) -> decltype(auto) { return std::ref(*x); });
			}
			else
			{
				return std::forward<RangeType>(Range)
					 | std::views::transform([](auto&& x) -> decltype(auto) { return std::ref(*x); });
			}
		}
	}
};

} // namespace Private

/**
 * Filters a sequence of pointer-like values, removing null elements.
 * Works on raw pointers (e.g. `UFoo*`) and smart pointers (e.g. `TObjectPtr<UFoo>`, `TWeakPointer<FBar>`, etc.) and
 * anything that can be null-checked.
 */
[[nodiscard]] inline constexpr auto NonNull()
{
	return std::views::filter([]<typename T>(T&& x) {
		if constexpr (TIsTWeakPtr_V<T>) // Support for `TWeakPtr`
		{
			return x.IsValid();
		}
		else
		{
			return x != nullptr;
		}
	});
}

/**
 * Same as `NonNull` but yields references to values instead of pointers.
 */
[[nodiscard]] inline constexpr auto NonNullRef()
{
	return std::ranges::_Range_closure<_IGRP NonNullRef_fn<true>>{};
}

/**
 * Same as `NonNullRef` but skips validating pointer
 */
[[nodiscard]] inline constexpr auto NonNullRefChecked()
{
	return std::ranges::_Range_closure<_IGRP NonNullRef_fn<false>>{};
}

/**
 * Same as `NonNullRefChecked`
 */
[[nodiscard]] inline constexpr auto ToRef()
{
	return NonNullRefChecked();
}

} // namespace IG::Ranges

#include "IGRanges/Impl/Epilogue.inl"
