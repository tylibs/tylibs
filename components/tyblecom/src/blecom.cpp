// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

#include <ty/blecom/blecom.hpp>

namespace ty {
namespace ble {
class BleCom::impl
{
};
BleCom::BleCom(const Configuration &aConfiguration)
    : mConfiguration(aConfiguration)
    , pimpl(new impl)
{
}
BleCom::~BleCom()
{
    delete pimpl;
}
} // namespace ble
} // namespace ty
