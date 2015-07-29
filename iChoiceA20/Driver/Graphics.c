/*********************************************************/
/*modified by liubingyan*/
/*********************************************************/
#include "stm32l1xx.h"
#include "Character.h"
#include "Graphics.h"
#include "string.h"
#include "System.h"
#include "IIC.h"

//屏幕控制指令表
static uint8 OledCommandBuff[25] = 
{
  	0xAE,	//Display off
	0XD5,	//Set Display Clock Divide Ratio/Oscillator Frequency
	0X82,
	0XA8,	//Set Multiplex Ratio
	0X1F,	
	0XD3,	//Set Display Offset
	0X00,
	0X40,	//Set Display Start Line
	0X8D,	//Set Display Offset
	0X14,	//Enable Charge Pump
  	0XA1,	//Set Segment Re-map
	0XC8,	//Set COM Output Scan Direction
	0XDA,	//Set COM Pins Hardware Configuration
	0X02,
	0X81,	//Set Contrast Control
	0X00,
	0XD9,	//Set Pre-charge Period
	0XF1,
	0XDB,	//Set VCOMH Deselect Level
	0X40,
  	0XA4,	//Output follows RAM content
	0XA6,	//Set Normal Display
	0X20,	//Set Memory Addressing Mode
	0X00,
	0xAF	//Set Display ON
};

//屏幕开关状态
static Graphics_stateDef isDisplayOn = DISPLAY_OFF;

//屏幕缓冲区
static uint8 screenBuffer[SCREEN_PAGE][SCREEN_LENGTH];		

// 函数功能：向屏幕发送指令
// 输入参数：uint8 command
// 输出参数：无
// 返 回 值：无
// 备    注：无
void writeCommand(uint8 command)
{
  	IIC_writeByte(0x78, 0, command);
}

// 函数功能：获取屏幕状态
// 输入参数：无
// 输出参数：无
// 返 回 值：Graphics_stateDef
// 备    注：无
Graphics_stateDef Graphics_isDisplayOn(void)
{
    return isDisplayOn;
}

// 函数功能：清空屏幕缓冲区
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：无
void Graphics_clearBuffer(void)
{
	memset(screenBuffer, 0, sizeof(screenBuffer));
}

// 函数功能：关闭屏幕
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：无
void Graphics_displayOff(void)
{
	if(isDisplayOn == DISPLAY_OFF)
	{
		return;
	}
	else
    {
	  	Graphics_clearScreen();
		
        Graphics_DISABLE();
		
		IIC_disable();
	
    	isDisplayOn = DISPLAY_OFF;
			
    	return;
   	}

}

// 函数功能：设置屏幕的写指针
// 输入参数：	page - 页码
//	       		col - 列号
// 输出参数：无
// 返 回 值：无
// 备    注：无
static void setAddress(uint8 page, uint8 col)
{
    if(page > SCREEN_PAGE - 1 || col > SCREEN_LENGTH - 1)
        return;
	
    uint8 p =(uint8)page;
    uint8 c =(uint8)col;       
    uint8 Command[3];
    Command[0] = 0xB0 | p;
    Command[1] = c & 0x0F;
    Command[2] = 0x10 |(c >> 4);
    _IIC_writeBlockCommand(Command, 3);
	
    return;
}

// 函数功能：更新屏幕
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：无
void Graphics_repaint(void)
{
    if(isDisplayOn == DISPLAY_OFF)
	{
        return;
	}
    
    setAddress(0,0);
	
    _IIC_writeBlock(screenBuffer[0], 512);
    
    return;
}

// 函数功能：清屏
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：无
void Graphics_clearScreen(void)
{
    memset(screenBuffer, 0, sizeof(screenBuffer));
	
    if(isDisplayOn == DISPLAY_OFF)
	{
        return;
	}
	
    setAddress(0,0);
	
    _IIC_writeBlock(screenBuffer[0], 512);
	
	writeCommand(0xAE);
	
    return;
}

