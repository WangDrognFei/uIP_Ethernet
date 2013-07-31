#define UART_GLOBALS

#include "uart.h"

xdata unsigned char uip_combuf[UIP_COM_RX_BUF_SIZE];
xdata unsigned int uip_comwr=0;
xdata unsigned int uip_comrd=0;
//xdata unsigned char uip_combufept=1;	//����bufferΪ�ձ�־

/**********************************************************************
**����ԭ�ͣ�    void Uart_Init(void)
**����˵����	��
**�� �� ֵ��	��      
**˵    ����	���ڳ�ʼ��������T2��ʱ�ӺͲ�������net_cfg.h�ж���
************************************************************************/
void uart_init(void)
{
// 	RCAP2H=(65536-FOSC/16/BPS)/256;		  //6-clock mode
//	RCAP2L=(65536-FOSC/16/BPS)%256;
	RCAP2H=(65536-ETH_CPU_XTAL/32/BPS)/256;		 //12-clock mode 
	RCAP2L=(65536-ETH_CPU_XTAL/32/BPS)%256;
	SCON = 0x50;	                      //���ڹ�����ʽ1
	T2CON = 0x34;	                   
//	TI=1;
	PS=1;	//���ж����ȼ�
	ES=1;
}

/**********************************************************************
**����ԭ�ͣ�    void  	Printf_Char(uchar ascii)
**����˵����	ascii��	���͵����ڵ�ASCII
**�� �� ֵ��	��      
**˵    ����	�����ڷ���һ���ַ�
************************************************************************/
void put(unsigned char ascii)
{	ES=0;
	SBUF=ascii;
	while(!TI);
	TI=0;
	ES=1;
}
/**********************************************************************
**����ԭ�ͣ�    void  		Printf_String(uchar code *string)
**����˵����	string��	�ַ�������ʼ��ַ
**�� �� ֵ��	��      
**˵    ����	�򴮿ڷ����ַ���������0x00����
************************************************************************/
void puts(unsigned char  *string)
{	while(*string!=0)
	{	put(*string++);
	}
}


/*******************************************************************
����ISR��ִ��һЩ����
********************************************************************/
void serial(void) interrupt 4
{	if(TI) 
	{	TI=0;} 

	if (RI)
	{	RI=0;
		uip_combuf[uip_comwr++]=SBUF;
		if(uip_comwr==UIP_COM_RX_BUF_SIZE) uip_comwr=0;	
	}
	 
}

