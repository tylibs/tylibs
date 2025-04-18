// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0
/**
 * @file
 *   This file implements the Tiny platform abstraction for non-volatile storage of settings.
 */

#include "ty/tycommon.h"

#include "blecom_p.hpp"
#include <ty/exit_code.h>
#include <ty/logging.h>
#include <ty/common/debug.hpp>
#include "ty/blecom/blecom.hpp"
#include "ty/new.hpp"

#include <etl/string.h>

namespace ty {
namespace ble {
// everything public is fine here...
struct BleComPlat : public BleCom
{
    explicit BleComPlat(BleCom::Configuration &aConfiguration)
        : BleCom(aConfiguration)
    {
    }

    etl::string<2048> data;
};
static TY_DEFINE_ALIGNED_VAR(sBleComPlatRaw, sizeof(BleComPlat), uint64_t);
auto BleCom::create(BleCom::Configuration &aConfiguration) -> etl::unique_ptr<BleCom>
{
    auto pImpl = etl::unique_ptr<BleCom>(new (sBleComPlatRaw) BleComPlat(aConfiguration));
    // auto pImpl = etl::unique_ptr<BleCom>(New<BleComPlat>(MALLOC_CAP_INTERNAL, aConfiguration));
    return pImpl;
}

void BleCom::init()
{
    auto &self = *(static_cast<BleComPlat *>(this));
    self.data  = "hello";
    self.d_func()->connect();
}
} // namespace ble
} // namespace ty
