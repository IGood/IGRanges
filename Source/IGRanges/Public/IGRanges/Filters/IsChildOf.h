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
/**
 * Gets the input UClass-like value as a `UStruct*`.
 * This function is expected to be used with types like...
 * - UClass*
 * - TObjectPtr<UClass>
 * - TWeakObjectPtr<UClass>
 * - TSubclassOf<T>
 * - TSoftClassPtr<T>
 * All of the smart pointer types have a `Get` method that we want to use to get the underlying value.
 */
template <typename ClassType>
[[nodiscard]] const UStruct* GetStructPointer(ClassType&& Class)
{
	if constexpr (_IGRP HasGet<ClassType>)
	{
		return Class.Get();
	}
	else
	{
		return Class;
	}
}

} // namespace Private

namespace Filters
{
template <typename ClassType>
[[nodiscard]] auto IsChildOf(ClassType Class)
{
	return [SomeBase = _IGRP GetStructPointer(Class)](auto&& x) {
		const UStruct* Struct = _IGRP GetStructPointer(x);
		return Struct != nullptr && Struct->IsChildOf(SomeBase);
	};
}

template <typename T>
[[nodiscard]] auto IsChildOf()
{
	return _IGR Filters::IsChildOf(T::StaticClass());
}

} // namespace Filters

} // namespace IG::Ranges

#include "IGRanges/Impl/Epilogue.inl"
