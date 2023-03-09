
/// @file   func_factory.c
/// @date   2012/02/14
/// @author jinhwa Lee
/// @brief  공장 TEST 모드 파일


#include "main.h"


extern U16 gFwVersion;


U8 mFactoryMode = true;
U8 mFactoryModeTimeOutCnt = 110; // 500ms
U8 mStartFactoryMode = false;
U8 mModeCnt;
U8 mKeyFactoryMode;
U8 mFndTestMode = false;


U8 gbKeyMap_Factory[16] = {12,13,      // 경비 / 호출
                           1,2,3,      //  1 / 2 / 3
                           4,5,6,      //  4 / 5 / 6
                           7,8,9,      //  7 / 8 / 9
                           11,0,10};   //  * / 0 / #

U16 bpreKeyVal_Factory = 0;
U8  bReleaseCnt_Factory[SCAN_LINE_X*SCAN_LINE_Y] = {0,};

HANDLE mHandleTimeOutFactoryMode;

/// @brief 공장 모드 초기화
/// @param void
/// @return void
void InitalizeFactoryMode(void)
{

  mHandleTimeOutFactoryMode = CreateTimeOut();
  SetupTimeOut(mHandleTimeOutFactoryMode, TIMEOUT_FACTORY);
  
  mKeyFactoryMode = KEY_GUARD;
  mModeCnt = 1;
  SettingValueHexFnd_Page0_Version(gFwVersion);
  sound_effect( KEY_TONE, 3 );
  
  InitializeUart0_19200();
  InitializeUart1_3906();
  InitializeUart2_3906();

  gLobbyHandle.Protocol = 2;

}


/// @brief 공장 모드 Key Scan
/// @param void
/// @return void
void KeyScan_Factory(void)
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
        if ( bpreKeyVal_Factory & tKeyVal)
        {
          if(++bReleaseCnt_Factory[(i*SCAN_LINE_X+j)] == KEY_RELEASE)
          {
            bReleaseCnt_Factory[(i*SCAN_LINE_X+j)] = 0;
            ExeSwRoutine_Factory((U8)(i*SCAN_LINE_X+j), STATUS_ON);
          }
        } 
        else
        {
          tKeyVal = (i * SCAN_LINE_X) + j;
          tKeyVal = 0x01 << tKeyVal;
          bpreKeyVal_Factory |= tKeyVal;
        }
      }
      else
      {

        tKeyVal = (i * SCAN_LINE_X) + j;
        tKeyVal = ~(0x01 << tKeyVal);
        bpreKeyVal_Factory &= tKeyVal;

        
        bReleaseCnt_Factory[(i*SCAN_LINE_X+j)] = 0;
        ExeSwRoutine_Factory((U8)(i*SCAN_LINE_X+j), STATUS_OFF);
      }
    }
  }
  __wdt_reset();
}


