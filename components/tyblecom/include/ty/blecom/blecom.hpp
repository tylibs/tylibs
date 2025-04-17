// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "tyconfig.h"
#include <etl/string.h>
#include "ty/pimpl.hpp"
#include "ty/printable.hpp"

#include "etl/memory.h"

#if CONFIG_TY_BLECOM

namespace ty {
namespace ble {
class BleCom
{
public:
    struct Configuration
    {
        const etl::string<31> &name;
    };
    BleCom() = delete;
    ~BleCom();

protected:
    explicit BleCom(const Configuration &aConfiguration);

public:
    [[nodiscard]] static auto create(Configuration &) -> etl::unique_ptr<BleCom>;
    static auto               destroy(etl::unique_ptr<BleCom> &) -> void;

private:
    const Configuration &mConfiguration;
    class impl;
    impl *pimpl;
    // pimpl<impl> impl;
};
} // namespace ble
} // namespace ty

#endif /* CONFIG_TY_BLECOM */
