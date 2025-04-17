
// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

#include <etl/string.h>
#include <ty/instance.h>

#include "ty/net/connection.hpp"
#include "ty/net/network_interface.hpp"

#include "etl/memory.h"
#include "etl/optional.h"

#if CONFIG_TY_NET_MQTT_ENABLED

#ifndef _TY_MQTT_HPP_
#define _TY_MQTT_HPP_

namespace ty {
class Mqtt : public Connection
{
public:
    struct MqttConfiguration
    {
        const etl::istring &serverUri;
        const etl::istring &clientId;
    };

protected:
    Mqtt(const MqttConfiguration &aConfiguration)
        : mConfiguration(aConfiguration)
    {
    }

public:
    size_t printDriverInfo(Print &out) const;
    auto   connect() -> void;

public:
    static auto create(MqttConfiguration &) -> etl::unique_ptr<Mqtt>;
    static auto destroy(etl::unique_ptr<Connection> &) -> void;

private:
    const MqttConfiguration &mConfiguration;
};
} // namespace ty

#endif /* _TY_MQTT_HPP_ */
#endif /* CONFIG_ETH_ENABLED */
