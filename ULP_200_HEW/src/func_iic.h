
/// @file   func_iic.h
/// @date   2012/02/14
/// @author jinhwa Lee
/// @brief  I2C 관련 파일

#ifndef ___FUNC_IIC_H___
#define ___FUNC_IIC_H___

#include "dev_iic.h"

#define EEPROM_START_ADDR   0x00
#define EEPROM_END_ADDR     0x7FF   // 8KByte

#define FACTORY_ADDR        0x00
#define FACTORY_TEST_ADDR   0x01

#define LOBBY_HANDLE_ADDR   0x03
#define LOBBY_HANDLE_SIZE   (sizeof(typeLobbyHandler)) // 사이즈를 계산해서 넣어 주어야 함

#define HOME_PASS_START_ADDR  (0x0064)  // addr = 100
#define HOME_PASS_SIZE        (U16)(4)
#define HOME_PASS_MAX_COUNT   (U16)(200)


U8 InitializeFactoryEEPROM(void);
void wr_RFCard( U8 data );
U8 rd_RFCard( U8 *ReadData );
U8 InitializeLobbyHandle(void);
U8 LobbyHandleErrorRamReset(void);
U8 PasswordWriteHome( U8 tHooH, U8 tHooL,U8 *tPassword );
U8 PasswordHomeCompare( U8 tHooH, U8 tHooL, U8 *tPassword );
U8 PasswordReadHome( U8 tHooH, U8 tHooL, U8 *tPassword );
U8 PasswordHomeAllDel ( void );

#endif  // #ifndef ___FUNC_IIC_H___

