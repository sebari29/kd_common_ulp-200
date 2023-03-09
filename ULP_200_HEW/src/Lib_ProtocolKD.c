/// @file   Lib_Kd
/// @date   2012/02/14
/// @author jinhwa Lee
/// @brief 
#include "main.h"



#define KD_UART_BAUDRATE            9600
#define KD_TIMER_INTERRUP_PERIOD    300    // 단위: usec // ((( 1 / USART_BAUDRATE_KDP ) * 10 ) / 4)

#define KD_BYTE_INTERVAL_MAX        10 // ( TIMER_INTERRUPT_PERIOD_KDP * X) > 3ms
#define KD_BYTE_INTERVAL_MIN        4  // ( TIMER_INTERRUPT_PERIOD_KDP * X) > 1.2ms
//#define MAX_ACK_TIMEOUT_KDP        50 // ( TIMER_INTERRUPT_PERIOD_KDP * X) > 15ms
//#define MAX_ACK_DELAY_TIME_KDP     34 // ( TIMER_INTERRUPT_PERIOD_KDP * X) > 10ms

#define KD_HAEDER_FIELD_SIZE        5
#define KD_CHECKSUM_FIELD_SIZE      2

#define KD_HAEDER_PLUS_DATA_FILD_SIZE     ( KD_HAEDER_FIELD_SIZE + KD_DATA_FIELD_SIZE )
#define KD_HAEDER_PLUS_CHECKSUM_FILD_SIZE ( KD_HAEDER_FIELD_SIZE + KD_CHECKSUM_FIELD_SIZE )

#define KD_PROTOCOL_DEFAULT_LENGTH  12

#define KD_PACKET_STX               0xF7

#define KD_RX_STATUS_STANDBY        0
#define KD_RX_STATUS_HEADER         1
#define KD_RX_STATUS_DATA           2
#define KD_RX_STATUS_CHECKSUM       3
#define KD_RX_STATUS_END            4

#define KD_TX_STATUS_STANDBY        0
#define KD_TX_STATUS_485EN          1
#define KD_TX_STATUS_DATA           2
#define KD_TX_STATUS_485DIS         3
#define KD_TX_STATUS_END            4

#define KD_LINE_STATUS_FREE         0
#define KD_LINE_STATUS_BUSY         1

#define KD_DEFUALT_TIME_100MS       100


typedef struct
{
  U8 Stx;
  U8 Device;
  U8 SubID;
  U8 Command;
  U8 Length;
  U8 *Data;
  U8 CheckSum;
  U8 AddSum;
}__typeKdProtocolFormat;


typedef struct
{
  U8 Status;
  U8 Count;
  U8 TempCheckSum;
  U8 TempAddSum;
  U8 AllLength;
  U8 *BytePointer;
  __typeKdProtocolFormat Protocol;
}__typeKdRxProtocolStatus;

typedef struct
{
  U8 Status;
  U8 RS485Enable;
  //U8 AckTimeOut;
  U8 WaitTime;
  void (*GPrt_485TxEnabletion)(void);
  void (*Rx485EnableFunction)(void);
  void (*TxBufferRegister)(U8);
  U8 Count;
  U8 AllLength;
  U8 *Buffer;
}__typeKdTxProtocolStatus;

typedef struct
{
  U8  Use;
  U16 Baudrate;
  U8  ThisDevice;
  U8  ThisSubID;
  U8  StatusLine;
  U8  TimeOut;
  __typeKdRxProtocolStatus Rx;
  __typeKdTxProtocolStatus Tx;
  
}__typeKdProtocolManager;

void __TransmitAckKDP( U8 tChannel, __typeKdProtocolFormat *tRxProtocol );
U8 Kd_ReceiveSuccess(U8 tChannel, __typeKdProtocolFormat *tRxProtocol);
U8 Kd_IDError(U8 tChannel, __typeKdProtocolFormat *tRxProtocol);
U8 Kd_CheckSumError(U8 tChannel, __typeKdProtocolFormat *tRxProtocol);
U8 Kd_BufferFullError(U8 tChannel, __typeKdProtocolFormat *tRxProtocol );

U8 KDP_ReceiveProtocol( typeKdProtocol *tRxKdProtocol );

////////////////////////////////////////////////////////////////////////////////


__typeKdProtocolManager mKdChannel[COUNT_CHANNEL_KDP];
U8 mRxDataKDP[COUNT_CHANNEL_KDP][KD_DATA_FIELD_SIZE];
U8 mTxBufferKDP[COUNT_CHANNEL_KDP][KD_DATA_FIELD_SIZE + KD_HAEDER_FIELD_SIZE + KD_CHECKSUM_FIELD_SIZE];


void Kd_Initialize(void)
{
  U8 i;
  for(i = 0 ; i < COUNT_CHANNEL_KDP ; i++ )
  {
    mKdChannel[i].Rx.Protocol.Data = &mRxDataKDP[i][0];
    mKdChannel[i].Tx.Buffer = &mTxBufferKDP[i][0];
  }
}

void Kd_SetupChannel( U8 tChannel, U16 tBaudrate,
                      U8 tThisDevice, U8 tThisSubID, 
                      void (*tTx485Enable)(void), void (*tRx485Enable)(void),
                      void (*tTxBufferReg)(U8))
{
  mKdChannel[tChannel].Use          = true;
  mKdChannel[tChannel].Baudrate     = tBaudrate;
  
  mKdChannel[tChannel].ThisDevice   = tThisDevice;
  mKdChannel[tChannel].ThisSubID    = tThisSubID;
  mKdChannel[tChannel].StatusLine   = KD_LINE_STATUS_FREE;
  mKdChannel[tChannel].TimeOut      = ZERO;

  mKdChannel[tChannel].Rx.Status     = KD_RX_STATUS_STANDBY;

  mKdChannel[tChannel].Tx.Status     = KD_TX_STATUS_STANDBY;
  mKdChannel[tChannel].Tx.GPrt_485TxEnabletion = tTx485Enable;
  mKdChannel[tChannel].Tx.Rx485EnableFunction = tRx485Enable;
  mKdChannel[tChannel].Tx.TxBufferRegister = tTxBufferReg;
  
  mKdChannel[tChannel].Tx.Rx485EnableFunction();
  
  
}


