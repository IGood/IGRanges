// Copyright Ian Good

#pragma once

#include "IGRanges/NonNull.h"
#include "IGRanges/Selectors/Find.h"
#include <ranges>

#include "IGRanges/Impl/Prologue.inl"

namespace IG::Ranges
{
template <typename ContainerType>
[[nodiscard]] constexpr auto Lookup(ContainerType& Container)
{
	return std::views::transform(_IGR Selectors::Find(Container)) | NonNull();
}

template <typename ContainerType>
[[nodiscard]] constexpr auto LookupRef(ContainerType& Container)
{
	return std::views::transform(_IGR Selectors::Find(Container)) | NonNullRef();
}

} // namespace IG::Ranges

#include "IGRanges/Impl/Epilogue.inl"
