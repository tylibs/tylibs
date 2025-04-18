// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

/**
 * @file
 * @brief
 *   This file includes the platform abstraction for the scheduler service.
 */

#ifndef TY_BUS_PLATFORM_SCHEDULER_H_
#define TY_BUS_PLATFORM_SCHEDULER_H_

#include <stdarg.h>
#include <stdint.h>

#include <ty/error.h>

#include "ty/bus/tybus.h"

typedef void (*TyBusSchedulerNotifyFn)(const TyBusEvent *aEvent);

/**
 * @addtogroup plat-scheduler
 *
 * @brief
 *   This module includes the platform abstraction for the debug log service.
 *
 * @{
 */

/**
 * add event to queue
 *
 * @param[in]  aLogLevel   The log level.
 * @param[in]  aFormat     A pointer to the format string.
 * @param[in]  ...         Arguments for the format specification.
 */
void tyBusSchedulerEventPush(const TyBusEvent &aEvent);

void tyBusOnSchedulerEvent(TyBusSchedulerNotifyFn aNotifyFn);

tyError tyBusSchedulerInit(void);

#endif // TY_BUS_PLATFORM_SCHEDULER_H_
