
/// @file   main.c
/// @date   2012/02/14
/// @author jinhwa Lee
/// @brief  프로그램의 시작하는 Main 파일


/// @mainpage ULP-200 메인페이지
/// @section intro 소개
/// - 소개 : 주장치 내장형 로비폰 개발을 위한 프로그램
/// @section Program 프로그램명
/// - 프로그램명  : ULP-200
/// - 프로그램내용 : 빌라형 로비폰 (주장치 내장형 로비폰)
/// @section info 개발 목적
/// - 빌라 및 소규모 아파트에 사용할 수 있는 로비폰 개발
/// @section advenced 사용 CPU
/// - Renesas : R8C Series - R5F2136CC
/// @section CREATEINFO 작성정보
/// - 작성자      :   이진화
/// - 작성일      :   2012/02/13
///


////////////////////////////////////////////////////////////////////////////////
// FW_VERSION = 0x1001 (EX)                                                   //
// Version  :     ^^     = Version 1.0                                        //
// Revision :       ^^   = Revison 01                                         //
////////////////////////////////////////////////////////////////////////////////

#define FW_VERSION 0x1600

/// @section MODIFYINFO 수정정보
//----------------------------------------------------------------------------
/// - 120203 ; Ver1.0 Rev00 ; DPP 품평회 / 신뢰성 의뢰 버전                       
/// - 120216 ; Ver1.0 Rev01 ; DPP 품평회 / 신뢰성 의뢰 버전(TimeOut 시간 조정)    
/// - 120223 ; Ver1.0 Rev02 ; 공장 테스트 모드 추가
/// - 120223 ; Ver1.0 Rev03 ; 세대/경비실 문열림 오픈음 추가
/// - 120223 ; Ver1.0 Rev04 ; 부팅시 E-00이라고 뜨는 문제 해결
/// - 120323 ; Ver1.0 Rev05 ; 공장 모드 프로그램 추가
/// - 120323 ; Ver1.0 Rev06 ; 층 최대호수 32로 증가/ 인터폰 CBUSY 올때 부저음 출력 안되는 문제 해결
/// - 120405 ; Ver1.0 Rev07 ; 최대 호수 넘어가가는 ID 호출 막는것 / 경비 타임아웃 추가
/// - 120406 ; Ver1.0 Rev08 ; 메시지 박스 풀ㄸㅐ 메시지 박스 클리어 해줌
/// - 120408 ; Ver1.0 Rev09 ; 리셋시 깨진 데이터를 받으면 재검사를 하는 부분 추가
/// - 120409 ; Ver1.1 Rev00 ; 양산 사양통보 버전 LPP
/// - 120420 ; Ver1.2 Rev00 ; 공장 모드 타임 아웃 10 초에서 60초로 늘림(MP사양통보)
/// - 120719 ; Ver1.3 Rev00 ; 아남 프로토콜 추가 / 로비ID 0x40 부터 시작으로 변경 / 서브로비 동정보를 가지고 통신하는 기준으로 변경 / 공장 모드에서 0을 누르고 있으면 리셋 되게 변경
/// - 120727 ; ver1.3 Rev01 ; 서브 로비를 아남 프로토콜 추가 / 동일 동 마스터 로비 여러게에서 동시에 경비 호출 가능하게
/// - 121204 ; Ver1.4 Rev00 ; 메뉴 진입방법 관리 비밀번호로 변경/ 경동,현대,아남 프로토콜 추가
/// - 140811 ; Ver1.5 Rev00 ; HT 프로토콜 변경 금지

///////////////////////////////////////////////////////////////////////////////

#include "main.h"


U16 gFwVersion = FW_VERSION;    ///< 펌웨어 버전
typeLobbyHandler gLobbyHandle;  ///< 로비폰 기본 정보

extern U16 gsTimerCnt;

U16 *gTestIOReg = (U16*)0x0000A6;

/* never used
void TalkAgeingTest(void)
{
  HOUSE_CALL1 = true;
  HOUSE_CALL2 = true;
  SUB_CALL1 = true;
  SUB_CALL2 = true;
  GUARD_CALL = true;

  MUX_CH1A_SEL1 = 0;
  MUX_CH1A_SEL2 = 1;
  MUX_CH1A_SEL3 = 1;

  MUX_CH1B_SEL1 = 0;
  MUX_CH1B_SEL2 = 0;
  MUX_CH1B_SEL3 = 0;

  MUX_CH2A_SEL1 = 0;
  MUX_CH2A_SEL2 = 1;
  MUX_CH2A_SEL3 = 0;

  MUX_CH2B_SEL1 = 1;
  MUX_CH2B_SEL2 = 0;
  MUX_CH2B_SEL3 = 0;

  nMUTE_SPK = SET;
  EN_MC = false;
  CAM_PWR_ON = true;
  nMUX_EN = false;
  
  while(1)
  {
    __wdt_reset();
  }

}
*/

