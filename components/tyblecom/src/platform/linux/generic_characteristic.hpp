// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gatt_characteristic_builder.hpp"

namespace org {
namespace bluez {

class GenericCharacteristic : public GattCharacteristicBuilder<GenericCharacteristic>
{
public:
    GenericCharacteristic(std::shared_ptr<GattService1> service, std::string uuid)
        : GattCharacteristicBuilder{move(service), move(uuid), false, false, true}
    {
    }

    static GenericCharacteristic &create(std::shared_ptr<GattService1> service, std::string uuid)
    {
        auto self = new GenericCharacteristic{move(service), move(uuid)};
        return *self;
    }
};

} // namespace bluez
} // namespace org
