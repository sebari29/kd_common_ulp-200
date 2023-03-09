
/// @file   func_fnd.c
/// @date   2012/02/14
/// @author jinhwa Lee
/// @brief  FND Display 관련 파일

#include "main.h"


//UnionFnd  unFndBuffer;

U8 mbFndTable[18]={FND_0,FND_1,FND_2,FND_3,FND_4,
          FND_5,FND_6,FND_7,FND_8,FND_9,
          FND_A,FND_B,FND_C,FND_D,FND_E,FND_F,FND_NUL,FND_L};

U8 mCircleData[8] = {FND_CIRCLE_1,FND_CIRCLE_2,FND_CIRCLE_3,FND_CIRCLE_4,
                     FND_CIRCLE_5,FND_CIRCLE_6,FND_CIRCLE_7};
          

typeDisplayFND mDisplayFnd;


/// @brief FND 초기화 함수
/// @param void
/// @return void
void InitializeFnd(void)
{
  U8 i,j;

  mDisplayFnd.Page = 0;
  mDisplayFnd.TimeOut1ms = 0;
  mDisplayFnd.Index= 0;
  

  for(i=0;i<MAX_PAGE_DISPLAY_FND;i++)
  {
    mDisplayFnd.UsePage[i] = NULL;
    for(j=0;j<MAX_FND_LENGTH;j++)
    {
      mDisplayFnd.Data[i][j] = NULL;
    }
  }
  mDisplayFnd.UsePage[PAGE_0] = true;
  
  SettingValueHexFnd(0);
}


/// @brief FND에 실제로 출력을 하는 함수
/// @param void
/// @return void
void InterruptDisplayFnd(void)
{
  FEN_COM_PORT = (U8)(FEN_COM_PORT | 0x0F);
  FND_SEG_PORT = (U8)((FND_SEG_PORT | 0x7F) & mDisplayFnd.Data[mDisplayFnd.Page][mDisplayFnd.Index]);
  FEN_COM_PORT &= (~( 0x08 >> mDisplayFnd.Index));

  if ( mDisplayFnd.Index == (MAX_FND_LENGTH - 1) )
  {
    mDisplayFnd.Index = 0;
  }
  else
  {
    mDisplayFnd.Index++;
  }
}


/// @brief Page0에 숫자를 쓰는 함수
/// @param data 출력될 숫자
/// @return void
void SettingValueHexFnd(U16 data)
{
  static U8 tIsNull;
  U8 i;
  U8 ucdata;
  U8 ucHighNumberChk;
  U16  wData;

  // Data 초기화
  wData = data;
  ucHighNumberChk = 0;

  if ( tIsNull == NULL && data == NULL )
  {
    return;
  }

  // unFndBuffer 에 data 값으를 FND에 맞게 데이터를 넣음
  for ( i = 0 ; i < MAX_FND_LENGTH ; i++ )
  {
    ucdata = (U8)(( wData & 0xf000 ) >> 12);
    tIsNull = NULL;
    if ( ( ucdata == 0 ) && ( ucHighNumberChk == 0 ) )
    {
      mDisplayFnd.Data[PAGE_0][(U8)((MAX_FND_LENGTH - 1) - i)] = (U8)~FND_NUL; // 상위 숫자가 없으면 NULL 문자를 표시한다.
    }
    else
    {
      mDisplayFnd.Data[PAGE_0][(MAX_FND_LENGTH - 1) - i] = ~mbFndTable[ucdata];
      tIsNull++;
      ucHighNumberChk = true;
    }
    wData <<= 4;
  }
}


/// @brief Page1에 숫자를 쓰는 함수 ( 비밀 번호 출력때 사용 )
/// @param data 출력될 숫자
/// @return void
void SettingValueHexFnd_Page1(U16 data)
{
  U8 i;
  U8 ucdata;
  U16  wData;

  // Data 초기화
  wData = data;

  mDisplayFnd.UsePage[PAGE_1] = true;

  for ( i = 0 ; i < MAX_FND_LENGTH ; i++ )
  {
    ucdata = (U8)(( wData & 0xf000 ) >> 12);
    mDisplayFnd.Data[PAGE_1][(U8)((MAX_FND_LENGTH - 1) - i)] = ~mbFndTable[ucdata];
    wData <<= 4;
  }
}