/// @brief  공장 TEST 모드 함수
/// @param  void
/// @return void
void FactoryTestMode(void)
{
  InitializeTimeScheduler();;
  SetupTimeScheduler( ThreadFactoryMode, THREAD_FACTORY_MODE_TIME);
  SetupTimeScheduler( ThreadTimeOut, THREAD_TIME_OUT_TIME);

  ENABLE_IRQ();
  gsTimerCnt = 0;

  while(IsFactoryMode())
  {
    wdt_reset();
    GoTimeScheduler();
  }
  DISABLE_IRQ();

}



void BootInfoOutput (  U8 tInitLobyHandle )
{
  U8 tBeepSoundMute;
  U8 tStartBeepCnt;
  U8 i;

  tBeepSoundMute = gLobbyHandle.BeepSoundMute;
  gLobbyHandle.BeepSoundMute = false;
  
  if ( tInitLobyHandle == false )
  {
    sound_effect(ERROR_TONE, 1);
    SettingValueHexFnd_Error(0);
    LobbyHandleErrorRamReset();
  }
  else
  {
    switch( gLobbyHandle.Protocol )
    {
      case 1: // 경동
        tStartBeepCnt = 3;
        break;
      case 2: // 현대
      case 3: // 현대
        tStartBeepCnt = 2;
        break;
      case 4: // 아남
        tStartBeepCnt = 4;
        break;
      default:
        tStartBeepCnt = 1;
        break;
    }
    for ( i = 0 ; i < tStartBeepCnt ; i++ ){
      sound_effect(KEY_TONE, 1);
      delay_ms(200);
    }    
  }

  gLobbyHandle.BeepSoundMute = tBeepSoundMute;
  
}

//U8 tArrData[7] = { 0x30,0x20,0x10,0x10,0x1F,0x10,0x1F };

/*
// EEPROM READ TEST
U8 tEepromRead;
U8 tEepromData[100];
U16 tEeprpmAddr;
U8 tEeprpmDDDr;
*/
/*
U8 gRfTest = false;
U8 gRfTestData[10];
*/
/// @brief  빌라형 로비폰의 메인이 되는 함수
/// @param  void
/// @return void
void main(void)
{
  U8 tInitLobyHandle;
  U16 i;


  
  DISABLE_IRQ();               //global interrupt disable
  
  InitializeClock();
  InitializeWdt();
  InitializePort();
  
  delay_ms(1000);
  
  tInitLobyHandle = InitializeLobbyHandle();
  
  InitializeTimer();
  
  wdt_reset();
  InitializeFnd();
  InitializeInt();
  InitializeTimeOut();
  
  InitializeConsole();
  InitializeSound();
  InitializeMessageBox();
  InitializeResourceCall();
  InitializeCallManager();
  Kd_Initialize();
  InitializeKeyScan();
  InitalizeRFCard();

  KDP_LobbySetupChannel();
  ProtocolSpecSetting();
  
  wdt_reset();

  FactoryTestMode();

  InitializeTimeScheduler();
  SetupTimeScheduler( ThreadKeyScan, THREAD_KEY_SCAN_TIME );
  SetupTimeScheduler( ThreadAnalysisProtocol, THREAD_ANALYSIS_PROTOCOL_TIME );
  SetupTimeScheduler( ThreadDoorOpen, THREAD_DOOR_OPEN_TIME );
  SetupTimeScheduler( ThreadTimeOut, THREAD_TIME_OUT_TIME);
  SetupTimeScheduler( ThreadConsole, THREAD_CONSOLE_TIME);
  SetupTimeScheduler( ThreadRFCard, THREAD_RFCARD_TIME);
  StopTimeScheduler ( ThreadConsole );

  ProtocolSpecSetting();

  ENABLE_IRQ();

  BootInfoOutput( tInitLobyHandle );

  gsTimerCnt = 0;
  wdt_reset();

  //SettingValueHexFnd(gFwVersion);
  //delay_ms(2000);
  //TalkAgeingTest(); // Talk Ageing Test
  //Anam_TransmitData( tArrData );

  
  while(1)
  {
    wdt_reset();
/*
    if( gRfTest == true ){
      rd_RFCard( gRfTestData );
    }
*/    
    GoTimeScheduler();
/*
// EEPROM READ TEST
    if( tEepromRead == true )
    {
      tEepromRead = false;
      for( i = 133; i < (140) ; i++)
      {
        EEPROM_ReadBlock( i, &tEepromData[i - 133], 1 );
      }
    }   
*/
  }
}








