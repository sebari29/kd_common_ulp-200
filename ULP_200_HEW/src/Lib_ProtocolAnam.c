
/// @file   Lib_AnamProtocol.c
/// @date   2012/07/20
/// @author jinhwa Lee
/// @brief  Anam Protocol ���� ����

#include "main.h"

#define ANAM_PORT_RX    __RS485_HOUSE_RX
#define ANAM_PORT_TX    __RS485_HOUSE_TX
#define ANAM_PORT_CNTR  __RS485_HOUSE_CNTR

#define ANAM_PROTOCOL_BYTE_SIZE       7
#define ANAM_PROTOCOL_BIT_SIZE        (ANAM_PROTOCOL_BYTE_SIZE * 8)

#define ANAM_PULSE_TIME_START         10
#define ANAM_PULSE_TIME_BIT_HIGH      6
#define ANAM_PULSE_TIME_BIT_LOW       2


#define ANAM_RX_PORTDATA_ERROR        3
#define ANAM_RX_PORTDATA_START        2
#define ANAM_RX_PORTDATA_1            1
#define ANAM_RX_PORTDATA_0            0


#define ANAM_RX_TIME_CNT_MAX          14

const U8 Anam_RxDataCntTable[15] = 
{
  ANAM_RX_PORTDATA_0,   // 0usec
  ANAM_RX_PORTDATA_0,   // 100usec
  ANAM_RX_PORTDATA_0,   // 200usec
  ANAM_RX_PORTDATA_0,   // 300usec
  ANAM_RX_PORTDATA_0,   // 400usec
  ANAM_RX_PORTDATA_1,   // 500usec
  ANAM_RX_PORTDATA_1,   // 600usec
  ANAM_RX_PORTDATA_1,   // 700usec
  ANAM_RX_PORTDATA_1,   // 800usec
  ANAM_RX_PORTDATA_START,   // 900usec
  ANAM_RX_PORTDATA_START,   // 1000usec
  ANAM_RX_PORTDATA_START,   // 1100usec
  ANAM_RX_PORTDATA_START,   // 1200usec
  ANAM_RX_PORTDATA_ERROR    // 1300usec
};

#define ANAM_STATUS_STANDBY   0
#define ANAM_STATUS_RX_DATA   1
#define ANAM_STATUS_TX_DELAY  2
#define ANAM_STATUS_TX_START  3
#define ANAM_STATUS_TX_DATA   4
#define ANAM_STATUS_TX_END    5

typedef struct
{
  U8 Status;
  U8 CountTimer;
  U8 BitCount;
  U8 BitData[ANAM_PROTOCOL_BIT_SIZE];
  U8 ByteData[ANAM_PROTOCOL_BYTE_SIZE];
  U8 RxOldBit;
}typeAnamProtocol;

typeAnamProtocol mAnamProtocol;


/// @brief �Ƴ� ���������� �����͸� ��Ʈ���� ����Ʈ�� ��ȯ
/// @param void
/// @return void
void Anam_DataBitToByte( void )
{
  U8 i,j;
  U8 tIndex;
  U8 tTemp;

  for ( i = 0 ; i < ANAM_PROTOCOL_BYTE_SIZE ; i++ )
  {
    tIndex = (ANAM_PROTOCOL_BYTE_SIZE - 1) - i;
    tTemp = (i * 8);
    mAnamProtocol.ByteData[ tIndex ] = 0;
    for ( j = 0; j < 8; j++ )
    {
      mAnamProtocol.ByteData[ tIndex ] >>= 1;
      mAnamProtocol.ByteData[ tIndex ] += (mAnamProtocol.BitData[ tTemp + j ] << 7);
    }
  }
}


