
/// @file   Lib_I2C.c
/// @date   2012/03/16
/// @author jinhwa Lee
/// @brief  I2C 통신 라이브러리


#include "main.h"
#include "Lib_I2C.h"

#define I2C_Delay(tCount, tDelay) { tCount = tDelay;while(tCount--); }
#define I2C_WriteDelay    5

void I2C_START(void)
{
  U8 tCount;
  
  I2C_DATA_PIN = SET;
  I2C_DIR_DATA_PIN = I2C_PIN_DIR_OUTPUT;
  I2C_Delay(tCount,I2C_CLK_DELAY);
  
  I2C_CLK_PIN = SET;
  I2C_Delay(tCount,I2C_CLK_DELAY);
  
  I2C_DATA_PIN = CLEAR;
  I2C_Delay(tCount,I2C_CLK_DELAY);
  
  I2C_CLK_PIN = CLEAR;
  I2C_Delay(tCount,I2C_CLK_DELAY);

}


void I2C_STOP(void) // SCL을 high일때 SDA를 low -> high로 해줌
{
  U8 tCount;
  
  I2C_CLK_PIN = CLEAR;
  I2C_Delay(tCount,I2C_CLK_DELAY);
  
  I2C_DATA_PIN = CLEAR;
  I2C_DIR_DATA_PIN = I2C_PIN_DIR_OUTPUT;
  I2C_Delay(tCount,I2C_CLK_DELAY);
  
  I2C_CLK_PIN = SET;
  I2C_Delay(tCount,I2C_CLK_DELAY);
  
  I2C_DATA_PIN = SET;
  I2C_Delay(tCount,I2C_CLK_DELAY);

}


void I2C_WRITE(U8 Data)
{
  U8 i;
  U8 tCount;
  
  wdt_reset();
  I2C_Delay(tCount,I2C_CLK_DELAY);

  for (i=0; i<8; i++)
  {
    I2C_Delay(tCount,I2C_CLK_DELAY);
    if( Data & 0x80 ){                    // MSB First 
      I2C_DATA_PIN = SET;
    }
    else{
      I2C_DATA_PIN = CLEAR;        // Hold Time>4us
    }
    
    I2C_DIR_DATA_PIN = I2C_PIN_DIR_OUTPUT;
    I2C_Delay(tCount,I2C_CLK_DELAY);

    if( Data & 0x80 ){                    // MSB First 
      I2C_DATA_PIN = SET;
    }
    else{
      I2C_DATA_PIN = CLEAR;        // Hold Time>4us
    }
    
    I2C_CLK_PIN = SET;          // SCLK is High>4.7us
    I2C_Delay(tCount,I2C_CLK_DELAY);
    
    I2C_CLK_PIN = CLEAR;        // SCLK is Low> 4us
    I2C_Delay(tCount,I2C_CLK_DELAY);
    Data<<=1;
  }

}


U8 I2C_READ(void)
{
  U8 i=0;
  U8 Data=0;
  U8 tCount;
  
  wdt_reset();

  I2C_DIR_DATA_PIN = I2C_PIN_DIR_INPUT;
  I2C_Delay(tCount,I2C_CLK_DELAY);

  for(i=0;i<8;i++)
  {
    I2C_Delay(tCount,I2C_CLK_DELAY);
    
    I2C_CLK_PIN = SET;
    I2C_Delay(tCount,I2C_CLK_DELAY);
    
    Data<<=1;
    if(I2C_DATA_PIN == SET){
      Data |= 0x01;
    }
    
    I2C_CLK_PIN = CLEAR;  
  }

  return Data;
}


U8 I2C_ACK_DETECT(void)                     // Slave Acknowledge Check
{
  U8 tAck;
  U8 tCount;

  I2C_DIR_DATA_PIN = I2C_PIN_DIR_INPUT;
  I2C_Delay(tCount,I2C_CLK_DELAY);
  
  I2C_CLK_PIN = SET;
  tAck = I2C_DATA_PIN;
  I2C_Delay(tCount,I2C_CLK_DELAY);
  
  I2C_CLK_PIN = CLEAR;
  I2C_Delay(tCount,I2C_CLK_DELAY);
  
  return tAck;
}


void I2C_ACK(void)                     // Slave Acknowledge Check
{
  U8 tAck;
  U8 tCount;

  I2C_DATA_PIN = CLEAR;
  I2C_DIR_DATA_PIN = I2C_PIN_DIR_OUTPUT;
  I2C_Delay(tCount,I2C_CLK_DELAY);
  
  I2C_CLK_PIN = SET;
  I2C_Delay(tCount,I2C_CLK_DELAY);
  
  I2C_CLK_PIN = CLEAR;
  I2C_Delay(tCount,I2C_CLK_DELAY);
  
}


void I2C_NACK(void)                     // Slave Acknowledge Check
{
  U8 tAck;
  U8 tCount;

  I2C_DATA_PIN = SET;
  I2C_DIR_DATA_PIN = I2C_PIN_DIR_OUTPUT;
  I2C_Delay(tCount,I2C_CLK_DELAY);

  I2C_CLK_PIN = SET;
  I2C_Delay(tCount,I2C_CLK_DELAY);

  I2C_CLK_PIN = CLEAR;
  I2C_Delay(tCount,I2C_CLK_DELAY);

}

U8 I2C_WriteRawData(U8 tDeviceID, U8 *tData, U8 tLength )
{
  U8 i;
  
  I2C_START();
  
  I2C_WRITE( tDeviceID & 0xFE );
  if(I2C_ACK_DETECT()){
    I2C_STOP();
    return false;
  }

  for ( i = 0 ; i < tLength ; i++ )
  {
    I2C_WRITE(*tData);
    if(I2C_ACK_DETECT()){
      I2C_STOP();
      return false;
    }
    tData++;
  }
  
  I2C_STOP();
 
  return true;
}

