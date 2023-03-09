
/// @file   func_key.c
/// @date   2012/02/14
/// @author jinhwa Lee
/// @brief  Key �Է� ���� ����

#include "main.h"


U8 gbKeyMap[16] = {12,13,      // ��� / ȣ��
                     1,2,3,      //  1 / 2 / 3
                     4,5,6,      //  4 / 5 / 6
                     7,8,9,      //  7 / 8 / 9
                     11,0,10};   //  * / 0 / #
extern U16 gFwVersion;

typeKeyScan mKey;
HANDLE mKeyTimeOut;

U16 bpreKeyVal = 0;
U8  bReleaseCnt[SCAN_LINE_X*SCAN_LINE_Y] = {0,};


/// @brief KeyScan �ʱ�ȭ �Լ�
/// @param void
/// @return void
void InitializeKeyScan(void)
{
  mKeyTimeOut  = CreateTimeOut();
}


/// @brief KeyScan �Լ�
/// @param void
/// @return void
void ThreadKeyScan(void)
{
  U8 i, j;
  U16 tKeyVal;
  
  __wdt_reset();
  for(i=0;i<SCAN_LINE_Y;i++)
  {
    KEY_OUT = 0xFF;
    KEY_DIR = (U8)(0x01 << i);
    KEY_OUT = 0x00;
    
    for(j=0;j<SCAN_LINE_X;j++)
    {
      if((KEY_IN & (0x10<<j)) == 0)
      {

        tKeyVal = (i * SCAN_LINE_X) + j;
        tKeyVal = 0x01 << tKeyVal;
        if ( bpreKeyVal & tKeyVal)
       // if(bpreKeyVal & (0x01<<(U16)((i*SCAN_LINE_X)+j)))
        {
          if(++bReleaseCnt[(i*SCAN_LINE_X+j)] == KEY_RELEASE)
          {
            bReleaseCnt[(i*SCAN_LINE_X+j)] = 0;
            ExeSwRoutine((U8)(i*SCAN_LINE_X+j), STATUS_ON);
          }
        } 
        else
        {
          tKeyVal = (i * SCAN_LINE_X) + j;
          tKeyVal = 0x01 << tKeyVal;
          bpreKeyVal |= tKeyVal;
        }
      }
      else
      {

        tKeyVal = (i * SCAN_LINE_X) + j;
        tKeyVal = ~(0x01 << tKeyVal);
        bpreKeyVal &= tKeyVal;

        
        bReleaseCnt[(i*SCAN_LINE_X+j)] = 0;
        ExeSwRoutine((U8)(i*SCAN_LINE_X+j), STATUS_OFF);
      }
    }
  }
  __wdt_reset();

  if ( IsTimeOut ( mKeyTimeOut ) )
  {
    //KeyMainFunctionMasterOnHook();
    mKey.Mode = KEY_MODE_NORMAL;
    mKey.KeyShadow = 0;
    //CancelTimeOut( mKeyTimeOut );
    KeyDataClear();
    KeyLockOff();
  }
  
  if ( NULL == mKey.TimeCnt-- )
  {
    mKey.TimeCnt = (500/THREAD_KEY_SCAN_TIME);
    if ( mKey.RealData == NULL )
    {
      SettingValueHexFnd_Circle();
    }
  }
  
  __wdt_reset();
}


/// @brief KeyScan �� ���� ���� �Լ�
/// @param swdata �Էµ� Ű ������
/// @param status �Էµ� Ű ����
/// @return void
void ExeSwRoutine(U8 swdata, U8 status)
{
  static U16 keyReleaseTimeCnt[SCAN_LINE_X*SCAN_LINE_Y] = {0,};
  static U8 keyPushedFlag[SCAN_LINE_X*SCAN_LINE_Y] = {0,};

  if(status == STATUS_OFF)
  {
    keyReleaseTimeCnt[swdata] = 0;
    keyPushedFlag[swdata] = 0;
  }
  else if(status == STATUS_ON)
  {

    if(keyPushedFlag[swdata] == 1)
    {
      //SettingValueHexFnd(swdata | 0x1000);
      keyReleaseTimeCnt[swdata]++;
      if( keyReleaseTimeCnt[swdata] > (1500/THREAD_KEY_SCAN_TIME) )
      {
        if ( gbKeyMap[swdata] == KEY_STAR )
        {
          while(1);
        }
      }
    }
    else
    {
      //debug_puts("Pushed keypad %d \r\n", swdata);

      KeyMainFunction(gbKeyMap[swdata]);
      
      keyPushedFlag[swdata] = 1;
      keyReleaseTimeCnt[swdata] = 0;
    }
  }
}