/// @brief Page1에 숫자를 쓰는 함수 \n 상위 숫자가 없으면 NULL 출력
/// @param data 출력될 숫자
/// @return void
void SettingValueHexFnd_Page1_NULL(U16 data)
{
  U8 i;
  U8 ucdata;
  U8 ucHighNumberChk;
  U16  wData;

  /// Data 초기화
  wData = data;
  ucHighNumberChk = 0;

  mDisplayFnd.UsePage[PAGE_1] = true;
  
  /// unFndBuffer 에 data 값으를 FND에 맞게 데이터를 넣음
  for ( i = 0 ; i < MAX_FND_LENGTH ; i++ )
  {
    ucdata = (U8)(( wData & 0xf000 ) >> 12);
    if ( ( ucdata == 0 ) && ( ucHighNumberChk == 0 ) )
    {
      mDisplayFnd.Data[PAGE_1][(U8)((MAX_FND_LENGTH - 1) - i)] = (U8)~FND_NUL; // 상위 숫자가 없으면 NULL 문자를 표시한다.
    }
    else
    {
      mDisplayFnd.Data[PAGE_1][(U8)((MAX_FND_LENGTH - 1) - i)] = ~mbFndTable[ucdata];
      ucHighNumberChk = true;
    }
    wData <<= 4;
  }

  if ( data == NULL ) mDisplayFnd.Data[PAGE_1][0] = ~mbFndTable[0];
  
  SettingValueHexFnd_FindEndPage();
}

/// @brief Page1에 숫자를 쓰는 함수 \n 상위 숫자가 없으면 NULL 출력
/// @param data 출력될 숫자
/// @return void
void SettingValueHexFnd_Page1_NULL_Cnt(U16 data, U8 tKeyCnt)
{
  U8 i;
  U8 ucdata;
  U8 ucHighNumberChk;
  U16  wData;

  /// Data 초기화
  wData = data;
  ucHighNumberChk = 0;

  mDisplayFnd.UsePage[PAGE_1] = true;
  
  mDisplayFnd.Data[PAGE_1][1] = (U8)~FND_NUL;
  mDisplayFnd.Data[PAGE_1][2] = (U8)~FND_NUL;
  mDisplayFnd.Data[PAGE_1][3] = (U8)~FND_NUL;
  mDisplayFnd.Data[PAGE_1][0] = (U8)~FND_NUL;  

  for ( i = 0 ; i < tKeyCnt ; i++ )
  {
    ucdata = (U8)( wData & 0x000F );
    mDisplayFnd.Data[PAGE_1][i] = ~mbFndTable[ucdata];
    wData >>= 4;
  }

  if ( data == NULL ) mDisplayFnd.Data[PAGE_1][0] = ~mbFndTable[0];
  
  SettingValueHexFnd_FindEndPage();
}

/// @brief Page1에 CALL 출력
/// @param void
/// @return void
void SettingValueHexFnd_CALL( void)
{
  mDisplayFnd.Data[PAGE_1][3] = (U8)~FND_C;
  mDisplayFnd.Data[PAGE_1][2] = (U8)~FND_A;
  mDisplayFnd.Data[PAGE_1][1] = (U8)~FND_L;
  mDisplayFnd.Data[PAGE_1][0] = (U8)~FND_L;
  mDisplayFnd.UsePage[PAGE_1] = true;
  SettingValueHexFnd_FindEndPage();
}

/// @brief Page1에 CODE 출력
/// @param void
/// @return void
void SettingValueHexFnd_CODE( void)
{
  mDisplayFnd.Data[PAGE_1][3] = (U8)~FND_C;
  mDisplayFnd.Data[PAGE_1][2] = (U8)~FND_O;
  mDisplayFnd.Data[PAGE_1][1] = (U8)~FND_D;
  mDisplayFnd.Data[PAGE_1][0] = (U8)~FND_E;
  mDisplayFnd.UsePage[PAGE_1] = true;
  SettingValueHexFnd_FindEndPage();
}

/// @brief 최상위 패이지를 찾아 출력 패이지를 넘겨줌
/// @param void
/// @return void
void SettingValueHexFnd_FindEndPage( void )
{
  static U8 mtPage;
  U8 i;
  
  for ( i = 0; i < MAX_PAGE_DISPLAY_FND; i++)
  {
    if ( mDisplayFnd.UsePage[(U8)((MAX_PAGE_DISPLAY_FND - 1) - i)] == true )
    { 
      mDisplayFnd.Page = (U8)((MAX_PAGE_DISPLAY_FND - 1) - i);
      return;
    }
  }
}


