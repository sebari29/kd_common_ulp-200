
/// @file func_call_manager_resource.c
/// @date 2012/02/14
/// @author jinhwa Lee
/// @brief  ��ȭ ���� ��� 2 - ������ ��ȭ �ڿ� ����


#include "main.h"


typeLineResource    mLineResource;
extern typeInfoTalk mInfoTalk[INFO_TALK_MAX];


/// @brief ��ȭ �ڿ� �ʱ�ȭ �Լ�
/// @param void
/// @return void
void InitializeResourceCall(void)
{
  U8 i,j;

  mLineResource.House[0] = false;
  mLineResource.House[1] = false;
  mLineResource.MasterLobby = false;
  mLineResource.SubLobby[0] = false;
  mLineResource.SubLobby[1] = false;
  mLineResource.Guard[0] = false;
  mLineResource.Guard[1] = false;
  mLineResource.IsHold = false;

  for ( i = 0 ; i < MAX_CHANNEL ; i++ )
  {
    for ( j = 0 ; j < MAX_MUX ; j++ )
    {
      mInfoTalk[i].Use = false;
      mInfoTalk[i].PathName[j] = false;
      mInfoTalk[i].PathNumber[j] = false;
    }
  }

}


/// @brief ��ȭ �ڿ�(����Ʈ����/ä��)�� �Ҵ� ���ִ� �Լ�
/// @param tUserCallResource ��ȭ �ڿ� ����
/// @return true = ���� \n false = ����
U8 AssignResourceCall( typeUserCallResource *tUserCallResource )
{
  U8 i;

  for ( i = 0 ; i < MAX_CHANNEL ; i++ )
  {
    if ( mInfoTalk[i].Use == true ) continue;
    
    tUserCallResource->Channel = i;
    if ( !AssignResourceLine ( tUserCallResource ) ) return false;
    
    mInfoTalk[i].PathName[PATH_1] = tUserCallResource->PathName[PATH_1];
    mInfoTalk[i].PathName[PATH_2] = tUserCallResource->PathName[PATH_2];
    mInfoTalk[i].PathNumber[PATH_1] = tUserCallResource->PathNumber[PATH_1];
    mInfoTalk[i].PathNumber[PATH_2] = tUserCallResource->PathNumber[PATH_2];
    mInfoTalk[i].Use = true;
    
    SelectCallMux( tUserCallResource );
    SelectVideoMux( );
    SelectCameraMic();
    //SelectRelay();
  
    return true;
  }
  return false;
}

/// @brief �Ƴ� ���� ��ȭ �ڿ�(����Ʈ����/ä��)�� �Ҵ� ���ִ� �Լ�
/// @param tUserCallResource ��ȭ �ڿ� ����
/// @return true = ���� \n false = ����
U8 Anam_AssignResourceCall( typeUserCallResource *tUserCallResource, U8 tHomePath  )
{
  U8 i;

  for ( i = 0 ; i < MAX_CHANNEL ; i++ )
  {
    if ( mInfoTalk[i].Use == true ) continue;
    
    tUserCallResource->Channel = i;
    if ( !Anam_AssignResourceLine ( tUserCallResource, tHomePath ) ) return false;
    
    mInfoTalk[i].PathName[PATH_1] = tUserCallResource->PathName[PATH_1];
    mInfoTalk[i].PathName[PATH_2] = tUserCallResource->PathName[PATH_2];
    mInfoTalk[i].PathNumber[PATH_1] = tUserCallResource->PathNumber[PATH_1];
    mInfoTalk[i].PathNumber[PATH_2] = tUserCallResource->PathNumber[PATH_2];
    mInfoTalk[i].Use = true;
    
    SelectCallMux( tUserCallResource );
    SelectVideoMux( );
    SelectCameraMic();
    //SelectRelay();
  
    return true;
  }
  return false;
}


