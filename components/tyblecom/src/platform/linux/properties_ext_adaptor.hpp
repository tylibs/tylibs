// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <sdbus-c++/sdbus-c++.h>

using namespace sdbus;

class PropertiesExt_adaptor : public Properties_adaptor
{
public:
    explicit PropertiesExt_adaptor(IObject &object)
        : Properties_adaptor(object)
    {
    }

    virtual ~PropertiesExt_adaptor() = default;

protected:
    void emitPropertyChangedSignal(const std::string &interfaceName, const PropertyName &property)
    {
        emitPropertiesChangedSignal(InterfaceName{interfaceName}, std::vector{property});
    }
};
