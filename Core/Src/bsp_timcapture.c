#include "bsp.h"

/*****
*	Fpwm =Tclk / ((Period + 1)*(Prescaler + 1))(单位：Hz) ，Prescaler = 8400 - 1， Period = 65535
*   = 8400 * 65535 = 550,494,000
		= 84000 000 / 550,494,000
		= 0.1525

比如  定时器频率Tclk = 72Mhz  arr=499   psc=71     那么PWM频率就是720000/500/72=  2000Hz，即2KHz

arr=499,TIM3->CCR1=250     则pwm的占空比为50%  

最终计算得到的频率值 = 分频后得到的时钟频率 / 上升沿个数 

                     = （ 定时器时钟频率 / 预分频系数 ）/ 上升沿个数 ；

最终计算的到的占空比 = 下降沿个数 / 上升沿个数；

值得注意的是：要明确输入PWM波的大致范围，因为需要根据PWM波范围来确定预分频系数；
							比如说输入一个频率大概为50Hz，占空比为10%的PWM波，需要将预分频系数稍微设置大一些；
							（这是因为：定时器所能测量到的最小频率 = 定时器频率 / （ 预分频系数 * 自动重载值 ），
							而因为ARR(Auto-Reload-Register)是一个十六位寄存器，其最大值为65536，所以当需要测量的频率值较小时，必须要将预分频系数设为较大值）

测量最小频率为 10HZ 最大为192HZ
定时器所能测量到的最小频率 = 定时器频率 / （ 预分频系数 * 自动重载值 ）
													 = 100HZ / 20
													 = 5hz
	8400 100
	84000 000 / 84000 0 
 =100
Prescaler = 840
Period    = 100

84000 000 / 84000
84000 /84
1000 = 1ms
*****/

TIM_HandleTypeDef htim3;
void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle);


/* TIM3 init function */


/******

******/
void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_SlaveConfigTypeDef sSlaveConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};
	TIM_OC_InitTypeDef sConfigOC = {0};
	
  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 8400 - 1;
	//htim3.Init.Prescaler = 840 - 1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period = 65535;
	//htim3.Init.Period = 32767;
	//htim3.Init.Period = 100;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
		#if DEBUG_MODE == 1  
		SEGGER_RTT_SetTerminal(2);
		SEGGER_RTT_WriteString (0, RTT_CTRL_TEXT_BRIGHT_RED "HAL_TIM_Base_Init &htim3 is error\r\n");						
		#endif
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
		#if DEBUG_MODE == 1  
		SEGGER_RTT_SetTerminal(2);
		SEGGER_RTT_WriteString (0, RTT_CTRL_TEXT_BRIGHT_RED "HAL_TIM_ConfigClockSource &htim3 is error\r\n");						
		#endif
  }
  if (HAL_TIM_IC_Init(&htim3) != HAL_OK)
  {
		#if DEBUG_MODE == 1  
		SEGGER_RTT_SetTerminal(2);
		SEGGER_RTT_WriteString (0, RTT_CTRL_TEXT_BRIGHT_RED "HAL_TIM_IC_Init &htim3 is error\r\n");						
		#endif
  }
	if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
	{
	#if DEBUG_MODE == 1  
		SEGGER_RTT_SetTerminal(2);
		SEGGER_RTT_WriteString (0, RTT_CTRL_TEXT_BRIGHT_RED "HAL_TIM_PWM_Init &htim3 is error\r\n");						
	#endif
	}
  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_RESET;
  sSlaveConfig.InputTrigger = TIM_TS_TI1FP1;
  sSlaveConfig.TriggerPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sSlaveConfig.TriggerFilter = 0;
  if (HAL_TIM_SlaveConfigSynchro(&htim3, &sSlaveConfig) != HAL_OK)
  {
		#if DEBUG_MODE == 1  
		SEGGER_RTT_SetTerminal(2);
		SEGGER_RTT_WriteString (0, RTT_CTRL_TEXT_BRIGHT_RED "HAL_TIM_SlaveConfigSynchro &htim3 is error\r\n");						
		#endif
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
		#if DEBUG_MODE == 1  
		SEGGER_RTT_SetTerminal(2);
		SEGGER_RTT_WriteString (0, RTT_CTRL_TEXT_BRIGHT_RED "HAL_TIMEx_MasterConfigSynchronization &htim3 is error\r\n");						
		#endif
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
		#if DEBUG_MODE == 1  
		SEGGER_RTT_SetTerminal(2);
		SEGGER_RTT_WriteString (0, RTT_CTRL_TEXT_BRIGHT_RED "HAL_TIM_IC_ConfigChannel &htim3 is error\r\n");						
		#endif
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;
  sConfigIC.ICSelection = TIM_ICSELECTION_INDIRECTTI;
  if (HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
  {
		#if DEBUG_MODE == 1  
		SEGGER_RTT_SetTerminal(2);
		SEGGER_RTT_WriteString (0, RTT_CTRL_TEXT_BRIGHT_RED "HAL_TIM_IC_ConfigChannel &htim3 is error\r\n");						
		#endif
  }
	
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
	{
		#if DEBUG_MODE == 1  
		SEGGER_RTT_SetTerminal(2);
		SEGGER_RTT_WriteString (0, RTT_CTRL_TEXT_BRIGHT_RED "HAL_TIM_PWM_ConfigChannel &htim3 is error\r\n");						
		#endif
	}
	__HAL_TIM_DISABLE_OCxPRELOAD(&htim3, TIM_CHANNEL_3);
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
	  HAL_TIM_MspPostInit(&htim3);

}

void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(timHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspPostInit 0 */

  /* USER CODE END TIM3_MspPostInit 0 */

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**TIM3 GPIO Configuration
    PB0     ------> TIM3_CH3
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN TIM3_MspPostInit 1 */

  /* USER CODE END TIM3_MspPostInit 1 */
  }

}


void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(tim_baseHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspInit 0 */

  /* USER CODE END TIM3_MspInit 0 */
    /* TIM3 clock enable */
    __HAL_RCC_TIM3_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**TIM3 GPIO Configuration
    PA6     ------> TIM3_CH1
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* TIM3 interrupt Init */
    HAL_NVIC_SetPriority(TIM3_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(TIM3_IRQn);
  /* USER CODE BEGIN TIM3_MspInit 1 */

  /* USER CODE END TIM3_MspInit 1 */
  }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspDeInit 0 */

  /* USER CODE END TIM3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM3_CLK_DISABLE();

    /**TIM3 GPIO Configuration
    PA6     ------> TIM3_CH1
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_6);
		HAL_GPIO_DeInit(GPIOB, GPIO_PIN_0);
    /* TIM3 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM3_IRQn);
  /* USER CODE BEGIN TIM3_MspDeInit 1 */

  /* USER CODE END TIM3_MspDeInit 1 */
  }
}