/// @brief �н����� �˻��ϴ� �Լ�
/// @param tKeyRealData �˻��� �н����� ������
/// @return True = Success \n False = Error
U8 PasswordCompare( U32 tKeyRealData )
{ 
  U16 tHalfHex;

  Dec2HalfHex( gLobbyHandle.Password , &tHalfHex );
  
  if ( tKeyRealData == tHalfHex )
  {
    MsgDoorOpenfunc();
    return true;
  }
  else
  {
    //sound_effect(ERROR_TONE, 1);
    return false;
  }
}


/// @brief �Էµ� �ɵ����� ���� ( ���÷��̵� �ʱ�ȭ )
/// @param void
/// @return void
void KeyDataClear(void)
{
  mKey.RealData = CLEAR;
  mKey.DisplayData = CLEAR;
  mKey.KeyCnt = CLEAR;
  //mKey.Mode = KEY_MODE_NORMAL;
  SettingValueHexFnd( mKey.DisplayData );
  SettingValueHexFnd_Number();
}


/// @brief �Էµ� �ɵ����� ���� ( ���÷��̴� �ʱ�ȭ ���� )
/// @param void
/// @return void
void KeyDataClear_NoDisplay(void)
{
  mKey.RealData = CLEAR;
  mKey.DisplayData = CLEAR;
  //mKey.Mode = KEY_MODE_NORMAL;
  mKey.KeyCnt= CLEAR;
}





/// @brief ���� ����϶� FND ��� �ϴ� �Լ�
/// @param void
/// @return void
void SettingValueDisplay(void)
{
  U8 tSendRFData;
  U16 tHalfHex;

  switch(mKey.SettingSwitch)
  {
    case ADMIN_DONG_NUMBER:
      Dec2HalfHex( gLobbyHandle.DongNumber, &tHalfHex );
      SettingValueHexFnd_Page1_NULL( tHalfHex );
      break;
    case ADMIN_LOBBY_ID:
      Dec2HalfHex( (gLobbyHandle.LobbyID & 0x3F), &tHalfHex );
      SettingValueHexFnd_Page1_NULL( tHalfHex );
      break;
    case ADMIN_MAX_HOO:
      Dec2HalfHex( gLobbyHandle.MaxHo, &tHalfHex );
      SettingValueHexFnd_Page1_NULL( tHalfHex );
      break;
    case ADMIN_GUARD_ID:
      Dec2HalfHex( gLobbyHandle.GuardID, &tHalfHex );
      SettingValueHexFnd_Page1_NULL( tHalfHex );
      break;
    case ADMIN_PROTOCOL:
      Dec2HalfHex( gLobbyHandle.Protocol, &tHalfHex );
      SettingValueHexFnd_Page1_NULL( tHalfHex );
      break;
    case ADMIN_DOOR_TIME:
      Dec2HalfHex( gLobbyHandle.OpenTimeDoor, &tHalfHex );
      SettingValueHexFnd_Page1_NULL( tHalfHex );
      break;
    case ADMIN_SOUND_MUTE:
      Dec2HalfHex( gLobbyHandle.BeepSoundMute, &tHalfHex );
      SettingValueHexFnd_Page1_NULL( tHalfHex );
      break;      
    case ADMIN_RFCARD_MASTER:
      tSendRFData = RF_READER_CMD_MASTER_REGISTER_MODE;
      SendMsgRFCard( false, &tSendRFData );
      mKey.Mode = KEY_MODE_NORMAL;
      CancelTimeOut( mKeyTimeOut );
      KeyDataClear();
      KeyLockOff();
      break;
    case ADMIN_RFCARD_ALL_DEL:
      SettingValueHexFnd_Page1_NULL_Cnt(0, 1);
      break;
    case ADMIN_HOME_PASSWORD_CHANGE:
      if (PasswordReadHome( (U8)(mKey.HooNumber >> 8), (U8)(mKey.HooNumber & 0xFF), (U8*)&tHalfHex ))
      {
        Dec2HalfHex( (U16)tHalfHex, &tHalfHex );
        SettingValueHexFnd_Page1( tHalfHex );
      }
      else
      {
        SettingValueHexFnd_Password();
      }
      break;
    case ADMIN_COMMON_PASSWORD_CHANGE:
      Dec2HalfHex( gLobbyHandle.Password, &tHalfHex );
      SettingValueHexFnd_Page1( tHalfHex );
      break;
    case ADMIN_MANAGER_PASSWORD_CHANGE:
      Dec2HalfHex( gLobbyHandle.ManagerPassword, &tHalfHex );
      SettingValueHexFnd_Page1( tHalfHex );
      break;
    case ADMIN_FW_VERSION_CHECK:  
      SettingValueHexFnd_Page1_Version(gFwVersion);
      KeyLockOn();
      break;
    case ADMIN_HOME_PASSWORD_ALL_DEL:
      SettingValueHexFnd_Page1_NULL_Cnt(0, 1);
      break;
    default:
      break;
      //return false;
  }
  //return false;
}


