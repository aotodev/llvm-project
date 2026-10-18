// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP___HIVE_HIVE_H
#define _LIBCPP___HIVE_HIVE_H

#include <__assert>
#include <__config>
#include <__hive/hive_block_capacity.h>
#include <__hive/hive_iterator.h>
#include <__hive/hive_limits.h>
#include <__hive/hive_storage.h>
#include <__iterator/reverse_iterator.h>
#include <__memory/allocator.h>
#include <__memory/allocator_traits.h>
#include <__type_traits/is_nothrow_constructible.h>
#include <__type_traits/is_same.h>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#  pragma GCC system_header
#endif

_LIBCPP_PUSH_MACROS
#include <__undef_macros>

#if _LIBCPP_STD_VER >= 26

_LIBCPP_BEGIN_NAMESPACE_STD

template <class _Tp, class _Allocator = allocator<_Tp>>
class hive {
private:
  using __alloc_traits _LIBCPP_NODEBUG   = allocator_traits<_Allocator>;
  using __void_ptr _LIBCPP_NODEBUG       = typename __alloc_traits::void_pointer;
  using __block_capacity _LIBCPP_NODEBUG = __hive_block_capacity<_Tp, __void_ptr>;
  using __storage _LIBCPP_NODEBUG        = __hive_storage<_Tp, _Allocator>;

  static_assert(__check_valid_allocator<_Allocator>::value, "");
  static_assert(is_same<typename _Allocator::value_type, _Tp>::value,
                "Allocator::value_type must be same type as value_type");

public:
  // types
  using value_type             = _Tp;
  using allocator_type         = _Allocator;
  using pointer                = typename __alloc_traits::pointer;
  using const_pointer          = typename __alloc_traits::const_pointer;
  using reference              = value_type&;
  using const_reference        = const value_type&;
  using size_type              = typename __alloc_traits::size_type;
  using difference_type        = typename __alloc_traits::difference_type;
  using iterator               = __hive_iterator<value_type, __void_ptr, false>;
  using const_iterator         = __hive_iterator<value_type, __void_ptr, true>;
  using reverse_iterator       = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  // [hive.cons], construct/copy/destroy
  _LIBCPP_HIDE_FROM_ABI constexpr hive() noexcept(is_nothrow_default_constructible_v<allocator_type>)
      : hive(allocator_type()) {}

  _LIBCPP_HIDE_FROM_ABI constexpr explicit hive(const allocator_type& __a) noexcept : __storage_(__a) {}

  _LIBCPP_HIDE_FROM_ABI constexpr explicit hive(hive_limits __l) : hive(__l, allocator_type()) {}

  _LIBCPP_HIDE_FROM_ABI constexpr hive(hive_limits __l, const allocator_type& __a) : __storage_(__l, __a) {
    _LIBCPP_ASSERT_ARGUMENT_WITHIN_DOMAIN(
        __block_capacity::__within_hard_limits(__l), "hive: block limits are not within the hard limits");
  }

  [[nodiscard]] _LIBCPP_HIDE_FROM_ABI allocator_type get_allocator() const noexcept { return __storage_.__alloc(); }

  // [hive.capacity], capacity
  [[nodiscard]] _LIBCPP_HIDE_FROM_ABI bool empty() const noexcept { return __storage_.__size() == 0; }
  [[nodiscard]] _LIBCPP_HIDE_FROM_ABI size_type size() const noexcept { return __storage_.__size(); }
  [[nodiscard]] _LIBCPP_HIDE_FROM_ABI size_type capacity() const noexcept { return __storage_.__capacity(); }

  [[nodiscard]] _LIBCPP_HIDE_FROM_ABI constexpr hive_limits block_capacity_limits() const noexcept {
    return __storage_.__limits();
  }

  [[nodiscard]] _LIBCPP_HIDE_FROM_ABI static constexpr hive_limits block_capacity_default_limits() noexcept {
    return __block_capacity::__default_limits;
  }

  [[nodiscard]] _LIBCPP_HIDE_FROM_ABI static constexpr hive_limits block_capacity_hard_limits() noexcept {
    return __block_capacity::__hard_limits;
  }

  [[nodiscard]] _LIBCPP_HIDE_FROM_ABI static constexpr bool is_within_hard_limits(hive_limits __l) noexcept {
    return __block_capacity::__within_hard_limits(__l);
  }

private:
  __storage __storage_;
};

_LIBCPP_END_NAMESPACE_STD

#endif // _LIBCPP_STD_VER >= 26

_LIBCPP_POP_MACROS

#endif // _LIBCPP___HIVE_HIVE_H
