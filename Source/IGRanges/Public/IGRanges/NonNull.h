// Copyright Ian Good

#pragma once

#include "Templates/SharedPointer.h"
#include <ranges>

#include "IGRanges/Impl/Prologue.inl"

namespace IG::Ranges
{
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

} // namespace IG::Ranges

#include "IGRanges/Impl/Epilogue.inl"
