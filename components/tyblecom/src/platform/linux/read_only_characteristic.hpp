// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gatt_characteristic_builder.hpp"

namespace org::bluez {
class ReadOnlyCharacteristic final : public GattCharacteristicBuilder<ReadOnlyCharacteristic>
{
public:
    ReadOnlyCharacteristic(const std::shared_ptr<GattService1> &service,
                           const std::string                   &uuid,
                           const std::string                   &value)
        : GattCharacteristicBuilder{service, uuid, false, false, true}
    {
        addFlag("read");
        addValue(value);
    }

    static std::shared_ptr<ReadOnlyCharacteristic> createFinal(const std::shared_ptr<GattService1> &service,
                                                               const std::string                   &uuid,
                                                               const std::string                   &value)
    {
        const auto self   = new ReadOnlyCharacteristic{service, uuid, value};
        auto       shared = std::shared_ptr<ReadOnlyCharacteristic>(self);
        self->registerWithService(shared);
        return shared;
    }
};
} // namespace org::bluez
