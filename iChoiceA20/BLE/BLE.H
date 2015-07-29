#ifndef BLE_H
#define BLE_H

#include "lpsa.h"


//#define       TIME_OUT

#define     BLE_IDLE_POWER_ON       0
#define		BLE_IDLE_TIME			1//15����
#define		BLE_IDLE_OPEN			2//������
#define		BLE_IDLE_CLOES			3//�ر�BLE
#define		BLE_IDLE_DELAY			4//��ʱ
#define		BLE_IDLE_COMM		        5//BLE��ʼͨ��
#define		BLE_IDLE_LINK			6



#define		WRITE_BLOCK_BEGIN		0
#define		WRITE_BLOCK_WAIT		1
#define		WRITE_BLOCK_DELAY		2
#define		WRITE_BLOCK_SEND		3
#define		DATA_SEND_BLOCK_BEGIN	4
#define		DATA_SEND_BLOCK_WAIT	5
#define		DATA_SEND_BLOCK_1		6//7������
#define		DATA_SEND_BLOCK_2		7//23���ݷ���
#define		DATA_SEND_BLOCK_DELAY	8
#define 	COMM_SEND_DATA			0//
#define		COMM_GET_DATA_0			1
#define 	COMM_GET_DATA_1			2
#define 	COMM_GET_DATA_2			3
#define		COMM_GET_COMMAND		4//��������
#define		COMM_GET_PWD			5//��Կ��֤
#define		COMM_GET_PWD_RNEW		6//��������


/***********************��ʱ*******************************/
#define		MS_CLEAN	0
#define		MS_SUM		1
#define		MS_READ		2


/*********************BLE���ݽṹ**************************/
#ifdef TIME_OUT
#define	COMM_TIME		6000u//ͨ�ų�ʱ
#define	PWD_TIME		1000u//������֤��ʱ
#define	PWE_RNEW		12000u//�������ó�ʱ
	typedef struct
	{
		lpsa_u16	REG;//��ʱ��־
		lpsa_u16	CommTime;//ͨѶ��ʱ
	
	}TIME_OUT_STRUCT;
#endif
#ifdef DEBUG
	typedef struct
	{
		lpsa_u16	tGB;
	}DEBUG_STRUCT;

#endif
typedef struct
{
	
  unsigned 		Power		:1,//ϵͳ��Դ
				Off			:1,
					
#ifdef	TIME_OUT
				CommTime	:1,//ͨ�ų�ʱ
				PwdTime		:1,//������֤��ʱ
				PwdRnewTime :1,//��������
#endif
                                WR_key,
				Pwd			:2,//������֤0:������֤ 1������ģʽ 2��������
				PwdSus		:1;//���������־λ
}STATUS_STRUCT; 


typedef struct
{
	void		(*powerOn)(void);		//�򿪵�Դ
	void		(*PowerOff)(void);		//�رյ�Դ
	lpsa_u8		(*BleCtrl)(void);		//1:�㲥��0����
	lpsa_u8		(*BleBuffCtrl)(void);	//0�������գ�1������������
	void		(*WriteBlock)(lpsa_u8 *, lpsa_u16);//д��
	void		(*ReadBlock)(lpsa_u8 *, lpsa_u16);//���飨�˴�Ԥ���ӿڣ���ʱ�����飩
	lpsa_u8		*(*ReadChar)(void);		//�ֽڶ�ȡ�������ݷ���NULL��ʹ����Ϣ���У�Ӧ�ò�ֱ�Ӷ�ȡ��
	void		(*BleWakeUp)(void);		//����BLE MCU;
	void		(*BleLowPower)(void);	//����BLE MCU���ߣ�
	void 		(*BleSavePwd)(void);	//�洢����
	void 		(*BleReadPwd)(void);

}BLE_STRUCT;

/********************BLE״̬�����ݽṹ*******************/
typedef struct
{
	lpsa_u8		BLE_IDLE_L;//BLE�͹���ģʽ״̬��
	lpsa_u8		BLE_IDLE_C;//ָ���ȡ״̬��
	lpsa_u8		BLE_IDLE_S;//���ݷ���״̬��
	lpsa_u8		BLE_IDLE_D;//���ݷ���״̬��
	
}BLE_IDLE_STRUCT;

