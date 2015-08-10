 /*********************************************************/
 /*modified by liubingyan*/
 /*********************************************************/
 #include "string.h"
 #include "EEPROM.h"
 #include "Upload.h"

 //��ʱ����
 struct
 {
   	/*7�����ݶΣ�
   	[startAddress_7,endAddress_7 - 1]
   	����
   	[startAddress_7,ADDRESS_END_7] & [ADDRESS_START_7,endAddress_7 - 1]*/

   	//7��������ʼ��ַ
 	uint16 startAddress_7;

 	//7�����ݽ�����ַ
 	uint16 endAddress_7;

 	/*23�����ݶΣ�
   	[startAddress_23,endAddress_23 - 1]
   	����
   	[startAddress_23,ADDRESS_END_23] & [ADDRESS_START_23,endAddress_23 - 1]*/

 	//23��������ʼ��ַ
 	uint16 startAddress_23;

 	//23�����ݽ�����ַ
 	uint16 endAddress_23;

 	//���ݶ�ȡָ��
 	uint16 readIndex7;
 	uint16 readIndex23;

 	//���ݻ�����
  	uint8 sendBuffer[FRAME_LENGTH_7];

 	//���ݰ�
 	Upload_PocketDef pocket;

 }Upload_DataDef;

 // �������ܣ���ʼ�������ϴ�ģ��
 // ������������
 // ������������
 // �� �� ֵ����
 // ��    ע����
 void Upload_init(void)
 {
 	memset(&Upload_DataDef,0,sizeof(Upload_DataDef));
 }

 // �������ܣ��ϴ�ǰ��ʼ��
 // ������������
 // ������������
 // �� �� ֵ����
 // ��    ע����
 void Upload_getReadyForUpload(void)
 {
   	//��ʼ����ʱ����
 	memset(&Upload_DataDef,0,sizeof(Upload_DataDef));

 	//��ȡ�洢�ռ���ͷָ����Ϊָ��
 	Upload_DataDef.startAddress_7  = DataSave_getStartAddress7();
 	Upload_DataDef.startAddress_23 = DataSave_getStartAddress23();
 	Upload_DataDef.endAddress_7    = DataSave_getEndAddress7();
 	Upload_DataDef.endAddress_23   = DataSave_getEndAddress23();

 	//����ָ��ָ��ͷָ��
 	Upload_DataDef.readIndex7  = Upload_DataDef.startAddress_7;
 	Upload_DataDef.readIndex23 = Upload_DataDef.startAddress_23;
 }

 // �������ܣ���ȡ7������ͷָ��
 // ������������
 // ������������
 // �� �� ֵ����
 // ��    ע����
 uint16 Upload_getHeadAddress(void)
 {
 	return Upload_DataDef.startAddress_7;
 }

 // �������ܣ���ȡ7������βָ��
 // ������������
 // ������������
 // �� �� ֵ����
 // ��    ע����
 uint16 Upload_getTailAddress(void)
 {
 	return Upload_DataDef.endAddress_7;
 }

 // �������ܣ��ϴ�����ʼ��
 // ������������
 // ������������
 // �� �� ֵ����
 // ��    ע����
 void Upload_initAfterUpload(void)
 {
   	DataSave_setHeadAddress(Upload_DataDef.endAddress_7);

 	memset(&Upload_DataDef,0,sizeof(Upload_DataDef));
 }

 // �������ܣ���ȡ7������
 // ������������
 // ������������
 // �� �� ֵ��Upload_PocketDef
 // ��    ע����
 Upload_PocketDef* Upload_readDataOf7Days(void)
 {
 	//�洢�ǿ�
 	if(Upload_DataDef.readIndex7 != Upload_DataDef.endAddress_7)
 	{
 	  	//�����洢��
 	  	Upload_DataDef.pocket.buffer = Upload_DataDef.sendBuffer;

 		//��ǰ��ָ����βָ��֮ǰ
 		if(Upload_DataDef.endAddress_7 > Upload_DataDef.readIndex7)
 		{
 		  	//ʣ�������㹻һ֡
 			if(Upload_DataDef.endAddress_7 - Upload_DataDef.readIndex7 >= FRAME_LENGTH_7)
 			{
 				Upload_DataDef.pocket.length = FRAME_LENGTH_7;
 			}
 			//ʣ�����ݲ���һ֡
 			else
 			{
 			  	//�������ݳ��ȣ��Ǳ�׼����
 				Upload_DataDef.pocket.length = Upload_DataDef.endAddress_7 - Upload_DataDef.readIndex7;
 			}

 			//�����ݴ�EEPROM���򵽴洢����
 			EEPROM_read(Upload_DataDef.readIndex7 + ADDRESS_START_7,Upload_DataDef.pocket.buffer,Upload_DataDef.pocket.length);

 			//���¶�ָ��
 			Upload_DataDef.readIndex7 += Upload_DataDef.pocket.length;
 		}
 		//��ǰ��ָ����βָ��֮��
 		else
 		{
 		  	//��ǰ��ָ���ʹ洢��ĩ��֮���������㹻һ֡
 			if(SPACE_7 - Upload_DataDef.readIndex7 >= FRAME_LENGTH_7)
 			{
 			  	//�������ݳ��ȣ���׼����
 				Upload_DataDef.pocket.length = FRAME_LENGTH_7;

 				//�����ݴ�EEPROM���򵽴洢����
 				EEPROM_read(Upload_DataDef.readIndex7 + ADDRESS_START_7,Upload_DataDef.pocket.buffer,Upload_DataDef.pocket.length);

 				//���¶�ָ��
 				Upload_DataDef.readIndex7 += Upload_DataDef.pocket.length;
 			}
 			//��ǰ��ָ���ʹ洢��ĩ��֮�������ݲ���һ֡
 			else
 			{
 			  	//ʣ�������㹻һ֡
 			  	if(SPACE_7 - Upload_DataDef.readIndex7 + Upload_DataDef.endAddress_7 >= FRAME_LENGTH_7)
 				{
 				  	//�������ݳ��ȣ���׼����
 					Upload_DataDef.pocket.length = FRAME_LENGTH_7;

 					//�����ݴ�EEPROM���򵽴洢����
 					EEPROM_read(Upload_DataDef.readIndex7 + ADDRESS_START_7,Upload_DataDef.pocket.buffer,SPACE_7 - Upload_DataDef.readIndex7);
 					EEPROM_read(ADDRESS_START_7,(Upload_DataDef.pocket.buffer + (SPACE_7 - Upload_DataDef.readIndex7)),FRAME_LENGTH_7 - (SPACE_7 - Upload_DataDef.readIndex7));

 					//���¶�ָ��
 					Upload_DataDef.readIndex7 = SPACE_7 - Upload_DataDef.readIndex7;
 				}
 				//ʣ�����ݲ���һ֡
 				else
 				{
 				  	//�������ݳ��ȣ��Ǳ�׼����
 					Upload_DataDef.pocket.length = SPACE_7 - Upload_DataDef.readIndex7 + Upload_DataDef.endAddress_7;

 					//�����ݴ�EEPROM���򵽴洢����
 					EEPROM_read(Upload_DataDef.readIndex7 + ADDRESS_START_7,Upload_DataDef.pocket.buffer,SPACE_7 - Upload_DataDef.readIndex7);
 					EEPROM_read(ADDRESS_START_7,(Upload_DataDef.pocket.buffer + (SPACE_7 - Upload_DataDef.readIndex7)),Upload_DataDef.endAddress_7);

 					//���¶�ָ��
 					Upload_DataDef.readIndex7 = Upload_DataDef.endAddress_7;
 				}
 			}
 		}

 		return &Upload_DataDef.pocket;
 	}
 	//�洢��
 	else
 	{
 	  	//��ָ������ָ����ʼ��ַ
 	  	Upload_DataDef.readIndex7 = Upload_DataDef.startAddress_7;

 		//���ؿ�ָ��
 	  	return (Upload_PocketDef*)NULL;
 	}
 }

 // �������ܣ���ȡ23������
 // ������������
 // ������������
 // �� �� ֵ��Upload_PocketDef
 // ��    ע����
 Upload_PocketDef* Upload_readDataOf23Days(void)
 {
   	//�洢�ǿ�
 	if(Upload_DataDef.readIndex23 != Upload_DataDef.endAddress_23)
 	{
 	  	//��ǰ��ָ����βָ��֮ǰ
 		if(Upload_DataDef.endAddress_23 > Upload_DataDef.readIndex23)
 		{
 		  	//ʣ�������㹻һ֡
 			if(Upload_DataDef.endAddress_23 - Upload_DataDef.readIndex23 >= FRAME_LENGTH_23)
 			{
 				Upload_DataDef.pocket.length = FRAME_LENGTH_23;
 			}
 			//ʣ�����ݲ���һ֡
 			else
 			{
 				Upload_DataDef.pocket.length = Upload_DataDef.endAddress_23 - Upload_DataDef.readIndex23;
 			}

 			//�����ݴ�EEPROM���򵽴洢����
 			EEPROM_read(Upload_DataDef.readIndex23 + ADDRESS_START_23,Upload_DataDef.pocket.buffer,Upload_DataDef.pocket.length);

 			//���¶�ָ��
 			Upload_DataDef.readIndex23 += Upload_DataDef.pocket.length;
 		}
 		//��ǰ��ָ����βָ��֮��
 		else
 		{
 		  	//��ǰ��ָ���ʹ洢��ĩ��֮���������㹻һ֡
 			if(SPACE_23 - Upload_DataDef.readIndex23 >= FRAME_LENGTH_23)
 			{
 			  	//�������ݳ��ȣ���׼����
 				Upload_DataDef.pocket.length = FRAME_LENGTH_23;

 				//�����ݴ�EEPROM���򵽴洢����
 				EEPROM_read(Upload_DataDef.readIndex23 + ADDRESS_START_23,Upload_DataDef.pocket.buffer,Upload_DataDef.pocket.length);

 				//���¶�ָ��
 				Upload_DataDef.readIndex23 += Upload_DataDef.pocket.length;
 			}
 			//��ǰ��ָ���ʹ洢��ĩ��֮�������ݲ���һ֡
 			else
 			{
 			  	//ʣ�������㹻һ֡
 			  	if(SPACE_23 - Upload_DataDef.readIndex23 + Upload_DataDef.endAddress_23 >= FRAME_LENGTH_23)
 				{
 				  	//�������ݳ��ȣ���׼����
 					Upload_DataDef.pocket.length = FRAME_LENGTH_23;

 					//�����ݴ�EEPROM���򵽴洢����
 					EEPROM_read(Upload_DataDef.readIndex23 + ADDRESS_START_23,Upload_DataDef.pocket.buffer,SPACE_23 - Upload_DataDef.readIndex23);
 					EEPROM_read(ADDRESS_START_23,(Upload_DataDef.pocket.buffer + (SPACE_23 - Upload_DataDef.readIndex23)),FRAME_LENGTH_23 - (SPACE_23 - Upload_DataDef.readIndex23));

 					//���¶�ָ��
 					Upload_DataDef.readIndex23 = SPACE_23 - Upload_DataDef.readIndex23;
 				}
 				//ʣ�����ݲ���һ֡
 				else
 				{
 				  	//�������ݳ��ȣ��Ǳ�׼����
 					Upload_DataDef.pocket.length = SPACE_23 - Upload_DataDef.readIndex23 + Upload_DataDef.endAddress_23;

 					//�����ݴ�EEPROM���򵽴洢����
 					EEPROM_read(Upload_DataDef.readIndex23 + ADDRESS_START_23,Upload_DataDef.pocket.buffer,SPACE_23 - Upload_DataDef.readIndex23);
 					EEPROM_read(ADDRESS_START_23,(Upload_DataDef.pocket.buffer + (SPACE_23 - Upload_DataDef.readIndex23)),Upload_DataDef.endAddress_23);

 					//���¶�ָ��
 					Upload_DataDef.readIndex23 = Upload_DataDef.endAddress_23;
 				}
 			}
 		}

 		return &Upload_DataDef.pocket;;
 	}
 	else
 	{
 	  	//��ָ������ָ����ʼ��ַ
 	  	Upload_DataDef.readIndex23  = Upload_DataDef.startAddress_23;

 		//���ؿ�ָ��
 	  	return (Upload_PocketDef*)NULL;
 	}
 }
