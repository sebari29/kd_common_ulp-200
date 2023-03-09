
/// @file   dev_int.h
/// @date   2012/02/14
/// @author jinhwa Lee
/// @brief  Interrupt Initialize 디바이스 드라이브 파일

#ifndef ___DEV_INT_H___
#define ___DEV_INT_H___

#include "dev_define.h"

#define __ENABLE_IRQ()  {_asm(" FSET I");}  ///< Global Interrupt Enable
#define __DISABLE_IRQ()  {_asm(" FCLR I");} ///< Global Interrupt Disable

void __InitializeInt( void );


#endif  // #ifndef ___DEV_INT_H___

