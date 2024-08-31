// Copyright Ian Good

#pragma once

#include "Templates/SharedPointer.h"
#include <functional>
#include <ranges>

namespace IG::Ranges
{
namespace Private
{
struct FirstOrDefaultToken
{
};

inline constexpr auto AlwaysTrue = [](auto&&) {
	return true;
};

template <typename RangeType, class _Pr>
[[nodiscard]] auto FirstOrDefault(RangeType&& Range, _Pr&& _Pred)
{
	using T = std::ranges::range_value_t<RangeType>;

	static_assert(!TIsTSharedRef_V<T>, "`FirstOrDefault` cannot operate on ranges of `TSharedRef`.");

	for (auto&& X : Range)
	{
		if (std::invoke(_Pred, X))
		{
			return X;
		}
	}

	return T{};
}

} // namespace Private

template <typename RangeType>
[[nodiscard]] auto FirstOrDefault(RangeType&& Range)
{
	return Private::FirstOrDefault(std::forward<RangeType>(Range), Private::AlwaysTrue);
}

template <typename RangeType, class _Pr>
[[nodiscard]] auto FirstOrDefault(RangeType&& Range, _Pr&& _Pred)
{
	return Private::FirstOrDefault(std::forward<RangeType>(Range), std::forward<_Pr>(_Pred));
}

template <typename RangeType, class _Rx, class _Ty>
[[nodiscard]] auto FirstOrDefault(RangeType&& Range, _Rx _Ty::*_Pm)
{
	return Private::FirstOrDefault(std::forward<RangeType>(Range), std::mem_fn(_Pm));
}

[[nodiscard]] inline constexpr Private::FirstOrDefaultToken FirstOrDefault()
{
	return {};
}

} // namespace IG::Ranges

template <typename RangeType>
[[nodiscard]] constexpr auto operator|(RangeType&& Range, IG::Ranges::Private::FirstOrDefaultToken)
{
	return IG::Ranges::Private::FirstOrDefault(std::forward<RangeType>(Range), IG::Ranges::Private::AlwaysTrue);
}
