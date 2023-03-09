
/// @file func_call_manager_slave.c
/// @date 2012/02/14
/// @author jinhwa Lee
/// @brief  통화 관리 모듈 3 - 기타 함수 모음


#include "main.h"


extern typeInfoTalk mInfoTalk[INFO_TALK_MAX];
extern HANDLE mhCallTimeOut[MAX_CHANNEL];
U8 mWhileSound_RL_CALL_REQUEST_flg = false;


/// @brief 통화 관리 모듈 처기화 함수
/// @param void
/// @return void
void InitializeCallManager(void)
{
  U8 i;
  
  for ( i = 0; i < MAX_CHANNEL ; i++ )
  {
    mhCallTimeOut[ i ]  = CreateTimeOut();
    
    mInfoTalk[ i ].Use = NULL;
    mInfoTalk[ i ].Channel = NULL;
    mInfoTalk[ i ].Status = INFO_TALK_STATUS_NORMAL;
    mInfoTalk[ i ].LobbyID = NULL;
    mInfoTalk[ i ].SrcPath = NULL;
    mInfoTalk[ i ].HouseID = NULL;
    mInfoTalk[ i ].GuardID = NULL;
    mInfoTalk[ i ].DistPath = NULL;
    mInfoTalk[ i ].Direction = NULL;
    mInfoTalk[ i ].DongNoH = NULL;
    mInfoTalk[ i ].DongNoL = NULL;
    mInfoTalk[ i ].HooNoH = NULL;
    mInfoTalk[ i ].HooNoL = NULL;
    //mInfoTalk[ i ].GuardPath= NULL;
  }
  
}


/// @brief 통화 관리 모듈 통화 종료 함수
/// @param tInfoTalkChannel 종료 하고자 하는 통화 채널
/// @return true only
U8 CallManagerRepeaterOnHook( U8 tInfoTalkChannel ) // 20ms 
{

  ReturnResourceCall( tInfoTalkChannel );
  
  mInfoTalk[tInfoTalkChannel].Use = false;
  mInfoTalk[tInfoTalkChannel].Channel = NULL;
  mInfoTalk[tInfoTalkChannel].Status = INFO_TALK_STATUS_NORMAL;
  mInfoTalk[tInfoTalkChannel].Direction = NULL;
  mInfoTalk[tInfoTalkChannel].LobbyID = NULL;
  mInfoTalk[tInfoTalkChannel].HouseID = NULL;
  mInfoTalk[tInfoTalkChannel].GuardID = NULL;
  mInfoTalk[tInfoTalkChannel].SrcPath = NULL;
  mInfoTalk[tInfoTalkChannel].DistPath = NULL;
  mInfoTalk[tInfoTalkChannel].DongNoH = NULL;
  mInfoTalk[tInfoTalkChannel].DongNoL = NULL;
  mInfoTalk[tInfoTalkChannel].HooNoH = NULL;
  mInfoTalk[tInfoTalkChannel].HooNoL = NULL;
  //mInfoTalk[tInfoTalkChannel].GuardPath= NULL;
  
  CancelTimeOut( mhCallTimeOut[tInfoTalkChannel] );

  return true;
  
}




