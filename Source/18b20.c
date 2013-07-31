//��18b20����Ԥ�������裺1����λ    2������һ��ROMָ��   3������һ��RAMָ��
//Ĭ��12λ���ת��ʱ��750ms

#include <reg52.h>
#include <intrins.h>

/* crystal 22.1184M */

//unsigned char code led_tbl[]={0X3F,0X6,0X5B,0X4F,0X66,0X6D,0X7D,0X7,0X7F,0X6F};	   	//����ܹ�����

code unsigned char decimalH[]={00,06,12,18,25,31,37,43,50,56,62,68,75,81,87,93};	//���:С����λ

code unsigned char decimalL[]={00,25,50,75,00,25,50,75,00,25,50,75,00,25,50,75};	//���:С����λ

//unsigned char code scan_tbl[]={0x01,0x02,0x04,0x08};								//�����ɨ����
//unsigned char display_buffer[4]={1,2,3,4};													//��ʾ����

//----------------------------------------------------------------------
#define SCAN_PORT P0	 //ɨ��� ����Ч P0_0->NUM3	 ~P0_3->NUM0
#define DATA_PORT P2	 //���ݿ� �͵���
sbit P1_0=P1^0;
#define DQ        P1_0   //����ͨ�Ŷ˿�
//----------------------------------------------------------------------
#define delay delay18b20
//���ܣ���ʱ����
void delay(unsigned int i)
{
	while(i--);
}
//-----------------------------------------------------------------------
//���ܣ�18B20��λ����
 unsigned char DS18B20_RST(void)
{
	unsigned char temp;
	DQ = 1;    	//DQ��λ
	delay(16);  	//������ʱ
	DQ = 0;    	//��Ƭ����DQ����
	delay(100); 	//��ȷ��ʱ ���� 480us	   ��ȷ��ʱ:505us
	DQ = 1;    	//��������	input(RX)			   
	delay(16);	//						   ��ȷ��ʱ: 96us
	temp=DQ;
	delay(80);  //						   ��ȷ��ʱ: 408us    408+96=504us>480us ����ʱ��
													   
	//Ϊʲô��delay(4);return DQ;ʱ��⵽18B20 ��ת�����ԣ�����delay(14);return DQ;û��
	//�⵽18B20����ת����ȷ  ��Ϊ�ظ�ʱ������480us

	return temp;	//����1����λʧ��     ����0����λ�ɹ�
}
//------------------------------------------------------------------------
//���ܣ���һ���ֽ�
unsigned char ReadOneChar(void)
{
	unsigned char i=0;
	unsigned char dat = 0;
	for (i=8;i>0;i--)
	{
		DQ = 0; 		// �������ߣ���slot��ʼ
		dat>>=1;
		DQ = 1; 		// �ͷ�����
		if(DQ)
		dat|=0x80;	  	//LSB first
		delay(8);
	}
	return(dat);
}

//���ܣ�дһ���ֽ�
WriteOneChar(unsigned char dat)
{
	unsigned char i=0;
	for (i=8; i>0; i--)
	{
		DQ = 0;
		_nop_();
		DQ = dat&0x01;		//LSB first
		delay(10);			//6.6ms
		DQ = 1;
		dat>>=1;
 }

}
//---------------------------------------------------------------------------
//��ȡ�¶�
unsigned int ReadTemperature(void)
{
	unsigned int  tp=0;
	unsigned char temp_L,temp_H; //��λ �� ��λ

	DS18B20_RST();	  			//						 ���� 1	
	WriteOneChar(0xCC); 		// ����������кŵĲ���	 ���� 2
	WriteOneChar(0x44); 		// �����¶�ת��		 	 ���� 3	
	while(DQ==0);				//�ȴ�ת�����  DQ=0��ʾ����ת����DQ=1��ʾת����� 12λ��Ҫ���750ms

	DS18B20_RST();
	WriteOneChar(0xCC); 		//����������кŵĲ��� 
	WriteOneChar(0xBE); 		//��ȡ�¶ȼĴ����ȣ����ɶ�9���Ĵ����� ǰ���������¶�

	temp_L = ReadOneChar();		//��8λ���ȵͺ�ߵķ������ģ�

	temp_H = ReadOneChar(); 	//�߰�λ
	tp|=temp_H;
	tp<<=8;
	tp|=temp_L;
	return tp;
}

unsigned int get_t(void)
{
	unsigned int t,return_t;
	unsigned char temp1,temp2;

	t=ReadTemperature();

	if(t&0x8000)
	{
	//�¶�Ϊ��
	}
	else
	{
	 	temp1=(unsigned char)(t>>4);
		temp2=(unsigned char)(t&0x000f);
		return_t=decimalH[temp2];
		
		temp1%=100;
		return_t|=(temp1<<8);
	}
	return return_t;		
}

	

#undef delay
  