void Kd_ReceiveData( U8 tChannel, U8 tRxData )
{

  if ( mKdChannel[tChannel].TimeOut > KD_BYTE_INTERVAL_MAX )
  {
    mKdChannel[tChannel].StatusLine = KD_LINE_STATUS_FREE;
    mKdChannel[tChannel].Rx.Status  = KD_RX_STATUS_STANDBY;
  }

  switch( mKdChannel[tChannel].Rx.Status )
  {
    case KD_RX_STATUS_STANDBY:
      if ( tRxData == KD_PACKET_STX )
      {
        //mKdChannel[tChannel].Tx.AckTimeOut = mKdChannel[tChannel].TimeOut;
        mKdChannel[tChannel].StatusLine = KD_LINE_STATUS_BUSY;
        mKdChannel[tChannel].Rx.Status  = KD_RX_STATUS_HEADER;

        mKdChannel[tChannel].Rx.BytePointer = &mKdChannel[tChannel].Rx.Protocol.Stx;
        *mKdChannel[tChannel].Rx.BytePointer = tRxData;
        *mKdChannel[tChannel].Rx.BytePointer++;
        mKdChannel[tChannel].Rx.Count = (ZERO + 1);

        mKdChannel[tChannel].Rx.TempCheckSum = tRxData;
        mKdChannel[tChannel].Rx.TempAddSum   = tRxData;
        
      }      
      break;
    case KD_RX_STATUS_HEADER:
      
      *mKdChannel[tChannel].Rx.BytePointer = tRxData;
      *mKdChannel[tChannel].Rx.BytePointer++;
      mKdChannel[tChannel].Rx.Count++;

      mKdChannel[tChannel].Rx.TempCheckSum ^= tRxData;
      mKdChannel[tChannel].Rx.TempAddSum += tRxData;

      if ( mKdChannel[tChannel].Rx.Count == KD_HAEDER_FIELD_SIZE )
      {
        if( mKdChannel[tChannel].ThisDevice != mKdChannel[tChannel].Rx.Protocol.Device )
        {
          mKdChannel[tChannel].StatusLine = KD_LINE_STATUS_FREE;
          mKdChannel[tChannel].Rx.Status  = KD_RX_STATUS_STANDBY;
          break;
        }
        mKdChannel[tChannel].Rx.Status = KD_RX_STATUS_DATA;
        mKdChannel[tChannel].Rx.BytePointer = &mRxDataKDP[tChannel][0];
        mKdChannel[tChannel].Rx.AllLength =   KD_HAEDER_FIELD_SIZE 
                                            + mKdChannel[tChannel].Rx.Protocol.Length
                                            + KD_CHECKSUM_FIELD_SIZE ;
      }
      break;
    case KD_RX_STATUS_DATA:
      if ( mKdChannel[tChannel].Rx.Count == KD_HAEDER_PLUS_DATA_FILD_SIZE ) // 메모리 풀 체크
      {
        mKdChannel[tChannel].Rx.Status = KD_RX_STATUS_STANDBY;
        mKdChannel[tChannel].StatusLine = KD_LINE_STATUS_FREE;
        Kd_BufferFullError( tChannel, &mKdChannel[tChannel].Rx.Protocol );
        break;
      }
        
      *mKdChannel[tChannel].Rx.BytePointer = tRxData;
      *mKdChannel[tChannel].Rx.BytePointer++;
      mKdChannel[tChannel].Rx.Count++;
      
      mKdChannel[tChannel].Rx.TempCheckSum ^= tRxData;
      mKdChannel[tChannel].Rx.TempAddSum += tRxData;

      if ( mKdChannel[tChannel].Rx.Count == ( KD_HAEDER_FIELD_SIZE + mKdChannel[tChannel].Rx.Protocol.Length) )
      {
        mKdChannel[tChannel].Rx.Status = KD_RX_STATUS_CHECKSUM;
        mKdChannel[tChannel].Rx.BytePointer =  &mKdChannel[tChannel].Rx.Protocol.CheckSum;
      }
      break;
    case KD_RX_STATUS_CHECKSUM:
      *mKdChannel[tChannel].Rx.BytePointer = tRxData;
      *mKdChannel[tChannel].Rx.BytePointer++;
      
      mKdChannel[tChannel].Rx.Count++;

      if ( mKdChannel[tChannel].Rx.Count < mKdChannel[tChannel].Rx.AllLength )  break;

      mKdChannel[tChannel].Rx.TempAddSum += mKdChannel[tChannel].Rx.Protocol.CheckSum;

      mKdChannel[tChannel].StatusLine = KD_LINE_STATUS_FREE;
      mKdChannel[tChannel].Rx.Status  = KD_RX_STATUS_STANDBY;

      if (( mKdChannel[tChannel].Rx.TempCheckSum == mKdChannel[tChannel].Rx.Protocol.CheckSum )
             && ( mKdChannel[tChannel].Rx.TempAddSum == mKdChannel[tChannel].Rx.Protocol.AddSum ))
      {
        if( mKdChannel[tChannel].ThisSubID == mKdChannel[tChannel].Rx.Protocol.SubID )
        {
          if( mKdChannel[tChannel].ThisDevice == mKdChannel[tChannel].Rx.Protocol.Device )
          {
            Kd_ReceiveSuccess(tChannel, &mKdChannel[tChannel].Rx.Protocol);
            break;
          }
          else if ( mKdChannel[tChannel].Rx.Protocol.Device == KD_PRT_DEVICE_LOBBY )
          {
            Kd_ReceiveSuccess(tChannel, &mKdChannel[tChannel].Rx.Protocol);
            break;
          }
        }
        Kd_IDError(tChannel, &mKdChannel[tChannel].Rx.Protocol);
      }
      else
      {
        Kd_CheckSumError(tChannel, &mKdChannel[tChannel].Rx.Protocol);
      }
      break;
    default:
      break;
  }
  
  mKdChannel[tChannel].TimeOut = ZERO;
 
}

///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

/*
void __TransmitAckKDP( U8 tChannel, typeStandardFormatKDP *tRxProtocol )
{
  U8 tCommand;
  tCommand = (tRxProtocol->Command | 0x80 );
  TransmitDataKDP( tChannel, tCommand, tRxProtocol->Data , tRxProtocol->Length, MAX_ACK_DELAY_TIME_KDP );
}
*/


U8 Kd_TransmitData( U8 tChannel, U8 tCommand, U8 *tBuffer, U8 tLength, U8 tWaitTime100us )
{
  return Kd_TransmitDataRawData( tChannel,
                           mKdChannel[tChannel].ThisDevice, mKdChannel[tChannel].ThisSubID,
                           tCommand, tBuffer, tLength, tWaitTime100us );
}


