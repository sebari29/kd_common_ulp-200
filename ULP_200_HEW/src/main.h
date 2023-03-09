/// @file   main.h
/// @date   2012/02/14
/// @author jinhwa Lee
/// @brief  ���α׷��� �����ϴ� Main ����

#ifndef ___MAIN_H___
#define ___MAIN_H___

#include "dev_define.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "func_port.h"
#include "func_fnd.h"
#include "func_console.h"
#include "func_timer.h"
#include "func_protocol.h"
#include "func_uart.h"
#include "func_key.h"
#include "func_sound.h"
#include "func_message.h"
#include "func_console.h"
#include "func_etc.h"
#include "func_iic.h"
#include "func_int.h"
#include "func_cpu.h"
#include "func_factory.h"
#include "func_call_manager.h"
#include "func_timeout.h"
#include "lib_protocolAnam.h"
#include "lib_protocolKD.h"
#include "lib_i2c.h"


#define THREAD_KEY_SCAN_TIME          10
#define THREAD_DOOR_OPEN_TIME         540
//#define THREAD_WDT_RESET_TIME       10
#define THREAD_ANALYSIS_PROTOCOL_TIME 50
#define THREAD_TIME_OUT_TIME          100
#define THREAD_CONSOLE_TIME           100
#define THREAD_RFCARD_TIME            130
#define THREAD_FACTORY_MODE_TIME      10

//#define __wdt_reset() {wdtr = 0x00;wdtr = 0xff;}

#define MAX_DongNumber    9999
#define MIN_DongNumber    1

#define MAX_LobbyID       32
#define MIN_LobbyID       1

#define MAX_GuardID       4
#define MIN_GuardID       1

#define MAX_Password      10000
#define MIN_Password      0

#define MAX_MaxHo         32
#define MIN_MaxHo         1

#define MAX_OpenTimeDoor  60
#define MIN_OpenTimeDoor  1

#define MAX_Protocol      4
#define MIN_Protocol      1

#define MAX_AudioPath     1
#define MIN_AudioPath     0

#define MAX_BeepSoundMute 1
#define MIN_BeepSoundMute 0

#define MAX_ManagerPassword 9999
#define MIN_ManagerPassword 0

#define MAX_HOUSE_ID      127


/// @brief  �κ����� �⺻�� �Ǵ� ������ ����
/// *** �ڷ��� ¦�� ���߾�� �� ��!!!!!!!!!!!!!!!!!!!
typedef struct
{
  U16 DongNumber;       ///< ����ȣ
  U8  LobbyID;          ///< �κ� ID
  U8  MaxHo;            ///< �ִ� ȣ�� (����)
  U16 Password;         ///< ��� ��ȣ
  U16 ManagerPassword;
  U8 GuardID;           ///< ���� ID
  U8 OpenTimeDoor;      ///< ������ �ð�
  U8 BeepSoundMute;     ///< Sound Mute
  U8 Protocol;          ///< Protocol ���� \n 1:HD, 2:KD
  U8 Password_ErrCount; ///< ��й�ȣ ���� ī����
  U8 PacketMonitoring;  ///< Packet Monitering Mode
  U8 AudioPathHouse[2]; ///< House Path ����
  U8 AudioPathLobby[2]; ///< Lobby Path ����
  U8 AudioPathGuard[2]; ///< Guard Path ����
  U16 B_Lobby_Mark;
}typeLobbyHandler;


extern typeLobbyHandler gLobbyHandle;

#endif  /// #ifndef ___MAIN_H___