/// @brief �Ҵ�� ��ȭ �ڿ�(����Ʈ����/ä��)�� ��ȯ �ϴ� �Լ�
/// @param tChannel ��ȯ �ϰ��� �ϴ� ä��
/// @return true = ���� \n false = ����
U8 ReturnResourceCall( U8 tChannel )
{
    
  typeUserCallResource tUserCallResource;

  tUserCallResource.Channel = tChannel;
  tUserCallResource.PathName[PATH_1] = mInfoTalk[tChannel].PathName[PATH_1];
  tUserCallResource.PathName[PATH_2] = mInfoTalk[tChannel].PathName[PATH_2];
  tUserCallResource.PathNumber[PATH_1] = mInfoTalk[tChannel].PathNumber[PATH_1];
  tUserCallResource.PathNumber[PATH_2] = mInfoTalk[tChannel].PathNumber[PATH_2];
  //tUserCallResource.Use = false;
  
  ReturnResourceLine ( &tUserCallResource );
  
  mInfoTalk[tChannel].PathName[PATH_1] = tUserCallResource.PathName[PATH_1];
  mInfoTalk[tChannel].PathName[PATH_2] = tUserCallResource.PathName[PATH_2];
  mInfoTalk[tChannel].PathNumber[PATH_1] = tUserCallResource.PathNumber[PATH_1];
  mInfoTalk[tChannel].PathNumber[PATH_2] = tUserCallResource.PathNumber[PATH_2];
  mInfoTalk[tChannel].Use = false;//tUserCallResource.Use;
  
  SelectCallMux( &tUserCallResource );
  SelectVideoMux(  );
  SelectCameraMic();
  SelectRelay();


  return true;
  
}


/// @brief �Ҵ�� ��ȭ �ڿ�(�ϵ����)�� ��ȯ �ϴ� �Լ�
/// @param tUserCallResource ��ȯ �ϰ��� �ϴ� ��ȭ �ڿ�
/// @return true = ���� \n false = ����
U8 ReturnResourceLine ( typeUserCallResource *tUserCallResource )
{
  U8 uLineCnt,uMuxCnt;
  typeLineResource TempLineResource;

  TempLineResource.House[PATH_1] = mLineResource.House[PATH_1];
  TempLineResource.House[PATH_2] = mLineResource.House[PATH_2];
  TempLineResource.MasterLobby  = mLineResource.MasterLobby;
  TempLineResource.SubLobby[PATH_1] = mLineResource.SubLobby[PATH_1];
  TempLineResource.SubLobby[PATH_2] = mLineResource.SubLobby[PATH_2];
  TempLineResource.Guard[PATH_1] = mLineResource.Guard[PATH_1];
  TempLineResource.Guard[PATH_2] = mLineResource.Guard[PATH_2];
  
  for ( uMuxCnt = 0 ; uMuxCnt < MAX_MUX ; uMuxCnt++ )
  {
    uLineCnt = (U8)(tUserCallResource->PathNumber[uMuxCnt] - 1);
    
    switch( tUserCallResource->PathName[uMuxCnt] )
    {
      case PATH_NAME_HOUSE:
        TempLineResource.House[uLineCnt] = NULL;
        break;
      case PATH_NAME_MASTER_LOBBY:
        TempLineResource.MasterLobby = NULL;
        break;
      case PATH_NAME_SUB_LOBBY_1:
      case PATH_NAME_SUB_LOBBY_2:
      case PATH_NAME_SUB_LOBBY_3:
      case PATH_NAME_SUB_LOBBY_NEXT:
        TempLineResource.SubLobby[uLineCnt] = NULL;
        break;
      case PATH_NAME_GUARD:
        TempLineResource.Guard[uLineCnt] = NULL;
        break;
      default:
        return false;
    }
    tUserCallResource->PathName[uMuxCnt] = NULL;
    tUserCallResource->PathNumber[uMuxCnt] = NULL;
  }

  mLineResource.House[PATH_1] = TempLineResource.House[PATH_1];
  mLineResource.House[PATH_2] = TempLineResource.House[PATH_2];
  mLineResource.MasterLobby  = TempLineResource.MasterLobby;
  mLineResource.SubLobby[PATH_1] = TempLineResource.SubLobby[PATH_1];
  mLineResource.SubLobby[PATH_2] = TempLineResource.SubLobby[PATH_2];
  mLineResource.Guard[PATH_1] = TempLineResource.Guard[PATH_1];
  mLineResource.Guard[PATH_2] = TempLineResource.Guard[PATH_2];
  
  return true;
}


