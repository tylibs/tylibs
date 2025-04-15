// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: GPL-3.0-only OR OR LicenseRef-BYG-Software-1.0
/**
 * @file
 * @brief
 *   BYG implementation
 */
#include "app.h"
#include <tybus/tybus.h>
#include <tysettings/platform/settings.h>
#include <ty/logging.h>
#include <ty/platform/toolchain.h>
#include "ty/error.h"
#include "ty/platform/thread.h"

static const char *kLogModule = "App";

TYBUS_EVENT_NAME(MAIN, HELLO_WORLD, "HelloWorld");
TYBUS_STATE_NAME(MAIN, START, "Start");
// 2. Declare the actions using in the state table
void actionHelloWorld(const TyBusEvent *aEvent);

// 3. Glue everything together
const TyBusStateTableRow mStateTable[] = {{
    .state = MAIN_STATE_START,                // only process the event, if the current state
                                              // match. TY_STATE_INITIAL is the default state.
                                              // use TY_STATE_ANY, if this should be ignored
    .event          = MAIN_EVENT_HELLO_WORLD, // process if the event matches
    .conditionCheck = NULL,                   // No condition function.  The action is always executed.
    .entryAction    = actionHelloWorld,       // The function to call when the event occurs.
    .nextState      = NULL,                   // Keep the current state after the action.
    .exitAction     = NULL,                   // No exit function.
    .stop           = true                    // Stop processing after this entry (this is the default case)
}};
TyBusSubscriber          mSubscriber =
    TYBUS_SUBSCRIBER("app", mStateTable, TYBUS_TABLE_ROW_COUNT(mStateTable), MAIN_STATE_START);

void actionHelloWorld(const TyBusEvent *aEvent)
{
    TY_UNUSED_VARIABLE(aEvent);
    tyLogCrit(kLogModule, "Action onHelloWorld() called from Tybus");
}

/*****************************************************************************/
namespace {
TY_TOOL_PACKED_BEGIN
struct AppPersistentSettings
{
    uint16_t key;
    int      a;
    int      b;
} TY_TOOL_PACKED_END;
AppPersistentSettings mAppPersistentSettings = {.key = 4242, .a = 10, .b = 10};

tyError initSettings(tyInstance *aInstance)
{
    uint16_t settings_length = 0;
    tyPlatSettingsInit(aInstance, NULL, 0);
    // check if the settings are already there
    // if not, set the default values
    if (tyPlatSettingsGet(aInstance, mAppPersistentSettings.key, 0, (uint8_t *)&mAppPersistentSettings,
                          &settings_length) == TY_ERROR_NOT_FOUND ||
        settings_length != sizeof(mAppPersistentSettings))
    {
        if (tyPlatSettingsSet(aInstance, mAppPersistentSettings.key, (uint8_t *)&mAppPersistentSettings,
                              sizeof(mAppPersistentSettings)) != TY_ERROR_NONE)
        {
            tyLogCrit(kLogModule, "Failed to set settings");
            return TY_ERROR_FAILED;
        }
    }
    return TY_ERROR_NONE;
}
} // namespace

void *appInit(void *)
{
    tyLogInfo(kLogModule, "Starting Application, keep fingers crossed");
    tyInstance *instance;
    instance = tyInstanceInitSingle();
    // Initialize the settings subsystem
    initSettings(instance);

    // initialize the Tybus subscriber
    tyBusSubscribe(&mSubscriber);

    tyBusPublish(MAIN_EVENT_HELLO_WORLD, NULL, 0);
    while (true)
    {
        // next event in 1 second
        tyPlatDelay(5000);
        tyLogInfo(kLogModule, "Sleep done");
    }
    tyInstanceFinalize(instance);
    return NULL;
}
