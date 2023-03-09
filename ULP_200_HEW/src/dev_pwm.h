
/// @file   dev_pwm.h
/// @date   2012/02/14
/// @author jinhwa Lee
/// @brief  PWM 관련 설정 디바이스 드라이브 파일


#ifndef ___DEV_PWM_H___
#define ___DEV_PWM_H___

#include "dev_define.h"

#define __TMH1_PWM_ENABLE()    trcmr = (U8)((trcmr | 0x80) & 0xbf)
#define __TMH1_PWM_DISABLE()   trcmr = (U8)((trcmr & 0x7F) & 0xbf)
void __InitializePWM(void);


#endif  // #ifndef ___DEV_PWM_H___a
