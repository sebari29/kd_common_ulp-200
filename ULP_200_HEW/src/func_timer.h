
/// @file   func_timer.h
/// @date   2012/02/14
/// @author jinhwa Lee
/// @brief  타밍 스케쥴러 관련 파일

#ifndef ___FUNC_TIMER_H___
#define ___FUNC_TIMER_H___


#include "dev_timer.h"

#define MAX_FUCTION_POINTER_CNT 15

/// @brief  스케쥴러 관리 자료형
typedef struct {
  void (*pFuctionPointer[MAX_FUCTION_POINTER_CNT])(void); ///< 함수 포인터 입력
  U16 bFuctionTimeCnt[MAX_FUCTION_POINTER_CNT];           ///< 함수 실행 주기 카운터 \n 카운트 되면서 주기적으로 함수를 실행함
  U16 bFuctionTime[MAX_FUCTION_POINTER_CNT];              ///< 함수 실행 주기 저장소 \n bFuctionTimeCnt = bFuctionTime 로 리셋됨
}typeTimeScheduleStruct;


void InitializeTimer(void);
void InitializeTimeScheduler( void );
void GoTimeScheduler( void );
void SetupTimeScheduler( void (*tFunction)(void), U16 tTimer );
void StopTimeScheduler( void (*tFunction)(void) );
void StartTimeScheduler( void (*tFunction)(void), U16 tTimer );

extern U16 gsTimerCnt;

#endif  // #ifndef ___FUNC_TIMER_H___

