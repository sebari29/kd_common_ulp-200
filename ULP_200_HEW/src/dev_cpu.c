
/// @file   dev_cpu.c
/// @date   2012/02/14
/// @author jinhwa Lee
/// @brief  delay, Whatchdog, InitializeClock ���� ����̽� �帮�̺� ����


#include "dev_cpu.h"

/// @brief      1msec ���� ������ �Լ�
/// @param[in]  tTime ������ �ϰ��� �ϴ� �ð�
/// @return     void
void __delay_ms(U16 tTime)
{
  U16 x;
  while(tTime) 
  {
    x = 200;        //for 1msec
    __wdt_reset();
    while(x) --x;
    --tTime;
  }
}


/// @brief  Whatchdog �ʱ�ȭ �Լ�
/// @param  void
/// @return void
void __wdt_init(void)
{
  prc1 = 1;               // protect disable
  pm12 = 1;               // wdt reset
  prc1 = 0;               // protect enable
  cspro = 0;              // cpu clock source
  //asm("FCLR I");        //global interrupt disable  
  //cspro = 1;
  wdtc7 = 1;              //128 prescaler
  //wdt_reset();          // watchdog timer reset
  wdts = 0xff;            // start
  //asm("FSET I");        //global interrupt enable
}


/// @brief  Clock �ʱ�ȭ �Լ�
/// @param  void
/// @return void
void __InitializeClock(void)  // cpu clock : 4MHz setting
{
  U16 i = 0;
  
  prc0 = 1;          /*system control resister Protect off*/
  cm13 = 1;          /*select XIN-XOUT pin*/
  cm05 = 0;          /*XIN Clock start*/
  while(i < 2000)
  {
    i++;          /* Clock stability */
  }
  ocd2 = 0;          /*select XIN clock*/
  cm07 = 0;          /*system clock XIN*/
  prc0 = 0;          /*system control resister Protect on*/
}