U8 Kd_TransmitDataRawData(U8 tChannel,U8 tDevice, U8 tSubID, U8 tCommand, U8 *tBuffer, U8 tLength, U8 tWaitTime100us )
{
  U8 i;
  U8 tExcSum = 0;
  U8 tAddSum = 0;

  if ( mKdChannel[tChannel].StatusLine == KD_LINE_STATUS_BUSY) return false;
  
  mKdChannel[tChannel].Tx.Buffer[0] = KD_PACKET_STX;
  mKdChannel[tChannel].Tx.Buffer[1] = tDevice;
  mKdChannel[tChannel].Tx.Buffer[2] = tSubID;
  mKdChannel[tChannel].Tx.Buffer[3] = tCommand;
  mKdChannel[tChannel].Tx.Buffer[4] = tLength;

  for ( i = 0 ; i < KD_HAEDER_FIELD_SIZE ; i++ )
  {
    tExcSum ^= mKdChannel[tChannel].Tx.Buffer[i];
    tAddSum += mKdChannel[tChannel].Tx.Buffer[i];
  }
  
  for ( i = KD_HAEDER_FIELD_SIZE ; i < (KD_HAEDER_FIELD_SIZE + tLength) ; i++ )
  {
    mKdChannel[tChannel].Tx.Buffer[i] = tBuffer[ i - KD_HAEDER_FIELD_SIZE ];
    tExcSum ^= mKdChannel[tChannel].Tx.Buffer[i];
    tAddSum += mKdChannel[tChannel].Tx.Buffer[i];
  }

  tAddSum += tExcSum;

  mKdChannel[tChannel].Tx.Buffer[ KD_HAEDER_FIELD_SIZE + tLength ]    = tExcSum;
  mKdChannel[tChannel].Tx.Buffer[ KD_HAEDER_FIELD_SIZE + tLength + 1] = tAddSum;

  mKdChannel[tChannel].Tx.AllLength = KD_HAEDER_PLUS_CHECKSUM_FILD_SIZE + tLength;

  mKdChannel[tChannel].Tx.Status = KD_TX_STATUS_485EN;

  if ( tWaitTime100us != ZERO )
  {
    mKdChannel[tChannel].Tx.WaitTime = 
      (U8)(((U16)tWaitTime100us * (U16)KD_DEFUALT_TIME_100MS)/((U16)KD_TIMER_INTERRUP_PERIOD) + 1);
  }
  else
  {
    tWaitTime100us = 0;
  }

  mKdChannel[tChannel].StatusLine = KD_LINE_STATUS_BUSY;

  mKdChannel[tChannel].TimeOut = 0;

  return true;

}


void __Kd_Transmit485( U8 tChannel )
{
  U8 tTxCount;
  switch ( mKdChannel[tChannel].Tx.Status )
  {
    case KD_TX_STATUS_STANDBY:
      break;
    case KD_TX_STATUS_485EN:
      if ( mKdChannel[tChannel].Tx.WaitTime != ZERO )
      {
        mKdChannel[tChannel].Tx.WaitTime--;
        mKdChannel[tChannel].TimeOut = ZERO;
        break;
      }
      mKdChannel[tChannel].Tx.RS485Enable = true;
      mKdChannel[tChannel].Tx.GPrt_485TxEnabletion();
      mKdChannel[tChannel].Tx.Count = ZERO;
      mKdChannel[tChannel].Tx.Status  = KD_TX_STATUS_DATA;
      mKdChannel[tChannel].TimeOut = KD_BYTE_INTERVAL_MIN;
      return;
    case KD_TX_STATUS_DATA:
      if ( mKdChannel[tChannel].TimeOut < KD_BYTE_INTERVAL_MIN) return;
      
      tTxCount = mKdChannel[tChannel].Tx.Count;
      mKdChannel[tChannel].Tx.TxBufferRegister(mKdChannel[tChannel].Tx.Buffer[tTxCount]);
      mKdChannel[tChannel].Tx.Count++;
      
      if ( mKdChannel[tChannel].Tx.Count == mKdChannel[tChannel].Tx.AllLength)
      {
        mKdChannel[tChannel].Tx.Status  = KD_TX_STATUS_485DIS;
      }
      mKdChannel[tChannel].TimeOut = ZERO;
      break;
    case KD_TX_STATUS_485DIS:
      if ( mKdChannel[tChannel].TimeOut < KD_BYTE_INTERVAL_MIN) return;
        mKdChannel[tChannel].Tx.RS485Enable = false;
        mKdChannel[tChannel].Tx.Rx485EnableFunction();
        mKdChannel[tChannel].Tx.Status  = KD_TX_STATUS_STANDBY;
        mKdChannel[tChannel].StatusLine = KD_LINE_STATUS_FREE;
        mKdChannel[tChannel].TimeOut = ZERO;
        break;
    default:
      break;
  }

  

}


void Kd_TimerInterrupt300us( void )
{
  U8 tChannel;
  for ( tChannel = 0 ; tChannel < (COUNT_CHANNEL_KDP) ; tChannel++ ) // 57600 오로 인해 3채널 삭제
  {
    if ( mKdChannel[tChannel].TimeOut != 0xFF ) 
      mKdChannel[tChannel].TimeOut++;
    if ( mKdChannel[tChannel].Tx.Status != KD_TX_STATUS_STANDBY )
      __Kd_Transmit485(tChannel);
  }
}


///////////////////////////// User Code ///////////////////////////////////////

// KD Protocol //
///////////////////////////////////////////////////////////////////////////////////////////
//        HEADER = 5             /           ID = 10           /    INFO = 2  / DATA ... //
//---------------------------------------------------------------------------------------//
// STX / DEV / SubID / CMD / Len / TXDEV / RXDEV / TXID / RXID / CHAN / VEDIO / DATA ... //
//---------------------------------------------------------------------------------------//
//  1  /  1  /   1   /  1  /  1  /   1   /   1   /  4   /   4  /   1  /   1   / LEN  ... //
///////////////////////////////////////////////////////////////////////////////////////////



U8 Kd_ProtocolFormatConverter( U8 *tTypeKdProtocol, U8 *tTypeKdProtocolFormat )
{
  U8 i;
  U8 tData;

  __typeKdProtocolFormat *KdProtocolFormat;
  typeKdProtocol *tKDProtocol;

  KdProtocolFormat = (__typeKdProtocolFormat*)tTypeKdProtocolFormat;
  tKDProtocol = (typeKdProtocol*)tTypeKdProtocol;

//  memset( tKDProtocol, ZERO, sizeof(typeKdProtocol));

  tKDProtocol->Header.Device   = KdProtocolFormat->Device;
  tKDProtocol->Header.SubID    = KdProtocolFormat->SubID;
  tKDProtocol->Header.Command  = KdProtocolFormat->Command;
  tKDProtocol->Header.Length   = KdProtocolFormat->Length;

  tKDProtocol->ID.TxDevice     = KdProtocolFormat->Data[0];
  tKDProtocol->ID.RxDevice     = KdProtocolFormat->Data[1];
  
  tKDProtocol->ID.TxDongIDHi   = KdProtocolFormat->Data[2];
  tKDProtocol->ID.TxDongIDLo   = KdProtocolFormat->Data[3];
  tKDProtocol->ID.TxHooIDHi    = KdProtocolFormat->Data[4];
  tKDProtocol->ID.TxHooIDLo    = KdProtocolFormat->Data[5];
  
  tKDProtocol->ID.RxDongIDHi   = KdProtocolFormat->Data[6];
  tKDProtocol->ID.RxDongIDLo   = KdProtocolFormat->Data[7];
  tKDProtocol->ID.RxHooIDHi    = KdProtocolFormat->Data[8];
  tKDProtocol->ID.RxHooIDLo    = KdProtocolFormat->Data[9];
  
  tKDProtocol->Channel.SlotID_Channel = KdProtocolFormat->Data[10];
  tKDProtocol->Channel.Video   = KdProtocolFormat->Data[11];

  for ( i = 0 ; i < ( tKDProtocol->Header.Length - 12 ) ; i++ )
  {
    tKDProtocol->Data[i] = KdProtocolFormat->Data[ i + 12 ];
  }

  //GPrt_SendMsgKdProtocol( MSG_ID_GPrt_ThreadAnalysisProtocol, (U8*)tKDProtocol);

  return true;

}

 
U8 Kd_ReceiveSuccess(U8 tChannel, __typeKdProtocolFormat *tRxProtocol)
{
  typeKdProtocol tRxKdProtocol;
  Kd_ProtocolFormatConverter( (U8*)&tRxKdProtocol ,(U8*)tRxProtocol );
  KDP_ReceiveProtocol( &tRxKdProtocol );
  return false;
}

