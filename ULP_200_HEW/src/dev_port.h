
/// @file   dev_port.h
/// @date   2012/02/14
/// @author jinhwa Lee
/// @brief  Port define 디바이스 드라이브 파일

#ifndef ___DEV_PORT_H___
#define ___DEV_PORT_H___

#include "dev_define.h"

#define __KEY_PORT p2
//#define __KEY_BACK_LIGHT p0_7

#define __DR_M_OPEN   p1_1
#define __CAM_PWR_ON  p1_2

#define __nMUX_EN   p1_3
#define __MUX_CH1A_SEL1 p6_0
#define __MUX_CH1A_SEL2 p6_1
#define __MUX_CH1A_SEL3 p8_3
#define __MUX_CH1B_SEL1 p8_4
#define __MUX_CH1B_SEL2 p8_5
#define __MUX_CH1B_SEL3 p8_6
#define __MUX_CH2A_SEL1 p1_6
#define __MUX_CH2A_SEL2 p1_7
#define __MUX_CH2A_SEL3 p5_0
#define __MUX_CH2B_SEL1 p1_0
#define __MUX_CH2B_SEL2 p4_3
#define __MUX_CH2B_SEL3 p4_4

//#define __nFIRE   p3_6
#define __RF_INT  p6_7

#define __CS_TX p1_4
#define __CS_RX p1_5

#define __RS485_GUARD_TX p6_3
#define __RS485_GUARD_RX p6_4
#define __RS485_GUARD_CNTR p6_2

#define __I2C_DATA p3_7
#define __I2C_CLK p3_5

#define __BEEP p3_4

#define __RS485_HOUSE_TX    p6_6
#define __RS485_HOUSE_RX    p4_5
#define __RS485_HOUSE_CNTR  p6_5

#define __EN_MC p5_1
#define __nMUTE_SPK p5_2


#define __HOUSE_CALL1 p0_7
#define __HOUSE_CALL2 p5_7
#define __SUB_CALL1 p5_3
#define __SUB_CALL2 p5_4
#define __MASTER_CALL1 p5_6
//#define __MASTER_CALL2 p5_7

#define __VIDEO_SEL1 p8_0
#define __VIDEO_SEL2 p8_1
#define __VIDEO_SEL3 p8_2

#define __DIPSW_ADC1 p1_0

//////////////// Direction //////////////////////////

#define __DIR_FND_SEG pd0
#define __DIR_FND_COM_CTRL1 pd3_0
#define __DIR_FND_COM_CTRL2 pd3_1
#define __DIR_FND_COM_CTRL3 pd3_2
#define __DIR_FND_COM_CTRL4 pd3_3

#define __DIR_KEY_PORT pd2
//#define __DIR_KEY_BACK_LIGHT pd0_7

#define __DIR_DR_M_OPEN pd1_1
#define __DIR_CAM_PWR_ON pd1_2

#define __DIR_nMUX_EN pd1_3
#define __DIR_MUX_CH1A_SEL1 pd6_0
#define __DIR_MUX_CH1A_SEL2 pd6_1
#define __DIR_MUX_CH1A_SEL3 pd8_3
#define __DIR_MUX_CH1B_SEL1 pd8_4
#define __DIR_MUX_CH1B_SEL2 pd8_5
#define __DIR_MUX_CH1B_SEL3 pd8_6
#define __DIR_MUX_CH2A_SEL1 pd1_6
#define __DIR_MUX_CH2A_SEL2 pd1_7
#define __DIR_MUX_CH2A_SEL3 pd5_0
#define __DIR_MUX_CH2B_SEL1 pd1_0//pd4_2
#define __DIR_MUX_CH2B_SEL2 pd4_3
#define __DIR_MUX_CH2B_SEL3 pd4_4

#define __DIR_nFIRE pd3_6
#define __DIR_RF_INT pd6_7

#define __DIR_CS_TX pd1_4
#define __DIR_CS_RX pd1_5

#define __DIR_RS485_GUARD_TX pd6_3
#define __DIR_RS485_GUARD_RX pd6_4
#define __DIR_RS485_GUARD_CNTR pd6_2

#define __DIR_I2C_DATA pd3_7
#define __DIR_I2C_CLK pd3_5

#define __DIR_BEEP pd3_4

#define __DIR_RS485_HOUSE_TX    pd6_6
#define __DIR_RS485_HOUSE_RX    pd4_5
#define __DIR_RS485_HOUSE_CNTR  pd6_5

#define __DIR_EN_MC pd5_1
#define __DIR_nMUTE_SPK pd5_2


#define __DIR_HOUSE_CALL1 pd0_7
#define __DIR_HOUSE_CALL2 pd5_7

#define __DIR_SUB_CALL1 pd5_3
#define __DIR_SUB_CALL2 pd5_4
#define __DIR_MASTER_CALL1 pd5_6
//#define __DIR_MASTER_CALL2 pd5_7

#define __DIR_VIDEO_SEL1 pd8_0
#define __DIR_VIDEO_SEL2 pd8_1
#define __DIR_VIDEO_SEL3 pd8_2

#define __DIR_DIPSW_ADC1 pd1_0
#define __DIR_DIPSW_ADC2 pd1_1
#define __DIR_DIPSW_ADC3 pd1_2
#define __DIR_DIPSW_ADC4 pd1_3

//////////////////////////////////////////////////////////

#define __FND_SEG_PORT p0
#define __FEN_COM_PORT p3

#endif  // #ifndef ___DEV_PORT_H___

