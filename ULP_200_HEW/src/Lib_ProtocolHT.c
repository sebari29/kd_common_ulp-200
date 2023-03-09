/// @file   Lib_KdProtocol.c
/// @date   2012/02/14
/// @author jinhwa Lee
/// @brief  

#include "main.h"

/// 체널 갯수
#define HT_CHANNEL_MAX          1


/// 기본정의
#define HT_PROTOCOL_5_MAX_SIZE  5
#define HT_PROTOCOL_8_MAX_SIZE  8
#define HT_PACKET_EOD           0xEE


/// 상태 관리 정의
#define HT_STATUS_LINE_FREE     0

#define HT_STATUS_RX_STANDBY    0

#define HT_STATUS_TX_STANDBY    0
#define HT_STATUS_TX_485EN      1
#define HT_STATUS_TX_DATA       2
#define STATUS_TX_485DIS        3


typedef union
{
  typeHtProtocolFormat5 L5;
  typeHtProtocolFormat8 L8;
}__typeHtProtocolFormat;


typedef struct
{
  U8 Status;
  U8 Count;
  U8 Length;
  U8 *BytePointer;
  __typeHtProtocolFormat Protocol;
}__typeHtProtocolRx;

typedef struct
{
  U8 Status;
  U8 Count;
  U8 Length;
  U8 RS485Enable;
  void (*GPrt_485TxEnabletion)(void);
  void (*Rx485EnableFunction)(void);
  void (*TxBufferRegister)(U16);  
  __typeHtProtocolFormat Protocol;
}__typeHtProtocolTx;


typedef struct
{
  U8 Use;
  U8 ThisID;
  U8 StatusLine;
  U8 TimeOut;
  __typeHtProtocolRx Rx;
  __typeHtProtocolTx Tx;
}__typeHtProtocolManager;


__typeHtProtocolManager mHtProtocolManager[HT_CHANNEL_MAX];


U8 Ht_ReceiveSuccessL5(U8 tChannel, __typeHtProtocolFormat *tRxProtocol);
U8 Ht_ReceiveSuccessL8(U8 tChannel, __typeHtProtocolFormat *tRxProtocol);

void Ht_SetupChannel( U8 tChannel, U8 tThisID, 
                      void (*tGPrt_485TxEnabletion)(void), void (*tRx485EnableFunction)(void),
                      void (*tTxBufferRegister)(U16))
{
  mHtProtocolManager[tChannel].Use              = true;
  mHtProtocolManager[tChannel].ThisID           = tThisID;
  mHtProtocolManager[tChannel].StatusLine       = HT_STATUS_LINE_FREE;
  mHtProtocolManager[tChannel].TimeOut          = 0;

  mHtProtocolManager[tChannel].Rx.Status        = HT_STATUS_RX_STANDBY;
  mHtProtocolManager[tChannel].Tx.Status        = HT_STATUS_TX_STANDBY;
  mHtProtocolManager[tChannel].Tx.GPrt_485TxEnabletion = tGPrt_485TxEnabletion;
  mHtProtocolManager[tChannel].Tx.Rx485EnableFunction = tRx485EnableFunction;
  mHtProtocolManager[tChannel].Tx.TxBufferRegister    = tTxBufferRegister;
}


