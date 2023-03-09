
/// @file func_call_manager.c
/// @date 2012/02/14
/// @author jinhwa Lee
/// @brief  통화 관리 모듈 1

#include "main.h"

extern typeLineResource    mLineResource;
extern U8 mWhileSound_RL_CALL_REQUEST_flg;

typeInfoTalk mInfoTalk[INFO_TALK_MAX];

HANDLE mhCallTimeOut[MAX_CHANNEL];



/// @brief 경비실->세대 프로토콜 중계 함수
/// @param tPrtBuf 프로토콜 데이터
/// @param tIndex 사용중인 통화 체널
/// @return true = 성공 \n false = 실패
U8 CallManagerRepeater_GR( typeProtocolData *tPrtBuf , U8 tIndex )
{
  U8 tCallEndFlg = false;

  switch(tPrtBuf->Cmd)
  {
    case GR_CALL_ON:
      if (mInfoTalk[tIndex].Status != INFO_TALK_STATUS_CALL_OK) return false;
      TransmitProtocolHouseSwitch( mInfoTalk[tIndex].HouseID, tPrtBuf->Cmd, mInfoTalk[tIndex].DistPath, 0 );

      if ( gLobbyHandle.Protocol == PSS_HT_PROTOCOL )
      {
        delay_ms(50);
        mInfoTalk[tIndex].Status = INFO_TALK_STATUS_CALL;
        TransmitProtocolGuardSwitch( mInfoTalk[tIndex].GuardID, GR_CALL_OK, mInfoTalk[tIndex].SrcPath,
                                mInfoTalk[tIndex].DongNoH, mInfoTalk[tIndex].DongNoL, 
                                mInfoTalk[tIndex].HooNoH, mInfoTalk[tIndex].HooNoL );
        SetupTimeOut( mhCallTimeOut[tIndex], CALL_TIME_OUT_36SEC );
      }
      else if ( gLobbyHandle.Protocol == PSS_HT_PROTOCOL_2ND )
      {
        mInfoTalk[tIndex].Status = INFO_TALK_STATUS_CALL;
        SetupTimeOut( mhCallTimeOut[tIndex], CALL_TIME_OUT_36SEC );        
      }
      break;
    case GR_CALL_OK:
      if ( tPrtBuf->ReceiveLine != RECEIVE_LINE_HOUSE ) return false;
      if ( mInfoTalk[tIndex].Status != INFO_TALK_STATUS_CALL_OK ) return false;
      mInfoTalk[tIndex].Status = INFO_TALK_STATUS_CALL;
      TransmitProtocolGuardSwitch( mInfoTalk[tIndex].GuardID, tPrtBuf->Cmd, mInfoTalk[tIndex].SrcPath,
                              mInfoTalk[tIndex].DongNoH, mInfoTalk[tIndex].DongNoL, 
                              mInfoTalk[tIndex].HooNoH, mInfoTalk[tIndex].HooNoL );
      SetupTimeOut( mhCallTimeOut[tIndex], CALL_TIME_OUT_36SEC );
      break;
    case GR_CALL_OFF:
      if ( (mInfoTalk[tIndex].Status != INFO_TALK_STATUS_CALL_OK) && (mInfoTalk[tIndex].Status != INFO_TALK_STATUS_CALL)) return false;
      tCallEndFlg = true;
      TransmitProtocolHouseSwitch( mInfoTalk[tIndex].HouseID, tPrtBuf->Cmd, mInfoTalk[tIndex].DistPath, 0 );      
      break;
    case RG_CBUSY:
    case RG_LBUSY:      
      if ( (mInfoTalk[tIndex].Status != INFO_TALK_STATUS_CALL_OK) && (mInfoTalk[tIndex].Status != INFO_TALK_STATUS_CALL)) return false;
      tCallEndFlg = true;
      TransmitProtocolGuardSwitch( mInfoTalk[tIndex].GuardID, tPrtBuf->Cmd, mInfoTalk[tIndex].SrcPath,
                            mInfoTalk[tIndex].DongNoH, mInfoTalk[tIndex].DongNoL, 
                            mInfoTalk[tIndex].HooNoH, mInfoTalk[tIndex].HooNoL );
      break;
    case RG_OFF_HOOK:
      if ( mInfoTalk[tIndex].Status != INFO_TALK_STATUS_CALL ) return false;
      mInfoTalk[tIndex].Status = INFO_TALK_STATUS_TALK;
      SelectRelay();
      TransmitProtocolGuardSwitch( mInfoTalk[tIndex].GuardID, tPrtBuf->Cmd, mInfoTalk[tIndex].SrcPath,
                              mInfoTalk[tIndex].DongNoH, mInfoTalk[tIndex].DongNoL, 
                              mInfoTalk[tIndex].HooNoH, mInfoTalk[tIndex].HooNoL );
      SetupTimeOut( mhCallTimeOut[tIndex], TALK_TIME_OUT_66SEC );
      break;      
    case GR_ON_HOOK:
      if ( (mInfoTalk[tIndex].Status == INFO_TALK_STATUS_CALL) || (mInfoTalk[tIndex].Status == INFO_TALK_STATUS_CALL_OK))
      {
        tCallEndFlg = true;
        TransmitProtocolHouseSwitch( mInfoTalk[tIndex].HouseID, GR_CALL_OFF, mInfoTalk[tIndex].DistPath, 0 );      
        break;
      }
      if (mInfoTalk[tIndex].Status != INFO_TALK_STATUS_TALK) return false;
      tCallEndFlg = true;
      TransmitProtocolHouseSwitch( mInfoTalk[tIndex].HouseID, tPrtBuf->Cmd, mInfoTalk[tIndex].DistPath, 0 );      
      break;
    case RG_ON_HOOK:
      if (mInfoTalk[tIndex].Status != INFO_TALK_STATUS_TALK ) return false;
      tCallEndFlg = true;
      TransmitProtocolGuardSwitch( mInfoTalk[tIndex].GuardID, tPrtBuf->Cmd, mInfoTalk[tIndex].SrcPath,
                              mInfoTalk[tIndex].DongNoH, mInfoTalk[tIndex].DongNoL, 
                              mInfoTalk[tIndex].HooNoH, mInfoTalk[tIndex].HooNoL );
      break; 
    case ___ON_HOOK:
      if ( tPrtBuf->ReceiveLine == RECEIVE_LINE_KEY ) break;
      tCallEndFlg = true;
      break;
/*
    case INTC_ON_HOOK:
      tCallEndFlg = true;
      TransmitProtocolHouseSwitch( mInfoTalk[tIndex].HouseID, tPrtBuf->Cmd, mInfoTalk[tIndex].DistPath, 0 );
      break;
    case GR_HOLD_ON:
      mLineResource.IsHold = INFO_TALK_STATUS_HOLD;
      TransmitProtocolHouseSwitch( mInfoTalk[tIndex].HouseID, tPrtBuf->Cmd, mInfoTalk[tIndex].DistPath, 0 );      
      break;
    case GR_HOLD_OFF:
      mLineResource.IsHold = NULL;
      TransmitProtocolHouseSwitch( mInfoTalk[tIndex].HouseID, tPrtBuf->Cmd, mInfoTalk[tIndex].DistPath, 0 );
      break;
    case GR_INT_COM:
      TransmitProtocolHouseSwitch( mInfoTalk[tIndex].HouseID, tPrtBuf->Cmd, mInfoTalk[tIndex].DistPath, 0 );      
      break;
*/    
    default:
      return false;
  }

  if ( tCallEndFlg ) CallManagerRepeaterOnHook( tIndex );
  return true;

}


