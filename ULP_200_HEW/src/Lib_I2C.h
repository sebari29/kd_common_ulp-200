
/// @file   Lib_I2C.c
/// @date   2012/03/16
/// @author jinhwa Lee
/// @brief  I2C 통신 라이브러리


#ifndef ___LIB_I2C_H___
#define ___LIB_I2C_H___

//#include "main.h"

//#define I2C_EEPROM_SIZE_16K_OVER   // EEPROM 크기가 2Kbit 이상이면 True

#define I2C_EEPROM_DEVICE_ID      0xA0


#define I2C_DATA_PIN              p3_7
#define I2C_CLK_PIN               p3_5
#define I2C_DIR_DATA_PIN          pd3_7
#define I2C_PIN_DIR_INPUT         0
#define I2C_PIN_DIR_OUTPUT        1

#define I2C_CLK_DELAY             10
#define I2C_WHILE_DELAY           (I2C_CLK_DELAY << 7);  // * 32



void  I2C_START(void);
void  I2C_STOP(void); // SCL을 high일때 SDA를 low -> high로 해줌
void  I2C_WRITE(U8 Data);
U8    I2C_READ(void);
U8    I2C_ACK_DETECT(void);                  // Slave Acknowledge Check
void  I2C_ACK(void);                       // Slave Acknowledge Check
void  I2C_NACK(void);                      // Slave Acknowledge Check

U8 I2C_WriteRawData(U8 tDeviceID, U8 *tData, U8 tLength );
U8 I2C_ReadRawData(U8 tDeviceID, U8 *tData, U8 tLength );

///////////////////////////////////////////////////////////////////////////////

U8 EEPROM_WriteByte( U16 tAddress, U8 tData );
U8 EEPROM_WriteBlcok( U16 tAddress, U8 *tWriteData, U8 tLength );
U8 EEPROM_ReadBlock( U16 tAddress, U8 *tReadData, U8 tLength );

////////////////////////////////////////////////////////////////////////////

#define SPECIALVAL_EXOR_VALUE     0xA5

U8 SpecialVal_WriteByte ( U16 tAddress, U8 tData );
U8 SpecialVal_ReadByte ( U16 tAddress, U8 *tData );
U8 SpecialVal_WriteBlock(U16 tAddress, U8 *tData, U8 tLength);
U8 SpecialVal_ReadBlock(U16 tAddress, U8 *tData, U8 tLength);



#endif  // #ifndef ___LIB_I2C_H___

