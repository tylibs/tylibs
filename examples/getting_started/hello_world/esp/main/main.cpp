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
#include <ty/platform/toolchain.h>

const char *TAG = "main";

/*****************************************************************************/

extern "C" void app_main()
{
    tyInstance *instance;
    instance = tyInstanceInitSingle();

    while (true)
    {
        tyLogInfo("%s", "Hello, World!");
        // next event in 1 second
        tyPlatDelay(1000);
    }
    tyInstanceFinalize(instance);
}
