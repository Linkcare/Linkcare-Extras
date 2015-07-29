#include "Event.h"
#include "Task.h"
#include "stm32l1xx.h"

//ϵͳ��ǰ�¼�
volatile static uint8 system_event = NO_EVENT;

//ϵͳ��ǰ����ģʽ
volatile static uint8 current_task;

//ϵͳ�ϴι���ģʽ
volatile static uint8 former_task;

// �������ܣ���ȡϵͳ�¼�
// �����������
// �����������
// �� �� ֵ���¼�
// ��    ע����
uint8 Event_getEvent(void)
{
    __disable_interrupt();
    
    uint16 tempEvent = system_event;
	
    system_event = NO_EVENT;
    
    __enable_interrupt();
    
    return tempEvent;
}

// �������ܣ�ע��ϵͳ�¼�
// �����������ע���¼�
// �����������
// �� �� ֵ���¼�
// ��    ע����
void Event_register(const uint8 event)
{
  	if(system_event == NO_EVENT)
	{
    	system_event = event;
	}
}

// �������ܣ����ϵͳ�¼�
// �����������
// �����������
// �� �� ֵ����
// ��    ע����
void Event_clearEvent(void)
{
    system_event = NO_EVENT;
}

// �������ܣ�����ϵͳ����ģʽ
// ����������������¼�
// �����������
// �� �� ֵ����
// ��    ע����
void Event_registerMode(const uint8 mode)
{
    __disable_interrupt();
	
    if(mode == TASK_USB || current_task == mode)
    {
    	__enable_interrupt();

        return;
    }

    former_task = current_task;
    current_task = mode;

    __enable_interrupt();

    return;
}

// �������ܣ���ȡϵͳ��ǰ����ģʽ
// �����������
// �����������
// �� �� ֵ����
// ��    ע����
uint8 Event_getCurrentMode(void)
{
    return current_task;
}

// �������ܣ���ȡϵͳ��һ�ι���ģʽ
// �����������
// �����������
// �� �� ֵ����
// ��    ע����
uint8 Event_getLastMode(void)
{
    return former_task;
}

