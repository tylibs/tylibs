// SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
// SPDX-License-Identifier: Apache-2.0

#include "ty/settings//platform/settings.h"
#include "esp_check.h"
#include "nvs.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TY_NAMESPACE "ty"
#define TY_PART_NAME s_storage_name
#define TY_KEY_PATTERN "TS%02x"
#define TY_KEY_INDEX_PATTERN "TS%02x%02x"
#define TY_KEY_PATTERN_LEN 5
#define TY_KEY_INDEX_PATTERN_LEN 7
static nvs_handle_t s_ot_nvs_handle;
static const char  *s_storage_name;

const char *TY_PLAT_LOG_TAG = "settings";

void esp_openthread_set_storage_name(const char *name)
{
    s_storage_name = name;
}

static esp_err_t get_next_empty_index(uint16_t aKey, uint8_t *index)
{
    ESP_RETURN_ON_FALSE((s_ot_nvs_handle != 0), ESP_ERR_INVALID_STATE, TY_PLAT_LOG_TAG, "OT NVS handle is invalid.");
    esp_err_t               ret                                  = ESP_OK;
    static volatile uint8_t s_unused_pos                         = 0;
    char                    ot_nvs_key[TY_KEY_INDEX_PATTERN_LEN] = {0};
    nvs_iterator_t          nvs_it                               = NULL;
    bool                    found                                = false;

    for (uint8_t i = 0; i != UINT8_MAX; i++)
    {
        s_unused_pos++;
        found = false;
        snprintf(ot_nvs_key, sizeof(ot_nvs_key), TY_KEY_INDEX_PATTERN, (uint8_t)aKey, s_unused_pos);
        ret = nvs_entry_find(TY_PART_NAME, TY_NAMESPACE, NVS_TYPE_BLOB, &nvs_it);
        while (ret == ESP_OK)
        {
            nvs_entry_info_t info;
            nvs_entry_info(nvs_it, &info);
            if (memcmp(ot_nvs_key, info.key, TY_KEY_INDEX_PATTERN_LEN - 1) == 0)
            {
                found = true;
                break;
            }
            ret = nvs_entry_next(&nvs_it);
        }
        nvs_release_iterator(nvs_it);
        if (!found)
        {
            // find an empty position, return ESP_OK
            *index = s_unused_pos;
            return ESP_OK;
        }
    }
    // all index was used, no memory for current data, return ESP_ERR_NOT_FOUND.
    return ESP_ERR_NOT_FOUND;
}

static esp_err_t find_target_key_using_index(uint16_t aKey, int aIndex, char *key, size_t key_len)
{
    /* ESP_RETURN_ON_FALSE((s_ot_nvs_handle != 0), ESP_ERR_INVALID_STATE, TY_PLAT_LOG_TAG, "OT NVS handle is
     * invalid.");
     */
    esp_err_t      ret                            = ESP_OK;
    nvs_iterator_t nvs_it                         = NULL;
    int            cur_index                      = 0;
    char           ot_nvs_key[TY_KEY_PATTERN_LEN] = {0};

    ret = nvs_entry_find(TY_PART_NAME, TY_NAMESPACE, NVS_TYPE_BLOB, &nvs_it);
    if (ret != ESP_OK)
    {
        return ret;
    }
    snprintf(ot_nvs_key, sizeof(ot_nvs_key), TY_KEY_PATTERN, (uint8_t)aKey);
    while (ret == ESP_OK)
    {
        nvs_entry_info_t info;
        nvs_entry_info(nvs_it, &info);
        if (memcmp(ot_nvs_key, info.key, TY_KEY_PATTERN_LEN - 1) == 0)
        {
            if (cur_index == aIndex)
            {
                memcpy(key, info.key, key_len);
                break;
            }
            else
            {
                cur_index++;
            }
        }
        ret = nvs_entry_next(&nvs_it);
    }
    nvs_release_iterator(nvs_it);

    if ((cur_index != aIndex) || (ret != ESP_OK))
    {
        return ESP_FAIL;
    }
    return ESP_OK;
}

static esp_err_t erase_all_key(uint16_t aKey)
{
    /* ESP_RETURN_ON_FALSE((s_ot_nvs_handle != 0), ESP_ERR_INVALID_STATE, TY_PLAT_LOG_TAG, "OT NVS handle is
     * invalid.");
     */
    esp_err_t      ret                            = ESP_OK;
    nvs_iterator_t nvs_it                         = NULL;
    char           ot_nvs_key[TY_KEY_PATTERN_LEN] = {0};

    ret = nvs_entry_find(TY_PART_NAME, TY_NAMESPACE, NVS_TYPE_BLOB, &nvs_it);
    if (ret == ESP_ERR_NVS_NOT_FOUND)
    {
        return ESP_OK;
    }
    /* ESP_RETURN_ON_FALSE((ret == ESP_OK && nvs_it != NULL), ESP_FAIL, TY_PLAT_LOG_TAG, */
    /*                     "Can not find any data in nvs flash, err: %d", ret); */
    while (ret == ESP_OK)
    {
        snprintf(ot_nvs_key, sizeof(ot_nvs_key), TY_KEY_PATTERN, (uint8_t)aKey);
        nvs_entry_info_t info;
        nvs_entry_info(nvs_it, &info);
        if (memcmp(ot_nvs_key, info.key, TY_KEY_PATTERN_LEN - 1) == 0)
        {
            ret = nvs_erase_key(s_ot_nvs_handle, info.key);
            if (ret != ESP_OK)
            {
                break;
            }
        }
        ret = nvs_entry_next(&nvs_it);
    }
    nvs_release_iterator(nvs_it);
    ret = nvs_commit(s_ot_nvs_handle);
    if (ret != ESP_OK)
    {
        return ESP_FAIL;
    }
    return ESP_OK;
}