// 函数功能：屏幕初始化
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：无
void Graphics_init(void)
{
	IIC_enable();
	
    Graphics_ENABLE();
	
    Graphics_RESET();
    Graphics_NORMAL();
	
	System_delay(5);
    
    _IIC_writeBlockCommand(OledCommandBuff, sizeof(OledCommandBuff));
    
    isDisplayOn = DISPLAY_ON;
	
    memset(screenBuffer, 0, sizeof(screenBuffer));
	
    setAddress(0,0);
	
    _IIC_writeBlock(screenBuffer[0], 512);         	
}

// 函数功能：显示字符
// 输入参数：	x -- 横坐标
//				data -- 待显示字符的ASIIC码
// 输出参数：无
// 返 回 值：无
// 备    注：无
void Graphics_DispChar(uint8 x,uint8 data)
{
    uint8 page = 0; 	
    data = Speed[data - 0x20];
    screenBuffer[0][x+0] = ASIIC_NEW[data][page ++];
    screenBuffer[0][x+1] = ASIIC_NEW[data][page ++];
    screenBuffer[0][x+2] = ASIIC_NEW[data][page ++];
    screenBuffer[0][x+3] = ASIIC_NEW[data][page ++];
    screenBuffer[0][x+4] = ASIIC_NEW[data][page ++];
    screenBuffer[0][x+5] = ASIIC_NEW[data][page ++];
    screenBuffer[0][x+6] = ASIIC_NEW[data][page ++];
    screenBuffer[0][x+7] = ASIIC_NEW[data][page ++];
    screenBuffer[0][x+8] = ASIIC_NEW[data][page ++];
    screenBuffer[0][x+9] = ASIIC_NEW[data][page ++];
    screenBuffer[0][x+10] = ASIIC_NEW[data][page ++];
    screenBuffer[0][x+11] = ASIIC_NEW[data][page ++];
    screenBuffer[0][x+12] = ASIIC_NEW[data][page ++];
    screenBuffer[1][x+0] = ASIIC_NEW[data][page ++];
    screenBuffer[1][x+1] = ASIIC_NEW[data][page ++];
    screenBuffer[1][x+2] = ASIIC_NEW[data][page ++];
    screenBuffer[1][x+3] = ASIIC_NEW[data][page ++];
    screenBuffer[1][x+4] = ASIIC_NEW[data][page ++];
    screenBuffer[1][x+5] = ASIIC_NEW[data][page ++];
    screenBuffer[1][x+6] = ASIIC_NEW[data][page ++];
    screenBuffer[1][x+7] = ASIIC_NEW[data][page ++];
    screenBuffer[1][x+8] = ASIIC_NEW[data][page ++];
    screenBuffer[1][x+9] = ASIIC_NEW[data][page ++];
    screenBuffer[1][x+10] = ASIIC_NEW[data][page ++];
    screenBuffer[1][x+11] = ASIIC_NEW[data][page ++];
    screenBuffer[1][x+12] = ASIIC_NEW[data][page ++];
    screenBuffer[2][x+0] = ASIIC_NEW[data][page ++];
    screenBuffer[2][x+1] = ASIIC_NEW[data][page ++];
    screenBuffer[2][x+2] = ASIIC_NEW[data][page ++];
    screenBuffer[2][x+3] = ASIIC_NEW[data][page ++];
    screenBuffer[2][x+4] = ASIIC_NEW[data][page ++];
    screenBuffer[2][x+5] = ASIIC_NEW[data][page ++];
    screenBuffer[2][x+6] = ASIIC_NEW[data][page ++];
    screenBuffer[2][x+7] = ASIIC_NEW[data][page ++];
    screenBuffer[2][x+8] = ASIIC_NEW[data][page ++];
    screenBuffer[2][x+9] = ASIIC_NEW[data][page ++];
    screenBuffer[2][x+10] = ASIIC_NEW[data][page ++];
    screenBuffer[2][x+11] = ASIIC_NEW[data][page ++];
    screenBuffer[2][x+12] = ASIIC_NEW[data][page ++];
    screenBuffer[3][x+0] = ASIIC_NEW[data][page ++];
    screenBuffer[3][x+1] = ASIIC_NEW[data][page ++];
    screenBuffer[3][x+2] = ASIIC_NEW[data][page ++];
    screenBuffer[3][x+3] = ASIIC_NEW[data][page ++];
    screenBuffer[3][x+4] = ASIIC_NEW[data][page ++];
    screenBuffer[3][x+5] = ASIIC_NEW[data][page ++];
    screenBuffer[3][x+6] = ASIIC_NEW[data][page ++];
    screenBuffer[3][x+7] = ASIIC_NEW[data][page ++];
    screenBuffer[3][x+8] = ASIIC_NEW[data][page ++];
    screenBuffer[3][x+9] = ASIIC_NEW[data][page ++];
    screenBuffer[3][x+10] = ASIIC_NEW[data][page ++];
    screenBuffer[3][x+11] = ASIIC_NEW[data][page ++];
    screenBuffer[3][x+12] = ASIIC_NEW[data][page ++];
}

