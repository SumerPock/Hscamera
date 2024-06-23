#ifndef __BSP_UART_DMA_H__
#define __BSP_UART_DMA_H__

#include "bsp.h"

#define	UART1_DMA_EN	1
#define	UART2_DMA_EN	1
#define	UART3_DMA_EN	1
#define	UART4_DMA_EN	1
#define	UART5_DMA_EN	1
#define	UART6_DMA_EN	1

#define USART1_RX_SIZE	200
#define USART2_RX_SIZE	128
#define USART3_RX_SIZE	32
#define UART4_RX_SIZE		128
#define UART5_RX_SIZE		128
#define USART6_RX_SIZE	128


/* 定义串口波特率和FIFO缓冲区大小，分为发送缓冲区和接收缓冲区, 支持全双工 */
#if UART1_DMA_EN == 1
	#define UART1_BAUD			115200
	#define UART1_TX_BUF_SIZE	1*1024
	#define UART1_RX_BUF_SIZE	1*1024
#endif

#if UART2_DMA_EN == 1
	#define UART2_BAUD			115200
	#define UART2_TX_BUF_SIZE	1*1024
	#define UART2_RX_BUF_SIZE	1*1024
#endif

#if UART3_DMA_EN == 1
	#define UART3_BAUD			4800
	#define UART3_TX_BUF_SIZE	1*1024
	#define UART3_RX_BUF_SIZE	1*1024
#endif

#if UART4_DMA_EN == 1
	#define UART4_BAUD			115200
	#define UART4_TX_BUF_SIZE	1*1024
	#define UART4_RX_BUF_SIZE	1*1024
#endif

#if UART5_DMA_EN == 1
	#define UART5_BAUD			115200
	#define UART5_TX_BUF_SIZE	1*1024
	#define UART5_RX_BUF_SIZE	1*1024
#endif

#if UART6_DMA_EN == 1
	#define UART6_BAUD			115200
	#define UART6_TX_BUF_SIZE	1*1024
	#define UART6_RX_BUF_SIZE	1*1024
#endif


void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle);
void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle);

void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);
void MX_USART3_UART_Init(void);
void MX_USART6_UART_Init(void);
void MX_UART4_UART_Init(void);
void MX_UART5_UART_Init(void);

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);
void BSP_UART_Transmit_IT(UART_HandleTypeDef *huart,  unsigned char *pData, unsigned short Size);
void BSP_UART_Transmit_DMA(UART_HandleTypeDef *huart,  unsigned char *pData, unsigned short Size);
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart); 
	  
extern unsigned char g_usart1_rx_buf[USART1_RX_SIZE];
extern unsigned char g_usart2_rx_buf[USART2_RX_SIZE];
extern unsigned char g_usart3_rx_buf[USART3_RX_SIZE];
//extern unsigned short g_usart3_rx_buf[USART3_RX_SIZE];
extern unsigned char g_usart6_rx_buf[USART6_RX_SIZE];
extern unsigned char g_uart5_rx_buf[UART5_RX_SIZE];
extern unsigned char g_uart4_rx_buf[UART4_RX_SIZE];


extern UART_HandleTypeDef husart1;
extern UART_HandleTypeDef husart2;
extern UART_HandleTypeDef husart3;
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef husart6;

extern DMA_HandleTypeDef hdma_usart1_tx;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef 	hdma_usart2_rx;
extern DMA_HandleTypeDef 	hdma_usart2_tx;
extern DMA_HandleTypeDef 	hdma_usart3_rx;
extern DMA_HandleTypeDef 	hdma_usart3_tx;
extern DMA_HandleTypeDef 	hdma_usart6_rx;
extern DMA_HandleTypeDef 	hdma_usart6_tx;
extern DMA_HandleTypeDef hdma_uart4_tx;
extern DMA_HandleTypeDef hdma_uart4_rx;
extern DMA_HandleTypeDef hdma_uart5_tx;
extern DMA_HandleTypeDef hdma_uart5_rx;

//#ifdef __cplusplus
//}
//#endif



#endif /* */