/// @brief 세대->경비실 프로토콜 중계 함수
/// @param tPrtBuf 프로토콜 데이터
/// @param tIndex 사용중인 통화 체널
/// @return true = 성공 \n false = 실패
U8 CallManagerRepeater_RG( typeProtocolData *tPrtBuf , U8 tIndex )
{
  U8 tCallEndFlg = false;

  switch(tPrtBuf->Cmd)
  {
    case RG_CALL_ON:
      if (mInfoTalk[tIndex].Status != INFO_TALK_STATUS_CALL_OK) return false;

      TransmitProtocolGuardSwitch( mInfoTalk[tIndex].GuardID, tPrtBuf->Cmd, mInfoTalk[tIndex].DistPath,
                              mInfoTalk[tIndex].DongNoH, mInfoTalk[tIndex].DongNoL,
                              mInfoTalk[tIndex].HooNoH, mInfoTalk[tIndex].HooNoL );      
      
      if (( gLobbyHandle.Protocol == PSS_HT_PROTOCOL) || ( gLobbyHandle.Protocol == PSS_HT_PROTOCOL_2ND ) )
      {
        mInfoTalk[tIndex].Status = INFO_TALK_STATUS_CALL;
        TransmitProtocolHouseSwitch( mInfoTalk[tIndex].HouseID, RG_CALL_OK, mInfoTalk[tIndex].SrcPath, 1 );   
        SetupTimeOut( mhCallTimeOut[tIndex], CALL_TIME_OUT_36SEC );
      }
      break;
    case RG_CALL_OK: //경비실에서 보내질 않음
      if (mInfoTalk[tIndex].Status != INFO_TALK_STATUS_CALL_OK) return false;
      mInfoTalk[tIndex].Status = INFO_TALK_STATUS_CALL;
      TransmitProtocolHouseSwitch( mInfoTalk[tIndex].HouseID, tPrtBuf->Cmd, mInfoTalk[tIndex].SrcPath, 1 );
      SetupTimeOut( mhCallTimeOut[tIndex], CALL_TIME_OUT_36SEC );
      break;
    case RG_CALL_OFF:
      if ( (mInfoTalk[tIndex].Status != INFO_TALK_STATUS_CALL_OK) && (mInfoTalk[tIndex].Status != INFO_TALK_STATUS_CALL)) return false;
      tCallEndFlg = true;   
      TransmitProtocolGuardSwitch( mInfoTalk[tIndex].GuardID, tPrtBuf->Cmd, mInfoTalk[tIndex].DistPath,
                              mInfoTalk[tIndex].DongNoH, mInfoTalk[tIndex].DongNoL, 
                              mInfoTalk[tIndex].HooNoH, mInfoTalk[tIndex].HooNoL );
      break;
    case GR_LBUSY:
    case GR_CBUSY:
      if ( (mInfoTalk[tIndex].Status != INFO_TALK_STATUS_CALL_OK) && (mInfoTalk[tIndex].Status != INFO_TALK_STATUS_CALL)) return false;     
      tCallEndFlg = true;
      TransmitProtocolHouseSwitch( mInfoTalk[tIndex].HouseID, tPrtBuf->Cmd, mInfoTalk[tIndex].SrcPath, 1 );
      break;
    case GR_OFF_HOOK:
      if ( mInfoTalk[tIndex].Status != INFO_TALK_STATUS_CALL ) return false;
      SelectRelay();
      mInfoTalk[tIndex].Status = INFO_TALK_STATUS_TALK;
      TransmitProtocolHouseSwitch( mInfoTalk[tIndex].HouseID, tPrtBuf->Cmd, mInfoTalk[tIndex].SrcPath, 1 ); 
      SetupTimeOut( mhCallTimeOut[tIndex], TALK_TIME_OUT_66SEC );
      break;
    case GR_ON_HOOK:
      if ( mInfoTalk[tIndex].Status != INFO_TALK_STATUS_TALK ) return false;
      tCallEndFlg = true;
      TransmitProtocolHouseSwitch( mInfoTalk[tIndex].HouseID, tPrtBuf->Cmd, mInfoTalk[tIndex].SrcPath, 1 );  
      break;
    case RG_ON_HOOK:
      if (mInfoTalk[tIndex].Status == INFO_TALK_STATUS_CALL )
      {
        tCallEndFlg = true;   
        TransmitProtocolGuardSwitch( mInfoTalk[tIndex].GuardID, RG_CALL_OFF, mInfoTalk[tIndex].DistPath,
                                mInfoTalk[tIndex].DongNoH, mInfoTalk[tIndex].DongNoL, 
                                mInfoTalk[tIndex].HooNoH, mInfoTalk[tIndex].HooNoL );
        break;
      }      
      if ( mInfoTalk[tIndex].Status != INFO_TALK_STATUS_TALK ) return false;
      tCallEndFlg = true;   
      TransmitProtocolGuardSwitch( mInfoTalk[tIndex].GuardID, tPrtBuf->Cmd, mInfoTalk[tIndex].DistPath,
                              mInfoTalk[tIndex].DongNoH, mInfoTalk[tIndex].DongNoL, 
                              mInfoTalk[tIndex].HooNoH, mInfoTalk[tIndex].HooNoL );
      break;
    case ___ON_HOOK:
      if ( tPrtBuf->ReceiveLine == RECEIVE_LINE_KEY ) break;
      tCallEndFlg = true;
      break;
    default:
      return false;
  }

  if ( tCallEndFlg ) CallManagerRepeaterOnHook( tIndex );
  return true;
  
}


