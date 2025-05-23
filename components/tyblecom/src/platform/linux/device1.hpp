// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "tyblecom-proxy-glue.h"
#include <sdbus-c++/Types.h>

namespace org {
namespace bluez {

using namespace sdbus;

class Device1 : public ProxyInterfaces<org::bluez::Device1_proxy>
{
public:
    Device1(IConnection &connection, ServiceName destination, ObjectPath objectPath)
        : ProxyInterfaces{connection, std::move(destination), std::move(objectPath)}
    {
        registerProxy();
    }

    ~Device1() { unregisterProxy(); }
};

} // namespace bluez
} // namespace org
