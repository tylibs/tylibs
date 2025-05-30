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
    NimBLEServer *mServer;
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
    NimBLEDevice::init(self.mConfiguration.name);
    NimBLEDevice::setSecurityAuth(/*BLE_SM_PAIR_AUTHREQ_BOND | BLE_SM_PAIR_AUTHREQ_MITM |*/ BLE_SM_PAIR_AUTHREQ_SC);
    self.mServer                = NimBLEDevice::createServer();
    NimBLEService *pDeadService = self.mServer->createService(etl::string<36>{"6e400001-b5a3-f393-e0a9-e50e24dcca9e"});
    NimBLECharacteristic *pRxCharacteristic = pDeadService->createCharacteristic(
        etl::string<36>{"6e400002-b5a3-f393-e0a9-e50e24dcca9e"}, NIMBLE_PROPERTY::READ
        /** Require a secure connection for read and write access */
        // NIMBLE_PROPERTY::READ_ENC | // only allow reading if paired / encrypted
        // NIMBLE_PROPERTY::WRITE_ENC  // only allow writing if paired / encrypted
    );
    NimBLECharacteristic *pTxCharacteristic = pDeadService->createCharacteristic(
        etl::string<36>{"6e400003-b5a3-f393-e0a9-e50e24dcca9e"}, NIMBLE_PROPERTY::WRITE);
    tyLogInfo("core", "BLE Char created %s", pTxCharacteristic->toString().c_str());
    tyLogInfo("core", "BLE Char created %s", pRxCharacteristic->toString().c_str());

    pRxCharacteristic->setValue("Hello");
    pDeadService->start();
    tyLogInfo("core", "BLE Service started: %s", pDeadService->toString().c_str());
    // pRxCharacteristic->setCallbacks(&chrCallbacks);
    /** Create an advertising instance and add the services to the advertised data */
    NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();
    pAdvertising->setName(self.mConfiguration.name);
    pAdvertising->addServiceUUID(pDeadService->getUUID());
    /**
     *  If your device is battery powered you may consider setting scan response
     *  to false as it will extend battery life at the expense of less data sent.
     */
    pAdvertising->enableScanResponse(true);
    pAdvertising->start();
    tyLogInfo("core", "BLE Adv started: %s", pAdvertising->getAdvertisementData().toString().c_str());
}
void BleCom::init()
{
    auto &self = *(static_cast<BleComPlat *>(this));
    self.d_func()->connect();
}

} // namespace ble
} // namespace ty
