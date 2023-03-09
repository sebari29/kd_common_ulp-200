
/// @file func_console.c
/// @date 2012/02/14
/// @author jinhwa Lee
/// @brief  터미널 관리 프로그램 - 사용자 설정

#include "main.h"


extern U16 gFwVersion;

U8  Console_ULP200[]            ="> ULP-200  Ver : ";

U8  Console_MainMenu[]          ="> ** MAIN MENU ** (Time Out : 10min)";
U8  Console_Packet[]            ="> 1. Print Debug Message : ";
U8  Console_Management[]        ="> 2. Management Setting";
U8  Console_Factory[]           ="> 3. Factory Test Mode";
U8  Console_HouseScan[]         ="> 3. Scan All Houses";

U8  Console_HouseScan2[]        ="*************** Scan All Houses ***************";

U8  Console_ManagerMenu[]       ="> ** Management Setting Main Menu ** ";
U8  Console_Dong[]              ="> 1. Dong Number----------------- : ";
U8  Console_Lobby[]             ="> 2. Lobby ID(1:Master,2~32:Slave): ";
U8  Console_GuardID[]           ="> 3. Guard ID(1~4)--------------- : ";
U8  Console_Password[]          ="> 4. Password-------------------- : ";
U8  Console_MaxHo[]             ="> 5. Max House Number------------ : ";
U8  Console_Opentime[]          ="> 6. Door Open time-------------- : ";
U8  Console_Protocol[]          ="> 7. Protocol(1:HD,2:KD,3:AN)---- : ";
U8  Console_AudioPathHouse1[]   ="> 8. House Line 1 (1:T, 0:F)----- : ";
U8  Console_AudioPathHouse2[]   ="> 9. House Line 2 (1:T, 0:F)----- : ";
U8  Console_AudioPathLobby1[]   ="> A. Lobby Line 1 (1:T, 0:F)----- : ";
U8  Console_AudioPahtLobby2[]   ="> B. Lobby Line 2 (1:T, 0:F)----- : ";
U8  Console_AudioPathGuard1[]   ="> C. Guard Line   (1:T, 0:F)----- : ";
U8  Console_BeepSoundMute[]     ="> D. Beep Sound Mute (1:T, 2:F)-- : ";
U8  Console_Exit[]              ="> E. Exit";

U8  Console_ManagerMenuSub[]    ="> ** Management Setting Sub Menu ** ";

U8  Console_True[]              ="True ";
U8  Console_False[]             ="False ";

U8  Console_House_RX[]          = "House Rx: ";
U8  Console_House_TX[]          = "House Tx: ";
U8  Console_Guard_RX[]          = "Guard Rx: ";
U8  Console_Guard_TX[]          = "Guard Tx: ";
U8  Console_Lobby_RX[]          = "Lobby Rx: ";
U8  Console_Lobby_TX[]          = "Lobby Tx: ";

U8  Console_TimeOutMsg[]        = "Time Out....\nPrint Debug Message Reset : False";

U8  Console_Input[]             ="> Please enter the number here : ";
U8  Console_Data_Error[]        ="> Data Error!!            ";

U8  Console_HooPrint[]          ="House Number";

U8  Console_PleaseDisable[]     ="Please Disable [Print Debug Message]...";

U8  Console_C[]                 ="[";

U8  Console_CC[]                ="[[";
U8  Console_JJ[]                ="]] ";
U8  Console_NULL8[]             ="        ";
U8  Console_BAR8[]              ="--------";
U8  Console_BAR1[]              ="| ";

U8  Console_X[]                 ="]:X ";
U8  Console_O[]                 ="]:O ";

U8 Console_GL_CBUSY[]       ="GL_CBUSY     "; 
U8 Console_GL_DOPEN[]       ="GL_DOPEN     ";
U8 Console_GL_OFF_HOOK[]    ="GL_OFF_HOOK  ";
U8 Console_GL_ON_HOOK[]     ="GL_ON_HOOK   ";
U8 Console_GR_CALL_OFF[]    ="GR_CALL_OFF  ";
U8 Console_GR_CALL_OK[]     ="GR_CALL_OK   ";
U8 Console_GR_CALL_ON[]     ="GR_CALL_ON   ";
U8 Console_GR_CBUSY[]       ="GR_CBUSY     ";
U8 Console_GR_HOLD_ON[]     ="GR_HOLD_ON   ";
U8 Console_GR_HOLD_OFF[]    ="GR_HOLD_OFF  ";
U8 Console_GR_INT_COM[]     ="GR_INT_COM   ";
U8 Console_GR_LBUSY[]       ="GR_LBUSY     ";
U8 Console_GR_OFF_HOOK[]    ="GR_OFF_HOOK  ";
U8 Console_GR_ON_HOOK[]     ="GR_ON_HOOK   ";
U8 Console_INTC_ON_HOOK[]   ="INTC_ON_HOOK ";
U8 Console_LG_CALL_OFF[]    ="LG_CALL_OFF  ";
U8 Console_LG_CALL_OK[]     ="LG_CALL_OK   ";
U8 Console_LG_CALL_ON[]     ="LG_CALL_ON   ";
U8 Console_LG_ON_HOOK[]     ="LG_ON_HOOK   ";
U8 Console_LR_CALL_OFF[]    ="LR_CALL_OFF  ";
U8 Console_LR_CALL_OK[]     ="LR_CALL_OK   ";
U8 Console_LR_CALL_ON[]     ="LR_CALL_ON   ";
U8 Console_LR_ON_HOOK[]     ="LR_ON_HOOK   ";
U8 Console_RG_CALL_OFF[]    ="RG_CALL_OFF  ";
U8 Console_RG_CALL_OK[]     ="RG_CALL_OK   ";
U8 Console_RG_CALL_ON[]     ="RG_CALL_ON   ";
U8 Console_RG_CBUSY[]       ="RG_CBUSY     ";
U8 Console_RG_OFF_HOOK[]    ="RG_OFF_HOOK  ";
U8 Console_RG_ON_HOOK[]     ="RG_ON_HOOK   ";
U8 Console_RL_CBUSY[]       ="RL_CBUSY     ";
U8 Console_RL_DOPEN[]       ="RL_DOPEN     ";
U8 Console_RL_OFF_HOOK[]    ="RL_OFF_HOOK  ";
U8 Console_RL_ON_HOOK[]     ="RL_ON_HOOK   ";
U8 Console____ON_HOOK[]     ="___ON_HOOK   ";
U8 Console_RL_CALL_REQUEST[]="RL_CALL_REQU ";
U8 Console_LR_ECHO[]        ="LR_ECHO      ";
U8 Console_RG_EMER[]        ="RG_EMER      ";
U8 Console_RG_EMER_CNCL[]   ="RG_EMER_CNCL ";
U8 Console_PROTOCOL_NULL[]  ="             ";


