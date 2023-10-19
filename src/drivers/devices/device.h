#ifndef __DEVICE_H__
#define __DEVICE_H__

#if defined(NERDMINERV2)
#include "nerdMinerV2.h"
#elif defined(DEVKITV1)
#include "esp32DevKit.h"
#elif defined(TDISPLAY)
#include "lilygoS3TDisplay.h"
#elif defined(NERMINER_S3_AMOLED)
#include "lilygoS3Amoled.h"
#elif defined(NERMINER_S3_DONGLE)
#include "lilygoS3Dongle.h"
#elif defined(ESP32_2432S028R)
#include "esp322432s028r.h"
#elif defined(NERMINER_T_QT)
#include "lilygoT_QT.h"
#elif defined(NERDMINER_T_DISPLAY_V1)
#include "lilygoV1TDisplay.h"
#elif defined(ESP32_CAM)
#include "esp32CAM.h"
#elif defined(LOLIN_S2_MINI)
#include "lollinS2Mini.h"

#else
#error "No device defined"
#endif

#endif // __DEVICE_H__