/// @brief 통화 관리 모듈 타임 아웃시 실행 되는 함수
/// @param void
/// @return true = 성공 \n false = 실패
U8 CallManagerTimeOut(void)
{
  U8 tChannel;
  typeProtocolData tData = 0;
  U16 tDongNoHex;
  
  for ( tChannel = 0; tChannel < MAX_CHANNEL ; tChannel++ )
  {
    if ( IsTimeOut( mhCallTimeOut[tChannel] ) )
    {
      Dec2HalfHex( gLobbyHandle.DongNumber, &tDongNoHex );
      tData.DeviceID = gLobbyHandle.LobbyID;
      tData.Cmd  = ___ON_HOOK;
      tData.ReceiveLine = RECEIVE_LINE_TIMEOUT;
      tData.DongNoH_LobbyID = tDongNoHex >> 8;
      tData.DongNoL = tDongNoHex & 0x00ff;
          
      if ( gLobbyHandle.LobbyID == MASTER_LOBBY_ID )
      {
        switch( mInfoTalk[tChannel].Direction )
        {
          case CALL_DIRECTION_GR:
            CallManagerRepeater_GR( &tData, tChannel );
            return true;
          case CALL_DIRECTION_RG:
            CallManagerRepeater_RG( &tData, tChannel );
            return true;
          case CALL_DIRECTION_SLR:
            CallManagerRepeater_SLR( &tData, tChannel );
            return true;
          case CALL_DIRECTION_SLG:
            CallManagerRepeater_SLG( &tData, tChannel );
            return true;
          case CALL_DIRECTION_MLR:
            CallManagerRepeater_MLR( &tData, tChannel );
            return true;
          case CALL_DIRECTION_MLG:
            CallManagerRepeater_MLG( &tData, tChannel );
            return true;
          default:
            return true;
        }
      }
      else
      {
        switch( mInfoTalk[tChannel].Direction )
        {
          case CALL_DIRECTION_MLR:
            CallManagerRepeater_MLR_SubLobby( &tData, tChannel );
            return true;
          case CALL_DIRECTION_MLG:
            CallManagerRepeater_MLG_SubLobby( &tData, tChannel );
            return true;
          default:
            return true;
        }
      }
    }
  }
  return false;
}


/// @brief 경비실 -> 세대 통화 시작 함수
/// @param tUserCallResource 통화 자원 관리 변수
/// @param typeProtocolData 프로토콜 데이터
/// @return void
void CallManagerRequest_GR_CALL_ON( typeUserCallResource *tUserCallResource, typeProtocolData *tBuffer )
{
  U8 tHouseID;
  U8 tWidthHouse,tHeightHouse;

  tUserCallResource->PathName[INFO_TALK_CH0] = PATH_NAME_GUARD;
  tUserCallResource->PathName[INFO_TALK_CH1] = PATH_NAME_HOUSE;

  tHeightHouse   = (U8)(((tBuffer->HooNoH >> 4) & 0x0F) * 10);
  tHeightHouse  += (U8)(tBuffer->HooNoH & 0x0F);
  tWidthHouse    = (U8)(((tBuffer->HooNoL >> 4) & 0x0F) * 10);
  tWidthHouse   += (U8)(tBuffer->HooNoL & 0x0F);
  
  tHouseID = (U8)((tHeightHouse - 1) * gLobbyHandle.MaxHo);
  tHouseID += tWidthHouse;
  
  tUserCallResource->HouseID = tHouseID;
  tUserCallResource->LobbyID = NULL;
  tUserCallResource->GuardID = (U8)(tBuffer->DeviceID &0x7F);
  
  tUserCallResource->DongNoH = tBuffer->DongNoH_LobbyID;
  tUserCallResource->DongNoL = tBuffer->DongNoL;
  tUserCallResource->HooNoH = tBuffer->HooNoH;
  tUserCallResource->HooNoL = tBuffer->HooNoL;

  //tUserCallResource->GuardPath = tBuffer->Path;
}


