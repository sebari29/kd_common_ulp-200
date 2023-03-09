
/// @file func_call_manager_sublobby.c
/// @date 2012/02/14
/// @author jinhwa Lee
/// @brief  통화 관리 모듈 4 - 서브 로비 관련 함수

#include "main.h"


extern typeInfoTalk mInfoTalk[INFO_TALK_MAX];
extern HANDLE mhCallTimeOut[MAX_CHANNEL];
extern typeLineResource    mLineResource;


/// @brief ID가 서브로비 일때 [마스터로비->세대] 프로토콜 중계 함수
/// @param tPrtBuf 프로토콜 데이터
/// @param tIndex 사용중인 통화 체널
/// @return true = 성공 \n false = 실패
U8 CallManagerRepeater_MLR_SubLobby( typeProtocolData *tPrtBuf , U8 tIndex )
{
  U8 tID,tCmd,tPath,tLobbyID,tHooNoH,tHooNoL;
  U8 tDongH,tDongL;
  U8 tCallEndFlg = false;
  typeUserCallResource tUserCallResource = NULL;
  typeMsgBox tMsgBuffer;
  
  switch(tPrtBuf->Cmd)
  {
    case LR_CALL_ON:
      if (mInfoTalk[tIndex].Status != INFO_TALK_STATUS_CALL_OK) return false;
      tPath = NULL;//mInfoTalk[tIndex].DistPath;
      tCmd = tPrtBuf->Cmd;
      tLobbyID = (U8)(mInfoTalk[tIndex].LobbyID | 0x80);
      tHooNoH = mInfoTalk[tIndex].HooNoH;
      tHooNoL = mInfoTalk[tIndex].HooNoL;
      tDongH = mInfoTalk[tIndex].DongNoH;
      tDongL = mInfoTalk[tIndex].DongNoL;
      TransmitProtocolLobbySwitch( tLobbyID , tCmd, tPath, tDongH, tDongL, tHooNoH, tHooNoL );
      SetupTimeOut( mhCallTimeOut[tIndex], CALL_OK_TIME_OUT_3SEC );
      sound_effect( MAIN_BELL, 10 );
      /*
      if ( gLobbyHandle.Protocol == PSS_HT_PROTOCOL )      
      {
        mInfoTalk[tIndex].Status = INFO_TALK_STATUS_CALL;
        SetupTimeOut( mhCallTimeOut[tIndex], CALL_TIME_OUT_SLAVE );
      } 
      */
      break;
    case LR_CALL_OK:
      if (mInfoTalk[tIndex].Status != INFO_TALK_STATUS_CALL_OK) return false;
      mInfoTalk[tIndex].Status = INFO_TALK_STATUS_CALL;
      SetupTimeOut( mhCallTimeOut[tIndex], CALL_TIME_OUT_30SEC );
      break;
    case RL_CBUSY: 
  //case RL_LBUSY:
      if ( (mInfoTalk[tIndex].Status != INFO_TALK_STATUS_CALL_OK) && (mInfoTalk[tIndex].Status != INFO_TALK_STATUS_CALL)) return false;          
      tCallEndFlg = true;
      sound_effect( BUSY_TONE, 1 );
      break;
    case RL_OFF_HOOK:
      if (mInfoTalk[tIndex].Status != INFO_TALK_STATUS_CALL) return false;
      mInfoTalk[tIndex].Status = INFO_TALK_STATUS_TALK;
      tUserCallResource.Channel = CHANNEL_1;
      tUserCallResource.PathName[PATH_1] = mInfoTalk[CHANNEL_1].PathName[PATH_1];
      tUserCallResource.PathName[PATH_2] = mInfoTalk[CHANNEL_1].PathName[PATH_2];
      tUserCallResource.PathNumber[PATH_1] = 1;
      tUserCallResource.PathNumber[PATH_2] = (U8)((tPrtBuf->Path) & 0x0F);
      
      mInfoTalk[CHANNEL_1].PathNumber[PATH_1] = CHANNEL_1;
      mInfoTalk[CHANNEL_1].PathNumber[PATH_2] = tUserCallResource.PathNumber[PATH_2];

      mLineResource.MasterLobby = true;
      mLineResource.SubLobby[tUserCallResource.PathNumber[PATH_2]-1]= true;

      SelectCallMux( &tUserCallResource );
      SelectCameraMic();
      SelectRelay();

      sound_effect( NO_SOUND, 1 );
      SetupTimeOut( mhCallTimeOut[tIndex], TALK_TIME_OUT_63SEC );
      break;    
    case RL_ON_HOOK:
      if (mInfoTalk[tIndex].Status != INFO_TALK_STATUS_TALK) return false;
      tCallEndFlg = true;
      break;
    case RL_DOPEN:
      if (mInfoTalk[tIndex].Status != INFO_TALK_STATUS_TALK) return false;
      MsgDoorOpenfunc();
      break;
    case ___ON_HOOK:
      tCallEndFlg = true;

      tPath = mInfoTalk[tIndex].DistPath;
      tLobbyID = (U8)(mInfoTalk[tIndex].LobbyID | 0x80);
      tHooNoH = mInfoTalk[tIndex].HooNoH;
      tHooNoL = mInfoTalk[tIndex].HooNoL;
      tDongH = mInfoTalk[tIndex].DongNoH;
      tDongL = mInfoTalk[tIndex].DongNoL;
    
      if ( tPrtBuf->ReceiveLine == RECEIVE_LINE_KEY )
      {
        if ( mInfoTalk[tIndex].Status == INFO_TALK_STATUS_TALK )
        {
          TransmitProtocolLobbySwitch( tLobbyID , LR_ON_HOOK, tPath, tDongH, tDongL, tHooNoH, tHooNoL );
        }
        else
        {
          TransmitProtocolLobbySwitch( tLobbyID , LR_CALL_OFF, tPath, tDongH, tDongL, tHooNoH, tHooNoL );
          sound_effect( NO_SOUND, 1 );
        }         
      }
      else
      {
        if ( mInfoTalk[tIndex].Status == INFO_TALK_STATUS_CALL_OK )
        {
          TransmitProtocolLobbySwitch( tLobbyID , LR_CALL_OFF, tPath, tDongH, tDongL, tHooNoH, tHooNoL );
          sound_effect( BUSY_TONE, 1 );
        }      
        else if ( mInfoTalk[tIndex].Status == INFO_TALK_STATUS_CALL )
        {
          TransmitProtocolLobbySwitch( tLobbyID , LR_CALL_OFF, tPath, tDongH, tDongL, tHooNoH, tHooNoL );
          sound_effect( NO_SOUND, 1 );
        }
        else
        {
          TransmitProtocolLobbySwitch( tLobbyID , LR_ON_HOOK, tPath, tDongH, tDongL, tHooNoH, tHooNoL );
        }   
      }      
      break;
    default:
      return false;
  }

  if ( tCallEndFlg ){
    KeyLockOff();
    CallManagerRepeaterOnHook( tIndex );
  }

  return true;

}


