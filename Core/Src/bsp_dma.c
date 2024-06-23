#include "bsp.h"

/**
  * Enable DMA controller clock
  */
//void MX_DMA_Init(void)
//{

//  /* DMA controller clock enable */
//  //__HAL_RCC_DMA2_CLK_ENABLE();
//  /* DMA interrupt init */
//}


/*
*********************************************************************************************************
*	函 数 名: MX_DMA_Init
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   					
*********************************************************************************************************
*/
void MX_DMA_Init(void){
  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();
	__HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
	
  HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 1, 0);	/*USART1RX DMA  */
  HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);
	
  HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 1, 0);	/*USART1TX DMA  */
  HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);
	
  HAL_NVIC_SetPriority(DMA1_Stream2_IRQn, 4, 0);	/*UART4RX DMA  */
  HAL_NVIC_EnableIRQ(DMA1_Stream2_IRQn);
	
  HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 4, 0);	/*UART4TX DMA  */
  HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);	
	
	HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 2, 0);	/*UART2RX DMA  */
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
	
  HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 2, 0);	/*UART2TX DMA  */
  HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);	
	
	HAL_NVIC_SetPriority(DMA2_Stream1_IRQn, 6, 0);	/*USART6RX DMA  */
  HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);
	
  HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, 6, 0);	/*USART6TX DMA  */
  HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);

  HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 3, 0);	/*USART3RX DMA  */
  HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);
	
  HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 3, 0);	/*USART3TX DMA  */
  HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);

  HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 5, 0);	/*UART5RX DMA  */
  HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
	
  HAL_NVIC_SetPriority(DMA1_Stream7_IRQn, 5, 0);	/*UART5TX DMA  */
  HAL_NVIC_EnableIRQ(DMA1_Stream7_IRQn);
}