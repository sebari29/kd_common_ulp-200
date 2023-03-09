
/// @file   func_etc.c
/// @date   2012/02/14
/// @author jinhwa Lee
/// @brief  Door RFID 관련 파일

#include "main.h"

U8 gDoorOpenMode = false;
U16 gDoorOpenDelay;


/// @brief DoorOpen 메시지 전송 함수
/// @param void
/// @return void
void MsgDoorOpenfunc(void)
{
  typeMsgBox tMsgBox;

  tMsgBox.DstID = MSG_ID_ThreadDoorOpen;
  tMsgBox.SrcID = 0;
  tMsgBox.Cmd   = MSG_CMD_DOOR_OPEN;
  bSendMessageBox( &tMsgBox );
  sound_effect(OPEN_TONE, 1);
}


/// @brief Door Open Thread
/// @param void
/// @return void
void ThreadDoorOpen(void) // 540ms
{
  typeMsgBox tMsgBox;

  wdt_reset();
  if ( bReceiveMessageBox( MSG_ID_ThreadDoorOpen, &tMsgBox ) )
  {
    if ( tMsgBox.Cmd == MSG_CMD_DOOR_OPEN )
    {
      DR_M_OPEN = SET;
      gDoorOpenMode = true;
      gDoorOpenDelay = (U16)(gLobbyHandle.OpenTimeDoor * 1000) ;
      gDoorOpenDelay /= THREAD_DOOR_OPEN_TIME;
      gDoorOpenDelay += 2; //1초이면 
      SettingValueHexFnd_Open( true );
    }
  }

  if ( gDoorOpenMode == true )
  {
    gDoorOpenDelay--;
    if( gDoorOpenDelay ) return;
    SettingValueHexFnd_Open( false );
    DR_M_OPEN = CLEAR;
    gDoorOpenMode = false;
    //LobbyHandleReroad();
  }

}

// ////////////////// RF ID //////////////////////

HANDLE hTimeOutRFCard;
U8 mReadDataRFCard[5];

/// @brief ThreadRFCard 함수로 수신 RFID 값 메시지 송신
/// @param[in] tIsRX true = 수신 \n false = 송신
/// @param[in/out] tData 송수신될 데이터
/// @return void
void SendMsgRFCard( U8 tIsRX, U8 *tData )
{
  typeMsgBox tMsgBox;

  tMsgBox.DstID = MSG_ID_ThreadRFCard;
  tMsgBox.SrcID = 0;

  if ( tIsRX == true )
  {
    tMsgBox.Cmd  = tIsRX;
    tMsgBox.Buf.Byte[0] = tData[0];
    tMsgBox.Buf.Byte[1] = tData[1];
    tMsgBox.Buf.Byte[2] = tData[2];
    tMsgBox.Buf.Byte[3] = tData[3];
    tMsgBox.Buf.Byte[4] = tData[4];
  }
  else
  {
    tMsgBox.Cmd  = tIsRX;
    tMsgBox.Buf.Byte[0] = tData[0];
  }
  bSendMessageBox( &tMsgBox );
}



/// @brief RF CARD Initalize
/// @param void
/// @return void
void InitalizeRFCard(void)
{
   hTimeOutRFCard = CreateTimeOut();
}


/// @brief RFID Thread 함수
/// @param void
/// @return void
void ThreadRFCard(void) // 100ms
{
//  static U8 TxCmd = NULL;
  static U8 mRFCardMode = RF_READER_MODE_NORMAL;
  typeMsgBox tMsgBox;
  U8 tIsRx;
  U8 tCmd;
  
  if ( IsTimeOut(hTimeOutRFCard) )
  {
    mRFCardMode = RF_READER_MODE_NORMAL;
    SettingValueHexFnd_RF(false, NULL);
    sound_effect(ERROR_TONE, 1 );
  }

  if ( bReceiveMessageBox( MSG_ID_ThreadRFCard, &tMsgBox ) )
  {
    tIsRx = tMsgBox.Cmd;
    
    if ( tIsRx )
    {
      mReadDataRFCard[0] = tMsgBox.Buf.Byte[0];
      mReadDataRFCard[1] = tMsgBox.Buf.Byte[1];
      mReadDataRFCard[2] = tMsgBox.Buf.Byte[2];
      mReadDataRFCard[3] = tMsgBox.Buf.Byte[3];
      mReadDataRFCard[4] = tMsgBox.Buf.Byte[4];

      tCmd = mReadDataRFCard[0];
      
      switch( mRFCardMode )
      {
        case RF_READER_MODE_NORMAL:
          if ( (tCmd & 0x0F) == RF_READER_CMD_SUCCESS_MARK )
          {
            MsgDoorOpenfunc();
            CancelTimeOut(hTimeOutRFCard);
          }
          if ( tCmd == RF_READER_CMD_NORMAL_MASTERCARD )
          {
            mRFCardMode = RF_READER_MODE_WAIT;
            SettingValueHexFnd_RF(true, RF_READER_MODE_NORMAL);
            SetupTimeOut( hTimeOutRFCard, 80);
          }
          break;
        case RF_READER_MODE_WAIT:
          if ( tCmd == RF_READER_CMD_HOUSE_REGISTER_MODE )
          {
            mRFCardMode = RF_READER_MODE_HOUSE;
            SettingValueHexFnd_RF(true, RF_READER_MODE_HOUSE);
            sound_effect(SUCCESS_TONE, 1 );
            SetupTimeOut( hTimeOutRFCard, 80);
          }          
          if ( tCmd == RF_READER_CMD_HOUSE_DELETE_MODE )
          {
            mRFCardMode = RF_READER_MODE_DELETE;
            SettingValueHexFnd_RF(true, RF_READER_MODE_DELETE);
            sound_effect(SUCCESS_TONE, 1 );
            SetupTimeOut( hTimeOutRFCard, 80);
          }
          break;            
        case RF_READER_MODE_HOUSE:
        case RF_READER_MODE_DELETE:
        case RF_READER_MODE_MASTER:
          SetupTimeOut( hTimeOutRFCard, 80);
          if ( tCmd == RF_READER_CMD_NORMAL_MODE )
          {
            mRFCardMode = RF_READER_MODE_NORMAL;
            SettingValueHexFnd_RF(false, NULL);
            CancelTimeOut(hTimeOutRFCard);
            sound_effect(END_TONE_2, 1 );
            CancelTimeOut(hTimeOutRFCard);
          }
          
          break;
        default:
          break;
      }
      
      if(gLobbyHandle.PacketMonitoring)
      {
        bPrintHex( mReadDataRFCard, 5 );
        EnterDisplay();
      }
      

    }
    else
    {
      tCmd = tMsgBox.Buf.Byte[0];
      wr_RFCard( tCmd );
      
      sound_effect(SUCCESS_TONE, 1 );

      if( tCmd == RF_READER_CMD_MASTER_REGISTER_MODE )
      {
        mRFCardMode = RF_READER_MODE_MASTER;
        SettingValueHexFnd_RF(true, RF_READER_MODE_MASTER);
        SetupTimeOut( hTimeOutRFCard, 80);
      }
//        TxCmd = tCmd;
    }

  }
}

