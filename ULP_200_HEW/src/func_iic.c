
/// @file   func_iic.c
/// @date   2012/02/14
/// @author jinhwa Lee
/// @brief  I2C ���� ����

#include "main.h"


/// @brief �κ� ��鷯 �ʱ�ȭ �Լ� 
/// @param void
/// @return void

U8 InitializeLobbyHandle(void)
{
  
  U8 tDoCnt;

// ���� �ʱ�ȭ �ϱ� TEST �Ҷ� ���
/*
  U16 j;
  U8 tData[100];
  for( j = 0; j < EEPROM_END_ADDR ; j++)
  {
    EEPROM_WriteByte(  j, 0xFF );
  } 

  //LobbyHandleFactoryReset();//////////////////////////////////////////////

  for( j = 0; j < 100 ; j++)
  {
    EEPROM_ReadBlock( j, &tData[j], 1 );
  } 
*/

  tDoCnt = 10;
  while(tDoCnt--)
  {
    if(SpecialVal_ReadBlock( LOBBY_HANDLE_ADDR,(U8*)&gLobbyHandle.DongNumber , LOBBY_HANDLE_SIZE))
    {
      if ( gLobbyHandle.B_Lobby_Mark == 0xBBBB ) return true;
      else return false;
    }
  }
  
  return false;

}


U8 LobbyHandleErrorRamReset(void)
{
  gLobbyHandle.DongNumber = 101;
  gLobbyHandle.LobbyID = MASTER_LOBBY_ID;
  gLobbyHandle.MaxHo = 8;
  gLobbyHandle.Password = 1234;
  gLobbyHandle.Password_ErrCount = 5;
  gLobbyHandle.GuardID = 1;
  gLobbyHandle.OpenTimeDoor = 5;
  gLobbyHandle.AudioPathHouse[0] = true;
  gLobbyHandle.AudioPathHouse[1] = true;
  gLobbyHandle.AudioPathLobby[0] = true;
  gLobbyHandle.AudioPathLobby[1] = true;
  gLobbyHandle.AudioPathGuard[0] = true;
  gLobbyHandle.AudioPathGuard[1] = false;
  gLobbyHandle.BeepSoundMute      = 0;
  gLobbyHandle.Protocol           = 1;
  gLobbyHandle.PacketMonitoring   = 0;
  gLobbyHandle.ManagerPassword    = 5482;
  gLobbyHandle.B_Lobby_Mark       = 0xBBBB;

}


/// @brief �⺻���� EEPROM�� ���� �Լ� Initialize
/// @param void
/// @return true = ���� \n false = ����
U8 InitializeFactoryEEPROM(void)
{
  U8 i; 
  
  LobbyHandleErrorRamReset();
  
  if( SpecialVal_WriteBlock( LOBBY_HANDLE_ADDR, (U8*)&gLobbyHandle.DongNumber, LOBBY_HANDLE_SIZE ))
  {
    return true;    
  }
  else
  {
    return false;
  }
  
}

U8 LobbyHandleReroad(void)
{
  U8 tDoCnt = 10;
  typeLobbyHandler tLobbyHandle;

  while(tDoCnt--)
  {
    if(SpecialVal_ReadBlock(LOBBY_HANDLE_ADDR, (U8*)&gLobbyHandle.DongNumber, LOBBY_HANDLE_SIZE))
    {
      if( memcmp( &tLobbyHandle.DongNumber, (U8*)&gLobbyHandle.DongNumber, LOBBY_HANDLE_SIZE ) == NULL )
      {
        return true;
      }
      else
      {
        memcpy( &gLobbyHandle.DongNumber, (U8*)&gLobbyHandle.DongNumber, LOBBY_HANDLE_SIZE );
        return true;
      }
    }
  }
  return false;
}


///////////////////////////////////////////////////////////////////////////////

/*

void Debug_eepromPrint(void)
{
  U16 tAddr;
  U8 tData;

  Tx485EnableFunc();
  for( tAddr = 0 ; tAddr < 30 ; tAddr++ )
  {
    EEPROM_ReadBlock(tAddr,&tData,1);
    TxBufferRegFuc(tData);
  }
  Tx485DisableFunc();

}

*/


