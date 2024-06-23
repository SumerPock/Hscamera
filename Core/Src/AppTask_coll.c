#include "bsp.h"

/*����ת�ٿ���ռ�ձ�*/
unsigned char fanpwmdata[2] = {0};
/*����Ƭ����ռ�ձ�*/
unsigned char Refrigeration_pwmdata[2] = {0};

uint16_t PWM_RisingCount  = 0;
uint16_t PWM_FallingCount = 0;
float duty = 0.00;


/*
*********************************************************************************************************
*	�� �� ��: AppTaskGPS
*	����˵��: ����1�Ľ����GPS����
*	��    ��: ��
*	�� �� ֵ: ��
* �� �� ��:   
*********************************************************************************************************
*/
void AppTaskGPS(void *argument)
{
	const uint16_t usFrequency = 100;	/* �ӳ����� */
	unsigned int tick = 0;
	tick = osKernelGetTickCount(); 	/* ��ȡ��ǰʱ�� */
	osStatus_t os_Status;
	uint32_t Flag = 0;	
	volatile unsigned int keil = 0;
	while(1)
	{		
		Flag = osEventFlagsGet(event_gps_ID);
		if(Flag)
		{
			g_tBspUsart3.rxFlag = 0;
			unsigned char buffusart3[256] = "";
			memcpy(buffusart3 , g_usart1_rx_buf , 256);	
			gps_pro(&buffusart3[0] , g_tBspUsart1.rxLen);
			/*�Ƿ�Ϊ����ָ��*/
			if(g_tBspUsart1.rxLen <= 8){
				unsigned char Getcontrol[8] = {0};
				memcpy(Getcontrol , g_usart1_rx_buf , 8);	
				if(Getcontrol[0] == 0xEE && Getcontrol[1] == 0xaa && Getcontrol[2] == 0x08)
				{
					/*�����ֶ�ģʽ*/
					switch(Getcontrol[3])
					{
						case 1:/****	LED����	****/
							if(Getcontrol[4] == 0x01)
							{
								if(Getcontrol[5] == 0x01)
								{
									SedLEDGPGIO();
								}
								else
								{
									HAL_GPIO_DeInit(GPIOB, GPIO_PIN_14);
								}
							}
							break;
							
						case 2://����ת�ٿ���
							/*�����ֶ�ģʽ*/
							osEventFlagsSet(event_ManualMode_ID , 0x01U << 0);
							/*����ת��ʱ�� ����Ƭ�ر�*/
							/*0 - 99 ת��Ϊ 0 - 9999*/
							bsp_SetTIMOutPWM(GPIOA, GPIO_PIN_7,  TIM14,  1,  10000, Getcontrol[5] * 100);
							__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_3, 3500);
							/** ͳһ��λΪ100 **/
							fanpwmdata[0] = Getcontrol[5];
							fanpwmdata[1] = Getcontrol[5] >> 8;	
							/****	������Ա��ΪӦ�û�0	****/
							Refrigeration_pwmdata[0] = 0x00;
							Refrigeration_pwmdata[1] = 0x00;
							break;
						
						case 3://����Ƭ����
							/*�����ֶ�ģʽ*/
							osEventFlagsSet(event_ManualMode_ID , 0x01U << 0);
							//25   99%					//50   34HZ 98%				//500  31HZ 83 - 84%		//800  31HZ 74%			//900  31HZ 70 - 71%	//1000 40HZ 60%
							//1500 41HZ 40%			//2000 31HZ 38 - 35%	//2500 35HZ 10% - 15%   //3000 32HZ 8.5%		//3500 ����û�� 0%
							/**	0 - 99 ת��Ϊ 3500 - 25	**/
						  int data = (100 - Getcontrol[5]) * 34.5;
							SEGGER_RTT_SetTerminal(5);
							SEGGER_RTT_printf(0, "the data = %d \r\n" , data);
							/*����Ƭ���������ȫ��*/						
							__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_3, data);	
							bsp_SetTIMOutPWM(GPIOA, GPIO_PIN_7,  TIM14,  1,  10000, 9999);
						
							/*���ȿ���ȫ�ֱ��� ȫ������ */
							fanpwmdata[0] = 0x63;
							fanpwmdata[1] = 0x00;															//����ȫ��
							Refrigeration_pwmdata[0] = Getcontrol[5];
							Refrigeration_pwmdata[1] = Getcontrol[5] >> 8;		//����Ƭռ�ձ�
							break;
				
						case 4:/****	��Դ�л�	****/
							if(Getcontrol[4] == 0x01)
							{
								if(Getcontrol[5] == 0x01)
								{
									HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
								}
								else
								{
									HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
								}
							}							
							break;			
							
						case 5://RK��Դ����
							if(Getcontrol[4] == 0x02)
							{
								switch(Getcontrol[5])
								{
									case 0:/*�ػ�*/
										ThreadIdTaskRKClos = osThreadNew(AppTaskRKClos, NULL, &ThreadRKClos_Attr);/*	RK�ػ�����	*/
										break;
									case 1:/*����*/
										HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13 , GPIO_PIN_SET);
										break;
									case 3:/*����*/
										ThreadIdTaskRKRes = osThreadNew(AppTaskRKRes, NULL, &ThreadRKRes_Attr);
										break;
								}
							}
							break;
				
						default:
							break;
					}
				}
			}		
			memset(g_usart1_rx_buf, 0, sizeof(g_usart1_rx_buf));
			memset(buffusart3, 0, sizeof(buffusart3));
			if(osEventFlagsClear(event_gps_ID , 0x01U) != (osFlagsErrorUnknown|osFlagsErrorParameter|osFlagsErrorResource))
			{	
			}				
		}			
		tick += usFrequency;    /* ����ӳ� */                      
		osDelayUntil(tick);	
	}
}


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
	{//ע������ΪHAL_TIM_ACTIVE_CHANNEL_1������TIM_CHANNEL_1
		PWM_RisingCount = HAL_TIM_ReadCapturedValue(&htim3 , TIM_CHANNEL_1);
		duty = (float)PWM_FallingCount / PWM_RisingCount;
	}
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
	{
		PWM_FallingCount = HAL_TIM_ReadCapturedValue(&htim3 , TIM_CHANNEL_2);
	}	
}



