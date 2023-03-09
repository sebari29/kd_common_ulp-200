
/// @file   func_timer.c
/// @date   2012/02/14
/// @author jinhwa Lee
/// @brief  Ÿ�� �����췯 ���� ����

#include "main.h"

U16 gsTimerCnt = 0;  ///< �۷ι� Ÿ�̸� ī���� ���� : 1ms

typeTimeScheduleStruct gtTimeSceduleHandler; ///< �۷ι� Ÿ�� �����췯 ����


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

/// @brief  While ���ȿ��� ����Ǹ� ���� �Լ��� ȣ���� �ִ� �Լ�
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


/// @brief  �����췯�� �Լ��� ��� �ϴ� �Լ�
/// @param[in]  *tFuction ����� �Լ� ������
/// @param[in]  tTimer ������ �ֱ� ( �ð� ) , 1msec
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


/// @brief  ��ϵ� �Լ��� ȣ���� ���� �ϴ� �Լ�
/// @param[in]  *tFuction ������ �Լ� ������
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

/// @brief  ������ �Լ��� ȣ���� �ٽ� ���� �ϴ� �Լ�
/// @brief  �Լ��� ȣ�� �ð��� �����ϰ��� �� �뵵 ����
/// @param[in]  *tFuction ������ �Լ� ������
/// @param[in]  tTimer ������ �ֱ� ( �ð� ) , 1msec
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