/// @brief ��ȭ �ڿ�(�ϵ����)�� �Ҵ� ���ִ� �Լ�
/// @param tUserCallResource �ӽ� ��ȭ �ڿ� ����
/// @return true = ���� \n false = ����
U8 AssignResourceLine ( typeUserCallResource *tUserCallResource )
{
  U8 uLineCnt,uMuxCnt;
  typeLineResource TempLineResource;
  U8 tReturnCnt = 0;

  TempLineResource.House[PATH_1] = mLineResource.House[PATH_1];
  TempLineResource.House[PATH_2] = mLineResource.House[PATH_2];
  TempLineResource.MasterLobby  = mLineResource.MasterLobby;
  TempLineResource.SubLobby[PATH_1] = mLineResource.SubLobby[PATH_1];
  TempLineResource.SubLobby[PATH_2] = mLineResource.SubLobby[PATH_2];
  TempLineResource.Guard[PATH_1] = mLineResource.Guard[PATH_1];
  TempLineResource.Guard[PATH_2] = mLineResource.Guard[PATH_2];

  
  for ( uMuxCnt = 0 ; uMuxCnt < MAX_MUX ; uMuxCnt++ ) // cCannel �� ä���� 0�� Mux�� 1�� �Խ� ���������� ����
  {
    switch( tUserCallResource->PathName[uMuxCnt] )
    {
      case PATH_NAME_HOUSE:
        for ( uLineCnt = 0 ; uLineCnt < MAX_LINE; uLineCnt++ )
        {
          if ( TempLineResource.House[uLineCnt] || (!gLobbyHandle.AudioPathHouse[uLineCnt]) )  continue; // ���� ���� �� ��� �� ���� Ȯ��
          TempLineResource.House[uLineCnt] = true;
          tUserCallResource->PathNumber[uMuxCnt] = (U8)(uLineCnt + 1);
          tReturnCnt++;
          break;
        }
        break;
      case PATH_NAME_MASTER_LOBBY:
        if ( TempLineResource.MasterLobby ) return false;// ���� ���� �� ��� �̤� ���� Ȯ��
        TempLineResource.MasterLobby = true;
        tUserCallResource->PathNumber[uMuxCnt] = 1;
        tReturnCnt++;
        break; 
      case PATH_NAME_SUB_LOBBY_1:
      case PATH_NAME_SUB_LOBBY_2:
      case PATH_NAME_SUB_LOBBY_3:
      case PATH_NAME_SUB_LOBBY_NEXT:
        for ( uLineCnt = 0 ; uLineCnt < MAX_LINE; uLineCnt++ )
        {
          if ( TempLineResource.SubLobby[uLineCnt] || (!gLobbyHandle.AudioPathLobby[uLineCnt]) ) continue; // ���� ���� �� ��� �̤� ���� Ȯ��
          TempLineResource.SubLobby[uLineCnt] = true;
          tUserCallResource->PathNumber[uMuxCnt] = (U8)(uLineCnt + 1);
          tReturnCnt++;
          break;
        }
        break;
      case PATH_NAME_GUARD:
        for ( uLineCnt = 0 ; uLineCnt < MAX_LINE; uLineCnt++ )
        {
          if( mLineResource.IsHold != INFO_TALK_STATUS_HOLD )
          {
            if ( TempLineResource.Guard[uLineCnt] || (!gLobbyHandle.AudioPathGuard[uLineCnt]) ) continue; // ���� ���� �� ��� �̤� ���� Ȯ��
          }
          TempLineResource.Guard[uLineCnt] = true;
          tUserCallResource->PathNumber[uMuxCnt] = (U8)(uLineCnt + 1);
          tReturnCnt++;
          break;
        }
        break;
      default:
        break;
    }
  }

  if( tReturnCnt < 2 ) return false;

  mLineResource.House[PATH_1] = TempLineResource.House[PATH_1];
  mLineResource.House[PATH_2] = TempLineResource.House[PATH_2];
  mLineResource.MasterLobby  = TempLineResource.MasterLobby;
  mLineResource.SubLobby[PATH_1] = TempLineResource.SubLobby[PATH_1];
  mLineResource.SubLobby[PATH_2] = TempLineResource.SubLobby[PATH_2];
  mLineResource.Guard[PATH_1] = TempLineResource.Guard[PATH_1];
  mLineResource.Guard[PATH_2] = TempLineResource.Guard[PATH_2];
  
  return true;
}