/// @brief 서브로비->세대 프로토콜 중계 함수
/// @param tPrtBuf 프로토콜 데이터
/// @param tIndex 사용중인 통화 체널
/// @return true = 성공 \n false = 실패
U8 CallManagerRepeater_SLR( typeProtocolData *tPrtBuf , U8 tIndex )
{
  U8 tCallEndFlg = false;
  U8 tDirUart2 = false;

  switch(tPrtBuf->Cmd)
  {
    case LR_CALL_ON:
      if (mInfoTalk[tIndex].Status != INFO_TALK_STATUS_CALL_OK) return false;
      
      TransmitProtocolHouseSwitch( mInfoTalk[tIndex].HouseID, tPrtBuf->Cmd, mInfoTalk[tIndex].DistPath, 1 ); 
      if ( (gLobbyHandle.Protocol == PSS_HT_PROTOCOL) || ( gLobbyHandle.Protocol == PSS_HT_PROTOCOL_2ND ))      
      {
        delay_ms(50);
        mInfoTalk[tIndex].Status = INFO_TALK_STATUS_CALL;
        TransmitProtocolLobbySwitch( mInfoTalk[tIndex].LobbyID , LR_CALL_OK, mInfoTalk[tIndex].SrcPath, 
                                      mInfoTalk[tIndex].DongNoH, mInfoTalk[tIndex].DongNoL, 
                                      mInfoTalk[tIndex].HooNoH, mInfoTalk[tIndex].HooNoL );    
        SetupTimeOut( mhCallTimeOut[tIndex], CALL_TIME_OUT_36SEC ); 
      }
      
      break;
    case LR_CALL_OK:
      if (mInfoTalk[tIndex].Status != INFO_TALK_STATUS_CALL_OK) return false;
      mInfoTalk[tIndex].Status = INFO_TALK_STATUS_CALL;
      TransmitProtocolLobbySwitch( mInfoTalk[tIndex].LobbyID , tPrtBuf->Cmd, mInfoTalk[tIndex].SrcPath, 
                                    mInfoTalk[tIndex].DongNoH, mInfoTalk[tIndex].DongNoL, 
                                    mInfoTalk[tIndex].HooNoH, mInfoTalk[tIndex].HooNoL );      
      SetupTimeOut( mhCallTimeOut[tIndex], CALL_TIME_OUT_36SEC ); 
      break;
    case LR_CALL_OFF:
      if ( (mInfoTalk[tIndex].Status != INFO_TALK_STATUS_CALL_OK) && (mInfoTalk[tIndex].Status != INFO_TALK_STATUS_CALL)) return false;          
      tCallEndFlg = true;
      TransmitProtocolHouseSwitch( mInfoTalk[tIndex].HouseID, tPrtBuf->Cmd, 
                                    mInfoTalk[tIndex].DistPath, mInfoTalk[tIndex].LobbyID );      
      break;
    case RL_CBUSY:
    //case RL_LBUSY:
      if ( (mInfoTalk[tIndex].Status != INFO_TALK_STATUS_CALL_OK) && (mInfoTalk[tIndex].Status != INFO_TALK_STATUS_CALL)) return false;     
      tCallEndFlg = true;
      TransmitProtocolLobbySwitch( mInfoTalk[tIndex].LobbyID , tPrtBuf->Cmd, mInfoTalk[tIndex].SrcPath, 
                                    mInfoTalk[tIndex].DongNoH, mInfoTalk[tIndex].DongNoL, 
                                    mInfoTalk[tIndex].HooNoH, mInfoTalk[tIndex].HooNoL );
      break;    
    case RL_OFF_HOOK:
      if ( mInfoTalk[tIndex].Status != INFO_TALK_STATUS_CALL ) return false;
      SelectRelay();
      mInfoTalk[tIndex].Status = INFO_TALK_STATUS_TALK;
      TransmitProtocolLobbySwitch( mInfoTalk[tIndex].LobbyID , tPrtBuf->Cmd, mInfoTalk[tIndex].SrcPath, 
                                    mInfoTalk[tIndex].DongNoH, mInfoTalk[tIndex].DongNoL, 
                                    mInfoTalk[tIndex].HooNoH, mInfoTalk[tIndex].HooNoL );
      SetupTimeOut( mhCallTimeOut[tIndex], TALK_TIME_OUT_66SEC );      
      break;
    case LR_ON_HOOK:
      if (mInfoTalk[tIndex].Status == INFO_TALK_STATUS_CALL )
      {
        tCallEndFlg = true;   
        tCallEndFlg = true;
        TransmitProtocolHouseSwitch( mInfoTalk[tIndex].HouseID, LR_CALL_OFF, 
                                      mInfoTalk[tIndex].DistPath, mInfoTalk[tIndex].LobbyID );    

        break;
      }        
      if ( mInfoTalk[tIndex].Status != INFO_TALK_STATUS_TALK ) return false;
      tCallEndFlg = true;
      TransmitProtocolHouseSwitch( mInfoTalk[tIndex].HouseID, tPrtBuf->Cmd, 
                                    mInfoTalk[tIndex].DistPath, mInfoTalk[tIndex].LobbyID );      
      break;
    case RL_ON_HOOK:
      if ( mInfoTalk[tIndex].Status != INFO_TALK_STATUS_TALK ) return false;
      tCallEndFlg = true;
      TransmitProtocolLobbySwitch( mInfoTalk[tIndex].LobbyID , tPrtBuf->Cmd, mInfoTalk[tIndex].SrcPath, 
                                    mInfoTalk[tIndex].DongNoH, mInfoTalk[tIndex].DongNoL, 
                                    mInfoTalk[tIndex].HooNoH, mInfoTalk[tIndex].HooNoL );
      break;
    case RL_DOPEN:
      if ( mInfoTalk[tIndex].Status != INFO_TALK_STATUS_TALK ) return false;
      TransmitProtocolLobbySwitch( mInfoTalk[tIndex].LobbyID , tPrtBuf->Cmd, mInfoTalk[tIndex].SrcPath, 
                                    mInfoTalk[tIndex].DongNoH, mInfoTalk[tIndex].DongNoL, 
                                    mInfoTalk[tIndex].HooNoH, mInfoTalk[tIndex].HooNoL );    
      break;
    case ___ON_HOOK:
      if ( tPrtBuf->ReceiveLine == RECEIVE_LINE_KEY ) break;
      tCallEndFlg = true;
      break;
    default:
      return false;
  }
  
  if ( tCallEndFlg ) CallManagerRepeaterOnHook( tIndex );
  return true;

}


