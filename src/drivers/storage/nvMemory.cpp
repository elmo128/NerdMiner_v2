#include "nvMemory.h"

#ifdef NVMEM_SPIFFS

#include <SPIFFS.h>
#include <FS.h>
#include <ArduinoJson.h>

nvMemory::nvMemory() : Initialized_(false){};

nvMemory::~nvMemory()
{
    if (Initialized_)
        SPIFFS.end();
};

/// @brief Save settings to config file on SPIFFS
/// @param TSettings* Settings to be saved.
/// @return true on success
bool nvMemory::saveConfig(TSettings* Settings)
{
    if (init())
    {
        // Save Config in JSON format
        Serial.println(F("SPIFS: Saving configuration."));

        // Create a JSON document
        StaticJsonDocument<512> json;
        json[JSON_SPIFFS_KEY_POOLURL] = Settings->PoolAddress;
        json[JSON_SPIFFS_KEY_POOLPORT] = Settings->PoolPort;
        json[JSON_SPIFFS_KEY_WALLETID] = Settings->BtcWallet;
        json[JSON_SPIFFS_KEY_TIMEZONE] = Settings->Timezone;
        json[JSON_SPIFFS_KEY_STATS2NV] = Settings->saveStats;

        // Open config file
        File configFile = SPIFFS.open(JSON_CONFIG_FILE, "w");
        if (!configFile)
        {
            // Error, file did not open
            Serial.println("SPIFS: Failed to open config file for writing");
            return false;
        }

        // Serialize JSON data to write to file
        serializeJsonPretty(json, Serial);
        Serial.print('\n');
        if (serializeJson(json, configFile) == 0)
        {
            // Error writing file
            Serial.println(F("SPIFS: Failed to write to file"));
            return false;
        }
        // Close file
        configFile.close();
        return true;
    };
    return false;
}

/// @brief Load settings from config file located in SPIFFS.
/// @param TSettings* Struct to update with new settings.
/// @return true on success
bool nvMemory::loadConfig(TSettings* Settings)
{
    // Uncomment if we need to format filesystem
    // SPIFFS.format();

    // Load existing configuration file
    // Read configuration from FS json

    if (init())
    {
        if (SPIFFS.exists(JSON_CONFIG_FILE))
        {
            // The file exists, reading and loading
            File configFile = SPIFFS.open(JSON_CONFIG_FILE, "r");
            if (configFile)
            {
                Serial.println("SPIFS: Loading config file");
                StaticJsonDocument<512> json;
                DeserializationError error = deserializeJson(json, configFile);
                configFile.close();
                serializeJsonPretty(json, Serial);
                Serial.print('\n');
                if (!error)
                {
                    Settings->PoolAddress = json[JSON_SPIFFS_KEY_POOLURL] | Settings->PoolAddress;
                    strcpy(Settings->BtcWallet, json[JSON_SPIFFS_KEY_WALLETID] | Settings->BtcWallet);
                    if (json.containsKey(JSON_SPIFFS_KEY_POOLPORT))
                        Settings->PoolPort = json[JSON_SPIFFS_KEY_POOLPORT].as<int>();
                    if (json.containsKey(JSON_SPIFFS_KEY_TIMEZONE))
                        Settings->Timezone = json[JSON_SPIFFS_KEY_TIMEZONE].as<int>();
                    if (json.containsKey(JSON_SPIFFS_KEY_STATS2NV))
                        Settings->saveStats = json[JSON_SPIFFS_KEY_STATS2NV].as<bool>();
                    return true;
                }
                else
                {
                    // Error loading JSON data
                    Serial.println("SPIFS: Error parsing config file!");
                }
            }
            else
            {
                Serial.println("SPIFS: Error opening config file!");
            }
        }
        else
        {
            Serial.println("SPIFS: No config file available!");
        }
    }
    return false;
}

/// @brief Delete config file from SPIFFS
/// @return true on successs
bool nvMemory::deleteConfig()
{
    Serial.println("SPIFS: Erasing config file..");
    return SPIFFS.remove(JSON_CONFIG_FILE); //Borramos fichero
}

/// @brief Prepare and mount SPIFFS
/// @return true on success
bool nvMemory::init()
{
    if (!Initialized_)
    {
        Serial.println("SPIFS: Mounting File System...");
        // May need to make it begin(true) first time you are using SPIFFS
        Initialized_ = SPIFFS.begin(false) || SPIFFS.begin(true);
        Initialized_ ? Serial.println("SPIFS: Mounted") : Serial.println("SPIFS: Mounting failed.");
    }
    else
    {
        Serial.println("SPIFS: Already Mounted");
    }
    return Initialized_;
};

