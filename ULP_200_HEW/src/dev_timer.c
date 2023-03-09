
/// @file   dev_timer.c
/// @date   2012/02/14
/// @author jinhwa Lee
/// @brief  Timer 관련 설정 디바이스 드라이브 파일


#include "dev_timer.h"

/// @brief      Timer 기본 설정 함수
/// @param      void
/// @return     void
void __InitializeTimer(void)
{
  // Timer RA: 8bit timer/count 1ms Timer setting 
  tstart_tracr = 0;       /* Timer count start bit */
  trasr  = 0x00;          /* unuse TRAIO pin */
  traic  = 0x00;          /* Timer RA interrupt Disable */
  trasr  = 0x00;          /* Timer RA pin select register */
  traioc = 0x00;          /* Timer mode */
  tramr  = 0x00;          /* Timer mode is selected , f1 , provides count source */
  
  /* set 1ms when f1=4MHz */
  /* t=(m+1)(n+1)/f */
  /* t=(39+1)(99+1)/4MHz */
  trapre = (U8)(40 - 1);  /* Timer RA prescaler register : set to 125-1 (4MHz x f1= 0.25usec) x 40 = 10usec) */
  tra    = (U8)(10 - 1); /* Timer RA Primary Register : set to 40-1 (10usec x 10 = 100us) */
  traic  = 0x05;          /* Timer RA interrupt : Priority level-6 is selected , Interrupt request bit = clear */

  // Timer RB: 8bit timer/count 100u Timer setting 
  tstart_trbcr = 0;        /* Timer count start bit */
  trbcr  = 0x00;           /* Timer mode , count stop */
  trbocr = 0x00;           /* Timer RB one-shot is not used */
  trbioc = 0x00;           /* Timer mode */
  trbmr  = 0x00;           /* Timer mode is selected , f1 , provides count source */
  
  /* set 1ms when f1=4MHz */
  /* t=(m+1)(n+1)/f */
  /* t=(39+1)(99+1)/4MHz */
  trbpre = (U8)(250 - 1);  /* Timer RA prescaler register : set to 125-1 (4MHz x f1= 0.25usec) x 250 = 62.5usec) */
  trbpr  = (U8)(200 - 1);  /* Timer RB Primary Register : set to 80-1 (62.5usec x 200 = 15.625usec) */
  trbic  = 0x07;           /* Timer RB interrupt : Priority level-5 is selected , Interrupt request bit = clear */
 
  // [TM51]NSF-ISR5: NSF-1ms Timer INT setting & Timer Start
  tstart_tracr = 1;        /* Timer count start bit */
  // [TMH1]SF-ISR2: SF-2ms Timer INT setting & Timer Start
  tstart_trbcr = 1;        /* Timer count start bit */
}