/// @brief ������ ���Ѱ� ���Ѱ� �˻��ϴ� �Լ�
/// @param tData �˻��� ������
/// @param tMax  ���Ѱ�
/// @param tMin  ���Ѱ�
/// @return True = ���� \n False = ����
U8 SettingValueSaveCheck( U32 tData, U32 tMax, U32 tMin)
{
  if ( tData > tMax || tData < tMin) return false;
  return true;
}


/// @brief Ű�� �Է��� �������� EEPROM�� ���� �ϴ� �Լ�
/// @param void
/// @return True = �÷��� ����Ʈ ���� \n False = �÷��� ����Ʈ ���� 
U8 SettingValueSave(void)
{
  U16 tData;
  U16 tAddr;
  U8  tData8[2];
  U8  tNull[4] = 0;
  U8 tSendRFData;
  switch(mKey.SettingSwitch)
  {
    case ADMIN_DONG_NUMBER:
      HalfHex2Dec( (U16)mKey.RealData, &tData );
      if ( SettingValueSaveCheck( tData, MAX_DongNumber, MIN_DongNumber) == false ) return false;
      tAddr = (((U32)&gLobbyHandle.DongNumber - (U32)&gLobbyHandle.DongNumber) * 2 ) + LOBBY_HANDLE_ADDR;
      if ( SpecialVal_WriteBlock( tAddr, (U8*)&tData, 2))
      {
        gLobbyHandle.DongNumber = (U16)tData;  
        sound_effect(SUCCESS_TONE,1);
        return true;
      }
      break;
    case ADMIN_LOBBY_ID:
      HalfHex2Dec( (U16)mKey.RealData, &tData );
      if ( SettingValueSaveCheck( tData, MAX_LobbyID, MIN_LobbyID) == false ) return false;
      tData |= LOBBY_ID_MASK;
      tAddr = (((U32)&gLobbyHandle.LobbyID - (U32)&gLobbyHandle.DongNumber) * 2 ) + LOBBY_HANDLE_ADDR;
      if ( SpecialVal_WriteBlock( tAddr, (U8*)&tData, 1))
      {
        gLobbyHandle.LobbyID = (U16)tData;  
        KDP_LobbySetupChannel();
        sound_effect(SUCCESS_TONE,1);
        return true;
      }
      break;
    case ADMIN_MAX_HOO:
      HalfHex2Dec( (U16)mKey.RealData, &tData );
      if ( SettingValueSaveCheck( tData, MAX_MaxHo, MIN_MaxHo) == false ) return false;
      tAddr = (((U32)&gLobbyHandle.MaxHo - (U32)&gLobbyHandle.DongNumber) * 2 ) + LOBBY_HANDLE_ADDR;
      if ( SpecialVal_WriteBlock( tAddr, (U8*)&tData, 1))
      {
        gLobbyHandle.MaxHo = (U16)tData;  
        sound_effect(SUCCESS_TONE,1);
        return true;
      }
      break;      
    case ADMIN_GUARD_ID:
      HalfHex2Dec( (U16)mKey.RealData, &tData );
      if ( SettingValueSaveCheck( tData, MAX_GuardID, MIN_GuardID) == false ) return false;
      tAddr = (((U32)&gLobbyHandle.GuardID - (U32)&gLobbyHandle.DongNumber) * 2 ) + LOBBY_HANDLE_ADDR;
      if ( SpecialVal_WriteBlock( tAddr, (U8*)&tData, 1))
      {
        gLobbyHandle.GuardID = (U16)tData;  
        sound_effect(SUCCESS_TONE,1);
        return true;
      }  
      break;
    case ADMIN_PROTOCOL:
      HalfHex2Dec( (U16)mKey.RealData, &tData );
      if ( SettingValueSaveCheck( tData, MAX_Protocol, MIN_Protocol) == false ) return false;
      if ( tData == PSS_HT_PROTOCOL || tData == PSS_HT_PROTOCOL_2ND ) return false; // �������� ���� �ȵ�
      tAddr = (((U32)&gLobbyHandle.Protocol - (U32)&gLobbyHandle.DongNumber) * 2 ) + LOBBY_HANDLE_ADDR;
      if ( SpecialVal_WriteBlock( tAddr, (U8*)&tData, 1))
      {
        gLobbyHandle.Protocol = (U16)tData;  
        ProtocolSpecSetting();
        sound_effect(SUCCESS_TONE,1);
        return true;
      }  
      break;
    case ADMIN_DOOR_TIME:
      HalfHex2Dec( (U16)mKey.RealData, &tData );
      if ( SettingValueSaveCheck( tData, MAX_OpenTimeDoor, MIN_OpenTimeDoor) == false ) return false;
      tAddr = (((U32)&gLobbyHandle.OpenTimeDoor - (U32)&gLobbyHandle.DongNumber) * 2 ) + LOBBY_HANDLE_ADDR;
      if ( SpecialVal_WriteBlock( tAddr, (U8*)&tData, 1))
      {
        gLobbyHandle.OpenTimeDoor = (U16)tData;  
        sound_effect(SUCCESS_TONE,1);
        return true;
      }  
      break;
    case ADMIN_SOUND_MUTE:
      HalfHex2Dec( (U16)mKey.RealData, &tData );
      if ( SettingValueSaveCheck( tData, MAX_BeepSoundMute, MIN_BeepSoundMute) == false ) return false;
      tAddr = (((U32)&gLobbyHandle.BeepSoundMute - (U32)&gLobbyHandle.DongNumber) * 2 ) + LOBBY_HANDLE_ADDR;
      if ( SpecialVal_WriteBlock( tAddr, (U8*)&tData, 1))
      {
        if ( tData == true )
        {
          sound_effect(SUCCESS_TONE,1);
          delay_ms(1000);
          gLobbyHandle.BeepSoundMute = (U16)tData;
        }
        else
        {
          gLobbyHandle.BeepSoundMute = (U16)tData;  
          sound_effect(SUCCESS_TONE,1);
        }
        return true;
      }  
    case ADMIN_MANAGER_PASSWORD_CHANGE:
      if ( mKey.KeyCnt != 4 ) return false;
      HalfHex2Dec( (U16)mKey.RealData, &tData );
      if ( SettingValueSaveCheck( tData, MAX_ManagerPassword, MIN_ManagerPassword) == false ) return false;
      tAddr = (((U32)&gLobbyHandle.ManagerPassword - (U32)&gLobbyHandle.DongNumber) * 2 ) + LOBBY_HANDLE_ADDR;
      if ( SpecialVal_WriteBlock( tAddr, (U8*)&tData, 2))
      {
        gLobbyHandle.ManagerPassword = (U16)tData;  
        sound_effect(SUCCESS_TONE,1);
        return true;
      }      
      break;
    case ADMIN_COMMON_PASSWORD_CHANGE:
      if ( mKey.KeyCnt != 4 ) return false;
      HalfHex2Dec( (U16)mKey.RealData, &tData );
      if ( SettingValueSaveCheck( tData, MAX_ManagerPassword, MIN_ManagerPassword) == false ) return false;
      tAddr = (((U32)&gLobbyHandle.Password - (U32)&gLobbyHandle.DongNumber) * 2 ) + LOBBY_HANDLE_ADDR;
      if ( SpecialVal_WriteBlock( tAddr, (U8*)&tData, 2))
      {
        gLobbyHandle.Password = (U16)tData;  
        sound_effect(SUCCESS_TONE,1);
        return true;
      }      
      break;
    case ADMIN_HOME_PASSWORD_CHANGE:
      if ( mKey.KeyCnt != 4 ) return false;
      HalfHex2Dec( (U16)mKey.RealData, &tData );
      if ( SettingValueSaveCheck( tData, MAX_ManagerPassword, MIN_ManagerPassword) == false ) return false;
      if (PasswordWriteHome( (U8)(mKey.HooNumber >> 8), (U8)(mKey.HooNumber & 0xFF), (U8*)&tData ))
      {
        SettingValueHexFnd_House();
        mKey.Mode = KEY_MODE_CHANGE_HOME_NUM_INPUT;
        KeyDataClear_NoDisplay();
        CancelTimeOut( mKeyTimeOut );
        sound_effect(SUCCESS_TONE,1);
        return (true + 1);
      }
      break;
    case ADMIN_HOME_PASSWORD_ALL_DEL:
      HalfHex2Dec( (U16)mKey.RealData, &tData );
      if ( tData == 1 )
      {
        PasswordHomeAllDel();
        sound_effect(SUCCESS_TONE,1);
        return true;
      }
      break;
    case ADMIN_RFCARD_ALL_DEL:
      HalfHex2Dec( (U16)mKey.RealData, &tData );
      if ( tData == 1 )
      {
        tSendRFData = RF_READER_CMD_ALL_DELETE;
        SendMsgRFCard( false, &tSendRFData );
        delay_ms(1000);
        return true;
      }
      break;
    default:
      //sound_effect(ERROR_TONE,1);
      return true;
  }
  
  sound_effect(ERROR_TONE,1);
  return true;
}


