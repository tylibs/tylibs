// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

#ifndef TY_CONNECTION_H_
#define TY_CONNECTION_H_

#include "tyconfig.h"

#include "ty/instance.h"
#include "ty/printable.hpp"

#include "ty/net/network_interface.hpp"

#include "etl/memory.h"
#include "etl/optional.h"
#include "etl/string.h"

namespace ty {
class Connection : public Printable
{
public:
    struct ConnectionConfiguration
    {
        const etl::istring &serverUri;
        const etl::istring &clientId;
    };

public:
    Connection();
    virtual ~Connection();

    size_t printTo(Print &out) const;

private:
    // NetworkInterface mNetworkInterface;
};
} // namespace ty
#endif // TY_CONNECTION_H_
