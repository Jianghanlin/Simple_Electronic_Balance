/*
 * LCD_128.c
 *
 *  Created on: 2013-3-30
 *      Author: Administrator
 */
#include "msp430g2553.h"
#include "LCD_128.h"


unsigned int LCD_Buffer[8]={0,0,0,0,0,0,0,0};	//ȫ�ֱ������Դ�


/***********************************************************************
 * ���ƣ�LCD_Clear( )
 * ���ܣ�����
  ********************************************************************** */
void LCD_Clear()
{
	unsigned char i=0;
  for ( i=0;i<=7;i++) LCD_Buffer[i]=0;
 }
/***********************************************************************
 * ���ƣ�LCD_DisplaySeg()
 * ���ܣ���ʾһ�ζ���
 * ���������SegNum:0~127�κ���
 * ������LCD_DisplaySeg(_LCD_TI_logo)����ʾTI logo
  ********************************************************************** */
void LCD_DisplaySeg(unsigned char SegNum)
{
	LCD_Buffer[SegNum/16]  |= 1<<(SegNum%16);
 }
/***********************************************************************
 * ���ƣ�LCD_ClearSeg()
 * ���ܣ����һ�ζ���
 * ���������SegNum:0~127�κ���
 * ������LCD_ClearSeg(_LCD_TI_logo)����ʾTI logo
  ********************************************************************** */
void LCD_ClearSeg(unsigned char SegNum)
{
	LCD_Buffer[SegNum/16]  &= ~(1<<(SegNum%16));
 }
/***********************************************************************
 *���ƣ�LCD_DisplayDigit(unsigned char Digit,unsigned char Position )
 * ���ܣ���128��ʽҺ�����ض���8�֡�����ʾ0~9
 * ˵���������롰8�֡����������Ϊ0~5��С���롰8�֡����������Ϊ6~9��
 *���������Digit������ʾ���� 0~9����������������Ϊ����
 *					Position����ʾ����λ���ڼ�����8�֡���
 *������LCD_DisplayDigit(9,1 )����1����8�֡�����ʾ9
 * 			   LCD_DisplayDigit(LCD_DIGIT_CLEAR,1)����1�� ��8�֡�������
 ********************************************************************** */