// 函数功能：显示字符串
// 输入参数：	x -- 横坐标
//				buf -- 待显示字符串
// 输出参数：无
// 返 回 值：无
// 备    注：无
void Graphics_DispString(uint8 x, int8 *buf)
{
  	uint8 sum = 0;
  	uint8 i = 0;
  	while(1)
  	{
    	Graphics_DispChar(x + sum, buf[i++]);
    	sum += 15;
    	if(buf[i] == '\0')
      		return;
  	}
}

// 函数功能：显示数字
// 输入参数：	x -- 横坐标
//				data -- 待显示数字
// 输出参数：无
// 返 回 值：无
// 备    注：无
void Graphics_DispStringDec(uint8 x,int32 data)
{
	uint8 dis[6];
	uint8 minusFlag = 0;
	uint8 sum = 0;
	if(data < 0)
	{
		data = -data;
		minusFlag = 1;
		Graphics_DispChar(x, '-');
	}
	
	dis[5] = data / 100000; 		
	dis[4] =(data % 100000)/10000; 	
	dis[3] =(data % 10000)/1000;   	
	dis[2] =(data % 1000)/100; 		
	dis[1] =(data % 100)/10; 	  	
	dis[0] =(data % 10);
	
	if(data > 99999)
	{  
		Graphics_DispChar(x + sum + minusFlag * 15, dis[5]+0x30);
		sum += 15;
		Graphics_DispChar(x + sum + minusFlag * 15, dis[4]+0x30);
		sum += 15;
		Graphics_DispChar(x + sum + minusFlag * 15, dis[3]+0x30);
		sum += 15;
		Graphics_DispChar(x + sum + minusFlag * 15, dis[2]+0x30);
		sum += 15;
		Graphics_DispChar(x + sum + minusFlag * 15, dis[1]+0x30);
		sum += 15;
		Graphics_DispChar(x + sum + minusFlag * 15, dis[0]+0x30);
		sum += 15;
  	}
	else if(data < 100000 && data > 9999)
	{ 
		Graphics_DispChar(x + sum + minusFlag * 15, dis[4]+0x30);
		sum += 15;
		Graphics_DispChar(x + sum + minusFlag * 15, dis[3]+0x30);
		sum += 15;
		Graphics_DispChar(x + sum + minusFlag * 15, dis[2]+0x30);
		sum += 15;
		Graphics_DispChar(x + sum + minusFlag * 15, dis[1]+0x30);
		sum += 15;
		Graphics_DispChar(x + sum + minusFlag * 15, dis[0]+0x30);
		sum += 15;  
  	}
  	else if(data < 10000 && data > 999)
  	{  
		Graphics_DispChar(x + sum + minusFlag * 15, dis[3]+0x30);
		sum += 15;
		Graphics_DispChar(x + sum + minusFlag * 15, dis[2]+0x30);
		sum += 15;
		Graphics_DispChar(x + sum + minusFlag * 15, dis[1]+0x30);
		sum += 15;
		Graphics_DispChar(x + sum + minusFlag * 15, dis[0]+0x30);
		sum += 15;    
  	}
  	else if(data < 1000 && data > 99)
  	{           
		Graphics_DispChar(x + sum + minusFlag * 15, dis[2]+0x30);
		sum += 15;
		Graphics_DispChar(x + sum + minusFlag * 15, dis[1]+0x30);
		sum += 15;
		Graphics_DispChar(x + sum + minusFlag * 15, dis[0]+0x30);
		sum += 15;   
  	}
  	else if(data < 100 && data > 9)
  	{          
		Graphics_DispChar(x + sum + minusFlag * 15, dis[1]+0x30);
		sum += 15;
		Graphics_DispChar(x + sum + minusFlag * 15, dis[0]+0x30);
		sum += 15;     
  	}
  	else if(data < 10)
  	{          
		Graphics_DispChar(x + sum + minusFlag * 15, dis[0]+0x30);
		sum += 15;    
  	}
        
}

