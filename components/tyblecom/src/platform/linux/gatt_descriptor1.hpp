// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <sdbus-c++/Types.h>
#include <sdbus-c++/sdbus-c++.h>

#include "properties_ext_adaptor.hpp"
#include "tyblecom-adaptor-glue.h"

namespace org::bluez {
// Forward declarations
class GattCharacteristic1;

using namespace sdbus;

class GattDescriptor1 : public AdaptorInterfaces<GattDescriptor1_adaptor, PropertiesExt_adaptor>,
                        public std::enable_shared_from_this<GattDescriptor1>
{
public:
    GattDescriptor1(const std::shared_ptr<GattCharacteristic1> &service, const std::string &name, std::string uuid);

    ~GattDescriptor1() override;

    const std::string &getPath() const { return path_; }

    std::vector<uint8_t> ReadValue(const std::map<std::string, Variant> &options) override
    {
        throw Error(Error::Name{"org.bluez.Error.NotSupported"}, "Method 'ReadValue' default handler");
    }

    void WriteValue(const std::vector<uint8_t> &value, const std::map<std::string, Variant> &options) override
    {
        throw Error(Error::Name{"org.bluez.Error.NotSupported"}, "Method 'WriteValue' default handler");
    }

    /**
     * @brief
     *
     * @return std::string
     */
    std::string UUID() override { return uuid_; }

    ObjectPath Characteristic() override
    {
        // TODO implement
        return {};
    }

    std::vector<uint8_t> Value() override
    {
        // TODO implement
        return {};
    }

    std::vector<std::string> Flags() override
    {
        // TODO implement
        return {};
    }

protected:
    void emitPropertyChangedSignal(const PropertyName &property)
    {
        PropertiesExt_adaptor::emitPropertyChangedSignal(GattDescriptor1_adaptor::INTERFACE_NAME, property);
    }

    std::map<std::string, Variant>       dict_;
    std::string                          path_;
    std::string                          uuid_;
    std::shared_ptr<GattCharacteristic1> characteristic_;
};
} // namespace org::bluez
