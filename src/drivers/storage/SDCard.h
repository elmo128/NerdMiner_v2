#ifndef _SDCARD_H_
#define _SDCARD_H_

#include "storage.h"
#include "nvMemory.h"
#include "..\devices\device.h"

#if defined (SDMMC_D0) && defined (SDMMC_D1) && defined (SDMMC_D2) && defined (SDMMC_D3)
#define BUILD_SDMMC_4
#undef BUILD_SDMMC_1
#undef BUILD_SDSPI
#include <SD_MMC.h>
#warning SD card support in 4-Bit mode enabled!
#elif defined (SDMMC_D0) && !(defined (SDMMC_D1) && defined (SDMMC_D2) && defined (SDMMC_D3))
#define BUILD_SDMMC_1
#undef BUILD_SDMMC_4
#undef BUILD_SDSPI
#include <SD_MMC.h>
#warning SD card support in 1-Bit mode enabled!
#elif defined (SDSPI_CLK) && defined (SDSPI_CS) && defined (SDSPI_MOSI) && defined (SDSPI_MISO)
#define BUILD_SDSPI
#undef BUILD_SDMMC_1
#undef BUILD_SDMMC_4
#include <SPI.h>
#include <SD.h>
#warning SD card support in SPI mode enabled!
#endif

// Handles the transfer of settings from sd card to nv memory (wifi credentials are handled by wifimanager)
class SDCard
{
public:
    SDCard();
    ~SDCard();
    void SD2nvMemory(nvMemory* nvMem, TSettings* Settings);
    bool loadConfigFile(TSettings* Settings);
private:
    bool initSDcard();
    void unmount();

#if defined (BUILD_SDMMC_1) || defined(BUILD_SDMMC_4)
    fs::SDMMCFS* iSD_;
#elif defined (BUILD_SDSPI)
    SPIClass* ispi_;
    fs::SDFS* iSD_;
#endif
};

#endif // _SDCARD_H_
