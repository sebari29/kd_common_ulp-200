
/// @file   func_key.h
/// @date   2012/02/14
/// @author jinhwa Lee
/// @brief  Key 입력 관련 파일

#ifndef ___FUNC_KEY_H___
#define ___FUNC_KEY_H___

#define STATUS_ON     1
#define STATUS_OFF    0

#define SCAN_LINE_X   4
#define SCAN_LINE_Y   4

#define SAMPLING_TIME 10  // 10ms
#define RELEASE_TIME  40  // 40ms
#define KEY_RELEASE   RELEASE_TIME/SAMPLING_TIME

#define KEY_TIMEOUT_CNT 150

#define KEY_STAR      11
#define KEY_SHARP     10
#define KEY_GUARD     12
#define KEY_CALL      13

#define KEY_DATA_LENGTH 8

#define PASSWORD_ADDR   0x04
#define KEY_COUNT_MAX   4

#define ADMIN_CODE          0x15264859
#define ADMIN_MANAGER_PASS_MARK   0x001C  



#define ADMIN_DONG_NUMBER               11
#define ADMIN_LOBBY_ID                  12
//#define ADMIN_VIDEOLINE               13      

#define ADMIN_DOOR_TIME                 21
#define ADMIN_SOUND_MUTE                22
      
#define ADMIN_RFCARD_MASTER             41
//#define ADMIN_RFCARD_HOME_DEL         42
#define ADMIN_RFCARD_ALL_DEL            43
      
#define ADMIN_PROTOCOL                  51
#define ADMIN_GUARD_ID                  52
#define ADMIN_MAX_HOO                   53
      
#define ADMIN_HOME_PASSWORD_CHANGE      91
#define ADMIN_COMMON_PASSWORD_CHANGE    92
#define ADMIN_MANAGER_PASSWORD_CHANGE   93

#define ADMIN_HOME_PASSWORD_ALL_DEL     96
      
#define ADMIN_FW_VERSION_CHECK          99




#define KEY_MODE_PASSWORD_OK_MANAGER    1
#define KEY_MODE_PASSWORD_OK_ADMIN      2
#define KEY_MODE_PASSWORD_JOIN_MANAGER  3  ///Ver 1.6


#define KEY_MODE_NORMAL                 0
//#define KEY_MODE_DONG_CALL            1
#define KEY_MODE_HOME_NUM_INPUT         2
#define KEY_MODE_HOME_PASS_INPUT        3
#define KEY_MODE_MENU_INPUT             4
#define KEY_MODE_SETTING                5
#define KEY_MODE_PASSWORD_CHANGE        6
#define KEY_MODE_CHANGE_HOME_NUM_INPUT  7




/// @brief 키 스켄에 필요한 구조체
typedef struct{
  U32 RealData;     ///< 입력된 키 값이 누적 되어 저장 되는 장소
  U8  KeyCnt;       ///< 입력된 키 값의 갯수
  U16 DisplayData;  ///< FND에 표현 되어야할 데이터
  U16 TimeCnt;      ///< Key Time Out에 사용 되는 변수
  U8  Lock;         ///< [*] 키를 제외한 키 잠금 변수
  U8  Mode;         ///< 키입력 모드
  U8  SettingSwitch;///< 설정 모드
  U16 HooNumber;
  U16 KeyShadow;    ///< [***##] 감지를 위하여
}typeKeyScan;

void ThreadKeyScan(void);
void ExeSwRoutine(U8 swdata, U8 status);
void KeyDataClear(void);
//void MsgDoorOpenfunc(void);
U8 SettingValueSave(void);
void InitializeKeyScan(void);

void KeyLockOn(void);
void KeyLockOff(void);
void KeyMainFunction( U8 tKeyMap );


#endif  // #ifndef ___FUNC_KEY_H___

