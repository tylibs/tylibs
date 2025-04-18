// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0
#include <stdarg.h>
#include <stdio.h>
#include <zephyr/kernel.h>

#include "platform/logging.h"

#define LOG_MODULE_NAME net_openthread
#define LOG_LEVEL LOG_LEVEL_DBG
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(LOG_MODULE_NAME);

#include "platform-zephyr.h"

/* Convert OT log level to zephyr log level. */
static inline int log_translate(tyLogLevel aLogLevel)
{
    switch (aLogLevel)
    {
    case TY_LOG_LEVEL_NONE:
    case TY_LOG_LEVEL_CRIT:
        return LOG_LEVEL_ERR;
    case TY_LOG_LEVEL_WARN:
        return LOG_LEVEL_WRN;
    case TY_LOG_LEVEL_NOTE:
    case TY_LOG_LEVEL_INFO:
        return LOG_LEVEL_INF;
    case TY_LOG_LEVEL_DEBG:
        return LOG_LEVEL_DBG;
    default:
        break;
    }

    return -1;
}

void tyPlatLog(tyLogLevel aLogLevel, const char *aLogRegion, const char *aFormat, ...)
{
    ARG_UNUSED(aLogRegion);

#if defined(CONFIG_LOG)
    int     level = log_translate(aLogLevel);
    va_list param_list;

    if (level < 0)
    {
        return;
    }

    va_start(param_list, aFormat);
    log_generic(level, aFormat, param_list);
    va_end(param_list);
#else
    ARG_UNUSED(aLogLevel);
    ARG_UNUSED(aFormat);
#endif
}
