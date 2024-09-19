// Copyright Ian Good

#pragma once

#include "IGRanges/NonNull.h"
#include <ranges>

#include "IGRanges/Impl/Prologue.inl"

namespace IG::Ranges
{
/**
 * Projects each element of a sequence into a new form.
 *
 * Alias for `std::views::transform`:
 * A range adaptor that represents view of an underlying sequence after applying a transformation function to each element.
 *
 * @usage
 * SomeNumbers | Select([](int32 N) { return N * N; })
 * SomeStructs | Select([](const FBar& B) { return B.GetId(); })
 * SomeStructs | Select(&FBar::GetId)
 * SomeObjects | Select([](const UFoo* F) { return (F != nullptr) ? F->GetName() : TEXT("???"); })
 */
template <class _Fn>
[[nodiscard]] constexpr auto Select(_Fn&& _Fun)
{
	return std::views::transform(std::forward<_Fn>(_Fun));
}

/**
 * Same as `Select` but intended for projections that produce pointer-like results.
 * Projection results that are Null are filtered out.
 * Equivalent to `Select(proj) | NonNull()`.
 *
 * @usage:
 * SomeActors | SelectNonNull([](const AActor* A) {
 *     return (A != nullptr) ? A.GetComponentByClass<UMyComponent>() : nullptr;
 * })
 */
template <class _Fn>
[[nodiscard]] constexpr auto SelectNonNull(_Fn&& _Fun)
{
	return std::views::transform(std::forward<_Fn>(_Fun)) | _IGR NonNull();
}

} // namespace IG::Ranges

#include "IGRanges/Impl/Epilogue.inl"