/// @brief 공장 모드 Key Scan 실행 함수
/// @param swdata 키 데이타
/// @param status 키 입력 상태
/// @return void
void ExeSwRoutine_Factory(U8 swdata, U8 status)
{
  static U16 keyReleaseTimeCnt_Factory[SCAN_LINE_X*SCAN_LINE_Y] = {0,};
  static U8 keyPushedFlag_Factory[SCAN_LINE_X*SCAN_LINE_Y] = {0,};
  static U8 tEepromResetFlg = false;
  U16 i;
  
  if(status == STATUS_OFF)
  {
    keyReleaseTimeCnt_Factory[swdata] = 0;
    keyPushedFlag_Factory[swdata] = 0;
  }
  else if(status == STATUS_ON)
  {

    if(keyPushedFlag_Factory[swdata] == 1)
    {
      //SettingValueHexFnd(swdata | 0x1000);
      keyReleaseTimeCnt_Factory[swdata]++;
      if ( mStartFactoryMode == false )
      {
        if( keyReleaseTimeCnt_Factory[swdata] > (100/THREAD_KEY_SCAN_TIME) )
        {
          if ( gbKeyMap_Factory[swdata] == KEY_SHARP)
          {
            if ( mFactoryModeTimeOutCnt != 0 )
            {
              //mFactoryModeTimeOutCnt = 0;
              mStartFactoryMode = true;
              mFndTestMode = false;
              InitalizeFactoryMode();
            }
          }
          if ( gbKeyMap_Factory[swdata] == KEY_CALL)
          {
            if ( mFactoryModeTimeOutCnt != 0 )
            {
              //mFactoryModeTimeOutCnt = 0;
              mStartFactoryMode = true;
              mFndTestMode = true;
              InitalizeFactoryMode();
            }
          }
        }
      }
      else
      {
        if( keyReleaseTimeCnt_Factory[swdata] > (800/THREAD_KEY_SCAN_TIME) )
        {
          if ( tEepromResetFlg == false )
          {
            if ( gbKeyMap_Factory[swdata] == 0 )
            {
              sound_effect(KEY_TONE, 1);
              for ( i = EEPROM_START_ADDR ; i < 50  ; i++ )
              {
                EEPROM_WriteByte( i, 0xFF );
              }
             
              InitializeFactoryEEPROM();
              sound_effect(SUCCESS_TONE, 1);
              delay_ms(500);
              tEepromResetFlg = true;
              while(1){};
            }
          }
        }
      
      }
    }
    else
    {
      if ( mFndTestMode == false ) KeyFunction_factory(gbKeyMap_Factory[swdata]);
      else KeyFunction_Fnd_factory(gbKeyMap_Factory[swdata]);
      keyPushedFlag_Factory[swdata] = 1;
      keyReleaseTimeCnt_Factory[swdata] = 0;
    }
  }
}


/// @brief Key입력에 따른 실행 함수
/// @param tKeyMap 입력된 키 값
/// @return void
void KeyFunction_Fnd_factory( U8 tKeyMap )
{
  U8 tTemp;
  
  if( mStartFactoryMode == false ) return;
  
  SetupTimeOut(mHandleTimeOutFactoryMode, TIMEOUT_FACTORY);

  switch ( mKeyFactoryMode )
  {
    case KEY_GUARD:                       // EEPROM TEST
      if( tKeyMap == mKeyFactoryMode )
      {
         sound_effect(KEY_TONE,1);
         SettingValueHexFnd_Fnd_Factory( mModeCnt++);
         mKeyFactoryMode = KEY_CALL;
      }
      break;
    case KEY_CALL:                     // Door Open
      if( tKeyMap == mKeyFactoryMode )
      {
        sound_effect(KEY_TONE,1);
        SettingValueHexFnd_Fnd_Factory( mModeCnt++);
        mKeyFactoryMode = 1;
      }  
      break;
    case 1:
      if( tKeyMap == mKeyFactoryMode )
      {
        sound_effect(KEY_TONE,1);
        SettingValueHexFnd_Fnd_Factory( mModeCnt++);
        mKeyFactoryMode = 2;
      }    
      break;
    case 2:
      if( tKeyMap == mKeyFactoryMode )
      {
        sound_effect(KEY_TONE,1);
        SettingValueHexFnd_Fnd_Factory( mModeCnt++);
        mKeyFactoryMode = 3;
      }     
      break;
    case 3:
      if( tKeyMap == mKeyFactoryMode )
      {
        sound_effect(KEY_TONE,1);
        SettingValueHexFnd_Fnd_Factory( mModeCnt++);
        mKeyFactoryMode = 4;
      }     
      break;
    case 4:
      if( tKeyMap == mKeyFactoryMode )
      {
        sound_effect(KEY_TONE,1);
        SettingValueHexFnd_Fnd_Factory( mModeCnt++);
        mKeyFactoryMode = 5;
      }     
      break;
    case 5:
      if( tKeyMap == mKeyFactoryMode )
      {
        sound_effect(KEY_TONE,1);
        SettingValueHexFnd_Fnd_Factory( mModeCnt++);
        mKeyFactoryMode = 6;
      }     
      break;
    case 6:
      if( tKeyMap == mKeyFactoryMode )
      {
        sound_effect(KEY_TONE,1);
        SettingValueHexFnd_Fnd_Factory( mModeCnt++);
        mKeyFactoryMode = 7;
      } 
      break;
    case 7:
      if( tKeyMap == mKeyFactoryMode )
      {
        sound_effect(KEY_TONE,1);
        SettingValueHexFnd_Fnd_Factory( mModeCnt++);
        mKeyFactoryMode = 8;
      }     
      break;
    case 8:
      if( tKeyMap == mKeyFactoryMode )
      {
        sound_effect(KEY_TONE,1);
        SettingValueHexFnd_Fnd_Factory( mModeCnt++);
        mKeyFactoryMode = 9;
      }     
      break;
    case 9:
      if( tKeyMap == mKeyFactoryMode )
      {
        sound_effect(KEY_TONE,1);
        SettingValueHexFnd_Fnd_Factory( mModeCnt++);
        mKeyFactoryMode = KEY_STAR;
      }  
      break;
    case KEY_STAR:
      if( tKeyMap == mKeyFactoryMode )
      {
        sound_effect(KEY_TONE,1);
        SettingValueHexFnd_Fnd_Factory( mModeCnt++);
        mKeyFactoryMode = 0;
      }  

      break;
    case 0:
      if( tKeyMap == mKeyFactoryMode )
      {
        sound_effect(KEY_TONE,1);
        SettingValueHexFnd_Fnd_Factory( mModeCnt++);
        mKeyFactoryMode = KEY_SHARP;
      }  

      break;
    case KEY_SHARP:
      if( tKeyMap == mKeyFactoryMode )
      {
        sound_effect(KEY_TONE,1);
        SettingValueHexFnd_Fnd_Factory( mModeCnt++);
        mKeyFactoryMode = 0xFF;
      }  
      break;
    default:
      break;    
  }
}