typeCircularBuffer gtTXConsolBuffer;
typeCircularBuffer gtRXConsolBuffer;
U8 pbTXConsolBuffer[CONSOLE_TX_BUFFER_SIZE];
U8 pbRXConsolBuffer[CONSOLE_RX_BUFFER_SIZE];

U8  SaveData[CONSOLE_SAVE_BUFFER_SIZE];

U8  ConsoleState;
U8  ConsoleSetNum;

HANDLE hConsoleTimeOut;


/// @brief Console Initialize
/// @param void
/// @return void
void InitializeConsole(void)
{
  InitializeCircularBuffer( &gtTXConsolBuffer, pbTXConsolBuffer, CONSOLE_TX_BUFFER_SIZE);
  InitializeCircularBuffer( &gtRXConsolBuffer, pbRXConsolBuffer, CONSOLE_RX_BUFFER_SIZE);
  hConsoleTimeOut = CreateTimeOut();
}


/// @brief 10진수 데이터를 Consol출력
/// @param tPrintData 출력할 데이터
/// @return void
void bPrintDec ( U8 tPrintData )
{
  U8 i;
  U8 tDec100,tDec10,tDec1;

  tDec1   = (U8)(tPrintData % 10);
  tDec10  = (U8)((tPrintData % 100)/10);
  tDec100 = (U8)(tPrintData / 100);
  
  tDec1   += '0';
  tDec10  += '0';
  tDec100 += '0';

  EnqueCircularBuffer( &gtTXConsolBuffer, tDec100 );
  EnqueCircularBuffer( &gtTXConsolBuffer, tDec10 );
  EnqueCircularBuffer( &gtTXConsolBuffer, tDec1 );
  bStartTXConsol();
}


/// @brief 10진수 데이터를 Consol출력 ( 상위 숫자 0은 출력 하지 않음 )
/// @param tPrintData 출력할 데이터
/// @return void
void bPrintDec_0isNull ( U8 tPrintData )
{
  U8 i;
  U8 tDec100,tDec10,tDec1;

  tDec1   = (U8)(tPrintData % 10);
  tDec10  = (U8)((tPrintData % 100)/10);
  tDec100 = (U8)(tPrintData / 100);
  
  tDec1   += '0';
  tDec10  += '0';
  tDec100 += '0';

  if(tDec100 != '0') EnqueCircularBuffer( &gtTXConsolBuffer, tDec100 );
  if(tDec10 != '0')  EnqueCircularBuffer( &gtTXConsolBuffer, tDec10 );
  EnqueCircularBuffer( &gtTXConsolBuffer, tDec1 );
  bStartTXConsol();
}


/// @brief Version 데이터를 Consol출력
/// @param void
/// @return void
void bVersionPrint( void )
{
  U8 tVersion;
  U8 tString[2] = '.';
  tVersion = (U8)(gFwVersion >> 8);
  bPrintHex_NonSpace( &tVersion, 1 );
  bPrintString( tString );
  tVersion = (U8)(gFwVersion & 0xFF);
  bPrintHex_NonSpace( &tVersion, 1 );
}


/// @brief 16진수 데이터를 Consol출력(16bit)
/// @param tPrintdata 출력할 데이터 
/// @return void
void bPrintHex16( U16 tPrintdata )
{
  U8 tdata;
  tdata = (U8)(tPrintdata >> 8);
  bPrintHex_NonSpace( &tdata, 1 );
  tdata = (U8)(tPrintdata & 0xFF);
  bPrintHex( &tdata, 1 );
}

/// @brief 16진수 데이터를 Length 길이 만큼 Consol출력 (Space O)
/// @param[in] data 출력될 데이터 포인터
/// @param[in] tLength 
/// @return void
void bPrintHex( U8 *data, U8 tLength )
{
  U8 i;
  U16 tHex;
  U8 tHexH,tHexL,tTemp;
  tTemp = ' ';

  for ( i = 0; i < tLength; i++ )
  {
    tHex = Dec2Hex( data[i] );
    tHexH = (U8)(tHex>>8);
    tHexL = (U8)(tHex&0x00FF);
    EnqueCircularBuffer( &gtTXConsolBuffer, tHexH );
    EnqueCircularBuffer( &gtTXConsolBuffer, tHexL );
    EnqueCircularBuffer( &gtTXConsolBuffer, tTemp );
  }

  
  bStartTXConsol();
}


