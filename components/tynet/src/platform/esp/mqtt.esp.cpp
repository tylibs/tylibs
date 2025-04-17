// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0
/**
 * @file
 *   This file implements the Tiny platform abstraction for non-volatile storage of settings.
 */

#include <ty/exit_code.h>
#include <ty/logging.h>
#include <ty/common/debug.hpp>
#include "ty/common/code_utils.hpp"

#include <ty/net/mqtt/mqtt.hpp>

namespace ty {
// everything public is fine here...
class EspMqtt : public Mqtt
{
public:
    EspMqtt(Mqtt::MqttConfiguration &aConfiguration)
        : Mqtt(aConfiguration) {
            // mClient = etl::unique_ptr<mqtt::client>(new mqtt::client(std::string(aConfiguration.serverUri.c_str()),
            //                                                          std::string(aConfiguration.clientId.c_str())));
        };
    // etl::unique_ptr<mqtt::client> mClient;
};

auto Mqtt::create(Mqtt::MqttConfiguration &aConfiguration) -> etl::unique_ptr<Mqtt>
{
    auto pImpl = etl::unique_ptr<Mqtt>(new EspMqtt(aConfiguration));
    return pImpl;
}

void Mqtt::connect()
{
    // auto &self = *(static_cast<EspMqtt *>(this));
    // self.mClient->connect();
    // try
    // {
    //     auto &self = *(static_cast<EspMqtt *>(this));
    //     tyLogInfo("esp.mqtt", "%s", self.mConfiguration.clientId.c_str());

    //     // self.mClient->connect();
    // } catch (...)
    // {
    //     tyLogCrit("esp.mqtt", "MQTT Connect failed: ");
    //     return;
    // }
}
} // namespace ty
