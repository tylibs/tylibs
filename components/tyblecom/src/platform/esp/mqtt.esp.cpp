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
};

Mqtt::Mqtt() {}

Mqtt::~Mqtt() {}
auto Mqtt::create(tyInstance const &aInstance) -> etl::optional<etl::unique_ptr<Mqtt>>
{
    auto pImpl = etl::unique_ptr<Mqtt>(new EspMqtt());
    if (pImpl)
    {
        return etl::optional(etl::move(pImpl));
    }
    return etl::nullopt;
}

void Mqtt::Init(Mqtt::MqttConfiguration &aConfiguration)
{
    // auto &self = *(static_cast<EspMqtt *>(this));
    tyLogInfo("esp.mqtt", "ESP MQTT Client initialized");
}
} // namespace ty
