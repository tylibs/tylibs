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
    etl::unique_ptr<mqtt::client> mClient;
};

Mqtt::Mqtt() {}

Mqtt::~Mqtt() {}

auto Mqtt::create(tyInstance const &aInstance) -> etl::optional<etl::unique_ptr<Mqtt>>
{
    auto pImpl = etl::unique_ptr<Mqtt>(new PosixMqtt());
    if (pImpl)
    {
        return etl::optional(etl::move(pImpl));
    }
    return etl::nullopt;
}

void Mqtt::Init(Mqtt::MqttConfiguration &aConfiguration)
{
    auto &self   = *(static_cast<PosixMqtt *>(this));
    self.mClient = etl::unique_ptr<mqtt::client>(
        new mqtt::client(std::string(aConfiguration.serverUri.c_str()), std::string(aConfiguration.clientId.c_str())));
    tyLogInfo("posix.mqtt", "MQTT Client initialized");
}
} // namespace ty