/// @brief -�Ƴ� ����- ��ȭ �ڿ�(�ϵ����)�� �Ҵ� ���ִ� �Լ�
/// @param tUserCallResource �ӽ� ��ȭ �ڿ� ����
/// @return true = ���� \n false = ����
U8 Anam_AssignResourceLine ( typeUserCallResource *tUserCallResource, U8 tHomePath )
{
  U8 uLineCnt,uMuxCnt;
  typeLineResource TempLineResource;
  U8 tReturnCnt = 0;

  TempLineResource.House[PATH_1] = mLineResource.House[PATH_1];
  TempLineResource.House[PATH_2] = mLineResource.House[PATH_2];
  TempLineResource.MasterLobby  = mLineResource.MasterLobby;
  TempLineResource.SubLobby[PATH_1] = mLineResource.SubLobby[PATH_1];
  TempLineResource.SubLobby[PATH_2] = mLineResource.SubLobby[PATH_2];
  TempLineResource.Guard[PATH_1] = mLineResource.Guard[PATH_1];
  TempLineResource.Guard[PATH_2] = mLineResource.Guard[PATH_2];

  
  for ( uMuxCnt = 0 ; uMuxCnt < MAX_MUX ; uMuxCnt++ ) // cCannel �� ä���� 0�� Mux�� 1�� �Խ� ���������� ����
  {
    switch( tUserCallResource->PathName[uMuxCnt] )
    {
      case PATH_NAME_HOUSE:
        uLineCnt = tHomePath;
        if ( TempLineResource.House[uLineCnt] || (!gLobbyHandle.AudioPathHouse[uLineCnt]) )  break; // ���� ���� �� ��� �� ���� Ȯ��
        TempLineResource.House[uLineCnt] = true;
        tUserCallResource->PathNumber[uMuxCnt] = (U8)(uLineCnt + 1);
        tReturnCnt++;
        break;
      case PATH_NAME_MASTER_LOBBY:
        if ( TempLineResource.MasterLobby ) return false;// ���� ���� �� ��� �̤� ���� Ȯ��
        TempLineResource.MasterLobby = true;
        tUserCallResource->PathNumber[uMuxCnt] = 1;
        tReturnCnt++;
        break; 
      case PATH_NAME_SUB_LOBBY_1:
      case PATH_NAME_SUB_LOBBY_2:
      case PATH_NAME_SUB_LOBBY_3:
      case PATH_NAME_SUB_LOBBY_NEXT:
        for ( uLineCnt = 0 ; uLineCnt < MAX_LINE; uLineCnt++ )
        {
          if ( TempLineResource.SubLobby[uLineCnt] || (!gLobbyHandle.AudioPathLobby[uLineCnt]) ) continue; // ���� ���� �� ��� �̤� ���� Ȯ��
          TempLineResource.SubLobby[uLineCnt] = true;
          tUserCallResource->PathNumber[uMuxCnt] = (U8)(uLineCnt + 1);
          tReturnCnt++;
          break;
        }
        break;
      case PATH_NAME_GUARD:
        for ( uLineCnt = 0 ; uLineCnt < MAX_LINE; uLineCnt++ )
        {
          if( mLineResource.IsHold != INFO_TALK_STATUS_HOLD )
          {
            if ( TempLineResource.Guard[uLineCnt] || (!gLobbyHandle.AudioPathGuard[uLineCnt]) ) continue; // ���� ���� �� ��� �̤� ���� Ȯ��
          }
          TempLineResource.Guard[uLineCnt] = true;
          tUserCallResource->PathNumber[uMuxCnt] = (U8)(uLineCnt + 1);
          tReturnCnt++;
          break;
        }
        break;
      default:
        break;
    }
  }

  if( tReturnCnt < 2 ) return false;

  mLineResource.House[PATH_1] = TempLineResource.House[PATH_1];
  mLineResource.House[PATH_2] = TempLineResource.House[PATH_2];
  mLineResource.MasterLobby  = TempLineResource.MasterLobby;
  mLineResource.SubLobby[PATH_1] = TempLineResource.SubLobby[PATH_1];
  mLineResource.SubLobby[PATH_2] = TempLineResource.SubLobby[PATH_2];
  mLineResource.Guard[PATH_1] = TempLineResource.Guard[PATH_1];
  mLineResource.Guard[PATH_2] = TempLineResource.Guard[PATH_2];
  
  return true;
}

