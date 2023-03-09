
/// @file func_console.h
/// @date 2012/02/14
/// @author jinhwa Lee
/// @brief  터미널 관리 프로그램 - 사용자 설정

#ifndef ___FUNC_CONSOLE_H___
#define ___FUNC_CONSOLE_H___


#define CONSOLE_TIMEOUT_SEC 6000 // 10분

#define CONSOLE_TX_BUFFER_SIZE     200
#define CONSOLE_RX_BUFFER_SIZE     10

#define CONSOLE_SAVE_BUFFER_SIZE   24

#define ASCII_ONE    0x31
#define ASCII_TWO    0x32
#define ASCII_THREE  0x33
#define ASCII_FOUR   0x34
#define ASCII_FIVE   0x35
#define ASCII_SIX    0x36
#define ASCII_SEVEN  0x37
#define ASCII_EIGHT  0x38
#define ASCII_NINE   0x39
#define ASCII_A      0x41
#define ASCII_a      0x61
#define ASCII_B      0x42
#define ASCII_b      0x62
#define ASCII_C      0x43
#define ASCII_c      0x63
#define ASCII_D      0x44
#define ASCII_d      0x64
#define ASCII_E      0x45
#define ASCII_e      0x65
#define ASCII_F      0x46
#define ASCII_f      0x66
#define ASCII_G      0x47
#define ASCII_g      0x67
#define ASCII_H      0x48
#define ASCII_h      0x68
#define ASCII_I      0x49
#define ASCII_i      0x69
#define ASCII_J      0x4A
#define ASCII_j      0x6A
#define ASCII_ENTER  13

#define CON_STS_0    0
#define CON_STS_1    1
#define CON_STS_2    2
#define CON_STS_3    3
#define CON_STS_4    4

void InitializeConsole(void);

void bPutTXConsoleData( U8* data, U8 length );
U8 bGetTXConsoleData( U8* data );
U8 bStartTXConsol(void);
void bPrintString( U8 *data );
void bPrintHex_Protocol( U8 *data, U8 tLength, U8 tPath );

void ConsoleDataDisplay( U8* data);
void bPrintHex( U8 *data, U8 tLength );
void ThreadConsole(void);
U8 SettingData(U8* Data, U8 Count);  

void bPrintHex_NonSpace( U8 *data, U8 tLength );

void ConsoleTimeOutReset(void);
void bPrintPtocotolCmd( U8 tCmd );
void EnterDisplay(void);

#endif  // #ifndef ___FUNC_CONSOLE_H___

