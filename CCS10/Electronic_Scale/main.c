#include "msp430g2553.h"
#include "LCD_128.h"
#include "HT1621.h"
#include "TCA6416A.h"

int zhenshu = 0, xiaoshu = 0;
unsigned char key = 0;   //ȥƤ������־λ
unsigned int count = 1; //ȥƤ���ݱ���λ
void ADC10_init(void);
void LCD_Init();
void LCD_Display();
void WDT_init();
void I2C_IODect();
void display_qupi();
void display_my_info();

int main()
{
    WDTCTL = WDTPW + WDTHOLD; //�ؿ��Ź�
    float Vin = 0, sum = 0,mv=0,g=0;
    int qupi = 0;
    int i = 0;

    ADC10_init();
    LCD_Init();
    WDT_init();
    display_my_info();
    _EINT();
    while (1)
    {
        ADC10CTL0 |= ENC + ADC10SC;     //����ѭ���п��� ADC10 ת��
        LPM0; //���߿����жϣ��ȴ� ADC10 ת����ɺ󣬽����ж����л��� CPU ����
        //-----ADCת������жϻ���CPU���ִ�����´���-----
        Vin = (ADC10MEM * 2.5) / 1023;
        sum = Vin + sum;
        i++;
        if (i == 170)
        {
            Vin = sum / 170.0;
            mv = Vin * 1000;
            if (key == 1) //ȥƤ��������
            {
                if (count == 1)
                {
                    display_qupi();
                    __delay_cycles(10000);
                    qupi = 0.00006*mv*mv+3.223*mv-2044.7; //��¼��Ʒ��װ������
                    count = 0;  //ȥƤ������һ�μ�¼��ͱ��������ı�־
                }
                g = 0.00006*mv*mv+3.223*mv-2044.7-qupi;
            }
            else
                g = 0.00006*mv*mv+3.223*mv-2044.7;

            zhenshu = (int)g;
            xiaoshu = (g - zhenshu) * 10;
            LCD_Display();
            i = 0;
            sum = 0;
        }
    }
}

void display_my_info()
{
    LCD_DisplaySeg(4);
    LCD_DisplaySeg(9);
    LCD_DisplaySeg(10); //��ʾJ
    LCD_DisplayDigit(8, 2);
    LCD_ClearSeg(12);
    LCD_ClearSeg(19); //��ʾH
    LCD_DisplayDigit(0, 3);
    LCD_ClearSeg(25);
    LCD_ClearSeg(26);
    LCD_ClearSeg(27); //��ʾL
    LCD_DisplayDigit(1, 4);
    LCD_DisplayDigit(0, 5);
    LCD_DisplayDigit(7, 6);
    HT1621_Reflash(LCD_Buffer);
    __delay_cycles(1000000);
    LCD_Clear();
    LCD_DisplayDigit(0, 5);
    LCD_DisplayDigit(0, 6);
    LCD_DisplaySeg(_LCD_DOT4);
    HT1621_Reflash(LCD_Buffer);
}

#pragma vector = WDT_VECTOR
__interrupt void watchdog_timer(void)
{
    PinIN();
    I2C_IODect();
    LPM0_EXIT;
}

void I2C_IODect() //����¼�ȷʵ������
{
    static unsigned char keyEY_Past = 0, keyEY_Now = 0;
    keyEY_Past = keyEY_Now;
    //----�ж�I2C_IO10������keyEY1�����Ƿ񱻰���------
    if ((TCA6416A_InputBuffer & BIT8) == BIT8)
        keyEY_Now |= BIT0;
    else
        keyEY_Now &= ~BIT0;
    if (((keyEY_Past & BIT0) == BIT0) && (keyEY_Now & BIT0) != BIT0)
    {
        PinOUT(0, 0); //LED1����LED234��
        PinOUT(1, 0);
        PinOUT(2, 0);
        PinOUT(3, 0);
        PinOUT(4, 0);
        PinOUT(5, 0);
        PinOUT(6, 0);
        PinOUT(7, 0);
        PinOUT(8, 0);
        key = 1;
    }
    //----�ж�I2C_IO12������keyEY3�����Ƿ񱻰���------
    if ((TCA6416A_InputBuffer & BITA) == BITA)
        keyEY_Now |= BIT2;
    else
        keyEY_Now &= ~BIT2;
    if (((keyEY_Past & BIT2) == BIT2) && (keyEY_Now & BIT2) == 0)
    {
        PinOUT(0, 1);
        PinOUT(1, 1);
        PinOUT(2, 1);
        PinOUT(3, 1);
        PinOUT(4, 1);
        PinOUT(5, 1);
        PinOUT(6, 1);
        PinOUT(7, 1);
        PinOUT(8, 1);
        count = 1;
        key = 3;
    }
}

