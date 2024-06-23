#include "bsp.h"
/*
*********************************************************************************************************
*	�� �� ��: AppTaskRKRes
*	����˵��: RK����
*	��    ��: ��
*	�� �� ֵ: ��
* �� �� ��:   
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
		tick += usFrequency;    /* ����ӳ� */                      
		osDelayUntil(tick);	
	}
}


/*
*********************************************************************************************************
*	�� �� ��: AppTaskRKClos
*	����˵��: RK�ػ�����
*	��    ��: ��
*	�� �� ֵ: ��
* �� �� ��:   
*********************************************************************************************************
*/
void AppTaskRKClos(void *argument){
	const uint16_t usFrequency = 250;	/* �ӳ����� */
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
*	�� �� ��: AppTaskSysPowOff
*	����˵��: ϵͳ�µ�����
*	��    ��: ��
*	�� �� ֵ: ��
* �� �� ��:   
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
	
	int loopdata = 0;								//���ڼ���ⲿ��Դ�µ��ʱ��
	int Loopdatapoweron = 0;				//���ڼ���ⲿ��Դ�ϵ�ʱ��
	int RK3588Shut_down = 0; 				//RK�ػ� ��־
	int Secondary_ShutDownloop = 0;	//���ε�Դ����ʱ��
	int RKNotShut_downtime = 0;			//RK�ػ��ȴ�ʱ��
	
	int forceoff = 0;			//ǿ�ƹػ�
	while(1)
	{
		msg_ADC_Number = osMessageQueueGetCount(msgQueue_ADC_Data);	
		if(msg_ADC_Number > 0)
		{	
			os_Status = osMessageQueueGet(msgQueue_ADC_Data,/* ��Ϣ���е����� */
															&myget_adcpow,    			/* �����Ϣ���ڴ� */
															NULL,            				/* ȡ����Ϣ�����ȼ� */
															NULL);  	  						/* ��ʱʱ�䣬���жϺ��������osMessageQueueGetʱ���˲���������NULL */	
			if(myget_adcpow.External_power < 50)
			{//����״̬
				SEGGER_RTT_SetTerminal(5);
				SEGGER_RTT_printf(0, "power is low %d\r\n" , loopdata);
				loopdata++;
			}
			else
			{//�ϵ�״̬
				if(myget_adcpow.External_power > 250)
				{
					Loopdatapoweron++;	//��ѹ���������ⲿ��
					SEGGER_RTT_SetTerminal(5);
					SEGGER_RTT_printf(0, "power is hight %d\r\n" , Loopdatapoweron);
					/**�����־λ**/
					if(Loopdatapoweron >= 4)
					{/**����2S����**/
						Loopdatapoweron = 0;
						loopdata = 0;
						osEventFlagsClear(event_ErrorCode_ID , 0x01U << 0);
						osEventFlagsClear(event_ErrorCode_ID , 0x01U << 1);		
						osEventFlagsClear(event_ErrorCode_ID , 0x01U << 2);
						/**RK�Ƿ���ɹػ�**/
						if(RK3588Shut_down == 1 && GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3))
						{/**** �Ȼ�RK���� ****/
							ThreadIdTaskRKRes = osThreadNew(AppTaskRKRes, NULL, &ThreadRKRes_Attr);/*	RK��������	*/																																				
							SEGGER_RTT_SetTerminal(5);	
							SEGGER_RTT_printf(0, "myget_adcpow.External_power secondary on \r\n");		/****		˵������������		****/
							RK3588Shut_down = 0;
						}
						else{
						 /**** ����Ҫ�Ȼ�RK ****/
						}
					}

				}
			}	
		}

		
		if(loopdata >= 30)
		{/****	500ms * 20 = 10S����ʱ����������Ҳ������һ��ϵ�����	****/
			if(RK3588Shut_down == 1 && loopdata >= 65)
			{	/**** RK�ػ���� ****/
				SEGGER_RTT_SetTerminal(5);	
				SEGGER_RTT_printf(0, "myget_adcpow.External_power is close \r\n");	
				Secondary_ShutDownloop = 0;
				/****		ST��ɱ���� , ������һ�����޷�����		****/
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);	
			}	
			else{
			/** ���Ȼ��ʱ�� 6S **/
			}
			if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3))
			{	/****	RK�ػ����	****/
				RK3588Shut_down = 1;
				/**	2024.4.26�� �����������������ػ�ָ��	**/
				ThreadIdTaskRKClos = osThreadNew(AppTaskRKClos, NULL, &ThreadRKClos_Attr);
				/****	�쳣״̬3	****/
				osEventFlagsClear(event_ErrorCode_ID , 0x01U << 0);	//����쳣״̬2
				osEventFlagsClear(event_ErrorCode_ID , 0x01U << 2); //����쳣״̬4
				osEventFlagsSet(event_ErrorCode_ID , 0x01U << 1);
			}
			else
			{	/****	RKû�йػ�	****/
				RKNotShut_downtime++;
				if(RKNotShut_downtime < 60)		
				{	/****	�ȴ�45����	****/									
					SEGGER_RTT_SetTerminal(5);
					SEGGER_RTT_printf(0, "RKNotShut_downtime = %d \r\n" , RKNotShut_downtime);
					/**** �쳣״̬2	****/	
					osEventFlagsClear(event_ErrorCode_ID , 0x01U << 1);	//����쳣״̬3
					osEventFlagsClear(event_ErrorCode_ID , 0x01U << 2); //����쳣״̬4
					osEventFlagsSet(event_ErrorCode_ID , 0x01U << 0);
				}
				else
				{	/****	45S��ʱ -> RK3588����	****/
					RKNotShut_downtime = 0;					/**45S����ʱFLAG**/				
					SEGGER_RTT_SetTerminal(5);			//ǿ�ƹػ�
					SEGGER_RTT_printf(0, "power is force close \r\n");	
					ThreadIdTaskRKClos = osThreadNew(AppTaskRKClos, NULL, &ThreadRKClos_Attr);
				} 
			}
		}
		else
		{/**�ȴ�10S���**/
		}		
		tick += usFrequency;    /* ����ӳ� */                      
		osDelayUntil(tick);		
	}
}

