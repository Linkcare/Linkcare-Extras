#ifndef __RESOURCEALLOC_H
#define __RESOURCEALLOC_H

//系统优先级分配
#define	  PRI_MCU_USART		0
#define	  PRI_MCU_TIMER		1
#define	  PRI_MCU_FIFO		2
#define	  PRI_MCU_KEY		3
#define	  PRI_MCU_VBUS		4
#define	  PRI_MCU_ALARM		5
#define	  PRI_USB_IRQ		6

//系统内部RAM分配(用户区)
#define USER_RAM_START		0x20002600
#define USER_RAM_END		0x200027FF

//系统内部EEPROM分配
#define INNER_EEPROM_START	0x08080000
#define INNER_EEPROM_END	0x08080FFF

//外部EEPROM分配
#define OUTER_EEPROM_START	0X0000000
#define OUTER_EEPROM_END	0X0007FFF



#endif