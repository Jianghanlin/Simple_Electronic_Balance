/**********************************************************************
----------------1.��������:Keil v4					   ----------------
----------------2.ʹ�õ�Ƭ���ͺţ�STC89C52RC		   ----------------

***********************************************************************/
#include "main.h"
#include "HX711.h"

unsigned long HX711_Buffer = 0;
unsigned long Weight_Maopi = 0;
long Weight_Shiwu = 0;

unsigned int Temp_Buffer = 0;
//code unsigned char table[]={ 0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0xff,0x00};			//0,1,2,3,4,5,6,7,8,9,ȫ����ȫ��			//����
code unsigned char table[] = { 0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0X00,0XFF,0x6f,0x79,0x77,0x3f,0x76,0x38};			//0,1,2,3,4,5,6,7,8,9,ȫ����ȫ��,g,e,r,o,h,l			//����
unsigned char COM1_DATA = 0,COM2_DATA = 0,COM3_DATA = 0,COM4_DATA = 0,COM5_DATA = 0,COM6_DATA = 0;
unsigned char flag = 0;
bit Flag_ERROR = 0;


//У׼����
//��Ϊ��ͬ�Ĵ������������߲��Ǻ�һ�£���ˣ�ÿһ����������Ҫ�������������������ʹ����ֵ��׼ȷ��
//�����ֲ��Գ���������ƫ��ʱ�����Ӹ���ֵ��
//������Գ���������ƫСʱ����С����ֵ��
//��ֵ����ΪС��
#define GapValue 430


//****************************************************
//������
//****************************************************
void main()
{
	Init_Timer0();

	COM1_DATA =	16;				//H
	COM2_DATA = 13;				//E		
	COM3_DATA = 17;				//L		
	COM4_DATA = 17;				//L		
	COM5_DATA = 15;				//0		
	COM6_DATA = 10;				//��
				
	Delay_ms(3000);		 //��ʱ,�ȴ��������ȶ�

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
			COM1_DATA = 13;			//E
			COM2_DATA = 14;			//R
			COM3_DATA = 14;			//R
			COM4_DATA = 15;			//O
			COM5_DATA = 14;			//R				
			COM6_DATA = 10;			//��	
		}
		else
		{
			COM1_DATA = Weight_Shiwu/1000;		
			COM2_DATA = Weight_Shiwu%1000/100;		
			COM3_DATA = Weight_Shiwu%100/10;		
			COM4_DATA = Weight_Shiwu%10;
			COM5_DATA = 10;			//��				
			COM6_DATA = 12;			//g	
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
		Flag_ERROR = 1;				//���ر���
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
//��ʼ����ʱ��0
//****************************************************
void Init_Timer0()
{
	TMOD = 0X01;			//T0, ����ģʽ1

	TH0 = (65536 - 2000)/256;
	TL0 = (65536 - 2000)%256;

	TR0 = 1;				//������ʱ��
	ET0 = 1;			    //������ʱ���ж�
	EA = 1;					//�������ж�		
}


//�жϺ���
//****************************************************
void Timer0() interrupt 1
{
	TH0 = (65536 - 2000)/256;
	TL0 = (65536 - 2000)%256;
	
	TF0 = 0;

	flag++;
	if(flag >= 6)
	{
		flag = 0;
	}


	switch(flag)
	{
		case 0:
				COM1 = 0;
				COM2 = 1;
				COM3 = 1;
				COM4 = 1;
				COM5 = 1;
				COM6 = 1;

//				if(COM1_DATA < 10)
//				{
//					SEG_DATA = table[COM1_DATA]|0x80;		////��ʾ��һλֵ��С����
//				}
//				else
//				{
					SEG_DATA = table[COM1_DATA];	
//				}
				
				break;

		case 1:
				COM1 = 1;
				COM2 = 0;
				COM3 = 1;
				COM4 = 1;
				COM5 = 1;
				COM6 = 1;
				SEG_DATA = table[COM2_DATA];		//��ʾ�ڶ�λֵ
				
				break;

							  
		case 2:
				COM1 = 1;
				COM2 = 1;
				COM3 = 0;
				COM4 = 1;
				COM5 = 1;
				COM6 = 1;
				SEG_DATA = table[COM3_DATA];		////��ʾ����λֵ
				
				break;


		case 3:
				COM1 = 1;
				COM2 = 1;
				COM3 = 1;
				COM4 = 0;
				COM5 = 1;
				COM6 = 1;
				SEG_DATA = table[COM4_DATA];			  //��ʾ����λֵ
				break;

		case 4:
				COM1 = 1;
				COM2 = 1;
				COM3 = 1;
				COM4 = 1;
				COM5 = 0;
				COM6 = 1;
				SEG_DATA = table[COM5_DATA];			  //��ʾ����λֵ
				break;

		case 5:
				COM1 = 1;
				COM2 = 1;
				COM3 = 1;
				COM4 = 1;
				COM5 = 1;
				COM6 = 0;
				SEG_DATA = table[COM6_DATA];			  //��ʾ����λֵ
				break;

		default : break;
	}

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