/// @brief 서브로비->경비실 프로토콜 중계 함수
/// @param tPrtBuf 프로토콜 데이터
/// @param tIndex 사용중인 통화 체널
/// @return true = 성공 \n false = 실패
U8 CallManagerRepeater_SLG( typeProtocolData *tPrtBuf , U8 tIndex )
{
  U8 tCallEndFlg = false;
  
  switch(tPrtBuf->Cmd)
  {

    case LG_CALL_ON:
      if (mInfoTalk[tIndex].Status != INFO_TALK_STATUS_CALL_OK) return false;
      TransmitProtocolGuardSwitch( mInfoTalk[tIndex].GuardID, tPrtBuf->Cmd, mInfoTalk[tIndex].DistPath,
                              mInfoTalk[tIndex].DongNoH, mInfoTalk[tIndex].DongNoL, 
                              mInfoTalk[tIndex].HooNoH, mInfoTalk[tIndex].HooNoL );   

      if ( (gLobbyHandle.Protocol == PSS_HT_PROTOCOL) || ( gLobbyHandle.Protocol == PSS_HT_PROTOCOL_2ND ) )
      {
        delay_ms(50);
        mInfoTalk[tIndex].Status = INFO_TALK_STATUS_CALL;
        TransmitProtocolLobbySwitch( mInfoTalk[tIndex].LobbyID , LG_CALL_OK, mInfoTalk[tIndex].SrcPath, 
                                      mInfoTalk[tIndex].DongNoH, mInfoTalk[tIndex].DongNoL, 
                                      mInfoTalk[tIndex].HooNoH, mInfoTalk[tIndex].HooNoL );      
        SetupTimeOut( mhCallTimeOut[tIndex], CALL_TIME_OUT_36SEC );
      }
      break;
    case LG_CALL_OK:
      if (mInfoTalk[tIndex].Status != INFO_TALK_STATUS_CALL_OK) return false;
      mInfoTalk[tIndex].Status = INFO_TALK_STATUS_CALL;
      TransmitProtocolLobbySwitch( mInfoTalk[tIndex].LobbyID , tPrtBuf->Cmd, mInfoTalk[tIndex].SrcPath, 
                                    mInfoTalk[tIndex].DongNoH, mInfoTalk[tIndex].DongNoL, 
                                    mInfoTalk[tIndex].HooNoH, mInfoTalk[tIndex].HooNoL );   
      SetupTimeOut( mhCallTimeOut[tIndex], CALL_TIME_OUT_36SEC );
      break;
    case LG_CALL_OFF://S_LG_CALL_OFF:
      if ( (mInfoTalk[tIndex].Status != INFO_TALK_STATUS_CALL_OK) && (mInfoTalk[tIndex].Status != INFO_TALK_STATUS_CALL)) return false;        
      tCallEndFlg = true;
      TransmitProtocolGuardSwitch( mInfoTalk[tIndex].GuardID, tPrtBuf->Cmd, mInfoTalk[tIndex].DistPath,
                        mInfoTalk[tIndex].DongNoH, mInfoTalk[tIndex].DongNoL, 
                        mInfoTalk[tIndex].HooNoH, mInfoTalk[tIndex].HooNoL );       
      
      break;
    //case GL_LBUSY:
    case GL_CBUSY:
      if ( (mInfoTalk[tIndex].Status != INFO_TALK_STATUS_CALL_OK) && (mInfoTalk[tIndex].Status != INFO_TALK_STATUS_CALL)) return false;          
      tCallEndFlg = true;
      TransmitProtocolLobbySwitch( mInfoTalk[tIndex].LobbyID , tPrtBuf->Cmd, mInfoTalk[tIndex].SrcPath, 
                                    mInfoTalk[tIndex].DongNoH, mInfoTalk[tIndex].DongNoL, 
                                    mInfoTalk[tIndex].HooNoH, mInfoTalk[tIndex].HooNoL );  
      break;
    case GL_OFF_HOOK:
      if ( mInfoTalk[tIndex].Status != INFO_TALK_STATUS_CALL ) return false;
      SelectRelay();
      mInfoTalk[tIndex].Status = INFO_TALK_STATUS_TALK;
      TransmitProtocolLobbySwitch( mInfoTalk[tIndex].LobbyID , tPrtBuf->Cmd, mInfoTalk[tIndex].SrcPath, 
                                    mInfoTalk[tIndex].DongNoH, mInfoTalk[tIndex].DongNoL, 
                                    mInfoTalk[tIndex].HooNoH, mInfoTalk[tIndex].HooNoL ); 
      SetupTimeOut( mhCallTimeOut[tIndex], TALK_TIME_OUT_66SEC );
      break;
    case LG_ON_HOOK:
      if (mInfoTalk[tIndex].Status == INFO_TALK_STATUS_CALL )
      {
        tCallEndFlg = true;
        TransmitProtocolGuardSwitch( mInfoTalk[tIndex].GuardID, LG_CALL_OFF, mInfoTalk[tIndex].DistPath,
                          mInfoTalk[tIndex].DongNoH, mInfoTalk[tIndex].DongNoL, 
                          mInfoTalk[tIndex].HooNoH, mInfoTalk[tIndex].HooNoL );    
        break;
      }        
      if ( mInfoTalk[tIndex].Status != INFO_TALK_STATUS_TALK ) return false;
      tCallEndFlg = true;
      TransmitProtocolGuardSwitch( mInfoTalk[tIndex].GuardID, tPrtBuf->Cmd, mInfoTalk[tIndex].DistPath,
                        mInfoTalk[tIndex].DongNoH, mInfoTalk[tIndex].DongNoL, 
                        mInfoTalk[tIndex].HooNoH, mInfoTalk[tIndex].HooNoL );       
      
      break;
    case GL_ON_HOOK:
      if ( mInfoTalk[tIndex].Status != INFO_TALK_STATUS_TALK ) return false;
      tCallEndFlg = true;
      TransmitProtocolLobbySwitch( mInfoTalk[tIndex].LobbyID , tPrtBuf->Cmd, mInfoTalk[tIndex].SrcPath, 
                                    mInfoTalk[tIndex].DongNoH, mInfoTalk[tIndex].DongNoL, 
                                    mInfoTalk[tIndex].HooNoH, mInfoTalk[tIndex].HooNoL );
      break;
    case GL_DOPEN:
      if ( mInfoTalk[tIndex].Status != INFO_TALK_STATUS_TALK ) return false;
      TransmitProtocolLobbySwitch( mInfoTalk[tIndex].LobbyID , tPrtBuf->Cmd, mInfoTalk[tIndex].SrcPath, 
                                    mInfoTalk[tIndex].DongNoH, mInfoTalk[tIndex].DongNoL, 
                                    mInfoTalk[tIndex].HooNoH, mInfoTalk[tIndex].HooNoL );      
    
      break;
    case ___ON_HOOK:
      if ( tPrtBuf->ReceiveLine == RECEIVE_LINE_KEY ) break;
      tCallEndFlg = true;
      break;
    default:
      return false;
  }

  if ( tCallEndFlg ) CallManagerRepeaterOnHook( tIndex );
  return true;


}


