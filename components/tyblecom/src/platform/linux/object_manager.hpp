// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "tyblecom-proxy-glue.h"
#include <sdbus-c++/sdbus-c++.h>
#include "ty/logging.h"

namespace sdbus {
class ObjectManagerProxy final : public sdbus::ProxyInterfaces<sdbus::ObjectManager_proxy>
{
public:
    ObjectManagerProxy(sdbus::IConnection &connection, const sdbus::ServiceName &destination, sdbus::ObjectPath path)
        : ProxyInterfaces(connection, destination, path)
        , m_path{std::move(path)}
    {
        registerProxy();
        tyLogInfo("proxy", "registered object manager proxy %s", m_path.c_str());
    }

    ~ObjectManagerProxy() { unregisterProxy(); }

    void handleInterfaces()
    {
        for (const auto &[object, interfacesAndProperties] : GetManagedObjects())
        {
            onInterfacesAdded(object, interfacesAndProperties);
        }
    }

private:
    void onInterfacesAdded(const sdbus::ObjectPath &objectPath,
                           const std::map<sdbus::InterfaceName, std::map<sdbus::PropertyName, sdbus::Variant>>
                               &interfacesAndProperties) override
    {
        for (const auto &[interface, _] : interfacesAndProperties)
        {
            tyLogInfo("proxy", "added: %s", interface.c_str());
            // spdlog::info("added: {}", interface);
        }
    }

    void onInterfacesRemoved(const sdbus::ObjectPath                 &objectPath,
                             const std::vector<sdbus::InterfaceName> &interfaces) override
    {
        for (const auto &interface : interfaces)
        {
            tyLogInfo("proxy", "removed: %s", interface.c_str());
            // spdlog::info("removed: {}", interface);
        }
    }

private:
    sdbus::ObjectPath m_path;
};

} // namespace sdbus
