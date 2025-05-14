// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

#include "gatt_service1.hpp"
#include "gatt_application1.hpp"
#include <sdbus-c++/Types.h>

namespace org::bluez {
static std::string makePath(const std::string &characteristicPath, const std::string &name)
{
    std::string path{characteristicPath};
    path.append("/").append(name);
    return path;
}

// ---- Creation ---------------------------------------------------------------

// std::shared_ptr<GattService1> GattService1::createWith( std::shared_ptr<GattApplication1> app, std::string
// serviceName, std::string serviceUUID )
// {
//     auto service = std::shared_ptr<GattService1>{ new GattService1{ std::move(app), std::move(serviceName),
//     std::move(serviceUUID) }}; return service;
// }

// std::shared_ptr<GattService1> GattService1::andRegister()
// {
//     app_->addService( shared_from_this() );
//     return shared_from_this();
// }

GattService1::GattService1(const std::shared_ptr<GattApplication1> &app,
                           const std::string                       &serviceName,
                           std::string                              serviceUUID)
    : AdaptorInterfaces{*app->getConnection(), ObjectPath{makePath(app->getPath(), serviceName)}}
    , app_{app}
    , path_{makePath(app->getPath(), serviceName)}
    , uuid_{std::move(serviceUUID)}
    , primary_{true}
{
    registerAdaptor();
    const std::shared_ptr<GattService1> ptr(this);
    app_->addService(ptr);
}

GattService1::~GattService1()
{
    app_->removeService(shared_from_this());
    unregisterAdaptor();
}

void GattService1::addCharacteristic(std::shared_ptr<GattCharacteristic1> characteristic)
{
    for (const auto &chrc : characteristics_)
    {
        if (chrc == characteristic)
            throw std::invalid_argument(std::string("GattService1::addCharacteristic '") + characteristic->getPath() +
                                        std::string("' already registered!"));
    }

    characteristics_.push_back(std::move(characteristic));
}

void GattService1::removeCharacteristic(const std::shared_ptr<GattCharacteristic1> &characteristic)
{
    for (auto iter{characteristics_.cbegin()}; iter != characteristics_.cend(); ++iter)
    {
        if (*iter == characteristic)
        {
            characteristics_.erase(iter);
            break;
        }
    }
}

void GattService1::addSubService(std::shared_ptr<GattService1> service)
{
    const std::string &srvPath = service->getPath();

    for (const auto &path : includes_)
    {
        if (path == srvPath)
            throw std::invalid_argument(std::string("GattService1::addSubService '") + srvPath +
                                        std::string("' already registered!"));
    }

    services_.push_back(std::move(service));
    includes_.emplace_back(srvPath);
}

void GattService1::removeSubService(const std::shared_ptr<GattService1> &service)
{
    const std::string &srvPath = service->getPath();

    for (auto iter{services_.cbegin()}; iter != services_.cend(); ++iter)
    {
        if (*iter == service)
        {
            services_.erase(iter);
            break;
        }
    }

    for (auto iter{includes_.cbegin()}; iter != includes_.cend(); ++iter)
    {
        if (*iter == srvPath)
        {
            includes_.erase(iter);
            break;
        }
    }
}

const sdbus::ObjectPath &GattService1::getPath() const
{
    return path_;
}

std::shared_ptr<IConnection> GattService1::getConnection() const
{
    return app_->getConnection();
}

int GattService1::nextCharacteristicIndex() const
{
    return static_cast<int>(characteristics_.size());
}
} // namespace org::bluez