//////////////////////////////////////////////////////////////////////////////

/// @brief ���� �Խ��� ��Ȳ�� �°� ������ �ִ� �Լ�
/// @param void
/// @return void
void SelectVideoMux( void )
{
  U8 i,j;
  U8 tCnt = 0;
  U8 tChannel = 0;
  //U8 mVideoMux;

  for ( i=0; i<MAX_CHANNEL; i++)
  {
    switch( mInfoTalk[i].PathName[MUX_1] )
    {
      case PATH_NAME_MASTER_LOBBY:
        if ( mInfoTalk[i].PathName[MUX_2] == PATH_NAME_GUARD ) break;
        VIDEO_SEL1 = 0;
        VIDEO_SEL2 = 0;
        VIDEO_SEL3 = 0;
        CAM_PWR_ON = true;
        //mVideoMux = 1;
        return;
      case PATH_NAME_SUB_LOBBY_1:
        if ( mInfoTalk[i].PathName[MUX_2] == PATH_NAME_GUARD ) break;
        VIDEO_SEL1 = 1;
        VIDEO_SEL2 = 0;
        VIDEO_SEL3 = 0;
        CAM_PWR_ON = false;
        //mVideoMux = 2;
        return;
      case PATH_NAME_SUB_LOBBY_2:
        if ( mInfoTalk[i].PathName[MUX_2] == PATH_NAME_GUARD ) break;
        VIDEO_SEL1 = 0;
        VIDEO_SEL2 = 1;
        VIDEO_SEL3 = 0;
        CAM_PWR_ON = false;
        //mVideoMux = 3;
        return;    
      case PATH_NAME_SUB_LOBBY_3:
        if ( mInfoTalk[i].PathName[MUX_2] == PATH_NAME_GUARD ) break;
        VIDEO_SEL1 = 0;
        VIDEO_SEL2 = 0;
        VIDEO_SEL3 = 1;
        CAM_PWR_ON = false;
        //mVideoMux = 4;
        return;    
     default:
        //mVideoMux = 0;
        break;      
    }
  }

  VIDEO_SEL1 = 0;
  VIDEO_SEL2 = 0;
  VIDEO_SEL3 = 0;
  CAM_PWR_ON = false;
  
}


/// @brief ī�޶�� ����ũ /����Ŀ��  ��Ȳ�� �°� ������ �ִ� �Լ�
/// @param void
/// @return void
void SelectCameraMic( void )
{
  if( mLineResource.MasterLobby == true ){
    EN_MC = false;
    SpeakerOnOff( SPK_ONOFF_ON, SPK_ONOFF_LOBBY);
  }
  else
  {
    CAM_PWR_ON = false;
    EN_MC = true;
    SpeakerOnOff( SPK_ONOFF_OFF, SPK_ONOFF_LOBBY);
  }
}


/// @brief �����̸� ��Ȳ�� �°� ������ �ִ� �Լ�
/// @param void
/// @return void
void SelectRelay(void)
{
  if( mLineResource.House[CHANNEL_1] == true ) HOUSE_CALL1 = true;
  else  HOUSE_CALL1 = false;

  if( mLineResource.House[CHANNEL_2] == true ) HOUSE_CALL2 = true;
  else  HOUSE_CALL2 = false;
  
  if( mLineResource.SubLobby[CHANNEL_1] == true ) SUB_CALL1 = true;
  else  SUB_CALL1 = false;

  if( mLineResource.SubLobby[CHANNEL_2] == true ) SUB_CALL2 = true;
  else  SUB_CALL2 = false;

  if( mLineResource.Guard[CHANNEL_1] == true ) GUARD_CALL= true;
  else  GUARD_CALL = false;

  //if( mLineResource.Guard[CHANNEL_2] == true ) MASTER_CALL2 = true;
  //else  MASTER_CALL2 = false;

}

