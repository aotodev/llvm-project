//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// REQUIRES: std-at-least-c++26

// <hive>

// __hive_iterator: member types, size, triviality, the const conversion and the bidirectional iterator concept.

#include <concepts>
#include <cstddef>
#include <hive>
#include <iterator>
#include <memory>
#include <type_traits>

#include "min_allocator.h"
#include "test_macros.h"

template <class T, class VoidPtr>
void check() {
  using It   = std::__hive_iterator<T, VoidPtr, false>;
  using CIt  = std::__hive_iterator<T, VoidPtr, true>;
  using Ptr  = typename std::pointer_traits<VoidPtr>::template rebind<T>;
  using CPtr = typename std::pointer_traits<VoidPtr>::template rebind<const T>;

  static_assert(std::same_as<typename It::iterator_category, std::bidirectional_iterator_tag>);
  static_assert(std::same_as<typename It::value_type, T>);
  static_assert(std::same_as<typename It::difference_type, typename std::pointer_traits<VoidPtr>::difference_type>);
  static_assert(std::same_as<typename It::reference, T&>);
  static_assert(std::same_as<typename It::pointer, Ptr>);
  static_assert(std::same_as<typename CIt::value_type, T>);
  static_assert(std::same_as<typename CIt::reference, const T&>);
  static_assert(std::same_as<typename CIt::pointer, CPtr>);

  static_assert(std::bidirectional_iterator<It>);
  static_assert(std::bidirectional_iterator<CIt>);
  static_assert(std::is_trivially_copyable_v<It>);
  static_assert(std::is_trivially_copyable_v<CIt>);
  static_assert(std::is_nothrow_default_constructible_v<It>);
  static_assert(std::is_convertible_v<It, CIt>);
  static_assert(!std::is_convertible_v<CIt, It>);
  static_assert(sizeof(It) == 2 * sizeof(Ptr));
}

// Value-initialized iterators are the empty hive's end, and equal, across constness too.
void test_value_initialized() {
  using It  = std::__hive_iterator<int, void*, false>;
  using CIt = std::__hive_iterator<int, void*, true>;
  It a;
  It b;
  CIt c;
  assert(a == b);
  assert(a == c);
  assert(c == a);
}

int main(int, char**) {
  check<int, void*>();
  check<int, min_pointer<void>>();
  test_value_initialized();

  return 0;
}
