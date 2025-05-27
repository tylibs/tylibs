// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0
/**
 * @file
 * @brief
 *   BYG implementation
 */
#include "app.h"
#include "ty/blecom/blecom.hpp"
#include "ty/bus/tybus.h"
#include "ty/error.h"
#include "ty/logging.h"
#include "ty/net/mqtt/mqtt.hpp"
#include "ty/platform/thread.h"
#include "ty/platform/toolchain.h"
#include "ty/settings/platform/settings.h"

#include "etl/string.h"

static const char *kLogModule = "App";

/*****************************************************************************/
namespace {

void initBleCom()
{
    ty::ble::BleCom::Configuration bleConfig = {.name = "Be101:BYG Gateway"};
    ty::ble::BleCom::create(bleConfig);
    ty::ble::BleCom::get().init();
    try
    {
        ty::ble::BleCom::get().start();
    } catch (const ty::ble::BleComControllerException &e)
    {
        tyLogCrit(kLogModule, "Failed to start BLE: %s", e.what());
    }
}
} // namespace

void *appInit(void *)
{
    tyLogInfo(kLogModule, "Starting Application, keep fingers crossed");
    initBleCom();

    while (true)
    {
        // next event in 1 second
        tyPlatDelay(5000);
        tyLogInfo(kLogModule, "In Loop");
    }
}
