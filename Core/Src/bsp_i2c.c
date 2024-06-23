#include "bsp.h"




/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c2.Instance = I2C2;
//  hi2c2.Init.ClockSpeed = 100000;
//	hi2c2.Init.ClockSpeed = 1000;
	hi2c2.Init.ClockSpeed = 500;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    //Error_Handler();
		printf("the i2c is config error");
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}


void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{
 
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(i2cHandle->Instance == I2C2)
  {
  /* USER CODE BEGIN I2C2_MspInit 0 */
 
  /* USER CODE END I2C2_MspInit 0 */
  /** Initializes the peripherals clock
  */
//    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2C2;
//    PeriphClkInitStruct.I2c123ClockSelection = RCC_I2C123CLKSOURCE_D2PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      //Error_Handler();
    }
 
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C2 GPIO Configuration
    PB6     ------> I2C2_SCL
    PB7     ------> I2C2_SDA
    */
//    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
		GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
 
    /* I2C2 clock enable */
    //__HAL_RCC_I2C1_CLK_ENABLE();
		__HAL_RCC_I2C2_CLK_ENABLE();
  /* USER CODE BEGIN I2C2_MspInit 1 */
 
  /* USER CODE END I2C2_MspInit 1 */
  }
}


unsigned char LM75BTemp(unsigned char *buff , unsigned char buffsize , unsigned char *gettempbuff , unsigned char getsize)
{
	unsigned char ucreturn = 0;
	unsigned char getbuff[2] = {0};
	unsigned short gettemp = 0;
	unsigned char i = 0;
	for(i = 0 ; i < buffsize ; i++)
	{
		getbuff[i] = *(buff + i );
	}
	gettemp = (getbuff[0] << 8) | getbuff[1];
	if((gettemp | 0x7FFF) == 0x7FFF)
	{
		ucreturn = 1;//+
		gettemp = (gettemp >> 5);
		gettemp = (gettemp * 0.125) * 100;
		*(gettempbuff + (getsize - 2) ) = gettemp;
		*(gettempbuff + (getsize - 1) ) = gettemp >> 8;
	}
	else
	{
		ucreturn = 2;//-
	}	
	return ucreturn;
}

