// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

#ifndef TY_POSIX_PLATFORM_SETTINGS_FILE_HPP_
#define TY_POSIX_PLATFORM_SETTINGS_FILE_HPP_

#include <ty/tycommon.h>

namespace ty {
namespace Posix {

class SettingsFile
{
public:
    SettingsFile(void)
        : mSettingsFd(-1)
    {
    }

    /**
     * Performs the initialization for the settings file.
     *
     * @param[in]  aSettingsFileBaseName    A pointer to the base name of the settings file.
     *
     * @retval TY_ERROR_NONE    The given settings file was initialized successfully.
     * @retval TY_ERROR_PARSE   The key-value format could not be parsed (invalid format).
     */
    tyError Init(const char *aSettingsFileBaseName);

    /**
     * Performs the de-initialization for the settings file.
     */
    void Deinit(void);

    /**
     * Gets a setting from the settings file.
     *
     * @param[in]      aKey          The key associated with the requested setting.
     * @param[in]      aIndex        The index of the specific item to get.
     * @param[out]     aValue        A pointer to where the value of the setting should be written.
     * @param[in,out]  aValueLength  A pointer to the length of the value.
     *
     * @retval TY_ERROR_NONE        The given setting was found and fetched successfully.
     * @retval TY_ERROR_NTY_FOUND   The given key or index was not found in the setting store.
     */
    tyError Get(uint16_t aKey, int aIndex, uint8_t *aValue, uint16_t *aValueLength);

    /**
     * Sets a setting in the settings file.
     *
     * @param[in]  aKey          The key associated with the requested setting.
     * @param[in]  aValue        A pointer to where the new value of the setting should be read from.
     * @param[in]  aValueLength  The length of the data pointed to by aValue.
     */
    void Set(uint16_t aKey, const uint8_t *aValue, uint16_t aValueLength);

    /**
     * Adds a setting to the settings file.
     *
     * @param[in]  aKey          The key associated with the requested setting.
     * @param[in]  aValue        A pointer to where the new value of the setting should be read from.
     * @param[in]  aValueLength  The length of the data pointed to by aValue.
     */
    void Add(uint16_t aKey, const uint8_t *aValue, uint16_t aValueLength);

    /**
     * Removes a setting from the settings file.
     *
     * @param[in]  aKey       The key associated with the requested setting.
     * @param[in]  aIndex     The index of the value to be removed. If set to -1, all values for this aKey will be
     *                        removed.
     *
     * @retval TY_ERROR_NONE        The given key and index was found and removed successfully.
     * @retval TY_ERROR_NTY_FOUND   The given key or index was not found in the setting store.
     */
    tyError Delete(uint16_t aKey, int aIndex);

    /**
     * Deletes all settings from the setting file.
     */
    void Wipe(void);

private:
    static const size_t kMaxFileDirectorySize   = sizeof(TY_CONFIG_LINUX_SETTINGS_PATH);
    static const size_t kSlashLength            = 1;
    static const size_t kMaxFileBaseNameSize    = 64;
    static const size_t kMaxFileExtensionLength = 5; ///< The length of `.Swap` or `.data`.
    static const size_t kMaxFilePathSize =
        kMaxFileDirectorySize + kSlashLength + kMaxFileBaseNameSize + kMaxFileExtensionLength;

    tyError Delete(uint16_t aKey, int aIndex, int *aSwapFd);
    void    GetSettingsFilePath(char aFileName[kMaxFilePathSize], bool aSwap);
    int     SwapOpen(void);
    void    SwapWrite(int aFd, uint16_t aLength);
    void    SwapPersist(int aFd);
    void    SwapDiscard(int aFd);

    char mSettingFileBaseName[kMaxFileBaseNameSize];
    int  mSettingsFd;
};

} // namespace Posix
} // namespace ty

#endif // TY_POSIX_PLATFORM_SETTINGS_FILE_HPP_
