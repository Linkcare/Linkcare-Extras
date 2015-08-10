#ifndef _LPSA
#define _LPSA

//#define CPU_16_QUEUE //有些16位系统的指针也是32位的此时需要打开此项，430的指针是16位的，不需要打开此项。
//#define CPU_16 //32加法，430没有嵌套中断，不用开

#define	SOFT_RESET

#define	LPSA_CREATE_TASK_MAX	10//最大任务个数

#define	LPSA_INT_TIME		10//系统时钟滴答，10ms

#define LPSA_TIMER_TASK_MAX	10//最大定时器任务个数

#define LPSA_TIMER_TASK_ENABLE  1

//#define ASM

#define	NORMAL_EAT		1
#define	ONCE_EAT		0

#define QUEUE_MUX_ENABLE        1
#define QUEUE_MUX_DISABLE       0



#define	MODULE_BIT0		1ul
#define	MODULE_BIT1		1ul << 1
#define	MODULE_BIT2		1ul	<< 2
#define	MODULE_BIT3		1ul	<< 3
#define	MODULE_BIT4		1ul	<< 4
#define	MODULE_BIT5		1ul	<< 5
#define	MODULE_BIT6		1ul	<< 6
#define	MODULE_BIT7		1ul	<< 7
#define	MODULE_BIT8		1ul	<< 8
#define	MODULE_BIT9		1ul	<< 9
#define	MODULE_BIT10	1ul	<< 10
#define	MODULE_BIT11	1ul	<< 11
#define	MODULE_BIT12	1ul	<< 12
#define	MODULE_BIT13	1ul	<< 13
#define	MODULE_BIT14	1ul	<< 14
#define	MODULE_BIT15	1ul	<< 15

#define	MODULE_0		1ul
#define	MODULE_1		3ul
#define	MODULE_2		7ul
#define	MODULE_3		15ul
#define	MODULE_4		31ul
#define	MODULE_5		63ul
#define	MODULE_6		127ul
#define	MODULE_7		255ul
#define	MODULE_8		511ul
#define	MODULE_9		1023ul
#define	MODULE_10		2047ul
#define	MODULE_11		4095ul
#define	MODULE_12		8191ul
#define	MODULE_13		16383ul
#define	MODULE_14		32767ul
#define	MODULE_15		65535ul


typedef signed char			lpsa_i8;
typedef signed short int	lpsa_i16;
typedef signed long int     lpsa_i32;

typedef unsigned char		lpsa_u8;
typedef unsigned short int  lpsa_u16;
typedef unsigned long int   lpsa_u32;

#ifdef	NULL
#else
	#define	NULL	0
#endif

typedef struct 
{
	lpsa_u8		*ReadPoint;
	lpsa_u8		*WritePoint;
	lpsa_u8		*Start;
	lpsa_u8		*End;
    lpsa_u32     Count_r;
    lpsa_u32     Count_w;
}Q_EVENT;

typedef struct
{
	lpsa_u8	TaskId;
	lpsa_u8	TaskCount;
	lpsa_u8	TaskError;
	
	void    *parg;
	void	(*CreateTask[LPSA_CREATE_TASK_MAX])(void *);

}LPSA_CREATE_TASK;

#if LPSA_TIMER_TASK_ENABLE
typedef struct
{
	lpsa_u8	TaskCount;
	lpsa_u8	TimerError;
	lpsa_u32	TimerCount[LPSA_TIMER_TASK_MAX];
	lpsa_u32	Timer[LPSA_TIMER_TASK_MAX];
	void	(*TimeTask[LPSA_TIMER_TASK_MAX])(void);


}LPSA_TIMER_TASK;
#endif

typedef struct
{
	lpsa_u16		ModuleReg;
	lpsa_u16		OnceReg;
	lpsa_u8		        ModuleSum;
        
        
        void (*eatwdt)(void);//喂狗函数指针
#ifdef	SOFT_RESET
        void (*reset)(void);//软件重启指针
#endif
        

}WDT_DATA;


extern LPSA_TIMER_TASK	LPSATimer;
extern LPSA_CREATE_TASK	LPSATask;






void LPSA_Init (void);
lpsa_u16 LPSA_GetRam (void);

void	QueueCreate (Q_EVENT *Queue, lpsa_u8 *Msg, lpsa_i16 Size);
void	QueuePlpsat (Q_EVENT *Queue, lpsa_u8 Data);
lpsa_u8	QueueGet (Q_EVENT *Queue);
lpsa_u8 *QueueAcc (Q_EVENT *Queue);//新增指针读取方式

void	LPSA_TaskInit (void);
lpsa_i8	LPSA_TaskCreate (void (*Task)(void *));
void	LPSA_TaskSwitch (lpsa_u8 Id);
void	LPSA_TaskStat (void);
void	LPSA_TaskSwitchUp (void);
void	LPSA_TaskSwitchDown (void);

#if LPSA_TIMER_TASK_ENABLE
void LPSA_TimerInit (void);
lpsa_i8 LPSA_TimerTaskCreate (void (*TimerTask)(void), lpsa_u32 Ms);
void LPSA_TimerTaskDel (void (*TimerTask)(void));
void LPSA_TimerRun (void);
#endif

lpsa_i8	CreateWdt (WDT_DATA *Wdt, lpsa_u8 ModuleSum);
lpsa_i8	WdtTaskEat (WDT_DATA *Wdt, lpsa_u16 Module, lpsa_u8  reg);
void	WdtEate (WDT_DATA *Wdt);
  
#endif


/*

Timer size:		

*/