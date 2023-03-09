
/// @file   dev_pwm.c
/// @date   2012/02/14
/// @author jinhwa Lee
/// @brief  PWM 관련 설정 디바이스 드라이브 파일


#include "dev_pwm.h"


/// @brief      PWM 기본 설정 함수
/// @param      void
/// @return     void
void __InitializePWM(void)
{

   msttrc = 0;
   trbrcsr = 0x00;
   trcpsr0 = 0x00;
  
   trcpsr1 = 0x02;    /* TRCIOC pin:P3_4 */
  
   trcic = 0x00;      /* Interrupt disabled */
  
   trcadcr = 0x00;    /* A/D trigger disabled */
   trcmr = 0x08;      /*PWM mode */
   asm(" ");          /* Description for preventing the abbreviation from optimization */
   trcmr = 0x0A;  

   trccr1 = 0xB0;     // f/8분주
   trccr2 = 0x02;     /* TRDIOB output level:"H" active */

   trc = 0x0000;      /* The Count value of TRC is initialized to "0" */
   trcgra = 0;        /* period : 50ns * 2000 = 100us */
   trcgrb = 0;        /* Low width : 50ns *  500 =  25us */
   trcgrc = 0;        /* Low width : 50ns * 1000 =  50us */
   trcgrd = 0;        /* Low width : 50ns * 1500 =  75us */

   trcoer = 0x7B;     /* TRCIOA Pin Output disabled */

   trcsr &= 0x8e;     /* IMFA flag clear */
   //trcier = 0x01;   /* Enable interrupts ,imiea */
   //trcic = 0x01;    /* Timer RC intterupt enabled(Level1) */
   trcmr = 0x8A;
   //TMH1_PWM_ENABLE();

}



