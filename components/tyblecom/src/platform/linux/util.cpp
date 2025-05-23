// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

#include "util.hpp"
#include <algorithm>

namespace org {
namespace bluez {

std::tuple<uint16_t /* real */, uint16_t /* usable */> Util::getMTUFromOptions(
    const std::map<std::string, sdbus::Variant> &options)
{
    auto deviceIter = options.find("mtu");
    if (deviceIter != options.end())
    {
        constexpr uint16_t ATT_MTU = 251;
        uint16_t           realMtu = deviceIter->second.get<uint16_t>();
        uint16_t           usableMtu;
        if (realMtu > ATT_MTU)
        {
            usableMtu =
                realMtu - ((realMtu / ATT_MTU) * 10) /* return usable MTU, subtract 10 byte overhead per ATT packet */;
        }
        else
        {
            usableMtu = realMtu - 3;
        }
        return std::make_tuple(realMtu, usableMtu);
    }
    else
    {
        return std::make_tuple(uint16_t(0), uint16_t(0));
    }
}

sdbus::ObjectPath Util::getObjectPathFromOptions(const std::map<std::string, sdbus::Variant> &options)
{
    auto deviceIter = options.find("device");
    if (deviceIter != options.end())
    {
        return deviceIter->second.get<sdbus::ObjectPath>();
    }
    else
    {
        return sdbus::ObjectPath{""};
    }
}

std::string Util::getDeviceMAC(const sdbus::ObjectPath &path)
{
    std::string mac;
    size_t      pLen = path.size();

    if (pLen > 17 && path.at(pLen - 18) == '_')
    {
        mac = path.substr(pLen - 17);
        std::replace(mac.begin(), mac.end(), '_', ':');
    }

    return mac;
}

std::string Util::optionsListToString(const std::map<std::string, sdbus::Variant> &options)
{
    std::string res;

    for (auto iter{options.cbegin()}; iter != options.cend(); iter++)
    {
        if (iter->first == "device")
        {
            res.append("device=").append(Util::getDeviceMAC(iter->second.get<sdbus::ObjectPath>())).append(", ");
        }
        else if (iter->first == "mtu")
        {
            res.append("mtu=").append(std::to_string(iter->second.get<uint16_t>())).append(", ");
        }
        else if (iter->first == "link")
        {
            res.append("link=").append(iter->second.get<std::string>()).append(", ");
        }
        else
        {
            res.append("'").append(iter->first).append("',");
        }
    }

    return res;
}

} // namespace bluez
} // namespace org
