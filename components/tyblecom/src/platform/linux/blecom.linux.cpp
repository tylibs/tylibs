// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0
/**
 * @file
 *   This file implements the Tiny platform abstraction for non-volatile storage of settings.
 */
#include "ty/logging.h"
#include "ty/tycommon.h"

#include "blecom_p.hpp"
#include "ty/blecom/blecom.hpp"

#include <memory>
#include <sdbus-c++/IConnection.h>
#include <sdbus-c++/Types.h>
#include <sdbus-c++/sdbus-c++.h>

#include "adapter1.hpp"
#include "gatt_application1.hpp"
#include "gatt_characteristic1.hpp"
#include "gatt_manager1.hpp"
#include "gatt_service1.hpp"
#include "le_advertisement1.hpp"
#include "le_advertising_manager1.hpp"
#include "read_only_characteristic.hpp"

constexpr const char *APP_PATH = "/org/bluez/example";
constexpr const char *ADV_PATH = "/org/bluez/example/advertisement1";

constexpr const char *NAME = "ExampleBlueZ";
constexpr const char *TAG  = "BLECOM-LINUX";

constexpr auto BLUEZ_SERVICE = "org.bluez";
constexpr auto DEVICE0       = "/org/bluez/hci0";

namespace ty {
namespace ble {
// everything public is fine here...
struct BleComPlat : public BleCom
{
    explicit BleComPlat(BleCom::Configuration &aConfiguration)
        : BleCom(aConfiguration)
    {
    }
    std::shared_ptr<sdbus::IConnection> mConnection;
};
static TY_DEFINE_ALIGNED_VAR(sBleComPlatRaw, sizeof(BleComPlat), uint64_t);
BleCom *BleCom::sBleCom = nullptr;

void BleCom::create(BleCom::Configuration &aConfiguration)
{
    // BleCom::sBleCom = new BleComPlat(aConfiguration);
    BleCom::sBleCom = new (&sBleComPlatRaw) BleComPlat(aConfiguration);
    // auto pImpl = etl::unique_ptr<BleCom>(New<BleComPlat>(MALLOC_CAP_INTERNAL, aConfiguration));
}

void BleCom::start()
{
    auto &self       = *(static_cast<BleComPlat *>(this));
    self.mConnection = sdbus::createSystemBusConnection();
    {
        org::bluez::Adapter1 adapter1{*self.mConnection, sdbus::ServiceName{BLUEZ_SERVICE}, sdbus::ObjectPath{DEVICE0}};

        adapter1.Powered(true);
        adapter1.Discoverable(true);
        adapter1.Pairable(true);
        adapter1.Alias(NAME);
    }
    // ---- Services ---------------------------------------------------------------------------------------------------
    const org::bluez::GattManager1 gattMgr{self.mConnection, sdbus::ServiceName{BLUEZ_SERVICE},
                                           sdbus::ObjectPath{DEVICE0}};
    auto       app  = std::make_shared<org::bluez::GattApplication1>(self.mConnection, sdbus::ObjectPath{APP_PATH});
    const auto srv1 = std::make_shared<org::bluez::GattService1>(app, "deviceinfo", "180A");
    org::bluez::ReadOnlyCharacteristic::createFinal(srv1, "2A24", NAME);               // model name
    org::bluez::ReadOnlyCharacteristic::createFinal(srv1, "2A25", "333-12345678-888"); // serial number
    org::bluez::ReadOnlyCharacteristic::createFinal(srv1, "2A26", "1.0.1");            // fw rev
    org::bluez::ReadOnlyCharacteristic::createFinal(srv1, "2A27", "rev A");            // hw rev
    org::bluez::ReadOnlyCharacteristic::createFinal(srv1, "2A28", "5.0");              // sw rev
    org::bluez::ReadOnlyCharacteristic::createFinal(srv1, "2A29", "ACME Inc.");        // manufacturer

    const auto srv2 = std::make_shared<org::bluez::GattService1>(app, "serial", "368a3edf-514e-4f70-ba8f-2d0a5a62bc8c");
    org::bluez::SerialCharacteristic::create(srv2, self.mConnection, "de0a7b0c-358f-4cef-b778-8fe9abf09d53").finalize();

    auto register_app_callback = [](const std::optional<sdbus::Error> error) {
        if (!error.has_value())
        {
            tyLogInfo(TAG, "Application registered");
        }
        else
        {
            tyLogInfo(TAG, "Error registering application %s with message %s", error->getName().c_str(),
                      error->getMessage().c_str());
        }
    };

    gattMgr.RegisterApplicationAsync(app->getPath(), {}).uponReplyInvoke(register_app_callback);

    // ---- Advertising ------------------------------------------------------------------------------------------------

    const auto mgr = std::make_shared<org::bluez::LEAdvertisingManager1>(
        *self.mConnection, sdbus::ServiceName{BLUEZ_SERVICE}, sdbus::ObjectPath{DEVICE0});
    tyLogInfo(TAG, "LEAdvertisingManager1");
    tyLogInfo(TAG, "  ActiveInstances: %d", mgr->ActiveInstances());
    tyLogInfo(TAG, "  SupportedInstances: %d", mgr->SupportedInstances());

    auto register_adv_callback = [](const std::optional<sdbus::Error> error) -> void {
        if (!error.has_value())
        {
            tyLogInfo(TAG, "Advertisement registered");
        }
        else
        {
            tyLogCrit(TAG, "Error registering advertisement %s with message %s", error->getName().c_str(),
                      error->getMessage().c_str());
        }
    };

    auto ad = org::bluez::LEAdvertisement1::create(*self.mConnection, sdbus::ObjectPath{ADV_PATH})
                  .withLocalName(NAME)
                  .withServiceUUIDs(std::vector{std::string{"368a3edf-514e-4f70-ba8f-2d0a5a62bc8c"}})
                  .withIncludes(std::vector{std::string{"tx-power"}})
                  .onReleaseCall([]() { tyLogInfo(TAG, "advertisement released"); })
                  .registerWith(mgr, register_adv_callback);
}

void BleCom::init()
{
    auto &self = *(static_cast<BleComPlat *>(this));
    self.d_func()->connect();
}
} // namespace ble
} // namespace ty
