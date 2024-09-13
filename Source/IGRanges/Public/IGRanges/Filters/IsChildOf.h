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
[[nodiscard]] const UStruct* AsStructPointer(ClassType&& ClassLike)
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

namespace Filters
{
/**
 * Filter that tests whether a UClass-like value is a child of the specified class.
 * Safe to use with null inputs.
 *
 * This filter is expected to be used with types like...
 * - UClass*
 * - TObjectPtr<UClass>
 * - TWeakObjectPtr<UClass>
 * - TSubclassOf<T>
 * - TSoftClassPtr<T>
 *
 * @usage SomeClasses | Where(Filters::IsChildOf(FooClass))
 */
template <typename ClassType>
[[nodiscard]] auto IsChildOf(ClassType&& ClassLike)
{
	return [SomeBase = _IGRP AsStructPointer(ClassLike)](auto&& x) {
		const UStruct* Struct = _IGRP AsStructPointer(x);
		return Struct != nullptr && Struct->IsChildOf(SomeBase);
	};
}

/**
 * Filter that tests whether a UClass-like value is a child of the specified class.
 * Safe to use with null inputs.
 *
 * This filter is expected to be used with types like...
 * - UClass*
 * - TObjectPtr<UClass>
 * - TWeakObjectPtr<UClass>
 * - TSubclassOf<T>
 * - TSoftClassPtr<T>
 *
 * @usage SomeClasses | Where(Filters::IsChildOf<UFoo>())
 */
template <typename T>
[[nodiscard]] auto IsChildOf()
{
	return _IGR Filters::IsChildOf(T::StaticClass());
}

} // namespace Filters

} // namespace IG::Ranges

#include "IGRanges/Impl/Epilogue.inl"