U8 mMuxData[4];


/// @brief ��ȭ �Խ� ���� �Լ�
/// @param tChannel ��ȭ ä��
/// @param tMux �����ϰ��� �ϴ� ��ȭ �Խ�
/// @param tPath ��ȭ Path ����
/// @param tData ��ȭ ����
/// @return void
void SelectMuxChannelDir( U8 tChannel, U8 tMux, U8 tPath, U8 tData )
{

  if ( tPath > 1 ) tData += (tPath - 1);
  
  if ( tChannel == CHANNEL_2 )
  { 
    if( tMux == MUX_1)
    {
      MUX_CH1A_SEL1 = (U8)((tData) & 0x01);
      MUX_CH1A_SEL2 = (U8)((tData>>1) & 0x01);
      MUX_CH1A_SEL3 = (U8)((tData>>2) & 0x01);
      mMuxData[0] = tData;
    }
    else
    {
      MUX_CH1B_SEL1 = (U8)((tData) & 0x01);
      MUX_CH1B_SEL2 = (U8)((tData>>1) & 0x01);
      MUX_CH1B_SEL3 = (U8)((tData>>2) & 0x01);
      mMuxData[1] = tData;
    }
  }
  else
  {
    if( tMux == MUX_1)
    {
      MUX_CH2A_SEL1 = (U8)((tData) & 0x01);
      MUX_CH2A_SEL2 = (U8)((tData>>1) & 0x01);
      MUX_CH2A_SEL3 = (U8)((tData>>2) & 0x01);
      mMuxData[2] = tData;
    }
    else
    {
      MUX_CH2B_SEL1 = (U8)((tData) & 0x01);
      MUX_CH2B_SEL2 = (U8)((tData>>1) & 0x01);
      MUX_CH2B_SEL3 = (U8)((tData>>2) & 0x01);
      mMuxData[3] = tData;
    }
  }
}


/// @brief ��ȭ �Խ��� ��Ȳ�� �°� ������ �ִ� �Լ�
/// @param tUserCallResource ��ȭ �ڿ� ����
/// @return void
void SelectCallMux( typeUserCallResource *tUserCallResource )
{
  U8 tMuxCnt,tChannel;

  tChannel = tUserCallResource->Channel;
  nMUX_EN = CLEAR;

  for ( tMuxCnt = 0 ; tMuxCnt < MAX_MUX ; tMuxCnt++ )
  {
    switch( tUserCallResource->PathName[tMuxCnt] )
    {
      case PATH_NAME_MASTER_LOBBY:
        SelectMuxChannelDir( tChannel, tMuxCnt, tUserCallResource->PathNumber[tMuxCnt], DIR_MUX_MASTERLOBBY );
        break;
      case PATH_NAME_SUB_LOBBY_1:
      case PATH_NAME_SUB_LOBBY_2:
      case PATH_NAME_SUB_LOBBY_3:
      case PATH_NAME_SUB_LOBBY_NEXT:
        SelectMuxChannelDir( tChannel, tMuxCnt, tUserCallResource->PathNumber[tMuxCnt], DIR_MUX_SUBLOBBY );
        break;
      case PATH_NAME_HOUSE:
        SelectMuxChannelDir( tChannel, tMuxCnt, tUserCallResource->PathNumber[tMuxCnt], DIR_MUX_HOUSE );
        break;
      case PATH_NAME_GUARD:
        SelectMuxChannelDir( tChannel, tMuxCnt, tUserCallResource->PathNumber[tMuxCnt], DIR_MUX_GUARD );
        break;
      default:
        SelectMuxChannelDir( tChannel, tMuxCnt, 1, DIR_MUX_NULL );
        break;
    }
  }

}







