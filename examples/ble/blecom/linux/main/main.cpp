// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

/**
 * @file
 * @brief
 *   TYLiBS simplest example: Hello World
 */
#include "app.h"
#include "pthread.h"

namespace {
static pthread_t mAppThread;
}

extern "C" int main()
{
    // initialize the application as a pthread
    pthread_create(&mAppThread, NULL, appInit, NULL);
    // wait for the application to finish
    pthread_join(mAppThread, NULL);
    return 0;
}
