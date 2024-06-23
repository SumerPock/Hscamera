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
	* Ŀǰ����汾Ϊ4��26�ն���汾ver1.0
	* �ѽ���GitHub�汾�����
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
/************************************** ���� *******************************************/

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


/*GPS���ݽ�������*/
void AppTaskGPS(void *argument);
const osThreadAttr_t ThreadGPS_Attr =
{
	.name = "osRtxGpsThread",
	.attr_bits = osThreadDetached, 
	.priority = osPriorityHigh4,
	.stack_size = 2048,
};
osThreadId_t ThreadIdTaskGPS = NULL;

/*����RK��������*/
void AppTaskReceiveRK(void *argument);
const osThreadAttr_t ThreadResRK_Attr =
{
	.name 			= "osRtxResRKThread",
	.attr_bits 	= osThreadDetached, 
	.priority 	= osPriorityHigh3,
	.stack_size = 2048,
};
osThreadId_t ThreadIdTaskResRK = NULL;

/*RK3588�ػ�����*/
void AppTaskRKClos(void *argument);
const osThreadAttr_t ThreadRKClos_Attr =
{
	.name = "osRtxRKClosThread",
	.attr_bits = osThreadDetached, 
	.priority = osPriorityHigh5,
	.stack_size = 1024,
};
osThreadId_t ThreadIdTaskRKClos = NULL;

/*RK3588��������*/
void AppTaskRKRes(void *argument);
const osThreadAttr_t ThreadRKRes_Attr =
{
	.name = "osRtxRKResThread",
	.attr_bits = osThreadDetached, 
	.priority = osPriorityHigh5,
	.stack_size = 1024,
};
osThreadId_t ThreadIdTaskRKRes = NULL;

/*ϵͳ�µ�����*/
void AppTaskSysPowOff(void *argument);
const osThreadAttr_t ThreadSysPowOff_Attr =
{
	.name = "osRtxSysPowOffThread",
	.attr_bits = osThreadDetached, 
	.priority = osPriorityHigh5,
	.stack_size = 1024,
};
osThreadId_t ThreadIdTaskSysPowOff = NULL;

/*��������*/
void AppTaskSysErrorCode(void *argument);
const osThreadAttr_t ThreadSysErrorCode_Attr =
{
	.name = "osRtxSysErrorCodeThread",
	.attr_bits = osThreadDetached, 
	.priority = osPriorityHigh6,
	.stack_size = 2048,
};
osThreadId_t ThreadIdTaskSysErrorCode = NULL;


/************************************** ��ʱ�� *******************************************/
void timer_Periodic_App(void *argument);
osTimerId_t  timerID_Periodic = NULL;
const osTimerAttr_t timer_Periodic_Attr = 
{ 
	.name = "periodic timer", 
};

void timer_rk3588reboot_App(void *argument);  		/* �����ʱ�� */
osTimerId_t  timerID_rk3588reboot = NULL; 				/* ���涨ʱ��ID */
const osTimerAttr_t timer_rk3588reboot_Attr = 		/*�����ʱ�����ڱ������ʱ��*/
{
	.name = "rk3588reboottimer",
};


/************************************** ��Ϣ���� *******************************************/
/*���ڽ���GPS����*/
osMessageQueueId_t msgQueue_GPS_Data; 	
const osMessageQueueAttr_t GPS_Data_Attr =
{
	.name = "GPS_Data",   					
};

/*���� adcֵ*/
osMessageQueueId_t msgQueue_ADC_Data; 	
const osMessageQueueAttr_t ADC_Data_Attr =
{
	.name = "ADC_Data",   					
};

/*���ڽ���RK����*/
osMessageQueueId_t msgQueue_RkrRes_Data; 	
const osMessageQueueAttr_t Rkres_Data_Attr =
{
	.name = "Rkres_Data",   					
};

/************************************** �¼���־λ **********************************************/
const osEventFlagsAttr_t event_gps_Attr = 	/* �¼���־������ */
{
	.name = "event_gps",   										/* �¼���־������� */
																						/* ����Ĭ�� */
};
osEventFlagsId_t event_gps_ID = NULL;				/*���߲���¼���־��ID */

