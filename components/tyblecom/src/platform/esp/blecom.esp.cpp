// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0
/**
 * @file
 *   This file implements the Tiny platform abstraction for non-volatile storage of settings.
 */

#include "etl/error_handler.h"
#include "ty/logging.h"
#include "ty/tycommon.h"

#include "blecom_p.hpp"
#include "ty/blecom/blecom.hpp"

#include <NimBLEDevice.h>

#include <etl/string.h>

namespace ty {
namespace ble {
// everything public is fine here...
struct BleComPlat : public BleCom
{
    explicit BleComPlat(BleCom::Configuration &aConfiguration)
        : BleCom(aConfiguration)
    {
    }
    NimBLEServer   *mServer;
    etl::string<31> mDeviceName;
};
static TY_DEFINE_ALIGNED_VAR(sBleComPlatRaw, sizeof(BleComPlat), uint64_t);
BleCom *BleCom::sBleCom = nullptr;

void BleCom::create(BleCom::Configuration &aConfiguration)
{
    BleCom::sBleCom = new (&sBleComPlatRaw) BleComPlat(aConfiguration);
    // auto pImpl = etl::unique_ptr<BleCom>(New<BleComPlat>(MALLOC_CAP_INTERNAL, aConfiguration));
}
void BleCom::start()
{
    auto &self = *(static_cast<BleComPlat *>(this));
    NimBLEDevice::init("NimBLE-Client");
    NimBLEDevice::setSecurityAuth(/*BLE_SM_PAIR_AUTHREQ_BOND | BLE_SM_PAIR_AUTHREQ_MITM |*/ BLE_SM_PAIR_AUTHREQ_SC);
    self.mServer                              = NimBLEDevice::createServer();
    NimBLEService        *pDeadService        = self.mServer->createService("DEAD");
    NimBLECharacteristic *pBeefCharacteristic = pDeadService->createCharacteristic(
        "BEEF", NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE |
                    /** Require a secure connection for read and write access */
                    NIMBLE_PROPERTY::READ_ENC | // only allow reading if paired / encrypted
                    NIMBLE_PROPERTY::WRITE_ENC  // only allow writing if paired / encrypted
    );

    pBeefCharacteristic->setValue("Burger");
    // pBeefCharacteristic->setCallbacks(&chrCallbacks);
    /** Create an advertising instance and add the services to the advertised data */
    NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();
    pAdvertising->setName(self.mConfiguration.name.c_str());
    pAdvertising->addServiceUUID(pDeadService->getUUID());
    /**
     *  If your device is battery powered you may consider setting scan response
     *  to false as it will extend battery life at the expense of less data sent.
     */
    pAdvertising->enableScanResponse(true);
    pAdvertising->start();
}
void BleCom::init()
{
    auto &self = *(static_cast<BleComPlat *>(this));
    self.d_func()->connect();
}

} // namespace ble
} // namespace ty