/*
*********************************************************************************************************
*	�� �� ��: AppTaskLED
*	����˵��: �������ݲɼ�����������
*	��    ��: ��
*	�� �� ֵ: ��
* �� �� ��:   
*********************************************************************************************************
*/
void AppTaskLED(void *argument)
{	
	osStatus_t os_StatusQueue; 
	uint16_t usFrequency = 100; /* �ӳ����� */
	uint32_t tick;
	/* ��ȡ��ǰʱ�� */
	tick = osKernelGetTickCount();	
	int iloop = 0;
	unsigned char sensortemp[6] = {0};
	unsigned char gettemp[2] = {0};	
	
//	__IO uint16_t ADC_Value[200] = {0};
	__IO uint16_t ADC_Value[2] = {0};	
	uint32_t chanel_value[2] = {0};
	if(HAL_I2C_Mem_Write(&hi2c2, 0x88, 0x2737, I2C_MEMADD_SIZE_16BIT,  sensortemp, 2, 100) != HAL_OK)
	{
		#if DEBUG_MODE == 1 
			SEGGER_RTT_SetTerminal(2);
			SEGGER_RTT_printf(0, "The i2c is ok \r\n");
		#endif		
	}
	osDelay(2000);
	MX_ADC1_Init();	
//	if(HAL_ADC_Start_DMA(&hadc1 , (uint32_t *)&ADC_Value , 200) == HAL_OK){
		if(HAL_ADC_Start_DMA(&hadc1 , (uint32_t *)&ADC_Value , sizeof(ADC_Value)) == HAL_OK){
		#if DEBUG_MODE == 1 
			SEGGER_RTT_SetTerminal(2);
			SEGGER_RTT_printf(0, "DMA is open ok\r\n");	
		#endif		
	}
	else{
		#if DEBUG_MODE == 1 
			SEGGER_RTT_SetTerminal(2);
			SEGGER_RTT_printf(0, "DMA is open filed\r\n");		
		#endif		
	}
	osDelay(2000);
	
	unsigned int modeloop = 0;	// ��ʼ����ǰ�¶Ⱥͷ���ת��
	int lm75_temp = 0;
	int timerloop = 0;
	int current_temp = 0;

	GPS_DECODE   Getgpsdecode = {0};
	BOARD_DECODE Getboardcode = {0};
	osStatus_t os_Status;
	unsigned char msg_GPS_Number = 0;	/*GPS��Ϣ��������*/
	GPS_T my_tGPS = {0};							/*GPS���ݽṹ��*/
	Adcpower my_adcpow = {0};
	int temperror = 0;
	int Flag = 0;
	int gpstimerloop = 0;
	while(1)
	{
		unsigned char setemp[2] = {0};
		unsigned char gettemp[2] = {0};	
		unsigned char sendBuffer[29] = {0xFF , 0xBB};			
		unsigned char sendBuffer_2[35] = {0xFF , 0xBB};	
		
		/*��ʵ֤�� �������¶ȴ��ж�ȡ���ݻ����Ӧ�ó�������*/
		if(timerloop <= 30){
			timerloop++;
			switch(timerloop)
			{
				case 10:
					/*I2C�¶ȴ����� A0,A1,A2 ����1 , 1001 A2 A1 A0
						x100��1111 << 1 = 1001,1110	= 9E		*/
					if(HAL_I2C_Mem_Read(&hi2c2 , 0X9E , 0x00, I2C_MEMADD_SIZE_8BIT, setemp, 2, 300) != HAL_OK){
						#if DEBUG_MODE == 1  
							SEGGER_RTT_SetTerminal(1);
							SEGGER_RTT_WriteString (0, RTT_CTRL_TEXT_BRIGHT_RED "the lm75b is error \r\n");						
						#endif
						/**** �쳣״̬4	****/
						temperror = 1;
						osEventFlagsSet(event_ErrorCode_ID , 0x01U << 2);
					}
					else
					{
						if(LM75BTemp(setemp , sizeof(setemp) , gettemp , sizeof(gettemp)))
						{
							float lm75b_in_temp = (gettemp[1] << 8 | gettemp[0]) * 0.01;
							lm75b_in_temp 			= lm75b_in_temp + 0.005;
							lm75_temp 					= (int)(lm75b_in_temp * 10);
							#if DEBUG_MODE == 1  
								SEGGER_RTT_SetTerminal(1);
								SEGGER_RTT_printf(0, "lm75_temp = %d \r\n",lm75_temp);	
							#endif
							Getboardcode.temp_builtin[0] = lm75_temp;
							Getboardcode.temp_builtin[1] = lm75_temp >> 8;
							temperror = 0;
						}
						else
						{
							#if DEBUG_MODE == 1
								SEGGER_RTT_SetTerminal(1);
								SEGGER_RTT_printf(0, "temp = -data \r\n");	
							#endif
						}
					}
					break;
					
				case 20:
					if(HAL_I2C_Mem_Read(&hi2c2 , 0x88 , 0xE000, I2C_MEMADD_SIZE_16BIT, sensortemp, 6, 300) == HAL_OK)
					{
						unsigned char temp[3] 		= {0};
						unsigned char humidity[3] = {0};
						memcpy(&temp[0] , &sensortemp[0] , 3);
						memcpy(&humidity[0] , &sensortemp[3] , 3);		
						if(sht30_crc8_check(&temp[0], 2, sensortemp[2]) == 0)
						{/** temp **/
							#if DEBUG_MODE == 1 
								SEGGER_RTT_SetTerminal(2);
								SEGGER_RTT_printf(0, "temp[0] = %x , temp[1] = %x , temp[2] = %x \r\n",temp[0] , temp[1] , temp[2]);													
							#endif		
							Getboardcode.temp_external[0] = temp[2];
							Getboardcode.temp_external[1] = temp[1];
							Getboardcode.temp_external[2] = temp[0];
						}else{
							#if DEBUG_MODE == 1 
								SEGGER_RTT_SetTerminal(2);
								SEGGER_RTT_WriteString (0, RTT_CTRL_TEXT_BRIGHT_RED "temp crc error \r\n");
							#endif
						}
						if(sht30_crc8_check(&humidity[0], 2, sensortemp[5]) == 0)
						{/**	humidity	**/
							#if DEBUG_MODE == 1  
								SEGGER_RTT_SetTerminal(7);
								SEGGER_RTT_printf(0, "humidity[0] = %x , humidity[1] = %x , humidity[2] = %x \r\n",humidity[0] , humidity[1] , humidity[2]);	
							#endif
							Getboardcode.humidity_external[0] = humidity[2];
							Getboardcode.humidity_external[1] = humidity[1];
							Getboardcode.humidity_external[2] = humidity[0];
						}else
						{
							#if DEBUG_MODE == 1 
								SEGGER_RTT_SetTerminal(2);
								SEGGER_RTT_WriteString (0, RTT_CTRL_TEXT_BRIGHT_RED "humidity crc error \r\n");						
							#endif						
						}
					}		
					else
					{
						#if DEBUG_MODE == 1  
							SEGGER_RTT_SetTerminal(2);
							SEGGER_RTT_WriteString (0, RTT_CTRL_TEXT_BRIGHT_RED "read SHT30 is error \r\n");	
						#endif	
						//�쳣״̬4
					}	
					/*���������ֶ�ģʽ������ 5���������*/
					Flag = osEventFlagsGet(event_ManualMode_ID);
					if(Flag == 0)
					{
						timerloop = timerloop;
					}
					else
					{
						timerloop = 0;
						/*�ֶ�ģʽ�� ���ȿ���ת���ź�*/
						Getboardcode.fan_feedback_duty[0] = fanpwmdata[0];
						Getboardcode.fan_feedback_duty[1] = fanpwmdata[1];
						/*�ִ�ģʽ�� ����Ƭ����ռ�ձ�*/
						Getboardcode.refrigeration[0] = Refrigeration_pwmdata[0];
						Getboardcode.refrigeration[1] = Refrigeration_pwmdata[1];
						
					}
					break;
					
				case 30:
					current_temp = lm75_temp;
					int fan_speed = 0;				// �����¶ȵ�������ʽPID���ƺ����������µķ���ת��							
					if(temperror == 0){				/****		�Զ��¿�		****/
						for (current_temp = TEMP_MIN; current_temp <= lm75_temp; current_temp++) {	//������
							fan_speed = pid_control(current_temp, fan_speed);// �����¶ȵ�������ʽPID���ƺ����������µķ���ת��
						}						
						bsp_SetTIMOutPWM(GPIOA, GPIO_PIN_7,  TIM14,  1,  10000, fan_speed); /* Ƶ�ʹ̶�10Khz �� ռ�ձȿ���ת�� */	
						//bsp_SetTIMOutPWM(GPIOA, GPIO_PIN_7,  TIM14,  1,  10000,  8000); 						
						int actual_value = 10000 - fan_speed; // ����ʵ��ֵΪ 12345
						int mapped_value = actual_value * 0.35;
						//25   99%					//50   34HZ 98%				//500  31HZ 83 - 84%		//800  31HZ 74%			//900  31HZ 70 - 71%	//1000 40HZ 60%
						//1500 41HZ 40%			//2000 31HZ 38 - 35%	//2500 35HZ 10% - 15%   //3000 32HZ 8.5%		//3500 ����û�� 0%
						__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_3, mapped_value);
						#if DEBUG_MODE == 1  
							SEGGER_RTT_SetTerminal(6);
							SEGGER_RTT_printf(0,"Fan_control = %d , mapped_value = %d \r\n" , fan_speed , mapped_value);
						#endif
						/*���ȿ���ռ�ձ�*/
						unsigned int fan_spped100 = fan_speed / 100;   /**	ͳһ��λΪ100	**/
						Getboardcode.fan_feedback_duty[0] = fan_spped100;
						Getboardcode.fan_feedback_duty[1] = fan_spped100  >> 8;									
						/* ����Ƭ����ռ�ձ� */
						/* ��ϵӳ�� ������ϵΪ 35 */
						unsigned int data = 0;
						data = 100 - (mapped_value) / 35; 
						Getboardcode.refrigeration[0] = (data);
						Getboardcode.refrigeration[1] = (data) >> 8;
					}
					else
					{
						/*���ȿ���ռ�ձ�*/
						Getboardcode.fan_feedback_duty[0] = 0x0f;
						Getboardcode.fan_feedback_duty[1] = 0x27;		
						bsp_SetTIMOutPWM(GPIOA, GPIO_PIN_7,  TIM14,  1,  10000, 9999);
						__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_3, 50);
						/*����Ƭ���� ռ�ձ�*/
						Getboardcode.refrigeration[0] = 50;
						Getboardcode.refrigeration[1] = 0;					
					}	
					/*�ֶ��¿�*/					
					break;
				
				default:
					break;				
			}
		}else{
			timerloop = 0;
		}

		switch(modeloop)
		{
			case 0:				
				modeloop = 1;
				break;
				
			case 1:
				modeloop = 2;			
				break;
			
			case 2:	/*adc�ɼ�*/				
//				for(int i = 0 ; i < 200;){//˳��ע�룬0 һͨ����2 ��ͨ�� ��3 һͨ�� �� 4 ��ͨ�� ����������˳��
//					chanel_value[0] += ADC_Value[i++];
//					chanel_value[1] += ADC_Value[i++];
//				}

				HAL_ADC_Start_DMA(&hadc1 , (uint32_t *)&ADC_Value , sizeof(ADC_Value));
				chanel_value[0] += ADC_Value[0];
				chanel_value[1] += ADC_Value[1];
			
				Getboardcode.adc_passage1[0] = chanel_value[0];
				Getboardcode.adc_passage1[1] = chanel_value[0] >> 8;
				Getboardcode.adc_passage1[2] = chanel_value[0] >> 16;
				Getboardcode.adc_passage1[3] = chanel_value[0] >> 24;
				Getboardcode.adc_passage2[0] = chanel_value[1];
				Getboardcode.adc_passage2[1] = chanel_value[1] >> 8;
				Getboardcode.adc_passage2[2] = chanel_value[1] >> 16;
				Getboardcode.adc_passage2[3] = chanel_value[1] >> 24;
				
				chanel_value[0] = (chanel_value[0] * 3.3 / 4096)*100;
				chanel_value[1] = (chanel_value[1] * 3.3 / 4096)*100;
				my_adcpow.Battery_power  = chanel_value[0];
				my_adcpow.External_power = chanel_value[1];	
				
				/*���״̬*/
				if(my_adcpow.External_power > 1.0){
					Getboardcode.powerstatic  = 0;
				}else{
					Getboardcode.powerstatic  = 1;				
				}

				/*�������*/
				osMessageQueuePut(msgQueue_ADC_Data , &my_adcpow , NULL , NULL);
				#if DEBUG_MODE == 1  
					SEGGER_RTT_SetTerminal(0);
					SEGGER_RTT_printf(0, "chanel_value[0]  = %d \r\n", chanel_value[0]);
					SEGGER_RTT_printf(0, "chanel_value[1] = %d \r\n" , chanel_value[1]);
				#endif					
				modeloop = 3;	
				break;
				
			case 3: /*����Ƶ�ʲɼ�*/
				#if DEBUG_MODE == 1  
					SEGGER_RTT_SetTerminal(3);
					int iduty = duty * 10000;
					SEGGER_RTT_printf(0 , "PWM_Frequency = %d \r\n", (uint16_t)PWM_RisingCount);
					//SEGGER_RTT_printf(0 , "PWM_Duty = %d \r\n", iduty);
				#endif
				/*�ȴ�����ֽں���ֽ�*/
				Getboardcode.fan_feedback_cycle[0] = PWM_RisingCount;
				Getboardcode.fan_feedback_cycle[1] = PWM_RisingCount >> 8;	
				modeloop = 4;	
				break;
			
			case 4:
				modeloop = 5;
				break;
			
			case 5:
				modeloop = 6;
				break;
			
			case 6: /*���ݴ������*/
				/**��io״̬**/
				if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3)){
					Getboardcode.rk3588_io 		= 0;
				}else{
					Getboardcode.rk3588_io 		= 1;
				}
				unsigned int ErrorFlag = osEventFlagsGet(event_ErrorCode_ID);
				Getboardcode.errorstatic  = ErrorFlag;
	
				/*���ж��쳣״̬*/				
				memcpy(&sendBuffer_2[4] , &Getboardcode , 27);	// 27 
				sendBuffer_2[2] = 0x21;
				sendBuffer_2[3] = 0x02;	
				unsigned short crc16 = crc16_check(sendBuffer_2 , 31); // 27 + 	4				
				sendBuffer_2[31] = crc16;
				sendBuffer_2[32] = crc16 >> 8;
				BSP_UART_Transmit_DMA(&husart1 , sendBuffer_2 , 33);
				BSP_UART_Transmit_DMA(&husart6 , sendBuffer_2 , 33);
				HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8);
				modeloop = 0;			
			break;
			
			default:
				break;			
		}
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_9);
		/*��ʱ��δ�յ�GPS���ݷ��ͼ�����*/