/// @brief 로비 -> 세대 통화 시작 함수
/// @param tUserCallResource 통화 자원 관리 변수
/// @param typeProtocolData 프로토콜 데이터
/// @return void
void CallManagerRequest_LR_CALL_ON( typeUserCallResource *tUserCallResource, typeProtocolData *tBuffer )
{
  U8 tHouseID;
  U8 tWidthHouse,tHeightHouse;

  tHeightHouse   = (U8)(((tBuffer->HooNoH >> 4) & 0x0F) * 10);
  tHeightHouse  += (U8)(tBuffer->HooNoH & 0x0F);
  tWidthHouse  = (U8)(((tBuffer->HooNoL >> 4) & 0x0F) * 10);
  tWidthHouse += (U8)(tBuffer->HooNoL & 0x0F);
  tHouseID = (U8)((tWidthHouse) + ((tHeightHouse - 1) * gLobbyHandle.MaxHo));

  switch( tBuffer->DeviceID )
  {
    case MASTER_LOBBY_ID:
      tUserCallResource->PathName[INFO_TALK_CH0] = PATH_NAME_MASTER_LOBBY; break;
    case SLAVE_LOBBY1_ID:
      tUserCallResource->PathName[INFO_TALK_CH0] = PATH_NAME_SUB_LOBBY_1; break;
    case SLAVE_LOBBY2_ID:
      tUserCallResource->PathName[INFO_TALK_CH0] = PATH_NAME_SUB_LOBBY_2; break;
    case SLAVE_LOBBY3_ID:
      tUserCallResource->PathName[INFO_TALK_CH0] = PATH_NAME_SUB_LOBBY_3; break;
    default:
      tUserCallResource->PathName[INFO_TALK_CH0] = PATH_NAME_SUB_LOBBY_NEXT; break;
  }
  tUserCallResource->PathName[INFO_TALK_CH1] = PATH_NAME_HOUSE;
  
  tUserCallResource->HouseID = tHouseID;
  tUserCallResource->LobbyID = tBuffer->DeviceID;
  tUserCallResource->GuardID = NULL;

  tUserCallResource->DongNoH = tBuffer->DongNoH_LobbyID;
  tUserCallResource->DongNoL = tBuffer->DongNoL;
  tUserCallResource->HooNoH = tBuffer->HooNoH;
  tUserCallResource->HooNoL = tBuffer->HooNoL;  

}


/// @brief 로비 -> 경비 통화 시작 함수
/// @param tUserCallResource 통화 자원 관리 변수
/// @param typeProtocolData 프로토콜 데이터
/// @return void
void CallManagerRequest_LG_CALL_ON( typeUserCallResource *tUserCallResource, typeProtocolData *tBuffer )
{
  U8 tHouseID;
  U8 tHooNoH,tHooNoL;
  U16 tDongNoHex;
  
  if ( gLobbyHandle.Protocol != PSS_KD_PROTOCOL )
  {
    switch( tBuffer->DeviceID )
    {
      case MASTER_LOBBY_ID:
        tUserCallResource->PathName[INFO_TALK_CH0] = PATH_NAME_MASTER_LOBBY;
        //tHouseID = (tBuffer->DeviceID &0x7F);
        //tHooNoH = ( (tHouseID - 1) / gLobbyHandle.MaxHo ) + 1;
        //tHooNoL = ( tHouseID % gLobbyHandle.MaxHo );
        //tUserCallResource->HooNoH = tHooNoH;
        //tUserCallResource->HooNoL = tHooNoL;
        tUserCallResource->HooNoH = (U8)(tBuffer->DeviceID &0x3F);
        tUserCallResource->HooNoL = 0;
        break;
      case SLAVE_LOBBY1_ID:
        tUserCallResource->PathName[INFO_TALK_CH0] = PATH_NAME_SUB_LOBBY_1;
        tUserCallResource->HooNoH = (U8)(tBuffer->DeviceID &0x3F);
        tUserCallResource->HooNoL = 0;
        break;
      case SLAVE_LOBBY2_ID:
        tUserCallResource->PathName[INFO_TALK_CH0] = PATH_NAME_SUB_LOBBY_2; 
        tUserCallResource->HooNoH = (U8)(tBuffer->DeviceID &0x3F);
        tUserCallResource->HooNoL = 0;
        break;
      case SLAVE_LOBBY3_ID:
        tUserCallResource->PathName[INFO_TALK_CH0] = PATH_NAME_SUB_LOBBY_3; 
        tUserCallResource->HooNoH = (U8)(tBuffer->DeviceID &0x3F);
        tUserCallResource->HooNoL = 0;
        break;
      default:
        tUserCallResource->PathName[INFO_TALK_CH0] = PATH_NAME_SUB_LOBBY_NEXT; 
        tUserCallResource->HooNoH = (U8)(tBuffer->DeviceID &0x3F);
        tUserCallResource->HooNoL = 0;
        break;
    }
  }
  else
  {
    switch( tBuffer->DeviceID )
    {
      case MASTER_LOBBY_ID:
        tUserCallResource->PathName[INFO_TALK_CH0] = PATH_NAME_MASTER_LOBBY;
        //tHouseID = (tBuffer->DeviceID &0x7F);
        //tHooNoH = ( (tHouseID - 1) / gLobbyHandle.MaxHo ) + 1;
        //tHooNoL = ( tHouseID % gLobbyHandle.MaxHo );
        //tUserCallResource->HooNoH = tHooNoH;
        //tUserCallResource->HooNoL = tHooNoL;
        tUserCallResource->HooNoH = 0;
        tUserCallResource->HooNoL = (U8)(tBuffer->DeviceID &0x3F);
        break;
      case SLAVE_LOBBY1_ID:
        tUserCallResource->PathName[INFO_TALK_CH0] = PATH_NAME_SUB_LOBBY_1;
        tUserCallResource->HooNoH = 0;
        tUserCallResource->HooNoL = (U8)(tBuffer->DeviceID &0x3F);

        break;
      case SLAVE_LOBBY2_ID:
        tUserCallResource->PathName[INFO_TALK_CH0] = PATH_NAME_SUB_LOBBY_2; 
        tUserCallResource->HooNoH = 0;
        tUserCallResource->HooNoL = (U8)(tBuffer->DeviceID &0x3F);

        break;
      case SLAVE_LOBBY3_ID:
        tUserCallResource->PathName[INFO_TALK_CH0] = PATH_NAME_SUB_LOBBY_3; 
        tUserCallResource->HooNoH = 0;
        tUserCallResource->HooNoL = (U8)(tBuffer->DeviceID &0x3F);

        break;
      default:
        tUserCallResource->PathName[INFO_TALK_CH0] = PATH_NAME_SUB_LOBBY_NEXT; 
        tUserCallResource->HooNoH = 0;
        tUserCallResource->HooNoL = (U8)(tBuffer->DeviceID &0x3F);
        break;
    }
  }
            
  tUserCallResource->PathName[INFO_TALK_CH1] = PATH_NAME_GUARD;
  
  tUserCallResource->HouseID = NULL;//(tBuffer->HomeGuardID &0x7F); // 서브 로비에서 통화중 101호 호출시 ID 겹침 현상 제거
  tUserCallResource->LobbyID = (U8)(tBuffer->DeviceID &0x7F);
  tUserCallResource->GuardID = gLobbyHandle.GuardID;
  //tUserCallResource->GuardPath = gLobbyHandle.GuardID;

 // Dec2HalfHex( gLobbyHandle.DongNumber, &tDongNoHex );
  
  tUserCallResource->DongNoH = tBuffer->DongNoH_LobbyID;
  tUserCallResource->DongNoL = tBuffer->DongNoL;
  //tUserCallResource->HooNoH = tBuffer->HooNoH;
  //tUserCallResource->HooNoL = tBuffer->HooNoL;  



}


