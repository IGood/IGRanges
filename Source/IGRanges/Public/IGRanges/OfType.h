// Copyright Ian Good

#pragma once

#include "IGRanges/Cast.h"
#include "UObject/SoftObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"
#include <ranges>

#include "IGRanges/Impl/Prologue.inl"

namespace IG::Ranges
{
namespace Private
{
[[nodiscard]] inline bool IsA(const UObject* Obj, const UClass* Class)
{
	return Obj != nullptr && Obj->IsA(Class);
}

template <typename T>
[[nodiscard]] bool IsA(const TObjectPtr<T> Obj, const UClass* Class)
{
	return Obj.IsA(Class);
}

template <typename T>
[[nodiscard]] bool IsA(const TWeakObjectPtr<T>& WeakObj, const UClass* Class)
{
	return _IGRP IsA(WeakObj.Get(), Class);
}

template <typename T>
[[nodiscard]] bool IsA(const TSoftObjectPtr<T>& SoftObj, const UClass* Class)
{
	return _IGRP IsA(SoftObj.Get(), Class);
}

[[nodiscard]] inline constexpr auto Dereference()
{
	return std::views::transform([](auto&& x) -> decltype(*x)& { return *x; });
}

[[nodiscard]] inline constexpr auto NonNull()
{
	return std::views::filter([](const void* x) { return x != nullptr; });
}

} // namespace Private

template <class T>
[[nodiscard]] constexpr auto OfType()
{
	return _IGR Cast<T>() | _IGRP NonNull();
}

template <class T>
[[nodiscard]] constexpr auto OfTypeRef()
{
	return _IGR Cast<T>() | _IGRP NonNull() | _IGRP Dereference();
}

template <class T>
[[nodiscard]] constexpr auto OfTypeExact()
{
	return _IGR CastExact<T>() | _IGRP NonNull();
}

template <class T>
[[nodiscard]] constexpr auto OfTypeExactRef()
{
	return _IGR CastExact<T>() | _IGRP NonNull() | _IGRP Dereference();
}

[[nodiscard]] inline constexpr auto OfType(const UClass* Class)
{
	return std::views::filter([Class](auto&& x) { return _IGRP IsA(x, Class); });
}

[[nodiscard]] inline constexpr auto OfTypeRef(const UClass* Class)
{
	return _IGR OfType(Class) | _IGRP Dereference();
}

} // namespace IG::Ranges

#include "IGRanges/Impl/Epilogue.inl"
