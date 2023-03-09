
/// @file   main.c
/// @date   2012/02/14
/// @author jinhwa Lee
/// @brief  ���α׷��� �����ϴ� Main ����


/// @mainpage ULP-200 ����������
/// @section intro �Ұ�
/// - �Ұ� : ����ġ ������ �κ��� ������ ���� ���α׷�
/// @section Program ���α׷���
/// - ���α׷���  : ULP-200
/// - ���α׷����� : ������ �κ��� (����ġ ������ �κ���)
/// @section info ���� ����
/// - ���� �� �ұԸ� ����Ʈ�� ����� �� �ִ� �κ��� ����
/// @section advenced ��� CPU
/// - Renesas : R8C Series - R5F2136CC
/// @section CREATEINFO �ۼ�����
/// - �ۼ���      :   ����ȭ
/// - �ۼ���      :   2012/02/13
///


////////////////////////////////////////////////////////////////////////////////
// FW_VERSION = 0x1001 (EX)                                                   //
// Version  :     ^^     = Version 1.0                                        //
// Revision :       ^^   = Revison 01                                         //
////////////////////////////////////////////////////////////////////////////////

#define FW_VERSION 0x1600

/// @section MODIFYINFO ��������
//----------------------------------------------------------------------------
/// - 120203 ; Ver1.0 Rev00 ; DPP ǰ��ȸ / �ŷڼ� �Ƿ� ����                       
/// - 120216 ; Ver1.0 Rev01 ; DPP ǰ��ȸ / �ŷڼ� �Ƿ� ����(TimeOut �ð� ����)    
/// - 120223 ; Ver1.0 Rev02 ; ���� �׽�Ʈ ��� �߰�
/// - 120223 ; Ver1.0 Rev03 ; ����/���� ������ ������ �߰�
/// - 120223 ; Ver1.0 Rev04 ; ���ý� E-00�̶�� �ߴ� ���� �ذ�
/// - 120323 ; Ver1.0 Rev05 ; ���� ��� ���α׷� �߰�
/// - 120323 ; Ver1.0 Rev06 ; �� �ִ�ȣ�� 32�� ����/ ������ CBUSY �ö� ������ ��� �ȵǴ� ���� �ذ�
/// - 120405 ; Ver1.0 Rev07 ; �ִ� ȣ�� �Ѿ���� ID ȣ�� ���°� / ��� Ÿ�Ӿƿ� �߰�
/// - 120406 ; Ver1.0 Rev08 ; �޽��� �ڽ� Ǯ���� �޽��� �ڽ� Ŭ���� ����
/// - 120408 ; Ver1.0 Rev09 ; ���½� ���� �����͸� ������ ��˻縦 �ϴ� �κ� �߰�
/// - 120409 ; Ver1.1 Rev00 ; ��� ����뺸 ���� LPP
/// - 120420 ; Ver1.2 Rev00 ; ���� ��� Ÿ�� �ƿ� 10 �ʿ��� 60�ʷ� �ø�(MP����뺸)
/// - 120719 ; Ver1.3 Rev00 ; �Ƴ� �������� �߰� / �κ�ID 0x40 ���� �������� ���� / ����κ� �������� ������ ����ϴ� �������� ���� / ���� ��忡�� 0�� ������ ������ ���� �ǰ� ����
/// - 120727 ; ver1.3 Rev01 ; ���� �κ� �Ƴ� �������� �߰� / ���� �� ������ �κ� �����Կ��� ���ÿ� ��� ȣ�� �����ϰ�
/// - 121204 ; Ver1.4 Rev00 ; �޴� ���Թ�� ���� ��й�ȣ�� ����/ �浿,����,�Ƴ� �������� �߰�
/// - 140811 ; Ver1.5 Rev00 ; HT �������� ���� ����

///////////////////////////////////////////////////////////////////////////////

#include "main.h"


U16 gFwVersion = FW_VERSION;    ///< �߿��� ����
typeLobbyHandler gLobbyHandle;  ///< �κ��� �⺻ ����

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

/// @brief  ���� TEST ��� �Լ�
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
      case 1: // �浿
        tStartBeepCnt = 3;
        break;
      case 2: // ����
      case 3: // ����
        tStartBeepCnt = 2;
        break;
      case 4: // �Ƴ�
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
/// @brief  ������ �κ����� ������ �Ǵ� �Լ�
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








