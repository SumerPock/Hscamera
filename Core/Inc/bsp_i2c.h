#ifndef __BSP_I2C_H
#define __BSP_I2C_H

extern void MX_I2C2_Init(void);
extern unsigned char LM75BTemp(unsigned char *buff , unsigned char buffsize , unsigned char *gettempbuff , unsigned char getsize);

#endif