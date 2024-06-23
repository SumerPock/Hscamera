
#include "bsp.h"

/**
  * @brief
  * @param
  * @retval
  */
void bsp_InitLEDGpio(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;

	/* 打开GPIO时钟 */
	GPIO_GPIO_CLK_ALLENABLE();

	/*
	配置所有的LED指示灯GPIO为推挽输出模式
	由于将GPIO设置为输出时，GPIO输出寄存器的值缺省是0，因此会驱动LED点亮.
	这是我不希望的，因此在改变GPIO为输出前，先关闭LED指示灯
	*/
//	bsp_GpioOff(1);
//	bsp_GpioOff(2);
//	bsp_GpioOff(3);
//	bsp_GpioOff(4);
//	bsp_GpioOff(5);
//	bsp_GpioOff(6);
//	bsp_GpioOff(7);
	
	GPIO_InitStruct.Pin = GPIO_PIN_SIG1 | GPIO_PIN_SIG2;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;   
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;	
	HAL_GPIO_Init(GPIO_PORT_SIG1 , &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin   = GPIO_PIN_LED;
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;   
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;	
	HAL_GPIO_Init(GPIO_PORT_LED , &GPIO_InitStruct);

//	GPIO_InitStruct.Pin   = GPIO_PIN_7;
//	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
//	GPIO_InitStruct.Pull  = GPIO_PULLUP;   
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;	
//	HAL_GPIO_Init(GPIOA , &GPIO_InitStruct);
//	
//	GPIO_InitStruct.Pin   = GPIO_PIN_0;
//	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
//	GPIO_InitStruct.Pull  = GPIO_PULLUP;   
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;	
//	HAL_GPIO_Init(GPIOB , &GPIO_InitStruct);
}


/**
  * @brief
  * @param
  * @retval
  */
void Read_Gpio(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;

	/* 打开GPIO时钟 */
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	GPIO_InitStruct.Pin   = GPIO_PIN_3;
	GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;   
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;	
	HAL_GPIO_Init(GPIOB , &GPIO_InitStruct);
}

/**
  * @brief		SedGPGIO
  * @param
  * @retval   PB13 ,  RK电源控制
							PB12 , 	电池控制
							PB15 ,
  */
void SedGPGIO(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;

	/* 打开GPIO时钟 */
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	GPIO_InitStruct.Pin   = GPIO_PIN_13 | GPIO_PIN_12 | GPIO_PIN_15;
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;   
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;	
	HAL_GPIO_Init(GPIOB , &GPIO_InitStruct);
	
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13 , GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12 , GPIO_PIN_RESET);
}

void SedLEDGPGIO(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	/* 打开GPIO时钟 */
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitStruct.Pin   = GPIO_PIN_14;
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;   
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;	
	HAL_GPIO_Init(GPIOB , &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14 , GPIO_PIN_RESET);
}





