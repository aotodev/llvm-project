// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP___HIVE_HIVE_BLOCK_CAPACITY_H
#define _LIBCPP___HIVE_HIVE_BLOCK_CAPACITY_H

#include <__algorithm/clamp.h>
#include <__algorithm/min.h>
#include <__config>
#include <__cstddef/size_t.h>
#include <__hive/hive_block.h>
#include <__hive/hive_limits.h>
#include <limits>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#  pragma GCC system_header
#endif

_LIBCPP_PUSH_MACROS
#include <__undef_macros>

#if _LIBCPP_STD_VER >= 26

_LIBCPP_BEGIN_NAMESPACE_STD

template <class _Tp, class _VoidPtr>
struct __hive_block_capacity {
  using __block _LIBCPP_NODEBUG = __hive_block<_Tp, _VoidPtr>;

  static constexpr size_t __min_block_bytes = 1024;      // placeholder, to benchmark
  static constexpr size_t __max_block_bytes = 64 * 1024; // placeholder, to benchmark
  static_assert(__min_block_bytes > sizeof(__block));

  static constexpr size_t __hard_min = 1;
  static constexpr size_t __hard_max = std::min(__block::__max_capacity, numeric_limits<size_t>::max() / sizeof(_Tp));

  static constexpr size_t __default_min =
      std::clamp((__min_block_bytes - sizeof(__block)) / sizeof(_Tp), __hard_min, __hard_max);
  static constexpr size_t __default_max = std::clamp(__max_block_bytes / sizeof(_Tp), __default_min, __hard_max);

  static constexpr hive_limits __hard_limits{__hard_min, __hard_max};
  static constexpr hive_limits __default_limits{__default_min, __default_max};

  _LIBCPP_HIDE_FROM_ABI static constexpr bool __within_hard_limits(hive_limits __l) noexcept {
    return __hard_min <= __l.min && __l.min <= __l.max && __l.max <= __hard_max;
  }

  // Maps any input into the hard limits with min <= max.
  _LIBCPP_HIDE_FROM_ABI static constexpr hive_limits __clamp(hive_limits __l) noexcept {
    size_t __max = std::clamp(__l.max, __hard_min, __hard_max);
    return hive_limits(std::clamp(__l.min, __hard_min, __max), __max);
  }
};

_LIBCPP_END_NAMESPACE_STD

#endif // _LIBCPP_STD_VER >= 26

_LIBCPP_POP_MACROS

#endif // _LIBCPP___HIVE_HIVE_BLOCK_CAPACITY_H