/// @brief 16진수 데이터를 Length 길이 만큼 Consol출력 (Space X)
/// @param[in] data 출력될 데이터 포인터
/// @param[in] tLength 출력될 길이
/// @return void
void bPrintHex_NonSpace( U8 *data, U8 tLength )
{
  U8 i;
  U16 tHex;
  U8 tHexH,tHexL,tTemp;
  for ( i = 0; i < tLength; i++ )
  {
    tHex = Dec2Hex( data[i] );
    tHexH = (U8)(tHex>>8);
    tHexL = (U8)(tHex&0x00FF);
    EnqueCircularBuffer( &gtTXConsolBuffer, tHexH );
    EnqueCircularBuffer( &gtTXConsolBuffer, tHexL );
  }
  bStartTXConsol();
}


/// @brief 입출력 프로토콜들을 Consol 출력
/// @param[in] data 출력될 데이터 포인터
/// @param[in] tLength 출력될 길이
/// @param[in] tPath 송수신 위치를 파악하기위한 변수
/// @return void
void bPrintHex_Protocol( U8 *data, U8 tLength, U8 tPath )
{
  U8 i;
  U16 tHex;
  U8 tHexH,tHexL,tTemp;

  //while(!IsEmptyCircularBuffer(&gtTXConsolBuffer));

  tTemp = 10;
  EnqueCircularBuffer( &gtTXConsolBuffer, tTemp );
  tTemp = 13;
  EnqueCircularBuffer( &gtTXConsolBuffer, tTemp );

  switch ( tPath )
  {
    case RECEIVE_LINE_HOUSE:
      ConsoleDataDisplay(Console_House_RX);
      break;
    case RECEIVE_LINE_LOBBY:
      ConsoleDataDisplay(Console_Lobby_RX);
      break;
    case RECEIVE_LINE_GUARD:
      ConsoleDataDisplay(Console_Guard_RX);
      break;
    case TRANSMIT_LINE_HOUSE:
      ConsoleDataDisplay(Console_House_TX);
      break;
    case TRANSMIT_LINE_LOBBY:
      ConsoleDataDisplay(Console_Lobby_TX);
      break;
    case TRANSMIT_LINE_GUARD:
      ConsoleDataDisplay(Console_Guard_TX);
      break;
    default:
      break;
  }

  tTemp = ' ';
  EnqueCircularBuffer( &gtTXConsolBuffer, tTemp );

  bPrintPtocotolCmd( data[1] );
  
  for ( i = 0; i < tLength; i++ )
  {
    tHex = Dec2Hex( data[i] );
    tHexH = (U8)(tHex>>8);
    tHexL = (U8)(tHex&0x00FF);
    EnqueCircularBuffer( &gtTXConsolBuffer, tHexH );
    EnqueCircularBuffer( &gtTXConsolBuffer, tHexL );
    EnqueCircularBuffer( &gtTXConsolBuffer, tTemp );
  }
  bStartTXConsol();
}


/// @brief 문자열을  Consol 출력
/// @param[in] data 출력될 데이터 포인터
/// @return void
void bPrintString( U8 *data )
{
  U8 i;
  i = 0;
  
  //while(!IsEmptyCircularBuffer(&gtTXConsolBuffer));
  while(data[i] && i < CONSOLE_TX_BUFFER_SIZE)
  {
    EnqueCircularBuffer( &gtTXConsolBuffer, data[i] );
    i++;
  }
  bStartTXConsol();
}


/// @brief 문자열을 Length 만큼  Consol 출력
/// @param[in] data 출력될 데이터 포인터
/// @param[in] length 출력되 데이터 길이
/// @return void
void bPutTXConsoleData( U8* data, U8 length )
{
  U8 i;
  for( i = 0; i < length; i++)
  {
    EnqueCircularBuffer( &gtTXConsolBuffer, data[i] );
    //delay_ms(1);
  }
}


/// @brief 버커에서 출력될 데이터 가지고 오는 함수
/// @param[out] data 출력될 데이터 포인터
/// @return true = 성공 \n false = 실패
U8 bGetTXConsoleData( U8* data )
{
  if (!DequeCircularBuffer( &gtTXConsolBuffer, data )) return true;
  return false;
}


/// @brief 버퍼에 있는 데이터 출력 시작
/// @param void
/// @return true = 성공 \n false = 실패
U8 bStartTXConsol(void)
{
  U8 tdata;


  if ( oer_u0rb || fer_u0rb || per_u0rb || sum_u0rb ) InitializeUart0_19200();

  while( !IsEmptyTxBufferUart0 );
  
  if ( bGetTXConsoleData( &tdata ) )
  {
    TransmitBufferUart0 = tdata;
    return true;
  }
  return false;
}


/// @brief 문자열을  Consol 출력
/// @param[in] data 출력될 데이터 포인터
/// @return void
void ConsoleDataDisplay( U8* data)
{
  wdt_reset();
  bPrintString(data);
}


/// @brief Enter를  Consol 출력
/// @param void
/// @return void
void  EnterDisplay(void)
{
  U8  ConsoleData[2];

  ConsoleData[0] = 0x0a;
  ConsoleData[1] = 0x0d;

  bPutTXConsoleData(ConsoleData, 2);
  bStartTXConsol();
}