/// @brief Key입력에 따른 실행 함수
/// @param tKeyMap 입력된 키 값
/// @return void
void KeyFunction_factory( U8 tKeyMap )
{
  U8 tTemp,i;
  U8 tAddr,tFactoryReset;
  
  if( mStartFactoryMode == false ) return;
  
  SetupTimeOut(mHandleTimeOutFactoryMode, TIMEOUT_FACTORY);

  switch ( mKeyFactoryMode )
  {
    case KEY_GUARD:                       // EEPROM TEST
      if( tKeyMap == mKeyFactoryMode )
      {
         sound_effect(KEY_TONE,1);
         
         // Start Factory Reset  //
         tFactoryReset = 0;
         for ( i = 0; i < 5; i++)
         {
           EEPROM_ReadBlock( LOBBY_HANDLE_ADDR + i, &tTemp, 1 );
           if ( tTemp == 0xFF ) tFactoryReset++;
         }     
         if ( tFactoryReset == 5 ) InitializeFactoryEEPROM();
         
         gLobbyHandle.Protocol = 2;
         // End Factory Reset  //

         EEPROM_WriteByte(FACTORY_TEST_ADDR, 0xAA);
         EEPROM_WriteByte(FACTORY_TEST_ADDR, 0xBB);
         EEPROM_ReadBlock( FACTORY_TEST_ADDR, &tTemp, 1 );

         if ( tTemp == 0xBB )
         {
           sound_effect(SUCCESS_TONE, 1);
           SettingValueHexFnd_Factory(true, mModeCnt++);
           mKeyFactoryMode = KEY_CALL;
         }
         else
         {
           SettingValueHexFnd_Factory(false, mModeCnt);
           sound_effect(ERROR_TONE, 1);
         }

         
      }

      break;
    case KEY_CALL:                     // Door Open
      if( tKeyMap == mKeyFactoryMode )
      {
         DR_M_OPEN = SET;              
         sound_effect(KEY_TONE,1);
         SettingValueHexFnd_Factory(true, mModeCnt++);
         mKeyFactoryMode = 1;
      }      
      break;
    case 1:
      if( tKeyMap == mKeyFactoryMode )
      {

        EN_MC = false;
        nMUTE_SPK = true;
        SpeakerOnOff( SPK_ONOFF_ON, SPK_ONOFF_LOBBY);
        DR_M_OPEN = CLEAR;
        
        MUX_CH1A_SEL1 = 0;  // ML
        MUX_CH1A_SEL2 = 1;
        MUX_CH1A_SEL3 = 1;
        
        MUX_CH1B_SEL1 = 0;  // H1
        MUX_CH1B_SEL2 = 0;
        MUX_CH1B_SEL3 = 0;
        
        MUX_CH2A_SEL1 = 1;
        MUX_CH2A_SEL2 = 1;
        MUX_CH2A_SEL3 = 1;
        
        MUX_CH2B_SEL1 = 1;
        MUX_CH2B_SEL2 = 1;
        MUX_CH2B_SEL3 = 1;

        HOUSE_CALL1   = true;
        HOUSE_CALL2   = false;
        SUB_CALL1     = false;
        SUB_CALL2     = false;
        GUARD_CALL  = false;

        TransmitProtocolHouse( 0x01, 0x00, TEST_HOME1_TALK, 0x00);

        VIDEO_SEL1 = 0;
        VIDEO_SEL2 = 0;
        VIDEO_SEL3 = 0;

        CAM_PWR_ON = true;            // Camera On
        sound_effect(KEY_TONE,1);
        SettingValueHexFnd_Factory(true, mModeCnt++);
        mKeyFactoryMode = 2;
      }
      break;    
    case 2:
      if( tKeyMap == mKeyFactoryMode )
      {

        EN_MC = false;
        nMUTE_SPK = true;
        SpeakerOnOff( SPK_ONOFF_ON, SPK_ONOFF_LOBBY);
        
        MUX_CH1A_SEL1 = 1;  // H2
        MUX_CH1A_SEL2 = 0;
        MUX_CH1A_SEL3 = 0;

        MUX_CH1B_SEL1 = 0;  // ML
        MUX_CH1B_SEL2 = 1;
        MUX_CH1B_SEL3 = 1;

        MUX_CH2A_SEL1 = 1;
        MUX_CH2A_SEL2 = 1;
        MUX_CH2A_SEL3 = 1;

        MUX_CH2B_SEL1 = 1;
        MUX_CH2B_SEL2 = 1;
        MUX_CH2B_SEL3 = 1;
        
        HOUSE_CALL1   = false;
        HOUSE_CALL2   = true;
        SUB_CALL1     = false;
        SUB_CALL2     = false;
        GUARD_CALL  = false;

        TransmitProtocolHouse( 0x01, 0x00, TEST_HOME2_TALK, 0x00);
        TransmitProtocolLobby( 0x01, 0x00, TEST_HOME2_TALK, 0x00, 0x00, 0x00, 0x00);
        
        CAM_PWR_ON = false; 
        
        VIDEO_SEL1 = 1;
        VIDEO_SEL2 = 0;
        VIDEO_SEL3 = 0;
        sound_effect(KEY_TONE,1);
        SettingValueHexFnd_Factory(true, mModeCnt++);
        mKeyFactoryMode = 3;
      }
      break;      
    case 3:
      if( tKeyMap == mKeyFactoryMode )
      {

        EN_MC = false;
        nMUTE_SPK = true;
        SpeakerOnOff( SPK_ONOFF_ON, SPK_ONOFF_LOBBY);
        
        MUX_CH1A_SEL1 = 1;
        MUX_CH1A_SEL2 = 1;
        MUX_CH1A_SEL3 = 1;
        
        MUX_CH1B_SEL1 = 1;
        MUX_CH1B_SEL2 = 1;
        MUX_CH1B_SEL3 = 1;
        
        MUX_CH2A_SEL1 = 0;  //ML
        MUX_CH2A_SEL2 = 1;
        MUX_CH2A_SEL3 = 1;
        
        MUX_CH2B_SEL1 = 0;  //SL1
        MUX_CH2B_SEL2 = 1;
        MUX_CH2B_SEL3 = 0;

        HOUSE_CALL1   = false;
        HOUSE_CALL2   = false;
        SUB_CALL1     = true;
        SUB_CALL2     = false;
        GUARD_CALL  = false;
        
        TransmitProtocolLobby( 0x01, 0x00, TEST_LOBBY1_TALK, 0x00, 0x00, 0x00, 0x00);

        VIDEO_SEL1 = 0;
        VIDEO_SEL2 = 1;
        VIDEO_SEL3 = 0;
        sound_effect(KEY_TONE,1);
        SettingValueHexFnd_Factory(true, mModeCnt++);
        mKeyFactoryMode = 4;
      }
      break;      
    case 4:
      if( tKeyMap == mKeyFactoryMode )
      {

        EN_MC = false;
        nMUTE_SPK = true;
        SpeakerOnOff( SPK_ONOFF_ON, SPK_ONOFF_LOBBY);
        
        MUX_CH1A_SEL1 = 1;
        MUX_CH1A_SEL2 = 1;
        MUX_CH1A_SEL3 = 1;
        
        MUX_CH1B_SEL1 = 1;
        MUX_CH1B_SEL2 = 1;
        MUX_CH1B_SEL3 = 1;
        
        MUX_CH2A_SEL1 = 1;  //SL2
        MUX_CH2A_SEL2 = 1;
        MUX_CH2A_SEL3 = 0;
        
        MUX_CH2B_SEL1 = 0;  //ML
        MUX_CH2B_SEL2 = 1;
        MUX_CH2B_SEL3 = 1;

        HOUSE_CALL1   = false;
        HOUSE_CALL2   = false;
        SUB_CALL1     = false;
        SUB_CALL2     = true;
        GUARD_CALL  = false;
        
        TransmitProtocolLobby( 0x01, 0x00, TEST_LOBBY2_TALK, 0x00, 0x00, 0x00, 0x00);

        VIDEO_SEL1 = 0;
        VIDEO_SEL2 = 0;
        VIDEO_SEL3 = 1;
        sound_effect(KEY_TONE,1);
        SettingValueHexFnd_Factory(true, mModeCnt++);
        mKeyFactoryMode = 5;
      }
      break;      
    case 5:
      if( tKeyMap == mKeyFactoryMode )
      {

        EN_MC = false;
        nMUTE_SPK = true;
        SpeakerOnOff( SPK_ONOFF_ON, SPK_ONOFF_LOBBY);

        
        MUX_CH1A_SEL1 = 0;  //ML
        MUX_CH1A_SEL2 = 1;
        MUX_CH1A_SEL3 = 1;
        
        MUX_CH1B_SEL1 = 0;  //GR
        MUX_CH1B_SEL2 = 0;
        MUX_CH1B_SEL3 = 1;
        
        MUX_CH2A_SEL1 = 1;
        MUX_CH2A_SEL2 = 1;
        MUX_CH2A_SEL3 = 1;
        
        MUX_CH2B_SEL1 = 1;
        MUX_CH2B_SEL2 = 1;
        MUX_CH2B_SEL3 = 1;

        HOUSE_CALL1   = false;
        HOUSE_CALL2   = false;
        SUB_CALL1     = false;
        SUB_CALL2     = false;
        GUARD_CALL  = true;
        
        TransmitProtocolHouse( 0x01, 0x00, TEST_GUARD_TALK, 0x00);
        TransmitProtocolLobby( 0x01, 0x00, TEST_GUARD_TALK, 0x00, 0x00, 0x00, 0x00);
        
        sound_effect(KEY_TONE,1);
        SettingValueHexFnd_Factory(true, mModeCnt++);
        mKeyFactoryMode = 6;
      }
      break;      
    case 6:
      if( tKeyMap == mKeyFactoryMode )
      {
        
        EN_MC = true;
        nMUTE_SPK = false;
        SpeakerOnOff( SPK_ONOFF_OFF, SPK_ONOFF_LOBBY);
        
        HOUSE_CALL1   = false;
        HOUSE_CALL2   = false;
        SUB_CALL1     = false;
        SUB_CALL2     = false;
        GUARD_CALL    = false;
        
        sound_effect(KEY_TONE,1);
        TransmitProtocolHouse( 0x01, 0x00, TEST_HOME1_UART, 0x00);
      }
      break;      
    case 7:
      if( tKeyMap == mKeyFactoryMode )
      {
        sound_effect(KEY_TONE,1);
        TransmitProtocolHouse( 0x01, 0x00, TEST_HOME2_UART, 0x00);
      }
      break;      
    case 8:
      if( tKeyMap == mKeyFactoryMode )
      {        
        sound_effect(KEY_TONE,1);
        TransmitProtocolHouse( 0x01, 0x00, TEST_HOME3_UART, 0x00);
      }
      break;      
    case 9:
      if( tKeyMap == mKeyFactoryMode )
      {  
        sound_effect(KEY_TONE,1);
        TransmitProtocolHouse( 0x01, 0x00, TEST_HOME4_UART, 0x00);
      }
      break;
    case KEY_STAR:
      if( tKeyMap == mKeyFactoryMode )
      {  
        sound_effect(KEY_TONE,1);
        TransmitProtocolLobby( 0x01, 0x00, TEST_LOBBY_UART, 0x00, 0x00, 0x00, 0x00 );
      }
      break;
    case 0:
      if( tKeyMap == mKeyFactoryMode )
      {  
        sound_effect(KEY_TONE,1);
        TransmitProtocolGuardSwitch( 0x01, 0x00, TEST_GUARD_UART, 0x00, 0x00, 0x00, 0x00 );
      }
      break;
    case KEY_SHARP:
      if( tKeyMap == mKeyFactoryMode )
      {  
        sound_effect(KEY_TONE,1);
        tTemp = TEST_CONSOLE_UART;
        bPutTXConsoleData(&tTemp, 1);
        bStartTXConsol();
      }
      break;
    default:
      break;    
  }
}