/// @brief 세대 -> 경비 통화 시작 함수
/// @param tUserCallResource 통화 자원 관리 변수
/// @param typeProtocolData 프로토콜 데이터
/// @return void
void CallManagerRequest_RG_CALL_ON( typeUserCallResource *tUserCallResource, typeProtocolData *tBuffer )
{
  U8 tHouseID;
  U8 tHooNoH,tHooNoL;
  U16 tDongNoHex;
  U16 tHex;

  tUserCallResource->PathName[INFO_TALK_CH0] = PATH_NAME_HOUSE;
  tUserCallResource->PathName[INFO_TALK_CH1] = PATH_NAME_GUARD;
  
  tHouseID = (U8)(tBuffer->DeviceID &0x7F);
  tHooNoH = (U8)(( (tHouseID - 1) / gLobbyHandle.MaxHo ) + 1);
  tHooNoL = (U8)( ((tHouseID - 1) % gLobbyHandle.MaxHo ) + 1);
  
  tUserCallResource->HouseID = tHouseID;
  tUserCallResource->LobbyID = NULL;
  tUserCallResource->GuardID = gLobbyHandle.GuardID;
  //tUserCallResource->GuardPath = gLobbyHandle.GuardID;

  Dec2HalfHex( gLobbyHandle.DongNumber, &tDongNoHex );
  
  tUserCallResource->DongNoH = (U8)(tDongNoHex>>8);
  tUserCallResource->DongNoL = (U8)(tDongNoHex & 0x00ff);

  Dec2HalfHex( tHooNoH, &tHex );
  tUserCallResource->HooNoH = tHex;
  Dec2HalfHex( tHooNoL, &tHex );
  tUserCallResource->HooNoL = tHex;

}


