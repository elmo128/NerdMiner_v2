#ifndef _STORAGE_H_
#define _STORAGE_H_

#include <Arduino.h>

// config files

// default settings
#define DEFAULT_SSID		"NerdMinerAP"
#define DEFAULT_WIFIPW		"MineYourCoins"
#define DEFAULT_POOLURL		"public-pool.io"
#define DEFAULT_WALLETID	"yourBtcAddress"
#define DEFAULT_POOLPORT	21496
#define DEFAULT_TIMEZONE	2
#define DEFAULT_SAVESTATS	false

// JSON config files
#define JSON_CONFIG_FILE	"/config.json"

// JSON config file SD card (for user interaction, readme.md)
#define JSON_KEY_SSID		"SSID"
#define JSON_KEY_PASW		"WifiPW"
#define JSON_KEY_POOLURL	"PoolUrl"
#define JSON_KEY_WALLETID	"BtcWallet"
#define JSON_KEY_POOLPORT	"PoolPort"
#define JSON_KEY_TIMEZONE	"Timezone"
#define JSON_KEY_STATS2NV	"SaveStats"

// JSON config file SPIFFS (different for backward compatibility with existing devices)
#define JSON_SPIFFS_KEY_POOLURL		"poolString"
#define JSON_SPIFFS_KEY_POOLPORT	"portNumber"
#define JSON_SPIFFS_KEY_WALLETID	"btcString"
#define JSON_SPIFFS_KEY_TIMEZONE	"gmtZone"
#define JSON_SPIFFS_KEY_STATS2NV	"saveStatsToNVS"

// settings
struct TSettings
{
	String WifiSSID{ DEFAULT_SSID };
	String WifiPW{ DEFAULT_WIFIPW };
	String PoolAddress{ DEFAULT_POOLURL };
	char BtcWallet[80]{ DEFAULT_WALLETID };
	int PoolPort{ DEFAULT_POOLPORT };
	int Timezone{ DEFAULT_TIMEZONE };
	bool saveStats{ DEFAULT_SAVESTATS };
};

// Logfile, Settings, inherited from TSettings are just for information and might be incomplete.
#define JSON_LOG_FILE "/log.json"
#define JSON_LOGS_OBJECT_SETTINGS "Settings"
#define JSON_LOGS_OBJECT_LOGS   "Logs"
#define JSON_LOGS_KEY_TEMPLATES "Templates"
#define JSON_LOGS_KEY_MHASHES   "MHashes"
#define JSON_LOGS_KEY_UPTIME    "UpTime"
#define JSON_LOGS_KEY_SHARES    "Shares"
#define JSON_LOGS_KEY_VALIDS    "Valids"
#define JSON_LOGS_KEY_BESTDIFF  "bestDiff"

struct TLog : TSettings
{
    uint32_t templates{0};
    //uint32_t hashes{0};
    uint32_t Mhashes{0};
    //uint32_t totalKHashes{0};
    //uint32_t elapsedKHs{0};
    uint64_t upTime{0};

    uint32_t shares{0}; // increase if blockhash has 32 bits of zeroes
    uint32_t valids{0}; // increased if blockhash <= target

    // Track best diff
    double best_diff{0.0};
};

#endif // _STORAGE_H_