void Ht_ReceiveData( U8 tChannel, U8 tStart, U8 tRxData )
{

  if ( tStart )
  {
    mHtProtocolManager[tChannel].Rx.BytePointer = &mHtProtocolManager[tChannel].Rx.Protocol.L5.ID;
    mHtProtocolManager[tChannel].Rx.Count = 0;
    mHtProtocolManager[tChannel].Rx.Status = true;
  }

  if ( mHtProtocolManager[tChannel].Rx.Status )
  {

    *mHtProtocolManager[tChannel].Rx.BytePointer = tRxData;
    mHtProtocolManager[tChannel].Rx.BytePointer++;
    mHtProtocolManager[tChannel].Rx.Count++;

    if ( tRxData == HT_PACKET_EOD )
    {
      if ( mHtProtocolManager[tChannel].Rx.Count == HT_PROTOCOL_5_MAX_SIZE )
      {
        Ht_ReceiveSuccessL5( tChannel, &mHtProtocolManager[tChannel].Rx.Protocol);
        mHtProtocolManager[tChannel].Rx.Length = HT_PROTOCOL_5_MAX_SIZE;
        mHtProtocolManager[tChannel].Rx.Status = false;
      }
      
      if ( mHtProtocolManager[tChannel].Rx.Count == HT_PROTOCOL_8_MAX_SIZE )
      {
        Ht_ReceiveSuccessL8( tChannel, &mHtProtocolManager[tChannel].Rx.Protocol);
        mHtProtocolManager[tChannel].Rx.Length = HT_PROTOCOL_8_MAX_SIZE;
        mHtProtocolManager[tChannel].Rx.Status = false;
      }
    }

    if ( mHtProtocolManager[tChannel].Rx.Count > HT_PROTOCOL_8_MAX_SIZE )
    {
      mHtProtocolManager[tChannel].Rx.Status = false;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////



void Ht_TransmitDataL5( U8 tChannel, U8 tID, U8 tCommand, U8 tPathInfo, U8 tLobbyID )
{
 
  mHtProtocolManager[tChannel].Tx.Protocol.L5.ID       = tID;
  mHtProtocolManager[tChannel].Tx.Protocol.L5.Command  = tCommand;
  mHtProtocolManager[tChannel].Tx.Protocol.L5.PathInfo = tPathInfo;
  mHtProtocolManager[tChannel].Tx.Protocol.L5.LobbyID  = tLobbyID;
  mHtProtocolManager[tChannel].Tx.Protocol.L5.EOD      = HT_PACKET_EOD;
  mHtProtocolManager[tChannel].Tx.Length = HT_PROTOCOL_5_MAX_SIZE;
  mHtProtocolManager[tChannel].Tx.Status = HT_STATUS_TX_485EN;

}

void Ht_TransmitDataL8( U8 tChannel, U8 tID, U8 tCommand, U8 tPathInfo, U8 tDongNoH, U8 tDongNoL, U8 tHooNoH, U8 tHooNoL )
{
 
  mHtProtocolManager[tChannel].Tx.Protocol.L8.ID       = tID;
  mHtProtocolManager[tChannel].Tx.Protocol.L8.Command  = tCommand;
  mHtProtocolManager[tChannel].Tx.Protocol.L8.PathInfo = tPathInfo;
  mHtProtocolManager[tChannel].Tx.Protocol.L8.DongNoH  = tDongNoH;
  mHtProtocolManager[tChannel].Tx.Protocol.L8.DongNoL  = tDongNoL;
  mHtProtocolManager[tChannel].Tx.Protocol.L8.HooNoH   = tHooNoH;
  mHtProtocolManager[tChannel].Tx.Protocol.L8.HooNoL   = tHooNoL;
  mHtProtocolManager[tChannel].Tx.Protocol.L8.EOD      = HT_PACKET_EOD;
  
  mHtProtocolManager[tChannel].Tx.Length = HT_PROTOCOL_8_MAX_SIZE;
  mHtProtocolManager[tChannel].Tx.Status = HT_STATUS_TX_485EN;

}


U8 __Ht_Transmit485( U8 tChannel )
{
  U16 tData;
  static U8 * tPointer;
 
  //if ( mHdChannel[tChannel].TimeOut < BYTE_INTERVAL_TIMEOUT ) return false;

  switch ( mHtProtocolManager[tChannel].Tx.Status )
  {
    case HT_STATUS_TX_STANDBY:
      return false;
    case HT_STATUS_TX_485EN:
      mHtProtocolManager[tChannel].Tx.RS485Enable = true;
      mHtProtocolManager[tChannel].Tx.GPrt_485TxEnabletion();
      mHtProtocolManager[tChannel].Tx.Count = 0;
      mHtProtocolManager[tChannel].Tx.Status  = HT_STATUS_TX_DATA;
      tPointer = &mHtProtocolManager[tChannel].Tx.Protocol.L5.ID;
      return false;
    case HT_STATUS_TX_DATA:
      tData = *tPointer;
      tPointer++;
      if ( mHtProtocolManager[tChannel].Tx.Count == ZERO ) tData |= 0x0100;
      mHtProtocolManager[tChannel].Tx.TxBufferRegister(tData);
      mHtProtocolManager[tChannel].Tx.Count++;
      if ( mHtProtocolManager[tChannel].Tx.Count == mHtProtocolManager[tChannel].Tx.Length)
      {
        mHtProtocolManager[tChannel].Tx.Status  = STATUS_TX_485DIS;
      }
      return true;
    case STATUS_TX_485DIS:
      mHtProtocolManager[tChannel].Tx.RS485Enable = false;
      mHtProtocolManager[tChannel].Tx.Rx485EnableFunction();
      mHtProtocolManager[tChannel].Tx.Status  = HT_STATUS_TX_STANDBY;
      return false;
    default:
      return false;
  }

}



void Ht_TimerInterrupt3ms( void )
{
  U8 tChannel;
  for ( tChannel = 0 ; tChannel < HT_CHANNEL_MAX ; tChannel++ )
  {
    if ( mHtProtocolManager[tChannel].Use == false) break;
//    if ( mHdChannel[tChannel].TimeOut != 0xFF ) mHdChannel[tChannel].TimeOut++;
    __Ht_Transmit485(tChannel);
  }
}


//////////////////////////// User Code ///////////////////////////////////////

U8 Ht_ReceiveSuccessL5(U8 tChannel, __typeHtProtocolFormat *tRxProtocol)
{
  return false;
}

U8 Ht_ReceiveSuccessL8(U8 tChannel, __typeHtProtocolFormat *tRxProtocol)
{
//  GPrt_AnalysisProtocolHT( (U8*)&tRxProtocol->L8 );
  return false;
}



