#include "stm32l1xx.h"
#include "string.h"
#include "Configuration.h"
#include "InnerEEPROM.h"
#include "stm32l1xx.h"
#include "lpsa.h"
#include "type.h"
#include "BLE.h"
/*********************���Ŷ���**************************/
#define BLE_WAKE		GPIO_Pin_15		//�˿�A
#define BLE_POWER		GPIO_Pin_10		//�˿�A

#define	BLE_STATUS0		GPIO_Pin_3		//�˿�B
#define BLE_STATUS1		GPIO_Pin_4		//�˿�B
#define BLE_CTRL		GPIO_Pin_5		//�˿�B
#define BLE_RXD			GPIO_Pin_6		//�˿�B
#define BLE_TXD			GPIO_Pin_7		//�˿�B

#define	PORT_WAKE		GPIOA
#define	PORT_OTHER		GPIOB
#define	PORT_POWER		GPIOA

#define	CLK_WAKE		RCC_AHBPeriph_GPIOA
#define	CLK_OTHER		RCC_AHBPeriph_GPIOB


#define BLE_powerOn()	GPIO_WriteBit(PORT_POWER,BLE_POWER ,Bit_RESET)
#define BLE_powerOff()	GPIO_WriteBit(PORT_POWER,BLE_POWER ,Bit_SET)
/********************************************************/
#define DELAY   7000
extern lpsa_i8	WrStart;
extern BLE_TOOLS BleTools;


struct
{
  	lpsa_u8       MsgStk[256];
  	lpsa_u8       WriteAdd;
  	lpsa_u8       ReadAdd;

  	//32λ��дָ�룬ÿ��������д����4G�Σ����ڻ����������Ƚ�
  	lpsa_u32      Count_w;
  	lpsa_u32      Count_r;
}BleRx;


lpsa_u8	Mstk [150];
Q_EVENT Q_SendData;

void powerOff(void);


static void BLE_BuffInit(void)
{
  	BleRx.WriteAdd = 0;
  	BleRx.ReadAdd  = 0;
  	BleRx.Count_r  = 0;
  	BleRx.Count_w  = 0;
  	memset(BleRx.MsgStk, 0, sizeof(BleRx.MsgStk));
}

static lpsa_u8 *BLE_ReadBuff(void)
{
  	static lpsa_u8 Rdata;

  	Rdata = 0;

  	if(BleRx.Count_r < BleRx.Count_w)
  	{
    	BleRx.Count_r ++;
    	Rdata = BleRx.MsgStk[BleRx.ReadAdd ++];
  	}
  	else
    	return NULL;

  	return &Rdata;
}

static void BLE_WriteBuff(lpsa_u8 Data)
{
  BleRx.MsgStk[BleRx.WriteAdd ++] = Data;
  BleRx.Count_w ++;
}

