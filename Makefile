#  SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
#  SPDX-License-Identifier: Apache-2.0

## Select the esp target to build (esp32 / esp32s3 / exp32c6 / ....)
ESP_TARGET := esp32s3
## Select the zephyr target/board to build
ZEPHYR_TARGET := native_sim/native/64
## Select the Application
APP_NAME := hello_world

BUILD_DIR := build
export TYLIBS_PATH := $(HOME)/_dev/clever/components/tylibs
export TYLIBS_COMPONENT_MANAGER := 0
# include common makefile
include tools/make/makefile.mk

# import settings for the used Platform
ifneq (,$(findstring esp,$(MAKECMDGOALS)))
APP_DIR := test-apps/esp/${APP_NAME}
include tools/make/espidf-54.mk
endif
ifneq (,$(findstring zephyr,$(MAKECMDGOALS)))
APP_DIR := test-apps/zephyr/${APP_NAME}
include tools/make/zephyr-410.mk
endif
ifneq (,$(findstring linux,$(MAKECMDGOALS)))
APP_DIR := test-apps/linux/${APP_NAME}
endif

# ESP-IDF specific targets
# ---------------------------------------------------------------------------
.PHONY: esp esp.build esp.flash esp.clean esp.linux

esp: esp.clean esp.build ## clean and build for the selected TARGET

esp.build: ## (re)compile for the selected TARGET
	@echo "Building for ${var1}"
	${IDFPY} -C ${APP_DIR} -B ${BUILD_DIR} -DIDF_TARGET=${ESP_TARGET} -DTY_PYTHON_DEPS_CHECKED=1 build size -l

esp.flash: esp.build ## Flash the firmware to ESP32
	${IDFPY} -C ${APP_DIR} -B ${BUILD_DIR} flash

## Delete build directory and sdkconfig
esp.clean:
	$(RMDIR) ${BUILD_DIR} && $(RM) ${APP_DIR}/sdkconfig

## Build the firmware for host execution (Linux only)
## This will start the monitor
esp.linux:
	${IDFPY} -C ${APP_DIR} -B ${BUILD_DIR}  -DTY_PYTHON_DEPS_CHECKED=1 --preview set-target linux build monitor

# Zephyr specific targets
# ---------------------------------------------------------------------------
.PHONY: zephyr zephyr.build zephyr.clean

zephyr: zephyr.clean zephyr.build ## clean and build for the selected TARGET

zephyr.build: ## (re)compile for the selected TARGET
	west build -b ${ZEPHYR_TARGET} --build-dir ${BUILD_DIR} -p always ${APP_DIR}

## Delete build directory
zephyr.clean:
	$(RMDIR) $(BUILD_DIR)

# Linux specific targets
# ---------------------------------------------------------------------------
.PHONY: linux linux.build linux.clean

linux: linux.clean linux.build ## clean and build

linux.build: ## (re)compile
	cmake -S ${APP_DIR} -B ${BUILD_DIR} -DTY_PYTHON_DEPS_CHECKED=1 && cmake --build ${BUILD_DIR} -- -j

## Delete build directory
linux.clean:
	$(RMDIR) $(BUILD_DIR) && $(RM) ${APP_DIR}/sdkconfig
