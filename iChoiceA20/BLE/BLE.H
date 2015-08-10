#ifndef BLE_H
#define BLE_H

#include "lpsa.h"


//#define       TIME_OUT

#define     BLE_IDLE_POWER_ON       0
#define		BLE_IDLE_TIME			1//15分钟
#define		BLE_IDLE_OPEN			2//打开蓝牙
#define		BLE_IDLE_CLOES			3//关闭BLE
#define		BLE_IDLE_DELAY			4//延时
#define		BLE_IDLE_COMM		        5//BLE开始通信
#define		BLE_IDLE_LINK			6



#define		WRITE_BLOCK_BEGIN		0
#define		WRITE_BLOCK_WAIT		1
#define		WRITE_BLOCK_DELAY		2
#define		WRITE_BLOCK_SEND		3
#define		DATA_SEND_BLOCK_BEGIN	4
#define		DATA_SEND_BLOCK_WAIT	5
#define		DATA_SEND_BLOCK_1		6//7天数据
#define		DATA_SEND_BLOCK_2		7//23数据发送
#define		DATA_SEND_BLOCK_DELAY	8
#define 	COMM_SEND_DATA			0//
#define		COMM_GET_DATA_0			1
#define 	COMM_GET_DATA_1			2
#define 	COMM_GET_DATA_2			3
#define		COMM_GET_COMMAND		4//解析数据
#define		COMM_GET_PWD			5//密钥认证
#define		COMM_GET_PWD_RNEW		6//重新配置


/***********************延时*******************************/
#define		MS_CLEAN	0
#define		MS_SUM		1
#define		MS_READ		2


/*********************BLE数据结构**************************/
#ifdef TIME_OUT
#define	COMM_TIME		6000u//通信超时
#define	PWD_TIME		1000u//密码验证超时
#define	PWE_RNEW		12000u//密码重置超时
	typedef struct
	{
		lpsa_u16	REG;//延时标志
		lpsa_u16	CommTime;//通讯超时
	
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
	
  unsigned 		Power		:1,//系统电源
				Off			:1,
					
#ifdef	TIME_OUT
				CommTime	:1,//通信超时
				PwdTime		:1,//密码验证超时
				PwdRnewTime :1,//密码重置
#endif
                                WR_key,
				Pwd			:2,//密码验证0:密码验证 1：运行模式 2：新密码
				PwdSus		:1;//密码重设标志位
}STATUS_STRUCT; 


typedef struct
{
	void		(*powerOn)(void);		//打开电源
	void		(*PowerOff)(void);		//关闭电源
	lpsa_u8		(*BleCtrl)(void);		//1:广播，0链接
	lpsa_u8		(*BleBuffCtrl)(void);	//0缓冲区空，1缓冲区有数据
	void		(*WriteBlock)(lpsa_u8 *, lpsa_u16);//写块
	void		(*ReadBlock)(lpsa_u8 *, lpsa_u16);//读块（此处预留接口，到时在商议）
	lpsa_u8		*(*ReadChar)(void);		//字节读取，无数据返回NULL，使用消息队列，应用层直接读取。
	void		(*BleWakeUp)(void);		//唤醒BLE MCU;
	void		(*BleLowPower)(void);	//设置BLE MCU休眠；
	void 		(*BleSavePwd)(void);	//存储密码
	void 		(*BleReadPwd)(void);

}BLE_STRUCT;

/********************BLE状态机数据结构*******************/
typedef struct
{
	lpsa_u8		BLE_IDLE_L;//BLE低功耗模式状态机
	lpsa_u8		BLE_IDLE_C;//指令获取状态机
	lpsa_u8		BLE_IDLE_S;//数据发送状态机
	lpsa_u8		BLE_IDLE_D;//数据发送状态机
	
}BLE_IDLE_STRUCT;

typedef struct
{

	//低功耗通信
	// 0  :通信中；
	// 1  :实时数据发送成功，退出本次通信；
	//-1  :打开BLE30秒后没能连接成功，退出本次连接等待下一个15分钟在通信；
	//-2  :等待接收指令超时，或者解析指令失败，退出本次通信；
	//开屏手动通信
	//-3  :内部错误，状态机混乱；
	//-4  :手动同步接收指令超时，退出；
	//-5  :内部错误，状态混乱
	//-6  :密码错误
	//-7  :重新配对超时
	lpsa_i8		Error;//关屏状态下BLE通信产生的ERROR；

	//识别码，是设备产生的4位0-9的随机密钥，上位机每次通信需要发送此密码
	lpsa_u16	PWD;//此密码掉电后需要保存
	
	//块数据发送属性
	//0:	无数据发送
	//1:	发送数据块1
	//...
	//10:	发送数据块10
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

/********************驱动函数***********************/
void powerOn(void);
void powerOff(void);
lpsa_u8 BleCtrl(void);//1:广播，0链接
lpsa_u8 BleBuffCtrl(void);//0缓冲区空，1缓冲区有数据
void WriteBlock(lpsa_u8 *buf, lpsa_u16 size);//写块
void ReadBlock(lpsa_u8 *buf, lpsa_u16 size);//读块（此处预留接口，到时在商议）
lpsa_u8 *ReadChar(void);//字节读取，无数据返回NULL，使用消息队列，应用层直接读取。 
void BleWakeUp (void);
void BleLowPower (void);
void BleSavePwd (void);
void BleReadPwd (void);
/*********************状态机调用函数******************/
//获取数据
//数据类型ComData.Com
//长度	  CommData.ComLen;
//数据	  CommData.Commbuff[];
lpsa_i8		BLE_COMMAND_CTRL (lpsa_u8 *Com, lpsa_u8	*ComLen, lpsa_u8 *Commbuff);
lpsa_i8		DATA_SEND_BLOCK_1_FUN (void);
lpsa_i8		DATA_SEND_BLOCK_2_FUN (void);
void		AllIdleInit (void);


/******************USER调用函数*******************/
#define		BLE_LOW_POWER	0//低功耗状态，屏幕关闭
#define		BLE_NORMAL		1//手动打开屏幕
//此函数放在循环中不停调用
//此函数运行必须确定BleTools.Power = 1;
void BLE_IDLE_RUN(void);

//数据结构初始化
//系统上电运行一次
//此函数主要是初始化BLE数据结构，密码部分需要修改，
//现在密码是0x1234
void BLE_INIT(void);	

//开启蓝牙单次通信功能，此函数运行，BLE发起一次连接
//如果连接失败自动进入低功耗模式，下次运行需打开
//Flag.Power = 1,在运行此函数进行发起通信
//void OPEN_BLE_COMM (void);
//if (Flag.PwdNew)
//	退出当前任务，进入密码重置任务

void APP_BLE (void);


//强制关闭BLE
void BLE_OFF (void);



/********************************************************/
/*BLE控制端口初始化	    								*/
/********************************************************/
void BleInit(void);

/********************************************************/
/*串口初始化											*/
/********************************************************/
void initSerialPort(void);

/********************************************************/
/*关闭BLE控制端口	    								*/
/********************************************************/
void BLE_closePort(void);

void WriteBlock(lpsa_u8 *buf, lpsa_u16 size);//写块
void USART_sendMulByte(lpsa_u8 *buf,unsigned int num);

/*打开蓝牙开关*/
void BLE_open(void);

void BLE_close(void);

#endif