/// @brief 임시 통화 자원 관리 함수를 실제 통화 자원 관리 함수로 적용하는 함수
/// @param tUserCallResource 통화 자원 관리 변수
/// @param tCallDirection 통화 방향
/// @return void
void CallManagerRequest_RegisterInfoTalk( typeUserCallResource *tUserCallResource,U8 tCallDirection )
{
  U8 tChannel;
  
  tChannel = tUserCallResource->Channel;
  mInfoTalk[tChannel].Use = true;
  mInfoTalk[tChannel].Channel   = tUserCallResource->Channel;
  mInfoTalk[tChannel].Status    = INFO_TALK_STATUS_CALL_OK;
  mInfoTalk[tChannel].Direction = tCallDirection;
  mInfoTalk[tChannel].HouseID    = tUserCallResource->HouseID;
  mInfoTalk[tChannel].LobbyID   = tUserCallResource->LobbyID;
  mInfoTalk[tChannel].GuardID   = tUserCallResource->GuardID;
  mInfoTalk[tChannel].DongNoH   = tUserCallResource->DongNoH;
  mInfoTalk[tChannel].DongNoL   = tUserCallResource->DongNoL;
  mInfoTalk[tChannel].HooNoH    = tUserCallResource->HooNoH;
  mInfoTalk[tChannel].HooNoL    = tUserCallResource->HooNoL;
  mInfoTalk[tChannel].SrcPath   = tUserCallResource->PathNumber[INFO_TALK_CH0];
  mInfoTalk[tChannel].DistPath  = tUserCallResource->PathNumber[INFO_TALK_CH1];
  //mInfoTalk[tChannel].GuardPath = tUserCallResource->GuardPath;

  SetupTimeOut( mhCallTimeOut[tChannel], CALL_OK_TIME_OUT_3SEC );///

}


/// @brief 통화가 비지일때 각 기기로 비지 프로토콜을 전송하는 함수
/// @param tBuffer 수신된 프로토콜 
/// @param tCallDirection 통화 방향
/// @return void
void CallManagerRequest_ReturnBusy(typeProtocolData *tBuffer,  U8 tCallDirection )
{
  U8 tID,tCmd,tPath,tLobbyID,tDongNoH,tDongNoL,tHooNoH,tHooNoL;
  
  switch(tCallDirection)
  {
    case CALL_DIRECTION_MLG:
    case CALL_DIRECTION_MLR:
      KeyLockOff();
      sound_effect( BUSY_TONE, 1 );
      break;
    case CALL_DIRECTION_SLG:
      tCmd = GL_CBUSY;
      tPath = 0;
      tLobbyID = tBuffer->DeviceID;
      tDongNoH = tBuffer->DongNoH_LobbyID;
      tDongNoL = tBuffer->DongNoL;
      tHooNoH = tBuffer->HooNoH;
      tHooNoL = tBuffer->HooNoL;      
      TransmitProtocolLobbySwitch( tLobbyID, tCmd, tPath, tDongNoH, tDongNoL,tHooNoH, tHooNoL );
      break;
    case CALL_DIRECTION_SLR:
      tCmd = RL_CBUSY;
      tPath = 0;
      tLobbyID = tBuffer->DeviceID;
      tDongNoH = tBuffer->DongNoH_LobbyID;
      tDongNoL = tBuffer->DongNoL;
      tHooNoH = tBuffer->HooNoH;
      tHooNoL = tBuffer->HooNoL;      
      TransmitProtocolLobbySwitch( tLobbyID, tCmd, tPath, tDongNoH, tDongNoL,tHooNoH, tHooNoL );   
      break;
    case CALL_DIRECTION_RG:
      tID = tBuffer->DeviceID;
      tCmd = GR_CBUSY;
      tPath = 0;
      tLobbyID = 0;
      TransmitProtocolHouseSwitch( tID, tCmd, tPath, tLobbyID );      
      break;
    case CALL_DIRECTION_GR:
      tID = tBuffer->DeviceID;
      tCmd = RG_CBUSY;
      tPath = tBuffer->Path;
      tDongNoH = tBuffer->DongNoH_LobbyID;
      tDongNoL = tBuffer->DongNoL;
      tHooNoH = tBuffer->HooNoH;
      tHooNoL = tBuffer->HooNoL;
      TransmitProtocolGuardSwitch( tID, tCmd, tPath, tDongNoH, tDongNoL, tHooNoH, tHooNoL );     
      break;        
    default:
      break;  
  }
}




