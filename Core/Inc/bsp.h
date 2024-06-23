#ifndef __BSP_H
#define __BSP_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "includes.h"
#include "stm32f4xx_hal.h"

#include "bsp_dwt.h"
#include "bsp_i2c.h"
#include "bsp_gpio.h"
#include "bsp_dma.h"
#include "bsp_adc.h"
#include "bsp_tim_pwm.h"
#include "bsp_timcapture.h"
#include "bsp_uart_dma.h"
#include "bsp_gps.h"

#include "AppTask_coll.h"
#include "AppTasK_rk.h"

#include "EventRecorder.h"
#include "EventRecorderConf.h"
#include "stm32f4xx_it.h"
#include "SEGGER_RTT.h"

#define DEBUG_MODE 1


// PID���Ʋ���
#define KP 0.075
//#define KI 0.01
#define KI 0.001
#define KD 0.0

// ����ת�ٷ�Χ
#define FAN_MIN_SPEED 0
#define FAN_MAX_SPEED 9999

// �������¶ȷ�Χ
#define TEMP_MIN 0
#define TEMP_MAX 999

extern const osEventFlagsAttr_t event_gps_Attr;
extern osEventFlagsId_t event_gps_ID;

extern osMessageQueueId_t msgQueue_GPS_Data; 	
extern const osMessageQueueAttr_t GPS_Data_Attr;

extern osMessageQueueId_t msgQueue_RkrRes_Data; 	
extern const osMessageQueueAttr_t Rkres_Data_Attr;

typedef struct{
	uint32_t Battery_power;
	uint32_t External_power;
}Adcpower;


typedef struct
{
	unsigned char *pRxBuf;
	unsigned char rxFlag;
	unsigned char rxLen;
	const unsigned char rxSize;	/*const �����ڽṹ���������ʱ��ֵ */
}BspUart_t;

extern BspUart_t g_tBspUsart1;
extern BspUart_t g_tBspUart4;
extern BspUart_t g_tBspUart5;
extern BspUart_t g_tBspUsart2;
extern BspUart_t g_tBspUsart3;
extern BspUart_t g_tBspUsart6;

// Unpack
typedef struct
{
	unsigned char Dimensions_degree[2];
	unsigned char Dimensions_minute[4];
	unsigned char Dimensions_earth;
	unsigned char longitude_degree[2];
	unsigned char longitude_minute[4];
	unsigned char longitude_earth;	
	
	unsigned char yeart[2];
	unsigned char moon;
	unsigned char day;
	unsigned char hour;
	unsigned char point;
	unsigned char second;
}GPS_DECODE;

//�忨����
typedef struct
{
	unsigned char temp_builtin[2];				//�����¶ȴ�����	
	unsigned char temp_external[4];				//�����¶ȴ�����
	unsigned char humidity_external[4];		//����ʪ�ȴ�����
	unsigned char adc_passage1[4];				//һͨ��ADCֵ
	unsigned char adc_passage2[4];				//��ͨ��ADCֵ
	unsigned char fan_feedback_cycle[2];	//���ȷ����ź�����
	unsigned char fan_feedback_duty[2];		//���ȷ����ź�ռ�ձ�
	
	unsigned char refrigeration[2];				//����Ƭ����ռ�ձ�
	unsigned char rk3588_io;							//RK3588 IO״̬
	unsigned char powerstatic;						//��ǰ��Դ״̬
	unsigned char errorstatic;						//����״̬
}BOARD_DECODE;

//RK����������
typedef struct{
	unsigned char rkruntimer[4];		//RK�忨����ʱ��
	unsigned char rkgetgpsnum[4];		//RK�ۼ��յ�GPS�İ����ݸ���
	unsigned char rkgetbacknum[4]; 	//RK�ۼ��յ��忨���ݰ��ĸ���
	unsigned char rksetipconfig[4];	//RK�ۼ��յ���IP��ַ
}RKHEART;

unsigned int crc16_check(unsigned char* data, unsigned int length);


extern int pid_control(int current_temp, int previous_speed);
extern I2C_HandleTypeDef hi2c2;
extern int crc8_compute(unsigned char* check_data, unsigned char num_of_data);
extern int sht30_crc8_check(unsigned char* p, unsigned char num_of_data, unsigned char CrcData);


/****	����ϵͳRTX5	****/

extern void AppTaskStart(void *argument);
extern const osThreadAttr_t ThreadStart_Attr;
extern osThreadId_t ThreadIdStart;	


extern void AppTaskLED(void *argument);
extern const osThreadAttr_t ThreadLED_Attr;
extern osThreadId_t ThreadIdTaskLED;


/*GPS���ݽ�������*/
extern void AppTaskGPS(void *argument);
extern const osThreadAttr_t ThreadGPS_Attr;
extern osThreadId_t ThreadIdTaskGPS;

/*����RK��������*/
extern void AppTaskReceiveRK(void *argument);
extern const osThreadAttr_t ThreadResRK_Attr;
extern osThreadId_t ThreadIdTaskResRK;

/*RK3588�ػ�����*/
extern void AppTaskRKClos(void *argument);
extern const osThreadAttr_t ThreadRKClos_Attr;
extern osThreadId_t ThreadIdTaskRKClos;

/*RK3588��������*/
extern void AppTaskRKRes(void *argument);
extern const osThreadAttr_t ThreadRKRes_Attr;
extern osThreadId_t ThreadIdTaskRKRes;

/*ϵͳ�µ�����*/
extern void AppTaskSysPowOff(void *argument);
extern const osThreadAttr_t ThreadSysPowOff_Attr;
extern osThreadId_t ThreadIdTaskSysPowOff;

/*��������*/
extern void AppTaskSysErrorCode(void *argument);
extern const osThreadAttr_t ThreadSysErrorCode_Attr;
extern osThreadId_t ThreadIdTaskSysErrorCode;


/*���ڽ���GPS����*/
extern osMessageQueueId_t msgQueue_GPS_Data; 	
extern const osMessageQueueAttr_t GPS_Data_Attr;

/*���� adcֵ*/
extern osMessageQueueId_t msgQueue_ADC_Data; 	
extern const osMessageQueueAttr_t ADC_Data_Attr;

/*���ڽ���RK����*/
extern osMessageQueueId_t msgQueue_RkrRes_Data; 	
extern const osMessageQueueAttr_t Rkres_Data_Attr;




extern const osEventFlagsAttr_t event_ErrorCode_Attr;
extern osEventFlagsId_t event_ErrorCode_ID;

extern const osEventFlagsAttr_t event_ManualMode_Attr;
extern osEventFlagsId_t event_ManualMode_ID;	

extern const osEventFlagsAttr_t event_gps_Attr;
extern osEventFlagsId_t event_gps_ID;

#endif



