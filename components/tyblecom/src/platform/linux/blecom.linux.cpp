// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0
/**
 * @file
 *   This file implements the Tiny platform abstraction for non-volatile storage of settings.
 */

#include <ty/exit_code.h>
#include <ty/logging.h>
#include <ty/common/debug.hpp>

#include "ty/blecom/blecom.hpp"

namespace ty {
namespace ble {
// everything public is fine here...
struct BleComPlat : public BleCom
{
    explicit BleComPlat(BleCom::Configuration &aConfiguration)
        : BleCom(aConfiguration)
    {
    }
    // explicit BleComPlat(BleCom::Configuration &aConfiguration)
    //     : BleCom() {};
};

auto BleCom::create(BleCom::Configuration &aConfiguration) -> etl::unique_ptr<BleCom>
{
    auto pImpl = etl::unique_ptr<BleCom>(new BleComPlat(aConfiguration));
    return pImpl;
}

void BleCom::init()
{
    auto &self    = *(static_cast<BleComPlat *>(this));
    self.d_ptr->x = 1;
}
} // namespace ble
} // namespace ty