/********************************************************/
/*�ر�BLE���ƶ˿�	    								*/
/********************************************************/
#pragma optimize = none
void BLE_closePort(void)
{
  	Flag.Power = 0;

  	//����ģ������
    BLE_powerOff();

  	//��λ��������
	GPIO_InitTypeDef gpioInit;
	gpioInit.GPIO_Pin 	= BLE_RXD | BLE_TXD | BLE_CTRL;
    gpioInit.GPIO_Speed = GPIO_Speed_400KHz;
    gpioInit.GPIO_Mode 	= GPIO_Mode_OUT;
    gpioInit.GPIO_OType = GPIO_OType_PP;
    gpioInit.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
	GPIO_Init(PORT_OTHER, &gpioInit);

	//��λ��������
	gpioInit.GPIO_Pin 	= BLE_WAKE;
	GPIO_Init(PORT_WAKE, &gpioInit);

	//��λ״̬����
	gpioInit.GPIO_Pin 	= BLE_STATUS0 | BLE_STATUS1;
	GPIO_Init(PORT_OTHER, &gpioInit);

	//�ر���������
	GPIO_WriteBit(PORT_WAKE  ,BLE_WAKE	 ,Bit_RESET);
    GPIO_WriteBit(PORT_OTHER ,BLE_CTRL   ,Bit_RESET);
	GPIO_WriteBit(PORT_OTHER ,BLE_RXD    ,Bit_RESET);
	GPIO_WriteBit(PORT_OTHER ,BLE_TXD    ,Bit_RESET);
    GPIO_WriteBit(PORT_OTHER ,BLE_STATUS0,Bit_RESET);
    GPIO_WriteBit(PORT_OTHER ,BLE_STATUS1,Bit_RESET);

	//�رմ���ģ��
	USART_Cmd(USART1,DISABLE);
	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, DISABLE);

	//�رմ����ж�
	NVIC_InitTypeDef NVIC_InitStructure;
 	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PRI_MCU_USART;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
  	NVIC_Init(&NVIC_InitStructure);

	AllIdleInit();
}
/********************************************************/
/*BLE���ƶ˿ڳ�ʼ��	    								*/
/********************************************************/
void BleInit(void)
{
  	RCC_AHBPeriphClockCmd(CLK_WAKE | CLK_OTHER, ENABLE);
	//�������ų�ʼ��
    GPIO_InitTypeDef gpioInit;
    gpioInit.GPIO_Pin 	= BLE_WAKE | BLE_POWER;
    gpioInit.GPIO_Speed = GPIO_Speed_400KHz;
    gpioInit.GPIO_Mode	= GPIO_Mode_OUT;
    gpioInit.GPIO_OType = GPIO_OType_PP;
    gpioInit.GPIO_PuPd 	= GPIO_PuPd_UP;
    GPIO_Init(PORT_WAKE, &gpioInit);

	//�������ų�ʼ��
	gpioInit.GPIO_Pin 	= BLE_CTRL;
	GPIO_Init(PORT_OTHER, &gpioInit);

	//״̬����0��ʼ��
	gpioInit.GPIO_Pin 	= BLE_STATUS0;
	gpioInit.GPIO_Mode	= GPIO_Mode_IN;
	gpioInit.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
	GPIO_Init(PORT_OTHER, &gpioInit);

	//״̬����1��ʼ��
	gpioInit.GPIO_Pin 	= BLE_STATUS1;
	gpioInit.GPIO_Mode	= GPIO_Mode_IN;
	gpioInit.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
	GPIO_Init(PORT_OTHER, &gpioInit);

	GPIO_WriteBit(PORT_WAKE ,BLE_WAKE,Bit_SET);
	GPIO_WriteBit(PORT_OTHER,BLE_CTRL,Bit_SET);

	GPIO_WriteBit(PORT_OTHER,BLE_RXD	,Bit_SET);
	GPIO_WriteBit(PORT_OTHER,BLE_TXD	,Bit_SET);
}

/********************************************************/
/*���ڳ�ʼ��											*/
/********************************************************/
#pragma optimize = none
void initSerialPort(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

	GPIO_InitTypeDef gpioInit;
	gpioInit.GPIO_Pin  	= BLE_TXD;
	gpioInit.GPIO_Mode 	= GPIO_Mode_AF;
	gpioInit.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_Init(PORT_OTHER, &gpioInit);

	gpioInit.GPIO_Pin  	= BLE_RXD;
	GPIO_Init(PORT_OTHER, &gpioInit);

	USART_InitTypeDef usartInit;

	usartInit.USART_BaudRate 	= 38400;
	usartInit.USART_WordLength 	= USART_WordLength_8b;
	usartInit.USART_StopBits 	= USART_StopBits_1;
	usartInit.USART_Parity 		= USART_Parity_No;
	usartInit.USART_Mode 		= USART_Mode_Rx | USART_Mode_Tx;
	usartInit.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART1,&usartInit);

	USART_ClearFlag(USART1,USART_FLAG_RXNE);
	USART_ClearITPendingBit(USART1,USART_IT_RXNE);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_USART1);

	NVIC_InitTypeDef NVIC_InitStructure;

  	/* Enable the USARTx Interrupt */
 	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PRI_MCU_USART;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);

	USART_Cmd(USART1,ENABLE);

    BLE_powerOn();

    QueueCreate(&Q_SendData, Mstk, sizeof(Mstk));
    BLE_BuffInit();
}

