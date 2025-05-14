
// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gatt_characteristic1.hpp"

namespace org::bluez {
// Forward declarations
class GattService1;

using namespace sdbus;

template <typename _Derived> class GattCharacteristicBuilder : public GattCharacteristic1
{
protected:
    GattCharacteristicBuilder(const std::shared_ptr<GattService1> &service,
                              const std::string                   &uuid,
                              const bool                           hasAcquireWrite  = false,
                              const bool                           hasAcquireNotify = false,
                              const bool                           hasValue         = false,
                              const bool                           valueIsDirected  = false)
        : GattCharacteristic1{service, uuid, hasAcquireWrite, hasAcquireNotify, hasValue, valueIsDirected}
    {
    }

public:
    _Derived &withValue(const std::string &value)
    {
        addValue(value);
        return static_cast<_Derived &>(*this);
    }

    _Derived &withValue(const std::vector<uint8_t> &value)
    {
        addValue(value);
        return static_cast<_Derived &>(*this);
    }

    _Derived &withFlag(const std::string &flag)
    {
        addFlag(flag);
        return static_cast<_Derived &>(*this);
    }

    std::shared_ptr<_Derived> finalize()
    {
        auto self = std::shared_ptr<GattCharacteristicBuilder>(this);
        registerWithService(self);
        return std::static_pointer_cast<_Derived>(self);
    }
};
} // namespace org::bluez