/// @brief ID가 서브로비 일때 [마스터로비->경비실] 프로토콜 중계 함수
/// @param tPrtBuf 프로토콜 데이터
/// @param tIndex 사용중인 통화 체널
/// @return true = 성공 \n false = 실패
U8 CallManagerRepeater_MLG_SubLobby( typeProtocolData *tPrtBuf , U8 tIndex )
{
  U8 tID,tCmd,tPath,tLobbyID,tDongNoH,tDongNoL,tHooNoH,tHooNoL;
  U8 tDongH,tDongL;
  U8 tCallEndFlg = false;
  typeUserCallResource tUserCallResource=NULL;
  typeMsgBox tMsgBuffer;
  
  switch(tPrtBuf->Cmd)
  {
    case LG_CALL_ON:
      if (mInfoTalk[tIndex].Status != INFO_TALK_STATUS_CALL_OK) return false;
      tPath = NULL;//mInfoTalk[tIndex].DistPath;
      tCmd = tPrtBuf->Cmd;
      tLobbyID = (U8)(mInfoTalk[tIndex].LobbyID | 0x80);
      tHooNoH = mInfoTalk[tIndex].HooNoH;
      tHooNoL = mInfoTalk[tIndex].HooNoL;
      tDongH = mInfoTalk[tIndex].DongNoH;
      tDongL = mInfoTalk[tIndex].DongNoL;
      TransmitProtocolLobbySwitch( tLobbyID , tCmd, tPath, tDongH, tDongL, tHooNoH, tHooNoL );      
      SetupTimeOut( mhCallTimeOut[tIndex], CALL_OK_TIME_OUT_3SEC );
      sound_effect( MAIN_BELL, 10 );
      break;
    case LG_CALL_OK:
      if (mInfoTalk[tIndex].Status != INFO_TALK_STATUS_CALL_OK) return false;
      mInfoTalk[tIndex].Status = INFO_TALK_STATUS_CALL;
      SetupTimeOut( mhCallTimeOut[tIndex], CALL_TIME_OUT_30SEC );
      break;
    case GL_CBUSY: 
  //case GL_LBUSY:
      if ( (mInfoTalk[tIndex].Status != INFO_TALK_STATUS_CALL_OK) && (mInfoTalk[tIndex].Status != INFO_TALK_STATUS_CALL)) return false;          
      tCallEndFlg = true;
      sound_effect( BUSY_TONE, 1 );
      break;
    case GL_OFF_HOOK:
      if (mInfoTalk[tIndex].Status != INFO_TALK_STATUS_CALL) return false;
      mInfoTalk[tIndex].Status = INFO_TALK_STATUS_TALK;
      tUserCallResource.Channel = CHANNEL_1;
      tUserCallResource.PathName[PATH_1] = mInfoTalk[CHANNEL_1].PathName[PATH_1];
      tUserCallResource.PathName[PATH_2] = mInfoTalk[CHANNEL_1].PathName[PATH_2];
      tUserCallResource.PathNumber[PATH_1] = CHANNEL_1;
      tUserCallResource.PathNumber[PATH_2] = (U8)((tPrtBuf->Path) & 0x0F);
      
      mInfoTalk[CHANNEL_1].PathNumber[PATH_1] = 1;
      mInfoTalk[CHANNEL_1].PathNumber[PATH_2] = tUserCallResource.PathNumber[PATH_2];

      mLineResource.MasterLobby = true;
      mLineResource.SubLobby[tUserCallResource.PathNumber[PATH_2]-1]= true;
      
      SelectCallMux( &tUserCallResource );
      SelectCameraMic();
      SelectRelay();
            
      sound_effect( NO_SOUND, 1 );
      SetupTimeOut( mhCallTimeOut[tIndex], TALK_TIME_OUT_63SEC );
      break;  
    case GL_ON_HOOK:  
      if (mInfoTalk[tIndex].Status != INFO_TALK_STATUS_TALK) return false;
      tCallEndFlg = true;
      break;
    case GL_DOPEN:
      if (mInfoTalk[tIndex].Status != INFO_TALK_STATUS_TALK) return false;
      MsgDoorOpenfunc();
      break;
    case ___ON_HOOK:
      tCallEndFlg = true;

      tPath = mInfoTalk[tIndex].DistPath;
      tLobbyID =(U8)( mInfoTalk[tIndex].LobbyID | 0x80);
      tHooNoH = mInfoTalk[tIndex].HooNoH;
      tHooNoL = mInfoTalk[tIndex].HooNoL;
      tDongH = mInfoTalk[tIndex].DongNoH;
      tDongL = mInfoTalk[tIndex].DongNoL;
               
      if ( tPrtBuf->ReceiveLine == RECEIVE_LINE_KEY )
      {
        if ( mInfoTalk[tIndex].Status == INFO_TALK_STATUS_TALK )
        {
          TransmitProtocolLobbySwitch( tLobbyID , LG_ON_HOOK, tPath, tDongH, tDongL, tHooNoH, tHooNoL );
        }
        else
        {
          TransmitProtocolLobbySwitch( tLobbyID , LG_CALL_OFF, tPath, tDongH, tDongL, tHooNoH, tHooNoL );
          sound_effect( NO_SOUND, 1 );
        }         
      }
      else
      {
        if ( mInfoTalk[tIndex].Status == INFO_TALK_STATUS_CALL_OK )
        {
          TransmitProtocolLobbySwitch( tLobbyID , LG_CALL_OFF, tPath, tDongH, tDongL, tHooNoH, tHooNoL );
          sound_effect( BUSY_TONE, 1 );
        }      
        else if ( mInfoTalk[tIndex].Status == INFO_TALK_STATUS_CALL )
        {
          TransmitProtocolLobbySwitch( tLobbyID , LG_CALL_OFF, tPath, tDongH, tDongL, tHooNoH, tHooNoL );
          sound_effect( NO_SOUND, 1 );
        }
        else
        {
          TransmitProtocolLobbySwitch( tLobbyID , LG_ON_HOOK, tPath, tDongH, tDongL, tHooNoH, tHooNoL );
        }   
      }
      break;
    default:
      return false;
  }

  if ( tCallEndFlg ){
    KeyLockOff();
    //KeyDataClear_NoDisplay();
    
    CallManagerRepeaterOnHook( tIndex );
  }

  return true;

}