/// @brief �Էµ� Ű ���� ��� ��ȣ ���� ���� ��� ���� ��ȣ ���� ���ϴ� �Լ�
/// @param void
/// @return True = ���� ��� ���� \n False = ��� ��ȣ �Է� �Ǵ� ��Ÿ
U8 KeyCompareFunction( void )
{

  U8 tPassword[4];
  U8 tHooNumber[2];
  U16 tTemp16bit;
  
  if ( (mKey.KeyCnt > KEY_COUNT_MAX) && ( mKey.HooNumber == 0x00 )) //Admin Mode
  {
    if ( (mKey.KeyShadow & 0x03FFF) == ((ADMIN_MANAGER_PASS_MARK << (8+1)) & 0x03FFF) )
    {
      if ( mKey.RealData == ADMIN_CODE )
      {
        sound_effect(SUCCESS_TONE,1);
        return KEY_MODE_PASSWORD_OK_ADMIN;
      }
    }
  }
  else if ( mKey.KeyCnt == KEY_COUNT_MAX ) //PASSWORD
  {
    HalfHex2Dec( mKey.RealData, &tTemp16bit );
        
    if ( (mKey.KeyShadow & 0x003FF) == ((ADMIN_MANAGER_PASS_MARK << (4+1)) & 0x003FF) )
    {
      if ( tTemp16bit == gLobbyHandle.ManagerPassword )
      {
        sound_effect(SUCCESS_TONE,1);
        return KEY_MODE_PASSWORD_OK_MANAGER;
      }
    }

    if ( mKey.HooNumber == NULL )
    {
      if( gLobbyHandle.Password == tTemp16bit )
      {
        MsgDoorOpenfunc();
        mKey.Mode = KEY_MODE_NORMAL;
        mKey.KeyShadow = NULL;
        CancelTimeOut( mKeyTimeOut );
        KeyDataClear();
        return false;
      }
    }
    else if ( mKey.HooNumber == 9999 )
    {
      if ( gLobbyHandle.ManagerPassword == tTemp16bit )
      {
        MsgDoorOpenfunc();
        mKey.Mode = KEY_MODE_NORMAL;
        mKey.KeyShadow = NULL;
        CancelTimeOut( mKeyTimeOut );
        KeyDataClear();
        return false;        
      }
    }
    else
    {
      if( PasswordHomeCompare( (U8)(mKey.HooNumber >> 8), (U8)(mKey.HooNumber & 0xFF), (U8*)&tTemp16bit ) )
      {
        MsgDoorOpenfunc();
        mKey.Mode = KEY_MODE_NORMAL;
        mKey.KeyShadow = NULL;
        CancelTimeOut( mKeyTimeOut );
        KeyDataClear();
        return false;
      }
    }
  }
  else if (!mKey.KeyCnt)    ///VER 1.6
  {
    if ( (mKey.KeyShadow & 0x001F) == (ADMIN_MANAGER_PASS_MARK & 0x001F) )   ///Ver 1.6    
    {
      return KEY_MODE_PASSWORD_JOIN_MANAGER;
    }
  }
 
  sound_effect(ERROR_TONE, 1);
  SettingValueHexFnd_Number(); 
  return false;



}


