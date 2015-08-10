#ifndef _EVENT_H_
#define _EVENT_H_
#include "type.h"

/*系统切换事件*/
#define  NO_EVENT			0	/*无事件		 */
#define  USB_VBUSON			1	/*USB插入		 */
#define  USB_VBUSOFF		2	/*USB拔出		 */
#define  KEY_PRESSED		4	/*按键唤醒信号   */
#define  USER_SHAKE 		8	/*晃动唤醒信号	 */
#define  NO_POWER 		    16	/*电池电量不足	 */

uint8 Event_getEvent(void);
void  Event_register(const uint8 event);
void  Event_clearEvent(void);
void  Event_registerMode(const uint8 mode);
uint8 Event_getCurrentMode(void);
uint8 Event_getLastMode(void);

#endif