/// @brief ID가 서브로비 일때 모든 프로토콜 중계 함수 (최상위)
/// @param tBuffer 프로토콜 데이터
/// @return False Only
U8 CallManagerRepeater_SubLobby( typeProtocolData *tBuffer ) // 20ms 
{
  U16 tDongNo;

  if( mInfoTalk[CHANNEL_1].Use == false ) return false; // 사용 하고 있는 통화 로인가?

  
  if( tBuffer->ReceiveLine != RECEIVE_LINE_KEY )
  {
    tDongNo = ((U16)tBuffer->DongNoH_LobbyID << 8) + ( tBuffer->DongNoL );
    HalfHex2Dec(tDongNo, &tDongNo);
    if ( tDongNo != gLobbyHandle.DongNumber ) return false;   
  }

  switch( mInfoTalk[CHANNEL_1].Direction )
  {
    case CALL_DIRECTION_MLG:
      if( tBuffer->ReceiveLine == RECEIVE_LINE_LOBBY){
        tDongNo = ((U16)tBuffer->DongNoH_LobbyID << 8) + ( tBuffer->DongNoL );
        HalfHex2Dec(tDongNo, &tDongNo);
        if ( tDongNo != gLobbyHandle.DongNumber ) break;        
        if( mInfoTalk[CHANNEL_1].LobbyID!= (tBuffer->DeviceID) ) break;
      }
      if( tBuffer->ReceiveLine == RECEIVE_LINE_GUARD ){
        if( mInfoTalk[CHANNEL_1].GuardID != (tBuffer->DeviceID) ) break;
      }
      CallManagerRepeater_MLG_SubLobby( tBuffer, CHANNEL_1 );
      return true;
    case CALL_DIRECTION_MLR:
      if( tBuffer->ReceiveLine == RECEIVE_LINE_LOBBY){
        tDongNo = ((U16)tBuffer->DongNoH_LobbyID << 8) + ( tBuffer->DongNoL );
        HalfHex2Dec(tDongNo, &tDongNo);
        if ( tDongNo != gLobbyHandle.DongNumber ) break;
        if( mInfoTalk[CHANNEL_1].LobbyID!= (tBuffer->DeviceID) ) break;
      }
      if( tBuffer->ReceiveLine == RECEIVE_LINE_HOUSE){
        if( mInfoTalk[CHANNEL_1].HouseID != (tBuffer->DeviceID) ) break;
      }
      CallManagerRepeater_MLR_SubLobby( tBuffer, CHANNEL_1 );
      return true;
    default:
      break;
  }
  return false;
}


