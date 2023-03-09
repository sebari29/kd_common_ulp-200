
/// @file   func_fnd.h
/// @date   2012/02/14
/// @author jinhwa Lee
/// @brief  FND Display 관련 파일

#ifndef ___FUNC_FND_H___
#define ___FUNC_FND_H___

#include "dev_port.h"

#define FND_SEG_PORT __FND_SEG_PORT
#define FEN_COM_PORT __FEN_COM_PORT

#define  FND_0      0b00111111
#define  FND_1      0b00000110
#define  FND_2      0b01011011
#define  FND_3      0b01001111
#define  FND_4      0b01100110
#define  FND_5      0b01101101
#define  FND_6      0b01111101
#define  FND_7      0b00000111
#define  FND_8      0b01111111
#define  FND_9      0b01101111
#define  FND_A      0b01110111
#define  FND_B      0b01111100
#define  FND_C      0b00111001
#define  FND_D      0b01011110
#define  FND_E      0b01111001
#define  FND_F      0b01110001//0b01110001
#define  FND_BAR    0b00001000
#define  FND_CBAR   0b01000000
#define  FND_NUL    0b00000000
#define  FND_L      0b00111000

#define  FND_P      0b01110011
#define  FND_A      0b01110111
#define  FND_S      0b01101101

#define  FND_T      0b01111000
#define  FND_H      0b01110110
#define  FND_R      0b01010000

#define  FND_O      0b00111111
#define  FND_P      0b01110011
#define  FND_N      0b00110111

#define  FND_U      0b00111110


#define  FND_CIRCLE_1 0b00000001
#define  FND_CIRCLE_2 0b00000010
#define  FND_CIRCLE_3 0b00000100
#define  FND_CIRCLE_4 0b00001000
#define  FND_CIRCLE_5 0b00010000
#define  FND_CIRCLE_6 0b00100000
#define  FND_CIRCLE_7 0b01000000

#define  FND_L_INDEX     17
#define  FND_NUL_INDEX   16
#define  MAX_FND_LENGTH  4

#define PAGE_0  0
#define PAGE_1  1
#define PAGE_2  2
#define PAGE_3  3
#define PAGE_4  4
#define PAGE_5  5

#define MAX_PAGE_DISPLAY_FND 6

/// @brief FND에 출력을 하기 위한 자료형
typedef struct
{
  U8 Page;                                      ///< Display될 Page
  U8 Data[MAX_PAGE_DISPLAY_FND][MAX_FND_LENGTH];///< Page별 Display Data
  U8 UsePage[MAX_PAGE_DISPLAY_FND];             ///< 각각의 Page가 사용 되어 지고 있는지 확인
  U16 TimeOut1ms;                               ///< Display Timeout Count
  U8 Index;                                     ///< 현재 출력되고 있는 FND 번호(4개)
}typeDisplayFND;



void InitializeFnd(void);
void InterruptDisplayFnd(void);
void SettingValueHexFnd(U16 data);
void SettingValueHexFnd_Page1(U16 data);
void SettingValueHexFnd_Page1_NULL(U16 data);
void SettingValueHexFnd_Page1_NULL_Cnt(U16 data, U8 tKeyCnt);


void SettingValueHexFnd_CALL(void); 
void SettingValueHexFnd_CODE( void);

void SettingValueHexFnd_Circle(void);
void SettingValueHexFnd_Password(void);
void SettingValueHexFnd_BAR_Shift( U8 tStart );
void SettingValueHexFnd_Open( U8 tStart );
void SettingValueHexFnd_Number(void);
void SettingValueHexFnd_FindEndPage( void );
void SettingValueHexFnd_RF ( U8 tEnable, U8 tMode );
void SettingValueHexFnd_TEST(void);
void SettingValueHexFnd_Error( U8 tErrNum );
void SettingValueHexFnd_ErrorClear(void);

void SettingValueHexFnd_CLEAR(void);
void SettingValueHexFnd_Factory(U8 tSuccess, U8 tIndex );
void SettingValueHexFnd_Fnd_Factory( U8 tIndex );
void SettingValueHexFnd_House(void);
void SettingValueHexFnd_Page0_Version(U16 data);
void SettingValueHexFnd_Page1_Version(U16 data);

#endif  // #ifndef ___FUNC_FND_H___

