// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/** Major version number (X.x.x) */
#define TYLIBS_VERSION_MAJOR 0
/** Minor version number (x.X.x) */
#define TYLIBS_VERSION_MINOR 0
/** Patch version number (x.x.X) */
#define TYLIBS_VERSION_PATCH 1

/**
 * Macro to convert TYLIBS version number into an integer
 *
 * To be used in comparisons, such as TYLIBS_VERSION >= TYLIBS_VERSION_VAL(4, 0, 0)
 */
#define TYLIBS_VERSION_VAL(major, minor, patch) ((major << 16) | (minor << 8) | (patch))

/**
 * Current TYLIBS version, as an integer
 *
 * To be used in comparisons, such as TYLIBS_VERSION >= TYLIBS_VERSION_VAL(4, 0, 0)
 */
#define TYLIBS_VERSION TYLIBS_VERSION_VAL(TYLIBS_VERSION_MAJOR, TYLIBS_VERSION_MINOR, TYLIBS_VERSION_PATCH)

#ifndef __ASSEMBLER__

/**
 * Return full TYLIBS version string, same as 'git describe' output.
 *
 * @note If you are printing the TYLIBS version in a log file or other information,
 * this function provides more information than using the numerical version macros.
 * For example, numerical version macros don't differentiate between development,
 * pre-release and release versions, but the output of this function does.
 *
 * @return constant string from TYLIBS_VER
 */
const char *ty_get_tylibs_version(void);

#endif

#ifdef __cplusplus
}
#endif