/// @brief 공장 모드 Uart 수신 함수 
/// @param tRxData 입력된 값
/// @return void
void UartRx_factory( U8 tRxData )
{

  SetupTimeOut(mHandleTimeOutFactoryMode, TIMEOUT_FACTORY);
  switch ( mKeyFactoryMode )
  {
    case 6:
      if( tRxData == TEST_HOME1_UART )
      {
        sound_effect(SUCCESS_TONE, 1);
        SettingValueHexFnd_Factory(true, mModeCnt++);
        mKeyFactoryMode = 7;
      }
      else
      {
        //SettingValueHexFnd_Factory(false, mModeCnt);
        //sound_effect(ERROR_TONE, 1);
      }
      break;      
    case 7:
      if( tRxData == TEST_HOME2_UART )
      {
        sound_effect(SUCCESS_TONE, 1);
        SettingValueHexFnd_Factory(true, mModeCnt++);
        mKeyFactoryMode = 8;
      }
      else
      {
        //SettingValueHexFnd_Factory(false, mModeCnt);
        //sound_effect(ERROR_TONE, 1);
      }

      break;      
    case 8:
      if( tRxData == TEST_HOME3_UART )
      {
        sound_effect(SUCCESS_TONE, 1);
        SettingValueHexFnd_Factory(true, mModeCnt++);
        mKeyFactoryMode = 9;
      }
      else
      {
        //SettingValueHexFnd_Factory(false, mModeCnt);
        //sound_effect(ERROR_TONE, 1);
      }

      break;      
    case 9:
      if( tRxData == TEST_HOME4_UART )
      {
        sound_effect(SUCCESS_TONE, 1);
        SettingValueHexFnd_Factory(true, mModeCnt++);
        mKeyFactoryMode = KEY_STAR;
      }
      else
      {
        //SettingValueHexFnd_Factory(false, mModeCnt);
        //sound_effect(ERROR_TONE, 1);
      }

      break;
    case KEY_STAR:
      if( tRxData == TEST_LOBBY_UART )
      {
        sound_effect(SUCCESS_TONE, 1);
        SettingValueHexFnd_Factory(true, mModeCnt++);
        mKeyFactoryMode = 0;
      }
      else
      {
        //ettingValueHexFnd_Factory(false, mModeCnt);
        //sound_effect(ERROR_TONE, 1);
      }

      break;
    case 0:
      if( tRxData == TEST_GUARD_UART )
      {
        sound_effect(SUCCESS_TONE, 1);
        SettingValueHexFnd_Factory(true, mModeCnt++);
        mKeyFactoryMode = KEY_SHARP;
      }
      else
      {
        //SettingValueHexFnd_Factory(false, mModeCnt);
        //sound_effect(ERROR_TONE, 1);
      }
      break;
    case KEY_SHARP:
      if( tRxData == TEST_CONSOLE_UART )
      {
        sound_effect(SUCCESS_TONE, 1);
        //SettingValueHexFnd_Factory(true, mModeCnt++);
        SettingValueHexFnd_Password();
        mKeyFactoryMode = 0xFF;
      }
      else
      {
        //SettingValueHexFnd_Factory(false, mModeCnt);
        //sound_effect(ERROR_TONE, 1);
      }
      break;
    default:
      break;    
  }
}





