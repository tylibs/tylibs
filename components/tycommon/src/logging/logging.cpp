
// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

/**
 * @file
 *   This file implements the OpenThread logging related APIs.
 */
#include "ty/tycommon.h"

#include "ty/common/code_utils.hpp"
#include "ty/common/debug.hpp"

#include "string.hpp"

#include "instance.hpp"
#include "ty/log.hpp"

using namespace ty;

tyLogLevel tyLoggingGetLevel(void)
{
    return static_cast<tyLogLevel>(Instance::GetLogLevel());
}

#if TY_CONFIG_LOG_LEVEL_DYNAMIC_ENABLE
tyError tyLoggingSetLevel(tyLogLevel aLogLevel)
{
    Error error = kErrorNone;

    VerifyOrExit(aLogLevel <= kLogLevelDebg && aLogLevel >= kLogLevelNone, error = kErrorInvalidArgs);
    Instance::SetLogLevel(static_cast<LogLevel>(aLogLevel));

exit:
    return error;
}
#endif

void tyLogCrit(const char *aModuleName, const char *aFormat, ...)
{
#if TY_SHOULD_LOG_AT(TY_LOG_LEVEL_CRIT) && TY_CONFIG_LOG_PLATFORM
    va_list args;

    va_start(args, aFormat);
    Logger::LogVarArgs(aModuleName, kLogLevelCrit, aFormat, args);
    va_end(args);
#else
    TY_UNUSED_VARIABLE(aFormat);
    TY_UNUSED_VARIABLE(aModuleName);
#endif
}

void tyLogWarn(const char *aModuleName, const char *aFormat, ...)
{
#if TY_SHOULD_LOG_AT(TY_LOG_LEVEL_WARN) && TY_CONFIG_LOG_PLATFORM
    va_list args;

    va_start(args, aFormat);
    Logger::LogVarArgs(aModuleName, kLogLevelWarn, aFormat, args);
    va_end(args);
#else
    TY_UNUSED_VARIABLE(aFormat);
#endif
}

void tyLogNote(const char *aModuleName, const char *aFormat, ...)
{
#if TY_SHOULD_LOG_AT(TY_LOG_LEVEL_NOTE) && TY_CONFIG_LOG_PLATFORM
    va_list args;

    va_start(args, aFormat);
    Logger::LogVarArgs(aModuleName, kLogLevelNote, aFormat, args);
    va_end(args);
#else
    TY_UNUSED_VARIABLE(aFormat);
#endif
}

void tyLogInfo(const char *aModuleName, const char *aFormat, ...)
{
#if TY_SHOULD_LOG_AT(TY_LOG_LEVEL_INFO) && TY_CONFIG_LOG_PLATFORM
    va_list args;

    va_start(args, aFormat);
    Logger::LogVarArgs(aModuleName, kLogLevelInfo, aFormat, args);
    va_end(args);
#else
    TY_UNUSED_VARIABLE(aFormat);
#endif
}

void tyLogDebg(const char *aModuleName, const char *aFormat, ...)
{
#if TY_SHOULD_LOG_AT(TY_LOG_LEVEL_DEBG) && TY_CONFIG_LOG_PLATFORM
    va_list args;

    va_start(args, aFormat);
    Logger::LogVarArgs(aModuleName, kLogLevelDebg, aFormat, args);
    va_end(args);
#else
    TY_UNUSED_VARIABLE(aFormat);
#endif
}

tyError tyLogGenerateNextHexDumpLine(tyLogHexDumpInfo *aInfo)
{
    AssertPointerIsNotNull(aInfo);

    return GenerateNextHexDumpLine(*aInfo);
}
