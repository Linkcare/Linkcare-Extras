#include "BLE.h"
#include "RTC.h"
#include "Type.h"
#include "System.h"
#include "string.h"
#include "Battery.h"
#include "Display.h"
#include "stm32l1xx.h"
#include "Upload.h"
#include "Configuration.h"

/*************************临时变量**************************/

//参数解析缓冲区
static UserConfig 	sysCfg;			//设置单位制结构体
static Calendar   	calendar;       //时间设置结构体
static UserInfo   	userInfo;   	//用户信息设置结构体

extern uint8 COM_REV_OK[5];
extern uint8 COM_REV_ER[5];
extern uint8 COM_REV_HE[5];
extern uint8 COM_PWD_OK[6];
extern uint8 COM_PWD_ER[6];
/****************************************************/
/*指令执行成功										*/
/***************************************************/
void excuteSuccess(void)
{
	WriteBlock(COM_REV_OK,sizeof(COM_REV_OK));
	
	return;
}

/****************************************************/
/*指令执行失败										*/
/***************************************************/
void excutefail(void)
{
	WriteBlock(COM_REV_ER,sizeof(COM_REV_ER));
	
	return;
}

/****************************************************/
/*硬件故障											*/
/***************************************************/
void hardwareError(void)
{
	WriteBlock(COM_REV_HE,sizeof(COM_REV_HE));
	
	return;
}

/****************************************************/
/*计算校验和										*/
/*计算从*buf开始len个数据的加和						*/
/*返回：校验和										*/
/****************************************************/
uint8 checkSum(const uint8 *buf,const uint8 len)
{
    uint8 num = 0;
    for(uint8 temp = 0;temp < len;temp++)
    {
        num += buf[temp];
    }
	
    return num;
}

/****************************************************/
/*获取ID号											*/
/****************************************************/
void c0_Process(void)
{ 
    uint8 cmd[11];
    uint32 tempID = UserConfig_readID();
	cmd[0] = 0x55;
	cmd[1] = 0xAA;
	cmd[2] = 8;
    cmd[3] = 0xA0;
    cmd[4] = deviceType;	//设备类型
    cmd[5] = 0;				//预留
    cmd[6] = (uint8)(tempID & 0xFF);
    cmd[7] = (uint8)((tempID & 0xFF00)>>8);
    cmd[8] = (uint8)((tempID & 0xFF0000)>>16);
    cmd[9] = (uint8)((tempID & 0xFF000000)>>24);
	cmd[10] = checkSum(&cmd[2],8);
    WriteBlock(cmd,sizeof(cmd));
	
	return;
}

/****************************************************/
/*获取版本号										*/
/****************************************************/
void c1_Process(void)
{
    uint8 cmd[16];
	cmd[0] = 0x55;
	cmd[1] = 0xAA;
	cmd[2] = 13;
    cmd[3] = 0xA1;
	memcpy(&cmd[4],version_issued,sizeof(version_issued));
	cmd[15] = checkSum(&cmd[2],13);
    WriteBlock(cmd,sizeof(cmd));//把生成的命令保存到发送缓冲区
	
	return;
}

/****************************************************/
/*设置系统时间										*/
/****************************************************/
void c2_Process(lpsa_u8 *ComLen, lpsa_u8 *Commbuff)
{
    calendar.date.RTC_Year 		= *Commbuff;
    calendar.date.RTC_Month 	= *(Commbuff + 1);
    calendar.date.RTC_Date		= *(Commbuff + 2);
  	calendar.date.RTC_WeekDay 	= *(Commbuff + 3);
	calendar.time.RTC_Hours 	= *(Commbuff + 4);
	calendar.time.RTC_Minutes 	= *(Commbuff + 5);
	calendar.time.RTC_Seconds	= *(Commbuff + 6);
	
	if(RTC_setTime(calendar.date,calendar.time) == SUCCESS)
	{
		excuteSuccess();
	}
	else
	{
		excutefail();
	}
	
	return;
}

