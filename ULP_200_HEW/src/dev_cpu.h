
/// @file   dev_cpu.h
/// @date   2012/02/14
/// @author jinhwa Lee
/// @brief  delay, Whatchdog, InitializeClock 관련 디바이스 드라이브 파일

#ifndef ___DEV_CPU_H___
#define ___DEV_CPU_H___

#include "dev_define.h"

#define __wdt_reset() {wdtr = 0x00;wdtr = 0xff;} ///< Whatchdog Reset
#define __NOP() asm("NOP") ///< NOP

void __delay_ms(U16 tTime);
void __wdt_init(void);
void __InitializeClock(void);
//void __wdt_reset(void);

#endif  // #ifndef ___DEV_CPU_H___