void LCD_DisplayDigit(unsigned char Digit,unsigned char Position )
{
  switch (Position)
  {
  case 1:
      LCD_Buffer[0] &= 0xf10f;     		//����ȫ�����0����8�֡���
      switch (Digit)
    {
    case 0:LCD_Buffer[0] |= 0x0eb0;      break;
    case 1:LCD_Buffer[0] |= 0x0600;      break;
    case 2:LCD_Buffer[0] |= 0x0c70;      break;
    case 3:LCD_Buffer[0] |= 0x0e50;      break;
    case 4:LCD_Buffer[0] |= 0x06c0;      break;
    case 5:LCD_Buffer[0] |= 0x0ad0;      break;
    case 6:LCD_Buffer[0] |= 0x0af0;      break;
    case 7:LCD_Buffer[0] |= 0x0e00;      break;
    case 8:LCD_Buffer[0] |= 0x0ef0;      break;
    case 9:LCD_Buffer[0] |= 0x0ed0;     break;
    default: 	break;				//�������˵����"����"�á�8�֡���
    }
      break;
  case 2:
	 LCD_Buffer[0] &= 0x0fff;			//��1����8�����漰2������λ��
	 LCD_Buffer[1] &= 0xfff1;			//�������߶�ʮһ���������1����8����
	 switch (Digit)
	{
	case 0:
	  LCD_Buffer[0] |= 0xB000;	  LCD_Buffer[1] |= 0x000E;	  break;
	case 1:
	  LCD_Buffer[1] |= 0x0006;	  break;
	case 2:
	  LCD_Buffer[0] |= 0x7000;	  LCD_Buffer[1] |= 0x000C;	  break;
	case 3:
	  LCD_Buffer[0] |= 0x5000;	  LCD_Buffer[1] |= 0x000E;	  break;
	case 4:
	   LCD_Buffer[0] |= 0xC000;  LCD_Buffer[1] |= 0x0006;	  break;
	case 5:
	  LCD_Buffer[0] |= 0xD000;	  LCD_Buffer[1] |= 0x000A;	  break;
	case 6:
	  LCD_Buffer[0] |= 0xF000;	  LCD_Buffer[1] |= 0x000A;	  break;
	case 7:
	  LCD_Buffer[0] |= 0x0000;	  LCD_Buffer[1] |= 0x000E;	  break;
	case 8:
	  LCD_Buffer[0] |= 0xF000;	  LCD_Buffer[1] |= 0x000E;	  break;
	case 9:
	  LCD_Buffer[0] |= 0xD000;	  LCD_Buffer[1] |= 0x000E;	  break;
	default: break;			//�������˵����"����"�á�8�֡���
	}
     break;
  case 3:
	LCD_Buffer[1] &= 0xF10F;		//����ȫ�����2����8�֡���
	switch (Digit)
	{
	case 0:LCD_Buffer[1] |= 0x0EB0;  break;
	case 1:LCD_Buffer[1] |= 0x0600;  break;
	case 2:LCD_Buffer[1] |= 0x0C70;  break;
	case 3:LCD_Buffer[1] |= 0x0E50;  break;
	case 4:LCD_Buffer[1] |= 0x06C0;  break;
	case 5:LCD_Buffer[1] |= 0x0AD0;  break;
	case 6:LCD_Buffer[1] |= 0x0AF0;  break;
	case 7:LCD_Buffer[1] |= 0x0E00;  break;
	case 8:LCD_Buffer[1] |= 0x0EF0;  break;
	case 9:LCD_Buffer[1] |= 0x0ED0;  break;
	default: 		break;
	}
    break;
  case 4:
	LCD_Buffer[3] &= 0xFF10;		//����ȫ�����3����8�֡���
	switch (Digit)
	{
	case 0:LCD_Buffer[3] |= 0x00EB;  break;
	case 1:LCD_Buffer[3] |= 0x0060;  break;
	case 2:LCD_Buffer[3] |= 0x00C7;  break;
	case 3:LCD_Buffer[3] |= 0x00E5;  break;
	case 4:LCD_Buffer[3] |= 0x006C;  break;
	case 5:LCD_Buffer[3] |= 0x00AD;  break;
	case 6:LCD_Buffer[3] |= 0x00AF;  break;
	case 7:LCD_Buffer[3] |= 0x00E0;  break;
	case 8:LCD_Buffer[3] |= 0x00EF;  break;
	case 9:LCD_Buffer[3] |= 0x00ED;  break;
	default: break;
	}
    break;

  case 5:
	LCD_Buffer[3] &= 0x10FF;
	switch (Digit)
	{
	case 0:LCD_Buffer[3] |= 0xEB00;  break;
	case 1:LCD_Buffer[3] |= 0x6000;  break;
	case 2:LCD_Buffer[3] |= 0xC700;  break;
	case 3:LCD_Buffer[3] |= 0xE500;  break;
	case 4:LCD_Buffer[3] |= 0x6C00;  break;
	case 5:LCD_Buffer[3] |= 0xAD00;  break;
	case 6:LCD_Buffer[3] |= 0xAF00;  break;
	case 7:LCD_Buffer[3] |= 0xE000;  break;
	case 8:LCD_Buffer[3] |= 0xEF00;  break;
	case 9:LCD_Buffer[3] |= 0xED00;  break;
	default: break;
	}
    break;

  case 6:
	LCD_Buffer[4] &= 0xFF10;
	switch (Digit)
	{
	case 0:LCD_Buffer[4] |= 0x00EB;	  break;
	case 1:LCD_Buffer[4] |= 0x0060;	  break;
	case 2:LCD_Buffer[4] |= 0x00C7;	  break;
	case 3:LCD_Buffer[4] |= 0x00E5;	  break;
	case 4:LCD_Buffer[4] |= 0x006C;	  break;
	case 5:LCD_Buffer[4] |= 0x00AD;	  break;
	case 6:LCD_Buffer[4] |= 0x00AF;	  break;
	case 7:LCD_Buffer[4] |= 0x00E0;	  break;
	case 8:LCD_Buffer[4] |= 0x00EF;	  break;
	case 9:LCD_Buffer[4] |= 0x00ED;	  break;
	default: break;
	}
    break;

  case 7:
	LCD_Buffer[6] &= 0xFF08;
	switch (Digit)
	{
	case 0:LCD_Buffer[6] |= 0x00D7;	  break;
	case 1:LCD_Buffer[6] |= 0x0006;	  break;
	case 2:LCD_Buffer[6] |= 0x00E3;	  break;
	case 3:LCD_Buffer[6] |= 0x00A7;	  break;
	case 4:LCD_Buffer[6] |= 0x0036;	  break;
	case 5:LCD_Buffer[6] |= 0x00B5;	  break;
	case 6:LCD_Buffer[6] |= 0x00F5;	  break;
	case 7:LCD_Buffer[6] |= 0x0007;	  break;
	case 8:LCD_Buffer[6] |= 0x00F7;	  break;
	case 9:LCD_Buffer[6] |= 0x00B7;	  break;
	default: break;
	}
    break;

  case 8:
	LCD_Buffer[6] &= 0x08ff;
	switch (Digit)
	{
	case 0:LCD_Buffer[6] |= 0xD700;	  break;
	case 1:LCD_Buffer[6] |= 0x0600;	  break;
	case 2:LCD_Buffer[6] |= 0xE300;	  break;
	case 3:LCD_Buffer[6] |= 0xA700;	  break;
	case 4:LCD_Buffer[6] |= 0x3600;	  break;
	case 5:LCD_Buffer[6] |= 0xB500;	  break;
	case 6:LCD_Buffer[6] |= 0xF500;	  	 break;
	case 7:LCD_Buffer[6] |= 0x0700;	  break;
	case 8:LCD_Buffer[6] |= 0xF700;	  	  break;
	case 9:LCD_Buffer[6] |= 0xB700;	  break;
	default: break;
	}
    break;

  case 9:
	LCD_Buffer[7] &= 0xFF08;
	switch (Digit)
	{
	case 0:LCD_Buffer[7] |= 0x00D7;	  break;
	case 1:LCD_Buffer[7] |= 0x0006;	  break;
	case 2:LCD_Buffer[7] |= 0x00E3;	  break;
	case 3:LCD_Buffer[7] |= 0x00A7;	  break;
	case 4:LCD_Buffer[7] |= 0x0036;	  break;
	case 5:LCD_Buffer[7] |= 0x00B5;	  break;
	case 6:LCD_Buffer[7] |= 0x00F5;	  break;
	case 7:LCD_Buffer[7] |= 0x0007;	  break;
	case 8:LCD_Buffer[7] |= 0x00F7;	  break;
	case 9:LCD_Buffer[7] |= 0x00B7;	  break;
	default: break;
	}
    break;

  case 10:
	LCD_Buffer[7] &= 0x08ff;
	switch (Digit)
	{
	case 0:LCD_Buffer[7] |= 0xD700;	  break;
	case 1:LCD_Buffer[7] |= 0x0600;	  break;
	case 2:LCD_Buffer[7] |= 0xE300;	  break;
	case 3:LCD_Buffer[7] |= 0xA700;	  break;
	case 4:LCD_Buffer[7] |= 0x3600;	  break;
	case 5:LCD_Buffer[7] |= 0xB500;	  break;
	case 6:LCD_Buffer[7] |= 0xF500;	  break;
	case 7:LCD_Buffer[7] |= 0x0700;	  break;
	case 8:LCD_Buffer[7] |= 0xF700;	  break;
	case 9:LCD_Buffer[7] |= 0xB700;	  break;
	default: break;
	}
    break;
	default:break;
  }
}
  /****************************************************************************
  * ��    �ƣ�LCD_DisplayNum()
  * ��    �ܣ���LCD��������ʾһ���������ݡ�
  * ��ڲ�����Digit:��ʾ��ֵ  (-32768~32767)
  * ���ڲ�������
  * ��    ��: LCD_DisplayNum( 12345); ��ʾ���: 12345
              	  LCD_DisplayNum(-12345); ��ʾ���:-12345
  * ˵    ��: �ú��������ڴ���Ļ��8�ֶ�����ʾ
  ****************************************************************************/
  void LCD_DisplayNum( long int Digit)
  {
	unsigned char i=0;
	unsigned   char DispBuff[6]={0};
    if(Digit<0)
	{
	Digit=-Digit;		//������
	LCD_DisplaySeg( _LCD_NEG); 		//��ʾ����
	}
    else
    	LCD_ClearSeg(_LCD_NEG); 		//�������
    for(i=0;i<6;i++)                       		//�������
     {
       DispBuff[i]=Digit%10;
       Digit/=10;
     }
    for(i=5;i>1;i--)                     			//������Ч"0"
     {
       if (DispBuff[i]==0) DispBuff[i]=LCD_DIGIT_CLEAR;
       else break;
     }
    for(i=0;i<6;i++)
      {
    	LCD_DisplayDigit(DispBuff[i],6-i);
      }
  }
  /****************************************************************************
  * ��    �ƣ�Calculate_NumBuff()
  * ��    �ܣ����㵥��8�ֵ��Դ����ʼ��ַ
  * ��ڲ�����Position��8�ֶε�λ��
  * 				 *Num_Buffer����Ҫ���µĶ����ָ��
  * 				 *Addr����Ҫ���µĶ������ʼ��ַ��ָ��
  * ���ڲ�������
  * ��         ��: ��
  * ˵         ��: ��
  ****************************************************************************/
  void Calculate_NumBuff(unsigned char Position,unsigned char *Num_Buffer,unsigned char *Addr)
   {
 	  switch(Position)
 	  {
 	  case 1: *Num_Buffer=(LCD_Buffer[0]&0x0ff0)>>4; 	*Addr=4/4;break;
 	  case 2: *Num_Buffer=((LCD_Buffer[0]&0xf000)>>12+((LCD_Buffer[1]&0x000f)<<4));	*Addr=12/4;break;
 	  case 3: *Num_Buffer=(LCD_Buffer[1]&0x0ff0)>>4; 	*Addr=20/4;break;
 	  case 4: *Num_Buffer=(LCD_Buffer[3]&0x00ff); 	*Addr=48/4;break;
 	  case 5: *Num_Buffer=(LCD_Buffer[3]&0xff00)>>8;	 *Addr=56/4;break;
 	  case 6: *Num_Buffer=(LCD_Buffer[4]&0x00ff); 	*Addr=64/4;break;
 	  case 7: *Num_Buffer=(LCD_Buffer[6]&0x00ff);	*Addr=96/4;break;
 	  case 8: *Num_Buffer=(LCD_Buffer[6]&0xff00)>>8;	*Addr=104/4;break;
 	  case 9: *Num_Buffer=(LCD_Buffer[7]&0x00ff);	*Addr=112/4;break;
 	  case 10: *Num_Buffer=(LCD_Buffer[7]&0xff00)>>8; 	*Addr=120/4;break;
 	  default: break;
 	  }
   }
