#include "BLE.H"
#include "stdlib.h"
#include "DataSave.h"
#include "Calculate.h"
#include "DeviceInfo.h"
#include "Upload.h"
#include "Rsa.h"

#define  IDCODE1	((uint32 *)0X1FF80050)
#define  IDCODE2	((uint32 *)0X1FF80054)
#define  IDCODE3	((uint32 *)0X1FF80064)

uint16 pwd = 0;

uint16 getIDcode(void)
{
    uint32 tempID;
    uint16 IDnum;

    tempID =(uint32)((*IDCODE1) +(*IDCODE2) +(*IDCODE3));

    IDnum = rand();

    IDnum = IDnum +((uint16)((tempID >> 16) + tempID));

    IDnum %= 10000;

    return IDnum;
}

extern Q_EVENT Q_SendData;

void USART_sendOneByte(unsigned char data);
void USART_sendMulByte(unsigned char *buf,unsigned int num);

uint8 COM_REV_OK[5] = {0x55, 0xaa, 0x02, 0xf0, 0xf2};
uint8 COM_REV_ER[5] = {0x55, 0xaa, 0x02, 0xfe, 0x01};
uint8 COM_REV_HE[5] = {0x55, 0xaa, 0x02, 0xff, 0x02};
uint8 COM_PWD_OK[6] = {0x55, 0xaa, 0x03, 0xb1, 0x00, 0xb4};
uint8 COM_PWD_ER[6] = {0x55, 0xaa, 0x03, 0xb1, 0x01, 0xb5};
/**************************��ʱ����****************************/
#define	COMMAND_TIME_OUT	500

#define MAX_BUFF	100
struct
{

	lpsa_u8	Com;//
	lpsa_u8	ComLen;
	lpsa_u8	Combuff[MAX_BUFF];

}ComData;


lpsa_i8	comm_get_command (void)
{
	return 0;
}

extern lpsa_i8	WrStart;
static lpsa_u8	DelayJmp = 0;
static lpsa_u16  DelayMs = 0;

/***********************���ݸ��ٷ��ͺ���****************************/
//0��ֱ�ӷ�����һ��
//-5:������һ��
static lpsa_i8 FunBleIdleD (void)
{
	lpsa_i8	DataK				= 0;

	if(BleCtrl() == 1 ||BleCtrl() == 0)
	{
		Idle.BLE_IDLE_D = 0xFF;
	}

	switch (Idle.BLE_IDLE_D)
	{
	case DATA_SEND_BLOCK_BEGIN:
		{
			Idle.BLE_IDLE_D = DATA_SEND_BLOCK_WAIT;
		}
	case DATA_SEND_BLOCK_WAIT:
		{
			BleDelay(MS_CLEAN);
			BleApi.BleWakeUp ();
			DelayMs = 2;	
			if (BleTools.SEND_BLOCK_INFO == 1)
				DelayJmp = DATA_SEND_BLOCK_1;
			else if (BleTools.SEND_BLOCK_INFO == 2)
				DelayJmp = DATA_SEND_BLOCK_2;
			Idle.BLE_IDLE_D = DATA_SEND_BLOCK_DELAY;
		}break;
	case DATA_SEND_BLOCK_1:
		{
			DataK = DATA_SEND_BLOCK_1_FUN ();
			if (DataK == 0)
				return 0;
			else
			{
				BleTools.SEND_BLOCK_INFO = 0;
				Idle.BLE_IDLE_D					 = DATA_SEND_BLOCK_BEGIN;
			}
		}break;
	case DATA_SEND_BLOCK_2:
		{
			DataK = DATA_SEND_BLOCK_2_FUN ();
			if (DataK == 0)
				return 0;
			else
			{
				BleTools.SEND_BLOCK_INFO = 0;
				Idle.BLE_IDLE_D					 = DATA_SEND_BLOCK_BEGIN;

			}
		}break;
	case DATA_SEND_BLOCK_DELAY:
		{
			if (BleDelay (MS_READ) > DelayMs)
			{
				Idle.BLE_IDLE_D = DelayJmp;
			}
		}break;
	default:
		{
			BleTools.SEND_BLOCK_INFO = 0;
			Idle.BLE_IDLE_D			 = DATA_SEND_BLOCK_BEGIN;
			Idle.BLE_IDLE_S  		 = WRITE_BLOCK_BEGIN;
			DelayJmp 				 = 0;
			DelayMs 				 = 0;
			Idle.BLE_IDLE_C				 = COMM_GET_DATA_0;
			return -5;//״̬�����˳�
		}
	}
	return 0;
}

