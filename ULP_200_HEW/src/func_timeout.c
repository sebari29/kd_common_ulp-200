
/// @file   func_timeout.c
/// @date   2012/02/14
/// @author jinhwa Lee
/// @brief  Ÿ�� �ƿ� ���� ����


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


/// @brief Ÿ�Ӿƿ� �Ҵ� �޴� �Լ�
/// @param  void
/// @return HANDLE Ÿ�Ӿƿ��� �����ϱ� ���� ��鷯 ��ȯ 
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


/// @brief �Ҵ�� �ڵ鷯�� Ÿ�� �ƿ� �ð� ���� �ϴ� �Լ�
/// @param  tHandle CreateTimeOut�� ���� �Ҵ�� ��鷯
/// @param  Time100ms Ÿ�� �ƿ� �ð� : 100ms
/// @return true : ���� \n false : ����
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


/// @brief Ÿ�� �ƿ� ��� ����
/// @param  tHandle CreateTimeOut�� ���� �Ҵ�� ��鷯
/// @return true : ���� \n false : ����
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


/// @brief Ÿ�� �ƿ� �Ǿ����� �˻��ϴ� �Լ�
/// @param  tHandle CreateTimeOut�� ���� �Ҵ�� ��鷯
/// @return true : ���� \n false : ����
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

/// @brief  �ڵ鷯�� ���ǰ� �ִ��� (TimeOut ����) Ȯ��
/// @param  tHandle CreateTimeOut�� ���� �Ҵ�� ��鷯
/// @return true : ���� \n false : ����
U8 IsTimeOutUse ( HANDLE tHandle )
{
  return gTimeOutTable[tHandle - 1].Use;
}


/// @brief  TimeOut�� ���� ������
/// @brief  100ms ���� ����Ǿ� Ÿ�� �ƿ� ī��Ʈ�� �Ѵ�.
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




