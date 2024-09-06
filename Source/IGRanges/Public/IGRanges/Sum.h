// Copyright Ian Good

#pragma once

#include <ranges>
#include <type_traits>

#define _IGRP IG::Ranges::Private::

namespace IG::Ranges
{
namespace Private
{
struct Sum_fn
{
	template <typename RangeType>
	[[nodiscard]] constexpr auto operator()(RangeType&& Range) const
	{
		using T = std::ranges::range_value_t<RangeType>;

		auto It = std::begin(Range);
		const auto& End = std::end(Range);

		// If the range is empty, then return a default-initialized value.
		if (It == End)
		{
			// For non-fundamental types (bool, int, etc.), use `EForceInit` because that seems desirable for things like `FVector`.
			if constexpr (!std::is_fundamental_v<T> && std::is_constructible_v<T, EForceInit>)
			{
				return T(EForceInit::ForceInitToZero);
			}
			else
			{
				return T{};
			}
		}

		T Result = *It;

		while (++It != End)
		{
			Result = std::move(Result) + *It;
		}

		return Result;
	}
};

} // namespace Private

[[nodiscard]] inline constexpr auto Sum()
{
	return std::ranges::_Range_closure<_IGRP Sum_fn>{};
}

template <typename TransformT>
[[nodiscard]] constexpr auto Sum(TransformT&& Trans)
{
	return std::views::transform(std::forward<TransformT>(Trans))
		 | IG::Ranges::Sum();
}

} // namespace IG::Ranges

#undef _IGRP