//		gpstimerloop++;
//		if(gpstimerloop >= 30)
//		{
//			gpstimerloop = 0;
//			unsigned char tempbuffer[27] = {0xFF, 0xBB, 0x1B, 0x01,  0x22, 
//																	    0x00, 0x53, 0x7A, 0x02,  0x00,  
//																			0x4E, 0x6D, 0x00, 0x02,  0x5E,  
//																		  0x00, 0x00, 0x45, 0x18,  0x00,  
//																			0x04, 0x08, 0x06, 0x12,  0x20,  0xAB,  0x23};
//			unsigned short crc16 = crc16_check(tempbuffer , 25);			
//			tempbuffer[25] = crc16;
//			tempbuffer[26] = crc16 >> 8;																						
//			BSP_UART_Transmit_DMA(&husart6 , tempbuffer , 27);		
//		}
		
		/*GPS�������*/	
		msg_GPS_Number = osMessageQueueGetCount(msgQueue_GPS_Data);	
		if(msg_GPS_Number > 0) //��Ϣ�����д�������
		{
			gpstimerloop = 0;
			os_Status = osMessageQueueGet(msgQueue_GPS_Data,								/* ��Ϣ���е����� */
																		&my_tGPS,    		  								/* �����Ϣ���ڴ� */
																		NULL,              	  						/* ȡ����Ϣ�����ȼ� */
																		NULL  	      						/* ��ʱʱ�䣬���жϺ��������osMessageQueueGetʱ���˲���������NULL */
																		);		
			Getgpsdecode.Dimensions_degree[0] = my_tGPS.WeiDu_Du;		/*γ��*/
			Getgpsdecode.Dimensions_degree[1] = my_tGPS.WeiDu_Du >> 8;
			Getgpsdecode.Dimensions_minute[0] = my_tGPS.WeiDu_Fen;
			Getgpsdecode.Dimensions_minute[1] = my_tGPS.WeiDu_Fen >> 8;
			Getgpsdecode.Dimensions_minute[2] = my_tGPS.WeiDu_Fen >> 16;
			Getgpsdecode.Dimensions_minute[3] = my_tGPS.WeiDu_Fen >> 24;
			Getgpsdecode.Dimensions_earth = my_tGPS.NS;	
			Getgpsdecode.longitude_degree[0] = my_tGPS.JingDu_Du;		/*����*/
			Getgpsdecode.longitude_degree[1] = my_tGPS.JingDu_Du >> 8;
			Getgpsdecode.longitude_minute[0] = my_tGPS.JingDu_Fen; 
			Getgpsdecode.longitude_minute[1] = my_tGPS.JingDu_Fen >> 8;
			Getgpsdecode.longitude_minute[2] = my_tGPS.JingDu_Fen >> 16;
			Getgpsdecode.longitude_minute[3] = my_tGPS.JingDu_Fen >> 24;
			Getgpsdecode.longitude_earth = my_tGPS.EW;
			Getgpsdecode.yeart[0] = my_tGPS.Year;										/*����*/
			Getgpsdecode.yeart[1] = my_tGPS.Year >> 8;
			Getgpsdecode.moon = my_tGPS.Month;
			Getgpsdecode.day = my_tGPS.Day;		
			Getgpsdecode.hour = my_tGPS.Hour;												/*ʱ��*/
			Getgpsdecode.point = my_tGPS.Min;
			Getgpsdecode.second = my_tGPS.Sec;
			memcpy(&sendBuffer[4] , &Getgpsdecode , 21);		
			sendBuffer[2] = 0x1d;
			sendBuffer[3] = 0x01;
			unsigned short crc16 = crc16_check(sendBuffer , 25);			
			sendBuffer[25] = crc16;
			sendBuffer[26] = crc16 >> 8;
			BSP_UART_Transmit_DMA(&husart1 , sendBuffer , 27);	
			BSP_UART_Transmit_DMA(&husart6 , sendBuffer , 27);
		}		
		tick += usFrequency;     /* ����ӳ� */                     
		osDelayUntil(tick);		
	}
}



