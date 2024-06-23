#include "bsp.h"

/**
  ******************************************************************************
  * File Name          : USART.c
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/

/* 串口1的GPIO  PA9, PA10   RS323 DB9接口 */
#define USART1_CLK_ENABLE()              __HAL_RCC_USART1_CLK_ENABLE()

#define USART1_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USART1_TX_GPIO_PORT              GPIOA
#define USART1_TX_PIN                    GPIO_PIN_9
#define USART1_TX_AF                     GPIO_AF7_USART1

#define USART1_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USART1_RX_GPIO_PORT              GPIOA
#define USART1_RX_PIN                    GPIO_PIN_10
#define USART1_RX_AF                     GPIO_AF7_USART1

/* 串口2的GPIO --- PA2 PA3  GPS (只用RX。 TX被以太网占用） */
#define USART2_CLK_ENABLE()              __HAL_RCC_USART2_CLK_ENABLE()

#define USART2_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOD_CLK_ENABLE()
#define USART2_TX_GPIO_PORT              GPIOD
#define USART2_TX_PIN                    GPIO_PIN_5
#define USART2_TX_AF                     GPIO_AF7_USART2

#define USART2_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOD_CLK_ENABLE()
#define USART2_RX_GPIO_PORT              GPIOD
#define USART2_RX_PIN                    GPIO_PIN_6
#define USART2_RX_AF                     GPIO_AF7_USART2

/* 串口3的GPIO --- PB10 PB11  RS485 */
#define USART3_CLK_ENABLE()              __HAL_RCC_USART3_CLK_ENABLE()

#define USART3_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOD_CLK_ENABLE()
#define USART3_TX_GPIO_PORT              GPIOD
#define USART3_TX_PIN                    GPIO_PIN_8
#define USART3_TX_AF                     GPIO_AF7_USART3

#define USART3_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOD_CLK_ENABLE()
#define USART3_RX_GPIO_PORT              GPIOD
#define USART3_RX_PIN                    GPIO_PIN_9
#define USART3_RX_AF                     GPIO_AF7_USART3

/* 串口4的GPIO --- PC10 PC11  被SD卡占用 */
#define UART4_CLK_ENABLE()              __HAL_RCC_UART4_CLK_ENABLE()

#define UART4_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()
#define UART4_TX_GPIO_PORT              GPIOC
#define UART4_TX_PIN                    GPIO_PIN_10
#define UART4_TX_AF                     GPIO_AF8_UART4

#define UART4_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()
#define UART4_RX_GPIO_PORT              GPIOC
#define UART4_RX_PIN                    GPIO_PIN_11
#define UART4_RX_AF                     GPIO_AF8_UART4

/* 串口5的GPIO --- PC12/UART5_TX PD2/UART5_RX (被SD卡占用） */
#define UART5_CLK_ENABLE()              __HAL_RCC_UART5_CLK_ENABLE()

#define UART5_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()
#define UART5_TX_GPIO_PORT              GPIOC
#define UART5_TX_PIN                    GPIO_PIN_12
#define UART5_TX_AF                     GPIO_AF8_UART5

#define UART5_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOD_CLK_ENABLE()
#define UART5_RX_GPIO_PORT              GPIOD
#define UART5_RX_PIN                    GPIO_PIN_2
#define UART5_RX_AF                     GPIO_AF8_UART5

/* 串口6的GPIO --- PG14 PC7  GPRS */
#define USART6_CLK_ENABLE()              __HAL_RCC_USART6_CLK_ENABLE()

#define USART6_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()
#define USART6_TX_GPIO_PORT              GPIOC
#define USART6_TX_PIN                    GPIO_PIN_6
#define USART6_TX_AF                     GPIO_AF8_USART6

#define USART6_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()
#define USART6_RX_GPIO_PORT              GPIOC
#define USART6_RX_PIN                    GPIO_PIN_7
#define USART6_RX_AF                     GPIO_AF8_USART6

/*	USART1 */
UART_HandleTypeDef 	husart1;
DMA_HandleTypeDef 	hdma_usart1_rx;
DMA_HandleTypeDef 	hdma_usart1_tx;