void display_qupi()
{
    LCD_Clear();
    LCD_DisplayDigit(9, 3);
    LCD_ClearSeg(20);
    LCD_DisplayDigit(0, 4);
    LCD_ClearSeg(55);
    LCD_DisplayDigit(8, 5);
    LCD_ClearSeg(56);
    LCD_ClearSeg(61);
    LCD_DisplayDigit(1, 6);
    HT1621_Reflash(LCD_Buffer);
}

void WDT_init()
{
    //-----�趨WDTΪ-----------
    WDTCTL = WDT_ADLY_16;
    //-----WDT�ж�ʹ��----------------------
    IE1 |= WDTIE;
}

void LCD_Init()
{
    TCA6416A_Init();
    HT1621_init();
    //���Ӳ���ĳ�ʼ�������� I2C ģ��ĳ�ʼ���� TCA6416A ��ʼ���������ڲ�����ˣ� LCD_128 �⺯���� HT1621 ��ʼ���������ڲ�������
    //-----��ʾ�̶������LCD��-----
}

void LCD_Display()
{
    LCD_DisplaySeg(_LCD_DOT4);
    LCD_DisplayDigit(LCD_DIGIT_CLEAR, 2);
    LCD_DisplayDigit(LCD_DIGIT_CLEAR, 3);
    LCD_DisplayDigit(LCD_DIGIT_CLEAR, 4);
    LCD_DisplayDigit(LCD_DIGIT_CLEAR, 5);
    LCD_DisplayDigit(LCD_DIGIT_CLEAR, 6);
    //-----����zhenshu��ֲ���ʾ����-----

    if (zhenshu > 999)
    {
        LCD_DisplayDigit(zhenshu / 1000, 2);
        LCD_DisplayDigit((zhenshu / 100) % 10, 3);
        LCD_DisplayDigit((zhenshu / 10) % 10, 4);
        LCD_DisplayDigit(zhenshu % 10, 5);
        LCD_DisplayDigit(xiaoshu, 6);
    }
    else if (zhenshu > 99) //100~999��3λ��
    {
        LCD_DisplayDigit(zhenshu / 100, 3);
        LCD_DisplayDigit((zhenshu / 10) % 10, 4);
        LCD_DisplayDigit(zhenshu % 10, 5);
        LCD_DisplayDigit(xiaoshu, 6);
    }
    else if (zhenshu > 9) //2λ
    {
        LCD_DisplayDigit(zhenshu / 10, 4);
        LCD_DisplayDigit(zhenshu % 10, 5);
        LCD_DisplayDigit(xiaoshu, 6);
    }
    else if (zhenshu > 1) //1λ
    {
        LCD_DisplayDigit(zhenshu, 5);
        LCD_DisplayDigit(xiaoshu, 6);
    }
    else
    {
        LCD_DisplayDigit(0, 5);
        LCD_DisplayDigit(xiaoshu, 6);
    }
    HT1621_Reflash(LCD_Buffer); //-----���»��棬������ʾ-----
}

void ADC10_init(void)
{
    ADC10CTL0 &= ~ENC;
    ADC10CTL0 = ADC10IE | ADC10ON | REFON | REF2_5V | ADC10SHT_0 | SREF_1;
    //�����ڲ�2.5V�ο���ѹ����������ʱ��Ϊ64 x ADC10CLKs
    ADC10CTL1 = CONSEQ_0 | ADC10SSEL_0 | ADC10DIV_3 | SHS_0 | INCH_0;
    //ADCʱ�ӷ�ƵΪ7��Ƶ
    ADC10AE0 = 0x1; //P1.0ΪADC�����
    __delay_cycles(20000);
    //ADC10CTL0 |= ENC;
}

#pragma vector = ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
    LPM0_EXIT;
}
