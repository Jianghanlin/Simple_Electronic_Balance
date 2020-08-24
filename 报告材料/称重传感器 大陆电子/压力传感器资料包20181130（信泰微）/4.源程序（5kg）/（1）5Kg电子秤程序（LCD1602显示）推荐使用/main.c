/**********************************************************************

***********************************************************************/

#include "main.h"
#include "HX711.h"
#include "uart.h"
#include "LCD1602.h"

unsigned long HX711_Buffer = 0;
unsigned long Weight_Maopi = 0;
long Weight_Shiwu = 0;

unsigned char flag = 0;
bit Flag_ERROR = 0;
sbit speak= P1^7;

//У׼����
//��Ϊ��ͬ�Ĵ������������߲��Ǻ�һ�£���ˣ�ÿһ����������Ҫ�������������������ʹ����ֵ��׼ȷ��
//�����ֲ��Գ���������ƫ��ʱ�����Ӹ���ֵ��
//������Գ���������ƫСʱ����С����ֵ��
//��ֵ����ΪС��
#define GapValue 400


//****************************************************
//������
//****************************************************
void main()
{
    Uart_Init();
    Send_Word("Welcome to use!\n");
    Send_Word("Made by Beetle Electronic Technology!\n");
	
	Init_LCD1602();
	LCD1602_write_com(0x80);
	LCD1602_write_word("Welcome to use!");
	Delay_ms(1000);		 //��ʱ,�ȴ��������ȶ�

	Get_Maopi();				//��ëƤ����
	
	while(1)
	{
		EA = 0;
		Get_Weight();			//����
		EA = 1;

		Scan_Key();

		//��ʾ��ǰ����
		if( Flag_ERROR == 1)
		{
            Send_Word("ERROR\n");
			
			LCD1602_write_com(0x80+0x40);
			LCD1602_write_word("ERROR ");
			speak=0;
		}		
		else
		{         
			      speak=1;
            Send_ASCII(Weight_Shiwu/1000 + 0X30);
            Send_ASCII(Weight_Shiwu%1000/100 + 0X30);
            Send_ASCII(Weight_Shiwu%100/10 + 0X30);
            Send_ASCII(Weight_Shiwu%10 + 0X30);
            Send_Word(" g\n");
			
			LCD1602_write_com(0x80+0x40);
			LCD1602_write_data(Weight_Shiwu/1000 + 0X30);
            LCD1602_write_data(Weight_Shiwu%1000/100 + 0X30);
            LCD1602_write_data(Weight_Shiwu%100/10 + 0X30);
            LCD1602_write_data(Weight_Shiwu%10 + 0X30);
			LCD1602_write_word(" g");
		}
		
	}
}

//ɨ�谴��
void Scan_Key()
{
	if(KEY1 == 0)
	{
		Delay_ms(5);
		if(KEY1 == 0)
		{
			while(KEY1 == 0);
			Get_Maopi();			//ȥƤ
		}	
	}
}

//****************************************************
//����
//****************************************************
void Get_Weight()
{
	Weight_Shiwu = HX711_Read();
	Weight_Shiwu = Weight_Shiwu - Weight_Maopi;		//��ȡ����
	if(Weight_Shiwu > 0)			
	{	
		Weight_Shiwu = (unsigned int)((float)Weight_Shiwu/GapValue); 	//����ʵ���ʵ������
																		
																		
		if(Weight_Shiwu > 5000)		//���ر���
		{
			Flag_ERROR = 1;	
		}
		else
		{
			Flag_ERROR = 0;
		}
	}
	else
	{
		Weight_Shiwu = 0;
	//	Flag_ERROR = 1;				//���ر���
	}
	
}

//****************************************************
//��ȡëƤ����
//****************************************************
void Get_Maopi()
{
	Weight_Maopi = HX711_Read();	
} 

//****************************************************
//MS��ʱ����(12M�����²���)
//****************************************************
void Delay_ms(unsigned int n)
{
	unsigned int  i,j;
	for(i=0;i<n;i++)
		for(j=0;j<123;j++);
}