/**	�ֶ�ģʽ	**/
const osEventFlagsAttr_t event_ManualMode_Attr = 	
{
	.name = "event_ManualMode",   									
																						
};
osEventFlagsId_t event_ManualMode_ID = NULL;		


/**	����״̬	�ұ������������ʱ����ȥ��������ģ����Ƿ����ڶ�ʱ����ȥ�������RTOS�������̽�������������ȥ
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

	SEGGER_RTT_ConfigUpBuffer(0, "RTTUP", NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_SKIP);			/* ����ͨ��0����������*/
	SEGGER_RTT_ConfigDownBuffer(0, "RTTDOWN", NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_SKIP);	/* ����ͨ��0����������*/	
	SEGGER_RTT_SetTerminal(1);
	SEGGER_RTT_printf(0, "SEGGER_RTT_GetKey \r\n");	

	bsp_InitLEDGpio();	/*����LED �� �ⲿ LED*/
	Read_Gpio();	/*RK��Դ�Ŷ�ȡ*/
	SedGPGIO();
	MX_I2C2_Init();	/*�����¶ȴ������������¶ȴ�����*/
  MX_TIM3_Init();	/*PWM����PWM���*/
  /* USER CODE BEGIN 2 */
	HAL_TIM_IC_Start_IT(&htim3 , TIM_CHANNEL_1);
	HAL_TIM_IC_Start_IT(&htim3 , TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3 , TIM_CHANNEL_3);

	MX_USART1_UART_Init();	/****		GPS/��λ��		****/
	/**UART DMA config**/	
	__HAL_UART_ENABLE_IT(&husart1, UART_IT_IDLE);	/* �������ڿ����ж� */
	#define UART_RX_MODE_DMA											/* �������ڽ��� ��С��������󳤶�rxSize */	
	#ifdef UART_RX_MODE_DMA
		HAL_UART_Receive_DMA(&husart1, g_tBspUsart1.pRxBuf, g_tBspUsart1.rxSize);
	#else
		HAL_UART_Receive_IT(&husart1, g_tBspUsart1.pRxBuf, g_tBspUsart1.rxSize);
	#endif

	MX_USART6_UART_Init();	/****		USART6 - RK		****/
	/**UART DMA config**/	
	__HAL_UART_ENABLE_IT(&husart6, UART_IT_IDLE);	/* �������ڿ����ж� */
	#define UART_RX_MODE_DMA											/* �������ڽ��� ��С��������󳤶�rxSize */	
	#ifdef UART_RX_MODE_DMA
		HAL_UART_Receive_DMA(&husart6, g_tBspUsart6.pRxBuf, g_tBspUsart6.rxSize);
	#else
		HAL_UART_Receive_IT(&husart6, g_tBspUsart6.pRxBuf, g_tBspUsart6.rxSize);
	#endif

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13 , GPIO_PIN_SET);	/*RK�ϵ�*/
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);	/*�����л�*/
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);	
	
	osDelay(500);
	ThreadIdTaskLED 			= osThreadNew(AppTaskLED, NULL, &ThreadLED_Attr);										/*	�󲿷ֵĲɼ�����*/
	/*GSP��Ϣ����*/
	msgQueue_GPS_Data 		= osMessageQueueNew(16 , sizeof(GPS_T) 		, &GPS_Data_Attr); 		//����� sizeof(GPS_T) �޸���
	msgQueue_ADC_Data 		= osMessageQueueNew(16 , sizeof(Adcpower) , &ADC_Data_Attr);
	msgQueue_RkrRes_Data 	= osMessageQueueNew(16 , sizeof(g_tBspUsart6) , &Rkres_Data_Attr);	//22
	/*�����¼���־��*/
	event_gps_ID 				= osEventFlagsNew(&event_gps_Attr);
	event_ManualMode_ID = osEventFlagsNew(&event_ManualMode_Attr);
	event_ErrorCode_ID  = osEventFlagsNew(&event_ErrorCode_Attr);	
	
	ThreadIdTaskSysPowOff 			= osThreadNew(AppTaskSysPowOff, NULL, &ThreadSysPowOff_Attr);				/*	ϵͳ�µ�����	*/	
	ThreadIdTaskGPS 						= osThreadNew(AppTaskGPS, NULL, &ThreadGPS_Attr);										/* 	GPS���� */
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
