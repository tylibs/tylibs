// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

#ifndef TY_BLECOM_HPP_
#define TY_BLECOM_HPP_

#include <etl/exception.h>
#include <etl/memory.h>
#include <etl/string.h>

#include "ty/pimpl.hpp"
#include "ty/tycommon.h"

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
    static void    create(Configuration &);
    void           start();
    void           init();
    static BleCom &get(void)
    {
        TY_ASSERT(sBleCom != nullptr);
        return *sBleCom;
    }

protected:
    static BleCom *sBleCom;

private:
    const Configuration &mConfiguration;
};

//***************************************************************************
/// Base exception class for BLE communication.
//***************************************************************************
class BleComException : public etl::exception
{
public:
    BleComException(string_type reason_, string_type file_name_, numeric_type line_number_)
        : etl::exception(reason_, file_name_, line_number_)
    {
    }
};

//***************************************************************************
/// Exception for BLE controller errors.
//***************************************************************************
class BleComControllerException : public ty::ble::BleComException
{
public:
    BleComControllerException(string_type file_name_, numeric_type line_number_)
        : ty::ble::BleComException(ETL_ERROR_TEXT("bt controller", "BLE1"), file_name_, line_number_)
    {
    }
};

} // namespace ble
} // namespace ty

#endif /* CONFIG_TY_BLECOM */
#endif // TY_BLECOM_HPP_