void AppTaskSysErrorCode(void *argument)
{
	uint32_t Flag = 0;
	
	/**	4��LEDģʽ	**/
	/**	�쳣״̬2��ST��⵽���Ͽ�����RK�ػ�δִ�н���		**/
	/**	�쳣״̬3��ST��⵽RK��3.3V�Ͽ���LED��״̬������ 	**/
	static int ledloop = 0;

	osStatus_t os_StatusQueue; 
	uint16_t usFrequency = 100; /* �ӳ����� */
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
				HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);//����״̬
				}
			}
			else
			{
				ledloop = 0;
			}
			break;

			case 1:/**	�쳣״̬2	**/
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

			case 2:/**	�쳣״̬3	**/
				HAL_GPIO_WritePin(GPIOB , GPIO_PIN_14 , GPIO_PIN_RESET);	//	�ر�
			break;

			case 3:
			break;

			case 4:/**	�쳣״̬4	�¶ȴ���������**/
				HAL_GPIO_WritePin(GPIOB , GPIO_PIN_14 , GPIO_PIN_SET);	//����
			break;

			default:
			break;
		}		
		tick += usFrequency;    /* ����ӳ� */                      
		osDelayUntil(tick);		
	}
}

/*
*********************************************************************************************************
*	�� �� ��: AppTaskReceiveRK
*	����˵��: ST���RK������
*	��    ��: ��
*	�� �� ֵ: ��
* �� �� ��:   
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
		os_Status = osMessageQueueGet(msgQueue_RkrRes_Data,	/* ��Ϣ���е����� */
																	&getUart6,    		  	/* �����Ϣ���ڴ� */
																	NULL,              	  /* ȡ����Ϣ�����ȼ� */
																	osWaitForever  	     	/* ��ʱʱ�䣬���жϺ��������osMessageQueueGetʱ���˲���������NULL */
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
							/*�ź����Լ�*/
							SEGGER_RTT_SetTerminal(6);
							SEGGER_RTT_printf(0, "power is low \r\n");						
						}
					}
				}
			}
		}
	}
}

