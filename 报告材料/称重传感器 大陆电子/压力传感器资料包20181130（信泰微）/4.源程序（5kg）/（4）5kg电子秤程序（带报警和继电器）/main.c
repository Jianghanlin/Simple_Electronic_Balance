/**********************************************************************

***********************************************************************/

#include "main.h"
#include "HX711.h"
#include "uart.h"
#include "LCD1602.h"

#define   uchar unsigned char
#define   uint unsigned int
unsigned long HX711_Buffer = 0;
unsigned long Weight_Maopi = 0;
long Weight_Shiwu = 0;
long Weight_warn = 0;

unsigned char flag = 0;
bit Flag_ERROR = 0;
sbit speak= P1^7;	 //������
sbit jiare= P3^0;	//�̵����ӿ�
uchar Ncunchu[5];
unsigned char num1,num2; 
//У׼����
//��Ϊ��ͬ�Ĵ������������߲��Ǻ�һ�£���ˣ�ÿһ����������Ҫ�������������������ʹ����ֵ��׼ȷ��
//�����ֲ��Գ���������ƫ��ʱ�����Ӹ���ֵ��
//������Գ���������ƫСʱ����С����ֵ�� ������ֵҲ����ͨ���������޸ģ��ó����������ֳ����ġ��
//��ֵ����ΪС��

uint GapValue= 430;

#include "eeprom52.h"


/******************�����ݱ��浽��Ƭ���ڲ�eeprom��******************/
void write_eeprom()
{  

	
	Ncunchu[0] = Weight_warn/256;
	Ncunchu[1] = Weight_warn%256;
	
	 num1=GapValue/256;
	num2=GapValue%256;
	
	SectorErase(0x2000);
	byte_write(0x2011,Ncunchu[0]);
	byte_write(0x2012,Ncunchu[1]);
	
	byte_write(0x2001,  num1);
	byte_write(0x2002,  num2);



  byte_write(0x2060, a_a);	
}

/******************�����ݴӵ�Ƭ���ڲ�eeprom�ж�����*****************/
void read_eeprom()
{
  num1   = byte_read(0x2001);
	num2   = byte_read(0x2002);

	
	Ncunchu[0] = byte_read(0x2011);
	Ncunchu[1] = byte_read(0x2012);
	
	Weight_warn  = Ncunchu[0]*256+Ncunchu[1];
	GapValue= num1*256+num2;


	
    a_a      = byte_read(0x2060);
}

/**************�����Լ�eeprom��ʼ��*****************/
void init_eeprom()
{
	read_eeprom();		//�ȶ�
	if(a_a != 156)		//�µĵ�Ƭ����ʼ��Ƭ������eeprom
	{
	   a_a =156;
	   Weight_warn = 100;
		 GapValue= 430;
     write_eeprom();
	}	
	
	
	
}

//****************************************************
//������
//****************************************************
void main()
{
   // Uart_Init();
   // Send_Word("Welcome to use!\n");
   // Send_Word("Made by Beetle Electronic Technology!\n");
	
	Init_LCD1602();
	LCD1602_write_com(0x80);
	LCD1602_write_word("Weight  Warning");
	init_eeprom();      			//��eeprom����
 
	Delay_ms(1000);		 //��ʱ,�ȴ��������ȶ�
	Get_Maopi();				//��ëƤ����
    Delay_ms(100);		 
	Get_Maopi();				//��ëƤ����
	Delay_ms(100);		 
	Get_Maopi();				//���ȥƤ�������ȶ�
	
	while(1)
	{
		EA = 0;
		Get_Weight();			//����
		EA = 1;

		Scan_Key();

		//��ʾ��ǰ����
		if( Flag_ERROR == 1)
		{
         //   Send_Word("ERROR\n");
			
			LCD1602_write_com(0x80+0x40);
			LCD1602_write_word("ERROR ");
			speak=0;
		}		
		else
		{         
		
            LCD1602_write_com(0x80+0x40);
			LCD1602_write_data(Weight_Shiwu/1000 + 0X30);
            LCD1602_write_data(Weight_Shiwu%1000/100 + 0X30);
            LCD1602_write_data(Weight_Shiwu%100/10 + 0X30);
            LCD1602_write_data(Weight_Shiwu%10 + 0X30);
			LCD1602_write_word(" g");

			LCD1602_write_com(0x80+0x40+8);
			LCD1602_write_data(Weight_warn/1000 + 0X30);
            LCD1602_write_data(Weight_warn%1000/100 + 0X30);
            LCD1602_write_data(Weight_warn%100/10 + 0X30);
            LCD1602_write_data(Weight_warn%10 + 0X30);
			LCD1602_write_word(" g");
		}
		
	}
}

//ɨ�谴��
void Scan_Key()
{
	if(KEY1 == 0  && KEY2 == 1 && KEY3 ==1)
	{
		Delay_ms(5);
		if(KEY1 == 0  && KEY2 == 1 && KEY3 ==1)
		{
	   	 Get_Maopi();
		}	
	}

	if(KEY2 == 0 && KEY1 == 1 && KEY3 ==1)
	{
		Delay_ms(20);
		if(KEY2 == 0 && KEY1 == 1 && KEY3 ==1)
		{
		//	while(KEY2 == 0);
		   if(Weight_warn<=5000)	 { Weight_warn+=1;}
		    write_eeprom();       //��������
		}	
	}

	if(KEY3 == 0 && KEY2 == 1 && KEY1 ==1)
	{
		Delay_ms(20);
		if(KEY3 == 0 && KEY2 == 1 && KEY1 ==1)
		{
		//	while(KEY3 == 0);
	    	if(Weight_warn>=2)	Weight_warn-=1;
			 write_eeprom();       //��������
		}	
	}
  
	
	//=======================��ʵ���������У��================================
	if(KEY1 == 0 && KEY2 == 0 && KEY3 ==1)
	{
		Delay_ms(25);
		if(KEY1 == 0 && KEY2 == 0 && KEY3 ==1)
		{

		    if(GapValue<9999)  { GapValue++; }
			   write_eeprom();       //��������

		}	
	}

	if(KEY1 == 0 && KEY2 == 1 && KEY3 ==0)
	{
		Delay_ms(25);
		if(KEY1 == 0 && KEY2 == 1 && KEY3 ==0)
		{	 

	    	if(GapValue>1)  { 	GapValue--; }
			  write_eeprom();       //��������


		}	
	}
	
    if(Weight_Shiwu<Weight_warn)   {jiare=1; speak=1;}	 //�̵����ر�
	  if(Weight_Shiwu>=Weight_warn)  {jiare=0; speak=0; }	 //�̵�����
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