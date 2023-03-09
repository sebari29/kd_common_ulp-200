
/// @file   func_protocol.c
/// @date   2012/02/14
/// @author jinhwa Lee
/// @brief  Protocol 관련 파일


#include "main.h"

U8 gUart1TimeOutCnt;
U8 gUart2TimeOutCnt;


void TransmitGuard( U8 tData )
{
  __TransmitBufferUart1 = tData;
}

void TransmitGuardCntrRx( void )
{
  RS485_GUARD_CNTR = CLEAR;
}

void TransmitGuardCntrTx( void )
{
  RS485_GUARD_CNTR = SET;
}


void TransmitHouseLobby( U8 tData )
{
  __TransmitBufferUart2 = tData;
}

void TransmitHouseLobbyCntrRx( void )
{
  RS485_HOUSE_CNTR = CLEAR;
}

void TransmitHouseLobbyCntrTx( void )
{
  RS485_HOUSE_CNTR = SET;
}



/// @brief 세대기 프로토콜을 프로토콜 설정에 의하여 결정해 주는 함수
/// @param tId 세대기 ID
/// @param tCmd Command
/// @param tPath 통화 채널
/// @param tLobbyID LobbyID
/// @return void
void TransmitProtocolHouseSwitch( U8 tId, U8 tCmd, U8 tPath, U8 tLobbyID )
{
  switch( gLobbyHandle.Protocol )
  {
    case PSS_KD_PROTOCOL:
      KDP_TransmitProtocol(tId, tCmd, tPath, tLobbyID, 0, 0, 0);
      break;    
    case PSS_HT_PROTOCOL:
    case PSS_HT_PROTOCOL_2ND:     
      TransmitProtocolHouse(tId, tCmd, tPath, tLobbyID );
      break;
    case PSS_ANAM_PROTOCOL:
      Anam_TransmitProtocolHouse(tId, tCmd, tLobbyID );
      break;
    default:
      break;
  }
}


/// @brief 세대기 프로토콜 전송 함수
/// @param tId 세대기 ID
/// @param tCmd Command
/// @param tPath 통화 채널
/// @param tLobbyID LobbyID
/// @return void
void TransmitProtocolHouse(U8 tId, U8 tCmd, U8 tPath, U8 tLobbyID )
{
  U8 x;
  U8 bTxProtocol[PROTOCOL_SIZE_5BYTE];

  if ( oer_u2rb || fer_u2rb || per_u2rb || sum_u2rb ) InitializeUart2_3906();

  bTxProtocol[0] = tId;
  bTxProtocol[1] = tCmd;
  bTxProtocol[2] = tPath;
  bTxProtocol[3] = tLobbyID;
  bTxProtocol[4] = DEF_EOD;

  while(gUart2TimeOutCnt!=0) wdt_reset();

  __MultiprocessorModeEnable();         /* Multiprocessor Communication enable bit */
  __EnableTransmitUart2();              /* Transmit enable*/
  
  RS485_HOUSE_CNTR = SET;              

  delay_ms(10);                /* rs-485 enable stabilization time*/      
  
  for(x = 0; x < PROTOCOL_SIZE_5BYTE; x++) 
  {
    __ID_Field_TX = CLEAR;
    if(x == 0)   __ID_Field_TX = SET;

    while(!__TransmitEmptyFlgUart2);// __NOP();      //when from transfer register to TXD is completed: txept_u2c0 = HIGH    

    __TransmitBufferUart2 = bTxProtocol[x];        //u2tbl -> mptb_u2tb not clear 
    //delay_ms(1);
    while(!__IsEmptyTxBufferUart2);// __NOP();    //when data transfer to transmit register : ti_u2c1 = HIGH, txept_u2c0 = LOW
    __IsEmptyTxBufferUart2 = SET;  
  }
  delay_ms(10);              
  
  RS485_HOUSE_CNTR = CLEAR;

  __DisableTransmitUart2();              /* Transmit disable*/          
  __MultiprocessorModeEnable();                /* Multiprocessor Communication enable bit */
  
  gUart2TimeOutCnt = (U8)(UART_PROTOCOL_INTERVAL_20ms + (gLobbyHandle.LobbyID & 0x3F));

  if(gLobbyHandle.PacketMonitoring)
  {
    bPrintHex_Protocol( bTxProtocol, PROTOCOL_SIZE_5BYTE, TRANSMIT_LINE_HOUSE);
    ConsoleTimeOutReset();
  }

  
}