/// @brief 마스터로비->세대 프로토콜 중계 함수
/// @param tPrtBuf 프로토콜 데이터
/// @param tIndex 사용중인 통화 체널
/// @return true = 성공 \n false = 실패
U8 CallManagerRepeater_MLR( typeProtocolData *tPrtBuf , U8 tIndex )
{
  U8 tCallEndFlg = false;
  typeMsgBox tMsgBuffer;  
      
  switch(tPrtBuf->Cmd)
  {
    case LR_CALL_ON:
      if (mInfoTalk[tIndex].Status != INFO_TALK_STATUS_CALL_OK) return false;
      TransmitProtocolHouseSwitch( mInfoTalk[tIndex].HouseID, tPrtBuf->Cmd, mInfoTalk[tIndex].DistPath, 1 );
      sound_effect( MAIN_BELL, 10 );
      if ( (gLobbyHandle.Protocol == PSS_HT_PROTOCOL) || (gLobbyHandle.Protocol == PSS_HT_PROTOCOL_2ND) )      
      {
        mInfoTalk[tIndex].Status = INFO_TALK_STATUS_CALL;
        SetupTimeOut( mhCallTimeOut[tIndex], CALL_TIME_OUT_30SEC ); 
      }
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
      if ( mInfoTalk[tIndex].Status != INFO_TALK_STATUS_CALL ) return false;
      SelectRelay();
      mInfoTalk[tIndex].Status = INFO_TALK_STATUS_TALK;
      if( mWhileSound_RL_CALL_REQUEST_flg == false ) sound_effect( NO_SOUND, 1 );
      SetupTimeOut( mhCallTimeOut[tIndex], TALK_TIME_OUT_63SEC );
      break; 
    case RL_ON_HOOK:
      if ( mInfoTalk[tIndex].Status != INFO_TALK_STATUS_TALK ) return false;
      tCallEndFlg = true;
      if( mWhileSound_RL_CALL_REQUEST_flg == true ) sound_effect( NO_SOUND, 1 );
      break;
    case RL_DOPEN:
      if ( mInfoTalk[tIndex].Status != INFO_TALK_STATUS_TALK ) return false;
      MsgDoorOpenfunc();
      break;
    case ___ON_HOOK:
      tCallEndFlg = true;
      if ( tPrtBuf->ReceiveLine == RECEIVE_LINE_KEY )
      {
        if ( mInfoTalk[tIndex].Status == INFO_TALK_STATUS_TALK )
        {
          TransmitProtocolHouseSwitch( mInfoTalk[tIndex].HouseID, LR_ON_HOOK, mInfoTalk[tIndex].DistPath, 1 );
        }
        else
        {
          TransmitProtocolHouseSwitch( mInfoTalk[tIndex].HouseID, LR_CALL_OFF, mInfoTalk[tIndex].DistPath, 1 );
          sound_effect( NO_SOUND, 1 );
        }         
      }
      else
      {
        if ( mInfoTalk[tIndex].Status == INFO_TALK_STATUS_CALL_OK )
        {
          TransmitProtocolHouseSwitch( mInfoTalk[tIndex].HouseID, LR_CALL_OFF, mInfoTalk[tIndex].DistPath, 1 );
          sound_effect( BUSY_TONE, 1 );
        }      
        else if ( mInfoTalk[tIndex].Status == INFO_TALK_STATUS_CALL )
        {
          TransmitProtocolHouseSwitch( mInfoTalk[tIndex].HouseID, LR_CALL_OFF, mInfoTalk[tIndex].DistPath, 1 );
          sound_effect( NO_SOUND, 1 );
        }
        else
        {
          TransmitProtocolHouseSwitch( mInfoTalk[tIndex].HouseID, LR_ON_HOOK, mInfoTalk[tIndex].DistPath, 1 );
        }   
      }
      break;
    default:
      return false;
  }

  if ( tCallEndFlg ){
    KeyLockOff();
    CallTest_RL_CALL_REQUEST( false , tPrtBuf);
    //KeyDataClear_NoDisplay();
    //sound_effect( NO_SOUND, 1 );
    CallManagerRepeaterOnHook( tIndex );
  }

  return true;

}


/// @brief 마스터로비->경비실 프로토콜 중계 함수
/// @param tPrtBuf 프로토콜 데이터
/// @param tIndex 사용중인 통화 체널
/// @return true = 성공 \n false = 실패
U8 CallManagerRepeater_MLG( typeProtocolData *tPrtBuf , U8 tIndex )
{
  U8 tCallEndFlg = false;
  typeMsgBox tMsgBuffer;
  

  switch(tPrtBuf->Cmd)
  {
    case LG_CALL_ON:
      if (mInfoTalk[tIndex].Status != INFO_TALK_STATUS_CALL_OK) return false;
      TransmitProtocolGuardSwitch( mInfoTalk[tIndex].GuardID, tPrtBuf->Cmd, mInfoTalk[tIndex].DistPath,
                              mInfoTalk[tIndex].DongNoH, mInfoTalk[tIndex].DongNoL, 
                              mInfoTalk[tIndex].HooNoH, mInfoTalk[tIndex].HooNoL );       
      sound_effect( MAIN_BELL, 10 );
      if ( (gLobbyHandle.Protocol == PSS_HT_PROTOCOL) || (gLobbyHandle.Protocol == PSS_HT_PROTOCOL_2ND) )      
      {
        mInfoTalk[tIndex].Status = INFO_TALK_STATUS_CALL;
        SetupTimeOut( mhCallTimeOut[tIndex], CALL_TIME_OUT_30SEC ); 
      }   
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
      if ( mInfoTalk[tIndex].Status != INFO_TALK_STATUS_CALL ) return false;
      SelectRelay();
      mInfoTalk[tIndex].Status = INFO_TALK_STATUS_TALK;
      sound_effect( NO_SOUND, 1 );
      SetupTimeOut( mhCallTimeOut[tIndex], TALK_TIME_OUT_63SEC );
      break;
    case GL_ON_HOOK:
      if ( mInfoTalk[tIndex].Status != INFO_TALK_STATUS_TALK ) return false;
      tCallEndFlg = true;
      //sound_effect( NO_SOUND, 1 );
      break;
    case GL_DOPEN:
      if ( mInfoTalk[tIndex].Status != INFO_TALK_STATUS_TALK ) return false;
      MsgDoorOpenfunc();
      break;
    case ___ON_HOOK:
      tCallEndFlg = true;

      if ( tPrtBuf->ReceiveLine == RECEIVE_LINE_KEY )
      {
        if ( mInfoTalk[tIndex].Status == INFO_TALK_STATUS_TALK )
        {
          TransmitProtocolGuardSwitch( mInfoTalk[tIndex].GuardID, LG_ON_HOOK, mInfoTalk[tIndex].DistPath,
                                  mInfoTalk[tIndex].DongNoH, mInfoTalk[tIndex].DongNoL, 
                                  mInfoTalk[tIndex].HooNoH, mInfoTalk[tIndex].HooNoL );  

        }
        else
        {
          sound_effect( NO_SOUND, 1 );
          TransmitProtocolGuardSwitch( mInfoTalk[tIndex].GuardID, LG_CALL_OFF, mInfoTalk[tIndex].DistPath,
                                  mInfoTalk[tIndex].DongNoH, mInfoTalk[tIndex].DongNoL, 
                                  mInfoTalk[tIndex].HooNoH, mInfoTalk[tIndex].HooNoL ); 
        }         
      }
      else
      {
        if ( mInfoTalk[tIndex].Status == INFO_TALK_STATUS_CALL_OK )
        {
          sound_effect( BUSY_TONE, 1 );
          TransmitProtocolGuardSwitch( mInfoTalk[tIndex].GuardID, LG_CALL_OFF, mInfoTalk[tIndex].DistPath,
                                  mInfoTalk[tIndex].DongNoH, mInfoTalk[tIndex].DongNoL, 
                                  mInfoTalk[tIndex].HooNoH, mInfoTalk[tIndex].HooNoL ); 
        }      
        else if ( mInfoTalk[tIndex].Status == INFO_TALK_STATUS_CALL )
        {
          sound_effect( NO_SOUND, 1 );
          TransmitProtocolGuardSwitch( mInfoTalk[tIndex].GuardID, LG_CALL_OFF, mInfoTalk[tIndex].DistPath,
                                  mInfoTalk[tIndex].DongNoH, mInfoTalk[tIndex].DongNoL, 
                                  mInfoTalk[tIndex].HooNoH, mInfoTalk[tIndex].HooNoL ); 

        }
        else
        {
          TransmitProtocolGuardSwitch( mInfoTalk[tIndex].GuardID, LG_ON_HOOK, mInfoTalk[tIndex].DistPath,
                                  mInfoTalk[tIndex].DongNoH, mInfoTalk[tIndex].DongNoL, 
                                  mInfoTalk[tIndex].HooNoH, mInfoTalk[tIndex].HooNoL );  

        }   
      }
      break;
    default:
      return false;
  }

  if ( tCallEndFlg ){
    KeyLockOff();
    //KeyDataClear_NoDisplay();
    //sound_effect( NO_SOUND, 1 );
    CallManagerRepeaterOnHook( tIndex );
  }

  return true;

}