/// @brief Page0에 초기 FND 출력 ( 바가 회전함 )
/// @param void
/// @return void
void SettingValueHexFnd_Circle(void)
{
  static U8 tCircleIndex = 0;

  mDisplayFnd.Data[PAGE_0][3] = ~mCircleData[tCircleIndex];
  mDisplayFnd.Data[PAGE_0][2] = ~mCircleData[tCircleIndex];
  mDisplayFnd.Data[PAGE_0][1] = ~mCircleData[tCircleIndex];
  mDisplayFnd.Data[PAGE_0][0] = ~mCircleData[tCircleIndex];
  mDisplayFnd.UsePage[PAGE_0] = true;
  SettingValueHexFnd_FindEndPage();

  if ( tCircleIndex++ == 5 )  tCircleIndex = 0;
  
}


/// @brief Page1에 PASS 출력
/// @param void
/// @return void
void SettingValueHexFnd_Password(void)
{
  mDisplayFnd.Data[PAGE_1][3] = (U8)~FND_P;
  mDisplayFnd.Data[PAGE_1][2] = (U8)~FND_A;
  mDisplayFnd.Data[PAGE_1][1] = (U8)~FND_S;
  mDisplayFnd.Data[PAGE_1][0] = (U8)~FND_S;
  mDisplayFnd.UsePage[PAGE_1] = true;
  SettingValueHexFnd_FindEndPage();
}

/// @brief Page5에 에러 메시지 출력
/// @param void
/// @return void
void SettingValueHexFnd_Error( U8 tErrNum )
{
  mDisplayFnd.Data[PAGE_5][3] = (U8)~FND_E;
  mDisplayFnd.Data[PAGE_5][2] = (U8)~FND_R;
  mDisplayFnd.Data[PAGE_5][1] = (U8)~FND_R;
  mDisplayFnd.Data[PAGE_5][0] = (U8)~mbFndTable[tErrNum];
  mDisplayFnd.UsePage[PAGE_5] = true;
  SettingValueHexFnd_FindEndPage();
}

/// @brief Page5에 에러 메시지 출력 삭제
/// @param void
/// @return void
void SettingValueHexFnd_ErrorClear(void)
{
  mDisplayFnd.UsePage[PAGE_5] = false;
  SettingValueHexFnd_FindEndPage();
}


/*
void SettingValueHexFnd_Setting(void)
{
  mDisplayFnd.Data[PAGE_1][3] = ~FND_NUL;
  mDisplayFnd.Data[PAGE_1][2] = ~FND_S;
  mDisplayFnd.Data[PAGE_1][1] = ~FND_E;
  mDisplayFnd.Data[PAGE_1][0] = ~FND_T;
  mDisplayFnd.UsePage[PAGE_1] = true;
  SettingValueHexFnd_FindEndPage();
  
}
*/


/// @brief Page1에 모든 출력 삭제
/// @param void
/// @return void
void SettingValueHexFnd_CLEAR(void)
{
  mDisplayFnd.Data[PAGE_1][3] = (U8)~FND_NUL;
  mDisplayFnd.Data[PAGE_1][2] = (U8)~FND_NUL;
  mDisplayFnd.Data[PAGE_1][1] = (U8)~FND_NUL;
  mDisplayFnd.Data[PAGE_1][0] = (U8)~FND_NUL;
  mDisplayFnd.UsePage[PAGE_1] = true;
  SettingValueHexFnd_FindEndPage();
  
}


/// @brief Page1에 TEST 출력
/// @param void
/// @return void
void SettingValueHexFnd_TEST(void)
{
  mDisplayFnd.Data[PAGE_1][3] = (U8)~FND_T;
  mDisplayFnd.Data[PAGE_1][2] = (U8)~FND_E;
  mDisplayFnd.Data[PAGE_1][1] = (U8)~FND_S;
  mDisplayFnd.Data[PAGE_1][0] = (U8)~FND_T;
  mDisplayFnd.UsePage[PAGE_1] = true;
  SettingValueHexFnd_FindEndPage();
}



/// @brief Page1에 [-]바 출력
/// @param tStart True = FND Clear \n False = Display [-]
/// @return void
void SettingValueHexFnd_BAR_Shift( U8 tStart )
{
  if ( tStart == true )
  {
    SettingValueHexFnd_CLEAR();
  }
  
  mDisplayFnd.Data[PAGE_1][3] = mDisplayFnd.Data[PAGE_1][2];
  mDisplayFnd.Data[PAGE_1][2] = mDisplayFnd.Data[PAGE_1][1];
  mDisplayFnd.Data[PAGE_1][1] = mDisplayFnd.Data[PAGE_1][0];
  mDisplayFnd.Data[PAGE_1][0] = (U8)~FND_CBAR;
  mDisplayFnd.UsePage[PAGE_1] = true;
  SettingValueHexFnd_FindEndPage();
}


