// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>
#include <sdbus-c++/Types.h>
#include <sdbus-c++/sdbus-c++.h>

namespace org {
namespace bluez {

using namespace sdbus;

class LEAdvertisingManager1 : public std::enable_shared_from_this<LEAdvertisingManager1>
{
    std::unique_ptr<sdbus::IProxy> proxy_;
    static constexpr const char   *INTERFACE_NAME = "org.bluez.LEAdvertisingManager1";

public:
    LEAdvertisingManager1(IConnection &connection, ServiceName destination, ObjectPath objectPath)
        : proxy_{createProxy(connection, std::move(destination), std::move(objectPath))}
    {
    }

    ~LEAdvertisingManager1() { proxy_->unregister(); }

public:
    void RegisterAdvertisement(const sdbus::ObjectPath                     &advertisementPath,
                               const std::map<std::string, sdbus::Variant> &options = {})
    {
        proxy_->callMethod("RegisterAdvertisement")
            .onInterface(INTERFACE_NAME)
            .withArguments(advertisementPath, options);
    }

    AsyncMethodInvoker RegisterAdvertisementAsync(const sdbus::ObjectPath                     &advertisementPath,
                                                  const std::map<std::string, sdbus::Variant> &options = {})
    {
        return proxy_->callMethodAsync("RegisterAdvertisement")
            .onInterface(INTERFACE_NAME)
            .withArguments(advertisementPath, options);
    }

    void UnregisterAdvertisement(const sdbus::ObjectPath &advertisementPath)
    {
        proxy_->callMethod("UnregisterAdvertisement").onInterface(INTERFACE_NAME).withArguments(advertisementPath);
    }

public:
    uint8_t ActiveInstances()
    {
        return proxy_->getProperty("ActiveInstances").onInterface(INTERFACE_NAME).get<uint8_t>();
    }

    uint8_t SupportedInstances()
    {
        return proxy_->getProperty("SupportedInstances").onInterface(INTERFACE_NAME).get<uint8_t>();
    }

    std::vector<std::string> SupportedIncludes()
    {
        return proxy_->getProperty("SupportedIncludes").onInterface(INTERFACE_NAME).get<std::vector<std::string>>();
    }
};

} // namespace bluez
} // namespace org