typedef struct
{

	//�͹���ͨ��
	// 0  :ͨ���У�
	// 1  :ʵʱ���ݷ��ͳɹ����˳�����ͨ�ţ�
	//-1  :��BLE30���û�����ӳɹ����˳��������ӵȴ���һ��15������ͨ�ţ�
	//-2  :�ȴ�����ָ�ʱ�����߽���ָ��ʧ�ܣ��˳�����ͨ�ţ�
	//�����ֶ�ͨ��
	//-3  :�ڲ�����״̬�����ң�
	//-4  :�ֶ�ͬ������ָ�ʱ���˳���
	//-5  :�ڲ�����״̬����
	//-6  :�������
	//-7  :������Գ�ʱ
	lpsa_i8		Error;//����״̬��BLEͨ�Ų�����ERROR��

	//ʶ���룬���豸������4λ0-9�������Կ����λ��ÿ��ͨ����Ҫ���ʹ�����
	lpsa_u16	PWD;//������������Ҫ����
	
	//�����ݷ�������
	//0:	�����ݷ���
	//1:	�������ݿ�1
	//...
	//10:	�������ݿ�10
	lpsa_u8		SEND_BLOCK_INFO;
        
	
}BLE_TOOLS;

extern BLE_STRUCT 			BleApi;
extern BLE_TOOLS 			BleTools;
extern BLE_IDLE_STRUCT		Idle;
extern STATUS_STRUCT		Flag;
#ifdef	TIME_OUT
	extern TIME_OUT_STRUCT	TimeOut;
#endif
#ifdef	DEBUG
	extern DEBUG_STRUCT Debug;
#endif

lpsa_u16	BleDelay (lpsa_u8	k);

lpsa_i8		BLE_NOMAL_COMM (void);

/********************��������***********************/
void powerOn(void);
void powerOff(void);
lpsa_u8 BleCtrl(void);//1:�㲥��0����
lpsa_u8 BleBuffCtrl(void);//0�������գ�1������������
void WriteBlock(lpsa_u8 *buf, lpsa_u16 size);//д��
void ReadBlock(lpsa_u8 *buf, lpsa_u16 size);//���飨�˴�Ԥ���ӿڣ���ʱ�����飩
lpsa_u8 *ReadChar(void);//�ֽڶ�ȡ�������ݷ���NULL��ʹ����Ϣ���У�Ӧ�ò�ֱ�Ӷ�ȡ�� 
void BleWakeUp (void);
void BleLowPower (void);
void BleSavePwd (void);
void BleReadPwd (void);
/*********************״̬�����ú���******************/
//��ȡ����
//��������ComData.Com
//����	  CommData.ComLen;
//����	  CommData.Commbuff[];
lpsa_i8		BLE_COMMAND_CTRL (lpsa_u8 *Com, lpsa_u8	*ComLen, lpsa_u8 *Commbuff);
lpsa_i8		DATA_SEND_BLOCK_1_FUN (void);
lpsa_i8		DATA_SEND_BLOCK_2_FUN (void);
void		AllIdleInit (void);


/******************USER���ú���*******************/
#define		BLE_LOW_POWER	0//�͹���״̬����Ļ�ر�
#define		BLE_NORMAL		1//�ֶ�����Ļ
//�˺�������ѭ���в�ͣ����
//�˺������б���ȷ��BleTools.Power = 1;
void BLE_IDLE_RUN(void);

//���ݽṹ��ʼ��
//ϵͳ�ϵ�����һ��
//�˺�����Ҫ�ǳ�ʼ��BLE���ݽṹ�����벿����Ҫ�޸ģ�
//����������0x1234
void BLE_INIT(void);	

//������������ͨ�Ź��ܣ��˺������У�BLE����һ������
//�������ʧ���Զ�����͹���ģʽ���´��������
//Flag.Power = 1,�����д˺������з���ͨ��
//void OPEN_BLE_COMM (void);
//if (Flag.PwdNew)
//	�˳���ǰ���񣬽���������������

void APP_BLE (void);


//ǿ�ƹر�BLE
void BLE_OFF (void);



/********************************************************/
/*BLE���ƶ˿ڳ�ʼ��	    								*/
/********************************************************/
void BleInit(void);

/********************************************************/
/*���ڳ�ʼ��											*/
/********************************************************/
void initSerialPort(void);

/********************************************************/
/*�ر�BLE���ƶ˿�	    								*/
/********************************************************/
void BLE_closePort(void);

void WriteBlock(lpsa_u8 *buf, lpsa_u16 size);//д��
void USART_sendMulByte(lpsa_u8 *buf,unsigned int num);

/*����������*/
void BLE_open(void);

void BLE_close(void);

#endif