/****************************************************/
/*设置用户信息										*/
/****************************************************/
void c3_Process(lpsa_u8 *ComLen, lpsa_u8 *Commbuff)
{
  	Calendar temp;
	RTC_getCalendar(&temp);

	userInfo.gender 		= (GENDAR_TypeDef)*Commbuff;
	userInfo.birthdayYear 	= *(Commbuff + 1) + (*(Commbuff + 2) << 8);
	userInfo.birthdayMon 	= *(Commbuff + 3);
	userInfo.birthdayDay 	= *(Commbuff + 4);
	userInfo.height 		= *(Commbuff + 5);
	userInfo.weight 		= *(Commbuff + 6) + (*(Commbuff + 7) << 8);
	userInfo.age 			= temp.date.RTC_Year + 2000 - userInfo.birthdayYear;
	
	if(UserConfig_setUserInfo(&userInfo) == SUCCESS)
	{
		excuteSuccess();
	}
	else
	{
		excutefail();
	}
	
	return;
}

/****************************************************/
/*设置时区											*/
/****************************************************/
void c4_Process(lpsa_u8 *ComLen, lpsa_u8 *Commbuff)
{
    sysCfg.timeZone.hour = *Commbuff;		 // 小时时区
    sysCfg.timeZone.min  = *(Commbuff + 1);  // 分钟时区
	
	if(UserConfig_setTimeZone(sysCfg.timeZone) == SUCCESS)
	{
		excuteSuccess();
	}
	else
	{
		excutefail();
	}
	
	return;
}

/****************************************************/
/*设置时间制										*/
/****************************************************/
void c5_Process(lpsa_u8 *ComLen, lpsa_u8 *Commbuff)
{
  	sysCfg.timeFormat = (TIME_FormatDef)*Commbuff;
	
	if(UserConfig_setTimeFormat(sysCfg.timeFormat) == SUCCESS)
	{
		excuteSuccess();
	}
	else
	{
		excutefail();
	}
	
	return;
}

/****************************************************/
/*设置问候语										*/
/****************************************************/
void c6_Process(lpsa_u8 *ComLen, lpsa_u8 *Commbuff)
{
  	memcpy(sysCfg.greeting,Commbuff,sizeof(sysCfg.greeting));
	
	if(UserConfig_setGreeting(sysCfg.greeting,sizeof(sysCfg.greeting)) == SUCCESS)
	{
		excuteSuccess();
	}
	else
	{
		excutefail();
	}
	
	return;
}

/****************************************************/
/*设置显示顺序										*/
/****************************************************/
void c7_Process(lpsa_u8 *ComLen, lpsa_u8 *Commbuff)
{
  	memcpy(sysCfg.displayOrder,Commbuff,sizeof(sysCfg.displayOrder));
    
	if(UserConfig_setDispOrder(sysCfg.displayOrder,sizeof(sysCfg.displayOrder)) == SUCCESS)
	{
		excuteSuccess();
	}
	else
	{
		excutefail();
	}
		
	return;
}

/****************************************************/
/*//设置目标步数									*/
/****************************************************/
void c8_Process(lpsa_u8 *ComLen, lpsa_u8 *Commbuff)
{
  	sysCfg.goalStep = *Commbuff + (*(Commbuff + 1) << 8) + ((*(Commbuff + 2) << 8) << 8) + (((*(Commbuff + 3) << 8) << 8) <<8);
	
	if(UserConfig_setGoalStep(sysCfg.goalStep) == SUCCESS)
	{
		excuteSuccess();
	}
	else
	{
		excutefail();
	}
	
	return;
}

/****************************************************/
/*获取运动数据										*/
/****************************************************/
void c9_Process(lpsa_u8 *ComLen, lpsa_u8 *Commbuff)
{	
    switch(Commbuff[0])
    {
        case 0x00:
        {
			BleTools.SEND_BLOCK_INFO = 1;
        }
		break;
		  
		case 0x01:
        {
			BleTools.SEND_BLOCK_INFO = 2;
        }
		break;
		
        case 0x02:
			break;
			
        default:break;
    }
}