/// @brief �Ƴ� ���������� �����͸� ����Ʈ���� ��Ʈ�� ��ȯ
/// @param void
/// @return void
void Anam_DataByteToBit( void )
{
  U8 i,j;
  U8 tIndex;
  U8 tTemp;

  for ( i = 0 ; i < ANAM_PROTOCOL_BYTE_SIZE ; i++ )
  {
    tIndex = ( i * 8 );
    tTemp = mAnamProtocol.ByteData[ (ANAM_PROTOCOL_BYTE_SIZE - 1) - i ];
    for ( j = 0; j < 8; j++ )
    {
      mAnamProtocol.BitData[ tIndex + j ] = (tTemp & 0x01);
      tTemp >>= 1;
    }
  }
}



/// @brief �κ��� �� ID�� �Ƴ������� ��ȯ
/// @param tDong �κ��� �����̵�
/// @return �Ƴ� �� ID
U16 Anam_DongToAnamDong( U16 tDong )
{
  U16 tReturnData;

  Dec2HalfHex( tDong, &tReturnData );

  if ( (tReturnData & 0xF000) == 0 )
  {
    tReturnData <<= 4;
    tReturnData += 0x000F;
  }
 
  return tReturnData;
}


/// @brief ���� ID�� �Ƴ�ID�� ����
/// @param tHdID ���� ID
/// @return �Ƴ� ID
U16 Anam_HdIdToAnamID( U8 tHdID )
{
  U8 tHooNoH,tHooNoL;
  U16 tHex;
  U16 tReturnData;
  
  tHdID = tHdID & 0x7F;
  tHooNoH  = (((tHdID - 1) / gLobbyHandle.MaxHo ) + 1);
  tHooNoL  = (((tHdID - 1) % gLobbyHandle.MaxHo ) + 1);

  Dec2HalfHex( tHooNoH, &tHex );
  
  tReturnData = tHex;
  tReturnData <<= 8;
  Dec2HalfHex( tHooNoL, &tHex );
  tReturnData += tHex;

  if ( (tReturnData & 0xF000) == 0 )
  {
    tReturnData <<= 4;
    tReturnData += 0x000F;
  }
 
  return tReturnData;
}



/// @brief �Ƴ� ID�� ����ID�� ����
/// @param tAnamID �Ƴ� ID
/// @return ���� ID
U8 Anam_AnamIdToHdId( U16 tAnamID )
{
  U8 tHooNoH,tHooNoL;
  U8 tHeightHouse,tWidthHouse,tHouseID;

  if ( (tAnamID & 0x000F ) == 0x000F )
  {
    tAnamID >>= 4;
  }

  tHooNoH = (tAnamID >> 8) & 0x00FF;
  tHooNoL = (tAnamID & 0x00FF);
  
  tHeightHouse   = (U8)(((tHooNoH >> 4) & 0x0F) * 10);
  tHeightHouse  += (U8)(tHooNoH & 0x0F);
  tWidthHouse  = (U8)(((tHooNoL >> 4) & 0x0F) * 10);
  tWidthHouse += (U8)(tHooNoL & 0x0F);
  tHouseID = (U8)((tWidthHouse) + ((tHeightHouse - 1) * gLobbyHandle.MaxHo));

  return tHouseID;
  
}



