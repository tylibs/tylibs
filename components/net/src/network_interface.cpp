#include "tynet/network_interface.hpp"

NetworkInterface::NetworkInterface()
    : _initial_bits(0)
    , _got_ip_event_id(-1)
    , _lost_ip_event_id(-1)
    , _interface_id(ESP_NETIF_ID_MAX)
{
}

NetworkInterface::~NetworkInterface()
{
    destroyNetif();
}

void NetworkInterface::destroyNetif() {}

size_t NetworkInterface::printTo(Print &out) const
{
    size_t bytes = 0;
    return bytes;
}