/// @brief 모든 프로토콜 중계 함수
/// @param tBuffer 프로토콜 데이터
/// @param tCallDirection 통화 방향
/// @return true = 통화 불가능 \n false = 통화 가능
U8 IsCallBusy( typeProtocolData *tBuffer, U8 tCallDirection)
{
  U8 InfoTalkChannel;
  U8 tTempId;

  for( InfoTalkChannel = 0 ; InfoTalkChannel < INFO_TALK_MAX ; InfoTalkChannel++ )
  {
    if( mInfoTalk[InfoTalkChannel].Use == false ) continue;

    switch( tCallDirection )
    {
      case CALL_DIRECTION_GR:
        if( mLineResource.IsHold != INFO_TALK_STATUS_HOLD )
        {
          if( mInfoTalk[InfoTalkChannel].GuardID == tBuffer->DeviceID ) return true;
        }
        break;
      case CALL_DIRECTION_RG:
        if( mInfoTalk[InfoTalkChannel].GuardID == (gLobbyHandle.GuardID) ) return true;
        break;
      case CALL_DIRECTION_SLR:
        if( mInfoTalk[CHANNEL_1].Direction == CALL_DIRECTION_SLR ) return true;
        if( mInfoTalk[CHANNEL_1].Direction == CALL_DIRECTION_MLR ) return true;
        if( mInfoTalk[CHANNEL_2].Direction == CALL_DIRECTION_SLR ) return true;
        if( mInfoTalk[CHANNEL_2].Direction == CALL_DIRECTION_MLR ) return true;
        break;
      case CALL_DIRECTION_SLG:
        if( mInfoTalk[InfoTalkChannel].GuardID == (tBuffer->DeviceID &0x7F) ) return true;
        break;
      case CALL_DIRECTION_MLR:
        if( mInfoTalk[CHANNEL_1].Direction == CALL_DIRECTION_SLR ) return true;
        if( mInfoTalk[CHANNEL_1].Direction == CALL_DIRECTION_MLR ) return true;
        if( mInfoTalk[CHANNEL_2].Direction == CALL_DIRECTION_SLR ) return true;
        if( mInfoTalk[CHANNEL_2].Direction == CALL_DIRECTION_MLR ) return true; 
        break;
      case CALL_DIRECTION_MLG:
        if( mInfoTalk[InfoTalkChannel].GuardID == (tBuffer->DeviceID &0x7F) ) return true;
        break;
      default:
        break;
    }
  }
  return false;
}


