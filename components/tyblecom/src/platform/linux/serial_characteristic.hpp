// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <gatt_characteristic_builder.hpp>

#include <queue>
#include <set>

#include "generic_characteristic.hpp"
#include "bluez-dbus-cpp/Util.h"

#include <spdlog/spdlog-inl.h>

namespace org::bluez {
class SerialClient : public Client
{
public:
    SerialClient(const ObjectPath &path, const uint16_t usable_mtu, std::vector<uint8_t> &&initialValue)
        : Client{path, usable_mtu}
        , value_{initialValue}
    {
    }

    void setData(std::vector<uint8_t> &&value) { this->value_ = std::move(value); }

    void setData(const std::vector<uint8_t> &value) { this->value_.assign(value.begin(), value.end()); }

    std::vector<uint8_t> &&getData() { return std::move(value_); }

protected:
    std::vector<uint8_t> value_;
};

class SerialCharacteristic final : public GattCharacteristicBuilder<GenericCharacteristic>
{
public:
    SerialCharacteristic(const std::shared_ptr<GattService1> &service,
                         std::shared_ptr<IConnection>         connection,
                         const std::string                   &uuid)
        : GattCharacteristicBuilder{service, uuid, false, false, true, true}
        , connection_{std::move(connection)}
    {
        flags_ = {"notify", "write-without-response"};
    }

    static SerialCharacteristic &create(const std::shared_ptr<GattService1> &service,
                                        const std::shared_ptr<IConnection>  &connection,
                                        const std::string                   &uuid)
    {
        const auto self = new SerialCharacteristic(service, connection, uuid);
        return *self;
    }

protected:
    std::vector<uint8_t> ReadValue(const std::map<std::string, sdbus::Variant> &options) override
    {
        spdlog::info("Serial RX: {}", std::string(value_.begin(), value_.end()));
        ;
        throw Error("org.bluez.Error.NotSupported",
                    "'read' not supported on 'SerialCharacteristic', use 'notify' instead");
    }

    void WriteValue(const std::vector<uint8_t> &value, const std::map<std::string, sdbus::Variant> &options) override
    {
        spdlog::info("Serial TX: {}", std::string(value.begin(), value.end()));
        const auto client = getClient(options);
        if (client == lastClient_)
        {
            if (lastDevice_ && !value.empty() && value[0] == 'd')
            {
                spdlog::info("Disconnecting from client");
                lastDevice_->Disconnect();
            }
        }
        client->setData(value);
        Notify(client);
    }

    void StartNotify(const std::map<std::string, sdbus::Variant> &options) override
    {
        // insert client into our list
        const auto client = getClient(options);
        notifying_.insert(client);
        spdlog::info("SerialCharacteristic::StartNotify '{}'", client->getPath());
    }

    void StopNotify(const std::map<std::string, sdbus::Variant> &options) override
    {
        // remove client from our list
        if (!options.empty())
        {
            // When disconnecting from a device, StopNotify is called with an empty options map
            const auto client = getClient(options);
            const auto iter   = notifying_.find(client);
            notifying_.erase(iter);
            spdlog::info("SerialCharacteristic::StopNotify '{}'", client->getPath());
        }
    }

    void Notify(const std::shared_ptr<SerialClient> &client)
    {
        // in this example, we always append a 'k' message to test the multi-packet aspect
        // that is part of the 'DirectedValue' BlueZ patch rev2 feature
        directedQueue_.insert(std::make_pair(
            client->getPath(), std::vector<std::vector<uint8_t>>{client->getData(), std::vector<uint8_t>{'k'}}));

        if (notifying_.find(client) != notifying_.end())
        {
            emitPropertyChangedSignal("DirectedValue");
        }
    }

    std::map<ObjectPath, std::vector<std::vector<uint8_t>>> DirectedValue() override
    {
        SPDLOG_TRACE("DirectedValue()");
        return std::move(directedQueue_);
    }

    std::shared_ptr<SerialClient> getClient(const std::map<std::string, Variant> &options)
    {
        auto [real_mtu, usable_mtu] = Util::getMTUFromOptions(options);

        auto path = Util::getObjectPathFromOptions(options);
        auto iter = clients_.find(path);
        if (iter == clients_.end())
        {
            spdlog::info("SerialCharacteristic::getClient - creating client '{}'", path);
            auto client = std::make_shared<SerialClient>(path, usable_mtu, std::vector<uint8_t>());
            iter        = clients_.insert(std::make_pair(path, client)).first;
            lastClient_ = iter->second;
            lastDevice_ = std::make_shared<Device1>(*connection_, "org.bluez", path);
        }
        return iter->second;
    }

    void removeClient(const std::map<std::string, Variant> &options)
    {
        const auto path = Util::getObjectPathFromOptions(options);
        if (const auto iter = clients_.find(path); iter != clients_.end())
        {
            spdlog::info("SerialCharacteristic::removeClient '{}'", path);
            clients_.erase(iter);
        }
    }

    std::map<ObjectPath, std::vector<std::vector<uint8_t>>> directedQueue_;
    std::map<ObjectPath, std::shared_ptr<SerialClient>>     clients_;
    std::set<std::shared_ptr<SerialClient>>                 notifying_;
    std::shared_ptr<SerialClient>                           lastClient_;
    std::shared_ptr<Device1>                                lastDevice_;
    std::shared_ptr<IConnection>                            connection_;
};
} // namespace org::bluez
