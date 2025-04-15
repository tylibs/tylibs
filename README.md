# TYLiBS

**TYLiBS** (short for **Tiny Libraries**) is a **multi-platform build system, architecture, and library collection** for embedded and desktop development in modern **C++**.

Currently supporting:
- 🐧 **Linux**
- ⚡ **Zephyr RTOS**
- 🔌 **ESP-IDF** (ESP32)

> ⚠️ **Work in Progress** – expect frequent changes and breaking updates!

---

## ✨ Overview

TYLiBS aims to provide a **unified codebase** and **modular architecture** that allows developers to:
- Share code between embedded platforms and Linux
- Write clean, testable C++ for constrained environments
- Abstract away hardware/platform differences where possible
- Leverage CMake for flexible, portable builds

---

## 🚀 Features (Planned / In Progress)

- ✅ Multi-target CMake build system (Zephyr, ESP-IDF, Linux)
- ✅ Modular library structure with reusable components
- ✅ Embedded Template Library (ETL) integration
- ⏳ Cross-platform abstraction layer (GPIO, Serial, etc.)
- ⏳ Unit test support (on-host)
- ⏳ Examples and application templates

---

## 🛠️ Requirements

- CMake 3.16+
- A supported toolchain:
  - Zephyr SDK
  - ESP-IDF (v5+)
  - GCC / Clang for Linux
- Python (for some SDKs like ESP-IDF)

---

## 📁 Project Structure (WIP)

```txt
TYLiBS/
├── cmake/                # CMake modules and toolchain support
├── components/           # Reusable components (platform-agnostic)
│   ├── common/           # Base component, used by all others
│   ├── settings/         # Persist project settings
│   └── net/              # Network connection (MQTT / Sockets)
├── examples/             # Example applications
└── Makefile              # Top-level build config
