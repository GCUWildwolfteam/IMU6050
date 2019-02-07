#ifndef __HW_I2C_H
#define __HW_I2C_H

#include "include.h"

#define Open_I2C1
//#define Open_I2C2

#define I2C1_ClockSpeed 400000
#define I2C1_OwnAddress1 0x57  // 101 0111

#define I2C2_ClockSpeed 400000
#define I2C2_OwnAddress1 0x58  // 101 1000

#define BUFFER_LENGTH 32

/*等待超时时间*/
#define I2CT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define I2CT_LONG_TIMEOUT         ((uint32_t)(10 * I2CT_FLAG_TIMEOUT))



void I2Cx_Init(void);
uint32_t I2C_Write(I2C_TypeDef* I2Cx, uint8_t D_Address, uint8_t R_Address, uint8_t data);
uint32_t I2C_requestFrom(I2C_TypeDef* I2Cx, uint8_t D_Address, uint8_t R_Address, uint8_t quantity);
uint8_t I2C1_read(void);
uint8_t I2C2_read(void);


#endif /* __HW_I2C_H */