/// @brief Star Ű�� �������� ��忡 ���� ���� �ϰ� �ϴ� �Լ�
/// @param void
/// @return void
void KeySharpFunction( void )
{
  U16 tHalfHex;
  
  SetupTimeOut( mKeyTimeOut, KEY_TIMEOUT_CNT );

  switch ( mKey.Mode )
  {
    case KEY_MODE_NORMAL:
      if ( mKey.KeyCnt != NULL ) // Ű�Է��� �����ÿ� ����
      {
          mKey.Mode = KEY_MODE_NORMAL;
          mKey.KeyShadow = NULL;
          CancelTimeOut( mKeyTimeOut );
          sound_effect(ERROR_TONE,1);
          KeyDataClear();
      }
      else // ��� ��ȣ �Է�
      {   ///Ver 1.6
        /*mKey.Mode = KEY_MODE_HOME_NUM_INPUT;    
        SettingValueHexFnd_House();
        KeyDataClear_NoDisplay();
        sound_effect(KEY_TONE,1);*/

        mKey.HooNumber = 0;      
      
        if ( mKey.KeyCnt != 0 ) {
          HalfHex2Dec(mKey.RealData, &mKey.HooNumber);
        }
        
        mKey.Mode = KEY_MODE_HOME_PASS_INPUT;
        SettingValueHexFnd_Password();
        KeyDataClear_NoDisplay();
        sound_effect(KEY_TONE,1);
      }
      break;
    case KEY_MODE_HOME_NUM_INPUT:
      mKey.HooNumber = 0;
      
      if ( mKey.KeyCnt != 0 ) {
        HalfHex2Dec(mKey.RealData, &mKey.HooNumber);
      }
      
      mKey.Mode = KEY_MODE_HOME_PASS_INPUT;
      SettingValueHexFnd_Password();
      KeyDataClear_NoDisplay();
      sound_effect(KEY_TONE,1);
      break;
    case KEY_MODE_HOME_PASS_INPUT:
      switch( KeyCompareFunction() )
      {
        case false:
          mKey.Mode = KEY_MODE_NORMAL;
          mKey.KeyShadow = NULL;
          KeyDataClear();
          KeyDataClear_NoDisplay();
          return;
        case KEY_MODE_PASSWORD_OK_MANAGER:
          KeyDataClear_NoDisplay();
          SettingValueHexFnd_CODE();
          mKey.Mode = KEY_MODE_MENU_INPUT;
          KeyDataClear_NoDisplay();
          break;
        case KEY_MODE_PASSWORD_OK_ADMIN:
          Dec2HalfHex( gLobbyHandle.ManagerPassword, &tHalfHex );
          SettingValueHexFnd_Page1(tHalfHex);
          KeyLockOn();          
          mKey.Mode = KEY_MODE_NORMAL;
          mKey.KeyShadow = NULL;
          KeyDataClear_NoDisplay();
          break;
        case KEY_MODE_PASSWORD_JOIN_MANAGER:             ///Ver 1.6
          sound_effect(KEY_TONE,1);
          break;
        default:
          KeyDataClear_NoDisplay();
          break;
      }
      break;
    case KEY_MODE_MENU_INPUT:
      HalfHex2Dec( mKey.RealData, (U16*)&mKey.SettingSwitch );
      
      switch(mKey.SettingSwitch)
      {
        case ADMIN_DONG_NUMBER:
        case ADMIN_LOBBY_ID:
        case ADMIN_DOOR_TIME:
        case ADMIN_SOUND_MUTE:
        case ADMIN_RFCARD_MASTER:
        case ADMIN_RFCARD_ALL_DEL:
        case ADMIN_PROTOCOL:
        case ADMIN_GUARD_ID:
        case ADMIN_MAX_HOO:
        case ADMIN_FW_VERSION_CHECK:
        case ADMIN_HOME_PASSWORD_ALL_DEL:
          sound_effect(KEY_TONE,1);
          mKey.Mode = KEY_MODE_SETTING;
          SettingValueDisplay();
          KeyDataClear_NoDisplay();
          break;
        case ADMIN_HOME_PASSWORD_CHANGE:
          sound_effect(KEY_TONE,1);
          SettingValueHexFnd_House();
          mKey.Mode = KEY_MODE_CHANGE_HOME_NUM_INPUT;
          KeyDataClear_NoDisplay();
          CancelTimeOut( mKeyTimeOut );
          break;
        case ADMIN_MANAGER_PASSWORD_CHANGE:
        case ADMIN_COMMON_PASSWORD_CHANGE:
          sound_effect(KEY_TONE,1);
          mKey.Mode = KEY_MODE_PASSWORD_CHANGE;
          SettingValueDisplay();
          KeyDataClear_NoDisplay();
          break;          
        default:
          sound_effect(ERROR_TONE,1);
          mKey.Mode = KEY_MODE_NORMAL;
          mKey.KeyShadow = NULL;
          KeyDataClear();
          break;
      }
      break;
    case KEY_MODE_CHANGE_HOME_NUM_INPUT:
      HalfHex2Dec( mKey.RealData, &tHalfHex );
      if( tHalfHex >= 101 && tHalfHex <= 9999 )
      {
        mKey.HooNumber = tHalfHex;
        mKey.Mode = KEY_MODE_PASSWORD_CHANGE;
        KeyDataClear_NoDisplay();
        sound_effect(KEY_TONE,1);
        SettingValueDisplay();
        CancelTimeOut( mKeyTimeOut );
      }
/*      else if ( tHalfHex == 0 )
      {
        mKey.HooNumber = 0;
        mKey.Mode = KEY_MODE_PASSWORD_CHANGE;
        KeyDataClear_NoDisplay();
        sound_effect(KEY_TONE,1);
        SettingValueDisplay();
        CancelTimeOut( mKeyTimeOut );
      }*/
      else
      {
        mKey.Mode = KEY_MODE_NORMAL;
        mKey.KeyShadow = NULL;
        CancelTimeOut( mKeyTimeOut );
        sound_effect(ERROR_TONE,1);
        KeyDataClear();
      }
      break;
    case KEY_MODE_SETTING:
    case KEY_MODE_PASSWORD_CHANGE:
      mKey.Mode = KEY_MODE_NORMAL;
      mKey.KeyShadow = NULL;
      //sound_effect(KEY_TONE,1);
      switch (  SettingValueSave())
      {
        case false:
          sound_effect(ERROR_TONE,1);
          KeyDataClear();
          break;
        case true:
          KeyDataClear();
          break;
        case (true + 1):
          KeyDataClear_NoDisplay();
          break;
        default:
          break;
      }
      gsTimerCnt = 0; 
      break;      
    default:
      break;
  }
  
}


