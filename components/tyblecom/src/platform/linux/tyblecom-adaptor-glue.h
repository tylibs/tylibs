
// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

/*
 * This file was automatically generated by sdbus-c++-xml2cpp; DO NOT EDIT!
 */

#ifndef __sdbuscpp___home_globi__dev_clever_components_tylibs_components_tyblecom_src_platform_linux_tyblecom_adaptor_glue_h__adaptor__H__
#define __sdbuscpp___home_globi__dev_clever_components_tylibs_components_tyblecom_src_platform_linux_tyblecom_adaptor_glue_h__adaptor__H__

#include <sdbus-c++/sdbus-c++.h>
#include <string>
#include <tuple>

namespace org {
namespace bluez {

class GattService1_adaptor
{
public:
    static constexpr const char *INTERFACE_NAME = "org.bluez.GattService1";

protected:
    GattService1_adaptor(sdbus::IObject &object)
        : m_object(object)
    {
    }

    GattService1_adaptor(const GattService1_adaptor &)            = delete;
    GattService1_adaptor &operator=(const GattService1_adaptor &) = delete;
    GattService1_adaptor(GattService1_adaptor &&)                 = delete;
    GattService1_adaptor &operator=(GattService1_adaptor &&)      = delete;

    ~GattService1_adaptor() = default;

    void registerAdaptor()
    {
        m_object
            .addVTable(sdbus::registerProperty("UUID").withGetter([this]() { return this->UUID(); }),
                       sdbus::registerProperty("Primary").withGetter([this]() { return this->Primary(); }),
                       sdbus::registerProperty("Includes").withGetter([this]() { return this->Includes(); }))
            .forInterface(INTERFACE_NAME);
    }

private:
    virtual std::string                    UUID()     = 0;
    virtual bool                           Primary()  = 0;
    virtual std::vector<sdbus::ObjectPath> Includes() = 0;

private:
    sdbus::IObject &m_object;
};

} // namespace bluez
} // namespace org

namespace org {
namespace bluez {

class GattDescriptor1_adaptor
{
public:
    static constexpr const char *INTERFACE_NAME = "org.bluez.GattDescriptor1";

protected:
    GattDescriptor1_adaptor(sdbus::IObject &object)
        : m_object(object)
    {
    }

    GattDescriptor1_adaptor(const GattDescriptor1_adaptor &)            = delete;
    GattDescriptor1_adaptor &operator=(const GattDescriptor1_adaptor &) = delete;
    GattDescriptor1_adaptor(GattDescriptor1_adaptor &&)                 = delete;
    GattDescriptor1_adaptor &operator=(GattDescriptor1_adaptor &&)      = delete;

    ~GattDescriptor1_adaptor() = default;

    void registerAdaptor()
    {
        m_object
            .addVTable(
                sdbus::registerMethod("ReadValue")
                    .withInputParamNames("options")
                    .withOutputParamNames("value")
                    .implementedAs([this](const std::map<std::string, sdbus::Variant> &options) {
                        return this->ReadValue(options);
                    }),
                sdbus::registerMethod("WriteValue")
                    .withInputParamNames("value", "options")
                    .implementedAs([this](const std::vector<uint8_t>                  &value,
                                          const std::map<std::string, sdbus::Variant> &options) {
                        return this->WriteValue(value, options);
                    }),
                sdbus::registerProperty("UUID").withGetter([this]() { return this->UUID(); }),
                sdbus::registerProperty("Characteristic").withGetter([this]() { return this->Characteristic(); }),
                sdbus::registerProperty("Value").withGetter([this]() { return this->Value(); }),
                sdbus::registerProperty("Flags").withGetter([this]() { return this->Flags(); }))
            .forInterface(INTERFACE_NAME);
    }

private:
    virtual std::vector<uint8_t> ReadValue(const std::map<std::string, sdbus::Variant> &options)  = 0;
    virtual void                 WriteValue(const std::vector<uint8_t>                  &value,
                                            const std::map<std::string, sdbus::Variant> &options) = 0;

private:
    virtual std::string              UUID()           = 0;
    virtual sdbus::ObjectPath        Characteristic() = 0;
    virtual std::vector<uint8_t>     Value()          = 0;
    virtual std::vector<std::string> Flags()          = 0;

private:
    sdbus::IObject &m_object;
};

} // namespace bluez
} // namespace org

namespace org {
namespace bluez {

class GattCharacteristic1_adaptor
{
public:
    static constexpr const char *INTERFACE_NAME = "org.bluez.GattCharacteristic1";

protected:
    GattCharacteristic1_adaptor(sdbus::IObject &object)
        : m_object(object)
    {
    }

