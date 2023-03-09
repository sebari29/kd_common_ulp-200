
/// @file   func_timer.h
/// @date   2012/02/14
/// @author jinhwa Lee
/// @brief  Ÿ�� �����췯 ���� ����

#ifndef ___FUNC_TIMER_H___
#define ___FUNC_TIMER_H___


#include "dev_timer.h"

#define MAX_FUCTION_POINTER_CNT 15

/// @brief  �����췯 ���� �ڷ���
typedef struct {
  void (*pFuctionPointer[MAX_FUCTION_POINTER_CNT])(void); ///< �Լ� ������ �Է�
  U16 bFuctionTimeCnt[MAX_FUCTION_POINTER_CNT];           ///< �Լ� ���� �ֱ� ī���� \n ī��Ʈ �Ǹ鼭 �ֱ������� �Լ��� ������
  U16 bFuctionTime[MAX_FUCTION_POINTER_CNT];              ///< �Լ� ���� �ֱ� ����� \n bFuctionTimeCnt = bFuctionTime �� ���µ�
}typeTimeScheduleStruct;


void InitializeTimer(void);
void InitializeTimeScheduler( void );
void GoTimeScheduler( void );
void SetupTimeScheduler( void (*tFunction)(void), U16 tTimer );
void StopTimeScheduler( void (*tFunction)(void) );
void StartTimeScheduler( void (*tFunction)(void), U16 tTimer );

extern U16 gsTimerCnt;

#endif  // #ifndef ___FUNC_TIMER_H___

