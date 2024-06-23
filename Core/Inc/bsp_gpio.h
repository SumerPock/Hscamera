
#ifndef __BSP_GPIO_H
#define __BSP_GPIO_H

#define GPIO_GPIO_CLK_ALLENABLE() {	  \
		__HAL_RCC_GPIOC_CLK_ENABLE(); \
		__HAL_RCC_GPIOB_CLK_ENABLE(); \
		__HAL_RCC_GPIOA_CLK_ENABLE(); \
	};

/*SIG P1 LED1*/
#define GPIO_PORT_SIG1  GPIOC
#define GPIO_PIN_SIG1		GPIO_PIN_8
	
/*SIG P2 LED2*/
#define GPIO_PORT_SIG2  GPIOC
#define GPIO_PIN_SIG2		GPIO_PIN_9	

/**/
#define GPIO_PORT_LED  GPIOB
#define GPIO_PIN_LED	 GPIO_PIN_14	

extern void SedLEDGPGIO(void);	
extern void SedGPGIO(void);
extern void Read_Gpio(void);
extern void bsp_InitLEDGpio(void);


#endif