/// @brief ���� �������� ��ȯ 
/// @param tId ID����
/// @param tCmd Command
/// @param tLobbyID Lobby ID
/// @return true = ���� \n false = ����
U8 Anam_TransmitProtocolHouse(U8 tId, U8 tCmd, U8 tLobbyID )
{
  U8 tData[ANAM_PROTOCOL_BYTE_SIZE];
  U16 tAnamID;

  tData[0] = ANAM_GUARD_CODE_MASTER_TO_HOME;

  switch( tCmd )
  {
    case GR_CALL_ON:
      tData[1] = ANAM_FUNC_CODE_GUARD_CALL;
      break;
    case GR_CALL_OFF:
      tData[1] = ANAM_FUNC_CODE_GUARD_OFF;
      break;
    case GR_ON_HOOK:
      tData[1] = ANAM_FUNC_CODE_GUARD_OFF;
      break;
    case GR_OFF_HOOK:
      tData[1] = ANAM_FUNC_CODE_GUARD_TALK;
      break;
    case RG_CALL_OK:
      tData[1] = ANAM_FUNC_CODE_GUARD_CALL_OK;
      break;
    case LR_CALL_ON:
      tData[1] = ANAM_FUNC_CODE_LOBBY_CALL;
      break;
    case LR_CALL_OFF:
      tData[1] = ANAM_FUNC_CODE_LOBBY_OFF;
      break;
    case LR_ON_HOOK:
      tData[1] = ANAM_FUNC_CODE_LOBBY_OFF;
      break;
    default:
      return false;
  }
  
  tData[2] = tLobbyID;

  tAnamID = Anam_DongToAnamDong( gLobbyHandle.DongNumber );
  tData[3] = tAnamID >> 8;
  tData[4] = tAnamID & 0x00FF;

  tAnamID = Anam_HdIdToAnamID( tId );
  tData[5] = tAnamID >> 8;
  tData[6] = tAnamID & 0x00FF;
  
  Anam_TransmitData( tData );
  return true;
  
}


/// @brief ���� �������� ��ȯ ����κ�
/// @param tLobbyID Lobby ID
/// @param tCmd Command
/// @param tPath ��ȭ ä��
/// @param tDongNoH ����ȣ ���� ����Ʈ
/// @param tDongNoL ����ȣ ���� ����Ʈ
/// @param tHooNoH  ȣ��ȣ ���� ����Ʈ
/// @param tHooNoL  ȣ��ȣ ���� ����Ʈ
/// @return void
void Anam_TransmitProtocolLobby(U8 tLobbyID, U8 tCmd, U8 tPath, U8  tDongNoH, U8 tDongNoL, U8 tHooNoH, U8 tHooNoL )
{
  U8 x;

  U8 bTxProtocol[ANAM_PROTOCOL_BYTE_SIZE];

  bTxProtocol[0] = tLobbyID;
  bTxProtocol[1] = tCmd;
  bTxProtocol[2] = tPath;
  bTxProtocol[3] = tDongNoH;
  bTxProtocol[4] = tDongNoL;
  bTxProtocol[5] = tHooNoH;
  bTxProtocol[6] = tHooNoL;
  Anam_TransmitData( bTxProtocol );
  
}



/// @brief ���� �������� ��ȯ 
/// @param void void
/// @return true = ���� \n false = ����
U8 Anam_ReceiveProtocolHouse(void)
{
  typeProtocolData tProtocolData;
  U8 tID;
  U16 tAnamID;

/*
  tAnamID = mAnamProtocol.ByteData[3];
  tAnamID <<= 8;
  tAnamID += mAnamProtocol.ByteData[4];
  tID = Anam_AnamDongToDong( tAnamID );
  
  if ( tID != gLobbyHandle.DongNumber ) return false;
*/
  
  tAnamID = mAnamProtocol.ByteData[5];
  tAnamID <<= 8;
  tAnamID += mAnamProtocol.ByteData[6];
  
  tID = Anam_AnamIdToHdId( tAnamID );
    
  tProtocolData.DeviceID = tID;

  tProtocolData.DongNoH_LobbyID = mAnamProtocol.ByteData[2];
  tProtocolData.ReceiveLine = RECEIVE_LINE_HOUSE;


  switch( mAnamProtocol.ByteData[1] )
  {
    case ANAM_FUNC_CODE_GUARD_CALL:
      tProtocolData.Cmd = RG_CALL_ON;
      tProtocolData.Path = 2;
      break;
    case ANAM_FUNC_CODE_GUARD_CALL_OK:
      tProtocolData.Cmd = GR_CALL_OK;
      tProtocolData.Path = 2;
      break;
    case ANAM_FUNC_CODE_GUARD_TALK:
      tProtocolData.Cmd = RG_OFF_HOOK;
      tProtocolData.Path = 2;
      break;
    case ANAM_FUNC_CODE_GUARD_OFF:
      tProtocolData.Cmd = RG_CALL_OFF;
      tProtocolData.Path = 2;
      SendMsgReceiveProtocol ( (U8*)&tProtocolData );
      tProtocolData.Cmd = RG_ON_HOOK;
      tProtocolData.Path = 2;
      break;
    case ANAM_FUNC_CODE_LOBBY_CALL_OK:
      tProtocolData.Cmd = LR_CALL_OK;
      tProtocolData.Path = 1;
      break;
    case ANAM_FUNC_CODE_LOBBY_TALK:
      tProtocolData.Cmd = RL_OFF_HOOK;
      tProtocolData.Path = 1;
      break;
    case ANAM_FUNC_CODE_LOBBY_OFF:
      tProtocolData.Cmd = RL_ON_HOOK;
      tProtocolData.Path = 1;
      break;
    case ANAM_FUNC_CODE_DOOR_OPEN:
      tProtocolData.Cmd = RL_DOPEN;
      tProtocolData.Path = 0;
      break;

    default:
      return false;
  }

  SendMsgReceiveProtocol ( (U8*)&tProtocolData );


  return true;
}



