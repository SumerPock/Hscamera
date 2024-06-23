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


// PID控制参数
#define KP 0.075
//#define KI 0.01
#define KI 0.001
#define KD 0.0

// 风扇转速范围
#define FAN_MIN_SPEED 0
#define FAN_MAX_SPEED 9999

// 传感器温度范围
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
	const unsigned char rxSize;	/*const 必须在结构体变量定义时赋值 */
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

//板卡解码
typedef struct
{
	unsigned char temp_builtin[2];				//内置温度传感器	
	unsigned char temp_external[4];				//外置温度传感器
	unsigned char humidity_external[4];		//外置湿度传感器
	unsigned char adc_passage1[4];				//一通道ADC值
	unsigned char adc_passage2[4];				//二通道ADC值
	unsigned char fan_feedback_cycle[2];	//风扇反馈信号周期
	unsigned char fan_feedback_duty[2];		//风扇反馈信号占空比
	
	unsigned char refrigeration[2];				//制冷片控制占空比
	unsigned char rk3588_io;							//RK3588 IO状态
	unsigned char powerstatic;						//当前电源状态
	unsigned char errorstatic;						//错误状态
}BOARD_DECODE;

//RK心跳包数据
typedef struct{
	unsigned char rkruntimer[4];		//RK板卡运行时间
	unsigned char rkgetgpsnum[4];		//RK累计收到GPS的包数据个数
	unsigned char rkgetbacknum[4]; 	//RK累计收到板卡数据包的个数
	unsigned char rksetipconfig[4];	//RK累计收到的IP地址
}RKHEART;

unsigned int crc16_check(unsigned char* data, unsigned int length);


extern int pid_control(int current_temp, int previous_speed);
extern I2C_HandleTypeDef hi2c2;
extern int crc8_compute(unsigned char* check_data, unsigned char num_of_data);
extern int sht30_crc8_check(unsigned char* p, unsigned char num_of_data, unsigned char CrcData);


/****	操作系统RTX5	****/

extern void AppTaskStart(void *argument);
extern const osThreadAttr_t ThreadStart_Attr;
extern osThreadId_t ThreadIdStart;	


extern void AppTaskLED(void *argument);
extern const osThreadAttr_t ThreadLED_Attr;
extern osThreadId_t ThreadIdTaskLED;


/*GPS数据解析任务*/
extern void AppTaskGPS(void *argument);
extern const osThreadAttr_t ThreadGPS_Attr;
extern osThreadId_t ThreadIdTaskGPS;

/*接收RK数据任务*/
extern void AppTaskReceiveRK(void *argument);
extern const osThreadAttr_t ThreadResRK_Attr;
extern osThreadId_t ThreadIdTaskResRK;

/*RK3588关机任务*/
extern void AppTaskRKClos(void *argument);
extern const osThreadAttr_t ThreadRKClos_Attr;
extern osThreadId_t ThreadIdTaskRKClos;

/*RK3588重启任务*/
extern void AppTaskRKRes(void *argument);
extern const osThreadAttr_t ThreadRKRes_Attr;
extern osThreadId_t ThreadIdTaskRKRes;

/*系统下电任务*/
extern void AppTaskSysPowOff(void *argument);
extern const osThreadAttr_t ThreadSysPowOff_Attr;
extern osThreadId_t ThreadIdTaskSysPowOff;

/*错误流程*/
extern void AppTaskSysErrorCode(void *argument);
extern const osThreadAttr_t ThreadSysErrorCode_Attr;
extern osThreadId_t ThreadIdTaskSysErrorCode;


/*用于接收GPS数据*/
extern osMessageQueueId_t msgQueue_GPS_Data; 	
extern const osMessageQueueAttr_t GPS_Data_Attr;

/*用于 adc值*/
extern osMessageQueueId_t msgQueue_ADC_Data; 	
extern const osMessageQueueAttr_t ADC_Data_Attr;

/*用于接收RK数据*/
extern osMessageQueueId_t msgQueue_RkrRes_Data; 	
extern const osMessageQueueAttr_t Rkres_Data_Attr;




extern const osEventFlagsAttr_t event_ErrorCode_Attr;
extern osEventFlagsId_t event_ErrorCode_ID;

extern const osEventFlagsAttr_t event_ManualMode_Attr;
extern osEventFlagsId_t event_ManualMode_ID;	

extern const osEventFlagsAttr_t event_gps_Attr;
extern osEventFlagsId_t event_gps_ID;

#endif



