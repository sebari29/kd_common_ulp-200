
/// @file   func_port.c
/// @date   2012/02/14
/// @author jinhwa Lee
/// @brief  GPIO 관련 파일


#include "main.h"


/// @brief Port Initialize 함수
/// @param void
/// @return void
void InitializePort(void)
{
  
  I2C_DATA = SET;
  I2C_CLK = SET;
     /// initialize the mode registers ///
      // 0: Input    1: Output    //
  prc2 = SET;        // PD0 enable
  
  DIR_FND_SEG = 0xFF;
  DIR_FND_COM_CTRL1 = SET; 
  DIR_FND_COM_CTRL2 = SET; 
  DIR_FND_COM_CTRL3 = SET;
  DIR_FND_COM_CTRL4 = SET;

  DIR_KEY_PORT = 0x0F;
  pu05 = SET;
  
  //DIR_KEY_BACK_LIGHT  = SET;
      
  DIR_DR_M_OPEN = SET;
  DIR_CAM_PWR_ON = SET;
      
  DIR_nMUX_EN = SET;
  DIR_MUX_CH1A_SEL1 = SET;
  DIR_MUX_CH1A_SEL2 = SET;
  DIR_MUX_CH1A_SEL3 = SET;
  DIR_MUX_CH1B_SEL1 = SET;
  DIR_MUX_CH1B_SEL2 = SET;
  DIR_MUX_CH1B_SEL3 = SET;
  DIR_MUX_CH2A_SEL1 = SET;
  DIR_MUX_CH2A_SEL2 = SET;
  DIR_MUX_CH2A_SEL3 = SET;
  DIR_MUX_CH2B_SEL1 = SET;
  DIR_MUX_CH2B_SEL2 = SET;
  DIR_MUX_CH2B_SEL3 = SET;
      
  //DIR_nFIRE = CLEAR;
  DIR_RF_INT = CLEAR;
      
  DIR_CS_TX = SET;
  DIR_CS_RX = CLEAR;
      
  DIR_RS485_GUARD_TX = SET;
  DIR_RS485_GUARD_RX = CLEAR;
  DIR_RS485_GUARD_CNTR = SET;
      
  DIR_I2C_DATA = SET;
  DIR_I2C_CLK = SET;
      
  DIR_BEEP = SET;
      
  DIR_RS485_HOUSE_TX = SET;
  DIR_RS485_HOUSE_RX = CLEAR;
  DIR_RS485_HOUSE_CNTR = SET;
      
  DIR_EN_MC = SET;
  DIR_nMUTE_SPK = SET;

  DIR_HOUSE_CALL1 = SET;
  DIR_HOUSE_CALL2 = SET;
  DIR_SUB_CALL1 = SET;
  DIR_SUB_CALL2 = SET;
  DIR_MASTER_CALL1 = SET;
  //DIR_MASTER_CALL2 = SET;
      
  DIR_VIDEO_SEL1 = SET;
  DIR_VIDEO_SEL2 = SET;
  DIR_VIDEO_SEL3 = SET;
      
  //DIR_DIPSW_ADC1 = CLEAR;
  //DIR_DIPSW_ADC2 = CLEAR;
  //DIR_DIPSW_ADC3 = CLEAR;
  //DIR_DIPSW_ADC4 = CLEAR;

  prc2 = CLEAR;        // PD0 disable
  
  ///////////////////////////////////////////////////////////////////////////////

  //pu00 = SET;        /* P0_0 to P0_3 pull-up : TBEL_DET */
  //pu20 = SET;        /* P8_0 to P8_3 pull-up : BGLR1_S, BGLR2_S, BGLR3_S, BGLR4_S*/
  //pu21 = SET;        /* P8_4 to P8_6 pull-up : GAS_S, DBEL_DET, GBEL_DET*/
  //pu04 = SET;
  //pu05 = SET;
  //pu15 = SET;
  pur0 = 0xFF;
  pur1 = 0xFF;
  pur2 = 0xFF;

  KEY_PORT = 0x00;
  //KEY_BACK_LIGHT = CLEAR;

  nMUX_EN = CLEAR;
  
  MUX_CH1A_SEL1 = SET;
  MUX_CH1A_SEL2 = SET;
  MUX_CH1A_SEL3 = SET;
  MUX_CH1B_SEL1 = SET;
  MUX_CH1B_SEL2 = SET;
  MUX_CH1B_SEL3 = SET;
  MUX_CH2A_SEL1 = SET;
  MUX_CH2A_SEL2 = SET;
  MUX_CH2A_SEL3 = SET;
  MUX_CH2B_SEL1 = SET;
  MUX_CH2B_SEL2 = SET;
  MUX_CH2B_SEL3 = SET;

  RS485_GUARD_CNTR = CLEAR;
  RS485_HOUSE_CNTR = CLEAR;

  I2C_DATA = SET;
  I2C_CLK = SET;

  BEEP = SET;

  EN_MC = CLEAR;
  nMUTE_SPK = CLEAR;
  
  DR_M_OPEN = CLEAR;
  CAM_PWR_ON = CLEAR;
  
  HOUSE_CALL1 = CLEAR;
  HOUSE_CALL2 = CLEAR;
  SUB_CALL1 = CLEAR;
  SUB_CALL2 = CLEAR;
  GUARD_CALL = CLEAR;
  //MASTER_CALL2 = CLEAR;

  VIDEO_SEL1 = CLEAR;
  VIDEO_SEL2 = CLEAR;
  VIDEO_SEL3 = CLEAR;
                   
}

