
/// @file   dev_uart.h
/// @date   2012/02/14
/// @author jinhwa Lee
/// @brief  Uart 관련 설정 디바이스 드라이브 파일


#ifndef ___DEV_UART_H___
#define ___DEV_UART_H___

#include "dev_define.h"

#define __IsEmptyTxBufferUart0 ti_u0c1
#define __IsEmptyTxBufferUart1 ti_u1c1
#define __IsEmptyTxBufferUart2 ti_u2c1

#define __TransmitBufferUart0  u0tb
#define __TransmitBufferUart1  u1tb
#define __TransmitBufferUart2  u2tbl

#define __ReceiveBufferUart0  u0rb
#define __ReceiveBufferUart1  u1rb
#define __ReceiveBufferUart2  u2rbl

#define __MultiprocessorModeEnable()  mpie = SET    /* Multiprocessor Communication control bit */
#define __MultiprocessorModeDisable() mpie = CLEAR  /* Multiprocessor Communication control bit */

#define __EnableTransmitUart0()       te_u0c1 = SET
#define __DisableTransmitUart0()      te_u0c1 = CLEAR

#define __EnableTransmitUart1()       te_u1c1 = SET
#define __DisableTransmitUart1()      te_u1c1 = CLEAR

#define __EnableTransmitUart2()       te_u2c1 = SET
#define __DisableTransmitUart2()      te_u2c1 = CLEAR

#define __ID_Field_RX                 mprb_u2rb
#define __ID_Field_TX                 mptb_u2tb

#define __TransmitEmptyFlgUart0       txept_u0c0
#define __TransmitEmptyFlgUart1       txept_u1c0
#define __TransmitEmptyFlgUart2       txept_u2c0

void __InitializeUart0_19200(void);
void __InitializeUart1_3906(void);
void __InitializeUart2_3906(void);
void __InitializeUart2_Disable(void);

#endif  // #ifndef ___DEV_UART_H___

