/// @file   Lib_KdProtocol.h
/// @date   2012/03/12
/// @author jinhwa Lee
/// @brief  경동 프로토콜 라이브러리

#ifndef ___LIB_PROTOCOLKD_H___
#define ___LIB_PROTOCOLKD_H___

// 라이브러리 버전 관리
// 120312 1.0 초기 작성

// -----------------------------------------------------------------------------
// Queue -----------------------------------------------------------------------
// -----------------------------------------------------------------------------

/*
예시>
*/


  // KD Protocol //
  ///////////////////////////////////////////////////////////////////////////////////////////
  //        HEADER = 5             /           ID = 10           /    INFO = 2  / DATA ... //
  //---------------------------------------------------------------------------------------//
  // STX / DEV / SubID / CMD / Len / TXDEV / RXDEV / TXID / RXID / CHAN / VEDIO / DATA ... //
  //---------------------------------------------------------------------------------------//
  //  1  /  1  /   1   /  1  /  1  /   1   /   1   /  4   /   4  /   1  /   1   / LEN  ... //
  ///////////////////////////////////////////////////////////////////////////////////////////



#define COUNT_CHANNEL_KDP        3
#define KD_DATA_FIELD_SIZE      20

typedef struct 
{
  U8 Device;
  U8 SubID;
  U8 Command; 
  U8 Length;
} __typeKdProtocolHeader;

typedef struct
{
  U8 TxDevice;
  U8 RxDevice;
  
  U8 TxDongIDHi;
  U8 TxDongIDLo;
  U8 TxHooIDHi;
  U8 TxHooIDLo;
  
  U8 RxDongIDHi;
  U8 RxDongIDLo;
  U8 RxHooIDHi;
  U8 RxHooIDLo;
} __typeKdProtocolID;

typedef struct
{
  U8 SlotID_Channel;
  U8 Video;
} __typeKdProtocolChannel;


typedef struct
{
  __typeKdProtocolHeader  Header;
  __typeKdProtocolID      ID;
  __typeKdProtocolChannel Channel;
  U8 Data[KD_DATA_FIELD_SIZE];
} typeKdProtocol;


void Kd_Initialize(void);
void Kd_SetupChannel( U8 tChannel, U16 tBaudrate,
                      U8 tThisDevice, U8 tThisSubID, 
                      void (*tTx485Enable)(void), void (*tRx485Enable)(void),
                      void (*tTxBufferReg)(U8));

U8 Kd_TransmitData( U8 tChannel, U8 tCommand, U8 *tBuffer, U8 tLength, U8 tWaitTime );
U8 Kd_TransmitDataRawData(U8 tChannel,U8 tDevice, U8 tSubID, U8 tCommand, U8 *tBuffer, U8 tLength, U8 tWaitTime100us );
void Kd_ReceiveData( U8 tChannel, U8 tRxData );
void Kd_TimerInterrupt300us( void );



///////////////////////////////////////////////////////////////////////////////
// Protocol Command
////////////////////////////////////////////////////////////////////////////////

/* Device ID Define */
#define KD_PRT_DEVICE_MAINDEVICE            0x00    // 주장치 디바이스 아이디
#define KD_PRT_DEVICE_HOUSE                 0x01    // 세대기 디바이스 아이디
#define KD_PRT_DEVICE_LOBBY                 0x02    // 로비 디바이스 아이디
#define KD_PRT_DEVICE_GUARD                 0x03    // 경비 디바이스 아이디
#define KD_PRT_DEVICE_ALL                   0x04    // 모든 디바이스 아이디
#define KD_PRT_DEVICE_PC                    0x30    // PC

/****************************** Command Define ********************************/
/* Device Comunication */
#define KD_PRT_CMD_CALL_ON                  0x01    // 호출 시작
#define KD_PRT_CMD_CALL_OK                  0x02    // 호출 수락
#define KD_PRT_CMD_CALL_BUSY                0x03    // 통화중
#define KD_PRT_CMD_TALK_ON                  0x04    // 통화 시작
#define KD_PRT_CMD_ALL_OFF                  0x05    // 호출/통화 종료
#define KD_PRT_CMD_DIRECT_TALK_CONVERSION_TX     0x06 // 세대통화 전환 전송
#define KD_PRT_CMD_GUARD_CONVERSION_CALL_REQUEST 0x07 // 경비 전환 호출 요청
#define KD_PRT_CMD_TALK_WAIT                0x08 // 통화 대기
#define KD_PRT_CMD_CALL_BUSY_ETC            0x09 // 기타 상황 통화중
#define KD_PRT_CMD_LINE_TEST_ON             0x0F


/* Function & Error */
#define KD_PRT_CMD_DOOR_OPEN                0x11    // 문열림
#define KD_PRT_CMD_FIRE_DOOR_OPEN           0x12    // 화재 문열림
#define KD_PRT_CMD_FIRE_DOOR_CLOSE          0x13    // 화재 문닫힘
#define KD_PRT_CMD_AUTO_CALL                0x14    // 오토 콜 요청
#define KD_PRT_CMD_TEMINAL_USE_REQUEST      0x15    // 얼라이브 요청
#define KD_PRT_CMD_TEMINAL_USE_DATA_TX      0x16    // 얼라이브 응답
#define KD_PRT_CMD_TEMINAL_SETUP_TX         0x17    // 단말기 설정 조회
#define KD_PRT_CMD_TEMINAL_SETUP_INQUIRY    0x18    // 단말기 설정 전송
#define KD_PRT_CMD_TEMINAL_SETUP_CHANGE     0x19    // 단말기 설정 변경
#define KD_PRT_CMD_CHANNEL_TEST             0x1A    // 채널 TEST
#define KD_PRT_CMD_SECURITY_DATA_COM_REQUEST 0x1B    // 보안 DATA
#define KD_PRT_CMD_SECURITY_DATA_RESULT_TX  0x1C    // 보안 DATA 
#define KD_PRT_CMD_ERROR                    0x1F    // 에러

