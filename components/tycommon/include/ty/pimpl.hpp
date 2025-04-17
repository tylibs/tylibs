// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

#ifndef PIMPL_H_
#define PIMPL_H_
// pimpl.h

#include <etl/memory.h>
namespace ty {

template <typename T> class pimpl
{
private:
    etl::unique_ptr<T> impl;

public:
    pimpl();
    ~pimpl();

    template <typename... Args> pimpl(Args &&...args);

    T *operator->();
    T &operator*();
};

} // namespace ty

#endif // PIMPL_H_
