
#include "bsp.h"

/**
  * @brief
  * @param
  * @retval
  */
void bsp_InitLEDGpio(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;

	/* ��GPIOʱ�� */
	GPIO_GPIO_CLK_ALLENABLE();

	/*
	�������е�LEDָʾ��GPIOΪ�������ģʽ
	���ڽ�GPIO����Ϊ���ʱ��GPIO����Ĵ�����ֵȱʡ��0����˻�����LED����.
	�����Ҳ�ϣ���ģ�����ڸı�GPIOΪ���ǰ���ȹر�LEDָʾ��
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

	/* ��GPIOʱ�� */
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
  * @retval   PB13 ,  RK��Դ����
							PB12 , 	��ؿ���
							PB15 ,
  */
void SedGPGIO(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;

	/* ��GPIOʱ�� */
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
	/* ��GPIOʱ�� */
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitStruct.Pin   = GPIO_PIN_14;
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;   
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;	
	HAL_GPIO_Init(GPIOB , &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14 , GPIO_PIN_RESET);
}





