// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP___HIVE_HIVE_STORAGE_H
#define _LIBCPP___HIVE_HIVE_STORAGE_H

#include <__config>
#include <__hive/hive_block.h>
#include <__hive/hive_block_capacity.h>
#include <__hive/hive_limits.h>
#include <__memory/allocator_traits.h>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#  pragma GCC system_header
#endif

_LIBCPP_PUSH_MACROS
#include <__undef_macros>

#if _LIBCPP_STD_VER >= 26

_LIBCPP_BEGIN_NAMESPACE_STD

template <class _Tp, class _Allocator>
class __hive_storage {
private:
  using __alloc_traits _LIBCPP_NODEBUG   = allocator_traits<_Allocator>;
  using __void_ptr _LIBCPP_NODEBUG       = typename __alloc_traits::void_pointer;
  using __block _LIBCPP_NODEBUG          = __hive_block<_Tp, __void_ptr>;
  using __block_pointer _LIBCPP_NODEBUG  = typename __block::__block_pointer;
  using __index_type _LIBCPP_NODEBUG     = typename __block::__index_type;
  using __block_capacity _LIBCPP_NODEBUG = __hive_block_capacity<_Tp, __void_ptr>;

public:
  using allocator_type _LIBCPP_NODEBUG = _Allocator;
  using size_type _LIBCPP_NODEBUG      = typename __alloc_traits::size_type;

  _LIBCPP_HIDE_FROM_ABI constexpr explicit __hive_storage(const allocator_type& __a) noexcept : __alloc_(__a) {}

  _LIBCPP_HIDE_FROM_ABI constexpr __hive_storage(hive_limits __l, const allocator_type& __a) noexcept : __alloc_(__a) {
    __set_limits(__l);
  }

  __hive_storage(const __hive_storage&)            = delete;
  __hive_storage& operator=(const __hive_storage&) = delete;

  [[nodiscard]] _LIBCPP_HIDE_FROM_ABI constexpr size_type __size() const noexcept { return __size_; }
  [[nodiscard]] _LIBCPP_HIDE_FROM_ABI constexpr size_type __capacity() const noexcept { return __capacity_; }
  [[nodiscard]] _LIBCPP_HIDE_FROM_ABI constexpr hive_limits __limits() const noexcept {
    return hive_limits(__min_, __max_);
  }

  [[nodiscard]] _LIBCPP_HIDE_FROM_ABI constexpr allocator_type& __alloc() noexcept { return __alloc_; }
  [[nodiscard]] _LIBCPP_HIDE_FROM_ABI constexpr const allocator_type& __alloc() const noexcept { return __alloc_; }

private:
  __block_pointer __first_         = nullptr;
  __block_pointer __last_          = nullptr;
  __block_pointer __free_head_     = nullptr; // active blocks with free slots
  size_type __size_                = 0;
  size_type __capacity_            = 0;
  __block_pointer __reserved_head_ = nullptr;
  __index_type __min_              = __block_capacity::__default_min;
  __index_type __max_              = __block_capacity::__default_max;
  _LIBCPP_NO_UNIQUE_ADDRESS allocator_type __alloc_;

  _LIBCPP_HIDE_FROM_ABI constexpr void __set_limits(hive_limits __l) noexcept {
    __l    = __block_capacity::__clamp(__l);
    __min_ = static_cast<__index_type>(__l.min);
    __max_ = static_cast<__index_type>(__l.max);
  }
};

_LIBCPP_END_NAMESPACE_STD

#endif // _LIBCPP_STD_VER >= 26

_LIBCPP_POP_MACROS

#endif // _LIBCPP___HIVE_HIVE_STORAGE_H