/// @brief Key입력에 따른 실행 함수df
/// @param tKeyMap 입력된 키 값
/// @return void
void KeyFunction_RxUartSlaveLobby_factory( U8 tData )
{

  switch ( tData )
  {
    case TEST_HOME2_TALK:
      
      VIDEO_SEL1 = 0;
      VIDEO_SEL2 = 0;
      VIDEO_SEL3 = 0;
      
      CAM_PWR_ON = true;            // Camera On
      break;
    case TEST_LOBBY1_TALK:

        EN_MC = false;
        nMUTE_SPK = true;
        SpeakerOnOff( SPK_ONOFF_ON, SPK_ONOFF_LOBBY);
        
        DR_M_OPEN = CLEAR;
        
        MUX_CH1A_SEL1 = 0;  // ML
        MUX_CH1A_SEL2 = 1;
        MUX_CH1A_SEL3 = 1;
        
        MUX_CH1B_SEL1 = 0;  // SL1
        MUX_CH1B_SEL2 = 1;
        MUX_CH1B_SEL3 = 0;
        
        MUX_CH2A_SEL1 = 1;
        MUX_CH2A_SEL2 = 1;
        MUX_CH2A_SEL3 = 1;
        
        MUX_CH2B_SEL1 = 1;
        MUX_CH2B_SEL2 = 1;
        MUX_CH2B_SEL3 = 1;

        HOUSE_CALL1   = false;
        HOUSE_CALL2   = false;
        SUB_CALL1     = true;
        SUB_CALL2     = false;
        GUARD_CALL    = false;

        VIDEO_SEL1 = 0;
        VIDEO_SEL2 = 0;
        VIDEO_SEL3 = 0;

        CAM_PWR_ON = true;            // Camera On
        
      break;    
    case TEST_LOBBY2_TALK:

      EN_MC = false;
      nMUTE_SPK = true;
      SpeakerOnOff( SPK_ONOFF_ON, SPK_ONOFF_LOBBY);
      DR_M_OPEN = CLEAR;
      
      MUX_CH1A_SEL1 = 0;  // ML
      MUX_CH1A_SEL2 = 1;
      MUX_CH1A_SEL3 = 1;
      
      MUX_CH1B_SEL1 = 1;  // SL2
      MUX_CH1B_SEL2 = 1;
      MUX_CH1B_SEL3 = 0;
      
      MUX_CH2A_SEL1 = 1;
      MUX_CH2A_SEL2 = 1;
      MUX_CH2A_SEL3 = 1;
      
      MUX_CH2B_SEL1 = 1;
      MUX_CH2B_SEL2 = 1;
      MUX_CH2B_SEL3 = 1;
      
      HOUSE_CALL1   = false;
      HOUSE_CALL2   = false;
      SUB_CALL1     = false;
      SUB_CALL2     = true;
      GUARD_CALL    = false;
      
      VIDEO_SEL1 = 0;
      VIDEO_SEL2 = 0;
      VIDEO_SEL3 = 0;
      
      CAM_PWR_ON = true;            // Camera On
      break;     
    case TEST_GUARD_TALK:
      
      EN_MC = true;
      nMUTE_SPK = false;
      SpeakerOnOff( SPK_ONOFF_OFF, SPK_ONOFF_LOBBY);
      DR_M_OPEN = SET;
      
      MUX_CH1A_SEL1 = 1; 
      MUX_CH1A_SEL2 = 1;
      MUX_CH1A_SEL3 = 1;
      
      MUX_CH1B_SEL1 = 1;
      MUX_CH1B_SEL2 = 1;
      MUX_CH1B_SEL3 = 1;
      
      MUX_CH2A_SEL1 = 1;
      MUX_CH2A_SEL2 = 1;
      MUX_CH2A_SEL3 = 1;
      
      MUX_CH2B_SEL1 = 1;
      MUX_CH2B_SEL2 = 1;
      MUX_CH2B_SEL3 = 1;
      
      HOUSE_CALL1   = false;
      HOUSE_CALL2   = false;
      SUB_CALL1     = false;
      SUB_CALL2     = false;
      GUARD_CALL    = false;
      
      VIDEO_SEL1 = 0;
      VIDEO_SEL2 = 0;
      VIDEO_SEL3 = 0;
      
      CAM_PWR_ON = false;            // Camera On
      break;
    case TEST_LOBBY_UART:
        TransmitProtocolLobby( 0x81, 0x00, TEST_LOBBY_UART, 0x00, 0x00, 0x00, 0x00);
      break;      
    case TEST_GUARD_UART:
        TransmitProtocolLobby( 0x81, 0x00, TEST_GUARD_UART, 0x00, 0x00, 0x00, 0x00);
      break; 
    default:
      break;    
  }
}