/// @brief RF Card Write
/// @param data RFID ���� ���޵� ������
/// @return true = ���� \n false = ����
void wr_RFCard( U8 data )
{
  I2C_WriteRawData(0xE4, &data, 1 );
  delay_ms(30);             // writing delay time 10ms 
}


/// @brief RF Card Read
/// @param[out] ���ŵ� RF������ �����
/// @return true = ���� \n false = ����
U8 rd_RFCard( U8 *ReadData )
{
  return I2C_ReadRawData(0xE4, ReadData, 5);        
}



//////////////////////////////////////////////////////////////////////////////////

U8 PasswordHomeAllDel ( void )
{
  U16 tAddr;
  U16 tCnt;
  U8 tData;
  
  U16 tFndCnt;

  tData = 0xFF;
  tAddr = HOME_PASS_START_ADDR;
   
  for( tCnt = 0 ; tCnt < ((U16)HOME_PASS_MAX_COUNT * (U16)HOME_PASS_SIZE ) ; tCnt++ )
  {
    EEPROM_WriteBlcok( tAddr, &tData, 1 );
    tAddr++;

    tFndCnt = tCnt >> 5;

    if ( (tFndCnt % 2) == 0 ) SettingValueHexFnd_Page1(0x8888);
    else SettingValueHexFnd_CLEAR();
    
  }
  
  return true;
}

U8 PasswordWriteHome( U8 tHooH, U8 tHooL,U8 *tPassword )
{
  U16 tAddr;
  U8 tCnt;
  U8 tData[10];

  tAddr = HOME_PASS_START_ADDR;
  for( tCnt = 0 ; tCnt < HOME_PASS_MAX_COUNT ; tCnt++ )
  {
    EEPROM_ReadBlock( tAddr , tData, 2);
    if ( (tData[0] == tHooH) && (tData[1] == tHooL))
    {
      memcpy( &tData[2], tPassword, 2);
      EEPROM_WriteBlcok( tAddr, tData, HOME_PASS_SIZE );
      return true;
    }
    else if ( (tData[0] == 0xFF) && (tData[1] == 0xFF))
    {
      tData[0] = tHooH;
      tData[1] = tHooL;
      memcpy( &tData[2], tPassword, 2);
      EEPROM_WriteBlcok( tAddr, tData, HOME_PASS_SIZE );
      return true;
    }
    tAddr += HOME_PASS_SIZE;
  }
  return false;
}


U8 PasswordHomeCompare( U8 tHooH, U8 tHooL, U8 *tPassword )
{
  U16 tAddr;
  U8 tCnt;
  U8 tData[10];

  tAddr = HOME_PASS_START_ADDR;
  for( tCnt = 0 ; tCnt < HOME_PASS_MAX_COUNT ; tCnt++ )
  {
    EEPROM_ReadBlock( tAddr , tData, 2 );
    if ( (tData[0] == tHooH) && (tData[1] == tHooL))
    {
      EEPROM_ReadBlock( tAddr + 2 , &tData[2], 2);
      if ( (tData[2] == tPassword[0]) && (tData[3] == tPassword[1]))
      {
        return true;
      }
    }
    else if ( (tData[0] == 0xFF) && (tData[1] == 0xFF))
    {
      return false;
    }
    tAddr += HOME_PASS_SIZE;
  }
  return false;
}



U8 PasswordReadHome( U8 tHooH, U8 tHooL, U8 *tPassword )
{
  U16 tAddr;
  U8 tCnt;
  U8 tData[10];

  tAddr = HOME_PASS_START_ADDR;
  for( tCnt = 0 ; tCnt < HOME_PASS_MAX_COUNT ; tCnt++ )
  {
    EEPROM_ReadBlock( tAddr , tData, 2 );
    if ( (tData[0] == tHooH) && (tData[1] == tHooL))
    {
      EEPROM_ReadBlock( tAddr + 2 , tPassword, 2);
      return true;
    }
    else if ( (tData[0] == 0xFF) && (tData[1] == 0xFF))
    {
      return false;
    }
    tAddr += HOME_PASS_SIZE;
  }
  return false;
}