U8 I2C_ReadRawData(U8 tDeviceID, U8 *tData, U8 tLength )
{
  U8 i;

  switch ( tLength )
  {
    case 0:
      return false;
    case 1:
      I2C_START();
      I2C_WRITE(tDeviceID | 0x01);
      if(I2C_ACK_DETECT()){
        I2C_STOP();
        return false;
      }
      *tData = I2C_READ();
      I2C_NACK();
      I2C_STOP();
      break;
    default:
      I2C_START();
      I2C_WRITE(tDeviceID | 0x01);
      if(I2C_ACK_DETECT()){
        I2C_STOP();
        return false;
      }
      for ( i = 0 ; i < (tLength - 1) ; i++ )
      {
        *tData = I2C_READ();
        tData++;
        I2C_ACK();
      }
      *tData = I2C_READ();
      I2C_NACK();
      I2C_STOP();
      break;
  }
 
  return true;
}

///////////////////////////////////////////////////////////////////////////////



U8 EEPROM_WriteByte( U16 tAddress, U8 tData )
{
  U16 tCount;
  U16 tWhileCnt;
  U8 tTemp[3];
  U8 tReadData;
  
#ifdef I2C_EEPROM_SIZE_16K_OVER
  tTemp[0] = tAddress >> 8;
  tTemp[1] = tAddress & 0xFF;
  tTemp[2] = tData;
  if( I2C_WriteRawData(I2C_EEPROM_DEVICE_ID, tTemp, 3 ) == false ) return false;
#else
  tTemp[0] = (tAddress >> 8) & 0x07;
  tTemp[1] = tAddress & 0xFF;
  tTemp[2] = tData;
  if( I2C_WriteRawData((I2C_EEPROM_DEVICE_ID | (tTemp[0] << 1)), &tTemp[1], 2 ) == false ) return false;
#endif  

/*
  tWhileCnt = 0xFF;
  while(tWhileCnt--)
  {
    wdt_reset();

    tReadData = (tData ^ 0xFF);

    if ( EEPROM_ReadBlock(tAddress, &tReadData, 1 ) )
    {
      if( tReadData == tData )
      {
        return true;
      }
    }
  }
  
  return false;
*/

  delay_ms(I2C_WriteDelay);

  tReadData = (tData ^ 0xFF);

  if ( EEPROM_ReadBlock(tAddress, &tReadData, 1 ) )
  {
    if( tReadData == tData )
    {
      delay_ms(I2C_WriteDelay);
      return true;
    }
  }  
  delay_ms(I2C_WriteDelay);
  return false;
    
}


U8 EEPROM_WriteBlcok( U16 tAddress, U8 *tWriteData, U8 tLength )
{
  U8 i;
  U16 tCount;
  if ( tLength == 0 || tLength > 32 ) return false;
  
  for ( i = 0 ; i < tLength ; i++ )
  {
    if(EEPROM_WriteByte( tAddress, *tWriteData ) == false ) return false;
    tAddress++;
    tWriteData++;
  }

  return true;
}


U8 EEPROM_ReadBlock( U16 tAddress, U8 *tReadData, U8 tLength )
{
  U8 tAddr[2];
  U8 tReturnData;

  if ( tLength == 0 ) return false;

#ifdef I2C_EEPROM_SIZE_16K_OVER
  tAddr[0] = tAddress >> 8;
  tAddr[1] = tAddress & 0xFF;
  if ( I2C_WriteRawData( I2C_EEPROM_DEVICE_ID, tAddr, 2 ) == false ) return false;
#else
  tAddr[0] = (tAddress >> 8) & 0x07;
  tAddr[1] = tAddress & 0xFF;
  if ( I2C_WriteRawData( (I2C_EEPROM_DEVICE_ID | (tAddr[0] << 1)) , &tAddr[1], 1 ) == false ) return false;
#endif

  delay_ms(I2C_WriteDelay);

  return I2C_ReadRawData( I2C_EEPROM_DEVICE_ID, tReadData, tLength );
        
}

////////////////////////////////////////////////////////////////////////////////

U8 SpecialVal_WriteByte ( U16 tAddress, U8 tData )
{
  U8 tWriteData[2];
  tWriteData[0] = tData;
  tWriteData[1] = ( tData ^ SPECIALVAL_EXOR_VALUE );
  return EEPROM_WriteBlcok( tAddress, tWriteData, 2 );
}

U8 SpecialVal_ReadByte ( U16 tAddress, U8 *tData )
{
  U8 tReadData[2]=0;
  EEPROM_ReadBlock( tAddress, tReadData, 2 );
  if ( tReadData[0] != (tReadData[1] ^ SPECIALVAL_EXOR_VALUE) ) return false;
  *tData = tReadData[0];
  return true;
}

U8 SpecialVal_WriteBlock(U16 tAddress, U8 *tData, U8 tLength)
{
  while( tLength-- )
  {
    if ( !SpecialVal_WriteByte ( tAddress, *tData ) ) return false;
    tData++;
    tAddress+=2;
  }
  return true;
}

U8 SpecialVal_ReadBlock(U16 tAddress, U8 *tData, U8 tLength)
{
  while(tLength--)
  {
    if( !SpecialVal_ReadByte ( tAddress, tData )) return false;
    tData++;
    tAddress+=2;
  }
  return true;
}





