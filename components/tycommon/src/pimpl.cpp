// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

#include "ty/pimpl.hpp"

namespace ty {

template <typename T>
pimpl<T>::pimpl()
    : impl{new T{}}
{
}

template <typename T> pimpl<T>::~pimpl() {}

template <typename T>
template <typename... Args>
pimpl<T>::pimpl(Args &&...args)
    : impl{new T{std::forward<Args>(args)...}}
{
}

template <typename T> T *pimpl<T>::operator->()
{
    return impl.get();
}

template <typename T> T &pimpl<T>::operator*()
{
    return *impl.get();
}
} // namespace ty
