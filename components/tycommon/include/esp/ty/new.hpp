// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

/**
 * @file
 *   This template can be used to allocate memory in SPIRAM or internal
 */

#pragma once

#include "esp_heap_caps.h"
#include <utility>

template <typename T, typename... Args> inline T *New(uint32_t alloc_caps, Args &&...args)
{
    void *p = heap_caps_calloc(1, sizeof(T), alloc_caps);
    if (p != nullptr)
    {
        return new (p) T(std::forward<Args>(args)...);
    }
    return nullptr;
}
