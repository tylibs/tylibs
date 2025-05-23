// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gatt_service1.hpp"

#include <sdbus-c++/AdaptorInterfaces.h>
#include <sdbus-c++/IConnection.h>
#include <sdbus-c++/TypeTraits.h>
#include <sdbus-c++/Types.h>
#include <sdbus-c++/sdbus-c++.h>

#include "tyblecom-adaptor-glue.h"

#include <ty/logging.h>

namespace org::bluez {
using namespace sdbus;

class GattApplication1 : public AdaptorInterfaces<sdbus::ObjectManager_adaptor>,
                         public std::enable_shared_from_this<GattApplication1>
{
public:
    GattApplication1(const std::shared_ptr<IConnection> &connection, ObjectPath objectPath)
        : AdaptorInterfaces{*connection, objectPath}

        , path_{std::move(objectPath)}
        , connection_{connection}
    {
        tyLogInfo("GattApp", "Register %s", path_.c_str());

        registerAdaptor();
    }

    ~GattApplication1() { unregisterAdaptor(); }

    void addService(std::shared_ptr<GattService1> service)
    {
        for (const auto &serv : services_)
        {
            if (serv == service)
                throw std::invalid_argument(std::string("GattApplication::addService '") + service->getPath() +
                                            std::string("' already registered!"));
        }

        services_.push_back(std::move(service));
    }

    void removeService(const std::shared_ptr<GattService1> &service)
    {
        for (auto iter{services_.cbegin()}; iter != services_.cend(); ++iter)
        {
            if (*iter == service)
            {
                services_.erase(iter);
                break;
            }
        }
    }

    const ObjectPath &getPath() const { return path_; }

    std::shared_ptr<IConnection> getConnection() const { return connection_; }

    // std::map<sdbus::ObjectPath, std::map<std::string, std::map<std::string, sdbus::Variant>>> GetManagedObjects();

protected:
    sdbus::ObjectPath                          path_;
    std::shared_ptr<IConnection>               connection_;
    std::vector<std::shared_ptr<GattService1>> services_;
};
} // namespace org::bluez