/// @brief ���� �������� ��ȯ �κ�
/// @param void void
/// @return true = ���� \n false = ����
U8 Anam_ReceiveProtocolLobby(void)
{
  typeProtocolData tProtocolData;

  tProtocolData.DeviceID  = (mAnamProtocol.ByteData[0] & 0x7F );        ///< Device ID
  tProtocolData.Cmd       = mAnamProtocol.ByteData[1];             ///< Command
  tProtocolData.Path      = mAnamProtocol.ByteData[2];            ///< ��ȭ Path
  tProtocolData.DongNoH_LobbyID = mAnamProtocol.ByteData[3]; ///< Dong High or LobbyID
  tProtocolData.DongNoL   = mAnamProtocol.ByteData[4];         ///< Dong Low
  tProtocolData.HooNoH    = mAnamProtocol.ByteData[5];          ///< Hoo High
  tProtocolData.HooNoL    = mAnamProtocol.ByteData[6];          ///< Hoo Low
  tProtocolData.Eof       = 0xEE;             ///< EOF
  tProtocolData.ReceiveLine = RECEIVE_LINE_LOBBY;     ///< ���ŵ� ��� ����

  SendMsgReceiveProtocol ( (U8*)&tProtocolData );


  return true;
}

/// @brief ���� �������� ���� �Լ�
/// @param void
/// @return true = ���� \n false = ����
U8 Anam_ReceiveProtocol( void )
{
  U8 tRxPortData;
  U8 tRxBitData;

  if ( mAnamProtocol.Status > ANAM_STATUS_RX_DATA ) return false;

  tRxPortData = ANAM_PORT_RX;

  if ( mAnamProtocol.CountTimer++ == ANAM_RX_TIME_CNT_MAX )
  {
    mAnamProtocol.Status = ANAM_STATUS_STANDBY;
    mAnamProtocol.CountTimer--;
  }
  
  if ( mAnamProtocol.RxOldBit != tRxPortData )
  {
    mAnamProtocol.RxOldBit = tRxPortData;
    
    tRxBitData = Anam_RxDataCntTable[mAnamProtocol.CountTimer];

    if ( mAnamProtocol.Status == ANAM_STATUS_RX_DATA )
    {
      if ( tRxBitData == ANAM_RX_PORTDATA_ERROR )
      {
        mAnamProtocol.Status = ANAM_STATUS_STANDBY;
      }
      else
      {
        mAnamProtocol.BitData[mAnamProtocol.BitCount] = tRxBitData;
        mAnamProtocol.BitCount++;
        
        if( mAnamProtocol.BitCount == ANAM_PROTOCOL_BIT_SIZE )
        {
          mAnamProtocol.Status = ANAM_STATUS_STANDBY;
          Anam_DataBitToByte();
          if ( mAnamProtocol.ByteData[0] == ANAM_GUARD_CODE_HOME_TO_MASTER )
          {
            Anam_ReceiveProtocolHouse();
          }
          else
          {
            Anam_ReceiveProtocolLobby();
          }
          
        }
      }
    }
    else
    {
      if( tRxBitData == ANAM_RX_PORTDATA_START )
      {
        mAnamProtocol.Status = ANAM_STATUS_RX_DATA;
        mAnamProtocol.BitCount = 0;
      }      
    }
    mAnamProtocol.CountTimer = 0;
  }
  
  return true;
}