/// @brief Main Page를 Consol 출력
/// @param void
/// @return void
void PrintMainPage(void)
{
  EnterDisplay();
  EnterDisplay();
  EnterDisplay(); 
  EnterDisplay(); 
  ConsoleDataDisplay(Console_ULP200);bVersionPrint();
  EnterDisplay();
  EnterDisplay(); 
  EnterDisplay(); 
  ConsoleDataDisplay(Console_MainMenu);EnterDisplay();
  ConsoleDataDisplay(Console_Packet);
  if ( gLobbyHandle.PacketMonitoring == true ) ConsoleDataDisplay(Console_True);
  else ConsoleDataDisplay(Console_False);

  EnterDisplay();
  ConsoleDataDisplay(Console_Management);EnterDisplay();
  ConsoleDataDisplay(Console_HouseScan);EnterDisplay();
  ConsoleDataDisplay(Console_Input);
}


/// @brief 설정 메뉴를 Consol 출력
/// @param void
/// @return void
void PrintSettingMenu(void)
{
  //HandleDataSave();
  U16 tDongNumber,tPassword;

  Dec2HalfHex( gLobbyHandle.DongNumber, &tDongNumber );
  Dec2HalfHex( gLobbyHandle.Password,   &tPassword );
  
  EnterDisplay();
  EnterDisplay();
  EnterDisplay();
  ConsoleDataDisplay(Console_ManagerMenu);
  EnterDisplay();  
  EnterDisplay();  
  ConsoleDataDisplay(Console_Dong);     bPrintHex16(tDongNumber);EnterDisplay();
  ConsoleDataDisplay(Console_Lobby);    bPrintDec_0isNull(gLobbyHandle.LobbyID & 0x3F);EnterDisplay();
  ConsoleDataDisplay(Console_GuardID);  bPrintDec_0isNull(gLobbyHandle.GuardID);EnterDisplay();
  ConsoleDataDisplay(Console_Password); bPrintHex16(tPassword);EnterDisplay();
  ConsoleDataDisplay(Console_MaxHo);    bPrintDec_0isNull(gLobbyHandle.MaxHo);EnterDisplay();
  ConsoleDataDisplay(Console_Opentime); bPrintDec_0isNull(gLobbyHandle.OpenTimeDoor);EnterDisplay();
  ConsoleDataDisplay(Console_Protocol); bPrintDec_0isNull(gLobbyHandle.Protocol);EnterDisplay();
  ConsoleDataDisplay(Console_AudioPathHouse1);  bPrintDec_0isNull(gLobbyHandle.AudioPathHouse[0]);EnterDisplay();
  ConsoleDataDisplay(Console_AudioPathHouse2);  bPrintDec_0isNull(gLobbyHandle.AudioPathHouse[1]);EnterDisplay();
  ConsoleDataDisplay(Console_AudioPathLobby1);  bPrintDec_0isNull(gLobbyHandle.AudioPathLobby[0]);EnterDisplay();
  ConsoleDataDisplay(Console_AudioPahtLobby2);  bPrintDec_0isNull(gLobbyHandle.AudioPathLobby[1]);EnterDisplay();
  ConsoleDataDisplay(Console_AudioPathGuard1);  bPrintDec_0isNull(gLobbyHandle.AudioPathGuard[0]);EnterDisplay();
  ConsoleDataDisplay(Console_BeepSoundMute);    bPrintDec_0isNull(gLobbyHandle.BeepSoundMute);EnterDisplay();
  ConsoleDataDisplay(Console_Exit); EnterDisplay(); 
  ConsoleDataDisplay(Console_Input);
  bStartTXConsol();
}


/// @brief 각각의 설정 메뉴를 Consol 출력
/// @param tData 출력될 설정 메뉴
/// @return true = 성공 \n false = 실패
U8 PrintEachSettingMenu(U8 tData)
{
  U8  OkFault = 0;

  U16 tDongNumber,tPassword;

  Dec2HalfHex( gLobbyHandle.DongNumber, &tDongNumber );
  Dec2HalfHex( gLobbyHandle.Password,   &tPassword );
  
  EnterDisplay();
  EnterDisplay();
  EnterDisplay();
  EnterDisplay();
  ConsoleDataDisplay(Console_ManagerMenuSub);
  EnterDisplay();
  EnterDisplay();
  switch(tData)
  {
    case  ASCII_ONE:
      ConsoleDataDisplay(Console_Dong);
      bPrintHex16(tDongNumber);EnterDisplay(); 
      break;
    case  ASCII_TWO:
      ConsoleDataDisplay(Console_Lobby);
      bPrintDec_0isNull(gLobbyHandle.LobbyID & 0x3F);EnterDisplay(); 
      break;
    case  ASCII_THREE:
      ConsoleDataDisplay(Console_GuardID);
      bPrintDec_0isNull(gLobbyHandle.GuardID);EnterDisplay(); 
      break;
    case  ASCII_FOUR:
      ConsoleDataDisplay(Console_Password);
      bPrintHex16(tPassword);EnterDisplay(); 
      break;
    case  ASCII_FIVE:
      ConsoleDataDisplay(Console_MaxHo);
      bPrintDec_0isNull(gLobbyHandle.MaxHo);EnterDisplay(); 
      break;
    case  ASCII_SIX:
      ConsoleDataDisplay(Console_Opentime);
      bPrintDec_0isNull(gLobbyHandle.OpenTimeDoor);EnterDisplay(); 
      break;
    case  ASCII_SEVEN:
      ConsoleDataDisplay(Console_Protocol);
      bPrintDec_0isNull(gLobbyHandle.Protocol);EnterDisplay(); 
      break;
    case  ASCII_EIGHT:
      ConsoleDataDisplay(Console_AudioPathHouse1);
      bPrintDec_0isNull(gLobbyHandle.AudioPathHouse[0]);EnterDisplay(); 
      break;
    case  ASCII_NINE:
      ConsoleDataDisplay(Console_AudioPathHouse2);
      bPrintDec_0isNull(gLobbyHandle.AudioPathHouse[1]);EnterDisplay(); 
      break;
    case  ASCII_A:
    case  ASCII_a:
      ConsoleDataDisplay(Console_AudioPathLobby1);
      bPrintDec_0isNull(gLobbyHandle.AudioPathLobby[0]);EnterDisplay(); 
      break;
    case  ASCII_B:
    case  ASCII_b:
      ConsoleDataDisplay(Console_AudioPahtLobby2);
      bPrintDec_0isNull(gLobbyHandle.AudioPathLobby[1]);EnterDisplay(); 
      break;
    case  ASCII_C:
    case  ASCII_c:
      ConsoleDataDisplay(Console_AudioPathGuard1);
      bPrintDec_0isNull(gLobbyHandle.AudioPathGuard[0]);EnterDisplay(); 
      break;
    case  ASCII_D:
    case  ASCII_d:
      ConsoleDataDisplay(Console_BeepSoundMute);
      bPrintDec_0isNull(gLobbyHandle.BeepSoundMute);EnterDisplay(); 
      break;
    default:
      return false;
      break;
  }
  ConsoleDataDisplay(Console_Exit); EnterDisplay(); 
  ConsoleDataDisplay(Console_Input);
  return true;

}



