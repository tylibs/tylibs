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

#include <tynet/mqtt/mqtt.hpp>

#include "mqtt/client.h"

const std::string DFLT_SERVER_URI("mqtt://localhost:1883");
const std::string CLIENT_ID("paho_cpp_async_subscribe");
const std::string TOPIC("#");

namespace ty {
// everything public is fine here...
class PosixMqtt : public Mqtt
{
public:
    PosixMqtt(Mqtt::MqttConfiguration &aConfiguration)
        : Mqtt(aConfiguration)
    {
        mClient = etl::unique_ptr<mqtt::client>(new mqtt::client(std::string(aConfiguration.serverUri.c_str()),
                                                                 std::string(aConfiguration.clientId.c_str())));
    };
    etl::unique_ptr<mqtt::client> mClient;
};

auto Mqtt::create(Mqtt::MqttConfiguration &aConfiguration) -> etl::unique_ptr<Mqtt>
{
    auto pImpl = etl::unique_ptr<Mqtt>(new PosixMqtt(aConfiguration));
    return pImpl;
}

void Mqtt::connect()
{
    auto &self = *(static_cast<PosixMqtt *>(this));
    self.mClient->connect();
    tyLogInfo("posix.mqtt", "MQTT Connect");
}
} // namespace ty
