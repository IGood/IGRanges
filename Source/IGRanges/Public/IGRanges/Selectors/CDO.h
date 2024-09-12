// Copyright Ian Good

#pragma once

#include "IGRanges/Impl/Common.h"
#include "Templates/SubclassOf.h"
#include "UObject/Class.h"
#include "UObject/SoftObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"

#include "IGRanges/Impl/Prologue.inl"

namespace IG::Ranges
{
namespace Private
{
template <typename T>
concept HasGetDefaultObject = requires(T t) {
	t.GetDefaultObject();
};

template <typename T>
concept IsSoftPtr = requires(T t) {
	t.LoadSynchronous();
};

/**
 * Gets the input UClass-like value as a `UClass*`.
 * This function is expected to be used with types like...
 * - UClass*
 * - TObjectPtr<UClass>
 * - TWeakObjectPtr<UClass>
 * - TSubclassOf<T>
 * - TSoftClassPtr<T>
 * All of the smart pointer types have a `Get` method that we want to use to get the underlying value.
 */
template <typename ClassType>
[[nodiscard]] const UClass* GetClassPointer(ClassType&& ClassLike)
{
	if constexpr (_IGRP HasGet<ClassType>)
	{
		return ClassLike.Get();
	}
	else
	{
		return ClassLike;
	}
}

} // namespace Private

namespace Selectors
{
inline constexpr auto CDO = []<typename ClassType>(ClassType&& ClassLike) {
	if constexpr (_IGRP HasGetDefaultObject<ClassType>)
	{
		// `TSubclassOf<UFoo>::GetDefaultObject` knows the class type & does the null-checks & casting for us.
		return ClassLike.GetDefaultObject();
	}
	else if constexpr (_IGRP IsSoftPtr<ClassType>)
	{
		// A `TSoftClassPtr<UFoo>` knows the class type, but we need to perform the cast (returns `UFoo*`).
		const UClass* Class = ClassLike.Get();
		return (Class != nullptr) ? static_cast<ClassType::ElementType*>(Class->GetDefaultObject()) : nullptr;
	}
	else
	{
		// Other UClass-like types don't know the class type, so we cannot cast (just returns `UObject*`).
		const UClass* Class = _IGRP GetClassPointer(ClassLike);
		return (Class != nullptr) ? Class->GetDefaultObject() : nullptr;
	}
};

} // namespace Selectors

} // namespace IG::Ranges

#include "IGRanges/Impl/Epilogue.inl"
