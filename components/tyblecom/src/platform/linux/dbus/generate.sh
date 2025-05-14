#!/bin/bash

# output paths
PROXY=../proxy
ADAPTOR=../adaptor
XML2CPP=sdbus-c++-xml2cpp

# Clients
${XML2CPP} adapter1.xml --proxy=${PROXY}/adapter1_proxy.h

# Services
# ${XML2CPP} GattService1.xml --adaptor=${ADAPTOR}/GattService1_adaptor.h
# ${XML2CPP} GattCharacteristic1.xml --adaptor=${ADAPTOR}/GattCharacteristic1_adaptor.h
# ${XML2CPP} GattDescriptor1.xml --adaptor=${ADAPTOR}/GattDescriptor1_adaptor.h
