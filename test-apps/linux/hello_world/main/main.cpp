// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

/**
 * @file
 * @brief
 *   TYLiBS simplest example: Hello World
 */

#include <ty/instance.h>
#include <ty/logging.h>
#include <ty/platform/thread.h>

#include "etl/string.h"

extern "C" int main(void)
{
    tyInstance     *instance;
    etl::string<32> hello_world = "Hello World!";

    instance = tyInstanceInitSingle();
    // Initialize the settings subsystem
    while (true)
    {
        tyLogInfo("modul", "%s", hello_world.c_str());
        tyLogCrit("modul", "Critical");
        // next event in 1 second
        tyPlatDelay(1000);
    }
    tyInstanceFinalize(instance);
    return 0;
}
