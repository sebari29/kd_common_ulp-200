
/// @file   func_message.h
/// @date   2012/02/14
/// @author jinhwa Lee
/// @brief  큐 / 메시지 통신 관련 파일



#ifndef ___FUNC_MESSAGE_H___
#define ___FUNC_MESSAGE_H___

#include "hd_command.h"

#define MSG_ID_MsgReceiveProtocol             0
#define MSG_ID_ThreadSlaveManagerHouse        1
#define MSG_ID_ThreadSlaveManagerMasterLobby  2
#define MSG_ID_ThreadSlaveManagerSubLobby     3
#define MSG_ID_ThreadSlaveManagerGuard        4
#define MSG_ID_ThreadMainManager              5
#define MSG_ID_ThreadAnalysisProtocol         6
#define MSG_ID_ThreadDoorOpen                 7
#define MSG_ID_KeyMainFunction                8
#define MSG_ID_ThreadConsole                  9
#define MSG_ID_ThreadRFCard                   10
#define MSG_ID_ScanAllHouses                  11

#define MSG_CMD_SM_CALL_REQUEST            0x11
#define MSG_CMD_MS_CALL_REQUEST_BUSY       0x19
#define MSG_CMD_MS_CALL_REQUEST_OFF_HOOK   0x0A

#define MSG_CMD_INTERNAL_CALL              0x04
#define MSG_CMD_EXTERNAL_CALL              0x05
#define MSG_CMD_RECEIVE_PROTOCOL           0x06
#define MSG_CMD_MASTER_CALL_REQUEST        0x08
#define MSG_CMD_MASTER_CALL_REQUEST_END    0x09


#define MSG_CMD_DOOR_OPEN                  0x90
#define MSG_CMD_DOOR_OPEN_FIRE             0x91

#define MSG_CMD_RF_CARD                    0xA0

//////////////////////////////////////////////

#define GL_CBUSY       HD__GL_CBUSY
#define GL_DOPEN       HD__GL_DOPEN
//#define GL_LBUSY       HD__GL_LBUSY
#define GL_OFF_HOOK    HD__GL_OFF_HOOK
#define GL_ON_HOOK     HD__GL_ON_HOOK

#define GR_CALL_OFF    HD__GR_CALL_OFF
#define GR_CALL_OK     HD__GR_CALL_OK
#define GR_CALL_ON     HD__GR_CALL_ON
#define GR_CBUSY       HD__GR_CBUSY
#define GR_HOLD_ON     HD__GR_HOLD_ON
#define GR_HOLD_OFF    HD__GR_HOLD_OFF
#define GR_INT_COM     HD__GR_INT_COM
#define GR_LBUSY       HD__GR_LBUSY
#define GR_OFF_HOOK    HD__GR_OFF_HOOK
#define GR_ON_HOOK     HD__GR_ON_HOOK
#define INTC_ON_HOOK   HD__INTC_ON_HOOK

#define LG_CALL_OFF    HD__LG_CALL_OFF
#define LG_CALL_OK     HD__LG_CALL_OK
#define LG_CALL_ON     HD__LG_CALL_ON
#define LG_ON_HOOK     HD__LG_ON_HOOK
#define LR_CALL_OFF    HD__LR_CALL_OFF
#define LR_CALL_OK     HD__LR_CALL_OK
#define LR_CALL_ON     HD__LR_CALL_ON
#define LR_ON_HOOK     HD__LR_ON_HOOK
#define RG_CALL_OFF    HD__RG_CALL_OFF
#define RG_CALL_OK     HD__RG_CALL_OK
#define RG_CALL_ON     HD__RG_CALL_ON
#define RG_CBUSY       HD__RG_CBUSY
#define RG_LBUSY       HD__RG_LBUSY
#define RG_OFF_HOOK    HD__RG_OFF_HOOK
#define RG_ON_HOOK     HD__RG_ON_HOOK
#define RL_CBUSY       HD__RL_CBUSY
#define RL_DOPEN       HD__RL_DOPEN
//#define RL_LBUSY       HD__RL_LBUSY
#define RL_OFF_HOOK    HD__RL_OFF_HOOK
#define RL_ON_HOOK     HD__RL_ON_HOOK
#define ___ON_HOOK     HD___ON_HOOK
#define RL_CALL_REQUEST HD__RL_CALL_REQUEST
#define LR_ECHO         HD__LR_ECHO

