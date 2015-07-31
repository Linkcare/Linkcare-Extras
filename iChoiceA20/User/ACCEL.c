 // 头文件定义
 #include "ACCEL.h"
 #include "Configuration.h"
 
 // 定义用于卷积运算的常量数组
 #define Lenwalet2 23
 
 #define Len SAMPLE
 
 // 重要的变量定义
 bit32_ACC Count_Step = 0;               // 记步数
 static bool_ACC  flagConCount_Step = false_ACC;// 连续走步标志
 static bit32_ACC ContinCount_Step = 0;         // 连续步数
 static bit64_ACC Time_Count_Step = 0;          // 连续走步时间
 static bit64_ACC CTime_Count_Step = 0;         // 临时走步时间
 bit16_ACC Data_ACCEL = 0;               // 和加速度数据
 static bool_ACC  Valid_Step = 0;               // 迈步标记
 
 //状态
 static bit32_ACC Time = 0;                     // 计时
 static bit8_ACC  procees = 0;                  // 状态——0：一步结束;1第一峰搜寻中;2第二峰搜寻中
 static bit16_ACC timeS1S2 = 0;
 static bit16_ACC timeS2S1 = 0;
 
 //算法过程参量
 static bit16_ACC filterData = 0;               // 当前滤波后的数据
 bit16_ACC lastfilterData = 0;           // 上次滤波后的数据
 static bit16_ACC THRESHOLDPEAK = MinTHRESHOLDPEAK;
 static bit32_ACC Sum_Sample = 0;               // 缓冲区MemData3[]中的累加和
 static bit16_ACC AVGTHRESHOLD = 0;             // 缓冲区MemData3[]中的平均值
 
 bit8_ACC Time_sleep = 0;
 static bit8_ACC Time_sleepS1S2 = 0;            // 
 static bit8_ACC Time_sleepS2S1 = 0;            // 8位有符号数
 
 static bit16_ACC MemData[Lenwalet2];           // 合加速缓冲区，用于卷积运算，大小是固定的
 bit16_ACC MemFilterData[Len_Mem];              // 合加速缓冲区，
 static bit16_ACC MemData3[Len];                // 合加速缓冲区
 
 #define Len_AvrSS 100                          // 定义缓冲区大小
 static bit16_ACC num_AvrSS = 0;                // AvrSS缓冲区中的有效个数
 bit16_ACC mean_AvrSS = 0;                      // AvrSS缓冲区中的平均值  
 static bit16_ACC AvrSS[Len_AvrSS];             // 用于保存时间间隔的缓冲区
 static bit16_ACC TimeVal = 0;                  // 时间间隔
 static bit16_ACC HighPeak = 0;
 static bit16_ACC HighVal = 1000;
 
 //上限水平
 static bit16_ACC MaxACC = MinTHRESHOLDPEAK;
 
 
 extern uint32 lastStepsForCalculate;
 // 函数原型
 void ProcessACCELWave();
 
 // 函数功能：求出一组数据的算术平均值
 // 输入参数：data[]  -- 数据缓冲区
 //           index   -- 起始索引值
 //           TempLen -- 个数
 // 输出参数：无
 // 返 回 值：平均值
 // 备    注：将数据缓冲区data[index]开始的单元，求出算术平均值，长度为TempLen
 //           为确保除法不溢出，必须保证TempLem!=0
 bit16_ACC mean_ACC(bit16_ACC data[],bit16_ACC index,bit16_ACC TempLen)
 {
 	// 定义2个局部变量，并初始化
   	bit16_ACC i=0;
 	bit16_ACC result = 0;
   
 	// 循环，求出累加和
   	for(i=index;i<index+TempLen;i++)
 	{
 		result+=data[i];
 
 	}
   
   	// 求出平均值
 	return result/TempLen;
 }
 
 // 函数功能：求出数据缓冲区中的最大值
 // 输入参数：data[]  -- 数据缓冲区
 //           Lendata -- 个数
 // 输出参数：无
 // 返 回 值：无
 // 备    注：将数据缓冲区data[0]开始的单元，求出最大值，长度为Lendata
 bit16_ACC Max_ACC(bit16_ACC data[],bit16_ACC Lendata)
 {
   // 定义2个局部变量，并初始化
 	bit16_ACC i=0;                        // 扫描起始索引
 	bit16_ACC mmax = data[0];             // 临时的最大值
 
 	for(i=0;i<Lendata;i++)                // 循环扫描整个缓冲区
 	{
 		if(mmax<data[i])
 		{
       		// 如果当前扫描到的缓冲区内容比原先找到的最大值大，就更新最大值
       		mmax=data[i];
     	}
 	}
 	
 	return mmax;                          // 返回最大值
 }
 
 // 函数功能：求出数据缓冲区中的最小值
 // 输入参数：data[]  -- 数据缓冲区
 //           Lendata -- 个数
 // 输出参数：无
 // 返 回 值：无
 // 备    注：将数据缓冲区data[0]开始的单元，求出最小值，长度为Lendata
 bit16_ACC Min_ACC(bit16_ACC data[],bit16_ACC Lendata)
 {
   // 定义2个局部变量，并初始化
 	bit16_ACC i=0;                        // 扫描起始索引
 	bit16_ACC mmin = data[0];             // 临时的最小值 
 
 	for(i=0;i<Lendata;i++)                // 循环扫描整个缓冲区
 	{
 		if(mmin>data[i])                    // 原先这个地方写成了mmin<data[i]，应该是不对的！
     	{
       		// 如果当前扫描到的缓冲区内容比原先找到的最小值小，就更新最小值
 			mmin=data[i];
     	}
 	}
 	return mmin;                          // 返回最小值
 }
 
 bit32_ACC result;
 
 // 函数功能：卷积运算
 // 输入参数：data -- 最新的数据
 // 输出参数：无
 // 返 回 值：卷积运算的结果
 // 备    注：卷积内部存在一个缓冲区MemData[]，最新数据存放在MemData[0]中
 //           卷积运算过程可以简化，将2个循环变成一个循环
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
 
 	MemData[0] =data;                     // 最新数据存放在单元0中
     result = result - 7*MemData[0];
 
   
     // 4.求出平均值，并返回平均值
 	result = result/20;
 	bit16_ACC result2 = result;
 	
 	return result2;
 }
 
 // 函数功能：将最新数据存入到MemData3[]中，并计算累加和和平均值
 // 输入参数：data -- 最新的数据
 // 输出参数：无
 // 返 回 值：无
 // 备    注：无
 void Mem_ACC(bit16_ACC data)
 {
 	// 定义一个局部变量，并初始化
   	bit8_ACC i = 0;
   
   	// 采样和等于旧值－要被移走的值
   	Sum_Sample -= MemData3[Len-1];
 	
 	// 1。将数组MemData3[]的内容右移，即单元0移到单元1，单元1移到单元2。。。
 	for(i=Len-1;i>0;i--)
 	{
 		MemData3[i] = MemData3[i-1];
 	}
 	MemData3[0] = data;                   // 最新数据存放在单元0中
 
   	// 2。新的采样和＝MemData3[0]+MemData3[1]+...+MemData3[Len-1]
 	Sum_Sample += data;
 
 	// 3.计算平均值
   	AVGTHRESHOLD = Sum_Sample/Len;
 }
 
 // 函数功能：寻找最大值
 // 输入参数：无
 // 输出参数：无
 // 返 回 值：无
 // 备    注：需要的参数都在全局变量中
 void FindMax()
 {
 	// 1.如果当前最大的加速度，小于滤波好的数据，则用滤波好的数据代替
   	if (MaxACC<filterData)
 	{
 		MaxACC = filterData;
 	}
   
   	// 2.如果时间是2倍的采样率，则修改阈值
 	if(Time%(2*SAMPLE)==0)
 	{
 		THRESHOLDPEAK = MaxACC;
 		MaxACC = MinTHRESHOLDPEAK;
 	}
 }
 
 // 函数功能：找步伐
 // 输入参数：无
 // 输出参数：无
 // 返 回 值：无
 // 备    注：无
 void FindPeakArea()
 {
 	if(filterData>(THRESHOLDPEAK*corr1/10+AVGTHRESHOLD))
 	{
 		// 如果当前滤波好的数据大于一定的数，则进入下一个状态
     	// 并将最高峰值设置成当前值
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
 
 // 函数功能：？？？
 // 输入参数：无
 // 输出参数：无
 // 返 回 值：无
 // 备    注：无
 void updataRR()
 {
   	// 1.当计数器num_AvrSS没到最后一个时，计数器就+1
   	if(num_AvrSS<Len_AvrSS-1)
   	{
     	num_AvrSS++;
   	}
   
   	// 2.如果缓冲区中有效数大于1，就进行下列处理
 	if(num_AvrSS>1)
 	{
 		if(num_AvrSS==Len_AvrSS)	
 		{	
       		// 如果计数器已经计到最大了
 			bit16_ACC i=0;
 			// 将缓冲区中的数移位，即AvrSS[0]<-AvrSS[1],AvrSS[1]<-AvrSS[2]....
       		for(i=0;i<Len_AvrSS-1;i++)
 			{
 				AvrSS[i] = AvrSS[i+1];
 			}
       		// 最新的时间间隔保存到最后一个单元 
 			AvrSS[Len_AvrSS-1] = TimeVal;		
 		}
 		else
 		{
 			// 如果计数器没计到最大值，就将当前时间保存到计数器指向的单元
       		// 按照这个算法，那么0号单元就没有数据了？？？
       		AvrSS[num_AvrSS] = TimeVal;
       		// 计算AvrSS[0]到AvrSS[num_AvrSS-1]的算术平均值，并保存到mean_AvrSS
 			mean_AvrSS = mean_ACC(AvrSS,0,num_AvrSS);
 		}
 	}
 	
   	// 3.每次调用该函数后，都将时间间隔清0
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
     	// 如果步伐数大于10亿步了，就清0了
     	Count_Step = 0;
   	}
   
   	if(Time_Count_Step > 1000000000)
   	{
     	// 如果大于10亿步了，就清0了
     	Time_Count_Step = 0;
   	}
 }
 
 // 函数功能：处理加速度波形
 // 输入参数：无
 // 输出参数：无
 // 返 回 值：无
 // 备    注：无
 void ProcessACCELWave()
 {
   	// 1.将当前滤波好的数据保存起来
 	lastfilterData = filterData;
   	// 2.将传感器得到的加速度作为最新的数据
 	filterData =Data_ACCEL;
   	// 3.对加速度数据进行卷积运算，卷积运算的结果将作为最新滤波好的数据
 	filterData = new_conv_ACC(filterData);
   	// 4.用最新滤波好的数据进行平均，算出最新的阈值  
 	Mem_ACC(filterData);
 }
 
 // 函数功能：加速度计算模块的初始化
 // 输入参数：无
 // 输出参数：无
 // 返 回 值：无
 // 备    注：对计算模块内涉及到的变量进行初始化
 void Init_ACCEL()
 {
   //输入输出
 	Count_Step = 0;                       // 记步数
 	lastStepsForCalculate  = 0;
 	Data_ACCEL = 0;                       // 和加速度数据
 	Valid_Step = 0;                       // 迈步标记
   	Time_Count_Step = 0;                  // 连续走步时间
   	CTime_Count_Step = 0;                 // 临时走步时间
 	flagConCount_Step = false_ACC;
   	ContinCount_Step = 0;                 // 连续步数
 
   	//状态
 	Time = 0;                             // 计时
 	procees = 0;                          // 状态——0：一步结束;1第一峰搜寻中;2第二峰搜寻中
 
   	//算法过程参量
 	filterData = 0;
 	lastfilterData = 0;
 	THRESHOLDPEAK = MinTHRESHOLDPEAK;
 	Sum_Sample = 0;
 	AVGTHRESHOLD = 0;
 	Time_sleep = 0;
 	Time_sleepS1S2 = 0;
   	Time_sleepS2S1 = 0;
 	timeS1S2 = 0;
 	timeS2S1 = 0;                         // 计时变量清0
 
 	bit16_ACC i =0;
 	for(i=0;i<Lenwalet2;i++)
 	{
 		MemData[i] = 0;                     // 合加速缓冲区
 	}
 	for(i=0;i<Len_Mem;i++)
 	{
 		MemFilterData[i] = 0;               // 合加速缓冲区
 	}
 	for(i=0;i<Len;i++)
 	{
 		MemData3[i] = 0;                    // 合加速缓冲区
 	}
 
 	for(i=0;i<Len_AvrSS;i++)
 	{
 		AvrSS[i] = 0;//
 	}
 
 	num_AvrSS = 0;                        // 计数器清0
 	mean_AvrSS = 0;                       // 平均值清0 
 	TimeVal = 0;                          // 间隔时间初始化成0
 	HighPeak = 0;
 	HighVal = 1000;
 
 	//数据变换
 	//上限水平
 	MaxACC = MinTHRESHOLDPEAK;
 }
 
 #if	(BACKUP == ON)
 void Init_ACCEL_recover(void)
 {
   	UserConfig_getStepBackup((uint32*)&Count_Step);
 	UserConfig_getLastStepBackup(&lastStepsForCalculate);
 }
 #endif
 
 // 函数功能：数据处理的主函数
 // 输入参数：无
 // 输出参数：无
 // 返 回 值：无
 // 备    注：无
 void main_ACCEL()
 {
   // 1.
 	if(Valid_Step)
 	{
 		Valid_Step = 0;
 	}
 	
   // 2.计时
 	Time++;                               // 每次调用时，都将总时间（其实就是次数）+1
 	TimeVal++;                            // 谷与谷间隔时间
 
   // 3.
   if(flagConCount_Step || (ContinCount_Step > 1))
   {
     if(TimeVal > MS850)
     {
       // 如果间隔时间超过了850MS
       timeS1S2=0;
       procees = 0;                      // 处理状态设置成0
       flagConCount_Step = false_ACC;
       ContinCount_Step = 0;
       CTime_Count_Step = 0;
       timeS2S1 = 0;
     }
   }
 	
   // 4.如果计时器超过了10分钟
   if(Time>(SAMPLE*60*10))
 	{
     // 如果计时器超过了10分钟的计数，就设置成2秒的计数值
     // 为什么不是0呢？？？
     Time = 2*SAMPLE;
   }
   
   // 5.处理加速度波形
 	ProcessACCELWave();
 	
   // 6.如果计时器超过了2秒钟
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
 
   // 8.如果计时器超过1秒钟以后，每次都要进行下列处理
 	if(Time>MS1000)
 	{
     // 根据状态变量进行不同的处理方案
     if(procees==0)
 		{
 		  // 如果当前处理状态为0
       if(Time_sleepS2S1==0)
 			{
 				// 如果该变量为0，就调用函数
         FindPeakArea();
 			}
 			else
       {
         // 如果该变量不等于0，就减1
         // 问题：该变量除了初始化时赋成0外，没有其它的操作了，怎么可能不等于0呢？？？
         Time_sleepS2S1--;
       }
 			if((timeS2S1++)>MS600)
 			{
         // 如果计时器+1后超过了600MS
 				flagConCount_Step = false_ACC;
 				ContinCount_Step = 0;
 				timeS2S1 = 0;
 				timeS1S2=0;
         CTime_Count_Step = 0;
 			}
 		} // end if process = 0
 		else if(procees==1)
 		{
 		  // 如果当前处理状态为1
 			FindPeak();
 			if((timeS2S1++)>MS600)
 			{
 				// 如果计时器+1后超过了600MS
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
 		  // 如果当前处理状态为2
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
 			// 如果process为其它
       		FindVal();
 		}
 	} // end if 1000毫秒计时
 }
