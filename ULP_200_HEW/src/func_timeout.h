
/// @file   func_timeout.h
/// @date   2012/02/14
/// @author jinhwa Lee
/// @brief  타임 아웃 관련 파일

#ifndef ___FUNC_TIMEOUT_H___
#define ___FUNC_TIMEOUT_H___

#define SIZE_TIME_OUT_TABLE 10


/// @brief 타임아웃을 하기 위한 구조체
typedef struct{
  U8  Assign;     ///< 할당 여부
  U8  Use;        ///< 사용 여부
  U32 TimeOutCnt; ///< 타임아웃 카운터
}typeTimeOutTable;

void InitializeTimeOut(void);

HANDLE CreateTimeOut( void );
U8 SetupTimeOut( HANDLE tHandle, U32 Time100ms );
U8 IsTimeOut ( HANDLE tHandle );
U8 CancelTimeOut(  HANDLE tHandle );
U8 IsTimeOutUse ( HANDLE tHandle );

void ThreadTimeOut(void); // 100ms

#endif  // #ifndef ___FUNC_TIMEOUT_H___

