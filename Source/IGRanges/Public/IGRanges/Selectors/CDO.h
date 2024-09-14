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
concept IsSoftPtr =
	_IGRP HasGet<T>
	&& requires {
		   typename T::ElementType;
	   } //
	&& requires(T t) {
		   t.LoadSynchronous();
	   };

} // namespace Private

namespace Selectors
{
/**
 * Selector that yields the "class default object" / "constructed default object" (CDO) from UClass-like values.
 * Safe to use with null inputs.
 * May yield null.
 *
 * This selector is expected to be used with types like...
 * - UClass*
 * - TObjectPtr<UClass>
 * - TWeakObjectPtr<UClass>
 * - TSubclassOf<T>
 * - TSoftClassPtr<T>
 *
 * @usage SomeClasses | Select(Selectors::CDO)
 */
inline constexpr auto CDO = []<typename ClassType>(ClassType&& ClassLike) {
	// Remove reference to go from things like (A) to (B) so we can do things with (C):
	// (A) const TSoftClassPtr<T>&
	// (B) const TSoftClassPtr<T>
	// (C) ValueClassType::ElementType
	using ValueClassType = std::remove_reference_t<ClassType>;

	// `TSubclassOf<UFoo>::GetDefaultObject` knows the class type & does the null-checks & casting for us.
	if constexpr (_IGRP HasGetDefaultObject<ClassType>)
	{
		return ClassLike.GetDefaultObject();
	}
	// `TSoftClassPtr<UFoo>` knows the class type, but we need to perform the cast (returns `UFoo*`).
	else if constexpr (_IGRP IsSoftPtr<ValueClassType>)
	{
		const UClass* Class = ClassLike.Get();
		return (Class != nullptr) ? static_cast<ValueClassType::ElementType*>(Class->GetDefaultObject()) : nullptr;
	}
	// Other UClass-like types don't know the class type, so we cannot cast (just returns `UObject*`).
	else if constexpr (_IGRP HasGet<ClassType>)
	{
		const UClass* Class = ClassLike.Get();
		return (Class != nullptr) ? Class->GetDefaultObject() : nullptr;
	}
	else
	{
		const UClass* Class = ClassLike;
		return (Class != nullptr) ? Class->GetDefaultObject() : nullptr;
	}
};

} // namespace Selectors

} // namespace IG::Ranges

#include "IGRanges/Impl/Epilogue.inl"
