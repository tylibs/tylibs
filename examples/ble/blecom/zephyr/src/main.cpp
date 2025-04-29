// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

/**
 * @file
 * @brief
 *   TYLiBS simplest example: Hello World
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <ty/logging.h>
#include <ty/platform/toolchain.h>

#define LOG_MODULE_NAME main
LOG_MODULE_REGISTER(main);
const char *TAG = "main";

/*****************************************************************************/

extern "C" int main(void)
{
    tyInstance *instance;
    instance = tyInstanceInitSingle();

    while (true)
    {
        tyLogInfo("%s", "Hello, World!");
        // next event in 1 second
        k_sleep(K_SECONDS(1));
    }
    tyInstanceFinalize(instance);
    return 0;
}