/// @brief Page2에 OPEN 출력 or 삭제
/// @param tStart True = 출력 시작 \n False = 출력 종료
/// @return void
void SettingValueHexFnd_Open( U8 tStart )
{
  if ( tStart )
  {
    
    mDisplayFnd.Data[PAGE_2][3] = (U8)~FND_O;
    mDisplayFnd.Data[PAGE_2][2] = (U8)~FND_P;
    mDisplayFnd.Data[PAGE_2][1] = (U8)~FND_E;
    mDisplayFnd.Data[PAGE_2][0] = (U8)~FND_N;
    mDisplayFnd.UsePage[PAGE_2] = true;
  }
  else
  {
    mDisplayFnd.UsePage[PAGE_2] = false;
  }
  SettingValueHexFnd_FindEndPage();
}   


/// @brief Page3에 RF Mode Display
/// @param tEnable True = Page3 Display Enable \n False \ Page3 Display Disable
/// @param tMode   RFID의 모드 출력
/// @return void
void SettingValueHexFnd_RF ( U8 tEnable, U8 tMode )
{
  if ( !tEnable )
  {
    mDisplayFnd.UsePage[PAGE_3] = false;
    SettingValueHexFnd_FindEndPage();
    return;
  }
  
  mDisplayFnd.Data[PAGE_3][3] = (U8)~FND_R;
  mDisplayFnd.Data[PAGE_3][2] = (U8)~FND_F;

  switch( tMode )
  {
    case RF_READER_MODE_NORMAL:
      mDisplayFnd.Data[PAGE_3][1] = (U8)~FND_NUL;      
      mDisplayFnd.Data[PAGE_3][0] = (U8)~FND_NUL;
      break;
    case RF_READER_MODE_HOUSE:
      mDisplayFnd.Data[PAGE_3][1] = (U8)~FND_CBAR;
      mDisplayFnd.Data[PAGE_3][0] = (U8)~FND_H;
      break;
    case RF_READER_MODE_MASTER:
      mDisplayFnd.Data[PAGE_3][1] = (U8)~FND_CBAR;
      mDisplayFnd.Data[PAGE_3][0] = (U8)~FND_S;
      break;
    case RF_READER_MODE_DELETE:
      mDisplayFnd.Data[PAGE_3][1] = (U8)~FND_CBAR;
      mDisplayFnd.Data[PAGE_3][0] = (U8)~FND_D;
      break;
    default:
      mDisplayFnd.Data[PAGE_3][0] = (U8)~FND_CBAR;
      break;
  }
  mDisplayFnd.UsePage[PAGE_3] = true;
  SettingValueHexFnd_FindEndPage();


}

void SettingValueHexFnd_House(void)
{
  mDisplayFnd.Data[PAGE_1][3] = (U8)~FND_H;
  mDisplayFnd.Data[PAGE_1][2] = (U8)~FND_O;
  mDisplayFnd.Data[PAGE_1][1] = (U8)~FND_U;
  mDisplayFnd.Data[PAGE_1][0] = (U8)~FND_S;
  mDisplayFnd.UsePage[PAGE_1] = true;
  SettingValueHexFnd_FindEndPage();
}

/// @brief Page1에 Display Disable
/// @param void
/// @return void
void SettingValueHexFnd_Number(void)
{
  mDisplayFnd.UsePage[PAGE_1] = false;
  SettingValueHexFnd_FindEndPage();
}



/// @brief Factory Mode Fnd Out
/// @param tSuccess 출력될 문자
/// @param tIndex 출력될 숫자
/// @return void
void SettingValueHexFnd_Factory(U8 tSuccess, U8 tIndex )
{
  U8 tTemp;


  
  if ( tSuccess == true )
  {
    mDisplayFnd.Data[PAGE_0][3] = (U8)~FND_S;
  }
  else
  {
    mDisplayFnd.Data[PAGE_0][3] = (U8)~FND_E;
  }
  

  mDisplayFnd.Data[PAGE_0][2] = (U8)~FND_CBAR;


  if ( tIndex  == 0xFF )
  {
    mDisplayFnd.Data[PAGE_0][1] = (U8)~FND_R;
    mDisplayFnd.Data[PAGE_0][0] = (U8)~FND_F;
    return;
  }


  tTemp = (U8)(tIndex / 10);
  mDisplayFnd.Data[PAGE_0][1] = ~mbFndTable[tTemp];
  tTemp = (U8)(tIndex % 10);
  mDisplayFnd.Data[PAGE_0][0] = ~mbFndTable[tTemp];
  
}