U8 Kd_IDError(U8 tChannel, __typeKdProtocolFormat *tRxProtocol)
{
  return false;
}

U8 Kd_CheckSumError(U8 tChannel, __typeKdProtocolFormat *tRxProtocol)
{
  return false;
}

U8 Kd_BufferFullError(U8 tChannel, __typeKdProtocolFormat *tRxProtocol )
{
  return false;
}

/////////////////////////////////////////////////////////////////////////////////


U16 KDP_HdIdToKdID( U8 tHdID )
{
  U8 tHooNoH,tHooNoL;
  U16 tHex;
  U16 tReturnData;
  
  tHdID = tHdID & 0x7F;
  tHooNoH  = (((tHdID - 1) / gLobbyHandle.MaxHo ) + 1);
  tHooNoL  = (((tHdID - 1) % gLobbyHandle.MaxHo ) + 1);

  tReturnData = tHooNoH * 100;
  tReturnData += tHooNoL;

  return tReturnData;
}



U8 KDP_KdIdToHdId( U16 tKdID )
{
  U16 tHeightHouse,tWidthHouse;
  U16 tHouseID;
  tHeightHouse   = (U16)(tKdID / 100);
  tWidthHouse  = (U16)(tKdID % 100);
  tHouseID = (U16)((tWidthHouse) + ((tHeightHouse - 1) * gLobbyHandle.MaxHo));
  if ( tHouseID > 127 ) tHouseID = 0;
  return (U8)tHouseID;
}