/// @brief LR_ECHO 프로토콜 전송 함수
/// @param tID 전송 될 ID
/// @return void
void TransmitProtocol_LR_ECHO( U8 tID )
{
  TransmitProtocolHouse( tID, LR_ECHO, 1, 1);
}


/// @brief Console에서 세대기에 Echo 호출 해봄
/// @param tStart 처음 시작할때 True를 넣어줌
/// @return true = 성공 \n false = 실패
U8 ScanAllHouses(U8 tStart)
{
  static U8 tHouseID;
  static U8 tFloor;
  static U8 tStartFlg = false;
  typeMsgBox tMsgBox;
  U8 i;
  
  if ( tStart )
  {
    if ( tStartFlg == true ) return false;
    tHouseID = 1;
    tFloor = 1;
    TransmitProtocol_LR_ECHO( tHouseID );
    EnterDisplay();
    EnterDisplay();
    ConsoleDataDisplay(Console_HouseScan2);
    EnterDisplay();
    ConsoleDataDisplay(Console_NULL8);

    for ( i = 0 ; i < gLobbyHandle.MaxHo ; i++)
    {
      ConsoleDataDisplay(Console_CC);
      bPrintDec((U8)(i+1));
      ConsoleDataDisplay(Console_JJ);
    }
    EnterDisplay();
    for ( i=0 ; i < gLobbyHandle.MaxHo+1 ; i++)
    {
      ConsoleDataDisplay(Console_BAR8);
    }
    
    EnterDisplay();
    ConsoleDataDisplay(Console_CC);
    bPrintDec(tFloor);
    ConsoleDataDisplay(Console_JJ);
    tStartFlg = true;
    return false;
  }

  if ( tStartFlg == false ) return false;

  if ( bReceiveMessageBox( MSG_ID_ScanAllHouses, &tMsgBox ) )
  {
    if ( (tMsgBox.Buf.Byte[0] & 0x7F) == tHouseID )
    {
      ConsoleDataDisplay(Console_C);
      bPrintDec(tHouseID);
      ConsoleDataDisplay(Console_O);
    }
  }
  else
  {
    ConsoleDataDisplay(Console_C);
    bPrintDec(tHouseID);
    ConsoleDataDisplay(Console_X);
  }

  if( tHouseID % gLobbyHandle.MaxHo == 0 )
  {
    EnterDisplay();
    tHouseID++;
    if ( tHouseID > 127 )
    {
      tStartFlg = false;
      return false;
    }
    ConsoleDataDisplay(Console_CC);
    bPrintDec(++tFloor);
    ConsoleDataDisplay(Console_JJ);
  }
  else
  {
    tHouseID++;
    if ( tHouseID > 127 )
    {
      tStartFlg = false;
      return false;
    }
  }

  TransmitProtocol_LR_ECHO( tHouseID );

  return true;

}


/// @brief 입력값을 상하한 값을 비교하여 변수에 적용
/// @param tInputCharAddr 입력된 문자열 포인터
/// @param tInputCharCnt  입력된 문자 갯수
/// @param tWriteAddr     출력할 변수 포인터
/// @param tWriteSizeof   출력할 변수 사이즈
/// @param tMax           입력된 문자열 상한값
/// @param tMin           입력된 문자열 하한값
/// @return true = 성공 \n false = 실패
U8 SettingDataConvertDec(U8 *tInputCharAddr, U8 tInputCharCnt, U8 *tWriteAddr, U8 tWriteSizeof, U16 tMax, U16 tMin)
{
  U8 i;
  U8  tInputChar;
  U16  tInputNum = 0;
  U16 *tWrite16;

  for( i=0; i< tInputCharCnt; i++)
  {
    tInputChar = (U8)(tInputCharAddr[i] - '0');
    if ( tInputChar > 9 ) return false;
    tInputNum *= 10;
    tInputNum += tInputChar;
  }

  if ( tInputNum > tMax || tInputNum < tMin) return false;

  if( tWriteSizeof == 2 )
  {
     tWrite16 = (U16*)tWriteAddr;
    *tWrite16 = tInputNum;
  }
  else
  {
    *tWriteAddr = tInputNum;
  }
  return true;
  
}



