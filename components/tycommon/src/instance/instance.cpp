// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

#include "ty/instance.h"
#include "instance.hpp"
#include "ty/common/as_core_type.hpp"
#include "ty/common/code_utils.hpp"
#include "ty/common/new.hpp"

/********************************************
 * C API
 *******************************************/
tyInstance *tyInstanceInitSingle(void)
{
    return &ty::Instance::InitSingle();
}
tyInstance *tyInstanceFinalize(void)
{
    return &ty::Instance::InitSingle();
}
void tyInstanceFinalize(tyInstance *aInstance)
{
    ty::AsCoreType(aInstance).Finalize();
}

// void tyInstanceReset(tyInstance *aInstance)
// {
//     ty::AsCoreType(aInstance).Reset();
// }

/********************************************
 * C++ Implementation
 *******************************************/
namespace ty {
// Define the raw storage used for Tiny instance (in single-instance case).
TY_DEFINE_ALIGNED_VAR(gInstanceRaw, sizeof(Instance), uint64_t);

Instance::Instance(void) {}

Instance &Instance::InitSingle(void)
{
    Instance *instance = &Get();

    VerifyOrExit(!instance->mIsInitialized);

    instance = new (&gInstanceRaw) Instance();

    instance->AfterInit();

exit:
    return *instance;
}

Instance &Instance::Get(void)
{
    void *instance = &gInstanceRaw;

    return *static_cast<Instance *>(instance);
}

void Instance::AfterInit(void)
{
    mIsInitialized = true;
}

void Instance::Finalize(void)
{
    VerifyOrExit(mIsInitialized);

    mIsInitialized = false;

    this->~Instance();

exit:
    return;
}
} // namespace ty