U8 KDP_ReceiveProtocol( typeKdProtocol *tRxKdProtocol )
{
  U8 bTxProtocol[PROTOCOL_SIZE_8BYTE + 1];
  U16 tDong;
  U16 tHoo;
  U16 tHalfHex;
  U8 tID;

  bTxProtocol[1] = 0;

  switch ( tRxKdProtocol->Header.Device )
  {
    case KD_PRT_DEVICE_HOUSE:;
      tDong = ((U16)tRxKdProtocol->ID.TxDongIDHi << 8);
      tDong += tRxKdProtocol->ID.TxDongIDLo;
      if ( tDong == 0 ) tDong = gLobbyHandle.DongNumber;
      if ( tDong != gLobbyHandle.DongNumber ) return false;
      
      tHoo = ((U16)tRxKdProtocol->ID.TxHooIDHi << 8);
      tHoo += tRxKdProtocol->ID.TxHooIDLo;
      tID = KDP_KdIdToHdId( tHoo );
      if ( tID == 0 ) return false;
      
      bTxProtocol[0] = tID;
      bTxProtocol[2] = tRxKdProtocol->Channel.SlotID_Channel;
      bTxProtocol[3] = tRxKdProtocol->ID.RxHooIDLo;
      bTxProtocol[4] = 0xEE;
      bTxProtocol[5] = 0x00;
      bTxProtocol[6] = 0x00;
      bTxProtocol[7] = 0x00;
      bTxProtocol[8] = RECEIVE_LINE_HOUSE;
      
      if ( tRxKdProtocol->ID.RxDevice == KD_PRT_DEVICE_LOBBY )
      {
        switch( tRxKdProtocol->Header.Command )
        {
          case KD_PRT_CMD_CALL_ON:;
            //bTxProtocol[1] = RL_CALL_ON;
            break;
          case KD_PRT_CMD_CALL_OK:;
            bTxProtocol[1] = LR_CALL_OK;
            break;
          case KD_PRT_CMD_CALL_BUSY:;
            bTxProtocol[1] = RL_CBUSY;
            break;
          case KD_PRT_CMD_TALK_ON:;
            bTxProtocol[1] = RL_OFF_HOOK;
            break;
          case KD_PRT_CMD_ALL_OFF:;
            bTxProtocol[1] = RL_ON_HOOK;
            break;
          case KD_PRT_CMD_DOOR_OPEN:;
            bTxProtocol[1] = RL_DOPEN;
            break;
          case KD_PRT_CMD_LINE_TEST_ON:
            bTxProtocol[1] = RL_CALL_REQUEST;
            break;
          default:
            return false;
        }
      }    
      else if ( tRxKdProtocol->ID.RxDevice == KD_PRT_DEVICE_GUARD )
      {
        switch( tRxKdProtocol->Header.Command )
        {
          case KD_PRT_CMD_CALL_ON:;
            bTxProtocol[1] = RG_CALL_ON;
            break;
          case KD_PRT_CMD_CALL_OK:;
            bTxProtocol[1] = GR_CALL_OK;
            break;
          case KD_PRT_CMD_CALL_BUSY:;
            bTxProtocol[1] = RG_CBUSY;
            break;
          case KD_PRT_CMD_TALK_ON:;
            bTxProtocol[1] = RG_OFF_HOOK;
            break;
          case KD_PRT_CMD_ALL_OFF:;
            bTxProtocol[1] = RG_ON_HOOK;
            break;
          case KD_PRT_CMD_EMERGENCY:
            if ( tRxKdProtocol->Data[0] == 1 )
            {
              bTxProtocol[1] = RG_EMER;
            }
            else
            {
              bTxProtocol[1] = RG_EMER_CNCL;
            }
            break;
          default:
            return false;
        }
      }
      else
      {
        return false;
      }
      break;
    case KD_PRT_DEVICE_LOBBY:;
      tDong = ((U16)tRxKdProtocol->ID.TxDongIDHi << 8);
      tDong += tRxKdProtocol->ID.TxDongIDLo;
      if ( tDong != gLobbyHandle.DongNumber ) return false;
    
      if ( gLobbyHandle.LobbyID == MASTER_LOBBY_ID )
      {
        tHoo = ((U16)tRxKdProtocol->ID.TxHooIDHi << 8);
        tHoo += tRxKdProtocol->ID.TxHooIDLo;
        bTxProtocol[0] = tHoo;
        bTxProtocol[2] = tRxKdProtocol->Channel.SlotID_Channel;

        tDong = (U16)tRxKdProtocol->ID.RxDongIDHi << 8;
        tDong += tRxKdProtocol->ID.RxDongIDLo;
        Dec2HalfHex( tDong , &tHalfHex );
        bTxProtocol[3] = (U8)((tHalfHex>>8) & 0x0FF);
        bTxProtocol[4] = (U8)(tHalfHex & 0x0FF);

        tHoo = (U16)tRxKdProtocol->ID.RxHooIDHi << 8;
        tHoo += tRxKdProtocol->ID.RxHooIDLo;
        Dec2HalfHex( tHoo , &tHalfHex );
        bTxProtocol[5] = (U8)((tHalfHex >> 8) & 0x0FF);
        bTxProtocol[6] = (U8)(tHalfHex & 0x0FF);
        bTxProtocol[7] = 0xEE;
        bTxProtocol[8] = RECEIVE_LINE_LOBBY;
        if ( tRxKdProtocol->ID.RxDevice == KD_PRT_DEVICE_HOUSE )
          switch( tRxKdProtocol->Header.Command )
          {
            case KD_PRT_CMD_CALL_ON:;
              bTxProtocol[1] = LR_CALL_ON;
              break;
            case KD_PRT_CMD_CALL_OK:;
              //bTxProtocol[1] = RL_CALL_OK;
              break;
            case KD_PRT_CMD_CALL_BUSY:;
              //bTxProtocol[1] = LR_CBUSY;
              break;
            case KD_PRT_CMD_TALK_ON:;
              //bTxProtocol[1] = LR_OFF_HOOK;
              break;
            case KD_PRT_CMD_ALL_OFF:;
              bTxProtocol[1] = LR_ON_HOOK;
              break;
            default:
              return false;
          }
          else if ( tRxKdProtocol->ID.RxDevice == KD_PRT_DEVICE_GUARD )
          {
            switch( tRxKdProtocol->Header.Command )
            {
              case KD_PRT_CMD_CALL_ON:;
                bTxProtocol[1] = LG_CALL_ON;
                break;
              case KD_PRT_CMD_CALL_OK:;
                //bTxProtocol[1] = GL_CALL_OK;
                break;
              case KD_PRT_CMD_CALL_BUSY:;
                //bTxProtocol[1] = LG_CBUSY;
                break;
              case KD_PRT_CMD_TALK_ON:;
                //bTxProtocol[1] = LG_OFF_HOOK;
                break;
              case KD_PRT_CMD_ALL_OFF:;
                bTxProtocol[1] = LG_ON_HOOK;
                break;
              default:
                return false;
            }
          }
          else
          {
            return false;
          }        
      }
      else
      {
        tHoo = ((U16)tRxKdProtocol->ID.RxHooIDHi << 8);
        tHoo += tRxKdProtocol->ID.RxHooIDLo;
        bTxProtocol[0] = tHoo;
        bTxProtocol[2] = tRxKdProtocol->Channel.SlotID_Channel;

        tDong = (U16)tRxKdProtocol->ID.TxDongIDHi << 8;
        tDong += tRxKdProtocol->ID.TxDongIDLo;
        Dec2HalfHex( tDong , &tHalfHex );
        bTxProtocol[3] = (U8)((tHalfHex>>8) & 0x0FF);
        bTxProtocol[4] = (U8)(tHalfHex & 0x0FF);

        tHoo = (U16)tRxKdProtocol->ID.TxHooIDHi << 8;
        tHoo += tRxKdProtocol->ID.TxHooIDLo;
        Dec2HalfHex( tHoo , &tHalfHex );
        bTxProtocol[5] = (U8)((tHalfHex >> 8) & 0x0FF);
        bTxProtocol[6] = (U8)(tHalfHex & 0x0FF);
        bTxProtocol[7] = 0xEE;
        bTxProtocol[8] = RECEIVE_LINE_LOBBY;

        if ( tRxKdProtocol->ID.TxDevice == KD_PRT_DEVICE_HOUSE )
          switch( tRxKdProtocol->Header.Command )
          {
            case KD_PRT_CMD_CALL_ON:;
              //bTxProtocol[1] = RL_CALL_ON;
              break;
            case KD_PRT_CMD_CALL_OK:;
              bTxProtocol[1] = LR_CALL_OK;
              break;
            case KD_PRT_CMD_CALL_BUSY:;
              bTxProtocol[1] = RL_CBUSY;
              break;
            case KD_PRT_CMD_TALK_ON:;
              bTxProtocol[1] = RL_OFF_HOOK;
              break;
            case KD_PRT_CMD_ALL_OFF:;
              bTxProtocol[1] = RL_ON_HOOK;
              break;
            case KD_PRT_CMD_DOOR_OPEN:;
              bTxProtocol[1] = RL_DOPEN;
              break;
            default:
              return false;
          }
          else if ( tRxKdProtocol->ID.TxDevice == KD_PRT_DEVICE_GUARD )
          {
            switch( tRxKdProtocol->Header.Command )
            {
              case KD_PRT_CMD_CALL_ON:;
                //bTxProtocol[1] = GL_CALL_ON;
                break;
              case KD_PRT_CMD_CALL_OK:;
                bTxProtocol[1] = LG_CALL_OK;
                break;
              case KD_PRT_CMD_CALL_BUSY:;
                bTxProtocol[1] = GL_CBUSY;
                break;
              case KD_PRT_CMD_TALK_ON:;
                bTxProtocol[1] = GL_OFF_HOOK;
                break;
              case KD_PRT_CMD_ALL_OFF:;
                bTxProtocol[1] = GL_ON_HOOK;
                break;
              case KD_PRT_CMD_DOOR_OPEN:;
                bTxProtocol[1] = GL_DOPEN;
                break;            
              default:
                return false;
            }

          }
          else
          {
            return false;
          } 
      }
      break;
    case KD_PRT_DEVICE_GUARD:;
      tDong = ((U16)tRxKdProtocol->ID.TxDongIDHi << 8);
      tDong += tRxKdProtocol->ID.TxDongIDLo;
      if ( tDong != 101 ) return false;
    
      bTxProtocol[0] = tRxKdProtocol->ID.TxHooIDLo;
      
      bTxProtocol[2] = tRxKdProtocol->Channel.SlotID_Channel;    
      
      tDong = (U16)tRxKdProtocol->ID.RxDongIDHi << 8;
      tDong += tRxKdProtocol->ID.RxDongIDLo;
      Dec2HalfHex( tDong , &tHalfHex );
      bTxProtocol[3] = (U8)((tHalfHex>>8) & 0x0FF);
      bTxProtocol[4] = (U8)(tHalfHex & 0x0FF);

      tHoo = (U16)tRxKdProtocol->ID.RxHooIDHi << 8;
      tHoo += tRxKdProtocol->ID.RxHooIDLo;
      Dec2HalfHex( tHoo , &tHalfHex );
      bTxProtocol[5] = (U8)((tHalfHex>>8) & 0x0FF);
      bTxProtocol[6] = (U8)(tHalfHex & 0x0FF);
      bTxProtocol[7] = 0xEE;

      bTxProtocol[8] = RECEIVE_LINE_GUARD;   
      
      if ( tRxKdProtocol->ID.RxDevice == KD_PRT_DEVICE_LOBBY )
      {
        switch( tRxKdProtocol->Header.Command )
        {
          case KD_PRT_CMD_CALL_ON:;
            //bTxProtocol[1] = GL_CALL_ON;
            break;
          case KD_PRT_CMD_CALL_OK:;
            bTxProtocol[1] = LG_CALL_OK;
            break;
          case KD_PRT_CMD_CALL_BUSY:;
            bTxProtocol[1] = GL_CBUSY;
            break;
          case KD_PRT_CMD_TALK_ON:;
            bTxProtocol[1] = GL_OFF_HOOK;
            break;
          case KD_PRT_CMD_ALL_OFF:;
            bTxProtocol[1] = GL_ON_HOOK;
            break;
          case KD_PRT_CMD_DOOR_OPEN:;
            bTxProtocol[1] = GL_DOPEN;
            break;
          default:
            return false;
        }
      }    
      else if ( tRxKdProtocol->ID.RxDevice == KD_PRT_DEVICE_HOUSE )
      {
        switch( tRxKdProtocol->Header.Command )
        {
          case KD_PRT_CMD_CALL_ON:;
            bTxProtocol[1] = GR_CALL_ON;
            break;
          case KD_PRT_CMD_CALL_OK:;
            bTxProtocol[1] = RG_CALL_OK;
            break;
          case KD_PRT_CMD_CALL_BUSY:;
            bTxProtocol[1] = GR_CBUSY;
            break;
          case KD_PRT_CMD_TALK_ON:;
            bTxProtocol[1] = GR_OFF_HOOK;
            break;
          case KD_PRT_CMD_ALL_OFF:;
            bTxProtocol[1] = GR_ON_HOOK;
            break;
          default:
            return false;
        }
      }
      else
      {
        return false;
      }
      break;
    default:
      return false;
  }

  SendMsgReceiveProtocol ( bTxProtocol );
  
  return true;
  
}