unsigned char g_usart1_rx_buf[USART1_RX_SIZE];
BspUart_t g_tBspUsart1 = 
{
	.pRxBuf = g_usart1_rx_buf,
	.rxSize = USART1_RX_SIZE,
};

/*	USART2 */
UART_HandleTypeDef 	husart2;
DMA_HandleTypeDef 	hdma_usart2_rx;
DMA_HandleTypeDef 	hdma_usart2_tx;

unsigned char g_usart2_rx_buf[USART2_RX_SIZE];
BspUart_t g_tBspUsart2 = 
{
	.pRxBuf = g_usart2_rx_buf,
	.rxSize = USART2_RX_SIZE,
};

/*	USART3 */
UART_HandleTypeDef 	husart3;
DMA_HandleTypeDef 	hdma_usart3_rx;
DMA_HandleTypeDef 	hdma_usart3_tx;

unsigned char g_usart3_rx_buf[USART3_RX_SIZE];
BspUart_t g_tBspUsart3 = 
{
	.pRxBuf = g_usart3_rx_buf,
	.rxSize = USART3_RX_SIZE,
};

/*	UART4 */
UART_HandleTypeDef 	huart4;
DMA_HandleTypeDef 	hdma_uart4_rx;
DMA_HandleTypeDef 	hdma_uart4_tx;

unsigned char g_uart4_rx_buf[UART4_RX_SIZE];
BspUart_t g_tBspUart4 = 
{
	.pRxBuf = g_uart4_rx_buf,
	.rxSize = UART4_RX_SIZE,
};

/*	UART5 */
UART_HandleTypeDef 	huart5;
DMA_HandleTypeDef 	hdma_uart5_rx;
DMA_HandleTypeDef 	hdma_uart5_tx;

unsigned char g_uart5_rx_buf[UART5_RX_SIZE];
BspUart_t g_tBspUart5 = 
{
	.pRxBuf = g_uart5_rx_buf,
	.rxSize = UART5_RX_SIZE,
};

/*	USART6 */
UART_HandleTypeDef 	husart6;
DMA_HandleTypeDef 	hdma_usart6_rx;
DMA_HandleTypeDef 	hdma_usart6_tx;

unsigned char g_usart6_rx_buf[USART6_RX_SIZE];
BspUart_t g_tBspUsart6 = 
{
	.pRxBuf = g_usart6_rx_buf,
	.rxSize = USART6_RX_SIZE,
};
/*
*********************************************************************************************************
*	函 数 名: HAL_UART_RxCpltCallback
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   					
*********************************************************************************************************
*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1)
	{ 
	}
	else if(huart->Instance == USART2)
	{
	}	
	else if(huart->Instance == USART3)
	{	
	}
	else if(huart->Instance == UART4)
	{	
	}
	else if(huart->Instance == UART5)
	{	
	}	
	else if(huart->Instance == USART6)
	{
	}
}



/*
*********************************************************************************************************
*	函 数 名: HAL_UART_TxCpltCallback
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   					
*********************************************************************************************************
*/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
}


/*
*********************************************************************************************************
*	函 数 名: BSP_UART_Transmit_IT
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   
						注意阻塞方式的发送函数不能通过开关中断开操作，避免使用阻塞方式
*********************************************************************************************************
*/
void BSP_UART_Transmit_IT(UART_HandleTypeDef *huart,  unsigned char *pData, unsigned short Size)
{
	while(huart->gState != HAL_UART_STATE_READY)
	{		
	}
	__disable_irq();
	HAL_UART_Transmit_IT(huart, pData, Size);
	__enable_irq();
}

/*
*********************************************************************************************************
*	函 数 名: BSP_UART_Transmit_DMA
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   
*********************************************************************************************************
*/
void BSP_UART_Transmit_DMA(UART_HandleTypeDef *huart,  unsigned char *pData, unsigned short Size)
{
	while(huart->gState != HAL_UART_STATE_READY)
	{		
	}
	__disable_irq();
	HAL_UART_Transmit_DMA(huart, pData, Size);
	__enable_irq();
}