/// @brief 데이터를 EEPROM에 저장함
/// @param tData 입력된 문자열 포인터
/// @param tCount  입력된 문자 갯수
/// @return true = 성공 \n false = 실패
U8 SettingData(U8* tData, U8 tCount)  
{
  //U8  *pLobbyHandle;
  //U16  tMax, tMin;
  //U8 tReturn;

  U16 tAddr;
  U16 tAccData;

  switch(ConsoleSetNum)
  {
    case  ASCII_ONE:
      if( SettingDataConvertDec( tData, tCount, (U8*)&tAccData, 2, MAX_DongNumber, MIN_DongNumber) )
      {
        tAddr = (((U32)&gLobbyHandle.DongNumber - (U32)&gLobbyHandle.DongNumber) * 2 ) + LOBBY_HANDLE_ADDR;
        if ( SpecialVal_WriteBlock( tAddr, (U8*)&tAccData, 2))
        {
          gLobbyHandle.DongNumber = (U16)tAccData;  
          return true;
        }
      }     
      break;
    case  ASCII_TWO:
      if ( (*tData > MAX_LobbyID) || (*tData < MIN_LobbyID)) return false;
           
      *tData |= LOBBY_ID_MASK;
      if( SettingDataConvertDec( tData, tCount, (U8*)&tAccData, 1, 0xFF, 0xFF) )
      {
        tAddr = (((U32)&gLobbyHandle.LobbyID - (U32)&gLobbyHandle.DongNumber) * 2 ) + LOBBY_HANDLE_ADDR;
        if ( SpecialVal_WriteBlock( tAddr, (U8*)&tAccData, 1))
        {
          gLobbyHandle.LobbyID = (U16)tAccData;
          KDP_LobbySetupChannel();
          return true;
        }
      }   
      break;
    case  ASCII_THREE:
      if( SettingDataConvertDec( tData, tCount, (U8*)&tAccData, 1, MAX_GuardID, MIN_GuardID) )
      {
        tAddr = (((U32)&gLobbyHandle.GuardID - (U32)&gLobbyHandle.DongNumber) * 2 ) + LOBBY_HANDLE_ADDR;
        if ( SpecialVal_WriteBlock( tAddr, (U8*)&tAccData, 1))
        {
          gLobbyHandle.GuardID = (U16)tAccData;  
          return true;
        }
      }     
      break;     
    case  ASCII_FOUR:
      if ( tCount < 4 ) return false;
      if( SettingDataConvertDec( tData, tCount, (U8*)&tAccData, 2, MAX_Password, MIN_Password) )
      {
        tAddr = (((U32)&gLobbyHandle.Password - (U32)&gLobbyHandle.DongNumber) * 2 ) + LOBBY_HANDLE_ADDR;
        if ( SpecialVal_WriteBlock( tAddr, (U8*)&tAccData, 2))
        {
          gLobbyHandle.Password = (U16)tAccData;  
          return true;
        }
      }     
      break;      
    case  ASCII_FIVE:
      if( SettingDataConvertDec( tData, tCount, (U8*)&tAccData, 1, MAX_MaxHo, MIN_MaxHo) )
      {
        tAddr = (((U32)&gLobbyHandle.MaxHo - (U32)&gLobbyHandle.DongNumber) * 2 ) + LOBBY_HANDLE_ADDR;
        if ( SpecialVal_WriteBlock( tAddr, (U8*)&tAccData, 1))
        {
          gLobbyHandle.MaxHo = (U16)tAccData;  
          return true;
        }
      }     
      break;   
    case  ASCII_SIX:
      if( SettingDataConvertDec( tData, tCount, (U8*)&tAccData, 1, MAX_OpenTimeDoor, MIN_OpenTimeDoor) )
      {
        tAddr = (((U32)&gLobbyHandle.OpenTimeDoor - (U32)&gLobbyHandle.DongNumber) * 2 ) + LOBBY_HANDLE_ADDR;
        if ( SpecialVal_WriteBlock( tAddr, (U8*)&tAccData, 1))
        {
          gLobbyHandle.OpenTimeDoor = (U16)tAccData;  
          return true;
        }
      }     
      break;   
    case  ASCII_SEVEN:
      if( SettingDataConvertDec( tData, tCount, (U8*)&tAccData, 1, MAX_Protocol, MIN_Protocol) )
      {
        tAddr = (((U32)&gLobbyHandle.Protocol - (U32)&gLobbyHandle.DongNumber) * 2 ) + LOBBY_HANDLE_ADDR;
        if ( SpecialVal_WriteBlock( tAddr, (U8*)&tAccData, 1))
        {
          gLobbyHandle.Protocol = (U16)tAccData;  
          ProtocolSpecSetting();
          return true;
        }
      }     
      break;   
    case  ASCII_EIGHT:
      if( SettingDataConvertDec( tData, tCount, (U8*)&tAccData, 1, MAX_AudioPath, MIN_AudioPath) )
      {
        tAddr = (((U32)&gLobbyHandle.AudioPathHouse[0] - (U32)&gLobbyHandle.DongNumber) * 2 ) + LOBBY_HANDLE_ADDR;
        if ( SpecialVal_WriteBlock( tAddr, (U8*)&tAccData, 1))
        {
          gLobbyHandle.AudioPathHouse[0] = (U16)tAccData;  
          return true;
        }
      }     
      break;  
    case  ASCII_NINE:
      if( SettingDataConvertDec( tData, tCount, (U8*)&tAccData, 1, MAX_AudioPath, MIN_AudioPath) )
      {
        tAddr = (((U32)&gLobbyHandle.AudioPathHouse[1] - (U32)&gLobbyHandle.DongNumber) * 2 ) + LOBBY_HANDLE_ADDR;
        if ( SpecialVal_WriteBlock( tAddr, (U8*)&tAccData, 1))
        {
          gLobbyHandle.AudioPathHouse[1] = (U16)tAccData;  
          return true;
        }
      }     
      break;
    case  ASCII_A:
    case  ASCII_a:
      if( SettingDataConvertDec( tData, tCount, (U8*)&tAccData, 1, MAX_AudioPath, MIN_AudioPath) )
      {
        tAddr = (((U32)&gLobbyHandle.AudioPathLobby[0] - (U32)&gLobbyHandle.DongNumber) * 2 ) + LOBBY_HANDLE_ADDR;
        if ( SpecialVal_WriteBlock( tAddr, (U8*)&tAccData, 1))
        {
          gLobbyHandle.AudioPathLobby[0] = (U16)tAccData;  
          return true;
        }
      }     
      break;     
    case  ASCII_B:
    case  ASCII_b:
      if( SettingDataConvertDec( tData, tCount, (U8*)&tAccData, 1, MAX_AudioPath, MIN_AudioPath) )
      {
        tAddr = (((U32)&gLobbyHandle.AudioPathLobby[1] - (U32)&gLobbyHandle.DongNumber) * 2 ) + LOBBY_HANDLE_ADDR;
        if ( SpecialVal_WriteBlock( tAddr, (U8*)&tAccData, 1))
        {
          gLobbyHandle.AudioPathLobby[1] = (U16)tAccData;  
          return true;
        }
      }     
      break;
    case  ASCII_C:
    case  ASCII_c:
      if( SettingDataConvertDec( tData, tCount, (U8*)&tAccData, 1, MAX_AudioPath, MIN_AudioPath) )
      {
        tAddr = (((U32)&gLobbyHandle.AudioPathGuard[1] - (U32)&gLobbyHandle.DongNumber) * 2 ) + LOBBY_HANDLE_ADDR;
        if ( SpecialVal_WriteBlock( tAddr, (U8*)&tAccData, 1))
        {
          gLobbyHandle.AudioPathGuard[1] = (U16)tAccData;  
          return true;
        }
      }     
      break;      
    case  ASCII_D:
    case  ASCII_d:
      if( SettingDataConvertDec( tData, tCount, (U8*)&tAccData, 1, MAX_BeepSoundMute, MIN_BeepSoundMute) )
      {
        tAddr = (((U32)&gLobbyHandle.BeepSoundMute - (U32)&gLobbyHandle.DongNumber) * 2 ) + LOBBY_HANDLE_ADDR;
        if ( SpecialVal_WriteBlock( tAddr, (U8*)&tAccData, 1))
        {
          gLobbyHandle.BeepSoundMute = (U16)tAccData;  
          return true;
        }
      }     
      break;          
    default:
      break;
  }
  
  return false;
}


