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
#include "object_manager.hpp"
#include "read_only_characteristic.hpp"
#include "serial_characteristic.hpp"

constexpr const char *APP_PATH = "/org/bluez/byg";
constexpr const char *ADV_PATH = "/org/bluez/byg/advertisement1";

constexpr const char *TAG = "BLECOM-LINUX";

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
    std::shared_ptr<sdbus::IConnection> aConnection;
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
    auto &self = *(static_cast<BleComPlat *>(this));

    std::shared_ptr<IConnection> connection{std::move(sdbus::createSystemBusConnection())};
    connection->requestName(sdbus::ServiceName{"org.bluez.byg"});
    // auto objectManager = std::make_shared<sdbus::ObjectManagerProxy>(*connection, connection->getUniqueName(),
    //                                                                  ObjectPath{"/"});

    // self.mConnection = std::make_shared<sdbus::IConnection>(sdbus::createSystemBusConnection());
    {
        org::bluez::Adapter1 adapter1{*connection, sdbus::ServiceName{BLUEZ_SERVICE}, sdbus::ObjectPath{DEVICE0}};

        adapter1.Powered(true);
        adapter1.Discoverable(true);
        adapter1.Pairable(true);
        adapter1.Alias(self.mConfiguration.name.c_str());
    }
    // ---- Services ---------------------------------------------------------------------------------------------------
    const org::bluez::GattManager1 gattMgr{connection, sdbus::ServiceName{BLUEZ_SERVICE}, sdbus::ObjectPath{DEVICE0}};
    auto       app = std::make_shared<org::bluez::GattApplication1>(connection, sdbus::ObjectPath{APP_PATH});
    const auto srv2 =
        std::make_shared<org::bluez::GattService1>(app, "service0", "6e400001-b5a3-f393-e0a9-e50e24dcca9e");
    org::bluez::SerialCharacteristic::create(srv2, connection, "6e400002-b5a3-f393-e0a9-e50e24dcca9e").finalize();
    org::bluez::SerialCharacteristic::create(srv2, connection, "6e400003-b5a3-f393-e0a9-e50e24dcca9e").finalize();

    auto register_app_callback = [](std::optional<sdbus::Error> error) {
        tyLogInfo(TAG, "Registering app callback %s", ADV_PATH);
        if (!error.has_value())
        {
            tyLogCrit(TAG, "Application registered");
        }
        else
        {
            tyLogCrit(TAG, "Error registering application %s with message %s", error->getName().c_str(),
                      error->getMessage().c_str());
        }
    };

    tyLogInfo(TAG, "Registering application %s", app->getPath().c_str());
    gattMgr.RegisterApplicationAsync(app->getPath(), {}).uponReplyInvoke(register_app_callback);
    tyLogInfo(TAG, "done");

    // ---- Advertising ------------------------------------------------------------------------------------------------

    const auto mgr = std::make_shared<org::bluez::LEAdvertisingManager1>(*connection, sdbus::ServiceName{BLUEZ_SERVICE},
                                                                         sdbus::ObjectPath{DEVICE0});
    tyLogInfo(TAG, "LEAdvertisingManager1");
    tyLogInfo(TAG, "  ActiveInstances: %d", mgr->ActiveInstances());
    tyLogInfo(TAG, "  SupportedInstances: %d", mgr->SupportedInstances());

    auto register_adv_callback = [](std::optional<sdbus::Error> error) -> void {
        tyLogInfo(TAG, "Registering advertisement callback %s", ADV_PATH);
        if (!error.has_value())
        {
            tyLogCrit(TAG, "Advertisement registered");
        }
        else
        {
            tyLogCrit(TAG, "Error registering advertisement %s with message %s", error->getName().c_str(),
                      error->getMessage().c_str());
        }
    };

    auto ad = org::bluez::LEAdvertisement1::create(*connection, sdbus::ObjectPath{ADV_PATH})
                  .withLocalName(self.mConfiguration.name.c_str())
                  .withServiceUUIDs(std::vector{std::string{"6e400001-b5a3-f393-e0a9-e50e24dcca9e"}})
                  .withIncludes(std::vector{std::string{"tx-power"}})
                  .onReleaseCall([]() { tyLogInfo(TAG, "advertisement released"); })
                  .registerWith(mgr, register_adv_callback);
    // Add an objectManager interface at the root of the service
    // objectManager->handleInterfaces();
    connection->enterEventLoop();
    // self.mConnection->enterProcessingLoopAsync();
}

void BleCom::init()
{
    auto &self = *(static_cast<BleComPlat *>(this));
    self.d_func()->connect();
}
} // namespace ble
} // namespace ty