//  ////////////// ETC FUNCTION ///////////////

/*never used
U8 Chr2Num( U8 tChr )
{
  U8 tReturn;
  
  if ( tChr >= '0' && tChr <= '9' )
  {
    tReturn = (U8)(tChr - '0');
  }
  else if((tChr >= 'a') && (tChr <= 'z'))
  {
    tReturn = (U8)(tChr - 'a');
  }
  else if((tChr >= 'A') && (tChr <= 'Z'))
  {
    tReturn = (U8)(tChr - 'A');
  }
  else
  {
    tReturn = NULL;
  }
  
  return tReturn;
}
*/


/// @brief 10진수를 16진수로 변환
/// @param tDec 변환한 10진수
/// @return 변환된 16진수
U16 Dec2Hex( U8 tDec )
{
  U16 tHex;
  U8  tTemp[2];
  U8  i;

  tTemp[0] = (U8)(tDec >> 4);
  tTemp[1] = (U8)(tDec & 0x0F);
  tHex = 0;

  for ( i = 0 ; i < 2; i++ )
  {
    tHex <<= 8;
    if( tTemp[i] > 9 ){
      tTemp[i] -= 10;
      tHex += 65 + tTemp[i];
    }
    else
    {
      tHex += 48 + tTemp[i];
    }
  }
  return tHex;
}


/// @brief 16진수(0~9 까지만 있는)를 10진수로 변환
/// @param[in] bHalfHex 변환할 16진수(0~9 까지만 있는)
/// @param[out] bDec 변환 되어 저장될 10진수 포인터
/// @return void
void HalfHex2Dec( U16 bHalfHex, U16 *bDec )
{
  U16 tHex;
  U16 tDec;
  U8 i;
  
  U8 tTemp[4];

  tTemp[0] = (U8)((bHalfHex) & 0x0F);
  tTemp[1] = (U8)((bHalfHex>>4) & 0x0F);
  tTemp[2] = (U8)((bHalfHex>>8) & 0x0F);
  tTemp[3] = (U8)((bHalfHex>>12) & 0x0F);

  tDec  = tTemp[0];
  tDec += tTemp[1] * 10;
  tDec += tTemp[2] * 100;
  tDec += tTemp[3] * 1000;

  *bDec = tDec;
}


/// @brief 10진수를 16진수(0~9 까지만 있는)로 변환
/// @param[in] bDec 변환할 10진수 데이터
/// @param[out] bHalfHex 16진수(0~9 까지만 있는)로 변환 되어 저장 될 포이터
/// @return void
U8 Dec2HalfHex( U16 tDec, U16 *tHalfHex )
{
  U16 tHex = 0;
  U16 tTempDec;
  U8 i;
  U8 tTemp[4];

  if ( tDec > 9999 ) return false;


  tTempDec = tDec / 1000;
  tTemp[0] = tTempDec;

  tTempDec = tDec % 1000;
  tTempDec = tTempDec / 100;
  tTemp[1] = tTempDec;

  tTempDec = tDec % 100;
  tTempDec = tTempDec / 10;
  tTemp[2] = tTempDec;

  tTempDec = tDec % 10;
  tTemp[3] = tTempDec;

  for ( i = 0 ; i < 4 ; i ++ )
  {
    tHex <<= 4;
    tHex += tTemp[i];
  }
  *tHalfHex = tHex;  

  return true;

}



U8 ProtocolSpecSetting(void)
{
  switch( gLobbyHandle.Protocol )
  {
    case PSS_KD_PROTOCOL:
      InitializeUart0_19200();
      InitializeUart1_9600();
      InitializeUart2_9600();      
      break;
    case PSS_HT_PROTOCOL:
    case PSS_HT_PROTOCOL_2ND:
      InitializeUart0_19200();
      InitializeUart1_3906();
      InitializeUart2_3906();
      break;
    case PSS_ANAM_PROTOCOL:
      /*
      InitializeUart0_19200();
      InitializeUart1_3906();
      InitializeUart2_Disable();
      */
      InitializeUart0_19200();
      InitializeUart1_9600();
      InitializeUart2_Disable();
      break;
    default:
      break;
  }
}


