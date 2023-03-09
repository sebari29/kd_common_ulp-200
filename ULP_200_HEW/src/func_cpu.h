
/// @file   func_cpu.h
/// @date   2012/02/14
/// @author jinhwa Lee
/// @brief  delay, Whatchdog, InitializeClock 관련 파일 헤더

#ifndef ___FUNC_CPU_H___
#define ___FUNC_CPU_H___

#include "dev_cpu.h"

#define wdt_reset() __wdt_reset()

void delay_ms(U16 tTime);
void InitializeWdt(void);
void InitializeClock(void);

#define NOP() __NOP()

#endif  // #ifndef ___DEV_CPU_H___

