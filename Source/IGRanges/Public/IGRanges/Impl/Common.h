// Copyright Ian Good

#pragma once

#include "Misc/CoreMiscDefines.h"
#include <type_traits>

namespace IG::Ranges::Private
{
template <typename T>
[[nodiscard]] T Construct()
{
	// For non-fundamental types (bool, int, etc.), use `EForceInit` if possible.
	// This is desirable for things like `FVector` & `FQuat`.
	if constexpr (!std::is_fundamental_v<T> && std::is_constructible_v<T, EForceInit>)
	{
		// We're intentionally using `ForceInit` (zero) & not `ForceInitToZero` (one) because `is_constructible_v` is
		// True even for types that have constructors like `Foo::Foo(int)`, which is not actually `EForceInit`, but
		// implicit conversions are accepted anyhow. So we hope that `Foo(0)` is correct here.
		// Also, the distinction between `ForceInit` & `ForceInitToZero` is sometimes important. For example, `FQuat`
		// will be "identity" or "zeroes" (respectively) & we probably want "identity" in all cases (like when using
		// `Sum` or `FirstOrDefault`).
		return T(EForceInit::ForceInit);
	}
	else
	{
		return T{};
	}
}

} // namespace IG::Ranges::Private
