 /*********************************************************/
 /*modified by liubingyan*/
 /*********************************************************/
 #include "Clock.h"
 #include "stdlib.h"
 #include "string.h"
 #include "LIS3DH.h"
 #include "LPS331AP.h"
 #include "System.h"
 #include "EEPROM.h"
 #include "Battery.h"
 #include "Display.h"
 #include "Character.h"
 #include "TimeManage.h"
 #include "Calculate.h"
 #include "DataSave.h"
 #include "Configuration.h"
 #include "DeviceInfo.h"

 /*****************************************************************/
 const pFunc infor_disp[INFO_DISPLAY_MAX] 	= 	//��Ϣ����
 {
 	displayCharging,	//��������
 	displayBatteryFull,	//���ص���������
 	displayBatteryLow,	//�����ͽ���
 	displayMemoryLow,	//�ڴ��ͽ���
 	displayMemoryFull,	//�ڴ�������
 	displayChoiceMark,	//��˼ˮӡ����
 	displaySleep,		//SLEEP����
 	displayLETGO,		//�����1������
 	displayCOMEON,		//�����2������
 	displayWELLDONE,	//�����3������
 	displayWakeByShake	//�ζ����ѽ���
 };

 /*****************************************************************/
 const pFunc origin_disp[FUNC_DISPLAY_MAX]	=
 {
   	displayClock,			/*ʱ��			*/
 	displayStep,			/*����			*/
 	displayDistance,		/*����			*/
 	displayCalory,			/*��·��������	*/
 	displayFat,				/*֬��ȼ����	*/
 	displayPercent,			/*Ŀ��������	*/
 	displayTemperature,		/*�����¶�		*/
 	displaySpeed,			/*�ٶ�			*/
 	displayClimbAltitude	/*�ۼ������߶�	*/
 };

 /*****************************************************************/
 pFunc func_disp[FUNC_DISPLAY_MAX]	=
 {
 	displayClock,			/*ʱ��			*/
 	displayStep,			/*����			*/
 	displayDistance,		/*����			*/
 	displayCalory,			/*��·��������	*/
 	displayFat,				/*֬��ȼ����	*/
 	displayPercent,			/*Ŀ��������	*/
 	displayTemperature,		/*�����¶�		*/
 	displaySpeed,			/*�ٶ�			*/
 	displayClimbAltitude	/*�ۼ������߶�	*/
 };

 /*****************************************************************/
 const pFunc test_disp[TEST_DISPLAY_MAX]	=	//���ܽ���
 {
 	displayMEMS_test,			/*���ٶ�		*/
 	displayTemperature_test,	/*�����¶�		*/
 	displayAirpressure_test,	/*��ѹ			*/
 	displayEEPROM_test			/*EEPROM ״̬	*/
 };

 //����ֵ������
 static uint8 func_buffer[FUNC_DISPLAY_MAX];

 //��������
 static uint8 interfaceCount = 0;

 // �������ܣ��ҳ��ַ����е��ַ�x������λ��
 // ����������	pString -- Ŀ���ַ���
 //				length  -- �ַ�������
 //				xchar   -- �������ַ�
 // ������������
 // �� �� ֵ��	>=0(���꣩
 //		  		-1���ַ�����Ч
 //		  		-2��δ����ָ���ַ�*/
 // ��    ע����
 static int8 Display_srtchr(uint8 *pString,const uint8 length,const uint8 xchar)
 {
 	const uint8* p = pString;

 	uint8 position = 0;

 	if(pString == NULL || length == 0)
 	{
 		return -1;
 	}

 	for(uint8 i = 0;i < length;i++)
 	{
 		if(*(p + i) == xchar)
 		{
 			position = i;

 			return position;
 		}
 	}

 	return -2;
 }

 // �������ܣ��趨��ʾ������˳��������
 // ����������	pvalue -- �ַ���
 //				length -- �ַ�������
 // ������������
 // �� �� ֵ����
 // ��    ע����
 ErrorStatus Display_setDisplay(uint8 *pvalue,const uint8 length)
 {
   	int8 err = 0;
 	uint8 ii = 0;

 	//���������Ľ����������Ƿ��зǷ�����
 	for(uint8 i = 0; i < length;i++)
 	{
 		if(*(pvalue + i) > FUNC_DISPLAY_MAX)
 		{
 			return ERROR;
 		}
 	}

 	//���������Ľ����������Ƿ��пս���
 	err = Display_srtchr(pvalue,length,0);

   	if(err == -1)
 	{
 		return ERROR;
 	}
 	else if(err == -2)
 	{
 		interfaceCount = length;
 	}
 	else if(err > 0)
 	{
 		interfaceCount = err;
 	}
 	else if(err == 0)
 	{
 		return ERROR;
 	}

 	memset(func_disp,0,sizeof(func_disp));
 	memcpy(func_buffer,pvalue,interfaceCount);

 	while(ii < interfaceCount)
 	{
 		func_disp[ii] = origin_disp[func_buffer[ii] - 1];
 		ii++;
 	}

 	return SUCCESS;
 }

 // �������ܣ���ȡ��������
 // ������������
 // ������������
 // �� �� ֵ����
 // ��    ע����
 uint8 Display_getDisplayCount(void)
 {
 	return interfaceCount;
 }

 // �������ܣ���ʾ�Ʋ�����
 // ������������
 // ������������
 // �� �� ֵ����
 // ��    ע����
 void displayStep(void)
 {
 	int32 step = 0;

 	//��ȡ����
 	step = Calculate_getSteps();

 	//��ʾ����
 	Graphics_DispStringDec(3,step);

 	//��ʾ�Ʋ���ʶ
 	Graphics_DispPic(94,IMAGE_STEP);
 }

 // �������ܣ���ʾʱ�ӽ���
 // ������������
 // ������������
 // �� �� ֵ����
 // ��    ע����
 void displayClock(void)
 {
   	Calendar realTime;

 	//��ȡʱ��
 	TimeManage_getTime(&realTime);

 	//��ʾСʱ��ʮλ��
 	Graphics_DispStringDec(3,realTime.time.RTC_Hours/10);

 	//��ʾСʱ����λ��
 	Graphics_DispStringDec(18,realTime.time.RTC_Hours%10);

 	if(realTime.time.RTC_Seconds % 2)
 	{
 	  	//ð��ÿһ����˸һ��
 	  	Graphics_DispChar(32,':');
 	}

 	//��ʾ���ӣ�ʮλ��
 	Graphics_DispStringDec(46,realTime.time.RTC_Minutes/10);

 	//��ʾ���ӣ���λ��
 	Graphics_DispStringDec(61,realTime.time.RTC_Minutes%10);

 	if(realTime.timeFormat == Time_12)
 	{
 		if(realTime.timePeriod == Time_AM)
 		{
 		  	//��ʾAM
 			Graphics_DispPic(94,IMAGE_AM);
 		}
 		else if(realTime.timePeriod == Time_PM)
 		{
 		  	//��ʾPM
 			Graphics_DispPic(94,IMAGE_PM);
 		}
 	}

 }

 // �������ܣ���ʾĿ�����ɰٷֱȽ���
 // ������������
 // ������������
 // �� �� ֵ����
 // ��    ע����
 void displayPercent(void)
 {
 	int32 percent = 0;

 	uint8 Carry_flag = 0;

 	//��ȡĿ��������
 	percent = (int32)Calculate_getPercent();

 	if(percent > 9)
 	{
 		Carry_flag = 1;
 	}
 	if(percent > 99)
 	{
 		Carry_flag = 2;
 	}

 	//��ʾ�ٷֱ�����
 	Graphics_DispStringDec(3,percent);

 	// ��ʾ%
 	Graphics_DispChar(20 + Carry_flag*16,'%');

 	// ��ʾgoal
 	Graphics_DispPic(94,IMAGE_GOAL);
        ;; // Fall through to label ?Subroutine0

 }

 // �������ܣ���ʾ��������
 // ������������
 // ������������
 // �� �� ֵ����
 // ��    ע����
 void displayDistance(void)
 {
   	//1 km = 0.621 mile
 	int32 distance = 0;
 	LUINT_TypeDef distanceUnit;

 	//��ȡ���뵥λ
 	UserConfig_getDistanceUnit(&distanceUnit);

 	//��ȡ����
 	distance = (int32)Calculate_getDistance();

 	//Ӣ�Ƶ�λ
 	if(distanceUnit == IMPERIAL)
 	{
 		distance = (int32)(distance * 0.621);
 	}

 	distance = (int32)(distance * 0.1);

 	if(distance/10000)
 	{
 		Graphics_DispStringDec(3,distance/10000);
 		Graphics_DispStringDec(18,(distance%10000)/1000);
 		Graphics_DispStringDec(33,(distance%1000)/100);
 		Graphics_DispChar(48,'.');
 		Graphics_DispStringDec(63,(distance%100)/10);
 		Graphics_DispStringDec(78,distance%10);
 	}
 	else if(distance/1000)
 	{
 		Graphics_DispStringDec(3,(distance%10000)/1000);
 		Graphics_DispStringDec(18,(distance%1000)/100);
 		Graphics_DispChar(33,'.');
 		Graphics_DispStringDec(48,(distance%100)/10);
 		Graphics_DispStringDec(63,distance%10);
 	}
 	else
 	{
 		Graphics_DispStringDec(3,(distance%1000)/100);
 		Graphics_DispChar(18,'.');
 		Graphics_DispStringDec(33,(distance%100)/10);
 		Graphics_DispStringDec(48,distance%10);
 	}

 	Graphics_DispPic(94,IMAGE_DISTANCE); //��ʾ������ʶ

 }

 // �������ܣ���ʾ�ٶȽ���
 // ������������
 // ������������
 // �� �� ֵ����
 // ��    ע����
 void displaySpeed(void)
 {
   	//1 km = 0.621 mile
 	int32 speed;
 	LUINT_TypeDef distanceUnit;

 	//��ȡ���뵥λ
 	UserConfig_getDistanceUnit(&distanceUnit);

 	//��ȡ�ٶ�
 	speed = (int32)Calculate_getSpeed();

 	//Ӣ�Ƶ�λ
 	if(distanceUnit == IMPERIAL)
 	{
 		speed = (int32)(speed * 22.356);

 		Graphics_DispPic(94,IMAGE_MPH);
 	}
 	else if(distanceUnit == METRIC)
 	{
 		speed = (int32)(speed * 36);

 		Graphics_DispPic(94,IMAGE_KPH);
 	}

 	Graphics_DispStringDec(3,speed/1000);
 	Graphics_DispStringDec(18,(speed%1000)/100);
 	Graphics_DispChar(33,'.');
 	Graphics_DispStringDec(48,(speed%100)/10);
 	Graphics_DispStringDec(63,speed%10);
 }

 // �������ܣ���ʾ��·������������
 // ������������
 // ������������
 // �� �� ֵ����
 // ��    ע����
 void displayCalory(void)
 {
 	int32 calories = 0;

 	//��ȡ��·��������
 	calories = (int32)Calculate_getCalories();

 	//��ʾ��·������
 	Graphics_DispStringDec(3,calories);

 	//��ʾ��·����ʶ
 	Graphics_DispPic(94,IMAGE_CALORY);

 }

 // �������ܣ���ʾ֬��ȼ��������
 // ������������
 // ������������
 // �� �� ֵ����
 // ��    ע����
 void displayFat(void)
 {
 	int32 fat = 0;

 	//��ȡ֬��ȼ����
 	fat = (int32)Calculate_getFat();

 	//��ʾ֬��ȼ��������
 	Graphics_DispStringDec(3,fat);

 	//��ʾ֬��ȼ������ʶ
 	Graphics_DispPic(94,IMAGE_FAT);

 }

 // �������ܣ���ʾ���θ߶Ƚ���
 // ������������
 // ������������
 // �� �� ֵ����
 // ��    ע����
 void displayAltitude(void)
 {
   	//1 km = 0.621 mile ��1 m = 3.281 feet
 	int32 altitude = 0;
 	LUINT_TypeDef distanceUnit;

 	//��ȡ�ٶ�
 	UserConfig_getDistanceUnit(&distanceUnit);

 	//��ȡ���θ߶�
 	altitude = (int32)Calculate_getAltitude();

 	if(distanceUnit == IMPERIAL)
 	{
 		altitude = (int32)(altitude * 3.281);
 	}

 	//��ʾ���θ߶�ֵ
 	Graphics_DispStringDec(3,altitude);

 	//��ʾ���θ߶ȱ�ʶ
 	Graphics_DispPic(94,IMAGE_ALTITUDE);

 }

 // �������ܣ���ʾ�����¶Ƚ���
 // ������������
 // ������������
 // �� �� ֵ����
 // ��    ע����
 void displayTemperature(void)
 {
 	int32 temperature = 0;
 	TEMP_TypeDef tempUnit;

 	//��ȡ�¶ȵ�λ
 	UserConfig_getTemperatureUnit(&tempUnit);

 	//��ȡ�����¶�
 	temperature = (int32)Calculate_getTemperature();

 	if(tempUnit == TEMP_F)
 	{
 	  	//�����¶�
 		temperature = (int32)(temperature * 1.8 + 32);
 	}

 	//��ʾ�����¶�����
 	Graphics_DispStringDec(3,temperature);

 	//��ʾ�¶ȱ�ʶ
 	Graphics_DispPic(94,IMAGE_TEMPERATURE);
        ;; // Fall through to label ?Subroutine1

 }

 // �������ܣ���ʾ�����߶Ƚ���
 // ������������
 // ������������
 // �� �� ֵ����
 // ��    ע����
 void displayClimbAltitude(void)
 {
   	//1 km = 0.621 mile ��1 m = 3.281 feet
 	int32 climbAltitude = 0;
 	LUINT_TypeDef distanceUnit;

 	//��ȡ���뵥λ
 	UserConfig_getDistanceUnit(&distanceUnit);

 	//��ȡ���������߶�
 	climbAltitude = (int32)Calculate_getClimbAltitude();

 	//Ӣ�Ƶ�λ
 	if(distanceUnit == IMPERIAL)
 	{
 		climbAltitude = (int32)(climbAltitude * 3.281);
 	}

 	//��ʾ�����߶�����
 	Graphics_DispStringDec(3,climbAltitude);

 	//��ʾ�����߶ȱ�ʶ
 	Graphics_DispPic(94,IMAGE_CLIMB);
 }

 // �������ܣ�ʾ��˼ˮӡ����
 // ������������
 // ������������
 // �� �� ֵ����
 // ��    ע����
 void displayChoiceMark(void)
 {
 	//Graphics_DispScreen(ichoice);
 }

 // �������ܣ���ʾ˯�߽���
 // ������������
 // ������������
 // �� �� ֵ����
 // ��    ע����
 void displaySleep(void)
 {
  	//��ʾ��SLEEP��
 	Graphics_DispChar(28,'S');
 	Graphics_DispChar(43,'L');
 	Graphics_DispChar(58,'E');
 	Graphics_DispChar(73,'E');
 	Graphics_DispChar(88,'P');
 }

 // �������ܣ���ʾLET'S GO����
 // ������������
 // ������������
 // �� �� ֵ����
 // ��    ע����
 void displayLETGO(void)
 {
   	//��ʾ��LET'S GO��
 	/*Graphics_DispChar(12,'L');
 	Graphics_DispChar(27,'E');
 	Graphics_DispChar(42,'T');
 	Graphics_DispChar(53,0x27);
 	Graphics_DispChar(65,'S');
 	Graphics_DispChar(91,'G');
 	Graphics_DispChar(106,'O');*/
 }

 // �������ܣ���ʾCOME ON����
 // ������������
 // ������������
 // �� �� ֵ����
 // ��    ע����
 void displayCOMEON(void)
 {
   	//��ʾCOME ON
 	/*Graphics_DispChar(13,'C');
 	Graphics_DispChar(28,'O');
 	Graphics_DispChar(43,'M');
 	Graphics_DispChar(58,'E');
 	Graphics_DispChar(86,'O');
 	Graphics_DispChar(101,'N');*/
 }

 // �������ܣ���ʾWELL DONE����
 // ������������
 // ������������
 // �� �� ֵ����
 // ��    ע����
 void displayWELLDONE(void)
 {
   	//��ʾWELL DONE
 	/*Graphics_DispChar(2  ,'W');
 	Graphics_DispChar(17 ,'E');
 	Graphics_DispChar(32 ,'L');
 	Graphics_DispChar(47 ,'L');
 	Graphics_DispChar(68 ,'D');
 	Graphics_DispChar(83 ,'O');
 	Graphics_DispChar(98 ,'N');
 	Graphics_DispChar(113,'E');*/
 }

 // �������ܣ���ʾ��������
 // ������������
 // ������������
 // �� �� ֵ����
 // ��    ע����
 void displayCharging(void)
 {
   	//��ʾ���綯��
   	static uint64 refreshTime = 0;
        uint8 battery = Battery_getBatteryLevel();

 	if(RTC_checkTimeout(refreshTime,4000) == IS_TIMEOUT)
 	{
 		RTC_obtainMilliSeconds(&refreshTime);
 	}
 	else if(RTC_checkTimeout(refreshTime,0) == IS_TIMEOUT && RTC_checkTimeout(refreshTime,1000) == NO_TIMEOUT)
 	{
          if(battery <= 1){
            Graphics_DispScreen(batterycharging3);
          }else if(battery <= 2){
            Graphics_DispScreen(batterycharging0);
          }else if(battery <= 3){
            Graphics_DispScreen(batterycharging1);
          }
          Graphics_repaint();
 	}
 	else if(RTC_checkTimeout(refreshTime,1000) == IS_TIMEOUT && RTC_checkTimeout(refreshTime,2000) == NO_TIMEOUT)
 	{
          if(battery <= 1){
            Graphics_DispScreen(batterycharging0);
          }else if(battery <= 2){
            Graphics_DispScreen(batterycharging1);
          }else if(battery <= 3){
            Graphics_DispScreen(batterycharging2);
          }
 		Graphics_repaint();
 	}
 	else if(RTC_checkTimeout(refreshTime,2000) == IS_TIMEOUT && RTC_checkTimeout(refreshTime,3000) == NO_TIMEOUT)
 	{
          if(battery <= 1){
            Graphics_DispScreen(batterycharging1);
          }else if(battery <= 2){
            Graphics_DispScreen(batterycharging2);
          }else if(battery <= 3){
            Graphics_DispScreen(batterycharging1);
          }
 		Graphics_repaint();
 	}
 	else if(RTC_checkTimeout(refreshTime,3000) == IS_TIMEOUT && RTC_checkTimeout(refreshTime,4000) == NO_TIMEOUT)
 	{
          if(battery <= 1){
            Graphics_DispScreen(batterycharging2);
          }else if(battery <= 2){
            Graphics_DispScreen(batterycharging0);
          }else if(battery <= 3){
            Graphics_DispScreen(batterycharging2);
          }
          Graphics_repaint();
 	}
 }

 // �������ܣ���ʾ�͵�������
 // ������������
 // ������������
 // �� �� ֵ����
 // ��    ע����
 void displayBatteryLow(void)
 {
 	Graphics_DispScreen(batterylow);
        Graphics_repaint();
 }

 // �������ܣ���ʾ����������
 // ������������
 // ������������
 // �� �� ֵ����
 // ��    ע����
 void displayBatteryFull(void)
 {
 	Graphics_DispScreen(batterycharging2);
        Graphics_repaint();
 }

 // �������ܣ���ʾ���ڴ�����
 // ������������
 // ������������
 // �� �� ֵ����
 // ��    ע����
 void displayMemoryLow(void)
 {
   	//��ʾ��MEMO LOW��
 	Graphics_DispChar(5  ,'M');
 	Graphics_DispChar(21 ,'E');
 	Graphics_DispChar(37 ,'M');
 	Graphics_DispChar(53 ,'O');
 	Graphics_DispChar(78 ,'L');
 	Graphics_DispChar(94 ,'O');
 	Graphics_DispChar(110,'W');
 }

 // �������ܣ���ʾ���ڴ�����
 // ������������
 // ������������
 // �� �� ֵ����
 // ��    ע����
 void displayMemoryFull(void)
 {
   	//��ʾ��MEMO FULL��
 	Graphics_DispChar(2  ,'M');
 	Graphics_DispChar(17 ,'E');
 	Graphics_DispChar(32 ,'M');
 	Graphics_DispChar(47 ,'O');
 	Graphics_DispChar(68 ,'F');
 	Graphics_DispChar(83 ,'U');
 	Graphics_DispChar(98 ,'L');
 	Graphics_DispChar(113,'L');
        ;; // Fall through to label ?Subroutine2
 }


 // �������ܣ���ʾ�ζ����ѽ���
 // ������������
 // ������������
 // �� �� ֵ����
 // ��    ע����
 void displayWakeByShake(void)
 {
 	uint8 startPosition = 0;
   	uint8 string[GREETING_MAX + 1];
 	uint8 i = 0;
 	uint8 digit = 0;

 	//��ʾ����״̬
 	if(Battery_getBatteryLevel() == BATTERY_LOW)
 	{
 	  	//��ʾ���ص�����
 		displayBatteryLow();
 		Graphics_repaint();
 		System_delay(1000);
 	}

 	//��ʾ�ڴ�״̬*
 	if(DataSave_getMemState() == FULL)
 	{
 	  	//��ʾ�ڴ���
 		displayMemoryFull();
 		Graphics_repaint();
 		System_delay(1000);
 	}
 	else if(DataSave_getMemState() == LOW)
 	{
 	  	//��ʾ�ڴ���
 		displayMemoryLow();
 		Graphics_repaint();
 		System_delay(1000);
 	}
        
/*        
 	//��ȡ�û����õ��ʺ���
 	UserConfig_getGreeting(string);

 	for(digit = 0;digit < GREETING_MAX + 1;digit++)
 	{
 	  	//�ж��ʺ��ﳤ��
 		if(*(string + digit) == '\0')
 		{
 			startPosition = (128 - digit * 15 - 2)/2;
 			break;
 		}
 	}

 	//�ж��ַ����Ƿ�����
 	while(*(string + i) != '\0')
 	{
 		Graphics_DispChar(startPosition + i * 15,*(string + i));
 		i++;
 	}

 	Graphics_repaint();
 	System_delay(1000);

 	//��ʾ������
 	if(Calculate_getPercent() == 0)
 	{
 		displayLETGO();
 		Graphics_repaint();
 	}
 	else if(Calculate_getPercent() == 100)
 	{
 		displayWELLDONE();
 		Graphics_repaint();
 	}
 	else
 	{
 		displayCOMEON();
 		Graphics_repaint();
 	}

 	System_delay(1000);
*/
 }

 // �������ܣ���ʾMEMS����
 // ������������
 // ������������
 // �� �� ֵ����
 // ��    ע����
 void displayMEMS_test(void)
 {
 	LIS3DH_DATA tempData;

 	LIS3DSH_getAxisData(&tempData);

 	Graphics_DispChar(0 ,'A');

 	Graphics_DispStringDec(29,abs(tempData.x));
 	Graphics_DispStringDec(64,abs(tempData.y));
 	Graphics_DispStringDec(99,abs(tempData.z));
 }

 // �������ܣ���ʾ�����¶Ƚ���
 // ������������
 // ������������
 // �� �� ֵ����
 // ��    ע����
 void displayTemperature_test(void)
 {
 	LPS331AP_DATA data;

 	LPS331AP_getData(&data);

 	Graphics_DispChar(0 ,'B');

 	//��ʾ�����¶�����
 	Graphics_DispStringDec(45,data.temperature);

 	//��ʾ�¶ȱ�ʶ
 	Graphics_DispPic(94,IMAGE_CELSIUS);

 }

 // �������ܣ���ʾ��ѹ����
 // ������������
 // ������������
 // �� �� ֵ����
 // ��    ע����
 void displayAirpressure_test(void)
 {
 	LPS331AP_DATA data;

 	LPS331AP_getData(&data);

 	Graphics_DispChar(0 ,'C');

 	//��ʾ��ǰ����ѹ
 	Graphics_DispStringDec(29,data.airPressure);

 	//��ʾmbar
 	Graphics_DispPic(94,IMAGE_MBAR);
        ;; // Fall through to label ?Subroutine3
 }

 // �������ܣ���ʾEEPROM����
 // ������������
 // ������������
 // �� �� ֵ����
 // ��    ע����
 static uint8 fixedData[10] =
 {
   	0x00,0x01,0x02,0x03,0x04,
 	0x05,0x06,0x07,0x08,0x09,
 };

 void displayEEPROM_test(void)
 {
     uint8 tempData[10];
 	static uint8 eeprom_statue = 0;

 	if(eeprom_statue == 0)
 	{
 		EEPROM_write(fixedData,60,10);

 		System_delay(20);

 		EEPROM_read(60,tempData,10);

 		for(uint8 j = 0;j < 10; j++)
 		{
 			if(tempData[j] != fixedData[j])
 			{
 				eeprom_statue = 1;

 				break;
 			}
 			else
 			{
 				eeprom_statue = 0;
 			}
 		}
 	}

 	//��������
 	Graphics_DispChar(0 ,'D');

 	//���������ж�
 	if(eeprom_statue == 1)
 	{
 	  	//EEPROM��������
 		Graphics_DispChar(27,'R');
 		Graphics_DispChar(42,'O');
 		Graphics_DispChar(57,'M');
 		Graphics_DispChar(82,'E');
 		Graphics_DispChar(97,'R');
 		Graphics_DispChar(112,'R');
 	}
 	else
 	{
 	  	//EEPROM��������
 		Graphics_DispChar(27,'R');
 		Graphics_DispChar(42,'O');
 		Graphics_DispChar(57,'M');
 		Graphics_DispChar(97,'O');
 		Graphics_DispChar(112,'K');
 	}
 }