/*
*********************************************************************************************************
*	函 数 名: HAL_UART_ErrorCallback
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   
*********************************************************************************************************
*/
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	#if DEBUG_MODE == 1  
		printf("HAL_UART_ErrorCallback \r\n");
	#endif
}

/*
*********************************************************************************************************
*	函 数 名: MX_USART1_UART_Init
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   
*********************************************************************************************************
*/
void MX_USART1_UART_Init(void)
{

  husart1.Instance = USART1;
  husart1.Init.BaudRate = UART1_BAUD;
  husart1.Init.WordLength = UART_WORDLENGTH_8B;
  husart1.Init.StopBits = UART_STOPBITS_1;
  husart1.Init.Parity = UART_PARITY_NONE;
  husart1.Init.Mode = UART_MODE_TX_RX;
  husart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  husart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&husart1) != HAL_OK)
  {
	#if DEBUG_MODE == 1  
		printf("MX_USART1_UART_Init is fald/r/n");
	#endif	
  }
}

/*
*********************************************************************************************************
*	函 数 名: MX_USART2_UART_Init
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   
*********************************************************************************************************
*/
void MX_USART2_UART_Init(void)
{

  husart2.Instance = USART2;
  husart2.Init.BaudRate = UART2_BAUD;
  husart2.Init.WordLength = UART_WORDLENGTH_8B;
  husart2.Init.StopBits = UART_STOPBITS_1;
  husart2.Init.Parity = UART_PARITY_NONE;
  husart2.Init.Mode = UART_MODE_TX_RX;
  husart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  husart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&husart2) != HAL_OK)
  {
		printf("MX_USART2_UART_Init is fald/r/n");
  }
}


/*
*********************************************************************************************************
*	函 数 名: MX_USART3_UART_Init
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   
*********************************************************************************************************
*/
void MX_USART3_UART_Init(void)
{
  husart3.Instance = USART3;
	husart3.Init.BaudRate = UART3_BAUD;
  husart3.Init.WordLength = UART_WORDLENGTH_8B;
  husart3.Init.StopBits = UART_STOPBITS_1;
  husart3.Init.Parity = UART_PARITY_NONE;
  husart3.Init.Mode = UART_MODE_TX_RX;
  husart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  husart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&husart3) != HAL_OK)
  {
		printf("MX_USART3_UART_Init is fald/r/n");
  }
}

/*
*********************************************************************************************************
*	函 数 名: MX_UART4_UART_Init
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   
*********************************************************************************************************
*/
void MX_UART4_UART_Init(void)
{
  huart4.Instance = UART4;
  huart4.Init.BaudRate = UART4_BAUD;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
		printf("MX_UART4_UART_Init is fald/r/n");
  }
}

/*
*********************************************************************************************************
*	函 数 名: MX_UART4_UART_Init
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   
*********************************************************************************************************
*/
void MX_UART5_UART_Init(void)
{
  huart5.Instance = UART5;
  huart5.Init.BaudRate = UART5_BAUD;
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart5) != HAL_OK)
  {
		printf("MX_UART5_UART_Init is fald/r/n");
  }
}


/*
*********************************************************************************************************
*	函 数 名: MX_USART6_UART_Init
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   
*********************************************************************************************************
*/
void MX_USART6_UART_Init(void)
{
  husart6.Instance = USART6;
  husart6.Init.BaudRate = UART6_BAUD;
  husart6.Init.WordLength = UART_WORDLENGTH_8B;
  husart6.Init.StopBits = UART_STOPBITS_1;
  husart6.Init.Parity = UART_PARITY_NONE;
  husart6.Init.Mode = UART_MODE_TX_RX;
  husart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  husart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&husart6) != HAL_OK)
  {
		printf("MX_USART6_UART_Init is fald/r/n");
  }
}


