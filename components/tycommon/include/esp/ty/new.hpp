// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

/**
 * @file
 *   This file defines the new operator used by OpenThread.
 */

#ifndef COMMON_NEW_HPP_
#define COMMON_NEW_HPP_

#include <stddef.h>

#include "esp_heap_caps.h"
#include "ty/platform/toolchain.h"

#ifdef __cplusplus
extern "C" {
#include "ty/new.hpp"
#endif

// inline void *operator new(size_t, void *p) throw()
// {
//     return p;
// }
void *operator new(size_t size)
{
    void *ptr = heap_caps_malloc(size, MALLOC_CAP_SPIRAM);
    if (!ptr)
    {
        abort();
    }
    return ptr;
}

void *operator new[](size_t size)
{
    void *ptr = heap_caps_malloc(size, MALLOC_CAP_SPIRAM);
    if (!ptr)
    {
        abort();
    }
    return ptr;
}

void operator delete(void *ptr) noexcept
{
    free(ptr);
}

void operator delete(void *ptr, std::size_t) noexcept
{
    free(ptr);
}

void operator delete[](void *ptr) noexcept
{
    free(ptr);
}

void operator delete[](void *ptr, std::size_t) noexcept
{
    free(ptr);
}

#ifdef __cplusplus
} // extern "C"
#endif

#endif // OT_INCLUDE_COMMON_NEW_HPP_