/// @brief 모든 프로토콜 중계 함수 (최상위)
/// @param tBuffer 프로토콜 데이터
/// @return False Only
U8 CallManagerRepeater( typeProtocolData *tBuffer ) // 20ms 
{
  U8 InfoTalkChannel;
  U16 tDongNo;
  U16 tHooNo;

  for( InfoTalkChannel = 0 ; InfoTalkChannel < INFO_TALK_MAX ; InfoTalkChannel++ )
  {
    if( mInfoTalk[InfoTalkChannel].Use == false ) continue; // 사용 하고 있는 통화 로인가?
    
    switch( mInfoTalk[InfoTalkChannel].Direction )
    {
      case CALL_DIRECTION_GR:
        if( tBuffer->ReceiveLine == RECEIVE_LINE_GUARD ){
          tDongNo = ((U16)tBuffer->DongNoH_LobbyID << 8) + ( tBuffer->DongNoL );
          HalfHex2Dec(tDongNo, &tDongNo);
          if ( tDongNo != gLobbyHandle.DongNumber ) break;
          if ( tBuffer->HooNoH != mInfoTalk[InfoTalkChannel].HooNoH ) break;
          if ( tBuffer->HooNoL != mInfoTalk[InfoTalkChannel].HooNoL ) break;          
          if( mInfoTalk[InfoTalkChannel].GuardID != (tBuffer->DeviceID) ) break;
        }
        else if( tBuffer->ReceiveLine == RECEIVE_LINE_HOUSE){
          if( mInfoTalk[InfoTalkChannel].HouseID != (tBuffer->DeviceID) ) break;
        }
        else
        {
          break;
        }
        CallManagerRepeater_GR( tBuffer, InfoTalkChannel );
        break;
      case CALL_DIRECTION_RG:
        if( tBuffer->ReceiveLine == RECEIVE_LINE_GUARD ){
          tDongNo = ((U16)tBuffer->DongNoH_LobbyID << 8) + ( tBuffer->DongNoL );
          HalfHex2Dec(tDongNo, &tDongNo);
          if ( tDongNo != gLobbyHandle.DongNumber ) break;
          if ( tBuffer->HooNoH != mInfoTalk[InfoTalkChannel].HooNoH ) break;
          if ( tBuffer->HooNoL != mInfoTalk[InfoTalkChannel].HooNoL ) break;
          if( mInfoTalk[InfoTalkChannel].GuardID != (tBuffer->DeviceID) ) break;
        }
        else if( tBuffer->ReceiveLine == RECEIVE_LINE_HOUSE){
          if( mInfoTalk[InfoTalkChannel].HouseID != (tBuffer->DeviceID) ) break;
        }
        else
        {
          break;
        }        
        CallManagerRepeater_RG( tBuffer, InfoTalkChannel );
        break;
      case CALL_DIRECTION_SLR:
        if( tBuffer->ReceiveLine == RECEIVE_LINE_LOBBY){
          tDongNo = ((U16)tBuffer->DongNoH_LobbyID << 8) + ( tBuffer->DongNoL );
          HalfHex2Dec(tDongNo, &tDongNo);
          if ( tDongNo != gLobbyHandle.DongNumber ) break;          
          if( mInfoTalk[InfoTalkChannel].LobbyID!= (tBuffer->DeviceID) ) break;
        }
        else if( tBuffer->ReceiveLine == RECEIVE_LINE_HOUSE){
          if( mInfoTalk[InfoTalkChannel].HouseID != (tBuffer->DeviceID) ) break;
        }
        else
        {
          break;
        }        
        CallManagerRepeater_SLR( tBuffer, InfoTalkChannel );
        break;
      case CALL_DIRECTION_SLG:
        if( tBuffer->ReceiveLine == RECEIVE_LINE_LOBBY){
          tDongNo = ((U16)tBuffer->DongNoH_LobbyID << 8) + ( tBuffer->DongNoL );
          HalfHex2Dec(tDongNo, &tDongNo);
          if ( tDongNo != gLobbyHandle.DongNumber ) break;          
          if( mInfoTalk[InfoTalkChannel].LobbyID!= (tBuffer->DeviceID) ) break;
        }
        else if( tBuffer->ReceiveLine == RECEIVE_LINE_GUARD ){
          tDongNo = ((U16)tBuffer->DongNoH_LobbyID << 8) + ( tBuffer->DongNoL );
          HalfHex2Dec(tDongNo, &tDongNo);
          if ( tBuffer->HooNoH != mInfoTalk[InfoTalkChannel].HooNoH ) break;
          if ( tBuffer->HooNoL != mInfoTalk[InfoTalkChannel].HooNoL ) break;          
          if ( tDongNo != gLobbyHandle.DongNumber ) break;
          if( mInfoTalk[InfoTalkChannel].GuardID != (tBuffer->DeviceID) ) break;
        }
        else
        {
          break;
        }        
        CallManagerRepeater_SLG( tBuffer, InfoTalkChannel );
        break;
      case CALL_DIRECTION_MLR:
        if( tBuffer->ReceiveLine == RECEIVE_LINE_LOBBY){
          if( mInfoTalk[InfoTalkChannel].LobbyID!= (tBuffer->DeviceID) ) break;
        }
        if( tBuffer->ReceiveLine == RECEIVE_LINE_HOUSE){
          if( mInfoTalk[InfoTalkChannel].HouseID != (tBuffer->DeviceID) ) break;
        }
        CallManagerRepeater_MLR( tBuffer, InfoTalkChannel );
        break;
      case CALL_DIRECTION_MLG:
        if( tBuffer->ReceiveLine == RECEIVE_LINE_LOBBY){
          if( mInfoTalk[InfoTalkChannel].LobbyID!= (tBuffer->DeviceID) ) break;
        }
        if( tBuffer->ReceiveLine == RECEIVE_LINE_GUARD ){
          tDongNo = ((U16)tBuffer->DongNoH_LobbyID << 8) + ( tBuffer->DongNoL );
          HalfHex2Dec(tDongNo, &tDongNo);
          if ( tDongNo != gLobbyHandle.DongNumber ) break;             
          if ( tBuffer->HooNoH != mInfoTalk[InfoTalkChannel].HooNoH ) break;
          if ( tBuffer->HooNoL != mInfoTalk[InfoTalkChannel].HooNoL ) break;
          if( mInfoTalk[InfoTalkChannel].GuardID != (tBuffer->DeviceID) ) break;
        }
        CallManagerRepeater_MLG( tBuffer, InfoTalkChannel );
        break;
      default:
        break;
    }

  }
  return false;
}


#define ANAM_HOME_PATH_1    0
#define ANAM_HOME_PATH_2    1


