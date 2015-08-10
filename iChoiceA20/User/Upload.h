/*********************************************************/
/*modified by liubingyan*/
/*********************************************************/
#ifndef __UPLOAD_H
#define __UPLOAD_H

#include "type.h"
#include "DataSave.h"

#define FRAME_LENGTH_7     54 		//ÿһ֡�ı�׼�����ֽ���(7�����ݴ���)
#define FRAME_LENGTH_23    54 		//ÿһ֡�ı�׼�����ֽ���(23�����ݴ���)

typedef struct
{
	uint8 length;
	uint8 *buffer;
	
}Upload_PocketDef;

uint16 Upload_getHeadAddress(void);
uint16 Upload_getTailAddress(void);
Upload_PocketDef* Upload_readDataOf7Days(void);
Upload_PocketDef* Upload_readDataOf23Days(void);
void Upload_init(void);
void Upload_initAfterUpload(void);
void Upload_getReadyForUpload(void);

#endif
