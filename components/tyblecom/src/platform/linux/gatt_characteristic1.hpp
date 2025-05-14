// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gatt_descriptor1.hpp"
#include "properties_ext_adaptor.hpp"
#include "tyblecom-adaptor-glue.h"
#include "tyblecom-proxy-glue.h"
#include <sdbus-c++/Types.h>
#include <sdbus-c++/sdbus-c++.h>

namespace org::bluez {
// Forward declarations
class GattService1;

using namespace sdbus;

class GattCharacteristic1 : public AdaptorInterfaces<GattCharacteristic1_adaptor, PropertiesExt_adaptor>,
                            public std::enable_shared_from_this<GattCharacteristic1>
{
    static constexpr auto INTERFACE_NAME = "org.bluez.GattCharacteristic1";

public:
    GattCharacteristic1(const std::shared_ptr<GattService1> &service,
                        std::string                          uuid,
                        bool                                 hasAcquireWrite  = false,
                        bool                                 hasAcquireNotify = false,
                        bool                                 hasValue         = false,
                        bool                                 valueIsDirected  = false);

    ~GattCharacteristic1() override;

    void addValue(std::string value);

    void addValue(const std::vector<uint8_t> &value);

    void addFlag(std::string flag);

protected:
    friend GattDescriptor1;
    friend GattService1;

    virtual void addDescriptor(std::shared_ptr<GattDescriptor1> descriptor);

    virtual void removeDescriptor(const std::shared_ptr<GattDescriptor1> &descriptor);

    const std::string &getPath() const;

    std::shared_ptr<IConnection> getConnection() const;

    /**
     * Issues a request to read the value of the
     * characteristic and returns the value if the
     * operation was successful.
     *
     * Possible options: "offset": uint16 offset
     *          "device": Object Device (Server only)
     *
     * Possible Errors: org.bluez.Error.Failed
     *          org.bluez.Error.InProgress
     *          org.bluez.Error.NotPermitted
     *          org.bluez.Error.NotAuthorized
     *          org.bluez.Error.InvalidOffset
     *          org.bluez.Error.NotSupported
     *
     */
    std::vector<uint8_t> ReadValue(const std::map<std::string, Variant> &options) override;

    /**
     * Issues a request to write the value of the
     * characteristic.
     *
     * Possible options: "offset": Start offset
     *             "device": Device path (Server only)
     *             "link": Link type (Server only)
     *             "prepare-authorize": boolean Is prepare
     *                         authorization
     *                         request
     *
     * Possible Errors: org.bluez.Error.Failed
     *             org.bluez.Error.InProgress
     *             org.bluez.Error.NotPermitted
     *             org.bluez.Error.InvalidValueLength
     *             org.bluez.Error.NotAuthorized
     *             org.bluez.Error.NotSupported
     */
    void WriteValue(const std::vector<uint8_t> &value, const std::map<std::string, Variant> &options) override;

    /**
     * Acquire file descriptor and MTU for writing. Usage of
     * WriteValue will be locked causing it to return
     * NotPermitted error.
     *
     * For server the MTU returned shall be equal or smaller
     * than the negotiated MTU.
     *
     * For client, it only works with characteristic that has
     * WriteAcquired property which relies on
     * write-without-response Flag.
     *
     * To release the lock the client shall close the file
     * descriptor, a HUP is generated in case the device
     * is disconnected.
     *
     * Note: the MTU can only be negotiated once and is
     * symmetric therefore this method may be delayed in
     * order to have the exchange MTU completed, because of
     * that the file descriptor is closed during
     * reconnections as the MTU has to be renegotiated.
     *
     * Possible options: "device": Object Device (Server only)
     *                   "MTU": Exchanged MTU (Server only)
     *                   "link": Link type (Server only)
     *
     * Possible Errors: org.bluez.Error.Failed
     *                  org.bluez.Error.NotSupported
     *
     * @param options
     */
    std::tuple<UnixFd, uint16_t> AcquireWrite(const std::map<std::string, Variant> &options) override;

    /**
     * Acquire file descriptor and MTU for notify. Usage of
     * StartNotify will be locked causing it to return
     * NotPermitted error.
     *
     * For server the MTU returned shall be equal or smaller
     * than the negotiated MTU.
     *
     * Only works with characteristic that has NotifyAcquired
     * which relies on notify Flag and no other client have
     * called StartNotify.
     *
     * Notification are enabled during this procedure so
     * StartNotify shall not be called, any notification
     * will be dispatched via file descriptor therefore the
     * Value property is not affected during the time where
     * notify has been acquired.
     *
     * To release the lock the client shall close the file
     * descriptor, a HUP is generated in case the device
     * is disconnected.
     *
     * Note: the MTU can only be negotiated once and is
     * symmetric therefore this method may be delayed in
     * order to have the exchange MTU completed, because of
     * that the file descriptor is closed during
     * reconnections as the MTU has to be renegotiated.
     *
     * Possible options: "device": Object Device (Server only)
     *                   "MTU": Exchanged MTU (Server only)
     *                   "link": Link type (Server only)
     *
     * Possible Errors: org.bluez.Error.Failed
     *                  org.bluez.Error.NotSupported
     *
     */
    std::tuple<UnixFd, uint16_t> AcquireNotify(const std::map<std::string, Variant> &options) override;

    /**
     * Starts a notification session from this characteristic
     * if it supports value notifications or indications.
     *
     * Possible Errors: org.bluez.Error.Failed
     *             org.bluez.Error.NotPermitted
     *             org.bluez.Error.InProgress
     *             org.bluez.Error.NotSupported
     *
     */
    // void StartNotify(const std::map<std::string, Variant> &options) override;
    void StartNotify() override;

    /**
     * This method will cancel any previous StartNotify
     * transaction. Note that notifications from a
     * characteristic are shared between sessions thus
     * calling StopNotify will release a single session.
     *
     * Possible Errors: org.bluez.Error.Failed
     */
    // void StopNotify(const std::map<std::string, Variant> &options) override;
    void StopNotify() override;

    /**
     * This method doesn't expect a reply so it is just a
     * confirmation that value was received.
     *
     * Possible Errors: org.bluez.Error.Failed
     */
    void Confirm() override;

    std::string UUID() override { return uuid_; }

    ObjectPath Service() override;

    std::vector<uint8_t> Value() override { return value_; }

    std::map<sdbus::UnixFd, std::vector<uint8_t>> DirectedValue() override;
    // std::map<ObjectPath, std::vector<std::vector<uint8_t>>> DirectedValue() override;

    bool                     Notifying() override { return notifyingSessions_ > 0; }
    std::vector<std::string> Flags() override { return flags_; }
    std::vector<ObjectPath>  Descriptors() override { return includes_; }
    bool                     WriteAcquired() override { return writeAcquired_; }
    bool                     NotifyAcquired() override { return notifyAcquired_; }

    void emitPropertyChangedSignal(const sdbus::PropertyName &property);

    void registerWithService(const std::shared_ptr<GattCharacteristic1> &self);

    std::map<std::string, Variant> dict_;
    std::shared_ptr<GattService1>  service_;
    sdbus::ObjectPath              path_;
    std::string                    uuid_;
    std::vector<uint8_t>           value_;
    int                            notifyingSessions_;
    std::vector<std::string>       flags_;
    bool                           writeAcquired_;
    bool                           notifyAcquired_;

    std::vector<std::shared_ptr<GattDescriptor1>> descriptors_;
    std::vector<ObjectPath>                       includes_;
};
} // namespace org::bluez
