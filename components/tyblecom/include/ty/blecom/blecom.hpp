// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

#ifndef TY_BLECOM_HPP_
#define TY_BLECOM_HPP_

#include "tyconfig.h"
#include <etl/string.h>

#include "etl/memory.h"
#include "ty/pimpl.hpp"
#if CONFIG_TY_BLECOM

namespace ty {
namespace ble {
class BleComPrivate;
class BleCom : public Pimpl<BleCom, BleComPrivate>
{
public:
    struct Configuration
    {
        const etl::string<31> &name;
    };
    // delete default and copy constructor
    BleCom()                    = delete;
    BleCom(const BleCom &other) = delete;
    // needed by Pimpl
    ~BleCom();

protected:
    explicit BleCom(const Configuration &aConfiguration);

public:
    [[nodiscard]] static auto create(Configuration &) -> etl::unique_ptr<BleCom>;
    static auto               destroy(etl::unique_ptr<BleCom> &) -> void;

    void init();

private:
    const Configuration &mConfiguration;
};
} // namespace ble
} // namespace ty

#endif /* CONFIG_TY_BLECOM */
#endif // TY_BLECOM_HPP_
