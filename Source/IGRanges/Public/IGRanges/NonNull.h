// Copyright Ian Good

#pragma once

#include "Templates/SharedPointer.h"
#include <ranges>

namespace IG::Ranges
{
namespace Private
{
struct NonNullRefToken
{
};

} // namespace Private

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

[[nodiscard]] inline constexpr Private::NonNullRefToken NonNullRef()
{
	return {};
}

} // namespace IG::Ranges

template <typename RangeType>
[[nodiscard]] constexpr auto operator|(RangeType&& Range, IG::Ranges::Private::NonNullRefToken)
{
	using T = std::ranges::range_value_t<RangeType>;

	if constexpr (TIsTWeakPtr_V<T>) // Support for `TWeakPtr`
	{
		return std::forward<RangeType>(Range)
			 | std::views::filter([](auto&& x) { return x.IsValid(); })
			 | std::views::transform([](auto&& x) -> T::ElementType& { return *x.Pin().Get(); });
	}
	else
	{
		return std::forward<RangeType>(Range)
			 | std::views::filter([](auto&& x) { return x != nullptr; })
			 | std::views::transform([](auto&& x) -> decltype(*x)& { return *x; });
	}
}
