/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
	* 目前这个版本为4月26日定稿版本ver1.0
	* 已建立GitHub版本管理库
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bsp.h"

I2C_HandleTypeDef hi2c2;
CRC_HandleTypeDef hcrc;
/************************************** 任务 *******************************************/

void AppTaskStart(void *argument);
const osThreadAttr_t ThreadStart_Attr = 	
{
	.name = "osRtxStartThread",
	.attr_bits = osThreadDetached, 
	.priority = osPriorityHigh7,
	.stack_size = 2048,
};
osThreadId_t ThreadIdStart = NULL;	


void AppTaskLED(void *argument);
const osThreadAttr_t ThreadLED_Attr = 
{
	.name = "osRtxLEDThread",
	.attr_bits = osThreadDetached, 
	.priority = osPriorityHigh7,
	.stack_size = 4096,
};
osThreadId_t ThreadIdTaskLED = NULL;


/*GPS数据解析任务*/
void AppTaskGPS(void *argument);
const osThreadAttr_t ThreadGPS_Attr =
{
	.name = "osRtxGpsThread",
	.attr_bits = osThreadDetached, 
	.priority = osPriorityHigh4,
	.stack_size = 2048,
};
osThreadId_t ThreadIdTaskGPS = NULL;

/*接收RK数据任务*/
void AppTaskReceiveRK(void *argument);
const osThreadAttr_t ThreadResRK_Attr =
{
	.name 			= "osRtxResRKThread",
	.attr_bits 	= osThreadDetached, 
	.priority 	= osPriorityHigh3,
	.stack_size = 2048,
};
osThreadId_t ThreadIdTaskResRK = NULL;

/*RK3588关机任务*/
void AppTaskRKClos(void *argument);
const osThreadAttr_t ThreadRKClos_Attr =
{
	.name = "osRtxRKClosThread",
	.attr_bits = osThreadDetached, 
	.priority = osPriorityHigh5,
	.stack_size = 1024,
};
osThreadId_t ThreadIdTaskRKClos = NULL;

/*RK3588重启任务*/
void AppTaskRKRes(void *argument);
const osThreadAttr_t ThreadRKRes_Attr =
{
	.name = "osRtxRKResThread",
	.attr_bits = osThreadDetached, 
	.priority = osPriorityHigh5,
	.stack_size = 1024,
};
osThreadId_t ThreadIdTaskRKRes = NULL;

/*系统下电任务*/
void AppTaskSysPowOff(void *argument);
const osThreadAttr_t ThreadSysPowOff_Attr =
{
	.name = "osRtxSysPowOffThread",
	.attr_bits = osThreadDetached, 
	.priority = osPriorityHigh5,
	.stack_size = 1024,
};
osThreadId_t ThreadIdTaskSysPowOff = NULL;

/*错误流程*/
void AppTaskSysErrorCode(void *argument);
const osThreadAttr_t ThreadSysErrorCode_Attr =
{
	.name = "osRtxSysErrorCodeThread",
	.attr_bits = osThreadDetached, 
	.priority = osPriorityHigh6,
	.stack_size = 2048,
};
osThreadId_t ThreadIdTaskSysErrorCode = NULL;


/************************************** 定时器 *******************************************/
void timer_Periodic_App(void *argument);
osTimerId_t  timerID_Periodic = NULL;
const osTimerAttr_t timer_Periodic_Attr = 
{ 
	.name = "periodic timer", 
};

void timer_rk3588reboot_App(void *argument);  		/* 软件定时器 */
osTimerId_t  timerID_rk3588reboot = NULL; 				/* 保存定时器ID */
const osTimerAttr_t timer_rk3588reboot_Attr = 		/*软件定时器用于标记重启时间*/
{
	.name = "rk3588reboottimer",
};


/************************************** 消息队列 *******************************************/
/*用于接收GPS数据*/
osMessageQueueId_t msgQueue_GPS_Data; 	
const osMessageQueueAttr_t GPS_Data_Attr =
{
	.name = "GPS_Data",   					
};

/*用于 adc值*/
osMessageQueueId_t msgQueue_ADC_Data; 	
const osMessageQueueAttr_t ADC_Data_Attr =
{
	.name = "ADC_Data",   					
};

/*用于接收RK数据*/
osMessageQueueId_t msgQueue_RkrRes_Data; 	
const osMessageQueueAttr_t Rkres_Data_Attr =
{
	.name = "Rkres_Data",   					
};

/************************************** 事件标志位 **********************************************/
const osEventFlagsAttr_t event_gps_Attr = 	/* 事件标志组属性 */
{
	.name = "event_gps",   										/* 事件标志组的名字 */
																						/* 其他默认 */
};
osEventFlagsId_t event_gps_ID = NULL;				/*网线插拔事件标志组ID */

/**	手动模式	**/
const osEventFlagsAttr_t event_ManualMode_Attr = 	
{
	.name = "event_ManualMode",   									
																						
};
osEventFlagsId_t event_ManualMode_ID = NULL;		


