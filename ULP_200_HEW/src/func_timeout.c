
/// @file   func_timeout.c
/// @date   2012/02/14
/// @author jinhwa Lee
/// @brief  타임 아웃 관련 파일


#include "main.h"

typeTimeOutTable gTimeOutTable[SIZE_TIME_OUT_TABLE]; ///< Time Out Counter


/// @brief TimeOut Initialize
/// @param  void
/// @return void
void InitializeTimeOut(void)
{
  U8 i;
  for(i = 0; i < SIZE_TIME_OUT_TABLE; i++)
  {
    gTimeOutTable[i].Assign = false;
    gTimeOutTable[i].Use = false;
    gTimeOutTable[i].TimeOutCnt= NULL;
  }
}


/// @brief 타임아웃 할당 받는 함수
/// @param  void
/// @return HANDLE 타임아웃을 관리하기 위한 헨들러 반환 
HANDLE CreateTimeOut( void )
{
  U8 i;
  HANDLE tHandle;

  for( i = 0; i < SIZE_TIME_OUT_TABLE; i++ )
  {
    if (gTimeOutTable[i].Assign == false)
    {
      gTimeOutTable[i].Assign = true;
      gTimeOutTable[i].Use = false;
      gTimeOutTable[i].TimeOutCnt = 0;
      tHandle = (U8)(i + 1);
      return tHandle;
    }
  }
  return false;
}


/// @brief 할당된 핸들러에 타임 아웃 시간 설정 하는 함수
/// @param  tHandle CreateTimeOut를 통해 할당된 헨들러
/// @param  Time100ms 타임 아웃 시간 : 100ms
/// @return true : 성공 \n false : 실패
U8 SetupTimeOut( HANDLE tHandle, U32 Time100ms )
{
  if (gTimeOutTable[tHandle - 1].Assign == true)
  {
    gTimeOutTable[tHandle - 1].Use = true;
    gTimeOutTable[tHandle - 1].TimeOutCnt = Time100ms;
    return true;
  }
  return false;
}


/// @brief 타임 아웃 잠시 멈춤
/// @param  tHandle CreateTimeOut를 통해 할당된 헨들러
/// @return true : 성공 \n false : 실패
U8 CancelTimeOut(  HANDLE tHandle )
{
  if (gTimeOutTable[tHandle - 1].Use == true)
  {
    gTimeOutTable[tHandle - 1].Use = NULL;
    gTimeOutTable[tHandle - 1].TimeOutCnt = NULL;
    return true;
  }
  return false;
}


/// @brief 타임 아웃 되었는지 검사하는 함수
/// @param  tHandle CreateTimeOut를 통해 할당된 헨들러
/// @return true : 성공 \n false : 실패
U8 IsTimeOut ( HANDLE tHandle )
{
  if (gTimeOutTable[tHandle - 1].Use == true)
  {
    if( gTimeOutTable[tHandle - 1].TimeOutCnt == NULL )
    {
      gTimeOutTable[tHandle - 1].Use = false;
      return true;
    }
  }
  return false;
}

/// @brief  핸들러가 사용되고 있는지 (TimeOut 동작) 확인
/// @param  tHandle CreateTimeOut를 통해 할당된 헨들러
/// @return true : 성공 \n false : 실패
U8 IsTimeOutUse ( HANDLE tHandle )
{
  return gTimeOutTable[tHandle - 1].Use;
}


/// @brief  TimeOut을 위한 스레드
/// @brief  100ms 마다 실행되어 타임 아웃 카운트를 한다.
/// @param  void
/// @return void
void ThreadTimeOut(void) // 100ms
{
  U8 i;
  wdt_reset();

  for(i = 0; i < SIZE_TIME_OUT_TABLE; i++)
  {
    if ( gTimeOutTable[i].TimeOutCnt != NULL)
    {
      gTimeOutTable[i].TimeOutCnt--;
    }
  }
}




