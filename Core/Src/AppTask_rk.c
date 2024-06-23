#include "bsp.h"
/*
*********************************************************************************************************
*	函 数 名: AppTaskRKRes
*	功能说明: RK重启
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   
*********************************************************************************************************
*/
void AppTaskRKRes(void *argument){
	const uint16_t usFrequency = 250;
	unsigned int tick = 0;
	tick = osKernelGetTickCount();
	osStatus_t os_Status;
	int loop = 0;
	while(1)
	{
		switch(loop)
		{
			case 0:
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13 , GPIO_PIN_RESET);
				loop = 1;
				break;
			case 1:
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13 , GPIO_PIN_RESET);
				loop = 2;
				break;
			case 2:
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13 , GPIO_PIN_SET);
				loop = 3;
				break;
			case 3:
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13 , GPIO_PIN_SET);
				loop = 4;
				break;
			case 4:
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13 , GPIO_PIN_SET);
				osThreadExit();
				loop = 0;
				break;
			default:
				break;
		}
		tick += usFrequency;    /* 相对延迟 */                      
		osDelayUntil(tick);	
	}
}


/*
*********************************************************************************************************
*	函 数 名: AppTaskRKClos
*	功能说明: RK关机任务
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   
*********************************************************************************************************
*/
void AppTaskRKClos(void *argument){
	const uint16_t usFrequency = 250;	/* 延迟周期 */
	unsigned int tick = 0;
	tick = osKernelGetTickCount();
	osStatus_t os_Status;
	int loop = 0;
	while(1){
		switch(loop){
			case 0:
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13 , GPIO_PIN_RESET);
				loop = 1;
				break;
			case 1:
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13 , GPIO_PIN_RESET);
				loop = 2;
				break;
			case 2:
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13 , GPIO_PIN_RESET);
				loop = 3;
				break;
			case 3:
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13 , GPIO_PIN_RESET);
				osThreadExit();
				loop = 1;
				break;
			default:
				break;
		}
		tick += usFrequency;                      
		osDelayUntil(tick);
	}
}


/*
*********************************************************************************************************
*	函 数 名: AppTaskSysPowOff
*	功能说明: 系统下电任务
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   
*********************************************************************************************************
*/
void AppTaskSysPowOff(void *argument)
{
	const uint16_t usFrequency = 500;	
	unsigned int tick = 0;
	tick = osKernelGetTickCount();
	osStatus_t os_Status;
	uint32_t Flag = 0;	
	volatile unsigned int keil = 0;
	unsigned char msg_ADC_Number = 0;
	Adcpower myget_adcpow = {0};
	
	int loopdata = 0;								//用于检测外部电源下电的时间
	int Loopdatapoweron = 0;				//用于检测外部电源上电时间
	int RK3588Shut_down = 0; 				//RK关机 标志
	int Secondary_ShutDownloop = 0;	//二次电源检测的时间
	int RKNotShut_downtime = 0;			//RK关机等待时间
	
	int forceoff = 0;			//强制关机
	while(1)
	{
		msg_ADC_Number = osMessageQueueGetCount(msgQueue_ADC_Data);	
		if(msg_ADC_Number > 0)
		{	
			os_Status = osMessageQueueGet(msgQueue_ADC_Data,/* 消息队列的名字 */
															&myget_adcpow,    			/* 存放消息的内存 */
															NULL,            				/* 取出消息的优先级 */
															NULL);  	  						/* 超时时间，在中断函数里调用osMessageQueueGet时，此参数必须是NULL */	
			if(myget_adcpow.External_power < 50)
			{//掉电状态
				SEGGER_RTT_SetTerminal(5);
				SEGGER_RTT_printf(0, "power is low %d\r\n" , loopdata);
				loopdata++;
			}
			else
			{//上电状态
				if(myget_adcpow.External_power > 250)
				{
					Loopdatapoweron++;	//电压足以驱动这部分
					SEGGER_RTT_SetTerminal(5);
					SEGGER_RTT_printf(0, "power is hight %d\r\n" , Loopdatapoweron);
					/**清除标志位**/
					if(Loopdatapoweron >= 4)
					{/**来电2S以上**/
						Loopdatapoweron = 0;
						loopdata = 0;
						osEventFlagsClear(event_ErrorCode_ID , 0x01U << 0);
						osEventFlagsClear(event_ErrorCode_ID , 0x01U << 1);		
						osEventFlagsClear(event_ErrorCode_ID , 0x01U << 2);
						/**RK是否完成关机**/
						if(RK3588Shut_down == 1 && GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3))
						{/**** 救活RK流程 ****/
							ThreadIdTaskRKRes = osThreadNew(AppTaskRKRes, NULL, &ThreadRKRes_Attr);/*	RK重启任务	*/																																				
							SEGGER_RTT_SetTerminal(5);	
							SEGGER_RTT_printf(0, "myget_adcpow.External_power secondary on \r\n");		/****		说明二次来电了		****/
							RK3588Shut_down = 0;
						}
						else{
						 /**** 不需要救活RK ****/
						}
					}

				}
			}	
		}

		
		if(loopdata >= 30)
		{/****	500ms * 20 = 10S，此时即便来电了也必须走一遍断电流程	****/
			if(RK3588Shut_down == 1 && loopdata >= 65)
			{	/**** RK关机完毕 ****/
				SEGGER_RTT_SetTerminal(5);	
				SEGGER_RTT_printf(0, "myget_adcpow.External_power is close \r\n");	
				Secondary_ShutDownloop = 0;
				/****		ST自杀开启 , 进入这一步后无法返回		****/
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);	
			}	
			else{
			/** 将救活的时间 6S **/
			}
			if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3))
			{	/****	RK关机完毕	****/
				RK3588Shut_down = 1;
				/**	2024.4.26号 刘经理让在这里加入关机指令	**/
				ThreadIdTaskRKClos = osThreadNew(AppTaskRKClos, NULL, &ThreadRKClos_Attr);
				/****	异常状态3	****/
				osEventFlagsClear(event_ErrorCode_ID , 0x01U << 0);	//清除异常状态2
				osEventFlagsClear(event_ErrorCode_ID , 0x01U << 2); //清除异常状态4
				osEventFlagsSet(event_ErrorCode_ID , 0x01U << 1);
			}
			else
			{	/****	RK没有关机	****/
				RKNotShut_downtime++;
				if(RKNotShut_downtime < 60)		
				{	/****	等待45秒内	****/									
					SEGGER_RTT_SetTerminal(5);
					SEGGER_RTT_printf(0, "RKNotShut_downtime = %d \r\n" , RKNotShut_downtime);
					/**** 异常状态2	****/	
					osEventFlagsClear(event_ErrorCode_ID , 0x01U << 1);	//清除异常状态3
					osEventFlagsClear(event_ErrorCode_ID , 0x01U << 2); //清除异常状态4
					osEventFlagsSet(event_ErrorCode_ID , 0x01U << 0);
				}
				else
				{	/****	45S超时 -> RK3588死机	****/
					RKNotShut_downtime = 0;					/**45S倒计时FLAG**/				
					SEGGER_RTT_SetTerminal(5);			//强制关机
					SEGGER_RTT_printf(0, "power is force close \r\n");	
					ThreadIdTaskRKClos = osThreadNew(AppTaskRKClos, NULL, &ThreadRKClos_Attr);
				} 
			}
		}
		else
		{/**等待10S检测**/
		}		
		tick += usFrequency;    /* 相对延迟 */                      
		osDelayUntil(tick);		
	}
}