/********************************************************/
/*���ڷ��ͺ���											*/
/********************************************************/
#pragma optimize = none
void USART_sendOneByte(unsigned char data)
{
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET)
	{
		__NOP();
	}

	USART_SendData(USART1,data);
}
#pragma optimize = none
void USART_sendMulByte(unsigned char *buf,unsigned int num)
{
  	unsigned int I = 0;

    for(;I < num;I ++)
    {
      if(I%20 == 0){
        System_delay(50);
      }
      USART_sendOneByte(*(buf + I));
    }
}
/********************************************************/
/*���ڽ����ж�											*/
/********************************************************/
#pragma optimize = none
void USART1_IRQHandler(void)
{
  	uint8 uartRxd;

	if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE) != RESET)
	{
		uartRxd =(uint8)USART_ReceiveData(USART1);

        BLE_WriteBuff(uartRxd);


		//QueuePlpsat(&Q1,uartRxd);
        //USART_SendData(USART1,uartRxd);

		USART_ClearFlag(USART1,USART_FLAG_RXNE);
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
}

/*********************************************************/
void BleWakeUp(void)
{
    GPIO_WriteBit(PORT_WAKE ,BLE_WAKE,Bit_RESET); 	//����8051����
    for(unsigned int i=0;i < DELAY;i++)
      __NOP();
}

/*********************************************************/
void BleLowPower(void)
{
    GPIO_WriteBit(PORT_WAKE ,BLE_WAKE,Bit_SET); 	//����8051����
    for(unsigned int i=0;i < DELAY;i++)
      __NOP();
}

/*********************************************************/
void powerOn(void)//��������RF���� ��ʼ�㲥
{
  	BleWakeUp();
    GPIO_WriteBit(PORT_OTHER ,BLE_CTRL,Bit_RESET);	//�½���
    for(unsigned int i=0;i < DELAY;i++)
      __NOP();
    GPIO_WriteBit(PORT_OTHER ,BLE_CTRL,Bit_SET);	//�ص���
      __NOP();
}

/*********************************************************/
void powerOff(void)
{




}

/*********************************************************/
lpsa_u8 BleCtrl(void)//1:�㲥��0����
{
	lpsa_u8	io = 0;
	if(GPIO_ReadInputDataBit(PORT_OTHER,BLE_STATUS1))
		io |= 2;
	if(GPIO_ReadInputDataBit(PORT_OTHER,BLE_STATUS0))
		io |= 1;
	return io;

}

/*********************************************************/
lpsa_u8 BleBuffCtrl(void)//0�������գ�1������������
{
    if(GPIO_ReadInputDataBit(PORT_OTHER,BLE_STATUS0))
        return 1;
    else
        return 0;
}

/*********************************************************/
void WriteBlock(lpsa_u8 *Data, lpsa_u16 Len)//д��
{
    int i = 0;
    for(; i < Len; i ++)
        QueuePlpsat(&Q_SendData, Data[i]);
    WrStart = 1;
    Flag.WR_key = 1;
}

/*********************************************************/
void ReadBlock(lpsa_u8 *buf, lpsa_u16 size)//���飨�˴�Ԥ���ӿڣ���ʱ�����飩
{
    ;
}

/*********************************************************/
lpsa_u8* ReadChar(void)//�ֽڶ�ȡ�������ݷ���NULL��ʹ����Ϣ���У�Ӧ�ò�ֱ�Ӷ�ȡ��
{
	//return QueueAcc(&Q1);
  	return BLE_ReadBuff();
  	//return NULL;
}

/*********************************************************/
void BleSavePwd(void)
{
	InnerEEPROM_write(((uint8*) &BleTools.PWD), PAIRCODE_ADDR,sizeof(BleTools.PWD));
}

/*********************************************************/
void BleReadPwd(void)
{
  	BleTools.PWD = *((uint16*)PAIRCODE_ADDR);
}
/*********************************************************/