/// @brief Factory Mode Fnd Out 2
/// @param tSuccess 출력될 문자
/// @param tIndex 출력될 숫자
/// @return void
void SettingValueHexFnd_Fnd_Factory( U8 tIndex )
{

  mDisplayFnd.Data[PAGE_0][1] = (U8)~FND_NUL;
  mDisplayFnd.Data[PAGE_0][2] = (U8)~FND_NUL;
  mDisplayFnd.Data[PAGE_0][3] = (U8)~FND_NUL;

  switch( tIndex-1 )
  {
    case 0:
      mDisplayFnd.Data[PAGE_0][3] = (U8)~FND_9;
      mDisplayFnd.Data[PAGE_0][2] = (U8)~FND_A;
      mDisplayFnd.Data[PAGE_0][1] = (U8)~FND_R;
      mDisplayFnd.Data[PAGE_0][0] = (U8)~FND_D;
      break;
    case 1:
      mDisplayFnd.Data[PAGE_0][3] = (U8)~FND_H;
      mDisplayFnd.Data[PAGE_0][2] = (U8)~FND_O;
      mDisplayFnd.Data[PAGE_0][1] = (U8)~FND_U;
      mDisplayFnd.Data[PAGE_0][0] = (U8)~FND_S;
      break;
    case 2:
      mDisplayFnd.Data[PAGE_0][0] = (U8)~FND_1;
      break;
    case 3:
      mDisplayFnd.Data[PAGE_0][0] = (U8)~FND_2;
      break;
    case 4:
      mDisplayFnd.Data[PAGE_0][0] = (U8)~FND_3;
      break;
    case 5:
      mDisplayFnd.Data[PAGE_0][0] = (U8)~FND_4;
      break;
    case 6:
      mDisplayFnd.Data[PAGE_0][0] = (U8)~FND_5;
      break;
    case 7:
      mDisplayFnd.Data[PAGE_0][0] = (U8)~FND_6;
      break;
    case 8:
      mDisplayFnd.Data[PAGE_0][0] = (U8)~FND_7;
      break;
    case 9:
      mDisplayFnd.Data[PAGE_0][0] = (U8)~FND_8;
      break;
    case 10:
      mDisplayFnd.Data[PAGE_0][0] = (U8)~FND_9;
      break;
    case 11:
      mDisplayFnd.Data[PAGE_0][3] = (U8)~FND_S;
      mDisplayFnd.Data[PAGE_0][2] = (U8)~FND_T;
      mDisplayFnd.Data[PAGE_0][1] = (U8)~FND_A;
      mDisplayFnd.Data[PAGE_0][0] = (U8)~FND_R;
      break;
    case 12:
      mDisplayFnd.Data[PAGE_0][0] = (U8)~FND_0;
      break;
    case 13:
      mDisplayFnd.Data[PAGE_0][3] = (U8)~FND_S;
      mDisplayFnd.Data[PAGE_0][2] = (U8)~FND_H;
      mDisplayFnd.Data[PAGE_0][1] = (U8)~FND_R;
      mDisplayFnd.Data[PAGE_0][0] = (U8)~FND_P;

      break;
    default:
      break;
  }
  
}

/// @brief Page1 버전 출력
/// @param data 출력될 숫자
/// @return void
void SettingValueHexFnd_Page0_Version(U16 data)
{
  U8 i;
  U8 ucdata;
  U16  wData;

  wData = data;

  mDisplayFnd.UsePage[PAGE_0] = true;
  
  mDisplayFnd.Data[PAGE_0][3] = (U8)~FND_B;
  mDisplayFnd.Data[PAGE_0][2] = (U8)~FND_CBAR;

  ucdata = (U8)(( wData & 0xF000 ) >> 12);
  mDisplayFnd.Data[PAGE_0][1] = ~mbFndTable[ucdata];
   
  ucdata = (U8)(( wData & 0x0F00 ) >> 8);
  mDisplayFnd.Data[PAGE_0][0] = ~mbFndTable[ucdata];

  
}

/// @brief Page1 버전 출력1
/// @param data 출력될 숫자
/// @return void
void SettingValueHexFnd_Page1_Version(U16 data)
{
  U8 i;
  U8 ucdata;
  U16  wData;

  // Data 초기화
  wData = data;

  mDisplayFnd.UsePage[PAGE_1] = true;
  
  mDisplayFnd.Data[PAGE_1][3] = (U8)~FND_B;
  mDisplayFnd.Data[PAGE_1][2] = (U8)~FND_CBAR;

  ucdata = (U8)(( wData & 0xF000 ) >> 12);
  mDisplayFnd.Data[PAGE_1][1] = ~mbFndTable[ucdata];
   
  ucdata = (U8)(( wData & 0x0F00 ) >> 8);
  mDisplayFnd.Data[PAGE_1][0] = ~mbFndTable[ucdata];

  
}


