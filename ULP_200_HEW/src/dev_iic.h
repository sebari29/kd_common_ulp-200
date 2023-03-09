
/// @file   dev_iic.h
/// @date   2012/02/14
/// @author jinhwa Lee
/// @brief  I2C 통신 디바이스 드라이브 파일

#ifndef ___DEV_I2C_H___
#define ___DEV_I2C_H___

#include "dev_define.h"

#define EEP_DATA        p3_7
#define EEP_CLK         p3_5
#define EEP_DATA_DIR    pd3_7
#define EEP_DATA_INPUT  0
#define EEP_DATA_OUTPUT 1


void __I2C_START(void);
void __I2C_STOP(void);
void __I2C_P2S(U8 Data);
U8 __I2C_S2P(void);
U8 __ACKDETECT(void);
void __EEPDATASET(void);
void __NACK(void);
void __ACK(void) ;

#endif  // #ifndef ___DEV_I2C_H___

