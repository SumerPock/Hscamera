
#include "bsp.h"


#define  DEM_CR_TRCENA               (1 << 24)
#define  DWT_CR_CYCCNTENA            (1 <<  0)


void bsp_InitDWT(void)
{
	DEM_CR         |= (unsigned int)DEM_CR_TRCENA;   
	DWT_CYCCNT      = (unsigned int)0u;
	DWT_CR         |= (unsigned int)DWT_CR_CYCCNTENA;
}

#if 1  /* bsp_timer.c�Ѿ���ʵ�� */

void bsp_DelayMS(uint32_t _ulDelayTime)
{
	bsp_DelayUS(1000*_ulDelayTime);
}


void bsp_DelayUS(uint32_t _ulDelayTime)
{
    uint32_t tCnt, tDelayCnt;
	uint32_t tStart;
		
	tStart = DWT_CYCCNT;                                     /* �ս���ʱ�ļ�����ֵ */
	tCnt = 0;
	tDelayCnt = _ulDelayTime * (SystemCoreClock / 1000000);	 /* ��Ҫ�Ľ����� */ 		      

	while(tCnt < tDelayCnt)
	{
		tCnt = DWT_CYCCNT - tStart; /* ��������У����������һ��32λ���������¼�������Ȼ������ȷ���� */	
	}
}

void bsp_DelayDWT(uint32_t _ulDelayTime)
{
    uint32_t tCnt, tDelayCnt;
	uint32_t tStart;
		
	tCnt = 0;
	tDelayCnt = _ulDelayTime;	 /* ��Ҫ�Ľ����� */ 		      
	tStart = DWT_CYCCNT;         /* �ս���ʱ�ļ�����ֵ */
	
	while(tCnt < tDelayCnt)
	{
		tCnt = DWT_CYCCNT - tStart; /* ��������У����������һ��32λ���������¼�������Ȼ������ȷ���� */	
	}
}
#endif