/// @brief ID가 서브로비 일때 최초 통화 가능 여부 판단 하여 통화를 시작 하는 함수
/// @param tPrtBuf 프로토콜 데이터
/// @return true = 성공 \n false = 실패
U8 CallManagerRequest_SubLobby( typeProtocolData *tBuffer ) // 20ms 
{
  typeUserCallResource tUserCallResource;
  U8 tCallDirection;
  U8 tHouseID;
  U8 tHooNoH,tHooNoL;

  switch(tBuffer->Cmd)
  {
    case LG_CALL_ON:
      if ( tBuffer->ReceiveLine != RECEIVE_LINE_KEY) return false;
      tCallDirection = CALL_DIRECTION_MLG;
      break;
    case LR_CALL_ON:
      if ( tBuffer->ReceiveLine != RECEIVE_LINE_KEY) return false;
      tCallDirection = CALL_DIRECTION_MLR;
      break;
    default:
      return true;
  }

  tUserCallResource.Channel = 0;
  tUserCallResource.HouseID = 0;
  tUserCallResource.LobbyID = gLobbyHandle.LobbyID;
  tUserCallResource.GuardID = 0;//gLobbyHandle.GuardNumber;
  tUserCallResource.DongNoH = tBuffer->DongNoH_LobbyID;//gLobbyHandle.DongNumber >> 8;
  tUserCallResource.DongNoL = tBuffer->DongNoL;//gLobbyHandle.DongNumber & 0x00ff;
  tUserCallResource.HooNoH  = tBuffer->HooNoH;//tHooNoH;
  tUserCallResource.HooNoL  = tBuffer->HooNoL;//tHooNoL;
  tUserCallResource.PathName[PATH_1] = PATH_NAME_MASTER_LOBBY;
  tUserCallResource.PathName[PATH_2] = PATH_NAME_SUB_LOBBY_1;

  mInfoTalk[CHANNEL_1].PathName[PATH_1] = tUserCallResource.PathName[PATH_1];
  mInfoTalk[CHANNEL_1].PathName[PATH_2] = tUserCallResource.PathName[PATH_2];
  mInfoTalk[CHANNEL_1].PathNumber[PATH_1] = NULL;
  mInfoTalk[CHANNEL_1].PathNumber[PATH_2] = NULL;
  mInfoTalk[CHANNEL_1].Use = true;

  mInfoTalk[CHANNEL_1].Use = true;
  mInfoTalk[CHANNEL_1].Channel   = tUserCallResource.Channel;
  mInfoTalk[CHANNEL_1].Status    = INFO_TALK_STATUS_CALL_OK;
  mInfoTalk[CHANNEL_1].Direction = tCallDirection;
  mInfoTalk[CHANNEL_1].HouseID   = tUserCallResource.HouseID;
  mInfoTalk[CHANNEL_1].LobbyID   = tUserCallResource.LobbyID;
  mInfoTalk[CHANNEL_1].GuardID   = tUserCallResource.GuardID;
  mInfoTalk[CHANNEL_1].DongNoH   = tUserCallResource.DongNoH;
  mInfoTalk[CHANNEL_1].DongNoL   = tUserCallResource.DongNoL;
  mInfoTalk[CHANNEL_1].HooNoH    = tUserCallResource.HooNoH;
  mInfoTalk[CHANNEL_1].HooNoL    = tUserCallResource.HooNoL;
  mInfoTalk[CHANNEL_1].SrcPath   = NULL;//tUserCallResource.PathNumber[INFO_TALK_CH0];
  mInfoTalk[CHANNEL_1].DistPath  = NULL;//tUserCallResource.PathNumber[INFO_TALK_CH1];
  
  SelectVideoMux();
  
  SetupTimeOut( mhCallTimeOut[CHANNEL_1], CALL_OK_TIME_OUT_3SEC );///

  return true;
  
}