U8 KDP_TransmitProtocol(U8 tId, U8 tCmd, U8 tChannel, U8 DongNoH_LobbyID, U8 DongNoL, U8 HooNoH, U8 HooNoL)
{
  typeKdProtocol tKdProtocolData;
  U16 tTemp16;
  U16 tTempHalfHex;
  U8 tCh;
  U16 tCnt;
  U16 tProtocolLength;

  if ( DongNoL == 0 && HooNoH == 0 && HooNoL == 0 )
  {
    tKdProtocolData.ID.RxDongIDHi = (gLobbyHandle.DongNumber >> 8) & 0xFF;
    tKdProtocolData.ID.RxDongIDLo = gLobbyHandle.DongNumber & 0xFF;
    tTemp16 = KDP_HdIdToKdID( tId );
    tKdProtocolData.ID.RxHooIDHi = (tTemp16 >> 8) & 0xFF;
    tKdProtocolData.ID.RxHooIDLo = tTemp16 & 0xFF;
    
    tKdProtocolData.ID.TxDongIDHi = (gLobbyHandle.DongNumber >> 8) & 0xFF;
    tKdProtocolData.ID.TxDongIDLo = gLobbyHandle.DongNumber & 0xFF;
    tKdProtocolData.ID.TxHooIDHi = 0;
    tKdProtocolData.ID.TxHooIDLo = DongNoH_LobbyID;
  }
  else
  {
    if ( gLobbyHandle.LobbyID == MASTER_LOBBY_ID )
    {
      tKdProtocolData.ID.RxDongIDHi = 0;//(gLobbyHandle.DongNumber >> 8) & 0xFF;
      tKdProtocolData.ID.RxDongIDLo = 101;//gLobbyHandle.DongNumber & 0xFF;
      //tTemp16 = KDP_HdIdToKdID( tId );
      tKdProtocolData.ID.RxHooIDHi = 0;//(tTemp16 >> 8) & 0xFF;
      tKdProtocolData.ID.RxHooIDLo = tId;//tTemp16 & 0xFF;
      
      tTempHalfHex = (U16)DongNoH_LobbyID << 8;
      tTempHalfHex += DongNoL;
      HalfHex2Dec(tTempHalfHex, &tTemp16);
      tKdProtocolData.ID.TxDongIDHi = (tTemp16 >> 8) & 0xFF;
      tKdProtocolData.ID.TxDongIDLo = tTemp16 & 0xFF;
      
      tTempHalfHex = (U16)HooNoH << 8;
      tTempHalfHex += HooNoL;
      HalfHex2Dec(tTempHalfHex, &tTemp16);
      tKdProtocolData.ID.TxHooIDHi = (tTemp16 >> 8) & 0xFF;
      tKdProtocolData.ID.TxHooIDLo = tTemp16 & 0xFF;
    }
    else
    {
      tKdProtocolData.ID.TxDongIDHi = (gLobbyHandle.DongNumber >> 8) & 0xFF;
      tKdProtocolData.ID.TxDongIDLo = gLobbyHandle.DongNumber & 0xFF;
      //tTemp16 = KDP_HdIdToKdID( tId );
      tKdProtocolData.ID.TxHooIDHi = 0;//(tTemp16 >> 8) & 0xFF;
      tKdProtocolData.ID.TxHooIDLo = tId;//tTemp16 & 0xFF;
      
      tTempHalfHex = (U16)DongNoH_LobbyID << 8;
      tTempHalfHex += DongNoL;
      HalfHex2Dec(tTempHalfHex, &tTemp16);
      tKdProtocolData.ID.RxDongIDHi = (tTemp16 >> 8) & 0xFF;
      tKdProtocolData.ID.RxDongIDLo = tTemp16 & 0xFF;
      
      tTempHalfHex = (U16)HooNoH << 8;
      tTempHalfHex += HooNoL;
      HalfHex2Dec(tTempHalfHex, &tTemp16);
      tKdProtocolData.ID.RxHooIDHi = (tTemp16 >> 8) & 0xFF;
      tKdProtocolData.ID.RxHooIDLo = tTemp16 & 0xFF;      
    }
  }

  tKdProtocolData.Channel.Video = 1;
  tProtocolLength = KD_PROTOCOL_DEFAULT_LENGTH;

  switch ( tCmd )
  {
    case RG_CALL_ON:;  // RG
      tKdProtocolData.Header.Command = KD_PRT_CMD_CALL_ON;
      tKdProtocolData.ID.TxDevice   = KD_PRT_DEVICE_HOUSE;
      tKdProtocolData.ID.RxDevice   = KD_PRT_DEVICE_GUARD;
      break;
    case RG_CALL_OK:;
      tKdProtocolData.Header.Command = KD_PRT_CMD_CALL_OK;
      tKdProtocolData.ID.TxDevice   = KD_PRT_DEVICE_GUARD;
      tKdProtocolData.ID.RxDevice   = KD_PRT_DEVICE_HOUSE;
      break;
    case RG_CALL_OFF:;
      tKdProtocolData.Header.Command = KD_PRT_CMD_ALL_OFF;
      tKdProtocolData.ID.TxDevice   = KD_PRT_DEVICE_HOUSE;
      tKdProtocolData.ID.RxDevice   = KD_PRT_DEVICE_GUARD;
      break;
    case GR_CBUSY:;
      tKdProtocolData.Header.Command = KD_PRT_CMD_CALL_BUSY;
      tKdProtocolData.ID.TxDevice   = KD_PRT_DEVICE_GUARD;
      tKdProtocolData.ID.RxDevice   = KD_PRT_DEVICE_HOUSE;
      break;    
    case GR_OFF_HOOK:;
      tKdProtocolData.Header.Command = KD_PRT_CMD_TALK_ON;
      tKdProtocolData.ID.TxDevice   = KD_PRT_DEVICE_GUARD;
      tKdProtocolData.ID.RxDevice   = KD_PRT_DEVICE_HOUSE;
      break;   
    case GR_ON_HOOK:;
      tKdProtocolData.Header.Command = KD_PRT_CMD_ALL_OFF;
      tKdProtocolData.ID.TxDevice   = KD_PRT_DEVICE_GUARD;
      tKdProtocolData.ID.RxDevice   = KD_PRT_DEVICE_HOUSE;
      break;       
    case RG_ON_HOOK:;
      tKdProtocolData.Header.Command = KD_PRT_CMD_ALL_OFF;
      tKdProtocolData.ID.TxDevice   = KD_PRT_DEVICE_HOUSE;
      tKdProtocolData.ID.RxDevice   = KD_PRT_DEVICE_GUARD;
      break;
    case RG_EMER:;
      tKdProtocolData.Header.Command = KD_PRT_CMD_EMERGENCY;
      tKdProtocolData.ID.TxDevice   = KD_PRT_DEVICE_HOUSE;
      tKdProtocolData.ID.RxDevice   = KD_PRT_DEVICE_GUARD;
      tKdProtocolData.Data[0]       = 1;
      tProtocolLength++;
      break;
    case RG_EMER_CNCL:;
      tKdProtocolData.Header.Command = KD_PRT_CMD_EMERGENCY;
      tKdProtocolData.ID.TxDevice   = KD_PRT_DEVICE_HOUSE;
      tKdProtocolData.ID.RxDevice   = KD_PRT_DEVICE_GUARD;
      tKdProtocolData.Data[0]       = 2;
      tProtocolLength++;
      break;
    case GR_CALL_ON:;  // GR
      tKdProtocolData.Header.Command = KD_PRT_CMD_CALL_ON;
      tKdProtocolData.ID.TxDevice   = KD_PRT_DEVICE_GUARD;
      tKdProtocolData.ID.RxDevice   = KD_PRT_DEVICE_HOUSE;
      break;    
    case GR_CALL_OK:;
      tKdProtocolData.Header.Command = KD_PRT_CMD_CALL_OK;
      tKdProtocolData.ID.TxDevice   = KD_PRT_DEVICE_HOUSE;
      tKdProtocolData.ID.RxDevice   = KD_PRT_DEVICE_GUARD;
      break;    
    case GR_CALL_OFF:;
      tKdProtocolData.Header.Command = KD_PRT_CMD_ALL_OFF;
      tKdProtocolData.ID.TxDevice   = KD_PRT_DEVICE_GUARD;
      tKdProtocolData.ID.RxDevice   = KD_PRT_DEVICE_HOUSE;
      break;      
    case RG_CBUSY:;
      tKdProtocolData.Header.Command = KD_PRT_CMD_CALL_BUSY;
      tKdProtocolData.ID.TxDevice   = KD_PRT_DEVICE_HOUSE;
      tKdProtocolData.ID.RxDevice   = KD_PRT_DEVICE_GUARD;
      break;        
    case RG_OFF_HOOK:;
      tKdProtocolData.Header.Command = KD_PRT_CMD_TALK_ON;
      tKdProtocolData.ID.TxDevice   = KD_PRT_DEVICE_HOUSE;
      tKdProtocolData.ID.RxDevice   = KD_PRT_DEVICE_GUARD;
      break;       
/*    case RG_ON_HOOK:;
      tKdProtocolData.Header.Command = KD_PRT_CMD_ALL_OFF;
      tKdProtocolData.ID.TxDevice   = KD_PRT_DEVICE_HOUSE;
      tKdProtocolData.ID.RxDevice   = KD_PRT_DEVICE_GUARD;
      break;      */
/*    case GR_ON_HOOK:;
      tKdProtocolData.Header.Command = KD_PRT_CMD_ALL_OFF;
      tKdProtocolData.ID.TxDevice   = KD_PRT_DEVICE_GUARD;
      tKdProtocolData.ID.RxDevice   = KD_PRT_DEVICE_HOUSE;
      break;      */
    case LR_CALL_ON:;  // LR
      tKdProtocolData.Header.Command = KD_PRT_CMD_CALL_ON;
      tKdProtocolData.ID.TxDevice   = KD_PRT_DEVICE_LOBBY;
      tKdProtocolData.ID.RxDevice   = KD_PRT_DEVICE_HOUSE;
      tKdProtocolData.Channel.Video = 2;
      break;    
    case LR_CALL_OK:;
      tKdProtocolData.Header.Command = KD_PRT_CMD_CALL_OK;
      tKdProtocolData.ID.TxDevice   = KD_PRT_DEVICE_HOUSE;
      tKdProtocolData.ID.RxDevice   = KD_PRT_DEVICE_LOBBY;
      break;    
    case LR_CALL_OFF:;
      tKdProtocolData.Header.Command = KD_PRT_CMD_ALL_OFF;
      tKdProtocolData.ID.TxDevice   = KD_PRT_DEVICE_LOBBY;
      tKdProtocolData.ID.RxDevice   = KD_PRT_DEVICE_HOUSE;
      break;      
    case RL_CBUSY:;
      tKdProtocolData.Header.Command = KD_PRT_CMD_CALL_BUSY;
      tKdProtocolData.ID.TxDevice   = KD_PRT_DEVICE_HOUSE;
      tKdProtocolData.ID.RxDevice   = KD_PRT_DEVICE_LOBBY;
      break;        
    case RL_OFF_HOOK:;
      tKdProtocolData.Header.Command = KD_PRT_CMD_TALK_ON;
      tKdProtocolData.ID.TxDevice   = KD_PRT_DEVICE_HOUSE;
      tKdProtocolData.ID.RxDevice   = KD_PRT_DEVICE_LOBBY;
      break;       
    case RL_ON_HOOK:;
      tKdProtocolData.Header.Command = KD_PRT_CMD_ALL_OFF;
      tKdProtocolData.ID.TxDevice   = KD_PRT_DEVICE_HOUSE;
      tKdProtocolData.ID.RxDevice   = KD_PRT_DEVICE_LOBBY;
      break;      
    case LR_ON_HOOK:;
      tKdProtocolData.Header.Command = KD_PRT_CMD_ALL_OFF;
      tKdProtocolData.ID.TxDevice   = KD_PRT_DEVICE_LOBBY;
      tKdProtocolData.ID.RxDevice   = KD_PRT_DEVICE_HOUSE;
      break;      
    case RL_DOPEN:;
      tKdProtocolData.Header.Command = KD_PRT_CMD_DOOR_OPEN;
      tKdProtocolData.ID.TxDevice   = KD_PRT_DEVICE_HOUSE;
      tKdProtocolData.ID.RxDevice   = KD_PRT_DEVICE_LOBBY;
      break;
    case LG_CALL_ON:;  // LG
      tKdProtocolData.Header.Command = KD_PRT_CMD_CALL_ON;
      tKdProtocolData.ID.TxDevice   = KD_PRT_DEVICE_LOBBY;
      tKdProtocolData.ID.RxDevice   = KD_PRT_DEVICE_GUARD;
      break;    
    case LG_CALL_OK:;
      tKdProtocolData.Header.Command = KD_PRT_CMD_CALL_OK;
      tKdProtocolData.ID.TxDevice   = KD_PRT_DEVICE_GUARD;
      tKdProtocolData.ID.RxDevice   = KD_PRT_DEVICE_LOBBY;
      break;    
    case LG_CALL_OFF:;
      tKdProtocolData.Header.Command = KD_PRT_CMD_ALL_OFF;
      tKdProtocolData.ID.TxDevice   = KD_PRT_DEVICE_LOBBY;
      tKdProtocolData.ID.RxDevice   = KD_PRT_DEVICE_GUARD;
      break;       
    case GL_CBUSY:;
      tKdProtocolData.Header.Command = KD_PRT_CMD_CALL_BUSY;
      tKdProtocolData.ID.TxDevice   = KD_PRT_DEVICE_GUARD;
      tKdProtocolData.ID.RxDevice   = KD_PRT_DEVICE_LOBBY;
      break;        
    case GL_OFF_HOOK:;
      tKdProtocolData.Header.Command = KD_PRT_CMD_TALK_ON;
      tKdProtocolData.ID.TxDevice   = KD_PRT_DEVICE_GUARD;
      tKdProtocolData.ID.RxDevice   = KD_PRT_DEVICE_LOBBY;
      break;       
    case GL_ON_HOOK:;
      tKdProtocolData.Header.Command = KD_PRT_CMD_ALL_OFF;
      tKdProtocolData.ID.TxDevice   = KD_PRT_DEVICE_GUARD;
      tKdProtocolData.ID.RxDevice   = KD_PRT_DEVICE_LOBBY;
      break;      
    case LG_ON_HOOK:;
      tKdProtocolData.Header.Command = KD_PRT_CMD_ALL_OFF;
      tKdProtocolData.ID.TxDevice   = KD_PRT_DEVICE_LOBBY;
      tKdProtocolData.ID.RxDevice   = KD_PRT_DEVICE_GUARD;
      break;      
    case GL_DOPEN:;
      tKdProtocolData.Header.Command = KD_PRT_CMD_DOOR_OPEN;
      tKdProtocolData.ID.TxDevice   = KD_PRT_DEVICE_GUARD;
      tKdProtocolData.ID.RxDevice   = KD_PRT_DEVICE_LOBBY;
      break; 
    default:
      return false;
  }
    
  tKdProtocolData.Channel.SlotID_Channel = tChannel;

  tKdProtocolData.Header.Device = tKdProtocolData.ID.RxDevice;
  tKdProtocolData.Header.SubID  = 0x80;


  if ( gLobbyHandle.LobbyID == MASTER_LOBBY_ID )
  {
    switch ( tKdProtocolData.Header.Device )
    {
      case KD_PRT_DEVICE_HOUSE:
        tCh = PROTOCOL_HOME_CH1;
        break;
      case KD_PRT_DEVICE_LOBBY:
        tCh = PROTOCOL_LOBBY_CH1;
        break;
      case KD_PRT_DEVICE_GUARD:
        tCh = PROTOCOL_GUARD_CH0;
        break;
      default:
        return false;      
    }
  }
  else
  {
    tKdProtocolData.Header.Device = tKdProtocolData.ID.TxDevice;
    tKdProtocolData.Header.SubID  = 0x00;
    tCh = PROTOCOL_LOBBY_CH1;
  }

  tCnt = 10000;
  while ( tCnt-- != 0 )
  {
    if( Kd_TransmitDataRawData(tCh, tKdProtocolData.Header.Device,
      tKdProtocolData.Header.SubID, tKdProtocolData.Header.Command, (U8*)&tKdProtocolData.ID, tProtocolLength, false ) ) break;
  }

  return true;
  
}

