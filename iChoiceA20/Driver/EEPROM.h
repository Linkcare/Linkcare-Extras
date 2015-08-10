#ifndef	__EEPROM_H
#define	__EEPROM_H

#include "type.h"

/*register address*/
#define		WREN		0X06
#define		WRDI		0X04
#define		RDSR		0X05
#define		WRSR		0X01
#define		READ		0X03
#define		WRITE		0X02

/*BIT*/
#define		WIP			0X01
#define		WEL			0X02

#define     E2PROM_ERR  (uint8)0xFF


/*SPI2		*/
#define	EEPROM_CS			GPIO_Pin_8
#define	EEPROM_CS_PORT		GPIOA

#define	EEPROM_SCK			GPIO_Pin_13
#define	EEPROM_MISO			GPIO_Pin_14
#define	EEPROM_MOSI			GPIO_Pin_15
#define	EEPROM_PORT			GPIOB

#define DUMMY_BYTE          0x00

#define CS_H()  	{GPIO_WriteBit(EEPROM_CS_PORT,EEPROM_CS,Bit_SET);__NOP();__NOP();}
#define CS_L()  	{GPIO_WriteBit(EEPROM_CS_PORT,EEPROM_CS,Bit_RESET);__NOP();__NOP();}

/**********************************/
#define		EEPROM_ADDR_RANGE	0x8000	//×ÜÈÝÁ¿32K
#define		EEPROM_PAGESIZE		0x40	//Ã¿Ò³64byte


void EEPROM_write(uint8 *src,uint16 des,uint16 num);

void EEPROM_read(uint16 src,uint8 *des,uint16 num);

void EEPROM_openSPI(void);

void EEPROM_closeSPI(void);



#endif