/// @brief Console Time Out Reset
/// @param void
/// @return void
void ConsoleTimeOutReset(void)
{
  if( IsTimeOutUse(hConsoleTimeOut) )
  {
    SetupTimeOut(hConsoleTimeOut, CONSOLE_TIMEOUT_SEC);
  }
  else
  {
    SetupTimeOut(hConsoleTimeOut, CONSOLE_TIMEOUT_SEC);
    StartTimeScheduler(ThreadConsole, THREAD_CONSOLE_TIME);
  }
}


/// @brief Console Thread 함수
/// @param void
/// @return void
void ThreadConsole(void)
{
  typeMsgBox tMsgBox;
  U8 Count;
  U8 *Data;
  U8 ConsoleData[20];
  U8 i;
  
  wdt_reset();

  if ( IsTimeOut(hConsoleTimeOut) )
  {
    if ( gLobbyHandle.PacketMonitoring )
    {
      gLobbyHandle.PacketMonitoring = 0;
      EnterDisplay();
      ConsoleDataDisplay(Console_TimeOutMsg);
      EnterDisplay();
    }
    StopTimeScheduler(ThreadConsole);
    //PrintPageHome();
    return;
  }

  if( ScanAllHouses(false) ) return;

  if ( bReceiveMessageBox( MSG_ID_ThreadConsole, &tMsgBox) )//MSG_ID_ThreadConsole, &tMsgBox ) )
  {
    Count = tMsgBox.Cmd;
    Data =  (U8*)&tMsgBox.Buf;
   
     switch(ConsoleState)
     {
       case  CON_STS_0: //default Mode
         if ( Count != 1 )
         {
           PrintMainPage();
           break;
         }
        switch( Data[0] )
        {
          case ASCII_ENTER:
            if ( gLobbyHandle.PacketMonitoring == true )
            {
              EnterDisplay();
              ConsoleDataDisplay(Console_PleaseDisable);
              EnterDisplay();
              break;
            }
             PrintMainPage();
            break;
          case ASCII_ONE:
            gLobbyHandle.PacketMonitoring ^= 1;
            PrintMainPage();
            break;
          case ASCII_TWO:
            if ( gLobbyHandle.PacketMonitoring == true )
            {
              EnterDisplay();
              ConsoleDataDisplay(Console_PleaseDisable);
              EnterDisplay();
              break;
            }          
            PrintSettingMenu();
            ConsoleState = CON_STS_1;  // Management Setting Mode
            break;
          case ASCII_THREE:
            if ( gLobbyHandle.PacketMonitoring == true )
            {
              EnterDisplay();
              ConsoleDataDisplay(Console_PleaseDisable);
              EnterDisplay();
              break;
            }          
            ScanAllHouses(true);
            break;
          default:
            PrintMainPage();
            break;
        }
        break;
       case  CON_STS_1: // Management Setting Mode
         if ( Count == 1 )
         {
           if ( Data[0] == ASCII_e || Data[0] == ASCII_E )
           {
             PrintMainPage();
             ConsoleState = CON_STS_0; //default Mode
             break;
           }
           
          ConsoleSetNum = Data[0];
          if(PrintEachSettingMenu(ConsoleSetNum) == false)
          {
            PrintSettingMenu();
          }
          else
          {
            ConsoleState = CON_STS_3;// Setting Mode
          }
          break;
         }
         PrintSettingMenu();
         break;
       case  CON_STS_2: // Factory Mode
         //SettingData(tMsgBox.Buf.Byte, Count);
         break;
      case  CON_STS_3: // Setting Mode
      
        if ( Count == 1 && (Data[0] == ASCII_e || Data[0] == ASCII_E) )
        {
          PrintSettingMenu();
          ConsoleState = CON_STS_1; //default Mode
          break;
        }
     
        if ( SettingData(Data, Count) )
        {
          ConsoleState = CON_STS_1;
          PrintSettingMenu();
        }
        else
        {
          EnterDisplay(); EnterDisplay(); 
          ConsoleDataDisplay(Console_Data_Error);
          PrintEachSettingMenu(ConsoleSetNum);
        }
        break;

      default:
        break;
     }
  }
}


