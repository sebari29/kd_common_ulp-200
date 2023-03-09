
/// @file   func_cpu.c
/// @date   2012/02/14
/// @author jinhwa Lee
/// @brief  delay, Whatchdog, InitializeClock 관련 파일

#include "main.h"

/// @brief Delay msec
/// @param void
/// @return void
void delay_ms(U16 tTime)
{
  __delay_ms(tTime);
}

/// @brief Whatchdog Initialize
/// @param void
/// @return void
void InitializeWdt(void)
{
  __wdt_init();
}

/// @brief Init the Clock Generator and Oscillation stabilization time
/// @param void
/// @return void
void InitializeClock(void)  // cpu clock : 4MHz setting
{
  __InitializeClock();
}