/// @brief 최초 통화 가능 여부 판단 하여 통화를 시작 하는 함수
/// @param tBuffer 프로토콜 데이터
/// @return true = 성공 \n false = 실패
U8 CallManagerRequest( typeProtocolData *tBuffer ) // 20ms 
{
  typeUserCallResource tUserCallResource;

  U8 tCallDirection;
  U8 tHouseID;
  U8 tHouseID16;
  U16 tDongNo;
  U16 tDongNoDec,tDongNoHex;
  U8 tID,tCmd,tPath,tLobbyID,tDongNoH,tDongNoL,tHooNoH,tHooNoL;
  U8 tWidthHouse,tHeightHouse;
  U16 tHex;
  U8 tHomePath;
  U8 tReturn;

  switch(tBuffer->Cmd)
  {
    case LG_CALL_ON:
      if ( tBuffer->DeviceID == MASTER_LOBBY_ID )
      {
        tCallDirection = CALL_DIRECTION_MLG;
      }
      else if ( ( tBuffer->DeviceID & LOBBY_ID_MASK ) == LOBBY_ID_MASK )
      {
        tCallDirection = CALL_DIRECTION_SLG;
        tDongNo = ((U16)tBuffer->DongNoH_LobbyID << 8) + ( tBuffer->DongNoL );
        HalfHex2Dec(tDongNo, &tDongNo);
        if ( tDongNo != gLobbyHandle.DongNumber )
        {
          //CallManagerRequest_ReturnBusy( tBuffer, tCallDirection );
          return false;
        }
      }
      else
      {
        return false;
      }
      CallManagerRequest_LG_CALL_ON( &tUserCallResource, tBuffer );
      break;
    case LR_CALL_ON:
      if ( tBuffer->DeviceID == MASTER_LOBBY_ID )
      {
        tCallDirection = CALL_DIRECTION_MLR;
      }
      else
      {
        tCallDirection = CALL_DIRECTION_SLR;
        tDongNo = ((U16)tBuffer->DongNoH_LobbyID << 8) + ( tBuffer->DongNoL );
        HalfHex2Dec(tDongNo, &tDongNo);
        if ( tDongNo != gLobbyHandle.DongNumber )
        {
          //CallManagerRequest_ReturnBusy( tBuffer, tCallDirection );
          return false;        
        }
      }
      CallManagerRequest_LR_CALL_ON( &tUserCallResource, tBuffer );

      tHeightHouse   = (U8)(((tBuffer->HooNoH >> 4) & 0x0F) * 10);
      tHeightHouse  += (U8)(tBuffer->HooNoH & 0x0F);
      tWidthHouse  = (U8)(((tBuffer->HooNoL >> 4) & 0x0F) * 10);
      tWidthHouse += (U8)(tBuffer->HooNoL & 0x0F);
      tHouseID16 = (U16)((tWidthHouse) + ((tHeightHouse - 1) * gLobbyHandle.MaxHo));
      
      if( (tWidthHouse) > gLobbyHandle.MaxHo || (tHouseID16 > MAX_HOUSE_ID) || NULL == tWidthHouse )
      {
          CallManagerRequest_ReturnBusy( tBuffer, tCallDirection );
          return false;
      }
      tHomePath = ANAM_HOME_PATH_1;
      break;
    case RG_CALL_ON:
      if ( tBuffer->ReceiveLine != RECEIVE_LINE_HOUSE ) return false;
      tCallDirection = CALL_DIRECTION_RG;
      CallManagerRequest_RG_CALL_ON( &tUserCallResource, tBuffer );
      tHomePath = ANAM_HOME_PATH_2;
      break;
    case GR_CALL_ON:
      tDongNo = tBuffer->DongNoH_LobbyID;
      tDongNo <<= 8;
      tDongNo += tBuffer->DongNoL;
      HalfHex2Dec( tDongNo, &tDongNoDec );
      if ( tDongNoDec != gLobbyHandle.DongNumber ) return false;
      tCallDirection = CALL_DIRECTION_GR;
      CallManagerRequest_GR_CALL_ON( &tUserCallResource, tBuffer );
      
      tHeightHouse   = (U8)(((tBuffer->HooNoH >> 4) & 0x0F) * 10);
      tHeightHouse  += (U8)(tBuffer->HooNoH & 0x0F);
      tWidthHouse  = (U8)(((tBuffer->HooNoL >> 4) & 0x0F) * 10);
      tWidthHouse += (U8)(tBuffer->HooNoL & 0x0F);
      tHouseID16 = (U16)((tWidthHouse) + ((tHeightHouse - 1) * gLobbyHandle.MaxHo));
      
      if( (tWidthHouse) > gLobbyHandle.MaxHo || (tHouseID16 > MAX_HOUSE_ID) || NULL == tWidthHouse )
      {
          CallManagerRequest_ReturnBusy( tBuffer, tCallDirection );
          return false;
      }
      tHomePath = ANAM_HOME_PATH_2;

      break;
    case RL_CALL_REQUEST:
      if ( gLobbyHandle.Protocol == PSS_KD_PROTOCOL )
      {
        TransmitProtocolHouseSwitch( tBuffer->DeviceID, LR_ON_HOOK, 0, 0 );
      }
      CallTest_RL_CALL_REQUEST( true, tBuffer );
      return false;
    case RG_FIRE:
    case RG_GAS:
    case RG_BURG1:
    case RG_EMER:
    case RG_EXIT_ON:
    case RG_EXIT_OFF:
    case RG_HOLD_OFF:
    case RG_EXIT:
    case RG_FIRE_CNCL:
    case RG_GAS_CNCL:
    case RG_BURG1_CNCL:
    case RG_BURG2:
    case RG_BURG2_CNCL:
    case RG_EMER_CNCL:
    case GR_CHK_OK:
      if ( tBuffer->ReceiveLine != RECEIVE_LINE_HOUSE ) return false;
      tID = gLobbyHandle.GuardID;
      tCmd = tBuffer->Cmd;
      tPath = 0;

      Dec2HalfHex( gLobbyHandle.DongNumber, &tDongNoHex );
      tDongNoH = (U8)((tDongNoHex>>8) & 0xFF);
      tDongNoL = (U8)((tDongNoHex) & 0xFF);

      tHouseID = (U8)(tBuffer->DeviceID &0x7F);
      tHooNoH  = (U8)(((tHouseID - 1) / gLobbyHandle.MaxHo ) + 1);
      tHooNoL  = (U8)(((tHouseID - 1) % gLobbyHandle.MaxHo ) + 1);

      Dec2HalfHex( tHooNoH, &tHex );
      tHooNoH  = tHex;
      Dec2HalfHex( tHooNoL, &tHex );
      tHooNoL  = tHex;   
      
      TransmitProtocolGuardSwitch( tID, tCmd, tPath, tDongNoH, tDongNoL, tHooNoH, tHooNoL ); 
      return false;
    case GR_CHK:
      if ( tBuffer->ReceiveLine != RECEIVE_LINE_HOUSE ) return false;
      tHeightHouse   = (U8)(((tBuffer->HooNoH >> 4) & 0x0F) * 10);
      tHeightHouse  += (U8)(tBuffer->HooNoH & 0x0F);
      tWidthHouse  = (U8)(((tBuffer->HooNoL >> 4) & 0x0F) * 10);
      tWidthHouse += (U8)(tBuffer->HooNoL & 0x0F);
      
      tID = (U8)((tWidthHouse) + ((tHeightHouse - 1) * gLobbyHandle.MaxHo));
      tCmd = tBuffer->Cmd;
      tPath = 0;
      tLobbyID = 0;
      TransmitProtocolHouseSwitch( tID, tCmd, tPath, tLobbyID );
      return false;
      
    default:
      return true;
  }

  if ( IsCallBusy( tBuffer, tCallDirection ) )
  {
    CallManagerRequest_ReturnBusy( tBuffer, tCallDirection );
    return false;
  }

  

  switch( gLobbyHandle.Protocol )
  {
    case PSS_KD_PROTOCOL:
    case PSS_HT_PROTOCOL:
    case PSS_HT_PROTOCOL_2ND:  
      tReturn = AssignResourceCall( &tUserCallResource );
      break;
    case PSS_ANAM_PROTOCOL:
      tReturn = Anam_AssignResourceCall( &tUserCallResource, tHomePath );
      break;
    default:
      tReturn = false;
      break;
  }

  if ( tReturn )
  {
    CallManagerRequest_RegisterInfoTalk( &tUserCallResource, tCallDirection );
    
    return true;
  }
  else
  {
    CallManagerRequest_ReturnBusy( tBuffer, tCallDirection );
    CallTest_RL_CALL_REQUEST( false, tBuffer );
    return false;

  }
  
}


/// @brief 통화 관리 모듈 Thread
/// @param void
/// @return void
void ThreadAnalysisProtocol(void)
{

  typeMsgBox tMsgBox;
  typeProtocolData *tData;
  U8 tID,tCmd,tPath,tLobbyID;

  wdt_reset();
  
  if( CallManagerTimeOut()) return;
  
  if ( !bReceiveMessageBox( MSG_ID_ThreadAnalysisProtocol, &tMsgBox ) ) return;
  if ( tMsgBox.Cmd == MSG_CMD_RECEIVE_PROTOCOL )
  {
    tData = (typeProtocolData*)&tMsgBox.Buf;
    if(gLobbyHandle.LobbyID == MASTER_LOBBY_ID)
    {
      wdt_reset();
      if(CallManagerRequest( tData ))
      {
        wdt_reset();
        CallManagerRepeater( tData );
      }
    }
    else
    {
      wdt_reset();
      
      if( tData->Cmd == (U8)0x00 )
      {
        if ( tData->DeviceID == (U8)0x01)
        {
          KeyFunction_RxUartSlaveLobby_factory( tData->Path );
        }
      }

      
      if(CallManagerRequest_SubLobby( tData ))
      {
        wdt_reset();
        CallManagerRepeater_SubLobby( tData );
      }
    }
  }
  

}


