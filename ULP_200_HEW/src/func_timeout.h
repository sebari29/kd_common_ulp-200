
/// @file   func_timeout.h
/// @date   2012/02/14
/// @author jinhwa Lee
/// @brief  Ÿ�� �ƿ� ���� ����

#ifndef ___FUNC_TIMEOUT_H___
#define ___FUNC_TIMEOUT_H___

#define SIZE_TIME_OUT_TABLE 10


/// @brief Ÿ�Ӿƿ��� �ϱ� ���� ����ü
typedef struct{
  U8  Assign;     ///< �Ҵ� ����
  U8  Use;        ///< ��� ����
  U32 TimeOutCnt; ///< Ÿ�Ӿƿ� ī����
}typeTimeOutTable;

void InitializeTimeOut(void);

HANDLE CreateTimeOut( void );
U8 SetupTimeOut( HANDLE tHandle, U32 Time100ms );
U8 IsTimeOut ( HANDLE tHandle );
U8 CancelTimeOut(  HANDLE tHandle );
U8 IsTimeOutUse ( HANDLE tHandle );

void ThreadTimeOut(void); // 100ms

#endif  // #ifndef ___FUNC_TIMEOUT_H___

