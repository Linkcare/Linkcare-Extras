#ifndef _EVENT_H_
#define _EVENT_H_
#include "type.h"

/*ϵͳ�л��¼�*/
#define  NO_EVENT			0	/*���¼�		 */
#define  USB_VBUSON			1	/*USB����		 */
#define  USB_VBUSOFF		2	/*USB�γ�		 */
#define  KEY_PRESSED		4	/*���������ź�   */
#define  USER_SHAKE 		8	/*�ζ������ź�	 */
#define  NO_POWER 		    16	/*��ص�������	 */

uint8 Event_getEvent(void);
void  Event_register(const uint8 event);
void  Event_clearEvent(void);
void  Event_registerMode(const uint8 mode);
uint8 Event_getCurrentMode(void);
uint8 Event_getLastMode(void);

#endif