/// @brief 공장 모드 RFID 인식
/// @param tRxData 입력된 값
/// @return void
void RFCARD_factory( U8 tRxData )
{
  SetupTimeOut(mHandleTimeOutFactoryMode, TIMEOUT_FACTORY);
  if ( (tRxData & 0xF0) == 0x10 )
  {
    SettingValueHexFnd_Number();
    sound_effect(SUCCESS_TONE, 1);
    SettingValueHexFnd_Factory( true, 0xFF );
  }
  else
  {
    SettingValueHexFnd_Number();
    sound_effect(ERROR_TONE, 1);
    SettingValueHexFnd_Factory( false, 0xFF );
  }

}



/// @brief 공장 모드 확인
/// @param void
/// @return mFacctoryMode 변수 반환
U8 IsFactoryMode(void)
{
  return mFactoryMode;
}

/// @brief 공장 모드 확인 2
/// @param void
/// @return mFacctoryMode 변수 반환
U8 IsFactoryMode2(void)
{
  return mStartFactoryMode;
}



/// @brief 공장 모드 Thread
/// @param void
/// @return void
void ThreadFactoryMode(void) // 10ms
{
  wdt_reset();
  if( mStartFactoryMode != true )
  {
    if( mFactoryModeTimeOutCnt != 0) mFactoryModeTimeOutCnt--;
    if( mFactoryModeTimeOutCnt == 0 )
    {
      mFactoryMode = false;
      return;
    }
  }
  
  if( IsTimeOut( mHandleTimeOutFactoryMode )){
    while(1);
  }
  
  KeyScan_Factory();
}