/*
*********************************************************************************************************
*	函 数 名: HAL_UART_MspInit
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   
*********************************************************************************************************
*/
void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
	
  if(uartHandle->Instance==USART1)
  {		
		#if UART1_DMA_EN == 1		/* 串口1 */
		USART1_CLK_ENABLE();
		USART1_TX_GPIO_CLK_ENABLE();
		USART1_RX_GPIO_CLK_ENABLE();

    GPIO_InitStruct.Pin = USART1_TX_PIN | USART1_RX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(USART1_TX_GPIO_PORT , &GPIO_InitStruct);
		
		/*DMA RX*/
    hdma_usart1_rx.Instance = DMA2_Stream2;
    hdma_usart1_rx.Init.Channel = DMA_CHANNEL_4;
    hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_rx.Init.Mode = DMA_NORMAL;
    hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart1_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK)
    {
			printf("HAL_UART_MspInit is fald/r/n");
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart1_rx);
		
		/*DMA TX*/
    hdma_usart1_tx.Instance = DMA2_Stream7;
    hdma_usart1_tx.Init.Channel = DMA_CHANNEL_4;
    hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_tx.Init.Mode = DMA_NORMAL;
    hdma_usart1_tx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart1_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK)
    {
			printf("HAL_DMA_Init is fald/r/n");
    }
    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart1_tx);

    HAL_NVIC_SetPriority(USART1_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
		#endif
  }
	else if(uartHandle->Instance==USART2)
  {	
		#if UART2_DMA_EN == 1		
		USART2_CLK_ENABLE();
		USART2_TX_GPIO_CLK_ENABLE();
		USART2_RX_GPIO_CLK_ENABLE();

    GPIO_InitStruct.Pin =  USART2_TX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = USART2_TX_AF;
    HAL_GPIO_Init(USART2_TX_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin =  USART2_RX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = USART2_RX_AF;
    HAL_GPIO_Init(USART2_RX_GPIO_PORT, &GPIO_InitStruct);
		
		/*DMA RX*/
    hdma_usart2_rx.Instance = DMA1_Stream5;
    hdma_usart2_rx.Init.Channel = DMA_CHANNEL_4;
    hdma_usart2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart2_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart2_rx.Init.Mode = DMA_NORMAL;
    hdma_usart2_rx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart2_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart2_rx) != HAL_OK)
    {
			#if DEBUG_MODE == 1  
				printf("HAL_UART_MspInit is fald/r/n");
			#endif
    }

    __HAL_LINKDMA(uartHandle , hdmarx , hdma_usart2_rx);
		
		/*DMA TX*/
    hdma_usart2_tx.Instance = DMA1_Stream6;
    hdma_usart2_tx.Init.Channel = DMA_CHANNEL_4;
    hdma_usart2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart2_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart2_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart2_tx.Init.Mode = DMA_NORMAL;
    hdma_usart2_tx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart2_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart2_tx) != HAL_OK)
    {
			#if DEBUG_MODE == 1  
				printf("HAL_DMA_Init is fald/r/n");
			#endif
    }
    __HAL_LINKDMA(uartHandle , hdmatx , hdma_usart2_tx);

    HAL_NVIC_SetPriority(USART2_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
		#endif
  }
	else if(uartHandle->Instance==USART3)
  {
		#if UART3_DMA_EN == 1	
		USART3_CLK_ENABLE();
		USART3_TX_GPIO_CLK_ENABLE();
		USART3_RX_GPIO_CLK_ENABLE();

    GPIO_InitStruct.Pin = USART3_TX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = USART3_TX_AF;
    HAL_GPIO_Init(USART3_TX_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin =  USART3_RX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = USART3_RX_AF;
    HAL_GPIO_Init(USART3_RX_GPIO_PORT, &GPIO_InitStruct);	
		
		/*DMA RX*/
    hdma_usart3_rx.Instance = DMA1_Stream1;
    hdma_usart3_rx.Init.Channel = DMA_CHANNEL_4;
    hdma_usart3_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart3_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart3_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart3_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart3_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart3_rx.Init.Mode = DMA_NORMAL;
    hdma_usart3_rx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart3_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart3_rx) != HAL_OK)
    {
			printf("HAL_UART_MspInit is fald/r/n");
    }

    __HAL_LINKDMA(uartHandle , hdmarx , hdma_usart3_rx);
		
		/*DMA TX*/
    hdma_usart3_tx.Instance = DMA1_Stream3;
    hdma_usart3_tx.Init.Channel = DMA_CHANNEL_4;
    hdma_usart3_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart3_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart3_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart3_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart3_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart3_tx.Init.Mode = DMA_NORMAL;
    hdma_usart3_tx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart3_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart3_tx) != HAL_OK)
    {
			printf("HAL_DMA_Init is fald/r/n");
    }
    __HAL_LINKDMA(uartHandle , hdmatx , hdma_usart3_tx);

    HAL_NVIC_SetPriority(USART3_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
		#endif
  }	
	else if(uartHandle->Instance==UART4)
  {
		#if UART4_DMA_EN == 1	
		UART4_CLK_ENABLE();
		UART4_TX_GPIO_CLK_ENABLE();
		UART4_RX_GPIO_CLK_ENABLE();

    GPIO_InitStruct.Pin = UART4_RX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = UART4_RX_AF;
    HAL_GPIO_Init(UART4_TX_GPIO_PORT, &GPIO_InitStruct);
	
    GPIO_InitStruct.Pin = UART4_TX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = UART4_TX_AF;
    HAL_GPIO_Init(UART4_TX_GPIO_PORT, &GPIO_InitStruct);
		
		/*DMA RX*/
    hdma_uart4_rx.Instance = DMA1_Stream2;
    hdma_uart4_rx.Init.Channel = DMA_CHANNEL_4;
    hdma_uart4_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_uart4_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_uart4_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_uart4_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_uart4_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_uart4_rx.Init.Mode = DMA_NORMAL;
    hdma_uart4_rx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_uart4_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_uart4_rx) != HAL_OK)
    {
			printf("HAL_UART_MspInit is fald/r/n");
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_uart4_rx);

		/*DMA TX*/
    hdma_uart4_tx.Instance = DMA1_Stream4;
    hdma_uart4_tx.Init.Channel = DMA_CHANNEL_4;
    hdma_uart4_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_uart4_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_uart4_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_uart4_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_uart4_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_uart4_tx.Init.Mode = DMA_NORMAL;
    hdma_uart4_tx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_uart4_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_uart4_tx) != HAL_OK)
    {
			printf("HAL_DMA_Init is fald/r/n");
    }

    __HAL_LINKDMA(uartHandle , hdmatx , hdma_uart4_tx);

    HAL_NVIC_SetPriority(UART4_IRQn , 1 , 0);
    HAL_NVIC_EnableIRQ(UART4_IRQn);
		#endif
  }	
	else if(uartHandle->Instance==UART5)
  {
		#if UART5_DMA_EN == 1	
		UART5_CLK_ENABLE();
		UART5_TX_GPIO_CLK_ENABLE();
		UART5_RX_GPIO_CLK_ENABLE();

    GPIO_InitStruct.Pin = UART5_RX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = UART5_RX_AF;
    HAL_GPIO_Init(UART5_RX_GPIO_PORT, &GPIO_InitStruct);
	
    GPIO_InitStruct.Pin = UART5_TX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = UART5_TX_AF;
    HAL_GPIO_Init(UART5_TX_GPIO_PORT, &GPIO_InitStruct);
		
		/*DMA RX*/
    hdma_uart5_rx.Instance = DMA1_Stream0;
    hdma_uart5_rx.Init.Channel = DMA_CHANNEL_4;
    hdma_uart5_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_uart5_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_uart5_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_uart5_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_uart5_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_uart5_rx.Init.Mode = DMA_NORMAL;
    hdma_uart5_rx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_uart5_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_uart5_rx) != HAL_OK)
    {
			printf("HAL_UART_MspInit is fald/r/n");
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_uart5_rx);

		/*DMA TX*/
    hdma_uart5_tx.Instance = DMA1_Stream7;
    hdma_uart5_tx.Init.Channel = DMA_CHANNEL_4;
    hdma_uart5_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_uart5_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_uart5_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_uart5_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_uart5_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_uart5_tx.Init.Mode = DMA_NORMAL;
    hdma_uart5_tx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_uart5_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_uart5_tx) != HAL_OK)
    {	
			printf("HAL_DMA_Init is fald/r/n");
    }
    __HAL_LINKDMA(uartHandle , hdmatx , hdma_uart5_tx);
    HAL_NVIC_SetPriority(UART5_IRQn , 1 , 0);
    HAL_NVIC_EnableIRQ(UART5_IRQn);
		#endif
  }	
	else if(uartHandle->Instance==USART6)
  {
		#if UART6_DMA_EN == 1	
		USART6_CLK_ENABLE();
		USART6_TX_GPIO_CLK_ENABLE();
		USART6_RX_GPIO_CLK_ENABLE();

    GPIO_InitStruct.Pin = USART6_TX_PIN | USART6_RX_PIN;
		//GPIO_InitStruct.Pin = USART6_TX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = USART6_TX_AF;
    HAL_GPIO_Init(USART6_TX_GPIO_PORT, &GPIO_InitStruct);

//    GPIO_InitStruct.Pin = USART6_RX_PIN;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//    GPIO_InitStruct.Alternate = USART6_RX_AF;
//    HAL_GPIO_Init(USART6_RX_GPIO_PORT, &GPIO_InitStruct);
		
		/*DMA RX*/
    hdma_usart6_rx.Instance = DMA2_Stream1;
    hdma_usart6_rx.Init.Channel = DMA_CHANNEL_5;
    hdma_usart6_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart6_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart6_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart6_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart6_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart6_rx.Init.Mode = DMA_NORMAL;
    hdma_usart6_rx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart6_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart6_rx) != HAL_OK)
    {
			#if DEBUG_MODE == 1  
				printf("HAL_USART6_MspInit is fald/r/n");
			#endif
    }
    __HAL_LINKDMA(uartHandle , hdmarx , hdma_usart6_rx);

		/*DMA TX*/
    hdma_usart6_tx.Instance = DMA2_Stream6;
    hdma_usart6_tx.Init.Channel = DMA_CHANNEL_5;
    hdma_usart6_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart6_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart6_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart6_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart6_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart6_tx.Init.Mode = DMA_NORMAL;
    hdma_usart6_tx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart6_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart6_tx) != HAL_OK)
    {
			#if DEBUG_MODE == 1  
				printf("HAL_DMA_Init is fald/r/n");
			#endif
    }
    __HAL_LINKDMA(uartHandle , hdmatx , hdma_usart6_tx);

    HAL_NVIC_SetPriority(USART6_IRQn , 4 , 0);
    HAL_NVIC_EnableIRQ(USART6_IRQn);
		#endif
  }
}


/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 反向初始化串口
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   
*********************************************************************************************************
*/
void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{
  if(uartHandle->Instance==USART1)
  {
    __HAL_RCC_USART1_CLK_DISABLE();

    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9 | GPIO_PIN_10);
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  }
	
	if(uartHandle->Instance==USART2)
  {
    __HAL_RCC_USART2_CLK_DISABLE();

    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2 | GPIO_PIN_3);
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);
    HAL_NVIC_DisableIRQ(USART2_IRQn);
  }

	if(uartHandle->Instance==USART3)
  {
    __HAL_RCC_USART3_CLK_DISABLE();

    //HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2 | GPIO_PIN_3);
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);
    HAL_NVIC_DisableIRQ(USART3_IRQn);
  }
	
	if(uartHandle->Instance==UART4)
  {
    __HAL_RCC_UART4_CLK_DISABLE();

    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_10 | GPIO_PIN_11);
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);
    HAL_NVIC_DisableIRQ(UART4_IRQn);
  }
	if(uartHandle->Instance==USART6)
  {
    __HAL_RCC_USART6_CLK_DISABLE();

    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_6 | GPIO_PIN_7);
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);
    HAL_NVIC_DisableIRQ(USART6_IRQn);
  }
}






/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
