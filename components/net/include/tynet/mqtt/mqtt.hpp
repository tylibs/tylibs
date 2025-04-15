
#include <etl/string.h>
#include <ty/instance.h>

#include "tynet/connection.hpp"
#include "tynet/network_interface.hpp"

#include "etl/memory.h"
#include "etl/optional.h"

#if CONFIG_TYNET_MQTT_ENABLED

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

public:
    Mqtt();
    ~Mqtt();
    size_t printDriverInfo(Print &out) const;

public:
    void Init(MqttConfiguration &aConfiguration);

public:
    static auto create(tyInstance const &) -> etl::optional<etl::unique_ptr<Mqtt>>;
    static auto destroy(etl::unique_ptr<Connection> &) -> void;

private:
};
} // namespace ty

#endif /* _TY_MQTT_HPP_ */
#endif /* CONFIG_ETH_ENABLED */