/**	错误状态	我本来想在这个定时器中去做错误处理的，但是发现在定时器中去做会造成RTOS死机，固将其移入任务中去
**/
const osEventFlagsAttr_t event_ErrorCode_Attr = 	
{
	.name = "event_ErrorCode",   									
																						
};
osEventFlagsId_t event_ErrorCode_ID = NULL;





/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
	MX_DMA_Init();	
  /* USER CODE BEGIN 2 */
	EventRecorderInitialize(EventRecordAll, 1U);	
	EventRecorderStart();
	HAL_SuspendTick();
	osKernelInitialize();                                  
	ThreadIdStart = osThreadNew(AppTaskStart, NULL, &ThreadStart_Attr);  
	osKernelStart();		
}


void AppTaskStart(void *argument)
{
	const uint16_t usFrequency = 100;
	uint32_t tick;	
	HAL_ResumeTick();
	tick = osKernelGetTickCount(); 

	EventRecorderInitialize(EventRecordAll, 1U);	
	EventRecorderStart();	

	SEGGER_RTT_ConfigUpBuffer(0, "RTTUP", NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_SKIP);			/* 配置通道0，上行配置*/
	SEGGER_RTT_ConfigDownBuffer(0, "RTTDOWN", NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_SKIP);	/* 配置通道0，下行配置*/	
	SEGGER_RTT_SetTerminal(1);
	SEGGER_RTT_printf(0, "SEGGER_RTT_GetKey \r\n");	

	bsp_InitLEDGpio();	/*板载LED 及 外部 LED*/
	Read_Gpio();	/*RK电源脚读取*/
	SedGPGIO();
	MX_I2C2_Init();	/*外置温度传感器，内置温度传感器*/
  MX_TIM3_Init();	/*PWM捕获及PWM输出*/
  /* USER CODE BEGIN 2 */
	HAL_TIM_IC_Start_IT(&htim3 , TIM_CHANNEL_1);
	HAL_TIM_IC_Start_IT(&htim3 , TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3 , TIM_CHANNEL_3);

	MX_USART1_UART_Init();	/****		GPS/上位机		****/
	/**UART DMA config**/	
	__HAL_UART_ENABLE_IT(&husart1, UART_IT_IDLE);	/* 开启串口空闲中断 */
	#define UART_RX_MODE_DMA											/* 开启串口接收 大小缓冲区最大长度rxSize */	
	#ifdef UART_RX_MODE_DMA
		HAL_UART_Receive_DMA(&husart1, g_tBspUsart1.pRxBuf, g_tBspUsart1.rxSize);
	#else
		HAL_UART_Receive_IT(&husart1, g_tBspUsart1.pRxBuf, g_tBspUsart1.rxSize);
	#endif

	MX_USART6_UART_Init();	/****		USART6 - RK		****/
	/**UART DMA config**/	
	__HAL_UART_ENABLE_IT(&husart6, UART_IT_IDLE);	/* 开启串口空闲中断 */
	#define UART_RX_MODE_DMA											/* 开启串口接收 大小缓冲区最大长度rxSize */	
	#ifdef UART_RX_MODE_DMA
		HAL_UART_Receive_DMA(&husart6, g_tBspUsart6.pRxBuf, g_tBspUsart6.rxSize);
	#else
		HAL_UART_Receive_IT(&husart6, g_tBspUsart6.pRxBuf, g_tBspUsart6.rxSize);
	#endif

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13 , GPIO_PIN_SET);	/*RK上电*/
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);	/*供电切换*/
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);	
	
	osDelay(500);
	ThreadIdTaskLED 			= osThreadNew(AppTaskLED, NULL, &ThreadLED_Attr);										/*	大部分的采集任务*/
	/*GSP消息队列*/
	msgQueue_GPS_Data 		= osMessageQueueNew(16 , sizeof(GPS_T) 		, &GPS_Data_Attr); 		//里面的 sizeof(GPS_T) 修改下
	msgQueue_ADC_Data 		= osMessageQueueNew(16 , sizeof(Adcpower) , &ADC_Data_Attr);
	msgQueue_RkrRes_Data 	= osMessageQueueNew(16 , sizeof(g_tBspUsart6) , &Rkres_Data_Attr);	//22
	/*创建事件标志组*/
	event_gps_ID 				= osEventFlagsNew(&event_gps_Attr);
	event_ManualMode_ID = osEventFlagsNew(&event_ManualMode_Attr);
	event_ErrorCode_ID  = osEventFlagsNew(&event_ErrorCode_Attr);	
	
	ThreadIdTaskSysPowOff 			= osThreadNew(AppTaskSysPowOff, NULL, &ThreadSysPowOff_Attr);				/*	系统下电任务	*/	
	ThreadIdTaskGPS 						= osThreadNew(AppTaskGPS, NULL, &ThreadGPS_Attr);										/* 	GPS任务 */
	//ThreadIdTaskResRK 					= osThreadNew(AppTaskReceiveRK, NULL, &ThreadResRK_Attr);
	ThreadIdTaskSysErrorCode 		= osThreadNew(AppTaskSysErrorCode, NULL, &ThreadSysErrorCode_Attr);
	while(1)
	{		
		tick += usFrequency;                          
		osDelayUntil(tick);
		osThreadExit();
	}
}




/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
