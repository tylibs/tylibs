// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <sdbus-c++/sdbus-c++.h>

namespace org {
namespace bluez {

struct Util
{
    static std::tuple<uint16_t /* real */, uint16_t /* usable */> getMTUFromOptions(
        const std::map<std::string, sdbus::Variant> &options);
    static sdbus::ObjectPath getObjectPathFromOptions(const std::map<std::string, sdbus::Variant> &options);
    static std::string       getDeviceMAC(const sdbus::ObjectPath &path);
    static std::string       optionsListToString(const std::map<std::string, sdbus::Variant> &options);
};

} // namespace bluez
} // namespace org