#define RG_FIRE        HD__RG_FIRE                //tx, room fire alarm
#define RG_GAS         HD__RG_GAS                 //tx, room gas alarm
#define RG_BURG1       HD__RG_BURG1               //tx, room burglar1 alarm
#define RG_EMER        HD__RG_EMER                //S-rx/tx, room/sub-phone emergency alarm
#define RG_EXIT_ON     HD__RG_EXIT_ON             //tx, room exit on
#define RG_EXIT_OFF    HD__RG_EXIT_OFF            //tx, room exit off
#define RG_HOLD_OFF    HD__RG_HOLD_OFF            //tx, room hold release by on-hk
#define RG_EXIT        HD__RG_EXIT                //tx, room exit by guard calling
#define RG_FIRE_CNCL   HD__RG_FIRE_CNCL           //tx, room fire alarm cancel(RG_FIRE_END)  
#define RG_GAS_CNCL    HD__RG_GAS_CNCL            //tx, room gas alarm cancel(RG_GAS_END)
#define RG_BURG1_CNCL  HD__RG_BURG1_CNCL          //tx, room burglar1 alarm cancel(RG_BURG1_END)
#define RG_BURG2       HD__RG_BURG2               //tx, room burglar2 alarm
#define RG_BURG2_CNCL  HD__RG_BURG2_CNCL          //tx, room burglar2 alarm cancel(RG_BURG2_END) 
#define RG_EMER_CNCL   HD__RG_EMER_CNCL           //tx/S-tx, room emergency alarm cancel 

#define GR_CHK         HD__GR_CHK                 //rx, guard test room
#define GR_CHK_OK      HD__GR_CHK_OK              //tx, room test acknowledge

/*
#define S_LG_CALL_ON  (LG_CALL_ON + 0x80)
#define S_LG_CALL_OK  (LG_CALL_OK + 0x80)
#define S_GL_CBUSY    (GL_CBUSY + 0x80)
#define S_LG_CALL_OFF (LG_CALL_OFF + 0x80)
#define S_GL_OFF_HOOK (GL_OFF_HOOK + 0x80)
#define S_GL_ON_HOOK  (GL_ON_HOOK + 0x80)
#define S_LG_ON_HOOK  (LG_ON_HOOK + 0x80)
#define S_GL_DOPEN    (GL_DOPEN + 0x80)
//#define S_GL_LBUSY    GL_LBUSY
*/


/* ***************************************** */
/* ************** RingBuffer *************** */
/* ***************************************** */


/// @brief 링버퍼 Struct
typedef struct
{
  U8 WritePointer;  ///< Write pointer
  U8 ReadPointer;   ///< Read pointer
  U8 BufferLength;  ///< Buffer Length
  U8 Lock;          ///< Lock
  U8 *Data;         ///< Data
} typeCircularBuffer;

void InitializeCircularBuffer( typeCircularBuffer* pQue, U8* tData, U8 tBufferLength );
U8 EnqueCircularBuffer( typeCircularBuffer* que, U8 tdata );
U8 DequeCircularBuffer( typeCircularBuffer* que, U8* tdata);
U8 IsFullCircularBuffer( typeCircularBuffer* que );
U8 IsEmptyCircularBuffer( typeCircularBuffer* que );


/**********************************************/
/*************** message queue ****************/
/**********************************************/


#define MSG_BOX_LENGTH    10
#define MSG_BUFFER_LENGTH 20

#define MSG_NULL 0x00

/// @brief 메시지 박스 데이터 유니온
typedef union{
  U8  Byte[MSG_BUFFER_LENGTH];   ///< Byte 배열
  U16 Word[MSG_BUFFER_LENGTH/2]; ///< Word 배열
} typeMsgBuffer;

/// @brief 메시지 방스 Struct
typedef struct 
{
  U8  DstID;          ///< 보내는 ID
  U8  SrcID;          ///< 받는 ID
  U8  Cmd;            ///< 명령어
  typeMsgBuffer Buf;  ///< 메시지 박스 데이터
} typeMsgBox;


void InitializeMessageBox(void);
U8 bSendMessageBox( typeMsgBox* tSendMsg );
//U8 bSendMessageBoxData( U8 tDstID, U8 tSrcID, U8 tCmd, U8 tMsgData0, U8 tMsgData1, U8 tMsgData2, U8 tMsgData3 );
U8 bReceiveMessageBox( U8 ID, typeMsgBox* tMsgBox );


#endif  // #ifndef ___FUNC_MESSAGE_H___