/// @brief Star ��忡 ���� Ű����(����) ���� ������ ���� �ϴ� �Լ�
/// @param tKeyMap Key Data
/// @return void
void KeyNumFunction(U8 tKeyMap)
{
  sound_effect(KEY_TONE,1);

  mKey.KeyCnt++;

  switch ( mKey.Mode )
  {
    case KEY_MODE_NORMAL:
//  case KEY_MODE_DONG_CALL:
    case KEY_MODE_HOME_NUM_INPUT:  
    case KEY_MODE_CHANGE_HOME_NUM_INPUT:

      if ( mKey.KeyCnt > KEY_COUNT_MAX ) return;
      SettingValueHexFnd_Number();
      mKey.RealData <<= 4;
      mKey.RealData |= (U8)(tKeyMap & 0x0f);

      mKey.DisplayData <<= 4;
      mKey.DisplayData |= (tKeyMap & 0x0f);
      if ( mKey.DisplayData == 0 ) mKey.KeyCnt--;
      SettingValueHexFnd_Page1_NULL( mKey.DisplayData );
      break;
    case KEY_MODE_HOME_PASS_INPUT:
      //if ( mKey.KeyCnt > KEY_COUNT_MAX ) return;
      mKey.RealData <<= 4;
      mKey.RealData |= (U8)(tKeyMap & 0x0f);
      if( mKey.KeyCnt == 1 ) SettingValueHexFnd_BAR_Shift( true );      
      else SettingValueHexFnd_BAR_Shift( false );
      break;      
    case KEY_MODE_MENU_INPUT:
    case KEY_MODE_SETTING:
      if ( mKey.KeyCnt > KEY_COUNT_MAX ) return;
      mKey.RealData <<= 4;
      mKey.RealData |= (U8)(tKeyMap & 0x0f);
      mKey.DisplayData <<= 4;
      mKey.DisplayData |= (tKeyMap & 0x0f);
      SettingValueHexFnd_Page1_NULL( mKey.DisplayData );
      break;
    case KEY_MODE_PASSWORD_CHANGE:
      if ( mKey.KeyCnt > KEY_COUNT_MAX ) return;
      mKey.RealData <<= 4;
      mKey.RealData |= (U8)(tKeyMap & 0x0f);
      mKey.DisplayData <<= 4;
      mKey.DisplayData |= (tKeyMap & 0x0f);
      SettingValueHexFnd_Page1_NULL_Cnt(mKey.DisplayData, mKey.KeyCnt);
      break;      
    default:
      break;
  }

}


