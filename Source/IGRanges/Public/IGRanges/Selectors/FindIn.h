// Copyright Ian Good

#pragma once

#include "IGRanges/Impl/Prologue.inl"

namespace IG::Ranges
{
namespace Selectors
{
template <typename ContainerType>
[[nodiscard]] constexpr auto FindIn(ContainerType& Container)
{
	return [&Container](auto&& Key) {
		// Lookup method is different for arrays & maps.
		const auto DoLookup = [&] {
			if constexpr (TIsTArray_V<ContainerType>)
			{
				return Container.FindByKey(Key);
			}
			else
			{
				return Container.Find(Key);
			}
		};

		// If keys can be null-checked, then do that.
		if constexpr (requires { Key != nullptr; })
		{
			return (Key != nullptr) ? DoLookup() : nullptr;
		}
		else
		{
			return DoLookup();
		}
	};
}

} // namespace Selectors

} // namespace IG::Ranges

#include "IGRanges/Impl/Epilogue.inl"
