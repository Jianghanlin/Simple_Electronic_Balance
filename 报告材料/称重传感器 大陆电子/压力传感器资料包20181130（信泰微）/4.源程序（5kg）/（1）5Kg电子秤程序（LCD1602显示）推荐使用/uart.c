#include "uart.h"

//unsigned char RX_Buffer[30];
unsigned char Num = 0;
bit Flag_Receive_OK = 0;


void Uart_Init()					  			   
{
	SCON = 0X50;  //UART��ʽ1��8λUART
	REN  = 0;     //�������пڽ�������
	PCON = 0x00;  //SMOD=0;�����ʲ��ӱ�
	TMOD = 0x21;  //T1��ʽ2�����ڲ���������
	TH1  = 0xFD;  //װ��ֵ
	TL1  = 0xFD;
	TR1  = 1;     //������ʱ��1
	EA   = 1;     //��ȫ���жϿ���
	ES   = 0;     //���п��ж�	
}

void Send_Word(unsigned char *p)				//�����ַ���
{	
 	while(*p)
 	{
	 	SBUF=*(p++);
		while(TI==0)
		{
			;
			;
			;
		};
		TI=0;
	}   
}

void Send_ASCII(unsigned char c)				//����һ���ַ�
{ 
    TI=0;   
    SBUF=c;   
    while(TI==0);   
    TI=0;   
}























