// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0
#include <ty/logging.h>

#include "blecom_p.hpp"
#include <ty/blecom/blecom.hpp>
namespace ty {
namespace ble {

BleCom::BleCom(const Configuration &aConfiguration)
    : mConfiguration(aConfiguration)
{
}
BleCom::~BleCom() {}

void BleComPrivate::connect()
{
    tyLogInfo("blecom_private", "connect");
}

} // namespace ble
} // namespace ty