void KDP_LobbySetupChannel(void)
{
  if ( gLobbyHandle.LobbyID == MASTER_LOBBY_ID )
  {
    Kd_SetupChannel( PROTOCOL_GUARD_CH0,  9600, KD_PRT_DEVICE_GUARD, 0, TransmitGuardCntrTx, TransmitGuardCntrRx, TransmitGuard);
    Kd_SetupChannel( PROTOCOL_HOME_CH1,   9600, KD_PRT_DEVICE_HOUSE, 0, TransmitHouseLobbyCntrTx, TransmitHouseLobbyCntrRx, TransmitHouseLobby);
    Kd_SetupChannel( PROTOCOL_LOBBY_CH1,  9600, KD_PRT_DEVICE_LOBBY, 0, TransmitHouseLobbyCntrTx, TransmitHouseLobbyCntrRx, TransmitHouseLobby);
  }
  else
  {
    Kd_SetupChannel( PROTOCOL_GUARD_CH0,  9600, KD_PRT_DEVICE_MAINDEVICE, 0, TransmitGuardCntrTx, TransmitGuardCntrRx, TransmitGuard);
    Kd_SetupChannel( PROTOCOL_HOME_CH1,   9600, KD_PRT_DEVICE_MAINDEVICE, 0, TransmitHouseLobbyCntrTx, TransmitHouseLobbyCntrRx, TransmitHouseLobby);
    Kd_SetupChannel( PROTOCOL_LOBBY_CH1,  9600, KD_PRT_DEVICE_LOBBY, 0x80, TransmitHouseLobbyCntrTx, TransmitHouseLobbyCntrRx, TransmitHouseLobby);
  }      
}



