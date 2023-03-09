
/// @file   func_timer.c
/// @date   2012/02/14
/// @author jinhwa Lee
/// @brief  타밍 스케쥴러 관련 파일

#include "main.h"

U16 gsTimerCnt = 0;  ///< 글로벌 타이머 카운터 변수 : 1ms

typeTimeScheduleStruct gtTimeSceduleHandler; ///< 글로벌 타임 스케쥴러 변수


/// @brief  Time Scheduler Initialize
/// @param  void
/// @return void
void InitializeTimeScheduler( void )
{
  U8 i;
  for ( i= 0 ; i<MAX_FUCTION_POINTER_CNT ; i++ )
  {
    gtTimeSceduleHandler.bFuctionTime[i] = 0;
    gtTimeSceduleHandler.bFuctionTimeCnt[i] = 0;
    gtTimeSceduleHandler.pFuctionPointer[i] = 0;
  }
}

/// @brief  While 문안에서 실행되며 실제 함수를 호출해 주는 함수
/// @param  void
/// @return void
void GoTimeScheduler( void )
{
  U8 i;
  U16 tTimerCnt;

  if( gsTimerCnt == 0 ) return;
  
  tTimerCnt = gsTimerCnt;
  gsTimerCnt = 0;
  
  for( i = 0 ; i < MAX_FUCTION_POINTER_CNT ; i++ )
  {
    if( gtTimeSceduleHandler.bFuctionTime[i] != 0 )
    {
      gtTimeSceduleHandler.bFuctionTimeCnt[i] += tTimerCnt;
      if( gtTimeSceduleHandler.bFuctionTimeCnt[i] > gtTimeSceduleHandler.bFuctionTime[i] )
      {
        gtTimeSceduleHandler.bFuctionTimeCnt[i] -= gtTimeSceduleHandler.bFuctionTime[i];
        gtTimeSceduleHandler.pFuctionPointer[i]();
      }
    }
  }

}


/// @brief  스케쥴러에 함수를 등록 하는 함수
/// @param[in]  *tFuction 등록할 함수 포인터
/// @param[in]  tTimer 실행할 주기 ( 시간 ) , 1msec
/// @return void
void SetupTimeScheduler( void (*tFunction)(void), U16 tTimer )
{
  U8 i;
  for( i = 0 ; i < MAX_FUCTION_POINTER_CNT ; i++ )
  {
    if( gtTimeSceduleHandler.pFuctionPointer[i] == 0 ){
      gtTimeSceduleHandler.pFuctionPointer[i] = tFunction;
      gtTimeSceduleHandler.bFuctionTime[i] = tTimer;
      return;
    }
  }
}


/// @brief  등록된 함수의 호출을 정지 하는 함수
/// @param[in]  *tFuction 정지할 함수 포인터
/// @return void
void StopTimeScheduler( void (*tFunction)(void))
{
  U8 i;
  for( i = 0 ; i < MAX_FUCTION_POINTER_CNT ; i++ )
  {
    if( gtTimeSceduleHandler.pFuctionPointer[i] == tFunction ){
      gtTimeSceduleHandler.bFuctionTime[i] = 0;
      return;
    }
  }
}

/// @brief  정지된 함수의 호출을 다시 시작 하는 함수
/// @brief  함수의 호출 시간을 변경하고자 할 대도 사용됨
/// @param[in]  *tFuction 시작할 함수 포인터
/// @param[in]  tTimer 실행할 주기 ( 시간 ) , 1msec
/// @return void
void StartTimeScheduler( void (*tFunction)(void), U16 tTimer )
{
  U8 i;
  for( i = 0 ; i < MAX_FUCTION_POINTER_CNT ; i++ )
  {
    if( gtTimeSceduleHandler.pFuctionPointer[i] == tFunction ){
      gtTimeSceduleHandler.bFuctionTime[i] = tTimer;
      return;
    }
  }

}

/// @brief  Timer Initialize
/// @param  void
/// @return void
void InitializeTimer(void)
{
  __InitializeTimer();
}




