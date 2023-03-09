
/// @file   interrupt_vector.c
/// @date   2012/02/14
/// @author jinhwa Lee
/// @brief  인터럽트 서비스 루틴

#include "main.h"

U8 RxReceiveData[20];

extern U8 gUart1TimeOutCnt;
extern U8 gUart2TimeOutCnt;


/// @brief  콘솔 Transmit Interrupt 함수
/// @param  void
/// @return void
#pragma interrupt  _uart0_transmit(vect=17)
void _uart0_transmit(void)
{
  U8 Txdata;
  if( bGetTXConsoleData(&Txdata) ) __TransmitBufferUart0 = Txdata;
}


/// @brief  콘솔 Receive Interrupt 함수
/// @param  void
/// @return void
#pragma interrupt  _uart0_receive(vect=18)
void _uart0_receive(void)
{
  typeMsgBox tMsgBox;
  U8 Rxdata;
  U8  i;
  static  U8 DataCount=0;
  
  Rxdata = (U8)__ReceiveBufferUart0;

  if ( IsFactoryMode2() )
  {
    UartRx_factory( Rxdata );
  }

  if(( Rxdata >= '0' && Rxdata <= '9') || (Rxdata >= 'a' && Rxdata <= 'z') 
    || (Rxdata >= 'A' && Rxdata <= 'Z') || (Rxdata == ASCII_ENTER) )
  {
    RxReceiveData[DataCount] = Rxdata;

    ConsoleTimeOutReset();

    //bPutTXConsoleData(&Rxdata, 1);
    //bStartTXConsol();

    if(RxReceiveData[DataCount] == 0x0d)
    {  
      tMsgBox.DstID = MSG_ID_ThreadConsole;
      tMsgBox.SrcID = 0;
      tMsgBox.Cmd   = DataCount;
      for(i = 0; i < DataCount + 1; i++)
      {
        tMsgBox.Buf.Byte[i] = RxReceiveData[i];
      }
      bSendMessageBox( &tMsgBox );
      DataCount = CLEAR;
    }
    else
    {
      DataCount++;
      if ( DataCount >= MSG_BUFFER_LENGTH ) DataCount = (U8)(MSG_BUFFER_LENGTH - 1);
    }
  }
}


/// @brief  경비실 Receive Interrupt 함수
/// @param  void
/// @return void
#pragma interrupt  _uart1_receive(vect=20)
void _uart1_receive(void)
{
  U16  rx_udr;                         // Index, 수신문자 보관
  static U8 Uart1Count;
  static U8 Uart1DataRX[PROTOCOL_SIZE_8BYTE+1];

  rx_udr = __ReceiveBufferUart1;

  if ( oer_u1rb || fer_u1rb || per_u1rb || sum_u1rb )
  {
    ProtocolSpecSetting();
    return;
  }
  
  if ( (gLobbyHandle.Protocol == PSS_KD_PROTOCOL) || (gLobbyHandle.Protocol == PSS_ANAM_PROTOCOL))
  {
    Kd_ReceiveData( PROTOCOL_GUARD_CH0, (U8)rx_udr );
    return;
  }

  if ( rx_udr & 0x0100 )
  {

    Uart1Count = 0;
    Uart1DataRX[Uart1Count] = rx_udr;
    gUart1TimeOutCnt = UART_BYTE_INTERVAL_10ms;
  }
  else                       //mprb_u2rb CLEAR 이므로 DATA field를 뜻하는 것임
  {
    if(gUart1TimeOutCnt == 0) // Byte 간 Interval 10ms
    {
      Uart1Count = 0;
      return;
    }
  
    Uart1Count++;
    if(Uart1Count < (PROTOCOL_SIZE_8BYTE - 1))
    {
      Uart1DataRX[Uart1Count] = rx_udr;
      gUart1TimeOutCnt = UART_BYTE_INTERVAL_10ms; // Byte 간 Interval 10ms
    }
    else 
    {
      if((Uart1Count == (PROTOCOL_SIZE_8BYTE - 1)) && (rx_udr == DEF_EOD))
      {

        if ( IsFactoryMode2() )
        {
          UartRx_factory( Uart1DataRX[2] );
        }

        Uart1DataRX[Uart1Count] = rx_udr;
        gUart1TimeOutCnt = (U8)(UART_PROTOCOL_INTERVAL_20ms + (gLobbyHandle.LobbyID & 0x3F)); // Byte 간 Interval 10ms

        if(Uart1DataRX[0] & 0x80){
          Uart1DataRX[0] &= 0x7F;
        }

        Uart1DataRX[PROTOCOL_SIZE_8BYTE] = RECEIVE_LINE_GUARD;
        SendMsgReceiveProtocol ( Uart1DataRX );
        if(gLobbyHandle.PacketMonitoring) bPrintHex_Protocol( Uart1DataRX, PROTOCOL_SIZE_8BYTE, RECEIVE_LINE_GUARD );
        
      }
      else
      {
        Uart1Count = 0;
      }
    }
  }

}


