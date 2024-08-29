// Copyright Ian Good

#pragma once

#include "Containers/Array.h"
#include "Containers/ArrayView.h"

//---------------------------------------------------------------------------------------

template <class T>
auto begin(TArray<T>& r)
{
	return r.GetData();
}

template <class T>
auto end(TArray<T>& r)
{
	return r.GetData() + r.Num();
}

template <class T>
auto begin(const TArray<T>& r)
{
	return r.GetData();
}

template <class T>
auto end(const TArray<T>& r)
{
	return r.GetData() + r.Num();
}

// optional; compiles fine w/o, but probably better to provide this
template <class T>
auto size(const TArray<T>& r)
{
	return r.Num();
}

//---------------------------------------------------------------------------------------

// optional; compiles fine w/o, but probably better to provide this
template <class T>
auto size(const TArrayView<T>& r)
{
	return r.Num();
}

//---------------------------------------------------------------------------------------
