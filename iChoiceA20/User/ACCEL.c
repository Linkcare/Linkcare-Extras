 // ͷ�ļ�����
 #include "ACCEL.h"
 #include "Configuration.h"
 
 // �������ھ������ĳ�������
 #define Lenwalet2 23
 
 #define Len SAMPLE
 
 // ��Ҫ�ı�������
 bit32_ACC Count_Step = 0;               // �ǲ���
 static bool_ACC  flagConCount_Step = false_ACC;// �����߲���־
 static bit32_ACC ContinCount_Step = 0;         // ��������
 static bit64_ACC Time_Count_Step = 0;          // �����߲�ʱ��
 static bit64_ACC CTime_Count_Step = 0;         // ��ʱ�߲�ʱ��
 bit16_ACC Data_ACCEL = 0;               // �ͼ��ٶ�����
 static bool_ACC  Valid_Step = 0;               // �������
 
 //״̬
 static bit32_ACC Time = 0;                     // ��ʱ
 static bit8_ACC  procees = 0;                  // ״̬����0��һ������;1��һ����Ѱ��;2�ڶ�����Ѱ��
 static bit16_ACC timeS1S2 = 0;
 static bit16_ACC timeS2S1 = 0;
 
 //�㷨���̲���
 static bit16_ACC filterData = 0;               // ��ǰ�˲��������
 bit16_ACC lastfilterData = 0;           // �ϴ��˲��������
 static bit16_ACC THRESHOLDPEAK = MinTHRESHOLDPEAK;
 static bit32_ACC Sum_Sample = 0;               // ������MemData3[]�е��ۼӺ�
 static bit16_ACC AVGTHRESHOLD = 0;             // ������MemData3[]�е�ƽ��ֵ
 
 bit8_ACC Time_sleep = 0;
 static bit8_ACC Time_sleepS1S2 = 0;            // 
 static bit8_ACC Time_sleepS2S1 = 0;            // 8λ�з�����
 
 static bit16_ACC MemData[Lenwalet2];           // �ϼ��ٻ����������ھ�����㣬��С�ǹ̶���
 bit16_ACC MemFilterData[Len_Mem];              // �ϼ��ٻ�������
 static bit16_ACC MemData3[Len];                // �ϼ��ٻ�����
 
 #define Len_AvrSS 100                          // ���建������С
 static bit16_ACC num_AvrSS = 0;                // AvrSS�������е���Ч����
 bit16_ACC mean_AvrSS = 0;                      // AvrSS�������е�ƽ��ֵ  
 static bit16_ACC AvrSS[Len_AvrSS];             // ���ڱ���ʱ�����Ļ�����
 static bit16_ACC TimeVal = 0;                  // ʱ����
 static bit16_ACC HighPeak = 0;
 static bit16_ACC HighVal = 1000;
 
 //����ˮƽ
 static bit16_ACC MaxACC = MinTHRESHOLDPEAK;
 
 
 extern uint32 lastStepsForCalculate;
 // ����ԭ��
 void ProcessACCELWave();
 
 // �������ܣ����һ�����ݵ�����ƽ��ֵ
 // ���������data[]  -- ���ݻ�����
 //           index   -- ��ʼ����ֵ
 //           TempLen -- ����
 // �����������
 // �� �� ֵ��ƽ��ֵ
 // ��    ע�������ݻ�����data[index]��ʼ�ĵ�Ԫ���������ƽ��ֵ������ΪTempLen
 //           Ϊȷ����������������뱣֤TempLem!=0
 bit16_ACC mean_ACC(bit16_ACC data[],bit16_ACC index,bit16_ACC TempLen)
 {
 	// ����2���ֲ�����������ʼ��
   	bit16_ACC i=0;
 	bit16_ACC result = 0;
   
 	// ѭ��������ۼӺ�
   	for(i=index;i<index+TempLen;i++)
 	{
 		result+=data[i];
 
 	}
   
   	// ���ƽ��ֵ
 	return result/TempLen;
 }
 
 // �������ܣ�������ݻ������е����ֵ
 // ���������data[]  -- ���ݻ�����
 //           Lendata -- ����
 // �����������
 // �� �� ֵ����
 // ��    ע�������ݻ�����data[0]��ʼ�ĵ�Ԫ��������ֵ������ΪLendata
 bit16_ACC Max_ACC(bit16_ACC data[],bit16_ACC Lendata)
 {
   // ����2���ֲ�����������ʼ��
 	bit16_ACC i=0;                        // ɨ����ʼ����
 	bit16_ACC mmax = data[0];             // ��ʱ�����ֵ
 
 	for(i=0;i<Lendata;i++)                // ѭ��ɨ������������
 	{
 		if(mmax<data[i])
 		{
       		// �����ǰɨ�赽�Ļ��������ݱ�ԭ���ҵ������ֵ�󣬾͸������ֵ
       		mmax=data[i];
     	}
 	}
 	
 	return mmax;                          // �������ֵ
 }
 
 // �������ܣ�������ݻ������е���Сֵ
 // ���������data[]  -- ���ݻ�����
 //           Lendata -- ����
 // �����������
 // �� �� ֵ����
 // ��    ע�������ݻ�����data[0]��ʼ�ĵ�Ԫ�������Сֵ������ΪLendata
 bit16_ACC Min_ACC(bit16_ACC data[],bit16_ACC Lendata)
 {
   // ����2���ֲ�����������ʼ��
 	bit16_ACC i=0;                        // ɨ����ʼ����
 	bit16_ACC mmin = data[0];             // ��ʱ����Сֵ 
 
 	for(i=0;i<Lendata;i++)                // ѭ��ɨ������������
 	{
 		if(mmin>data[i])                    // ԭ������ط�д����mmin<data[i]��Ӧ���ǲ��Եģ�
     	{
       		// �����ǰɨ�赽�Ļ��������ݱ�ԭ���ҵ�����СֵС���͸�����Сֵ
 			mmin=data[i];
     	}
 	}
 	return mmin;                          // ������Сֵ
 }
 
 bit32_ACC result;
 
 // �������ܣ��������
 // ���������data -- ���µ�����
 // �����������
 // �� �� ֵ���������Ľ��
 // ��    ע������ڲ�����һ��������MemData[]���������ݴ����MemData[0]��
 //           ���������̿��Լ򻯣���2��ѭ�����һ��ѭ��
 bit16_ACC new_conv_ACC(bit16_ACC data)
 {
     // 1.
     MemData[22] = MemData[21];
     result = -7*MemData[22];
   
     MemData[21] = MemData[20];
     result = result - MemData[21];
 
     MemData[20] = MemData[19];
     result = result - MemData[20];
 
     MemData[19] = MemData[18];
     result = result + MemData[19];
 
     MemData[18] = MemData[17];
     result = result + 3*MemData[18];
 
     MemData[17] = MemData[16];
     result = result + 5*MemData[17];
 
     MemData[16] = MemData[15];
     result = result + 7*MemData[16];
 
     MemData[15] = MemData[14];
     result = result + 10*MemData[15];
 
 	MemData[14] = MemData[13];
     result = result + 12*MemData[14];
 
 	MemData[13] = MemData[12];
     result = result + 14*MemData[13];
 
 	MemData[12] = MemData[11];
     result = result + 16*MemData[12];
 
 	MemData[11] = MemData[10];
     result = result + 16*MemData[11];
 
 	MemData[10] = MemData[9];
     result = result + 16*MemData[10];
 
 	MemData[9] = MemData[8];
     result = result + 14*MemData[9];
 
 	MemData[8] = MemData[7];
     result = result + 12*MemData[8];
 
 	MemData[7] = MemData[6];
     result = result + 10*MemData[7];
 
 	MemData[6] = MemData[5];
     result = result + 7*MemData[6];
 
 	MemData[5] = MemData[4];
     result = result + 5*MemData[5];
 
 	MemData[4] = MemData[3];
     result = result + 3*MemData[4];
 
 	MemData[3] = MemData[2];
     result = result + 1*MemData[3];
 
 	MemData[2] = MemData[1];
     result = result - MemData[2];
 
 	MemData[1] = MemData[0];
     result = result - MemData[1];
 
 	MemData[0] =data;                     // �������ݴ���ڵ�Ԫ0��
     result = result - 7*MemData[0];
 
   
     // 4.���ƽ��ֵ��������ƽ��ֵ
 	result = result/20;
 	bit16_ACC result2 = result;
 	
 	return result2;
 }
 
 // �������ܣ����������ݴ��뵽MemData3[]�У��������ۼӺͺ�ƽ��ֵ
 // ���������data -- ���µ�����
 // �����������
 // �� �� ֵ����
 // ��    ע����
 void Mem_ACC(bit16_ACC data)
 {
 	// ����һ���ֲ�����������ʼ��
   	bit8_ACC i = 0;
   
   	// �����͵��ھ�ֵ��Ҫ�����ߵ�ֵ
   	Sum_Sample -= MemData3[Len-1];
 	
 	// 1��������MemData3[]���������ƣ�����Ԫ0�Ƶ���Ԫ1����Ԫ1�Ƶ���Ԫ2������
 	for(i=Len-1;i>0;i--)
 	{
 		MemData3[i] = MemData3[i-1];
 	}
 	MemData3[0] = data;                   // �������ݴ���ڵ�Ԫ0��
 
   	// 2���µĲ����ͣ�MemData3[0]+MemData3[1]+...+MemData3[Len-1]
 	Sum_Sample += data;
 
 	// 3.����ƽ��ֵ
   	AVGTHRESHOLD = Sum_Sample/Len;
 }
 
 // �������ܣ�Ѱ�����ֵ
 // �����������
 // �����������
 // �� �� ֵ����
 // ��    ע����Ҫ�Ĳ�������ȫ�ֱ�����
 void FindMax()
 {
 	// 1.�����ǰ���ļ��ٶȣ�С���˲��õ����ݣ������˲��õ����ݴ���
   	if (MaxACC<filterData)
 	{
 		MaxACC = filterData;
 	}
   
   	// 2.���ʱ����2���Ĳ����ʣ����޸���ֵ
 	if(Time%(2*SAMPLE)==0)
 	{
 		THRESHOLDPEAK = MaxACC;
 		MaxACC = MinTHRESHOLDPEAK;
 	}
 }
 
 // �������ܣ��Ҳ���
 // �����������
 // �����������
 // �� �� ֵ����
 // ��    ע����
 void FindPeakArea()
 {
 	if(filterData>(THRESHOLDPEAK*corr1/10+AVGTHRESHOLD))
 	{
 		// �����ǰ�˲��õ����ݴ���һ���������������һ��״̬
     	// ������߷�ֵ���óɵ�ǰֵ
    	 	procees = 1;
 		HighPeak = filterData;
 	}
 }
 
 void FindPeak()
 {
 	if(HighPeak<filterData)
 	{	
 		HighPeak=filterData;
 		timeS2S1=0;	
 		timeS1S2=0;	
 	}
 	if(filterData<AVGTHRESHOLD-THRESHOLDPEAK*corr1/10)
 	{
 		procees = 2;
 		HighVal = filterData;
 	}
 }
 
 void FindValArea()
 {
 	if(filterData<AVGTHRESHOLD-THRESHOLDPEAK*corr1/10)
 	{
 		if(HighVal>filterData)
 		{	
 			HighVal=filterData;
 			timeS1S2=0;	
 		}
 		if(HighPeak- HighVal>THRESHOLDPEAK*corr2/10)
 		{
 			if(TimeVal>MS250)
 			{
 				procees = 3;
 			}
 		}
 	}
 	else if(filterData>AVGTHRESHOLD+THRESHOLDPEAK*corr1/10||timeS1S2>MS1000)
 	{
 		procees = 0;
 		HighPeak = filterData;
 		flagConCount_Step = false_ACC;
 		ContinCount_Step = 0;
 		timeS2S1 = 0;
 		timeS1S2 = 0;
 	}
 }
 
 // �������ܣ�������
 // �����������
 // �����������
 // �� �� ֵ����
 // ��    ע����
 void updataRR()
 {
   	// 1.��������num_AvrSSû�����һ��ʱ����������+1
   	if(num_AvrSS<Len_AvrSS-1)
   	{
     	num_AvrSS++;
   	}
   
   	// 2.�������������Ч������1���ͽ������д���
 	if(num_AvrSS>1)
 	{
 		if(num_AvrSS==Len_AvrSS)	
 		{	
       		// ����������Ѿ��Ƶ������
 			bit16_ACC i=0;
 			// ���������е�����λ����AvrSS[0]<-AvrSS[1],AvrSS[1]<-AvrSS[2]....
       		for(i=0;i<Len_AvrSS-1;i++)
 			{
 				AvrSS[i] = AvrSS[i+1];
 			}
       		// ���µ�ʱ�������浽���һ����Ԫ 
 			AvrSS[Len_AvrSS-1] = TimeVal;		
 		}
 		else
 		{
 			// ���������û�Ƶ����ֵ���ͽ���ǰʱ�䱣�浽������ָ��ĵ�Ԫ
       		// ��������㷨����ô0�ŵ�Ԫ��û�������ˣ�����
       		AvrSS[num_AvrSS] = TimeVal;
       		// ����AvrSS[0]��AvrSS[num_AvrSS-1]������ƽ��ֵ�������浽mean_AvrSS
 			mean_AvrSS = mean_ACC(AvrSS,0,num_AvrSS);
 		}
 	}
 	
   	// 3.ÿ�ε��øú����󣬶���ʱ������0
   	TimeVal = 0;
 }
 void FindVal()
 {
 	HighPeak = filterData;
 
   	if(flagConCount_Step)
 	{
 		updataRR();
 		Valid_Step = true_ACC;
 		Count_Step++;
 		Time_sleepS2S1 = 0;
 		procees = 0;
 		timeS1S2=0;
 		timeS2S1 = 0;
 	}
 	else
 	{
 		updataRR();
 
     	ContinCount_Step++;
 		Time_sleepS2S1 = 0;
 		procees = 0;
 		timeS1S2=0;
 		timeS2S1 = 0;
 				
 		if(ContinCount_Step>=8)
 		{
 			flagConCount_Step = true_ACC;
 			Count_Step+=ContinCount_Step;
       		Time_Count_Step += CTime_Count_Step;
 		}
 		else if(ContinCount_Step>=4)
 		{
 			if(Max_ACC(AvrSS,4)-Min_ACC(AvrSS,4)<MS100)
 			{
 				flagConCount_Step = true_ACC;
 				Count_Step+=ContinCount_Step;
         		Time_Count_Step += CTime_Count_Step;
 			}
 		}
 	}
 	
   	if(Count_Step > 1000000000)
   	{
     	// �������������10�ڲ��ˣ�����0��
     	Count_Step = 0;
   	}
   
   	if(Time_Count_Step > 1000000000)
   	{
     	// �������10�ڲ��ˣ�����0��
     	Time_Count_Step = 0;
   	}
 }
 
 // �������ܣ�������ٶȲ���
 // �����������
 // �����������
 // �� �� ֵ����
 // ��    ע����
 void ProcessACCELWave()
 {
   	// 1.����ǰ�˲��õ����ݱ�������
 	lastfilterData = filterData;
   	// 2.���������õ��ļ��ٶ���Ϊ���µ�����
 	filterData =Data_ACCEL;
   	// 3.�Լ��ٶ����ݽ��о�����㣬�������Ľ������Ϊ�����˲��õ�����
 	filterData = new_conv_ACC(filterData);
   	// 4.�������˲��õ����ݽ���ƽ����������µ���ֵ  
 	Mem_ACC(filterData);
 }
 
 // �������ܣ����ٶȼ���ģ��ĳ�ʼ��
 // �����������
 // �����������
 // �� �� ֵ����
 // ��    ע���Լ���ģ�����漰���ı������г�ʼ��
 void Init_ACCEL()
 {
   //�������
 	Count_Step = 0;                       // �ǲ���
 	lastStepsForCalculate  = 0;
 	Data_ACCEL = 0;                       // �ͼ��ٶ�����
 	Valid_Step = 0;                       // �������
   	Time_Count_Step = 0;                  // �����߲�ʱ��
   	CTime_Count_Step = 0;                 // ��ʱ�߲�ʱ��
 	flagConCount_Step = false_ACC;
   	ContinCount_Step = 0;                 // ��������
 
   	//״̬
 	Time = 0;                             // ��ʱ
 	procees = 0;                          // ״̬����0��һ������;1��һ����Ѱ��;2�ڶ�����Ѱ��
 
   	//�㷨���̲���
 	filterData = 0;
 	lastfilterData = 0;
 	THRESHOLDPEAK = MinTHRESHOLDPEAK;
 	Sum_Sample = 0;
 	AVGTHRESHOLD = 0;
 	Time_sleep = 0;
 	Time_sleepS1S2 = 0;
   	Time_sleepS2S1 = 0;
 	timeS1S2 = 0;
 	timeS2S1 = 0;                         // ��ʱ������0
 
 	bit16_ACC i =0;
 	for(i=0;i<Lenwalet2;i++)
 	{
 		MemData[i] = 0;                     // �ϼ��ٻ�����
 	}
 	for(i=0;i<Len_Mem;i++)
 	{
 		MemFilterData[i] = 0;               // �ϼ��ٻ�����
 	}
 	for(i=0;i<Len;i++)
 	{
 		MemData3[i] = 0;                    // �ϼ��ٻ�����
 	}
 
 	for(i=0;i<Len_AvrSS;i++)
 	{
 		AvrSS[i] = 0;//
 	}
 
 	num_AvrSS = 0;                        // ��������0
 	mean_AvrSS = 0;                       // ƽ��ֵ��0 
 	TimeVal = 0;                          // ���ʱ���ʼ����0
 	HighPeak = 0;
 	HighVal = 1000;
 
 	//���ݱ任
 	//����ˮƽ
 	MaxACC = MinTHRESHOLDPEAK;
 }
 
 #if	(BACKUP == ON)
 void Init_ACCEL_recover(void)
 {
   	UserConfig_getStepBackup((uint32*)&Count_Step);
 	UserConfig_getLastStepBackup(&lastStepsForCalculate);
 }
 #endif
 
 // �������ܣ����ݴ����������
 // �����������
 // �����������
 // �� �� ֵ����
 // ��    ע����
 void main_ACCEL()
 {
   // 1.
 	if(Valid_Step)
 	{
 		Valid_Step = 0;
 	}
 	
   // 2.��ʱ
 	Time++;                               // ÿ�ε���ʱ��������ʱ�䣨��ʵ���Ǵ�����+1
 	TimeVal++;                            // ����ȼ��ʱ��
 
   // 3.
   if(flagConCount_Step || (ContinCount_Step > 1))
   {
     if(TimeVal > MS850)
     {
       // ������ʱ�䳬����850MS
       timeS1S2=0;
       procees = 0;                      // ����״̬���ó�0
       flagConCount_Step = false_ACC;
       ContinCount_Step = 0;
       CTime_Count_Step = 0;
       timeS2S1 = 0;
     }
   }
 	
   // 4.�����ʱ��������10����
   if(Time>(SAMPLE*60*10))
 	{
     // �����ʱ��������10���ӵļ����������ó�2��ļ���ֵ
     // Ϊʲô����0�أ�����
     Time = 2*SAMPLE;
   }
   
   // 5.������ٶȲ���
 	ProcessACCELWave();
 	
   // 6.�����ʱ��������2����
   if(Time>(2*SAMPLE))
 	{
 //		FindMax();
 	}
 
   // 7.
   if(flagConCount_Step)
   {
     Time_Count_Step++;
   }
   else if(ContinCount_Step>0)
   {
     CTime_Count_Step++;
   }
 
   // 8.�����ʱ������1�����Ժ�ÿ�ζ�Ҫ�������д���
 	if(Time>MS1000)
 	{
     // ����״̬�������в�ͬ�Ĵ�����
     if(procees==0)
 		{
 		  // �����ǰ����״̬Ϊ0
       if(Time_sleepS2S1==0)
 			{
 				// ����ñ���Ϊ0���͵��ú���
         FindPeakArea();
 			}
 			else
       {
         // ����ñ���������0���ͼ�1
         // ���⣺�ñ������˳�ʼ��ʱ����0�⣬û�������Ĳ����ˣ���ô���ܲ�����0�أ�����
         Time_sleepS2S1--;
       }
 			if((timeS2S1++)>MS600)
 			{
         // �����ʱ��+1�󳬹���600MS
 				flagConCount_Step = false_ACC;
 				ContinCount_Step = 0;
 				timeS2S1 = 0;
 				timeS1S2=0;
         CTime_Count_Step = 0;
 			}
 		} // end if process = 0
 		else if(procees==1)
 		{
 		  // �����ǰ����״̬Ϊ1
 			FindPeak();
 			if((timeS2S1++)>MS600)
 			{
 				// �����ʱ��+1�󳬹���600MS
 				flagConCount_Step = false_ACC;
 				ContinCount_Step = 0;
 				CTime_Count_Step = 0;
 				timeS2S1 = 0;
 				timeS1S2=0;
 				procees = 0;
 		  	}
 		} // end if process = 1
 		else if(procees==2)
 		{
 		  // �����ǰ����״̬Ϊ2
 			timeS1S2++;
 			if(Time_sleepS1S2==0)
 			{	
 				FindValArea();
 				
 				if(timeS1S2>MS500)
 				{
 					timeS1S2=0;
 					procees = 0;
 					flagConCount_Step = false_ACC;
 					ContinCount_Step = 0;
           			CTime_Count_Step = 0;
 					timeS2S1 = 0;
 				}	
 			}
 			else
 			{
         		Time_sleepS1S2--;
       		}
 		} // end if process = 2
 		else
 		{
 			// ���processΪ����
       		FindVal();
 		}
 	} // end if 1000�����ʱ
 }
