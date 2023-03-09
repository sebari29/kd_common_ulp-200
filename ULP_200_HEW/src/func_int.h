
/// @file   func_int.h
/// @date   2012/02/14
/// @author jinhwa Lee
/// @brief  Interrupt 관련 파일



#ifndef ___FUNC_INT_H___
#define ___FUNC_INT_H___

#include "dev_int.h"

#define ENABLE_IRQ()    __ENABLE_IRQ()
#define DISABLE_IRQ()   __DISABLE_IRQ()

void InitializeInt( void );

#endif  // #ifndef ___FUNC_INT_H___