void AppTaskSysErrorCode(void *argument)
{
	uint32_t Flag = 0;
	
	/**	4种LED模式	**/
	/**	异常状态2：ST检测到外电断开，但RK关机未执行结束		**/
	/**	异常状态3：ST检测到RK的3.3V断开，LED灯状态：长灭 	**/
	static int ledloop = 0;

	osStatus_t os_StatusQueue; 
	uint16_t usFrequency = 100; /* 延迟周期 */
	uint32_t tick;
	
	while(1){
		Flag = osEventFlagsGet(event_ErrorCode_ID);
		switch(Flag)
		{
			case 0:
			if(ledloop < 88)
			{
				ledloop++;
				if(ledloop % 4 == 0)
				{
				HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);//正常状态
				}
			}
			else
			{
				ledloop = 0;
			}
			break;

			case 1:/**	异常状态2	**/
			if(ledloop < 26)
			{
				ledloop++;
				if(ledloop >0 && ledloop < 21)
				{
					HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);
				}
				else if(ledloop > 21 && ledloop < 25)
				{
					HAL_GPIO_WritePin(GPIOB , GPIO_PIN_14 , GPIO_PIN_RESET);
				}
			}
			else
			{
				ledloop = 0;
			}			
			break;

			case 2:/**	异常状态3	**/
				HAL_GPIO_WritePin(GPIOB , GPIO_PIN_14 , GPIO_PIN_RESET);	//	关闭
			break;

			case 3:
			break;

			case 4:/**	异常状态4	温度传感器出错**/
				HAL_GPIO_WritePin(GPIOB , GPIO_PIN_14 , GPIO_PIN_SET);	//常量
			break;

			default:
			break;
		}		
		tick += usFrequency;    /* 相对延迟 */                      
		osDelayUntil(tick);		
	}
}

/*
*********************************************************************************************************
*	函 数 名: AppTaskReceiveRK
*	功能说明: ST解包RK的数据
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   
*********************************************************************************************************
*/
void AppTaskReceiveRK(void *argument)
{
	const uint16_t usFrequency = 250;
	unsigned int tick = 0;
	tick = osKernelGetTickCount();
	osStatus_t os_Status;
	int loop = 0;
	unsigned char getbuffer[30] = {0};
	BspUart_t getUart6;
	while(1)
	{
		os_Status = osMessageQueueGet(msgQueue_RkrRes_Data,	/* 消息队列的名字 */
																	&getUart6,    		  	/* 存放消息的内存 */
																	NULL,              	  /* 取出消息的优先级 */
																	osWaitForever  	     	/* 超时时间，在中断函数里调用osMessageQueueGet时，此参数必须是NULL */
																	);
		if(osOK == os_Status)
		{
			if(getUart6.rxLen >= 16)
			{
				if(getUart6.pRxBuf[0] == 0xee && getUart6.pRxBuf[1] == 0xaa)
				{
					if(getUart6.rxLen == getUart6.pRxBuf[2])
					{
						if(getUart6.pRxBuf[3] == 0x06)
						{
							/*信号量自加*/
							SEGGER_RTT_SetTerminal(6);
							SEGGER_RTT_printf(0, "power is low \r\n");						
						}
					}
				}
			}
		}
	}
}