/// @brief 로비 프로토콜을 프로토콜 설정에 의하여 결정해 주는 함수
/// @param tLobbyID Lobby ID
/// @param tCmd Command
/// @param tPath 통화 채널
/// @param tDongNoH 동번호 상위 바이트
/// @param tDongNoL 동번호 하위 바이트
/// @param tHooNoH  호번호 상위 바이트
/// @param tHooNoL  호번호 하위 바이트
/// @return void
void TransmitProtocolLobbySwitch( U8 tLobbyID, U8 tCmd, U8 tPath, U8  tDongNoH, U8 tDongNoL, U8 tHooNoH, U8 tHooNoL )
{
  switch( gLobbyHandle.Protocol )
  {
    case PSS_KD_PROTOCOL:
      KDP_TransmitProtocol((tLobbyID&0x7F) , tCmd, tPath, tDongNoH, tDongNoL, tHooNoH, tHooNoL);
      break;
    case PSS_HT_PROTOCOL:
    case PSS_HT_PROTOCOL_2ND:
      //TransmitProtocolLobby(tLobbyID, tCmd, tPath, tDongNoH, tDongNoL, tHooNoH, tHooNoL );
      break;
    case PSS_ANAM_PROTOCOL:
      Anam_TransmitProtocolLobby(tLobbyID, tCmd, tPath, tDongNoH, tDongNoL, tHooNoH, tHooNoL );
      break;
    default:
      break;
  }
}


/// @brief 로비 프로토콜 전송 함수
/// @param tLobbyID Lobby ID
/// @param tCmd Command
/// @param tPath 통화 채널
/// @param tDongNoH 동번호 상위 바이트
/// @param tDongNoL 동번호 하위 바이트
/// @param tHooNoH  호번호 상위 바이트
/// @param tHooNoL  호번호 하위 바이트
/// @return void
void TransmitProtocolLobby(U8 tLobbyID, U8 tCmd, U8 tPath, U8  tDongNoH, U8 tDongNoL, U8 tHooNoH, U8 tHooNoL )
{
  U8 x;

  U8 bTxProtocol[PROTOCOL_SIZE_8BYTE];
 

  bTxProtocol[0] = tLobbyID;
  bTxProtocol[1] = tCmd;
  bTxProtocol[2] = tPath;
  bTxProtocol[3] = tDongNoH;
  bTxProtocol[4] = tDongNoL;
  bTxProtocol[5] = tHooNoH;
  bTxProtocol[6] = tHooNoL;
  bTxProtocol[7] = DEF_EOD;

  while(gUart2TimeOutCnt!=0) wdt_reset();

 __MultiprocessorModeEnable();              /* Multiprocessor Communication enable bit */
 __EnableTransmitUart2();              /* Transmit enable*/
 
 RS485_HOUSE_CNTR = SET;              

 delay_ms(10);                /* rs-485 enable stabilization time*/      
 
 for(x = 0; x < PROTOCOL_SIZE_8BYTE; x++) 
 {
   __ID_Field_TX = CLEAR;
   if(x == 0)   __ID_Field_TX = SET;

   while(!__TransmitEmptyFlgUart2);// __NOP();      //when from transfer register to TXD is completed: txept_u2c0 = HIGH    

   __TransmitBufferUart2 = bTxProtocol[x];        //u2tbl -> mptb_u2tb not clear 
   //delay_ms(1);
   while(!__IsEmptyTxBufferUart2);// __NOP();    //when data transfer to transmit register : ti_u2c1 = HIGH, txept_u2c0 = LOW
   __IsEmptyTxBufferUart2 = SET;  
 }
 delay_ms(10);              
 
 RS485_HOUSE_CNTR = CLEAR;                

 __DisableTransmitUart2();              /* Transmit disable*/          
 __MultiprocessorModeEnable();                /* Multiprocessor Communication enable bit */

 gUart2TimeOutCnt = (U8)(UART_PROTOCOL_INTERVAL_20ms + (gLobbyHandle.LobbyID & 0x3F));

 if(gLobbyHandle.PacketMonitoring)
 {
   bPrintHex_Protocol( bTxProtocol, PROTOCOL_SIZE_8BYTE, TRANSMIT_LINE_LOBBY);
   ConsoleTimeOutReset();
 }

  
}

/// @brief 로비 프로토콜을 프로토콜 설정에 의하여 결정해 주는 함수
/// @param tLobbyID Lobby ID
/// @param tCmd Command
/// @param tPath 통화 채널
/// @param tDongNoH 동번호 상위 바이트
/// @param tDongNoL 동번호 하위 바이트
/// @param tHooNoH  호번호 상위 바이트
/// @param tHooNoL  호번호 하위 바이트
/// @return void
void TransmitProtocolGuardSwitch( U8 tLobbyID, U8 tCmd, U8 tPath, U8  tDongNoH, U8 tDongNoL, U8 tHooNoH, U8 tHooNoL )
{
  switch( gLobbyHandle.Protocol )
  {
    case PSS_KD_PROTOCOL:
      KDP_TransmitProtocol(tLobbyID, tCmd, tPath, tDongNoH, tDongNoL, tHooNoH, tHooNoL);
      break;
    case PSS_HT_PROTOCOL:
    case PSS_HT_PROTOCOL_2ND:
      TransmitProtocolGuard(tLobbyID, tCmd, tPath, tDongNoH, tDongNoL, tHooNoH, tHooNoL );
      break;
    case PSS_ANAM_PROTOCOL:
      KDP_TransmitProtocol(tLobbyID, tCmd, tPath, tDongNoH, tDongNoL, tHooNoH, tHooNoL);
      //TransmitProtocolGuard(tLobbyID, tCmd, tPath, tDongNoH, tDongNoL, tHooNoH, tHooNoL );
      break;
    default:
      break;
  }
}

