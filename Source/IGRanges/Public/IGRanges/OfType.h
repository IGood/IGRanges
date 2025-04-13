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

/**
 * Filters the values of a range of UObjects based on a specified type.
 * Performs a cast to the specified type and then filters null elements.
 * Equivalent to `Cast<T>() | NonNull()`.
 *
 * All the "Of Type" range adapters are safe to accept null values and never yield null results.
 *
 * @usage
 * SomeActors | OfType<UMyActor>()
 * SomeComponents | OfType<UMeshComponent>()
 */
template <class T>
[[nodiscard]] constexpr auto OfType()
{
	return _IGR Cast<T>() | _IGRP NonNull();
}

/**
 * Same as `OfType<T>` but yields references to values instead of pointers.
 */
template <class T>
[[nodiscard]] constexpr auto OfTypeRef()
{
	return _IGR Cast<T>() | _IGRP NonNull() | _IGRP Dereference();
}

/**
 * Similar to `OfType<T>` but uses `ExactCast<T>`.
 */
template <class T>
[[nodiscard]] constexpr auto OfExactType()
{
	return _IGR ExactCast<T>() | _IGRP NonNull();
}

/**
 * Same as `OfExactType<T>` but yields references to values instead of pointers.
 */
template <class T>
[[nodiscard]] constexpr auto OfExactTypeRef()
{
	return _IGR ExactCast<T>() | _IGRP NonNull() | _IGRP Dereference();
}

/**
 * Similar to `OfType<T>` (checks types) but does not perform a cast.
 */
[[nodiscard]] inline constexpr auto OfType(const UClass* Class)
{
	return std::views::filter([Class](auto&& x) { return _IGRP IsA(x, Class); });
}

/**
 * Similar to `OfType` (one parameter) but yields references to values instead of pointers.
 */
[[nodiscard]] inline constexpr auto OfTypeRef(const UClass* Class)
{
	return _IGR OfType(Class) | _IGRP Dereference();
}

} // namespace IG::Ranges

#include "IGRanges/Impl/Epilogue.inl"
