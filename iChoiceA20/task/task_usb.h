#ifndef __TASK_USB_H
#define __TASK_USB_H

#include "task.h"
#include "lpsa.h"
#include "type.h"

#include "RTC.h"
#include "Key.h"
#include "BLE.h"
#include "Clock.h"
#include "Battery.h"
#include "Graphics.h"

#include "usb_lib.h"
#include "usb_prop.h"
#include "vbus.h"

#include "Display.h"
#include "System.h"
#include "Event.h"
#include "CommandManager.h"
#include "DeviceInfo.h"
#include "task_stop.h"

/*function declare*/
extern void USB_Interrupts_Config(void);
extern void Set_USBClock(void);

#endif