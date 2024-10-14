// Copyright Ian Good

#pragma once

#include <functional>
#include <ranges>

#include "IGRanges/Impl/Prologue.inl"

namespace IG::Ranges
{
/**
 * Filters a sequence of values based on a predicate.
 *
 * Alias for `std::views::filter`:
 * A range adaptor that represents view of an underlying sequence without the elements that fail to satisfy a predicate.
 *
 * @usage
 * SomeNumbers | Where([](int32 N) { return N > 0; })
 * SomeStructs | Where([](const FBar& B) { return B.IsGood(); })
 * SomeStructs | Where(&FBar::IsGood)
 * SomeObjects | Where([](const UFoo* F) { return F != nullptr && F->IsGood(); })
 */
template <class _Pr>
[[nodiscard]] constexpr auto Where(_Pr&& _Pred)
{
	return std::views::filter(std::forward<_Pr>(_Pred));
}

/**
 * Same as `Where` but intended for pointer-like elements that can be null-checked.
 * Elements are null-checked before invoking the predicate.
 * Similar to `NonNull() | Where(filter)`.
 *
 * @usage
 * SomeObjects | SafeWhere([](const UFoo* F) { return F->IsGood(); })
 * SomeObjects | SafeWhere(&UFoo::IsGood)
 */
template <class _Pr>
[[nodiscard]] constexpr auto SafeWhere(_Pr _Pred)
{
	return std::views::filter([Pred = std::move(_Pred)]<typename T>(T&& x) {
		return x != nullptr && std::invoke(Pred, std::forward<T>(x));
	});
}

/**
 * Same as `Where` but the result of the predicate is negated.
 */
template <class _Pr>
[[nodiscard]] constexpr auto WhereNot(_Pr&& _Pred)
{
	return std::views::filter(std::not_fn(std::forward<_Pr>(_Pred)));
}

/**
 * Same as `SafeWhere` but the result of the predicate is negated.
 */
template <class _Pr>
[[nodiscard]] constexpr auto SafeWhereNot(_Pr&& _Pred)
{
	return _IGR SafeWhere(std::not_fn(std::forward<_Pr>(_Pred)));
}

} // namespace IG::Ranges

#include "IGRanges/Impl/Epilogue.inl"
