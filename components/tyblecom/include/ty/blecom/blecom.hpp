// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "tyconfig.h"
#include <etl/string.h>

#include "etl/memory.h"
#include "ty/pimpl.hpp"
#if CONFIG_TY_BLECOM

namespace ty {
namespace ble {
class BleComPrivate
{
public:
    void connect();
    int  x;
};
class BleCom : public Pimpl<BleCom, BleComPrivate>
{
public:
    struct Configuration
    {
        const etl::string<31> &name;
    };
    BleCom()  = delete;
    ~BleCom() = default;

protected:
    explicit BleCom(const Configuration &aConfiguration)
        : mConfiguration(aConfiguration)
    {
    }

public:
    [[nodiscard]] static auto create(Configuration &) -> etl::unique_ptr<BleCom>;
    static auto               destroy(etl::unique_ptr<BleCom> &) -> void;

    void init();

private:
    const Configuration &mConfiguration;
};
} // namespace ble
} // namespace ty

#endif /* CONFIG_TY_BLECOM */