/****************************************************/
/*设置显示的单位制									*/
/****************************************************/
void cb_Process(lpsa_u8 *ComLen, lpsa_u8 *Commbuff)
{
  	sysCfg.distanceUnit = (LUINT_TypeDef)*Commbuff;

	if(UserConfig_setDistanceUnit(sysCfg.distanceUnit) == SUCCESS)
	{
		excuteSuccess();
	}
	else
	{
		excutefail();
	}
}

/****************************************************/
/*设置显示的温度单位								*/
/****************************************************/
void cc_Process(lpsa_u8 *ComLen, lpsa_u8 *Commbuff)
{
  	sysCfg.temperUnit = (TEMP_TypeDef)*Commbuff;

	if(UserConfig_setTemperatureUnit(sysCfg.temperUnit) == SUCCESS)
	{
		excuteSuccess();
	}
	else
	{
		excutefail();
	}
}

/****************************************************/
/*获取电池电量									*/
/****************************************************/
void ce_Process(void)
{
  	uint8 cmd[6];
	uint8 battery_degree = 0;
	battery_degree = Battery_getBatteryLevel();
	
	cmd[0] = 0x55;
	cmd[1] = 0xAA;
	cmd[2] = 3;
	cmd[3] = 0xB9;
	cmd[4] = battery_degree;
	cmd[5] = checkSum(&cmd[2],3);
    WriteBlock(cmd,sizeof(cmd));
}

/****************************************************/
/*获取当前时钟										*/
/****************************************************/
void d2_Process(void)
{
    uint8 cmd[11];
	Calendar tracker_time;
	RTC_getCalendar(&tracker_time);
	
	cmd[0] = 0x55;
	cmd[1] = 0xAA;
	cmd[2] = 8;
	cmd[3] = 0xC2;
	cmd[4] = tracker_time.date.RTC_Year;
	cmd[5] = tracker_time.date.RTC_Month;
	cmd[6] = tracker_time.date.RTC_Date;
	cmd[7] = tracker_time.time.RTC_Hours;
	cmd[8] = tracker_time.time.RTC_Minutes;
	cmd[9] = tracker_time.time.RTC_Seconds;
	cmd[10] = checkSum(&cmd[2],8);
    WriteBlock(cmd,sizeof(cmd));
}

/****************************************************/
/*清空数据											*/
/****************************************************/
void e0_Process(void)
{
  	//回复确认信息
	excuteSuccess();	

	//上传完毕后存储模块初始化	
	DataSave_initAfterUpload();
	
	//上传模块初始化
	Upload_initAfterUpload();
	
}

/****************************************************/
/*发送7天数据										*/
/****************************************************/
int8 DATA_SEND_BLOCK_1_FUN (void)
{
    static uint8 wake = 0;
	
    if(wake == 0)
    {
        wake = 1;
        BleWakeUp();
        System_delay(10);
    }
	  
    if(BleCtrl() == 3)//缓冲区满
    {
		return 0;
    }
    else if (BleCtrl() == 2)
    {
	  	Upload_PocketDef *data = NULL;
		uint8 cmd[FRAME_LENGTH_7 + 6];
		
		//获取一条用户活动数据（7天数据）
		data = Upload_readDataOf7Days();
		
		if(data == NULL)
		{
			excuteSuccess();	//上传完毕,返回成功
			wake = 0;
			BleLowPower();
			return 1;
		}
		else
		{	
		  	memset(cmd,0,sizeof(cmd));
		
		  	cmd[0] = 0x55;
			cmd[1] = 0xAA;
			cmd[2] = FRAME_LENGTH_7 + 3;
			cmd[3] = 0xC3;
			cmd[4] = data->length;
		  
		  	for(uint8 i = 0;i < data->length;i++)
			{
			 	cmd[i + 5] =  *((uint8*)(data->buffer + i));
			}
		  
			cmd[FRAME_LENGTH_7 + 5] = checkSum(&cmd[2],sizeof(cmd) - 3);
			
			USART_sendMulByte(cmd,sizeof(cmd));	//把生成的命令保存到发送缓冲区		
			
			return 0;
		}
    } 
	else
	{
		return 0;
	}
}