/// @brief 통화 테스트 요청 프로토콜 전송 함수
/// @param tID 세대 ID
/// @param tCmd Command
/// @param tPath 통화로
/// @param tLobbyID LobbyID
/// @return 없음
U8 TransmitProtocol_RL_CALL_REQUEST(U8 tID, U8 tCmd, U8 tPath, U8 tLobbyID )
{
  TransmitProtocolHouseSwitch( tID, tCmd, tPath, tLobbyID );
}


/// @brief 통화 테스트 요청 하는 함수
/// @param tStart True = Start \n False = Normal
/// @param tData 프로토콜 데이터
/// @return true = 성공 \n false = 실패
U8 CallTest_RL_CALL_REQUEST( U8 tStart, typeProtocolData *tData )
{
  typeMsgBox tMsgBuffer;
  U8 tID,tCmd,tPath,tLobbyID,tDongNoH,tDongNoL,tHooNoH,tHooNoL;
  U8 tHouseID;
  U16 tHex;

  tPath = (tData->Path & 0x0F);
  
  if ( tStart )
  {
    if ( tPath == 1 || tPath == 2 )
    {
      if ( mInfoTalk[0].Use == false && mInfoTalk[1].Use == false )
      {
        gLobbyHandle.AudioPathHouse[0] = false;
        gLobbyHandle.AudioPathHouse[1] = false;
        gLobbyHandle.AudioPathHouse[ tPath - 1 ] = true;
        
        tHouseID = (U8)(tData->DeviceID &0x7F);
        tHooNoH  = (U8)(((tHouseID - 1) / gLobbyHandle.MaxHo ) + 1);
        tHooNoL  = (U8)(((tHouseID - 1) % gLobbyHandle.MaxHo ) + 1);
        
        Dec2HalfHex( tHooNoH, &tHex );
        tHooNoH  = tHex;
        Dec2HalfHex( tHooNoL, &tHex );
        tHooNoL  = tHex;   

        tMsgBuffer.DstID = MSG_ID_ThreadAnalysisProtocol;
        tMsgBuffer.SrcID = MSG_ID_KeyMainFunction;
        tMsgBuffer.Cmd   = MSG_CMD_RECEIVE_PROTOCOL;
        tMsgBuffer.Buf.Byte[0] = gLobbyHandle.LobbyID;
        tMsgBuffer.Buf.Byte[1] = LR_CALL_ON;
        tMsgBuffer.Buf.Byte[2] = 0;
        tMsgBuffer.Buf.Byte[3] = 0;
        tMsgBuffer.Buf.Byte[4] = 0;
        tMsgBuffer.Buf.Byte[5] = tHooNoH;
        tMsgBuffer.Buf.Byte[6] = tHooNoL;
        tMsgBuffer.Buf.Byte[8] = RECEIVE_LINE_KEY;
        
        bSendMessageBox( &tMsgBuffer );
        
        SettingValueHexFnd_TEST();

        mWhileSound_RL_CALL_REQUEST_flg = true;

        return true;
      }
    }
    TransmitProtocol_RL_CALL_REQUEST((U8)(tData->DeviceID&0x7F),RL_CALL_REQUEST,0,0);
  }
  else
  {
    mWhileSound_RL_CALL_REQUEST_flg = false;
    InitializeLobbyHandle();
  }
  return false;
}