// �������ܣ�Ӧ��B0ָ����ͼ��ܵ�����������
// ����������δ���ܵ�����pwd,0<=pwd<10000
// ������������
// �� �� ֵ������
// ��    ע��
static void Ble_respondAck_B0(uint16 pwd)
{
    uint8   cmd[9];
    uint8   temp[2];
    uint16  enData = 0; //���ɵ�����

	//ָ������
	cmd[0] = 0x55;
	cmd[1] = 0xAA;

    //��ʼ��BUFF����
    cmd[2] = 0x06;
	cmd[3] = 0xB3;

    //��ȡ��λ����Կ
    RsaData.d = Get_d();

    //�����������룬��8λ�͵�8λ
    temp[0] = (pwd&0xFF00)>>8;
    temp[1] = pwd&0x00FF;

    //���ɵ�8λ����
    enData  = EncrData(temp[1]);
    cmd[4] = enData&0x00FF;
    cmd[5] = (enData&0xFF00)>>8;

    //���ɸ�8λ������
    enData  = EncrData(temp[0]);
    cmd[6] = enData&0x00FF;
    cmd[7] = (enData&0xFF00)>>8;

    //У����
    cmd[8] = (uint8)(cmd[2] + cmd[3] + cmd[4] + cmd[5] + cmd[6] + cmd[7]);

    //����
    WriteBlock(cmd, sizeof(cmd));
}

/**************************���ݷ��ͺ���****************************/
//0:ֱ�ӷ�����һ��
//1:break;
//-5��������һ��
static lpsa_i8	FunBleIdleS (void)
{
	lpsa_u8	*data               = NULL;

	switch (Idle.BLE_IDLE_S)
	{
	//�ж��Ƿ���������Ҫ����
	case WRITE_BLOCK_BEGIN:
		{
			if (BleApi.BleCtrl () == 3)
				return 1;
			if (WrStart == 0)
				return 1;
			else
			{
				goto CASE_WRITE_BLOCK_WAIT;
			}
		}break;
	//����BLE
	case WRITE_BLOCK_WAIT:
	CASE_WRITE_BLOCK_WAIT:
		{
			BleApi.BleWakeUp();
			DelayJmp = WRITE_BLOCK_SEND;
            BleDelay(0);
			DelayMs = 2;
			Idle.BLE_IDLE_S = WRITE_BLOCK_DELAY;
			goto CASE_WRITE_BLOCK_DELAY;
		}
	//��������
	case WRITE_BLOCK_SEND:
		{
			while (1)
			{
				data = QueueAcc(&Q_SendData);
				if (data == NULL)
				{
					Idle.BLE_IDLE_S = WRITE_BLOCK_BEGIN;
                                        BleApi.BleLowPower ();
					WrStart = 0;
                                        Flag.WR_key = 0;
					return 1;
				}
				else
				{
					USART_sendOneByte(*data);
				}
			}
		}break;
	/************************BLE_IDLE_DELAY**************************/
	//״̬����ʱ
	case WRITE_BLOCK_DELAY:
	CASE_WRITE_BLOCK_DELAY:
		{
			if (BleDelay (MS_READ) > DelayMs)
			{
				BleDelay (MS_CLEAN);
				Idle.BLE_IDLE_S = DelayJmp;
			}
		}return 1;
	default :
		{
			BleTools.SEND_BLOCK_INFO = 0;
			Idle.BLE_IDLE_D					 = DATA_SEND_BLOCK_BEGIN;
			Idle.BLE_IDLE_S  				 = WRITE_BLOCK_BEGIN;
			DelayJmp 				 = 0;
			DelayMs 				 = 0;
			Idle.BLE_IDLE_C 					 = COMM_GET_DATA_0;
			return -5;//״̬�����˳�
		}break;
	/***************************BREAK*******************************/
	}
}