/// @brief Call ��ư �� �������� ��ȭ ���� �޽��� �����ϴ� �Լ�
/// @param tKeyData �Էµ� ȣ ��ȣ
/// @return void
void KeyMainFunctionMasterCallHouse( U32 tKeyData )
{
  U8 WidthHouse,HeightHouse;
  U16 tDongNoHex;
  U8 HouseID;
  typeMsgBox tMsgBuffer;

  Dec2HalfHex( gLobbyHandle.DongNumber, &tDongNoHex );
  
  tMsgBuffer.DstID = MSG_ID_ThreadAnalysisProtocol;
  tMsgBuffer.SrcID = MSG_ID_KeyMainFunction;
  tMsgBuffer.Cmd   = MSG_CMD_RECEIVE_PROTOCOL;
  tMsgBuffer.Buf.Byte[0] = gLobbyHandle.LobbyID;
  tMsgBuffer.Buf.Byte[1] = LR_CALL_ON;
  tMsgBuffer.Buf.Byte[2] = 0;
  tMsgBuffer.Buf.Byte[3] = tDongNoHex >> 8;;
  tMsgBuffer.Buf.Byte[4] = tDongNoHex & 0x00ff;;
  tMsgBuffer.Buf.Byte[5] = (U8)((tKeyData>>8) & 0xFF);
  tMsgBuffer.Buf.Byte[6] = (U8)(tKeyData & 0xFF);
  tMsgBuffer.Buf.Byte[8] = RECEIVE_LINE_KEY;
   

  bSendMessageBox( &tMsgBuffer );


}


