 /*********************************************************/
 /*modified by liubingyan*/
 /*********************************************************/
 #include "Configuration.h"
 #include "InnerEEPROM.h"
 #include "DeviceInfo.h"
 #include "Calculate.h"
 #include "string.h"
 #include "EEPROM.h"
 #include "Display.h"

 /*Ĭ���û���Ϣ*/
 UserInfo default_userInfo =
 {
 	680,	//Ĭ������
 	1983,	//Ĭ�����գ���
 	1,		//Ĭ�����գ���
 	1,		//Ĭ�����գ���
 	MAN,	//Ĭ���Ա�
 	175,	//Ĭ������
 	30,		//Ĭ������
 	72.625	//Ĭ�ϲ���
 };

 /*Ĭ���û�����*/
 UserConfig default_userConfig =
 {
 	99999,		//Ĭ��Ŀ�경��
 	Time_24,	//Ĭ��ʱ����
 	TEMP_C,		//Ĭ���¶ȵ�λ
 	METRIC,		//Ĭ�Ͼ��뵥λ
 	{'I','C','H','O','I','C','E'},	//Ĭ���ʺ���
 	{2},			//Ĭ����ʾ˳��
 	{0,0}							//Ĭ��ʱ��
 };

 /*Ĭ�ϳ�ʼʱ��*/
 Calendar default_calendar =
 {
 	{DEFAULT_HOUR,DEFAULT_MIN,DEFAULT_SEC,DEFAULT_NOON},
 	{DEFAULT_WEEK,DEFAULT_MON,DEFAULT_DAY,DEFAULT_YEAR},
 	Time_24,
 	Time_AM
 };

 #if	(BACKUP == ON)

 //ϵͳ�洢����
 SystemRun systemBackup;

 #endif

 //�ϵ���¼��־λ
 static uint8 initConfig_flag = FIRST_POWERON;

 // �������ܣ���ȡ Tracker ID��
 // ������������
 // ������������
 // �� �� ֵ��Tracker ID��
 // ��    ע����
 uint32 UserConfig_readID(void)
 {
   	//����ID������ȡ������������USB HID���� ��дID
   	return *(uint32*)TRACKERID_ADDR;
 }

 // �������ܣ����ϵ���־λ��λ
 // ������������
 // ������������
 // �� �� ֵ����
 // ��    ע����
 void UserConfig_setPowerOnState(void)
 {
   	//�ϵ���־λ��λ
 	initConfig_flag = NON_FIRST_POWERON;

 	//���ϵ���־λд���ڲ�EEPROM
 	InnerEEPROM_write((uint8*)&initConfig_flag,POWERON_FLAG_ADDR,sizeof(initConfig_flag));
 }

 // �������ܣ������ϵ���־λ
 // ������������
 // ������������
 // �� �� ֵ����
 // ��    ע����
 void UserConfig_clearPowerOnState(void)
 {
   	//������־λ��λ
   	initConfig_flag = FIRST_POWERON;
        ;; // Fall through to label ?Subroutine1

 	//���ϵ���־λд���ڲ�EEPROM
 	InnerEEPROM_write((uint8*)&initConfig_flag,POWERON_FLAG_ADDR,sizeof(initConfig_flag));
 }

 // �������ܣ���ȡ�ϵ���־λ
 // ������������
 // ������������
 // �� �� ֵ��PowerOn_Status
 // ��    ע����
 PowerOn_Status UserConfig_getPowerOnState(void)
 {
   	if(*((uint8 *)POWERON_FLAG_ADDR) == 0x00)
 	{
 		return FIRST_POWERON;
 	}
 	else
 	{
 		return NON_FIRST_POWERON;
 	}
 }

 // �������ܣ������ʺ���
 // ����������	buf -- �������ݰ���ַ
 //				num -- ���ݰ�����
 // ������������
 // �� �� ֵ��SUCCESS & ERROR
 // ��    ע����
 ErrorStatus UserConfig_setGreeting(const uint8 *buf,const uint8 num)
 {
 	uint8 checkTime_AMount = 0;

 	//��������������ԭ�������Ƿ���ͬ
   	for(checkTime_AMount = 0;checkTime_AMount < num;checkTime_AMount++)
 	{
 		if(*(buf + checkTime_AMount) != *((uint8 *)(GREETING_ADDR + checkTime_AMount)))
 		{
 			checkTime_AMount = 0xFF;

 			break;
 		}
 	}

 	//���������޷����仯����ֱ�ӷ���
 	if(checkTime_AMount != 0xFF)
 	{
 		return SUCCESS;
 	}

 	InnerEEPROM_write(buf,GREETING_ADDR,num);

 	return SUCCESS;
 }

 // �������ܣ���ȡ�ʺ���
 // ������������
 // �����������ʺ���
 // �� �� ֵ����
 // ��    ע����
 void UserConfig_getGreeting(uint8 getGreeting[GREETING_MAX + 1])
 {
   	for(uint8 i = 0;i < GREETING_MAX + 1;i++)
 	{
 		getGreeting[i] = *((uint8 *)(GREETING_ADDR + i));
 	}
 }

 // �������ܣ�����ʱ��
 // ����������ʱ��ֵ
 // ������������
 // �� �� ֵ��SUCCESS & ERROR
 // ��    ע����
 ErrorStatus UserConfig_setTimeZone(const TimeZone setTimeZone)
 {
   	//��������������ԭ�������Ƿ���ͬ
 	if(setTimeZone.hour == *((int8 *)TIMEZONE_HOUR_ADDR) &&
 	   setTimeZone.min == *((uint8 *)TIMEZONE_MIN_ADDR))
 	{
 		return SUCCESS;
 	}

 	//���÷Ƿ�
   	if(setTimeZone.hour > 12 || setTimeZone.hour < -12 || setTimeZone.min > 59)
 	  	return ERROR;

 	InnerEEPROM_write((uint8*)&setTimeZone.min,TIMEZONE_MIN_ADDR,sizeof(setTimeZone.min));
 	InnerEEPROM_write((uint8*)&setTimeZone.hour,TIMEZONE_HOUR_ADDR,sizeof(setTimeZone.hour));

     return SUCCESS;
 }

 // �������ܣ���ȡʱ��ֵ
 // ������������
 // ����������ʱ��ֵ
 // �� �� ֵ����
 // ��    ע����
 void UserConfig_getTimeZone(TimeZone *getTimeZone)
 {
   	getTimeZone->hour = *((int8 *)TIMEZONE_HOUR_ADDR);
 	getTimeZone->min  = *((uint8 *)TIMEZONE_MIN_ADDR);
 }

 // �������ܣ�����ʱ����
 // ����������ʱ����
 // ������������
 // �� �� ֵ��SUCCESS & ERROR
 // ��    ע����
 ErrorStatus UserConfig_setTimeFormat(const TIME_FormatDef setFormat)
 {
   	//��������������ԭ�������Ƿ���ͬ
   	if(setFormat == *((uint8 *)TIMEFORMAT_ADDR))
 	{
 		return SUCCESS;
 	}

 	//���÷Ƿ�
   	if(setFormat != Time_12 && setFormat != Time_24)
 	  	return ERROR;

 	InnerEEPROM_write((uint8*)&setFormat,TIMEFORMAT_ADDR,sizeof(setFormat));

 	return SUCCESS;
 }

 // �������ܣ���ȡʱ����
 // ������������
 // ����������ʱ����
 // �� �� ֵ����
 // ��    ע����
 void UserConfig_getTimeFormat(uint8 *getTimeFormat)
 {
   	*getTimeFormat = *((uint8 *)TIMEFORMAT_ADDR);
 }

 // �������ܣ���ʼ����ʾ˳��
 // ������������
 // ������������
 // �� �� ֵ����
 // ��    ע����
 void UserConfig_initDispOrder(void)
 {
 	UserConfig_setDispOrder(default_userConfig.displayOrder,sizeof(default_userConfig.displayOrder));
        ;; // Fall through to label UserConfig_setDispOrder
 }

 // �������ܣ�������ʾ˳��
 // ����������	buf -- �������ݰ���ַ
 //				num -- ���ݰ�����
 // ������������
 // �� �� ֵ��SUCCESS & ERROR
 // ��    ע����
 ErrorStatus UserConfig_setDispOrder(const uint8 *order,const uint8 num)
 {
   	uint8 checkTime_AMount = 0;

 	//��������������ԭ�������Ƿ���ͬ
   	for(checkTime_AMount = 0;checkTime_AMount < num;checkTime_AMount++)
 	{
 		if(*(order + checkTime_AMount) != *((uint8 *)(DISPORDER_ADDR + checkTime_AMount)))
 		{
 			checkTime_AMount = 0xFF;

 			break;
 		}
 	}

 	//���������޷����仯����ֱ�ӷ���
 	if(checkTime_AMount != 0xFF)
 	{
 	  	//����ֵ�Ƿ��Ϸ�
 		if(Display_setDisplay((uint8 *)DISPORDER_ADDR,num) == ERROR)
 		{
 		  	Display_setDisplay((uint8 *)default_userConfig.displayOrder,sizeof(default_userConfig.displayOrder));

 			return ERROR;
 		}

 		return SUCCESS;
 	}

 	//����ֵ�Ƿ��Ϸ�
 	uint8 result = 0;
 	result = Display_setDisplay((uint8 *)order,num);

 	//���������д���
 	if(result == ERROR)
 	{
 	  	//������Ĭ����������
 	  	Display_setDisplay((uint8 *)default_userConfig.displayOrder,sizeof(default_userConfig.displayOrder));

 		return ERROR;
 	}

 	InnerEEPROM_write(order,DISPORDER_ADDR,num);

 	return SUCCESS;
 }

 // �������ܣ���ȡ��ʾ˳��
 // ������������
 // ������������ʾ˳��
 // �� �� ֵ����
 // ��    ע����
 void UserConfig_getDispOrder(uint8 *getOrder)
 {
   	for(uint8 i = 0;i < FUNC_DISPLAY_MAX;i++)
 	{
 		getOrder[i] = *((uint8 *)(DISPORDER_ADDR + i));
 	}
 }

 // �������ܣ����þ��뵥λ��
 // ������������λ��
 // ������������
 // �� �� ֵ��SUCCESS & ERROR
 // ��    ע����
 ErrorStatus UserConfig_setDistanceUnit(const LUINT_TypeDef setDisUnit)
 {
   	//��������������ԭ�������Ƿ���ͬ
   	if(setDisUnit == *((uint8 *)DISTANCEUNIT_ADDR))
 	{
 		return SUCCESS;
 	}

 	//���÷Ƿ�
   	if(setDisUnit != IMPERIAL && setDisUnit != METRIC )
 	{
 		return ERROR;
 	}

 	InnerEEPROM_write((uint8*)&setDisUnit,DISTANCEUNIT_ADDR,sizeof(setDisUnit));

 	return SUCCESS;
 }

 // �������ܣ���ȡ���뵥λ��
 // ������������
 // ������������λ��
 // �� �� ֵ����
 // ��    ע����
 void UserConfig_getDistanceUnit(LUINT_TypeDef *getDistanceUnit)
 {
   	*getDistanceUnit = (LUINT_TypeDef)(*((uint8 *)DISTANCEUNIT_ADDR));
 }

 // �������ܣ������¶ȵ�λ��
 // �����������¶ȵ�λ��
 // ������������
 // �� �� ֵ��SUCCESS & ERROR
 // ��    ע��
 ErrorStatus UserConfig_setTemperatureUnit(const TEMP_TypeDef setTemUnit)
 {
   	//��������������ԭ�������Ƿ���ͬ
   	if(setTemUnit == *((uint8 *)TEMPERUNIT_ADDR))
 	{
 		return SUCCESS;
 	}

 	//���÷Ƿ�
   	if(setTemUnit != TEMP_F && setTemUnit != TEMP_C)
 	  	return ERROR;

 	InnerEEPROM_write((uint8*)&setTemUnit,TEMPERUNIT_ADDR,sizeof(setTemUnit));

 	return SUCCESS;
 }

 // �������ܣ���ȡ�¶ȵ�λ��
 // ������������
 // ������������λ��
 // �� �� ֵ����
 // ��    ע����
 void UserConfig_getTemperatureUnit(TEMP_TypeDef *getTempUnit)
 {
   	*getTempUnit = (TEMP_TypeDef)(*((uint8 *)TEMPERUNIT_ADDR));
        ;; // Fall through to label ?Subroutine0
 }

 // �������ܣ������û�Ŀ�경��
 // ����������Ŀ�경��
 // ������������
 // �� �� ֵ��SUCCESS & ERROR
 // ��    ע��
 ErrorStatus UserConfig_setGoalStep(const uint32 setGoalStep)
 {
   	//��������������ԭ�������Ƿ���ͬ
   	if(setGoalStep == *((uint32 *)GOALSTEP_ADDR))
 	{
 		return SUCCESS;
 	}

 	//���÷Ƿ�
 	if(setGoalStep > 99999 || setGoalStep == 0)
 	{
 		return ERROR;
 	}

 	InnerEEPROM_write((uint8*)&setGoalStep,GOALSTEP_ADDR,sizeof(setGoalStep));

 	return SUCCESS;
 }

 // �������ܣ���ȡĿ�경��
 // ������������
 // ����������Ŀ�경��
 // �� �� ֵ����
 // ��    ע����
 void UserConfig_geGoalSteps(uint32 *goalSteps)
 {
   	*goalSteps = (*((uint32 *)GOALSTEP_ADDR));
 }

 // �������ܣ������û�����
 // �����������û�����
 // ������������
 // �� �� ֵ��SUCCESS & ERROR
 // ��    ע����
 ErrorStatus UserConfig_setUserConfig(UserConfig* setUserConfig)
 {
 	InnerEEPROM_write((uint8*)&setUserConfig->displayOrder	,DISPORDER_ADDR		 	,sizeof(setUserConfig->displayOrder));
 	InnerEEPROM_write((uint8*)&setUserConfig->distanceUnit	,DISTANCEUNIT_ADDR		,sizeof(setUserConfig->distanceUnit));
 	InnerEEPROM_write((uint8*)&setUserConfig->goalStep		,GOALSTEP_ADDR		 	,sizeof(setUserConfig->goalStep));
 	InnerEEPROM_write((uint8*)&setUserConfig->greeting		,GREETING_ADDR		 	,sizeof(setUserConfig->greeting));
 	InnerEEPROM_write((uint8*)&setUserConfig->temperUnit	,TEMPERUNIT_ADDR		,sizeof(setUserConfig->temperUnit));
 	InnerEEPROM_write((uint8*)&setUserConfig->timeFormat 	,TIMEFORMAT_ADDR 		,sizeof(setUserConfig->timeFormat));
 	InnerEEPROM_write((uint8*)&setUserConfig->timeZone.hour ,TIMEZONE_HOUR_ADDR 	,sizeof(setUserConfig->timeZone.hour));
 	InnerEEPROM_write((uint8*)&setUserConfig->timeZone.min  ,TIMEZONE_MIN_ADDR 		,sizeof(setUserConfig->timeZone.min));

 	return SUCCESS;
 }

 // �������ܣ������û���Ϣ
 // �����������û���Ϣ
 // ������������
 // �� �� ֵ��SUCCESS & ERROR
 // ��    ע����
 ErrorStatus UserConfig_setUserInfo(UserInfo* setUserInfo)
 {
   	float stepLength = 0;

 	//��������������ԭ�������Ƿ���ͬ
 	if(setUserInfo->gender 		== *((uint8 *)GENDER_ADDR) &&
 	   setUserInfo->age 		== *((uint8 *)AGE_ADDR)    &&
 	   setUserInfo->height 		== *((uint8 *)HEIGHT_ADDR) &&
 	   setUserInfo->weight 		== *((uint16*)WEIGHT_ADDR) &&
 	   setUserInfo->birthdayYear== *((uint16*)BIRTHDAYYEAR_ADDR) &&
 	   setUserInfo->birthdayMon == *((uint8 *)BIRTHDAYMON_ADDR)  &&
 	   setUserInfo->birthdayDay == *((uint8 *)BIRTHDAYDAY_ADDR))
 	{
 		return SUCCESS;
 	}

 	//�Ա��Ƿ�
   	if(setUserInfo->gender != WOMAN &&
 	   setUserInfo->gender != MAN &&
 	   setUserInfo->gender != NON)
 	  	return ERROR;

 	//��������
 	if(setUserInfo->gender == WOMAN)
 	{
 		stepLength = (float)(setUserInfo->height * 0.413);
 	}
 	else
 	{
 		stepLength = (float)(setUserInfo->height * 0.415);
 	}

 	InnerEEPROM_write((uint8*)&stepLength			,STEPLENGTH_ADDR		,sizeof(setUserInfo->stepLenght));
 	InnerEEPROM_write((uint8*)&setUserInfo->gender	,GENDER_ADDR		 	,sizeof(setUserInfo->gender));
 	InnerEEPROM_write((uint8*)&setUserInfo->age		,AGE_ADDR		 	 	,sizeof(setUserInfo->age));
 	InnerEEPROM_write((uint8*)&setUserInfo->height	,HEIGHT_ADDR		 	,sizeof(setUserInfo->height));
 	InnerEEPROM_write((uint8*)&setUserInfo->weight	,WEIGHT_ADDR		 	,sizeof(setUserInfo->weight));
 	InnerEEPROM_write((uint8*)&setUserInfo->birthdayYear,BIRTHDAYYEAR_ADDR	,sizeof(setUserInfo->birthdayYear));
 	InnerEEPROM_write((uint8*)&setUserInfo->birthdayMon ,BIRTHDAYMON_ADDR 	,sizeof(setUserInfo->birthdayMon));
 	InnerEEPROM_write((uint8*)&setUserInfo->birthdayDay ,BIRTHDAYDAY_ADDR 	,sizeof(setUserInfo->birthdayDay));

 	return SUCCESS;
 }

 // �������ܣ���ȡ�û���Ϣ
 // ������������
 // �����������û���Ϣ
 // �� �� ֵ����
 // ��    ע����
 void UserConfig_getUserInfo(UserInfo *getUserInfo)
 {
 	getUserInfo->age 			= *((uint8 *)AGE_ADDR);
 	getUserInfo->height 		= *((uint8 *)HEIGHT_ADDR);
 	getUserInfo->weight 		= *((uint16*)WEIGHT_ADDR);
 	getUserInfo->birthdayYear 	= *((uint16*)BIRTHDAYYEAR_ADDR);
 	getUserInfo->birthdayMon 	= *((uint8 *)BIRTHDAYMON_ADDR);
 	getUserInfo->birthdayDay 	= *((uint8 *)BIRTHDAYDAY_ADDR);
 	getUserInfo->stepLenght     = *((float *)STEPLENGTH_ADDR);
   	getUserInfo->gender 		= (GENDAR_TypeDef)(*((uint8 *)GENDER_ADDR));
 }

 // �������ܣ���ȡ�û�����
 // ������������
 // ������������
 // �� �� ֵ���û�����
 // ��    ע����
 void UserConfig_getUserConfig(UserConfig *getUserConfig)
 {
 	getUserConfig->goalStep 	 = *((uint32*) GOALSTEP_ADDR);
 	getUserConfig->timeZone.min  = *((uint8 *) TIMEZONE_MIN_ADDR);
 	getUserConfig->timeZone.hour = *((int8  *) TIMEZONE_HOUR_ADDR);
 	getUserConfig->temperUnit    = (TEMP_TypeDef)(*((uint8 *) TEMPERUNIT_ADDR));
   	getUserConfig->distanceUnit  = (LUINT_TypeDef)(*((uint8 *) DISTANCEUNIT_ADDR));
 	getUserConfig->timeFormat    = (TIME_FormatDef)(*((uint8 *) TIMEFORMAT_ADDR));

 	for(uint8 i = 0;i < sizeof(getUserConfig->displayOrder);i++)
 	{
 		getUserConfig->displayOrder[i] =  *((uint8 *)(DISPORDER_ADDR + i));
 	}

 	for(uint8 j = 0;j < sizeof(getUserConfig->greeting);j++)
 	{
 		getUserConfig->greeting[j] =  *((uint8 *)(GREETING_ADDR + j));
 	}
 }

 #if	(BACKUP == ON)

 // �������ܣ���ȡ����ʱ��
 // ������������
 // ��������������ʱ��
 // �� �� ֵ����
 // ��    ע����
 void UserConfig_getCalendarBackup(Calendar *getCalendar)
 {
   	if(initConfig_flag == FIRST_POWERON)
 	{
 		return;
 	}

 	*getCalendar = systemBackup.systemClock;
 }

 // �������ܣ���ȡ����״̬��
 // ������������
 // ��������������״̬��
 // �� �� ֵ����
 // ��    ע����
 void UserConfig_getMemoryBackup(MemoryContent *getMemory)
 {
     if(initConfig_flag == FIRST_POWERON)
 	{
 		return;
 	}

 	*getMemory = systemBackup.memoryRun;
 }

 // �������ܣ���ȡ������ʾ����
 // ������������
 // ����������������ʾ����
 // �� �� ֵ����
 // ��    ע����
 void UserConfig_getStepDisplay(uint32 *displayStep)
 {
     if(initConfig_flag == FIRST_POWERON)
 	{
 		return;
 	}

 	*displayStep = systemBackup.displayStep;
 }

 // �������ܣ���ȡ���ݲ���
 // ������������
 // �������������ݲ���
 // �� �� ֵ����
 // ��    ע�����ڼǲ��㷨
 void UserConfig_getStepBackup(uint32 *step)
 {
     if(initConfig_flag == FIRST_POWERON)
 	{
 		return;
 	}

 	*step = systemBackup.userSteps;
 }

 // �������ܣ���ȡ������ʷ����
 // ������������
 // ������������ʷ����
 // �� �� ֵ����
 // ��    ע�����ڼǲ��㷨
 void UserConfig_getLastStepBackup(uint32 *lastStep)
 {
     if(initConfig_flag == FIRST_POWERON)
 	{
 		return;
 	}

 	*lastStep = systemBackup.lastSteps;
 }

 /*******************************************************************************
 *���ܣ���EEPROM��ȡ���ݸ߶�
 *******************************************************************************/
 void UserConfig_getClimbBackup(uint32 *climb)
 {
     if(initConfig_flag == FIRST_POWERON)
 	{
 		return;
 	}

 	*climb = systemBackup.userClimb;
 }
 #endif

 // �������ܣ���ʼ�����ù���ģ��
 // ������������
 // ������������
 // �� �� ֵ����
 // ��    ע����
 void UserConfig_init(void)
 {
   	//�״��ϵ�
     if(UserConfig_getPowerOnState() == FIRST_POWERON)
 	{
 	  	//��ʼ���û���Ϣ
 	  	UserConfig_setUserInfo(&default_userInfo);

 		//��ʼ���û�����
 	  	UserConfig_setUserConfig(&default_userConfig);

         //��ʼ��ϵͳ����ϵͳ
 		#if	(BACKUP == ON)
         systemBackup.systemClock = default_calendar;
         systemBackup.userSteps = 0;
         systemBackup.lastSteps = 0;

         memset((uint8 *) &systemBackup.memoryRun, 0,sizeof(systemBackup.memoryRun));

 		*((uint8 *)SYSTEM_BACKUP_ADDR_0) = 0;
 		*((uint8 *)SYSTEM_BACKUP_ADDR_1) = 0;

         memcpy((uint32 *)SYSTEM_BACKUP_ADDR_0,(uint32 *)&systemBackup,sizeof(systemBackup));
 		memcpy((uint32 *)SYSTEM_BACKUP_ADDR_1,(uint32 *)&systemBackup,sizeof(systemBackup));
 		#endif
     }
 	else
     {
 		#if	(BACKUP == ON)
         //��ȡϵͳ����
 		uint8 checkSum_backup0 = 0;
 		uint8 checkSum_backup1 = 0;

 		//���㱸������У���ͣ����򱸷ݣ�
 		for(uint8 i = 0;i < sizeof(systemBackup);i++)
 		{
 			checkSum_backup0 += *((uint8 *)SYSTEM_BACKUP_ADDR_0 + i);
 			checkSum_backup1 += *((uint8 *)SYSTEM_BACKUP_ADDR_1 + i);
 		}

 		//��������У�������洢�ıȽϣ�ѡ��У����ȷ�ı�������
 		if(checkSum_backup0 != *((uint8 *)CHECKSUM_ADDR_0) &&
 		   checkSum_backup1 != *((uint8 *)CHECKSUM_ADDR_1))
 		{
 			memset((uint8 *)&systemBackup,0,sizeof(systemBackup));
 		}
 		else if(checkSum_backup0 == *((uint8 *)CHECKSUM_ADDR_0))
 		{
 			memcpy((uint32 *)&systemBackup,(uint32 *)SYSTEM_BACKUP_ADDR_0,sizeof(systemBackup));
 		}
 		else if(checkSum_backup1 == *((uint8 *)CHECKSUM_ADDR_1))
 		{
 			memcpy((uint32 *)&systemBackup,(uint32 *)SYSTEM_BACKUP_ADDR_1,sizeof(systemBackup));
 		}

 		#endif
     }
 }
