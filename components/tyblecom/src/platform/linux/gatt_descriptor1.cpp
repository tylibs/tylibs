// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

#include "gatt_descriptor1.hpp"
#include "gatt_characteristic1.hpp"

namespace org::bluez {
static std::string makePath(const std::string &characteristicPath, const std::string &name)
{
    std::string path{characteristicPath};
    path.append("/").append(name);
    return path;
}

GattDescriptor1::GattDescriptor1(const std::shared_ptr<GattCharacteristic1> &service,
                                 const std::string                          &name,
                                 std::string                                 uuid)
    : AdaptorInterfaces{*service->getConnection(), ObjectPath{makePath(service->getPath(), name)}}
    , path_{makePath(service->getPath(), name)}
    , uuid_{std::move(uuid)}
    , characteristic_{service}
{
    registerAdaptor();
    characteristic_->addDescriptor(shared_from_this());
}

GattDescriptor1::~GattDescriptor1()
{
    characteristic_->removeDescriptor(shared_from_this());
    unregisterAdaptor();
}
} // namespace org::bluez