/// @brief 경비실 프로토콜 전송 함수
/// @param tGuardId Guard Id
/// @param tCmd Command
/// @param tPath 통화 채널
/// @param tDongNoH 동번호 상위 바이트
/// @param tDongNoL 동번호 하위 바이트
/// @param tHooNoH  호번호 상위 바이트
/// @param tHooNoL  호번호 하위 바이트
/// @return void
void TransmitProtocolGuard(U8 tGuardId, U8 tCmd, U8 tPath, U8  tDongNoH, U8 tDongNoL, U8 tHooNoH, U8 tHooNoL )
{
  U8 i;

  U16 bTxProtocol[PROTOCOL_SIZE_8BYTE];
  U8  bTxProtocolByte[PROTOCOL_SIZE_8BYTE];

  bTxProtocol[0] = tGuardId | 0x0100;
  bTxProtocol[1] = tCmd;
  bTxProtocol[2] = tPath;
  bTxProtocol[3] = tDongNoH;
  bTxProtocol[4] = tDongNoL;
  bTxProtocol[5] = tHooNoH;
  bTxProtocol[6] = tHooNoL;
  bTxProtocol[7] = DEF_EOD;

  while(gUart1TimeOutCnt!=0) wdt_reset();

  __EnableTransmitUart1();              /* Transmit enable*/
  
  RS485_GUARD_CNTR = SET;              

  delay_ms(10);                /* rs-485 enable stabilization time*/      
  
  for(i = 0; i < PROTOCOL_SIZE_8BYTE; i++) 
  {
    while(!__TransmitEmptyFlgUart1);// __NOP();      //when from transfer register to TXD is completed: txept_u2c0 = HIGH    
    __TransmitBufferUart1 = bTxProtocol[i];        //u2tbl -> mptb_u2tb not clear 
    while(!__IsEmptyTxBufferUart1);// __NOP();    //when data transfer to transmit register : ti_u2c1 = HIGH, txept_u2c0 = LOW
    __IsEmptyTxBufferUart1 = SET;  
  }
  
  delay_ms(10);              
  
  RS485_GUARD_CNTR = CLEAR;                
  __DisableTransmitUart1();              /* Transmit disable*/   

  gUart1TimeOutCnt = (U8)(UART_PROTOCOL_INTERVAL_20ms + (gLobbyHandle.LobbyID & 0x3F));

  if(gLobbyHandle.PacketMonitoring){
    for(i = 0; i < PROTOCOL_SIZE_8BYTE; i++) 
    {
      bTxProtocolByte[i] = (U8)(bTxProtocol[i] & 0x00ff);
    }
    bPrintHex_Protocol( bTxProtocolByte, PROTOCOL_SIZE_8BYTE,TRANSMIT_LINE_GUARD);
    ConsoleTimeOutReset();
  }

  
}



/// @brief 수신된 프로토콜을 ThreadAnalysisProtocol 함수로 메시지 전송
/// @param *tRXData 수신된 프로토콜 데이터 포인터
/// @return void
void SendMsgReceiveProtocol ( U8 *tRXData )
{
  typeMsgBox tMsgBuffer;
  U8 i;
  
  tMsgBuffer.DstID = MSG_ID_ThreadAnalysisProtocol;
  tMsgBuffer.SrcID = MSG_ID_MsgReceiveProtocol;
  tMsgBuffer.Cmd  = MSG_CMD_RECEIVE_PROTOCOL;
  for ( i = 0; i < PROTOCOL_SIZE_8BYTE+1; i++ )
  {
    tMsgBuffer.Buf.Byte[i] = tRXData[i];
  }
  bSendMessageBox( &tMsgBuffer );
}


/// @brief 수신된 프로토콜을 ScanAllHouses 함수로 메시지 전송 \n Scan House 기능에서 사용됨
/// @param *tRXData 수신된 프로토콜 데이터 포인터
/// @return void
void SendMsgReceiveProtocol_Echo ( U8 *tRXData )
{
  typeMsgBox tMsgBuffer;
  U8 i;
  
  tMsgBuffer.DstID = MSG_ID_ScanAllHouses;
  tMsgBuffer.SrcID = MSG_ID_MsgReceiveProtocol;
  tMsgBuffer.Cmd  = MSG_CMD_RECEIVE_PROTOCOL;
  for ( i = 0; i < PROTOCOL_SIZE_8BYTE+1; i++ )
  {
    tMsgBuffer.Buf.Byte[i] = tRXData[i];
  }
  bSendMessageBox( &tMsgBuffer );
}


