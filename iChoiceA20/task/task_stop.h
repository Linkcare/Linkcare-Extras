#ifndef __TASK_STOP_H
#define __TASK_STOP_H

#include "task.h"
#include "lpsa.h"
#include "type.h"

#include "Key.h"
#include "RTC.h"
#include "BLE.h"
#include "vbus.h"
#include "BLE.h"
#include "Clock.h"
#include "LIS3DH.h"
#include "Battery.h"
#include "LPS331AP.h"
#include "Graphics.h"
#include "EEPROM.h"

#include "Display.h"
#include "System.h"
#include "Event.h"
#include "DeviceInfo.h"

#include "stm32l1xx.h"

extern volatile uint8 offMode;
extern uint64 lastDeviceOff;

#endif
