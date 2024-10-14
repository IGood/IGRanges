// Copyright Ian Good

#pragma once

#include "IGRanges/NonNull.h"
#include <ranges>

#include "IGRanges/Impl/Prologue.inl"

namespace IG::Ranges
{
namespace Private
{
template <typename ContainerType>
[[nodiscard]] constexpr auto LookupIn(ContainerType& Container)
{
	return [&Container](auto&& Key) {
		// Lookup method is different for arrays & maps.
		const auto DoLookup = [&] {
			if constexpr (TIsTArray_V<ContainerType>)
			{
				// Elements need to be equatable to keys.
				// e.g. For elements of type `FBar` & keys of type `FGuid`, a function like one of these must exist:
				//	bool FBar::operator==(const FGuid& Id) const { ... }
				//	bool operator==(const FBar& Bar, const FGuid& Id) { ... }
				static_assert(
					requires { Container[0] == Key; },
					"Lookups into a `TArray` uses `FindByKey`, which requires `op==` for the element type & the key type.");
				return Container.FindByKey(Key);
			}
			else
			{
				return Container.Find(Key);
			}
		};

		// If keys can be null-checked, then do that because it's *probably* not meaningful.
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

} // namespace Private

/**
 * Performs lookups into the contain using the input range as keys and then filters null elements (i.e. failed lookups).
 * If a key is null, then the lookup is *not* performed (this is an optimization that assumes null keys are not meaningful).
 * Results are pointers to elements in the container.
 *
 * @usage
 * TArray<FGuid> SomeGuids;
 * TMap<FGuid, TObjectPtr<AActor>> MapOfIdsToActors;
 * TArray<FBar> SomeStructs; // pretend `bool FBar::operator==(const FGuid& Id) const` exists
 * for (TObjectPtr<AActor>* A : SomeGuids | Lookup(MapOfIdsToActors)) ...
 * for (FBar* Bar : SomeGuids | Lookup(SomeStructs)) ...
 */
template <typename ContainerType>
[[nodiscard]] constexpr auto Lookup(ContainerType& Container)
{
	return std::views::transform(_IGRP LookupIn(Container)) | _IGR NonNull();
}

/**
 * Same as `Lookup` but yields references to elements instead of pointers.
 *
 * @usage
 * TArray<FGuid> SomeGuids;
 * TMap<FGuid, TObjectPtr<AActor>> MapOfIdsToActors;
 * TArray<FBar> SomeStructs; // pretend `bool FBar::operator==(const FGuid& Id) const` exists
 * for (AActor* A : SomeGuids | LookupRef(MapOfIdsToActors)) ...
 * for (FBar& Bar : SomeGuids | LookupRef(SomeStructs)) ...
 */
template <typename ContainerType>
[[nodiscard]] constexpr auto LookupRef(ContainerType& Container)
{
	return std::views::transform(_IGRP LookupIn(Container)) | _IGR NonNullRef();
}

} // namespace IG::Ranges

#include "IGRanges/Impl/Epilogue.inl"
