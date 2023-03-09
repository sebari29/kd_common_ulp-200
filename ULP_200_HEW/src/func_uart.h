
/// @file   func_uart.h
/// @date   2012/02/14
/// @author jinhwa Lee
/// @brief  Uart 관련 파일

#ifndef ___FUNC_UART_H___
#define ___FUNC_UART_H___

#include "dev_uart.h"

#define  MAX_UART           3
#define  MAX_BYTE_INTERVAL  5    // 5ms
#define  PKT_TIME           20    // 20ms

#define  LENGTH_INDEX       4

//#define  0TX_BUF u0tb

#define  UART_CONSOLE     0
#define  UART_MASTER      1
#define  UART_SUB         2

#define  IsEmptyTxBufferUart0 __IsEmptyTxBufferUart0
#define  TransmitBufferUart0  __TransmitBufferUart0

extern U8 g485TxBusy;
extern U8 uUartFlag;

#define g485RxBusy      uUartFlag.bit.b0


void InitializeUart0_19200(void);
void InitializeUart1_9600(void);
void InitializeUart1_3906(void);
void InitializeUart1_Disable(void);
void InitializeUart2_9600(void);
void InitializeUart2_3906(void);
void InitializeUart2_Disable(void);


void ComTxEnCtrl(U8 bInOut, U8 Port);
void ComPacketSendCtrl(U8 cMode, U8 cPort);

//void debug_puts(const char *fmt, ...);

U8 EnqueReceiveUart2( U8 *data );
U8 DequeReceiveUart2( U8 *data );

#endif  // #ifndef ___FUNC_UART_H___

