/*
 * HT1621.c
 *
 *  Created on: 2013-3-30
 *      Author: Administrator
 */

#include "msp430g2553.h"
#include "HT1621.h"
#include "TCA6416A.h"
#include"LCD_128.h"

//-----�����ź��ߺ궨��-----
#define HT1621_CS_LOW       	PinOUT(14,0)
#define HT1621_CS_HIGH  		PinOUT(14,1)
#define HT1621_RD_LOW       	PinOUT(15,0)
#define HT1621_RD_HIGH  		PinOUT(15,1)
#define HT1621_WR_LOW   		PinOUT(16,0)
#define HT1621_WR_HIGH      	PinOUT(16,1)
#define HT1621_DATA_LOW     PinOUT(17,0)
#define HT1621_DATA_HIGH 	PinOUT(17,1)
//-----Ht1621����궨��-----
#define HT1621_COMMAND 		4			//100
#define HT1621_WRITEDISBUF		5			//101
#define HT1621_INIT						0x29 	//����LCD����+1/3bias+4coms
#define HT1621_ON						0x03 	//����LCD����+1/3bias+4coms
#define HT1621_OSC						0x01		//�����ڲ�����
/******************************************************************************************************
 * ��       �ƣ�HT1621_SendBit()
 * ��       �ܣ���HT1621����һλ
 * ��ڲ�����Code������λ����0���������õͣ���֮�ø�
 * ���ڲ�������
 * ˵       ������
 * ��       ������
 ******************************************************************************************************/
void HT1621_SendBit(unsigned int Code)
{
	HT1621_WR_LOW;
	if (Code==0)
	{
		HT1621_DATA_LOW;
	}
	else
	{
		HT1621_DATA_HIGH;
	}
	HT1621_WR_HIGH;
}
/******************************************************************************************************
 * ��       �ƣ�HT1621_init()
 * ��       �ܣ�HT1621�ĳ�ʼ������
 * ��ڲ�������
 * ���ڲ�������
 * ˵       ������ʼ��Ϊ����LCD������1/3Bias��4COM
 * ��       ������
 ******************************************************************************************************/
void HT1621_init()
{
   unsigned char  i =0;
   unsigned char Temp=0;
   // ----��ȫ���ź������ߣ���CSʹ��----
   HT1621_CS_HIGH;
   HT1621_WR_HIGH;
   HT1621_DATA_HIGH;
   HT1621_CS_LOW;
   //----���ͳ�ʼ��������100��������д����-------
   Temp=HT1621_COMMAND;
   for(i=0;i<3;i++)
   {
	 HT1621_SendBit(Temp&BIT2);
	 Temp=Temp<<1;
   }
   //-----����LCD����-----
   Temp=HT1621_ON;
   for(i=0;i<8;i++)
   {
	 HT1621_SendBit(Temp&BIT7);
	 Temp=Temp<<1;
   }
   	HT1621_WR_LOW;	//-----�շ�1λX-----
	HT1621_WR_HIGH;
   //-----������Ϊ1/3bias��4com-----
   Temp=HT1621_INIT;
   for(i=0;i<8;i++)
   {
	 HT1621_SendBit(Temp&BIT7);
	 Temp=Temp<<1;
   }
	HT1621_WR_LOW;	  //-----�շ�1λX-----
	HT1621_WR_HIGH;
	//-----�����ڲ�����-----
   Temp=HT1621_OSC;
   for(i=0;i<8;i++)
   {
	 HT1621_SendBit(Temp&BIT7);
	 Temp=Temp<<1;
   }
	HT1621_WR_LOW;	//-----�շ�1λX-----
	HT1621_WR_HIGH;
    HT1621_CS_HIGH;	//----CS�øߣ�Ƭѡ��ֹ
}
/******************************************************************************************************
 * ��       �ƣ�HT1621_Reflash()
 * ��       �ܣ�����HT1621����ʾ����
 * ��ڲ����� *p���Դ��ӳ��ָ��
 * ���ڲ�������
 * ˵       ������
 * ��       ������
 ******************************************************************************************************/
void HT1621_Reflash(unsigned int *p)
{
   unsigned char i =0,j=0;
   unsigned int Temp=0;
   HT1621_CS_LOW;		//ʹ��
   //----����������101��������д�Դ�-------
   for(i=0;i<3;i++)
   {
	 HT1621_SendBit((HT1621_WRITEDISBUF<<i)&BIT2);
   }
   //-----�����Դ��׵�ַ000000----
   for(i=0;i<=5;i++)
   {
	   HT1621_SendBit(0);
   }
   //----����128λ�Դ棬��8��ÿ��16λ-------
   for(i=0;i<8;i++)
   {
	   Temp= *p++;
	   for(j=0;j<16;j++)
	     {
	  	 HT1621_SendBit(Temp&BIT0);
	  	 Temp=Temp>>1;
	     }
   }
     HT1621_CS_HIGH;		//ʹ�ܽ�ֹ
}
/******************************************************************************************************
 * ��       �ƣ�HT1621_Reflash_Digit()
 * ��       �ܣ�ֻ����8���������ڵĶ�
 * ��ڲ�����Position��8�ֶε�λ��
 * ���ڲ�������
 * ˵       ����
 * ��       ������
 ******************************************************************************************************/
void HT1621_Reflash_Digit(unsigned char Position)
{
  unsigned char Num_Buffer=0;
  unsigned char Addr=0;
   unsigned char i =0;
   Calculate_NumBuff( Position,&Num_Buffer,&Addr);

   HT1621_CS_LOW;		//ʹ��
   //-----����������101��������д�Դ�-------
   for(i=0;i<3;i++)
   {
	 HT1621_SendBit((HT1621_WRITEDISBUF<<i)&BIT2);
   }
   //----����6λ��д�Դ��׵�ַ------
   for(i=0;i<6;i++)
   {
	 HT1621_SendBit((Addr<<i)&BIT5);
   }

    //----����1�ֽ�-------
   for(i=0;i<8;i++)
	 {
	 HT1621_SendBit(Num_Buffer&BIT0);
	 Num_Buffer=Num_Buffer>>1;
	 }
     HT1621_CS_HIGH;		//ʹ�ܽ�ֹ
}