    GattCharacteristic1_adaptor(const GattCharacteristic1_adaptor &)            = delete;
    GattCharacteristic1_adaptor &operator=(const GattCharacteristic1_adaptor &) = delete;
    GattCharacteristic1_adaptor(GattCharacteristic1_adaptor &&)                 = delete;
    GattCharacteristic1_adaptor &operator=(GattCharacteristic1_adaptor &&)      = delete;

    ~GattCharacteristic1_adaptor() = default;

    void registerAdaptor()
    {
        m_object
            .addVTable(
                sdbus::registerMethod("ReadValue")
                    .withInputParamNames("options")
                    .withOutputParamNames("value")
                    .implementedAs([this](const std::map<std::string, sdbus::Variant> &options) {
                        return this->ReadValue(options);
                    }),
                sdbus::registerMethod("WriteValue")
                    .withInputParamNames("value", "options")
                    .implementedAs([this](const std::vector<uint8_t>                  &value,
                                          const std::map<std::string, sdbus::Variant> &options) {
                        return this->WriteValue(value, options);
                    }),
                sdbus::registerMethod("AcquireWrite")
                    .withInputParamNames("options")
                    .withOutputParamNames("fd", "mtu")
                    .implementedAs([this](const std::map<std::string, sdbus::Variant> &options) {
                        return this->AcquireWrite(options);
                    }),
                sdbus::registerMethod("AcquireNotify")
                    .withInputParamNames("options")
                    .withOutputParamNames("fd", "mtu")
                    .implementedAs([this](const std::map<std::string, sdbus::Variant> &options) {
                        return this->AcquireNotify(options);
                    }),
                sdbus::registerMethod("StartNotify").implementedAs([this]() { return this->StartNotify(); }),
                sdbus::registerMethod("StopNotify").implementedAs([this]() { return this->StopNotify(); }),
                sdbus::registerMethod("Confirm").implementedAs([this]() { return this->Confirm(); }),
                sdbus::registerProperty("UUID").withGetter([this]() { return this->UUID(); }),
                sdbus::registerProperty("Service").withGetter([this]() { return this->Service(); }),
                sdbus::registerProperty("Value").withGetter([this]() { return this->Value(); }),
                sdbus::registerProperty("DirectedValue").withGetter([this]() { return this->DirectedValue(); }),
                sdbus::registerProperty("Notifying").withGetter([this]() { return this->Notifying(); }),
                sdbus::registerProperty("Flags").withGetter([this]() { return this->Flags(); }),
                sdbus::registerProperty("Descriptors").withGetter([this]() { return this->Descriptors(); }),
                sdbus::registerProperty("WriteAcquired").withGetter([this]() { return this->WriteAcquired(); }),
                sdbus::registerProperty("NotifyAcquired").withGetter([this]() { return this->NotifyAcquired(); }))
            .forInterface(INTERFACE_NAME);
    }

private:
    virtual std::vector<uint8_t>                ReadValue(const std::map<std::string, sdbus::Variant> &options)     = 0;
    virtual void                                WriteValue(const std::vector<uint8_t>                  &value,
                                                           const std::map<std::string, sdbus::Variant> &options)    = 0;
    virtual std::tuple<sdbus::UnixFd, uint16_t> AcquireWrite(const std::map<std::string, sdbus::Variant> &options)  = 0;
    virtual std::tuple<sdbus::UnixFd, uint16_t> AcquireNotify(const std::map<std::string, sdbus::Variant> &options) = 0;
    virtual void                                StartNotify()                                                       = 0;
    virtual void                                StopNotify()                                                        = 0;
    virtual void                                Confirm()                                                           = 0;

private:
    virtual std::string                                   UUID()           = 0;
    virtual sdbus::ObjectPath                             Service()        = 0;
    virtual std::vector<uint8_t>                          Value()          = 0;
    virtual std::map<sdbus::UnixFd, std::vector<uint8_t>> DirectedValue()  = 0;
    virtual bool                                          Notifying()      = 0;
    virtual std::vector<std::string>                      Flags()          = 0;
    virtual std::vector<sdbus::ObjectPath>                Descriptors()    = 0;
    virtual bool                                          WriteAcquired()  = 0;
    virtual bool                                          NotifyAcquired() = 0;

private:
    sdbus::IObject &m_object;
};

} // namespace bluez
} // namespace org

#endif
