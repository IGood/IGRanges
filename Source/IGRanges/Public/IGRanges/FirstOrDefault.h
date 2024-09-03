// Copyright Ian Good

#pragma once

#include "Templates/SharedPointer.h"
#include <functional>
#include <ranges>

namespace IG::Ranges
{
namespace Private
{
struct FirstOrDefault_fn
{
	template <typename RangeType, class _Pr>
	[[nodiscard]] constexpr auto operator()(RangeType&& Range, _Pr&& _Pred) const
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
};

inline constexpr auto AlwaysTrue = [](auto&&) {
	return true;
};

} // namespace Private

template <class _Pr>
[[nodiscard]] constexpr auto FirstOrDefault(_Pr&& _Pred)
{
	return std::ranges::_Range_closure<IG::Ranges::Private::FirstOrDefault_fn, std::decay_t<_Pr>>{std::forward<_Pr>(_Pred)};
}

[[nodiscard]] constexpr auto FirstOrDefault()
{
	return FirstOrDefault(IG::Ranges::Private::AlwaysTrue);
}

} // namespace IG::Ranges
