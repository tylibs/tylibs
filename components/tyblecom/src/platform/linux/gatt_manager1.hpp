// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <sdbus-c++/sdbus-c++.h>

namespace org::bluez {
using namespace sdbus;

class GattManager1 : public std::enable_shared_from_this<GattManager1>
{
    std::unique_ptr<IProxy> proxy_;
    static constexpr auto   INTERFACE_NAME = "org.bluez.GattManager1";

public:
    GattManager1(IConnection &connection, ServiceName destination, ObjectPath objectPath)
        : proxy_{createProxy(connection, std::move(destination), std::move(objectPath))}
    {
    }

    GattManager1(const std::shared_ptr<IConnection> &connection, ServiceName destination, ObjectPath objectPath)
        : proxy_{createProxy(*connection, std::move(destination), std::move(objectPath))}
    {
    }

    ~GattManager1() { proxy_->unregister(); }

    void RegisterApplication(const ObjectPath &application, const std::map<std::string, Variant> &options) const
    {
        proxy_->callMethod("RegisterApplication").onInterface(INTERFACE_NAME).withArguments(application, options);
    }

    AsyncMethodInvoker RegisterApplicationAsync(const ObjectPath                     &application,
                                                const std::map<std::string, Variant> &options) const
    {
        return proxy_->callMethodAsync("RegisterApplication")
            .onInterface(INTERFACE_NAME)
            .withArguments(application, options);
    }

    void UnregisterApplication(const ObjectPath &application) const
    {
        proxy_->callMethod("UnregisterApplication").onInterface(INTERFACE_NAME).withArguments(application);
    }
};
} // namespace org::bluez
