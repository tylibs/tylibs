// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0
/**
 * @file
 * @brief
 *   This file defines the platform-specific functions needed by TYLiBS's example applications.
 */

#ifndef TY_PLATFORM_H_
#define TY_PLATFORM_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Performs all platform-specific initialization of TYLiBS's drivers.
 *
 * @note This function is not called by the TYLiBS library. Instead, the system/RTOS should call this function
 *       when initialization of TYLiBS's drivers is most appropriate.
 *
 */
void tiPlatformInit(void);

/**
 * Performs all platform-specific deinitialization for TYLiBS's drivers.
 *
 * @note This function is not called by the TYLiBS library. Instead, the system/RTOS should call this function
 *       when deinitialization of TYLiBS's drivers is most appropriate.
 */
void tiPlatformDeinit(void);

#ifdef __cplusplus
} // end of extern "C"
#endif

#endif // TY_PLATFORM_H_