/****************************************************/
/*发送23天数据										*/
/****************************************************/
int8 DATA_SEND_BLOCK_2_FUN (void)
{
    static int8 wake = 0;
	
    if(wake == 0)
    {
        wake = 1;
        BleWakeUp();
        System_delay(10);
    }
    if(BleCtrl() == 3)//缓冲区满
    {
		return 0;
    }
    else if (BleCtrl() == 2)
    {
		Upload_PocketDef *data = NULL;
		uint8 cmd[FRAME_LENGTH_23 + 6];
		
		//获取一条用户活动数据（23天数据）
		data = Upload_readDataOf23Days();
		
		if(data == NULL)
		{
		  	//上传完毕,返回成功
			excuteSuccess();	
			wake = 0;
			BleLowPower();
			
			return 1;
		}
		else
		{	
		  	memset(cmd,0,sizeof(cmd));
		
		  	cmd[0] = 0x55;
			cmd[1] = 0xAA;
			cmd[2] = FRAME_LENGTH_23 + 3;
			cmd[3] = 0xC4;
			cmd[4] = data->length;
		  
		  	for(uint8 i = 0;i < data->length;i++)
			{
			 	cmd[i + 5] =  *((uint8*)(data->buffer + i));
			}
		  
			cmd[FRAME_LENGTH_7 + 5] = checkSum(&cmd[2],sizeof(cmd) - 3);
			
			USART_sendMulByte(cmd,sizeof(cmd));	//把生成的命令保存到发送缓冲区	
			
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

/*********************************************************
*	命令解析函数
*	param：Com:数据种类
*		   ComLen:数据长度
*		   Commbuff:数据
*	return：1:退出 
*			0：运行中
*********************************************************/
int8 BLE_COMMAND_CTRL (lpsa_u8 *Com, lpsa_u8 *ComLen, lpsa_u8 *Commbuff)
{
  	lpsa_u8 temp = 0;
    uint8_t len = *ComLen;
	
    for(lpsa_u8 i = 0;i < *ComLen - 1;i++)
    {
        temp += Commbuff[i];
    }
	
    temp =temp + *ComLen + 1 + *Com;
	
    if(temp != Commbuff[len - 1])//校验和错误
    {
        hardwareError();
        return 0;
    }    
    
    switch(*Com)
    {
        case 0xc0://获取ID号
		  		  	c0_Process();
				  	break;					
        case 0xc1://获取软件版本
		  		  	c1_Process();
				  	break;					
        case 0xc2://设置时钟
		  			c2_Process(ComLen,Commbuff);
		  			break;	
        case 0xc3://设置用户信息
		  			c3_Process(ComLen,Commbuff);
		  			break;	
        case 0xc4://设置时区
		  			c4_Process(ComLen,Commbuff);
		  			break;	
        case 0xc5://设置时间制
		  			c5_Process(ComLen,Commbuff);
		  			break;	
        case 0xc6://设置问候语
		  			c6_Process(ComLen,Commbuff);
		  			break;	
        case 0xc7://设置界面顺序
		  			c7_Process(ComLen,Commbuff);
		  			break;	
        case 0xc8://设置用户目标步数
		  			c8_Process(ComLen,Commbuff);
		  			break;	
        case 0xc9://获取数据
		  
		  			//初始化相关模块，为上传做好准备
					Upload_getReadyForUpload();
					DataSave_getReadyForUpload();
					
		  			c9_Process(ComLen,Commbuff);
		  			break;	
        case 0xcb://设置长度单位
		  			cb_Process(ComLen,Commbuff);
		  			break;	
        case 0xcc://设置温度单位
		  			cc_Process(ComLen,Commbuff);
		  			break;    
        case 0xce://获取电池电量
		  			ce_Process();
					break;					
        case 0xd2://获取TRACKER时间
		  			d2_Process();
					break;								
        case 0xe0://清空数据
					e0_Process();
					break;
		case 0xef:
		  			break;
		default:
		  			excutefail();
					break;
    }
	
    return 0;
}