void tyPlatSettingsInit(const uint16_t *aSensitiveKeys, uint16_t aSensitiveKeysLength)
{
    esp_err_t err = nvs_open(TY_NAMESPACE, NVS_READWRITE, &s_ot_nvs_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TY_PLAT_LOG_TAG, "Failed to open NVS namespace (0x%x)", err);
        assert(0);
    }
}

void tyPlatSettingsDeinit()
{
    if (s_ot_nvs_handle != 0)
    {
        nvs_close(s_ot_nvs_handle);
    }
}

tyError tyPlatSettingsGet(uint16_t aKey, int aIndex, uint8_t *aValue, uint16_t *aValueLength)
{
    ESP_RETURN_ON_FALSE((s_ot_nvs_handle != 0), TY_ERROR_NOT_FOUND, TY_PLAT_LOG_TAG, "OT NVS handle is invalid.");
    esp_err_t ret                                  = ESP_OK;
    char      ot_nvs_key[TY_KEY_INDEX_PATTERN_LEN] = {0};

    ret = find_target_key_using_index(aKey, aIndex, ot_nvs_key, TY_KEY_INDEX_PATTERN_LEN);
    if (ret != ESP_OK)
    {
        return TY_ERROR_NOT_FOUND;
    }
    size_t length = *aValueLength;
    ret           = nvs_get_blob(s_ot_nvs_handle, ot_nvs_key, aValue, &length);
    *aValueLength = (uint16_t)length;
    ESP_RETURN_ON_FALSE((ret == ESP_OK), TY_ERROR_NOT_FOUND, TY_PLAT_LOG_TAG, "Data not found, err: %d", ret);
    return TY_ERROR_NONE;
}

tyError tyPlatSettingsSet(uint16_t aKey, const uint8_t *aValue, uint16_t aValueLength)
{
    ESP_RETURN_ON_FALSE((s_ot_nvs_handle != 0), TY_ERROR_NOT_FOUND, TY_PLAT_LOG_TAG, "OT NVS handle is invalid.");
    esp_err_t ret                                  = ESP_OK;
    char      ot_nvs_key[TY_KEY_INDEX_PATTERN_LEN] = {0};

    snprintf(ot_nvs_key, sizeof(ot_nvs_key), TY_KEY_INDEX_PATTERN, (uint8_t)aKey, 0);
    ret = nvs_set_blob(s_ot_nvs_handle, ot_nvs_key, aValue, aValueLength);
    ESP_RETURN_ON_FALSE((ret == ESP_OK), TY_ERROR_NO_BUFS, TY_PLAT_LOG_TAG, "No buffers, err: %d", ret);
    ret = nvs_commit(s_ot_nvs_handle);
    ESP_RETURN_ON_FALSE((ret == ESP_OK), TY_ERROR_NO_BUFS, TY_PLAT_LOG_TAG, "OT NVS handle shut down, err: %d", ret);
    return TY_ERROR_NONE;
}

tyError tyPlatSettingsAdd(uint16_t aKey, const uint8_t *aValue, uint16_t aValueLength)
{
    ESP_RETURN_ON_FALSE((s_ot_nvs_handle != 0), TY_ERROR_NOT_FOUND, TY_PLAT_LOG_TAG, "OT NVS handle is invalid.");
    esp_err_t ret = ESP_OK;
    uint8_t   unused_pos;
    char      ot_nvs_key[TY_KEY_INDEX_PATTERN_LEN] = {0};

    ret = get_next_empty_index(aKey, &unused_pos);
    ESP_RETURN_ON_FALSE((ret == ESP_OK), TY_ERROR_NO_BUFS, TY_PLAT_LOG_TAG, "No buffers, err: %d", ret);
    snprintf(ot_nvs_key, sizeof(ot_nvs_key), TY_KEY_INDEX_PATTERN, (uint8_t)aKey, unused_pos);
    ret = nvs_set_blob(s_ot_nvs_handle, ot_nvs_key, aValue, aValueLength);
    ESP_RETURN_ON_FALSE((ret == ESP_OK), TY_ERROR_NO_BUFS, TY_PLAT_LOG_TAG, "No buffers, err: %d", ret);
    ret = nvs_commit(s_ot_nvs_handle);
    ESP_RETURN_ON_FALSE((ret == ESP_OK), TY_ERROR_NO_BUFS, TY_PLAT_LOG_TAG, "OT NVS handle shut down, err: %d", ret);
    return TY_ERROR_NONE;
}

tyError tyPlatSettingsDelete(uint16_t aKey, int aIndex)
{
    /* ESP_RETURN_ON_FALSE((s_ot_nvs_handle != 0), TY_ERROR_NOT_FOUND, TY_PLAT_LOG_TAG, "OT NVS handle is
     * invalid.");
     */
    esp_err_t ret = ESP_OK;

    if (aIndex == -1)
    {
        ret = erase_all_key(aKey);
    }
    else
    {
        char ot_nvs_key[TY_KEY_INDEX_PATTERN_LEN] = {0};
        ret = find_target_key_using_index(aKey, aIndex, ot_nvs_key, TY_KEY_INDEX_PATTERN_LEN);
        if (ret != ESP_OK)
        {
            return TY_ERROR_NOT_FOUND;
        }
        ret = nvs_erase_key(s_ot_nvs_handle, ot_nvs_key);
        nvs_commit(s_ot_nvs_handle);
    }
    return TY_ERROR_NONE;
}

void tyPlatSettingsWipe()
{
    nvs_erase_all(s_ot_nvs_handle);
}