/**************************ͨ���л�ȡָ��**************************/
//0:ֱ�ӷ�����һ��
//1:break;
//-5: ֱ�ӷ�����һ��
//-6��ֱ�ӷ�����һ��
static lpsa_i8	FunBleIdleC (void)
{
	lpsa_i8 CommStart           = 0;
	static lpsa_u16 Head        = 0;
	static lpsa_u8  i           = 0;
	lpsa_u8	*data               = NULL;

	switch (Idle.BLE_IDLE_C)
	{
	case COMM_GET_DATA_0://��������ͷ
		{
			if (Flag.WR_key == 1)
			  return 1;

			data = BleApi.ReadChar ();
			if (data == NULL)
			  return 1;
			Head<<= 8;
			Head |= *data;
			if (Head != 0xaa55)
                            return 1;
			else
			  Idle.BLE_IDLE_C = COMM_GET_DATA_1;

		}//��break;
	case COMM_GET_DATA_1://��ȡ���ݳ���
		{
			data = BleApi.ReadChar ();
                        if (data == NULL)
                          return 1;

                        if (*data > MAX_BUFF)
                        {
                          Idle.BLE_IDLE_C = COMM_GET_DATA_0;
                          return 1;
                        }

			Idle.BLE_IDLE_C	= COMM_GET_DATA_2;
			ComData.ComLen 	= *data;
			Head           	= 0;
                        i               = 0;
                        data           	= NULL;

		}
	case COMM_GET_DATA_2:
    CASE_COMM_GET_DATA_2:
		{
			data = BleApi.ReadChar ();
			if (data == NULL)
			  return 1;
                        if (data != NULL)
                          ComData.Combuff[i ++] = *data;

                        //�������������⣬���������˳򱾴���������
                        if (i == MAX_BUFF - 1)
                        {
                          i = 0;
                          Idle.BLE_IDLE_C	= COMM_GET_DATA_0;
                          return 1;
                        }

                        if (i < ComData.ComLen)
                          goto CASE_COMM_GET_DATA_2;
			/*****************����У��*********************/



			/**********************************************/
			if (i == ComData.ComLen)
			{
				i = 0;
				ComData.Com = ComData.Combuff[0];
				ComData.ComLen -= 1;
				if (ComData.Com == 0xb0)//��������ָ��
				{
					//��������4λ���룻
					BleTools.PWD = getIDcode();
					pwd = BleTools.PWD;

					Flag.Pwd		  = 2;

					#ifdef TIME_OUT
						Flag.PwdRnewTime  = 1;
						Flag.PwdTime 	  = 0;
					#endif

					//������Կ
					Ble_respondAck_B0(BleTools.PWD);
				}
				switch (Flag.Pwd)
				{
					case 0: Idle.BLE_IDLE_C = COMM_GET_PWD; break;
					case 1: Idle.BLE_IDLE_C = COMM_GET_COMMAND; break;
					case 2: Idle.BLE_IDLE_C = COMM_GET_PWD_RNEW; break;
				}
			}
		}break;
	/**************************break*************************/

	/********************COMM_GET_COMMAND********************/
	//��ȡ����
	//��������ComData.Com
	//����	  ComData.ComLen;
	//����	  ComData.Commbuff[];
	case COMM_GET_COMMAND:
		{
                  CommStart = BLE_COMMAND_CTRL (&ComData.Com, &ComData.ComLen, &ComData.Combuff[1]);
                  if (CommStart)
                  {
                      Flag.Pwd	     = 0;
                      Idle.BLE_IDLE_C      = COMM_GET_DATA_0;
                      Idle.BLE_IDLE_S  	 = WRITE_BLOCK_BEGIN;
                      Head    			 = 0;
                      i       			 = 0;
                      data    			 = NULL;
                      return 1;
                  }

                  Idle.BLE_IDLE_C = COMM_GET_DATA_0;
		}break;
	/**************************break************************/

	/**********************COMM_GET_PWD**********************/
	//��ȡ����
	//��������ComData.Com
	//����	  CommData.ComLen;
	//����	  CommData.Commbuff[];
	case COMM_GET_PWD:
		{
			if (ComData.Com == 0xb1)
			{
			  	uint16 currentCode = 0;

			  	//����ʱ����ȡ�Ѵ洢������
			  	BleApi.BleReadPwd();

				//��ȡ�յ�������
				currentCode = (ComData.Combuff[1] >> 4) * 1000 + (ComData.Combuff[1] & 0x0F) * 100 + \
				  			  (ComData.Combuff[2] >> 4) * 10   + (ComData.Combuff[2] & 0x0F);

				if (BleTools.PWD == currentCode)
				{
					Flag.Pwd 		= 1;

					#ifdef	TIME_OUT
						TimeOut.CommTime = 0;
						Flag.PwdRnewTime = 0;
						Flag.PwdTime 	 = 0;
						Flag.CommTime	 = 1;
					#endif

					WriteBlock(COM_PWD_OK,sizeof(COM_PWD_OK));
				}
				else
				{
					WriteBlock(COM_PWD_ER,sizeof(COM_PWD_ER));
					//return -6;
				}
			}
			Idle.BLE_IDLE_C = COMM_GET_DATA_0;
		}break;
	/**************************break*************************/

	/*****************COMM_GET_PWD_RNEW**********************/
	case COMM_GET_PWD_RNEW:
		{
			//��������ʱ������������֤��ȷ���洢�����롣
			if (ComData.Com == 0xb1)
			{
			  	uint16 currentCode = 0;

				//��ȡ�յ�������
				currentCode = (ComData.Combuff[1] >> 4) * 1000 + (ComData.Combuff[1] & 0x0F) * 100 + \
				  			  (ComData.Combuff[2] >> 4) * 10   + (ComData.Combuff[2] & 0x0F);

				if (BleTools.PWD == currentCode)
				{
					Flag.Pwd 		= 1;

					#ifdef	TIME_OUT
						TimeOut.CommTime = 0;
						Flag.PwdRnewTime = 0;
						Flag.PwdTime 	 = 0;
						Flag.CommTime	 = 1;
					#endif

					WriteBlock(COM_PWD_OK,sizeof(COM_PWD_OK));

					/*�������Գɹ������̴洢������*/
					BleApi.BleSavePwd();
                }
				else
				{
					WriteBlock(COM_PWD_ER,sizeof(COM_PWD_ER));
					//return -6;
				}
			}
			Idle.BLE_IDLE_C = COMM_GET_DATA_0;

		}break;
	/**************************break*************************/

	default:
		{
			return -5;//״̬�����˳�
		}
	}
	return 1;
}

