// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

/**
 * @file
 * @brief
 *   TYLiBS simplest example: Hello World
 */

#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <ty/instance.h>
#include <ty/logging.h>
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
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
    tyInstanceFinalize(instance);
}
