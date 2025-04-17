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

#include "mqtt/client.h"

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
    try
    {
        self.mClient->connect();
    } catch (const mqtt::exception &e)
    {
        tyLogCrit("posix.mqtt", "MQTT Connect failed: %s", e.what());
        return;
    }
}
} // namespace ty
