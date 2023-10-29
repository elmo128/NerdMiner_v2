#ifndef __LOLLIN_S2_MINI_H__
#define __LOLLIN_S2_MINI_H__

#define PIN_BUTTON_1 0
#define LED_PIN 15

#define NO_DISPLAY

// example how to configure SD card.
// if you would define everything, 
// to select 1 bit mode, make sure SDMMC_D1-3 are undefined
// to use spi mode, make sure all SDMMC_x pins are undefined

/*
// use SDMMC interface:
// 1-bit mode (might cause issues):
#define SDMMC_CLK 14
#define SDMMC_CMD 15
#define SDMMC_D0 2
// additional defines to enable 4-bit mode
#define SDMMC_D1 4
#define SDMMC_D2 12
#define SDMMC_D3 13
*/

// use SPI interface
// (default SPI unit provided by <SPI.h>)
// setup SPI pins.
#define SDSPI_CS    16
// The following pins can be retreived from the TFT_eSPI lib, 
// if a display that is using it is activated.
#define SDSPI_CLK   18
#define SDSPI_MOSI  35
#define SDSPI_MISO  36

#endif // __LOLLIN_S2_MINI_H__