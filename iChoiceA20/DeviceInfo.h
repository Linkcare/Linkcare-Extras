#ifndef __DEVICEINFO_H
#define __DEVICEINFO_H

#define	 OFF	0
#define	 ON 	1

//A20�豸����
#define  A20	   			 2			//A20�豸����
#define  FIRMWARE_ISSUED	"V1.0.5"	//���������汾
#define  FIRMWARE_DEVELOPER	"V2.0.55"	//���������汾

//�Ƿ�ʹ�ÿ��Ź�
#define  WATCHDOG	OFF

//�Ƿ�ʹ���ļ�����
#define  BACKUP		OFF

//�Ƿ�ʹ�ô���
#define  TAP		OFF

//�Ƿ�����ʵʱͨ��
#define	 REALTIME	OFF

//�Ƿ���������ģʽ
#define	 A20_DEBUG	OFF

/*��ʱ����������*/
#define  SCREEN_ON     		(5 * 1000UL)	//�豸��������ʱ�䣬ms

#define  DEVICE_ON     		(30* 1000UL)	//�豸�����޶�������ʱ��,ms

#define  LONGKEY_TIMER		2000UL			//������ʱ����ms

#define  SCREEN_REFRESH		200UL			//��Ļˢ��ʱ�䣬ms

#define  BATTERY_CHECK		(20 * 1000UL)	//���ص��ص�����������,ms

#define	 RTC_WORK_TIME	 	30				//TRACKER and SLEEPģʽ�£�RTC�������ڣ�ms
#define	 RTC_STOP_TIME	  	5000			//STOPģʽ�£�RTC�������ڣ�ms

#define  TURNOFF_PERIOD		(6000UL)		//�ػ�ʱ���ж�����ֵ,��λ��ms

/*�ζ�����������*/
#define	 SENSE_DEFINE		SENSE_LEVEL1	//���ٶȴ����������ȶ���

#define	 SENSE_LEVEL0		0x06	//��������
#define	 SENSE_LEVEL1		0x08	//һ������
#define	 SENSE_LEVEL2		0x0A	//��������
#define	 SENSE_LEVEL3		0x14	//˦������
#define	 SENSE_LEVEL4		0x30	//һ˯����

/*�ػ�����*/
#define  LITTLE_OFF			0x00	//ǳ�ȹػ�
#define  DEEPLY_OFF			0x01	//���ȹػ�

//�¶ȵ�λ���Ͷ���
typedef enum
{
  	TEMP_C  = 0x00, //�����±�
  	TEMP_F  = 0x01	//�����±�

}TEMP_TypeDef;

//���뵥λ���Ͷ���
typedef enum
{
  	METRIC    = 0x00, 	//���Ƶ�λ
  	IMPERIAL  = 0x01	//Ӣ�Ƶ�λ

}LUINT_TypeDef;

//�Ա����Ͷ���
typedef enum
{
  	NON   = 0x01, 	//����
  	MAN   = 0x02,	//��
	WOMAN = 0x03  	//Ů

}GENDAR_TypeDef;

//ʱ�������Ͷ���
typedef enum
{
  	Time_24  = 0x00,
  	Time_12  = 0x01

}TIME_FormatDef;

//ʱ�����Ͷ���
typedef enum
{
  	Time_AM  = 0x00,
  	Time_PM  = 0x01

}TIME_PeriodDef;

//��ʱ���Ͷ���
typedef enum
{
  	NO_TIMEOUT  = 0x00,
  	IS_TIMEOUT  = 0x01

}TIMEOUT_TypeDef;

#endif