/// @brief ��� ��ư �� �������� ��ȭ ���� �޽��� �����ϴ� �Լ�
/// @param void
/// @return void
void KeyMainFunctionMasterCallGuard( void )
{
  typeMsgBox tMsgBuffer;
  
  U16 tDongNoHex;
  
  Dec2HalfHex( gLobbyHandle.DongNumber, &tDongNoHex );

  tMsgBuffer.DstID = MSG_ID_ThreadAnalysisProtocol;
  tMsgBuffer.SrcID = MSG_ID_KeyMainFunction;
  tMsgBuffer.Cmd   = MSG_CMD_RECEIVE_PROTOCOL;
  tMsgBuffer.Buf.Byte[0] = gLobbyHandle.LobbyID;
  tMsgBuffer.Buf.Byte[1] = LG_CALL_ON;
  tMsgBuffer.Buf.Byte[2] = 0;
  tMsgBuffer.Buf.Byte[3] = tDongNoHex >> 8;;
  tMsgBuffer.Buf.Byte[4] = tDongNoHex & 0x00ff;;

  if (gLobbyHandle.Protocol == PSS_KD_PROTOCOL )
  {
    tMsgBuffer.Buf.Byte[5] = 0;
    tMsgBuffer.Buf.Byte[6] = ( MASTER_LOBBY_ID & 0x3F );//(gLobbyHandle.LobbyID& 0x3F);
  }
  else if (gLobbyHandle.Protocol == PSS_ANAM_PROTOCOL )
  {
    tMsgBuffer.Buf.Byte[5] = 0;
    tMsgBuffer.Buf.Byte[6] = (gLobbyHandle.LobbyID& 0x3F);
  }
  else
  {
    tMsgBuffer.Buf.Byte[5] = (gLobbyHandle.LobbyID& 0x3F);
    tMsgBuffer.Buf.Byte[6] = 0;
  }
  
  tMsgBuffer.Buf.Byte[8] = RECEIVE_LINE_KEY;

  bSendMessageBox( &tMsgBuffer );

}


/// @brief ���(*) ��ư �� �������� ��ȭ ���� �޽��� �����ϴ� �Լ�
/// @param void
/// @return void
void KeyMainFunctionMasterOnHook( void )
{
  typeMsgBox tMsgBuffer;

  tMsgBuffer.DstID = MSG_ID_ThreadAnalysisProtocol;
  tMsgBuffer.SrcID = MSG_ID_KeyMainFunction;
  tMsgBuffer.Cmd  = MSG_CMD_RECEIVE_PROTOCOL;
  tMsgBuffer.Buf.Byte[0] = 0;
  tMsgBuffer.Buf.Byte[1] = ___ON_HOOK;
  tMsgBuffer.Buf.Byte[2] = 0;
  tMsgBuffer.Buf.Byte[3] = gLobbyHandle.LobbyID;
  tMsgBuffer.Buf.Byte[8] = RECEIVE_LINE_KEY;
  bSendMessageBox( &tMsgBuffer );

}


/// @brief ���(*) ��ư�� ������ �ٸ� Ű���� �Էµ��� �ʵ��� ��
/// @param void
/// @return void
void KeyLockOn(void)
{
  mKey.Lock = true;
}


/// @brief ��� ��ư �ٽ� ��
/// @param void
/// @return void
void KeyLockOff(void)
{
  mKey.Lock = false;
  SettingValueHexFnd_Number();
}


/// @brief Ű �Է¿� ���� ������ �ְ��ϴ� Key Main �Լ�
/// @param tKeyMap �Էµ� Key
/// @return void
void KeyMainFunction( U8 tKeyMap )
{

  if ( mKey.Lock ){
    if ( tKeyMap != KEY_STAR ) return;
  }

  mKey.KeyShadow <<= 1;
  
  switch ( tKeyMap )
  {
    case KEY_SHARP: 
      KeySharpFunction();
      break;
    case KEY_STAR:// ��� Ű
      if ( mKey.Mode == KEY_MODE_NORMAL ) mKey.KeyShadow |= 1;
      else mKey.KeyShadow = 0;    
      KeyMainFunctionMasterOnHook();
      mKey.Mode = KEY_MODE_NORMAL;
      CancelTimeOut( mKeyTimeOut );
      KeyDataClear();
      KeyLockOff();
      sound_effect(KEY_TONE, 1);
      break;
    case KEY_GUARD:
      if ( mKey.RealData != NULL ){
        KeyDataClear();
        sound_effect(ERROR_TONE, 1);
        CancelTimeOut( mKeyTimeOut );
        break;
      }
      KeyDataClear_NoDisplay();
      KeyMainFunctionMasterCallGuard();
      KeyLockOn();
      SettingValueHexFnd_CALL();
      CancelTimeOut( mKeyTimeOut );
      sound_effect(KEY_TONE, 1);
      break;
    case KEY_CALL:
      if (mKey.Mode == KEY_MODE_NORMAL)
      {
        if( mKey.RealData >= 0x101 )
        {
          KeyMainFunctionMasterCallHouse( (U16)mKey.RealData );
          SettingValueHexFnd_Page1_NULL((U16)mKey.RealData);
          KeyLockOn();
          KeyDataClear_NoDisplay();
          CancelTimeOut( mKeyTimeOut );
          break;
        }
      }
      mKey.Mode = KEY_MODE_NORMAL;
      KeyDataClear();
      sound_effect(ERROR_TONE, 1);
      CancelTimeOut( mKeyTimeOut );
      break;
    default:
      KeyNumFunction(tKeyMap);
      SetupTimeOut( mKeyTimeOut, KEY_TIMEOUT_CNT );
      break;    
  }
}





