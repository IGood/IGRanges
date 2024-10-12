// Copyright Ian Good

#pragma once

#include "IGRanges/Impl/Prologue.inl"

namespace IG::Ranges
{
namespace Selectors
{
template <typename ContainerType>
[[nodiscard]] constexpr auto Find(ContainerType& Container)
{
	return [&Container](auto&& Key) -> decltype(Container.Find({})) {
		// If keys can be null-checked, then do that.
		if constexpr (requires { Key == nullptr; })
		{
			if (Key == nullptr)
			{
				return nullptr;
			}
		}

		// If the container has `FindByKey` (like arrays), then use that; otherwise, use `Find` (like maps).
		if constexpr (requires { Container.FindByKey({}); })
		{
			return Container.FindByKey(Key);
		}
		else
		{
			return Container.Find(Key);
		}
	};
}

} // namespace Selectors

} // namespace IG::Ranges

#include "IGRanges/Impl/Epilogue.inl"