/// @brief  세대&로비 Receive Interrupt 함수
/// @param  void
/// @return void
#pragma interrupt  _uart2_receive(vect=12)
void _uart2_receive(void)
{
  static U8 Uart2Count;
  static U8 Uart2DataRX[PROTOCOL_SIZE_8BYTE+1];

  U8 rx_udr, x;
  
  rx_udr = __ReceiveBufferUart2;

  if ( oer_u2rb || fer_u2rb || per_u2rb || sum_u2rb )
  {
    ProtocolSpecSetting();
    return;
  }

  if ( gLobbyHandle.Protocol == PSS_KD_PROTOCOL )
  {
    Kd_ReceiveData( PROTOCOL_HOME_CH1, rx_udr );
    Kd_ReceiveData( PROTOCOL_LOBBY_CH1, rx_udr );
    return;
  }

  if( __ID_Field_RX == SET )                 //mprb_u2rb SET 이므로 ID field를 뜻하는 것임
  {
    __MultiprocessorModeDisable();                // Multiprocessor Communication control bit 
    Uart2Count = INDEX_ZERO;
    Uart2DataRX[Uart2Count] = rx_udr;
    gUart2TimeOutCnt = UART_BYTE_INTERVAL_10ms;
  }
  else                       //mprb_u2rb CLEAR 이므로 DATA field를 뜻하는 것임
  {    
    Uart2Count++;
    Uart2DataRX[Uart2Count] = rx_udr;
    if(gUart2TimeOutCnt == 0) // Byte 간 Interval 10ms
    {
      Uart2Count = INDEX_ZERO;
      __MultiprocessorModeEnable();
      return;
    }
    
    gUart2TimeOutCnt = UART_BYTE_INTERVAL_10ms;

    if((Uart2Count == (PROTOCOL_SIZE_5BYTE-1)) && (rx_udr == DEF_EOD))
    {

      gUart2TimeOutCnt = (U8)(UART_PROTOCOL_INTERVAL_20ms + (gLobbyHandle.LobbyID & 0x3F)); 
      
      Uart2DataRX[PROTOCOL_SIZE_8BYTE] = RECEIVE_LINE_HOUSE;

      if ( IsFactoryMode2() )
      {
        UartRx_factory( Uart2DataRX[2] );
      }
      
      if ( Uart2DataRX[1] == LR_ECHO )
      {
        if(Uart2DataRX[0] & 0x80) SendMsgReceiveProtocol_Echo ( Uart2DataRX );
        __MultiprocessorModeEnable();
        return;
      }
      
      if(Uart2DataRX[0] & 0x80)
      {
        if(gLobbyHandle.PacketMonitoring) bPrintHex_Protocol( Uart2DataRX, PROTOCOL_SIZE_5BYTE,RECEIVE_LINE_HOUSE );
        Uart2DataRX[0] &= 0x7F;
        SendMsgReceiveProtocol ( Uart2DataRX );
      }
      __MultiprocessorModeEnable();
      return;
    }


    if((Uart2Count == (PROTOCOL_SIZE_8BYTE-1)) && (rx_udr == DEF_EOD))
    {
      gUart2TimeOutCnt = (U8)(UART_PROTOCOL_INTERVAL_20ms + (gLobbyHandle.LobbyID & 0x3F)); 

      Uart2DataRX[PROTOCOL_SIZE_8BYTE] = RECEIVE_LINE_LOBBY;

      if ( IsFactoryMode2() )
      {
        UartRx_factory( Uart2DataRX[2] );
      }

    
      if(gLobbyHandle.LobbyID == MASTER_LOBBY_ID)
      {
        if(Uart2DataRX[0] & 0x80)
        {
          if(gLobbyHandle.PacketMonitoring) bPrintHex_Protocol( Uart2DataRX, PROTOCOL_SIZE_8BYTE, RECEIVE_LINE_LOBBY );
          Uart2DataRX[0] &= 0x7F;
          SendMsgReceiveProtocol ( Uart2DataRX );
        }
      }
      else
      {
        if((Uart2DataRX[0] & 0x80) == NULL) SendMsgReceiveProtocol ( Uart2DataRX );
      }
      __MultiprocessorModeEnable();
      return;
    }

    if( Uart2Count > (PROTOCOL_SIZE_8BYTE-1))
    {
      Uart2Count = 0;
      __MultiprocessorModeEnable();
      return;
    }
  }
}



