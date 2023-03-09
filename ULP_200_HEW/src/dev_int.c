
/// @file   dev_int.c
/// @date   2012/02/14
/// @author jinhwa Lee
/// @brief  Interrupt Initialize 디바이스 드라이브 파일

#include "dev_int.h"


/// @brief      인터럽트 설정 함수
/// @param      void
/// @return     void
void __InitializeInt( void )
{
  /*  int3pl : 0 = falling edge, 1 = both edge  */
  //int1pl = 0;
  int3pl = 0;

  /* Configure INT0 interrupt priority
  b2:b0   - ILVL2:ILVL0   - 010 (Set the interrupt priority to 2)
  b3    - IR       - 0   (Reset interrupt flag)
  b7:b4   - Reserved    - Set to 0000   */
  //int1ic = 0x02;
  int3ic = 0x02;

  /* Configure INT Interrupt Input Pin Select Register 
  b0    - Reserved      - Set to 0  
  b3:b1   - INT1SEL2:INT1SEL0 - 000 (Unused, keep default)
  b4    - INT2SEL0      - 0   (Unused, keep default)
  b5    - Reserved      - Set to 0
  b7:b6   - INT3SEL1:INT3SEL0 - 10  (Configure port pin 6_7 as INT3n input) */
  intsr = 0x80;

  /* Enable INT3n Interrupt for RF_INT */   
  int3en = 1;
  //int1en = 1;

  ir_int3ic = 0;
  //ir_int1ic = 0;
  
}