/// @brief ���� �������� �۽Ž� �Լ�
/// @param void
/// @return true = ���� \n false = ����
U8 Anam_TransmitProtocol(void)
{
  
  switch( mAnamProtocol.Status )
  {
    case ANAM_STATUS_TX_DELAY:
      ANAM_PORT_TX    = SET;
      ANAM_PORT_CNTR  = SET;
      mAnamProtocol.CountTimer = ANAM_PULSE_TIME_START * 3;
      mAnamProtocol.Status = ANAM_STATUS_TX_START;
      break;
    case ANAM_STATUS_TX_START:
      if ( mAnamProtocol.CountTimer-- != 0 ) return false;  
      ANAM_PORT_TX = CLEAR;
      mAnamProtocol.CountTimer = (ANAM_PULSE_TIME_START - 1);
      mAnamProtocol.BitCount = 0;
      mAnamProtocol.Status = ANAM_STATUS_TX_DATA;
      break;
    case ANAM_STATUS_TX_DATA:
      if ( mAnamProtocol.CountTimer-- != 0 ) return false;  
      if ( mAnamProtocol.BitData[mAnamProtocol.BitCount] )
      {
        ANAM_PORT_TX ^= SET;
        mAnamProtocol.CountTimer = (ANAM_PULSE_TIME_BIT_HIGH - 1 );
      }
      else
      {
        ANAM_PORT_TX ^= SET;
        mAnamProtocol.CountTimer = (ANAM_PULSE_TIME_BIT_LOW - 1 );
      }
      mAnamProtocol.BitCount++;
      if ( mAnamProtocol.BitCount > ANAM_PROTOCOL_BIT_SIZE )
      {
        ANAM_PORT_TX = SET;
        mAnamProtocol.CountTimer = (ANAM_PULSE_TIME_BIT_LOW - 1 );
        mAnamProtocol.Status = ANAM_STATUS_TX_END;
      }
      break;
    case ANAM_STATUS_TX_END:
      if ( mAnamProtocol.CountTimer-- != 0 ) return false; 
      ANAM_PORT_CNTR = CLEAR;
      mAnamProtocol.Status = ANAM_STATUS_STANDBY;
      break;
    default:
      return true;
      break;
  }
  return true;
}


/// @brief ���� �������� �۽� �Լ�, �ǵ���Ÿ ����
/// @param tData ���� ������
/// @return void
void Anam_TransmitData( U8 *tData )
{
  U8 i;
  while ( mAnamProtocol.Status != ANAM_STATUS_STANDBY )
  {
    ;
  }

  for ( i = 0 ; i < ANAM_PROTOCOL_BYTE_SIZE; i++ )
  {
     mAnamProtocol.ByteData[i] = tData[i];
  }
  Anam_DataByteToBit();
  mAnamProtocol.Status = ANAM_STATUS_TX_DELAY;
  
}


/// @brief 100us ���� ���� �Ǹ� �ۼ����� ���� �ϴ� �Լ�
/// @param void
/// @return void
void Anam_TimerInterrutp100us(void)
{
  if( Anam_ReceiveProtocol() == false )
  {
    Anam_TransmitProtocol();
  }
}