/// @brief  RFID Int Interrupt 
/// @param  void
/// @return void
#pragma interrupt  _int3(vect=26)  // RF_INT
void _int3(void)
{ 
  U8 tReadDataRFCard[5];

  rd_RFCard( tReadDataRFCard );

  if ( IsFactoryMode2() )
  {
    RFCARD_factory(tReadDataRFCard[0]);
  }
  else
  {
    SendMsgRFCard( true , tReadDataRFCard );
  }

  /* Clear interrupt flag */
  ir_int3ic = 0;
  
}


/// @brief  1ms 타이머 인터럽트 \n gsTimerCnt 카운터 \n 타임아웃 카운터 \n FND 디스플레이
/// @param  void
/// @return void
#pragma interrupt  _timer_ra(vect=22)
void _timer_ra(void)  // 100us Timer
{
  static U8 mCount10;
  static U8 mProtocolCnt = 0;

  switch ( gLobbyHandle.Protocol )
  {
    case PSS_KD_PROTOCOL:;
      if( mProtocolCnt == 3 )
      {
        Kd_TimerInterrupt300us();
        mProtocolCnt = 0;
      }
      mProtocolCnt++;
      break;
    case PSS_ANAM_PROTOCOL:;
      Anam_TimerInterrutp100us();
      
      if( mProtocolCnt == 3 )
      {
        Kd_TimerInterrupt300us();
        mProtocolCnt = 0;
      }
      mProtocolCnt++;      
      break;
    default:
      break;
  }

  if ( mCount10 == 10 )
  {
    InterruptDisplayFnd();
    gsTimerCnt++;
    if(gUart1TimeOutCnt != 0) gUart1TimeOutCnt--;
    if(gUart2TimeOutCnt != 0) gUart2TimeOutCnt--;
    mCount10 = 0;
  }
  mCount10++;
}


/// @brief  15ms 타이머 인터럽트 \n Sound 출력을 위한 타이머 인터럽트
/// @param  void
/// @return void
#pragma interrupt  _timer_rb(vect=24)
void _timer_rb(void)  
{
  InterruptSoundMode();
}

#pragma interrupt  _timer_rc(vect=7)
void _timer_rc(void)
{
  trcsr &= 0x8e;                  /* IMFA flag clear */
}

#pragma interrupt  _int1(vect=25) // nFIRE
void _int1(void)
{
  /* Clear interrupt flag */
  ir_int1ic = 0;
}

#pragma interrupt  _uart1_transmit(vect=19)
void _uart1_transmit(void)
{
//  __TransmitBufferUart1 = buffer;
}

#pragma interrupt  _uart2_transmit(vect=11)
void _uart2_transmit(void)
{
}



#pragma interrupt  _timer_rd0(vect=8)
void _timer_rd0(void)
{
}

#pragma interrupt  _timer_rd1(vect=9)
void _timer_rd1(void)
{
}

#pragma interrupt  _timer_re(vect=10)
void _timer_re(void)
{
}

#pragma interrupt  _timer_rf(vect=27)
void _timer_rf(void)
{
}

#pragma interrupt  _timer_rg(vect=43)
void _timer_rg(void)
{
}




