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

#include <etl/string.h>

#include "esp_nimble_hci.h"
#include "nvs_flash.h"
#include <freertos/FreeRTOS.h>
#include "host/ble_hs.h"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "services/gap/ble_svc_gap.h"
#include "services/gatt/ble_svc_gatt.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define B0(x) ((x) & 0xFF)
#define B1(x) (((x) >> 8) & 0xFF)
#define B2(x) (((x) >> 16) & 0xFF)
#define B3(x) (((x) >> 24) & 0xFF)
#define B4(x) (((x) >> 32) & 0xFF)
#define B5(x) (((x) >> 40) & 0xFF)

// clang-format off
#define UUID128_CONST(a32, b16, c16, d16, e48) \
  BLE_UUID128_INIT( \
    B0(e48), B1(e48), B2(e48), B3(e48), B4(e48), B5(e48), \
    B0(d16), B1(d16), B0(c16), B1(c16), B0(b16), \
    B1(b16), B0(a32), B1(a32), B2(a32), B3(a32), \
  )
// clang-format off

static const ble_uuid128_t SERVICE_UUID = UUID128_CONST(0x6E400001, 0xB5A3, 0xF393, 0xE0A9, 0xE50E24DCCA9E);
static const ble_uuid128_t CHAR_UUID_RX = UUID128_CONST(0x6E400002, 0xB5A3, 0xF393, 0xE0A9, 0xE50E24DCCA9E);
static const ble_uuid128_t CHAR_UUID_TX = UUID128_CONST(0x6E400003, 0xB5A3, 0xF393, 0xE0A9, 0xE50E24DCCA9E);
static uint16_t ble_conn_hdl;
static uint16_t notify_char_attr_hdl;

// Enum for Nordic UART callback types
enum nordic_uart_callback_type {
  NORDIC_UART_DISCONNECTED, // Callback type when disconnected
  NORDIC_UART_CONNECTED,    // Callback type when connected
};
// Type definition for UART receive callback function
typedef void (*uart_receive_callback_t)(struct ble_gatt_access_ctxt *ctxt);
static void (*_nordic_uart_callback)(enum nordic_uart_callback_type callback_type) = NULL;

static uart_receive_callback_t _uart_receive_callback = NULL;
    esp_err_t nordic_uart_yield(uart_receive_callback_t uart_receive_callback) {
  _uart_receive_callback = uart_receive_callback;
  return ESP_OK;
}

static int _uart_receive(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg) {
  if (_uart_receive_callback) {
    _uart_receive_callback(ctxt);
  } else {
    // for (int i = 0; i < ctxt->om->om_len; ++i) {
    //   const char c = ctxt->om->om_data[i];
    //   _nordic_uart_linebuf_append(c);
    // }
  }
  return 0;
}

// notify GATT callback is no operation.
static int _uart_noop(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg) {
  return 0;
}

namespace ty {
namespace ble {
// everything public is fine here...
struct BleComPlat : public BleCom
{
    explicit BleComPlat(BleCom::Configuration &aConfiguration)
        : BleCom(aConfiguration)
    {
    }
    static void host_task(void* param);
    etl::string<31> mDeviceName;
    ble_gatt_svc_def mSvcDef[2]{ };
    ble_gatt_chr_def mChrs[3]{ };
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
    auto &self       = *(static_cast<BleComPlat *>(this));
    self.mDeviceName = self.mConfiguration.name;
    self.mSvcDef[0].type = BLE_GATT_SVC_TYPE_PRIMARY;
    self.mSvcDef[0].uuid  = (ble_uuid_t *)&SERVICE_UUID.u;
    self.mChrs[0].uuid      = (ble_uuid_t *)&CHAR_UUID_RX;
    self.mChrs[0].flags     = BLE_GATT_CHR_F_WRITE | BLE_GATT_CHR_F_WRITE_NO_RSP;
    self.mChrs[0].access_cb = _uart_receive;
    self.mChrs[1].uuid      = (ble_uuid_t *)&CHAR_UUID_TX;
    self.mChrs[1].flags     = BLE_GATT_CHR_F_NOTIFY;
    self.mChrs[1].val_handle = &notify_char_attr_hdl;
    self.mChrs[1].access_cb  = _uart_noop;
    self.mChrs[2].uuid      = NULL;
    self.mChrs[2].flags     = 0;
    self.mChrs[2].access_cb = NULL;
    self.mSvcDef[0].characteristics = self.mChrs;
    self.mSvcDef[1].type = 0; // End of the array
                              //
    // Initialize NimBLE
    esp_err_t ret = nimble_port_init();
    if (ret != ESP_OK)
    {
        ETL_ERROR(ty::ble::BleComControllerException);
    }

    // Initialize the NimBLE Host configuration
    // Bluetooth device name for advertisement
    ble_svc_gap_device_name_set(self.mDeviceName.c_str());
    ble_svc_gap_init();
    ble_svc_gatt_init();

    ble_gatts_count_cfg(self.mSvcDef);
    ble_gatts_add_svcs(self.mSvcDef);

    // ble_hs_cfg.sync_cb = ble_app_on_sync_cb;

    // Create NimBLE thread
    nimble_port_freertos_init(BleComPlat::host_task);
}
void BleCom::init()
{
    auto &self = *(static_cast<BleComPlat *>(this));
    self.d_func()->connect();
}
/**
 * @brief The main host task.
 */
void BleComPlat::host_task(void* param) {
    tyLogInfo("blecom:esp", "BLE Host Task Started");
    nimble_port_run(); // This function will return only when nimble_port_stop() is executed
    nimble_port_freertos_deinit();
    tyLogInfo("blecom:esp", "BLE Host Task Stopped");
} // host_task

} // namespace ble
} // namespace ty