lpsa_i8	BLE_NOMAL_COMM (void)//0:ͨ���У�2����ͨ��
{
#ifdef	TIME_OUT
	//�����ȴ���ʱ
	if (Flag.PwdTime)
		if (TimeOut.CommTime > PWD_TIME)
		{
			Flag.PwdTime = 0;
			return -6;
		}

	//ͨ�ų�ʱ��������Ԥ��30s�ڴ��ꣻ
	if (Flag.CommTime)
		if (TimeOut.CommTime > COMM_TIME)
		{
			Flag.CommTime = 0;
			return -4;
		}

	//�������볬ʱ
	if (Flag.PwdRnewTime)
		if (TimeOut.CommTime > PWE_RNEW)
		{
			Flag.PwdRnewTime = 0;
			return -7;
		}

#endif

	/******************ͨ���л�ȡָ��************************/
	switch (FunBleIdleC ())
	{
		case 0:		return 0;
		case -5: 	return -5;
		case -6: 	return -6;
		case 1: 	break;
	}

	/************************���ݷ���*************************/
	//��״̬�������ݷ��ͣ�
	if (BleTools.SEND_BLOCK_INFO != 0)
		return FunBleIdleD ();


	/********************����*********************************/
	switch (FunBleIdleS ())
	{
		case 0:	 return 0;
		case -5: return -5;
		case 1:  break;
	}

#if (REALTIME == ON)
	/********************ʵʱ��������**************************/
	//��������ʱFlag.WR_key����Ϊ0�����ܵ���WriteBlock()������
	//�˴����Ͳ���̫��Ƶ�����������������ã�
	//�˴�������delay��������ʱ������
	{
		static uint32 currentSteps = 0;
		static uint32 lastSteps = 0;

		currentSteps = Calculate_getSteps();

		if(lastSteps != currentSteps)
		{
		  	uint8 cmd[9];

			lastSteps = currentSteps;

			cmd[0] = 0x55;
			cmd[1] = 0xAA;
			cmd[2] = 0x06;
			cmd[3] = 0xA3;
			cmd[4] = (uint8)( currentSteps & 0xFF);
			cmd[5] = (uint8)((currentSteps & 0xFF00) >> 8);
			cmd[6] = (uint8)((currentSteps & 0xFF0000) >> 16);
			cmd[7] = (uint8)((currentSteps & 0xFF000000) >> 24);
			cmd[8] = (uint8)(cmd[2] + cmd[3] + cmd[4] + cmd[5] + cmd[6]);

			if(Flag.WR_key == 0)
			{
				WriteBlock (cmd, sizeof(cmd));
			}
		}
	}
#endif

	return 0;
}
