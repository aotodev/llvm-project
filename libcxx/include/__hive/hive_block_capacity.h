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
#include <__algorithm/max.h>
#include <__algorithm/min.h>
#include <__config>
#include <__cstddef/size_t.h>
#include <__hive/hive_bitmap.h>
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
  using __block_type _LIBCPP_NODEBUG =
      __hive_block<_Tp, _VoidPtr>; // not __block: a keyword in Objective-C++ and under -fblocks

  // A block's words and slots are one allocation of units of this size.
  static constexpr size_t __align = std::max(alignof(_Tp), alignof(typename __block_type::__word_type));

  // The words of the largest block plus the padding after the words and after the slots.
  static constexpr size_t __max_overhead =
      __hive_bitmap<__block_type>::__words_for(__block_type::__max_capacity) *
          sizeof(typename __block_type::__word_type) +
      2 * __align;

  static constexpr size_t __hard_min = 1;
  // Below SIZE_MAX / sizeof(_Tp) by __max_overhead, so a block's size in bytes cannot wrap.
  static constexpr size_t __hard_max =
      std::min(__block_type::__max_capacity, (numeric_limits<size_t>::max() - __max_overhead) / sizeof(_Tp));

  static constexpr size_t __default_max_bytes = 128 * 1024;

  static constexpr size_t __default_min = std::min<size_t>(64, __hard_max);
  static constexpr size_t __default_max = std::clamp(__default_max_bytes / sizeof(_Tp), __default_min, __hard_max);

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