#elif defined(NVMEM_FLASH)

#include <nvs_flash.h>
#include <nvs.h>

nvMemory::nvMemory() : Initialized_(false) {};
nvMemory::~nvMemory() {};
bool nvMemory::saveConfig(TSettings* Settings) 
{
    Serial.println("NVM: saving data");
    esp_err_t ret = nvs_flash_init();

    nvs_handle_t stat_handle;

    ret = nvs_open(NVMEM_FLASH_SETTINGS_LABEL, NVS_READWRITE, &stat_handle);
    Serial.printf("NVM: partition opened successfully? %d\n", (uint32_t) ret);
    if (ret == ESP_OK)
    {
        ret = nvs_set_str(stat_handle, JSON_SPIFFS_KEY_POOLURL, Settings->PoolAddress.c_str());
        ret = nvs_set_str(stat_handle, JSON_SPIFFS_KEY_WALLETID, Settings->BtcWallet);
        ret = nvs_set_i32(stat_handle, JSON_SPIFFS_KEY_POOLPORT, Settings->PoolPort);
        ret = nvs_set_i32(stat_handle, JSON_SPIFFS_KEY_TIMEZONE, Settings->Timezone);

        int8_t out = (Settings->saveStats == true) ? 1 : 0;
        ret = nvs_set_i8(stat_handle, JSON_SPIFFS_KEY_STATS2NV, out);
        nvs_commit(stat_handle);

        nvs_close(stat_handle);
        nvs_flash_deinit_partition(NVMEM_FLASH_SETTINGS_LABEL);
        return true;
    }
    nvs_close(stat_handle);
    nvs_flash_deinit_partition(NVMEM_FLASH_SETTINGS_LABEL);
    return false;
}

bool nvMemory::loadConfig(TSettings* Settings) 
{
    Serial.println("NVM: loading data");
    esp_err_t ret = nvs_flash_init();

    nvs_handle_t stat_handle;

    ret = nvs_open(NVMEM_FLASH_SETTINGS_LABEL, NVS_READONLY, &stat_handle);
    Serial.printf("NVM: partition opened successfully? %d\n", (uint32_t) ret);
    if(ret == ESP_OK)
    {
        size_t required_size = 0;
        nvs_get_str(stat_handle, JSON_SPIFFS_KEY_POOLURL, NULL, &required_size);
        if(required_size>0)
        {
            StringX result = StringX(required_size);
            ret = nvs_get_str(stat_handle, JSON_SPIFFS_KEY_POOLURL, result.getBuffer(), &required_size);
            Serial.println(result.getString());
            Settings->PoolAddress=result.getString();
        }
        Serial.print("NVM: get poolURL:");
        Serial.println(Settings->PoolAddress);

        required_size = sizeof(Settings->BtcWallet);
        ret = nvs_get_str(stat_handle, JSON_SPIFFS_KEY_WALLETID, Settings->BtcWallet, &required_size);
        Serial.print("NVM: get walletID:");
        Serial.println(Settings->BtcWallet);
        
        ret = nvs_get_i32(stat_handle, JSON_SPIFFS_KEY_POOLPORT, &Settings->PoolPort);
        Serial.printf("NVM: get port: %d\n", Settings->PoolPort);

        ret = nvs_get_i32(stat_handle, JSON_SPIFFS_KEY_TIMEZONE, &Settings->Timezone);
        Serial.printf("NVM: get Timezone: %d\n", Settings->Timezone);

        int8_t out = 0;
        ret = nvs_get_i8(stat_handle, JSON_SPIFFS_KEY_STATS2NV, &out);
        Settings->saveStats = (out > 0)?true:false;
        Serial.printf("NVM: get saveStats: %d\n", (uint8_t) Settings->saveStats);

        nvs_close(stat_handle);
        return true;
    }
    nvs_close(stat_handle);
    return false;
}
bool nvMemory::deleteConfig() 
{ 
    esp_err_t ret = nvs_flash_erase();
    return true;
}
bool nvMemory::init()
{
    esp_err_t ret = nvs_flash_init();
    return true;
}

#else

nvMemory::nvMemory() : Initialized_(false) {}; {}
nvMemory::~nvMemory() {}
bool nvMemory::saveConfig(TSettings* Settings) { return false; }
bool nvMemory::loadConfig(TSettings* Settings) { return false; }
bool nvMemory::deleteConfig() { return false; }
bool nvMemory::init() { return false; }


#endif //NVMEM_TYPE