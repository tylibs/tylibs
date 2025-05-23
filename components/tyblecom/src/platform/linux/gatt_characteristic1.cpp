// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

#include "gatt_characteristic1.hpp"
#include "gatt_service1.hpp"
#include "tyblecom-adaptor-glue.h"

#include <memory>
#include <sdbus-c++/AdaptorInterfaces.h>
#include <sdbus-c++/Types.h>
#include <sdbus-c++/VTableItems.h>
#include <set>
#include <spdlog/pattern_formatter-inl.h>
#include <stdexcept>
#include "ty/logging.h"

namespace {
constexpr auto kTag = "GattChar1";
}

namespace org::bluez {

static std::string makePath(const GattService1 &service)
{
    std::string path{service.getPath()};
    path.append("/char").append(std::to_string(service.nextCharacteristicIndex()));
    return path;
}

GattCharacteristic1::GattCharacteristic1(const std::shared_ptr<GattService1> &service,
                                         std::string                          uuid,
                                         const bool                           hasAcquireWrite,
                                         const bool                           hasAcquireNotify,
                                         const bool                           hasValue,
                                         const bool                           valueIsDirected)
    : AdaptorInterfaces{*service->getConnection(), ObjectPath{makePath(*service)}}
    , service_{service}
    , path_{AdaptorInterfaces::getObject().getObjectPath()}
    , uuid_{std::move(uuid)}
    , notifyingSessions_{0}
    , writeAcquired_{false}
    , notifyAcquired_{false}
{
    tyLogInfo(kTag, "GattCharacteristic1::GattCharacteristic1() %s", path_.c_str());
}

GattCharacteristic1::~GattCharacteristic1()
{
    service_->removeCharacteristic(shared_from_this());
    unregisterAdaptor();
}

// ---- Add Properties ---------------------------------------------------------

void GattCharacteristic1::addValue(std::string value)
{
    value_.assign(value.begin(), value.end());
}

void GattCharacteristic1::addValue(const std::vector<uint8_t> &value)
{
    value_ = value;
}

void GattCharacteristic1::addFlag(std::string flag)
{
    static const std::set<std::string> valid{"broadcast",
                                             "read",
                                             "write-without-response",
                                             "write",
                                             "notify",
                                             "indicate",
                                             "authenticated-signed-writes",
                                             "reliable-write",
                                             "writable-auxiliaries",
                                             "encrypt-read",
                                             "encrypt-write",
                                             "encrypt-authenticated-read",
                                             "encrypt-authenticated-write",
                                             "secure-read",  // (Server only)
                                             "secure-write", // (Server only)
                                             "authorize"};

    if (valid.find(flag) == valid.end())
        throw std::invalid_argument("'flag' not valid");

    flags_.push_back(std::move(flag));
}

// ---- Default Handlers -------------------------------------------------------

std::vector<uint8_t> GattCharacteristic1::ReadValue(const std::map<std::string, Variant> &options)
{
    return value_;
}

void GattCharacteristic1::WriteValue(const std::vector<uint8_t> &value, const std::map<std::string, Variant> &options)
{
    value_ = value;
}

std::map<sdbus::UnixFd, std::vector<uint8_t>> GattCharacteristic1::DirectedValue()
// std::map<ObjectPath, std::vector<std::vector<uint8_t>>> GattCharacteristic1::DirectedValue()
{
    throw Error(Error::Name("org.bluez.Error.NotSupported"),
                "Property 'GattCharacteristic1::DirectedValue' not overloaded!");
}

std::tuple<UnixFd, uint16_t> GattCharacteristic1::AcquireWrite(const std::map<std::string, Variant> &options)
{
    throw Error(Error::Name("org.bluez.Error.NotSupported"),
                "Method 'GattCharacteristic1::AcquireWrite' not overloaded!");
}

std::tuple<UnixFd, uint16_t> GattCharacteristic1::AcquireNotify(const std::map<std::string, Variant> &options)
{
    throw Error(Error::Name("org.bluez.Error.NotSupported"),
                "Method 'GattCharacteristic1::AcquireNotify' not overloaded!");
}

void GattCharacteristic1::StartNotify()
// void GattCharacteristic1::StartNotify(const std::map<std::string, sdbus::Variant> &options)
{
    notifyingSessions_ += 1;
}

void GattCharacteristic1::StopNotify()
// void GattCharacteristic1::StopNotify(const std::map<std::string, sdbus::Variant> &options)
{
    notifyingSessions_ -= 1;
}

void GattCharacteristic1::Confirm()
{
    tyLogCrit(kTag, "WARNING: Method 'GattCharacteristic1::Confirm' default handler");
}

// ---- for Friends ------------------------------------------------------------
void GattCharacteristic1::addDescriptor(std::shared_ptr<GattDescriptor1> descriptor)
{
    const std::string &descPath = descriptor->getPath();

    for (const auto &path : includes_)
    {
        if (path == descPath)
            throw std::invalid_argument(std::string("GattCharacteristic1::addDescriptor '") + descPath +
                                        std::string("' already registered!"));
    }

    descriptors_.push_back(std::move(descriptor));
    includes_.emplace_back(descPath);
}

void GattCharacteristic1::removeDescriptor(const std::shared_ptr<GattDescriptor1> &descriptor)
{
    const std::string &descPath = descriptor->getPath();

    for (auto iter{includes_.begin()}; iter != includes_.end(); ++iter)
    {
        if (*iter == descPath)
        {
            includes_.erase(iter);
            break;
        }
    }

    for (auto iter{descriptors_.begin()}; iter != descriptors_.end(); ++iter)
    {
        if (*iter == descriptor)
        {
            descriptors_.erase(iter);
            break;
        }
    }
}

const std::string &GattCharacteristic1::getPath() const
{
    return path_;
}

std::shared_ptr<IConnection> GattCharacteristic1::getConnection() const
{
    return service_->getConnection();
}

ObjectPath GattCharacteristic1::Service()
{
    return service_->getPath();
}

void GattCharacteristic1::emitPropertyChangedSignal(const sdbus::PropertyName &property)
{
    PropertiesExt_adaptor::emitPropertyChangedSignal(INTERFACE_NAME, property);
}

void GattCharacteristic1::registerWithService(const std::shared_ptr<GattCharacteristic1> &self)
{
    registerAdaptor();
    service_->addCharacteristic(self);
}
} // namespace org::bluez
