
/// @file   func_factory.h
/// @date   2012/02/14
/// @author jinhwa Lee
/// @brief  공장 TEST 모드 파일


#ifndef ___FUNC_FACTORY_H___

#define ___FUNC_FACTORY_H___

#define TEST_HOME1_TALK   0x11
#define TEST_HOME2_TALK   0x12
#define TEST_LOBBY1_TALK  0x13
#define TEST_LOBBY2_TALK  0x14
#define TEST_GUARD_TALK   0x15


#define TEST_HOME1_UART   0x21
#define TEST_HOME2_UART   0x22
#define TEST_HOME3_UART   0x23
#define TEST_HOME4_UART   0x24
#define TEST_LOBBY_UART   0x31
#define TEST_GUARD_UART   0x41

#define TEST_CONSOLE_UART 0x51

#define TIMEOUT_FACTORY   600

void KeyScan_Factory(void);
void ExeSwRoutine_Factory(U8 swdata, U8 status);
void ThreadFactoryMode(void);
U8 IsFactoryMode(void);
void KeyFunction_factory( U8 tKeyMap );
void KeyFunction_Fnd_factory( U8 tKeyMap );

void UartRx_factory( U8 tRxData );
void RFCARD_factory( U8 tRxData );
U8 IsFactoryMode2(void);
void KeyFunction_RxUartSlaveLobby_factory( U8 tData );



#endif  // #ifndef ___FUNC_FACTORY_H___

