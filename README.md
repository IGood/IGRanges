# ![Logo](Resources/Icon128.png) <sup>for Unreal</sup>
 
[![MIT License](https://img.shields.io/badge/license-MIT-green.svg?style=flat-square)](/LICENSE)

**IGRanges** is an Unreal Engine 5 plugin that leverages the [Ranges library (C++20)](https://en.cppreference.com/w/cpp/ranges) to provide [LINQ (C#)](https://learn.microsoft.com/en-us/dotnet/csharp/linq/) style code patterns.

Unreal's container types (e.g. `TArray<T>`) do not support Ranges out of the box (yet?), so this plugin first adds the necessary customization point objects to make them compatible.\
Beyond that, a handful of common mapping & filtering operations have been implemented in ways that are familiar to programmers acquainted with UE & LINQ.

----

## C++20 Ranges, But Think LINQ

A quick primer for Ranges from [cppreference.com](https://en.cppreference.com/w/cpp/ranges):
> The ranges library is an extension and generalization of the algorithms and iterator libraries that makes them more powerful by making them composable and less error-prone.\
The library creates and manipulates range views, lightweight objects that indirectly represent iterable sequences (ranges).

### 👩‍💻 Example A:
The following example "pipes" an array of integers through views to generate a sequence that is a modified subset of the original values.

#### 👩‍💻 Example A.1:
```cpp
int myInts[] = { 0, 1, 2, 3, 4, 5 };
auto even = [](int i) { return i % 2 == 0; };
auto square = [](int i) { return i * i; };
// the "pipe" syntax of composing the views:
for (int i : myInts | std::views::filter(even) | std::views::transform(square))
    std::cout << i << ' ';
// Output: 0 4 16
```

This example is very *C++*-looking. It's not very *UE*-looking & it doesn't read quite as clearly as LINQ.\
If we were to rewrite it in a UE style with IGRanges, then it might look something like this:

#### 👩‍💻 Example A.2:
```cpp
TArray<int32> MyInts = { 0, 1, 2, 3, 4, 5 };
auto IsEven = [](int i) { return i % 2 == 0; };
auto Square = [](int i) { return i * i; };
FString Result;
for (int32 i : MyInts | Where(IsEven) | Select(Square))
{
    Result.AppendInt(i);
    Result += TEXT(", ");
}
UE_LOG(LogTemp, Log, TEXT("Result{%s}"), *Result);
// Output: Result{0, 4, 16,}
```

Or, depending on style preferences, it might look something like this:

#### 👩‍💻 Example A.3:
```cpp
TArray<int32> MyInts = { 0, 1, 2, 3, 4, 5 };
auto Sqevens = MyInts
    | Where([](int i) { return i % 2 == 0; })
    | Select([](int i) { return i * i; }); 
FString Result;
for (int32 i : Sqevens)
{
    Result.AppendInt(i);
    Result += TEXT(", ");
}
```

### 👩‍💻 Example B:

A better demonstration of IGRanges is shown in the following example.\
The declarative syntax can make it easier to understand the intent of the code.\
In this case, we have a collection of pointers to Actors (some of which might be null) & we want to get the World from one of them.

<table>
<tr><th>Imperative</th><th>Declarative</th></tr>
<tr>
<td>

```cpp
TArray<AActor*> MaybeActors = ...;
UWorld* World = nullptr;
for (const AActor* Actor : MaybeActors)
{
    if (Actor != nullptr)
    {
        World = Actor->GetWorld();
        break;
    }
}
if (World != nullptr) ...
```

</td>
<td>

```cpp
TArray<AActor*> MaybeActors = ...;
UWorld* World = MaybeActors
    | NonNull()
    | Select(&AActor::GetWorld)
    | FirstOrDefault();
if (World != nullptr) ...





```

</td>
</tr>
</table>

### 👩‍💻 Example C:

Filtering & collecting elements also becomes very easy with IGRanges.

<table>
<tr><th>Imperative</th><th>Declarative</th></tr>
<tr>
<td>

```cpp
TArray<UObject*> SomeObjects = ...;
TArray<UFoo*> Foos;
for (UObject* Obj : SomeObjects)
{
    if (UFoo* Foo = Cast<Foo>(Obj))
    {
        Foos.Add(Foo);
    }
}
```

</td>
<td>

```cpp
TArray<UObject*> SomeObjects = ...;
TArray<UFoo*> Foos = SomeObjects | OfType<Foo>() | ToArray();







```

</td>
</tr>
</table>

----

### ✨ Features

- `Where`, `WhereNot`
- `NonNull`, `NonNullRef`
- `Select`
- `OfType<T>`, `OfTypeRef<T>`
- `FirstOrDefault`
- `Count`
- `ToArray`
- `ToSet`

----

### 🔗 Related Links

- [Ranges library (C++20)](https://en.cppreference.com/w/cpp/ranges)
- [\<ranges\> | Microsoft Learn](https://learn.microsoft.com/en-us/cpp/standard-library/ranges)
- [Language Integrated Query (LINQ) - C# | Microsoft Learn](https://learn.microsoft.com/en-us/dotnet/csharp/linq/)