/// @brief Protocol Command 출력 함수
/// @param tCmd Command 
/// @return void
void bPrintPtocotolCmd( U8 tCmd )
{
  switch(tCmd)
  {
    case GL_CBUSY:          ConsoleDataDisplay(Console_GL_CBUSY       ); break;
    case GL_DOPEN:          ConsoleDataDisplay(Console_GL_DOPEN       ); break;
    case GL_OFF_HOOK:       ConsoleDataDisplay(Console_GL_OFF_HOOK    ); break;
    case GL_ON_HOOK:        ConsoleDataDisplay(Console_GL_ON_HOOK     ); break;
    case GR_CALL_OFF:       ConsoleDataDisplay(Console_GR_CALL_OFF    ); break;
    case GR_CALL_OK:        ConsoleDataDisplay(Console_GR_CALL_OK     ); break;
    case GR_CALL_ON:        ConsoleDataDisplay(Console_GR_CALL_ON     ); break;
    case GR_CBUSY:          ConsoleDataDisplay(Console_GR_CBUSY       ); break;
    case GR_HOLD_ON:        ConsoleDataDisplay(Console_GR_HOLD_ON     ); break;
    case GR_HOLD_OFF:       ConsoleDataDisplay(Console_GR_HOLD_OFF    ); break;
    case GR_INT_COM:        ConsoleDataDisplay(Console_GR_INT_COM     ); break;
    case GR_LBUSY:          ConsoleDataDisplay(Console_GR_LBUSY       ); break;
    case GR_OFF_HOOK:       ConsoleDataDisplay(Console_GR_OFF_HOOK    ); break;
    case GR_ON_HOOK:        ConsoleDataDisplay(Console_GR_ON_HOOK     ); break;
    case INTC_ON_HOOK:      ConsoleDataDisplay(Console_INTC_ON_HOOK   ); break;
    case LG_CALL_OFF:       ConsoleDataDisplay(Console_LG_CALL_OFF    ); break;
    case LG_CALL_OK:        ConsoleDataDisplay(Console_LG_CALL_OK     ); break;
    case LG_CALL_ON:        ConsoleDataDisplay(Console_LG_CALL_ON     ); break;
    case LG_ON_HOOK:        ConsoleDataDisplay(Console_LG_ON_HOOK     ); break;
    case LR_CALL_OFF:       ConsoleDataDisplay(Console_LR_CALL_OFF    ); break;
    case LR_CALL_OK:        ConsoleDataDisplay(Console_LR_CALL_OK     ); break;
    case LR_CALL_ON:        ConsoleDataDisplay(Console_LR_CALL_ON     ); break;
    case LR_ON_HOOK:        ConsoleDataDisplay(Console_LR_ON_HOOK     ); break;
    case RG_CALL_OFF:       ConsoleDataDisplay(Console_RG_CALL_OFF    ); break;
    case RG_CALL_OK:        ConsoleDataDisplay(Console_RG_CALL_OK     ); break;
    case RG_CALL_ON:        ConsoleDataDisplay(Console_RG_CALL_ON     ); break;
    case RG_CBUSY:          ConsoleDataDisplay(Console_RG_CBUSY       ); break;
    case RG_OFF_HOOK:       ConsoleDataDisplay(Console_RG_OFF_HOOK    ); break;
    case RG_ON_HOOK:        ConsoleDataDisplay(Console_RG_ON_HOOK     ); break;
    case RL_CBUSY:          ConsoleDataDisplay(Console_RL_CBUSY       ); break;
    case RL_DOPEN:          ConsoleDataDisplay(Console_RL_DOPEN       ); break;
    case RL_OFF_HOOK:       ConsoleDataDisplay(Console_RL_OFF_HOOK    ); break;
    case RL_ON_HOOK:        ConsoleDataDisplay(Console_RL_ON_HOOK     ); break;
    case ___ON_HOOK:        ConsoleDataDisplay(Console____ON_HOOK     ); break;
    case RL_CALL_REQUEST:   ConsoleDataDisplay(Console_RL_CALL_REQUEST); break;
    case LR_ECHO:           ConsoleDataDisplay(Console_LR_ECHO        ); break;
    case RG_EMER:           ConsoleDataDisplay(Console_RG_EMER        ); break;
    case RG_EMER_CNCL:      ConsoleDataDisplay(Console_RG_EMER_CNCL   ); break;
    default:
      ConsoleDataDisplay(Console_PROTOCOL_NULL   );
      break;
  }
}

