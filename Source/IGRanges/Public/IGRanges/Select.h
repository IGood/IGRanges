// Copyright Ian Good

#pragma once

#include "IGRanges/NonNull.h"
#include <ranges>

#include "IGRanges/Impl/Prologue.inl"

namespace IG::Ranges
{
template <class _Fn>
[[nodiscard]] constexpr auto Select(_Fn&& _Fun)
{
	return std::views::transform(std::forward<_Fn>(_Fun));
}

template <class _Fn>
[[nodiscard]] constexpr auto SelectNonNull(_Fn&& _Fun)
{
	return _IGR Select(std::forward<_Fn>(_Fun)) | _IGR NonNull();
}

} // namespace IG::Ranges

#include "IGRanges/Impl/Epilogue.inl"
