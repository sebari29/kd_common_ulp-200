
/// @file   dev_iic.c
/// @date   2012/02/14
/// @author jinhwa Lee
/// @brief  I2C 통신 디바이스 드라이브 파일

#include "dev_iic.h"


/// @brief      I2C Start
/// @param      void
/// @return     void
void __I2C_START(void)
{
  EEP_DATA = SET;
  EEP_DATA_DIR = EEP_DATA_OUTPUT;
  //asm("nop");
  asm("nop");
  EEP_CLK = SET;
  asm("nop");
  asm("nop");
  EEP_DATA = CLEAR;
  asm("nop");
  asm("nop");
  EEP_CLK = CLEAR;
  asm("nop");
  asm("nop");
  //EEP_DATA_DIR = EEP_DATA_INPUT; 
}


/// @brief      I2C Stop
/// @param      void
/// @return     void
void __I2C_STOP(void) // SCL을 high일때 SDA를 low -> high로 해줌
{
  EEP_CLK = CLEAR;
  asm("nop");
  asm("nop");
  EEP_DATA = CLEAR;
  EEP_DATA_DIR = EEP_DATA_OUTPUT;
  //asm("nop");
  asm("nop");
  EEP_CLK = SET;
  asm("nop");
  asm("nop");
  EEP_DATA = SET;
  asm("nop");
  asm("nop");
  //EEP_DATA_DIR = EEP_DATA_INPUT; 
}


/// @brief      I2C Write
/// @param      Data 스여질 데이터
/// @return     void
void __I2C_P2S(U8 Data)
{
  U8 i;
  asm("nop");
  asm("nop");
  
  for (i=0; i<8; i++)
  {
    asm("nop");
    if(Data & 0x80){                    // MSB First 
      EEP_DATA = SET;
    }
    else{
      EEP_DATA = CLEAR;        // Hold Time>4us
    }
    EEP_DATA_DIR = EEP_DATA_OUTPUT;
    //asm("nop");
    asm("nop");
    EEP_CLK = SET;          // SCLK is High>4.7us
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    EEP_CLK = CLEAR;        // SCLK is Low> 4us
    asm("nop");
    asm("nop");
    Data<<=1;
  }
  //EEP_DATA_DIR = EEP_DATA_INPUT; 
}


/// @brief      I2C Read
/// @param      void
/// @return     void
U8 __I2C_S2P(void)
{
  U8 i=0;
  U8 Data=0;

  EEP_DATA_DIR = EEP_DATA_INPUT;          // P1.4 : direction change (eep_data input mode)
  asm("nop");
  asm("nop");
  asm("nop");
  asm("nop");

  for(i=0;i<8;i++)
  {
    asm("nop");
    asm("nop");
    EEP_CLK = SET;
    asm("nop");
    asm("nop");
    Data<<=1;
    if(EEP_DATA == SET){
      Data |= 0x01;
    }
    //asm("nop");
    //asm("nop");
    EEP_CLK = CLEAR;  
  }

  //EEP_DATA_DIR = EEP_DATA_OUTPUT;          // P1.4 : direction change (eep_data outmode mode)

  return Data;
}


/// @brief      I2C Read
/// @param      void
/// @return     Ack = 0, Nack = 1
U8 __ACKDETECT(void)                     // Slave Acknowledge Check
{
  U8 tAck;

  EEP_DATA_DIR = EEP_DATA_INPUT;
  //asm("nop");
  asm("nop");
  EEP_CLK = SET;
  tAck = EEP_DATA;
  asm("nop");
  asm("nop");
  EEP_CLK = CLEAR;
  asm("nop");
  asm("nop");
  //EEP_DATA_DIR = EEP_DATA_OUTPUT;
  return tAck;
}


/// @brief      I2C Ack Write
/// @param      void
/// @return     void
void __ACK(void)                     // Slave Acknowledge Check
{
  U8 tAck;

  EEP_DATA = CLEAR;
  EEP_DATA_DIR = EEP_DATA_OUTPUT;
  asm("nop");
  asm("nop");
  EEP_CLK = SET;
  asm("nop");
  asm("nop");
  EEP_CLK = CLEAR;
  asm("nop");
  asm("nop");
  //EEP_DATA_DIR = EEP_DATA_INPUT;
  
}


/// @brief      I2C Nack Write
/// @param      void
/// @return     void
void __NACK(void)                     // Slave Acknowledge Check
{
  U8 tAck;

  EEP_DATA = SET;
  EEP_DATA_DIR = EEP_DATA_OUTPUT;
  asm("nop");
  asm("nop");
  EEP_CLK = SET;
  asm("nop");
  asm("nop");
  EEP_CLK = CLEAR;
  asm("nop");
  asm("nop");
  //EEP_DATA_DIR = EEP_DATA_INPUT;
  
}

