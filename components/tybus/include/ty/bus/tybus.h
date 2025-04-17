// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

/**
 * @file
 * @brief
 *   Main include for C based applications
 */

#ifndef TY_BUS_TY_BUS_H
#define TY_BUS_TY_BUS_H

#include <stdbool.h>
#include <stddef.h>

#include <ty/error.h>

#ifdef __cplusplus
extern "C" {
#endif

// Macro for genereting a const char* from a string literal
#define TY_BUS_EVENT_NAME(tag, name, value) const char *const tag##_EVENT_##name = value;
#define TY_BUS_STATE_NAME(tag, name, value) const char *const tag##_STATE_##name = value;
#define TY_BUS_EVENT(event, data, len) ((TyBusEvent){event, data, len})
#define TY_BUS_SUBSCRIBER(module, table, tableRowCount, startState) \
    ((TyBusSubscriber){module, table, tableRowCount, startState, NULL})

// ARRAY_SIZE ... (be careful, this is base on sizeof, only use on arrays)
#define TY_BUS_TABLE_ROW_COUNT(x) (sizeof(x) / sizeof((x)[0]))

typedef struct TyBusEvent
{
    const char *event;
    void       *data;
    size_t      dataLen;
} TyBusEvent;

typedef void (*TyBusStateActionFn)(const TyBusEvent *aEvent);

typedef bool (*TyBusStateConditionFn)();

typedef struct TyBusStateTableRow
{
    const char           *state;
    const char           *event;
    TyBusStateConditionFn conditionCheck;
    TyBusStateActionFn    entryAction;
    const char           *nextState;
    TyBusStateActionFn    exitAction;
    bool                  stop;
} TyBusStateTableRow;

typedef struct TyBusSubscriber
{
    const char *const               name;
    const TyBusStateTableRow *const table;
    const size_t                    tableRowCount;
    const char                     *currentState;
    TyBusStateActionFn              exitAction;
} TyBusSubscriber;

tyError tyBusSubscribe(TyBusSubscriber *aSubscriber);

tyError tyBusPublish(const char *aEventName, void *aData, size_t aDataLen);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // TY_BUS_TY_BUS_H
