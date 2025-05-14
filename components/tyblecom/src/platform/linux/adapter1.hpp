// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "tyblecom-proxy-glue.h"
#include <sdbus-c++/Types.h>

namespace org {
namespace bluez {

using namespace sdbus;

class Adapter1 : public ProxyInterfaces<org::bluez::Adapter1_proxy>
{
public:
    Adapter1(IConnection &connection, ServiceName destination, ObjectPath objectPath)
        : ProxyInterfaces{connection, std::move(destination), std::move(objectPath)}
    {
        registerProxy();
    }

    ~Adapter1() { unregisterProxy(); }
};

} // namespace bluez
} // namespace org