/* Event */
#define KD_PRT_CMD_OUT                      0x21    // 외출
#define KD_PRT_CMD_HOME                     0x22    // 재택
#define KD_PRT_CMD_FIRE                     0x23    // 화재
#define KD_PRT_CMD_GAS                      0x24    // 가스
#define KD_PRT_CMD_EMERGENCY                0x25    // 비상
#define KD_PRT_CMD_BURGALR1                 0x26    // 방범1
#define KD_PRT_CMD_BURGALR2                 0x27    // 방범2
#define KD_PRT_CMD_BURGALR3                 0x28    // 방범3
#define KD_PRT_CMD_BURGALR4                 0x29    // 방범4

/* Setup & Delete & Info 1 */
#define KD_PRT_CMD_EPASS_SETUP              0x31    // 각세대 P/W 설정 및 초기화
#define KD_PRT_CMD_CPASS_SETUP              0x32    // 공통 P/W 설정 및 초기화
#define KD_PRT_CMD_RF_SETUP                 0x33    // RF카드 설정 및 삭제/초기화
#define KD_PRT_CMD_PHONE_SETUP              0x34    // 전화번호 설정 및 초기화
#define KD_PRT_CMD_SECURITY_INFO_INQUIRY    0x35    // 보안 정보 조회
#define KD_PRT_CMD_SECURITY_INFO_TX         0x36    // 보안 정보 전송
#define KD_PRT_CMD_MAIN_VERSION_INQUIRY     0x37    // 주장치 버전 조회
#define KD_PRT_CMD_MAIN_VERSION_TX          0x38    // 주장치 버전 전송
#define KD_PRT_CMD_EVENT_INFO_INQUIRY       0x39    // 이벤트 정보 조회
#define KD_PRT_CMD_EVENT_INFO_DELETE        0x3A    // 이벤트 정보 삭제
#define KD_PRT_CMD_EVENT_INFO_TX            0x3B    // 이벤트 정보 전송

/* Setup & Delete & Info 2 */
#define KD_PRT_CMD_APT_SETUP                0x41    // 단지 설정
#define KD_PRT_CMD_APT_INQUIRY              0x42    // 단지 조회
#define KD_PRT_CMD_APT_INFO_TX              0x43    // 단지 정보 전송
#define KD_PRT_CMD_MAIN_INFO_SETUP          0x44    // 주장치 정보 설정 및 삭제
#define KD_PRT_CMD_MAIN_INFO_INQUIRY        0x45    // 주장치 정보 조회
#define KD_PRT_CMD_MAIN_INFO_TX             0x46    // 주장치 정보 전송
#define KD_PRT_CMD_MAIN_BACKUP_INFO_INQUIRY 0x47    // 주장치 백업 정보 조회
#define KD_PRT_CMD_MAIN_BACKUP_INFO_TX      0x48    // 주장치 백업 정보 전송
#define KD_PRT_CMD_GUARD_CON_INQUIRY        0x49    // 경비실 전환 조회
#define KD_PRT_CMD_GUARD_CON_INQUIRY_TX     0x4A    // 경비실 전환 조회 전송
#define KD_PRT_CMD_SETUP_OK                 0x4B    // 설정 OK
#define KD_PRT_CMD_DELETE_OK                0x4C    // 삭제 OK
#define KD_PRT_CMD_SETUP_FAIL               0x4D    // 설정 FAIL
#define KD_PRT_CMD_DELETE_FAIL              0x4E    // 삭제 FAIL
#define KD_PRT_CMD_INQUIRY_FAIL             0x4F    // 조회 FAIL

/* Etc */
#define KD_PRT_CMD_DONG_INFO_REQUEST        0x51    // 동 정보 요청/ 슬롯카드 버전 전송
#define KD_PRT_CMD_DONG_INFO_TX             0x52    // 동 정보 전송
#define KD_PRT_CMD_CH_INFO_REQUEST          0x53    // 채널 정보 요청
#define KD_PRT_CMD_CH_INFO_TX               0x54    // 채널 정보 전송
#define KD_PRT_CMD_MONITER_REQUEST          0x55    // 모니터 요청
#define KD_PRT_CMD_MONITER_DATA_TX          0x56    // 모니터 데이터 전송
#define KD_PRT_CMD_TIME_INQ                 0x57    // 시간 요청 
#define KD_PRT_CMD_TIME_SETUP               0x58    // 시간 전송

////////////////////////////////////////////////////////////////////////////////
U8 KDP_TransmitProtocol(U8 tId, U8 tCmd, U8 tChannel, U8 DongNoH_LobbyID, U8 DongNoL, U8 HooNoH, U8 HooNoL);
void KDP_LobbySetupChannel(void);

#endif  // #ifndef ___LIB_PROTOCOLKD_H___

