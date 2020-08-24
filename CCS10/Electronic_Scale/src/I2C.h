/*
 * I2C.h
 *
 *  Created on: 2013-4-6
 *      Author: Administrator
 */

#ifndef I2C_H_
#define I2C_H_
//#define HARD_I2C			//����Ӳ��I2C����
#define SOFT_I2C				//�������I2C����
extern void I2C_Init();
extern void I2C_Tx_Init();
extern void I2C_Rx_Init();
extern unsigned char I2C_TxFrame(unsigned char *p_Tx,unsigned char num);
extern unsigned char I2C_RxFrame(unsigned char *p_Tx,unsigned char num);

#endif /* I2C_H_ */