// 函数功能：显示数字
// 输入参数：	x -- 横坐标
//				k -- 待显示图片的编码
// 输出参数：无
// 返 回 值：无
// 备    注：无
void Graphics_DispPic(uint8 x, uint8 k)
{
    uint8 page = 0; 
    for(uint16 i = 0; i < SCREEN_PAGE; i ++)
    {
		screenBuffer[i][x+0] = PIC[k][page ++];
		screenBuffer[i][x+1] = PIC[k][page ++];
		screenBuffer[i][x+2] = PIC[k][page ++];
		screenBuffer[i][x+3] = PIC[k][page ++];
		screenBuffer[i][x+4] = PIC[k][page ++];
		screenBuffer[i][x+5] = PIC[k][page ++];
		screenBuffer[i][x+6] = PIC[k][page ++];
		screenBuffer[i][x+7] = PIC[k][page ++];
		screenBuffer[i][x+8] = PIC[k][page ++];
		screenBuffer[i][x+9] = PIC[k][page ++];
		screenBuffer[i][x+10] = PIC[k][page ++];
		screenBuffer[i][x+11] = PIC[k][page ++];
		screenBuffer[i][x+12] = PIC[k][page ++];   
		screenBuffer[i][x+13] = PIC[k][page ++];
		screenBuffer[i][x+14] = PIC[k][page ++];
		screenBuffer[i][x+15] = PIC[k][page ++];
		screenBuffer[i][x+16] = PIC[k][page ++];
		screenBuffer[i][x+17] = PIC[k][page ++];
		screenBuffer[i][x+18] = PIC[k][page ++];
		screenBuffer[i][x+19] = PIC[k][page ++];
		screenBuffer[i][x+20] = PIC[k][page ++];
		screenBuffer[i][x+21] = PIC[k][page ++];
		screenBuffer[i][x+22] = PIC[k][page ++];
		screenBuffer[i][x+23] = PIC[k][page ++];
		screenBuffer[i][x+24] = PIC[k][page ++];
		screenBuffer[i][x+25] = PIC[k][page ++];
		screenBuffer[i][x+26] = PIC[k][page ++];
		screenBuffer[i][x+27] = PIC[k][page ++];
		screenBuffer[i][x+28] = PIC[k][page ++];
		screenBuffer[i][x+29] = PIC[k][page ++];
		screenBuffer[i][x+30] = PIC[k][page ++];
		screenBuffer[i][x+31] = PIC[k][page ++];
    }
}

// 函数功能：显示数字
// 输入参数：	image -- 图片指针
// 输出参数：无
// 返 回 值：无
// 备    注：无
void Graphics_DispScreen(const uint8* image)
{
    uint8 len = 0;
    uint8* temp =(uint8*)image;
	uint8 page = 0; 
	while(1)
	{  
		screenBuffer[page][len++] = *temp++;
		if(len == SCREEN_LENGTH)
		{
			len = 0;
			page ++;
		}
		if(page == SCREEN_PAGE)
		  	return;
	}  
}
