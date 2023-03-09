
/// @file   func_sound.c
/// @date   2012/02/14
/// @author jinhwa Lee
/// @brief  비프음 출력 관련 파일

#include "main.h"

#define ELIZE_COUNT     81
#define NOTE8_2         18
#define BUSY_DELAY      28

const U16 FurElize[ELIZE_COUNT][2]=  
{
  HMI_F,  NOTE8_2,  // 미    1
  HRI_F,  NOTE8_2,  // 샾레
  HMI_F,  NOTE8_2,  // 미    2
  HRI_F,  NOTE8_2,  // 샾레
  HMI_F,  NOTE8_2,  // 미
  MSI_F,  NOTE8_2,  // 아래 시
  HRE_F,  NOTE8_2,  // 레
  HDO_F,  NOTE8_2,  // 도
  MLA_F,  NOTE8_2*2,// 아래 라  3
  0,  NOTE8_2,      // 쉼표
  MDO_F,  NOTE8_2,  // 아래 도
  MMI_F,  NOTE8_2,  // 아래 미
  MLA_F,  NOTE8_2,  // 아래 라
  MSI_F,  NOTE8_2*2,// 아래 시  4
  0,  NOTE8_2,      // 쉼표
  MMI_F,  NOTE8_2,  // 아래 미
  MSIL_F,  NOTE8_2, // 아래 샾솔
  MSI_F,  NOTE8_2,  // 아래 시
  HDO_F,  NOTE8_2*2,// 도    5
  0,  NOTE8_2,      // 쉼표
  MMI_F,  NOTE8_2,  // 아래 미
  HMI_F,  NOTE8_2,  // 미
  HRI_F,  NOTE8_2,  // 샾 레
  HMI_F,  NOTE8_2,  // 미    6
  HRI_F,  NOTE8_2,  // 샾 레
  HMI_F,  NOTE8_2,  // 미
  MSI_F,  NOTE8_2,  // 아래 시
  HRE_F,  NOTE8_2,  // 레
  HDO_F,  NOTE8_2,  // 도
  MLA_F,  NOTE8_2*2,// 아래 라  7
  0,  NOTE8_2,      // 쉼표
  MDO_F,  NOTE8_2,  // 아래 도
  MMI_F,  NOTE8_2,  // 아래 미
  MLA_F,  NOTE8_2,  // 아래 라
  MSI_F,  NOTE8_2*2,// 아래 시  8
  0,  NOTE8_2,      // 쉼표
  MMI_F,  NOTE8_2,  // 아래 미
  HDO_F,  NOTE8_2,  // 도
  MSI_F,  NOTE8_2,  // 아래 시
  MLA_F,  NOTE8_2*4,// 아래 라  9
  0,  NOTE8_2*2,    // 쉼표
  HMI_F,  NOTE8_2,  // 미    10
  HRI_F,  NOTE8_2,  // 샾레
  HMI_F,  NOTE8_2,  // 미
  MSI_F,  NOTE8_2,  // 아래 시
  HRE_F,  NOTE8_2,  // 레
  HDO_F,  NOTE8_2,  // 도
  MLA_F,  NOTE8_2*2,// 아래 라  11
  0,  NOTE8_2,      // 쉼표
  MDO_F,  NOTE8_2,  // 아래 도
  MMI_F,  NOTE8_2,  // 아래 미
  MLA_F,  NOTE8_2,  // 아래 라
  MSI_F,  NOTE8_2*2,// 아래 시  12
  0,  NOTE8_2,      // 쉼표
  MMI_F,  NOTE8_2,  // 아래 미
  MSIL_F,  NOTE8_2, // 아래 샾솔
  MSI_F,  NOTE8_2,  // 아래 시
  HDO_F,  NOTE8_2*2,// 도    13
  0,  NOTE8_2,      // 쉼표
  MMI_F,  NOTE8_2,  // 아래 미
  HMI_F,  NOTE8_2,  // 미
  HRI_F,  NOTE8_2,  // 샾 레
  HMI_F,  NOTE8_2,  // 미    14
  HRI_F,  NOTE8_2,  // 샾 레
  HMI_F,  NOTE8_2,  // 미
  MSI_F,  NOTE8_2,  // 아래 시
  HRE_F,  NOTE8_2,  // 레
  HDO_F,  NOTE8_2,  // 도
  MLA_F,  NOTE8_2*2,// 아래 라  15
  0,  NOTE8_2,      // 쉼표
  MDO_F,  NOTE8_2,  // 아래 도
  MMI_F,  NOTE8_2,  // 아래 미
  MLA_F,  NOTE8_2,  // 아래 라
  MSI_F,  NOTE8_2*2,// 아래 시  16
  0,  NOTE8_2,      // 쉼표
  MMI_F,  NOTE8_2,  // 아래 미
  HDO_F,  NOTE8_2,  // 도
  MSI_F,  NOTE8_2,  // 아래 시
  MLA_F,  NOTE8_2*4,// 아래 라  17  // 78
  0,  NOTE8_2       // 쉼표
/*
  NOTE16,  NOTE8_2, // 쉼표
  MSI_F,  NOTE8_2,  // 아래 시
  HDO_F,  NOTE8_2,  // 도
  HRE_F,  NOTE8_2,  // 레    // 82
*/
/*
  HMI_F,  NOTE8_2*3,// 미    18
  MSI_F,  NOTE8_2,  // 아래 시
  HFA_F,  NOTE8_2,  // 파
  HMI_F,  NOTE8_2,  // 미

  HRE_F,  NOTE8_2*3,  //      19
  MFA_F,  NOTE8_2,
  HMI_F,  NOTE8_2,
  HRE_F,  NOTE8_2,

  HDO_F,  NOTE8_2*3,  //      20
  MMI_F,  NOTE8_2,
  HRE_F,  NOTE8_2,
  HDO_F,  NOTE8_2,

  MSI_F,  NOTE8_2*2,  //      21
  NOTE16,  NOTE8_2,  // 쉼표
  MMI_F,  NOTE8_2,
  HMI_F,  NOTE8_2,
  NOTE16,  NOTE8_2,  // 쉼표

  NOTE16,  NOTE8_2,  // 쉼표    22
  HMI_F,  NOTE8_2,
  VHMI_F,  NOTE8_2,
  NOTE16,  NOTE8_2,  // 쉼표
  NOTE16,  NOTE8_2,  // 쉼표
  HRI_F,  NOTE8_2,

  HRE_F,  NOTE8_2*2,  //      23
  NOTE16,  NOTE8_2,  // 쉼표
  HRI_F,  NOTE8_2,
  HMI_F,  NOTE8_2,
  HRI_F,  NOTE8_2,

  HMI_F,  NOTE8_2,  //      24
  HRI_F,  NOTE8_2,
  HMI_F,  NOTE8_2,
  MSI_F,  NOTE8_2,
  HRE_F,  NOTE8_2,
  HDO_F,  NOTE8_2,

  MLA_F,  NOTE8_2*2,  //      25
  NOTE16,  NOTE8_2,
  MDO_F,  NOTE8_2,
  MMI_F,  NOTE8_2,
  MLA_F,  NOTE8_2,
*/
};


const U16 Cookoo[24][2] =  
{
  VHMI_F,    24,
  VHDO_F,    48,
  VHMI_F,    24,
  VHDO_F,    48,
  VHMI_F,    24,
  VHSOL_F,   24,
  VHSOL_F,   12,
  VHMI_F,    12,
  VHDO_F,    12,
  VHMI_F,    12,
  VHRE_F,    48,
  VHFA_F,    24,
  HSI_F,     48,
  VHRE_F,    24,
  HSOL_F,    48,
  VHRE_F,    24,
  VHFA_F,    24,
  VHFA_F,    12,
  VHRE_F,    12,
  HSI_F,     12,
  VHRE_F,    12,
  VHDO_F,    48,
  0,         48  // 쉼표
};

const U16 OngDalSaem[43][2] =  
{
  MSI_F,  NOTE8,
  HRE_F,  NOTE8,
  HSOL_F,  NOTE8,
  MSI_F,  NOTE8,
  HRE_F,  NOTE4,
  HDO_F,  NOTE4,
  HMI_F,  NOTE4,
  HMI_F,  NOTE4,
  HRE_F,  NOTE8,
  HFA_F,  NOTE8,
  HLA_F,  NOTE8,
  VHDO_F,  NOTE8,
  HSI_F,  NOTE8,
  HLA_F,  NOTE8,
  HSOL_F,  NOTE2,

  MSOL_F,  NOTE4,
  HSI_F,  NOTE4,
  VHRE_F,  NOTE4,
  VHDO_F,  NOTE4,
  HSI_F,  NOTE8,
  HLA_F,  NOTE4,
  HRE_F,  NOTE4,
  HFA_F,  NOTE8,
  HLA_F,  NOTE8,
  VHDO_F,  NOTE4,
  HSI_F,  NOTE4,
  HLA_F,  NOTE8,
  HSOL_F,  NOTE4,

  MSI_F,  NOTE8,
  HRE_F,  NOTE8,
  HSOL_F,  NOTE8,
  MSI_F,  NOTE8,
  HRE_F,  NOTE4,
  HDO_F,  NOTE4,
  HMI_F,  NOTE4,
  HMI_F,  NOTE4,
  HRE_F,  NOTE8,
  HFA_F,  NOTE8,
  HLA_F,  NOTE8,
  VHDO_F,  NOTE8,
  HSI_F,  NOTE8,
  HLA_F,  NOTE8,
  HSOL_F,  NOTE2,
};

const U16 ThreeBear[49][2] =  
{
  HDO_F,  NOTE4,
  HDO_F,  NOTE8,
  HDO_F,  NOTE8,
  HDO_F,  NOTE4,
  HDO_F,  NOTE4,
  HMI_F,  NOTE4,
  HSOL_F,  NOTE8,
  HSOL_F,  NOTE8,
  HMI_F,  NOTE4,
  HDO_F,  NOTE4,
  
  HSOL_F,  NOTE8,
  HSOL_F,  NOTE8,
  HMI_F,  NOTE4,
  HSOL_F,  NOTE8,
  HSOL_F,  NOTE8,
  HMI_F,  NOTE4,
  HDO_F,  NOTE4,
  HDO_F,  NOTE4,
  HDO_F,  NOTE2,

  HSOL_F,  NOTE4,
  HSOL_F,  NOTE4,
  HMI_F,  NOTE4,
  HDO_F,  NOTE4,
  HSOL_F,  NOTE4,
  HSOL_F,  NOTE4,
  HSOL_F,  NOTE2,

  HSOL_F,  NOTE4,
  HSOL_F,  NOTE4,
  HMI_F,  NOTE4,
  HDO_F,  NOTE4,
  HSOL_F,  NOTE4,
  HSOL_F,  NOTE4,
  HSOL_F,  NOTE2,

  HSOL_F,  NOTE4,
  HSOL_F,  NOTE4,
  HMI_F,  NOTE4,
  HDO_F,  NOTE4,
  HSOL_F,  NOTE8,
  HSOL_F,  NOTE8,
  HSOL_F,  NOTE8,
  HLA_F,  NOTE8,
  HSOL_F,  NOTE2,

  HDO_F,  NOTE4,
  HSOL_F,  NOTE4,
  HDO_F,  NOTE4,
  HSOL_F,  NOTE4,
  HMI_F,  NOTE4,
  HRE_F,  NOTE4,
  HDO_F,  NOTE2,
};

/* 320hz(25ms) + 480hz(25ms) : 20count*/
const U16 Tel[20][2] =  
{
  HMI_F,  NOTE16,
  HSI_F,  NOTE16,
  HMI_F,  NOTE16,
  HSI_F,  NOTE16,
  HMI_F,  NOTE16,
  HSI_F,  NOTE16,
  HMI_F,  NOTE16,
  HSI_F,  NOTE16,
  HMI_F,  NOTE16,
  HSI_F,  NOTE16,
  HMI_F,  NOTE16,
  HSI_F,  NOTE16,
  HMI_F,  NOTE16,
  HSI_F,  NOTE16,
  HMI_F,  NOTE16,
  HSI_F,  NOTE16,
  HMI_F,  NOTE16,
  HSI_F,  NOTE16,
  HMI_F,  NOTE16,
  HSI_F,  NOTE16,
};

/* 480hz(500ms) + 360hz(500ms) : 8count*/
const U16 Siren[16][2] =  
{
  VHSI_F,  NOTE8,
  VHFA_F,  NOTE8,
  VHSI_F,  NOTE8,
  VHFA_F,  NOTE8,
  VHSI_F,  NOTE8,
  VHFA_F,  NOTE8,
  VHSI_F,  NOTE8,
  VHFA_F,  NOTE8,
  VHSI_F,  NOTE8,
  VHFA_F,  NOTE8,
  VHSI_F,  NOTE8,
  VHFA_F,  NOTE8,
  VHSI_F,  NOTE8,
  VHFA_F,  NOTE8,
  VHSI_F,  NOTE8,
  VHFA_F,  NOTE8,
};

const U16 KyungDong[63][2] =  
{
  HRE_F,  NOTE4,
  HRE_F,  NOTE6,
  HMI_F,  NOTE8,
  HFA_F,  NOTE2,
  HLA_F,  NOTE8,//5
  HSI_F,  NOTE4,
  HFA_F,  NOTE8,
  HLA_F,  NOTE2,
  VHRE_F,  NOTE4,
  VHDO_F,  NOTE4,//10
  HSI_F,  NOTE8,
  HSI_F,  NOTE3,
  VHRE_F,  NOTE8,
  HSI_F,  NOTE4,
  HFA_F,  NOTE8,//15
  HLA_F,  NOTE2,

  HSOL_F,  NOTE4,
  HFA_F,  NOTE4,
  HMI_F,  NOTE4,
  HRE_F,  NOTE4,//20
  HSOL_F,  NOTE4,
  HFA_F,  NOTE6,
  HMI_F,  NOTE8,
  HRE_F,  NOTE4,
  HFA_F,  NOTE4,//25
  HSOL_F,  NOTE4,
  HFA_F,  NOTE4,
  HMI_F,  NOTE8,
  HMI_F,  NOTE3,
  HRE_F,  NOTE8,//30
  HMI_F,  NOTE4,
  HDO_F,  NOTE8,
  HRE_F,  NOTE4,
  MLA_F,  NOTE6,
  MLA_F,  NOTE8,//35

  HFA_F,  NOTE1,
  HMI_F,  NOTE6,
  HFA_F,  NOTE8,
  HRE_F,  NOTE2,
  MLA_F,  NOTE2,
  MSI_F,  NOTE4,
  MSI_F,  NOTE6,
  HDO_F,  NOTE8,
  HRE_F,  NOTE2,
  HSOL_F,  NOTE4,
  HFA_F,  NOTE6,
  HRE_F,  NOTE8,
  HMI_F,  NOTE2,

  HFA_F,  NOTE4,
  HFA_F,  NOTE4,
  HFA_F,  NOTE4,
  HFA_F,  NOTE8,
  HSOL_F,  NOTE8,
  HLA_F,  NOTE8,
  HSI_F,  NOTE1,
  VHRE_F,  NOTE6,
  VHRE_F,  NOTE8,
  HSI_F,  NOTE4,
  VHRE_F,  NOTE4,
  VHDO_F,  NOTE4,
  HSI_F,  NOTE6,
  VHDO_F,  NOTE8,
  VHRE_F,  NOTE1,
};

U16 sound_type;
U16 sound_seq;
U16 sound_cnt;
U16 sound_play_16ms;


/// @brief  비프음 출력 함수 Initialize
/// @param  void
/// @return void
void InitializeSound(void)
{
   __InitializePWM();
   SpeakerOnOff( SPK_ONOFF_ON, SPK_ONOFF_MELODY);
}


/// @brief  버튼음 출력 함수
/// @param  void
/// @return void
void key_tone(void)
{
  switch(sound_seq)
  {
    case 0:
      sound_freq_play(VHDO_F,NOTE_SHORT);
      break;
    case 1:
      if(--sound_cnt == 0)
        sound_freq_stop();
      else
        sound_seq = 0;
      return;
    default:
      break;
  }
  sound_seq++;
}


/// @brief  비지음 출력 함수
/// @param  void
/// @return void
void busy_tone(void)
{
  switch(sound_seq)
  {
    case 0:
      sound_freq_play(HLA_F,BUSY_DELAY);
      break;
    case 1:
      sound_freq_play(0,BUSY_DELAY/2);
      break;
    case 2:
      sound_freq_play(HLA_F,BUSY_DELAY);
      break;
    case 3:
      sound_freq_play(0,BUSY_DELAY/2);
      break;
    case 4:
      sound_freq_play(HLA_F,BUSY_DELAY);
      break;
    case 5:
      sound_freq_play(0,BUSY_DELAY/2);
      break;
    case 6:
      sound_freq_play(HLA_F,BUSY_DELAY);
      break;
    case 7:
      sound_freq_play(0,BUSY_DELAY/2);
      break;
    case 8:
      sound_freq_play(HLA_F,BUSY_DELAY);
      break;
    case 9:
      sound_freq_play(0,BUSY_DELAY/2);
      break;    
    case 10:
      if(--sound_cnt == 0)
        sound_freq_stop();
      else
        sound_seq = 0;
      return;
    default:
      break;
  }
  sound_seq++;
}


/// @brief  에러음 출력 함수
/// @param  void
/// @return void
void error_tone(void)
{
  switch(sound_seq)
  {
    case 0:
      sound_freq_play(HSI_F,30);
      break;
    case 1:
      sound_freq_play(0,10);
      break;
    case 2:
      sound_freq_play(HSI_F,30);
      break;
    case 3:
      sound_freq_play(0,10);
      break;
    case 4:
      sound_freq_play(HSI_F,30);
      break;    
    case 5:
      if(--sound_cnt == 0)
        sound_freq_stop();
      else
        sound_seq = 0;
      return;
    default:
      break;
  }
  sound_seq++;
}


/// @brief  성공음 출력 함수
/// @param  void
/// @return void
void success_tone(void)
{
  switch(sound_seq)
  {
    case 0:
      sound_freq_play(MDO_F,10);
      break;
    case 1:
      sound_freq_play(MSOL_F,12);
      break;
    case 2:
      sound_freq_play(HDO_F,12);
      break;
    case 3:
      if(--sound_cnt == 0)
        sound_freq_stop();
      else
        sound_seq = 0;
      return;
    default:
      break;
  }
  sound_seq++;
}


/// @brief  성공음2 출력 함수
/// @param  void
/// @return void
void success_tone2(void)
{
  switch(sound_seq)
  {
    case 0:
      sound_freq_play(HDO_F,10);
      break;
    case 1:
      sound_freq_play(MSOL_F,12);
      break;
    case 2:
      sound_freq_play(MDO_F,12);
      break;
    case 3:
      if(--sound_cnt == 0)
        sound_freq_stop();
      else
        sound_seq = 0;
      return;
    default:
      break;
  }
  sound_seq++;
}


/// @brief 도 ~ 미 ~ 솔 ~도 을 출력하기 위한 함수.
/// @param  void
/// @return void
void initial_tone(void)
{
  switch(sound_seq)
  {
    case 0:
      sound_freq_play(MLA_F,NOTE4);
      break;
    case 1:
      sound_freq_play(HDI_F,NOTE4);
      break;
    case 2:
      sound_freq_play(HMI_F,NOTE4);
      break;
    case 3:
      sound_freq_play(HLA_F,NOTE4);
      break;
    case 4:
      if(--sound_cnt == 0)
        sound_freq_stop();
      else
        sound_seq = 0;
      return;
    default:
      break;
  }
  sound_seq++;
}


/// @brief 문이 열렸을 때의 소리를 출력하기 위한 함수.
/// @param  void
/// @return void
void open_tone(void)
{
  switch(sound_seq)
  {
    case 0:
      sound_freq_play(MLA_F,NOTE8);
      break;
    case 1:
      sound_freq_play(HDI_F,NOTE8);
      break;
    case 2:
      sound_freq_play(HMI_F,NOTE8);
      break;
    case 3:
      sound_freq_play(HLA_F,NOTE8);
      break;
    case 4:
      if(--sound_cnt == 0)
        sound_freq_stop();
      else
        sound_seq = 0;
      return;
    default:
      break;
  }
  sound_seq++;
}



/// @brief 띵동음을  소리를 출력하기 위한 함수.
/// @param  void
/// @return void
void chime_bell(void)
{
  switch(sound_seq)
  {
    case 0:
      sound_freq_play(VHDO_F,NOTE2);
      break;
    case 1:
      sound_freq_play(HSOL_F,NOTE2);
      break;
    case 2:
      sound_freq_play(VHDO_F,NOTE2);
      break;
    case 3:
      sound_freq_play(HSOL_F,NOTE2);
      break;
    case 4:
      if(--sound_cnt == 0)
        sound_freq_stop();
      else
        sound_seq = 0;
      return;
    default:
      break;
  }
  sound_seq++;
}


/// @brief 전화 벨 소리를 출력하기 위한 함수.
/// @param  void
/// @return void
void tel_bell(void)
{
  sound_freq_play(Tel[sound_seq][0],Tel[sound_seq][1]);
  if(++sound_seq > 18)
  {
    if(--sound_cnt == 0)
      sound_freq_stop();
    else
      sound_seq = 0;
  }
}


/// @brief 비상음 소리를 출력하기 위한 함수.
/// @param  void
/// @return void
void siren_melody(void)
{
  sound_freq_play(Siren[sound_seq][0],Siren[sound_seq][1]);
  if(++sound_seq > 16)
  {
    if(--sound_cnt == 0)
      sound_freq_stop();
    else
      sound_seq = 0;
  }
}



/// @brief Elize 소리를 출력하기 위한 함수.
/// @param  void
/// @return void
void elize_melody(void)
{
  sound_freq_play(FurElize[sound_seq][0],FurElize[sound_seq][1]);
  if(++sound_seq > (ELIZE_COUNT-1))
//  if(++sound_seq > (92-1))
  {
    if(--sound_cnt == 0)
      sound_freq_stop();
    else
      sound_seq = 0;
  }
}


/// @brief waltz 소리를 출력하기 위한 함수.
/// @param  void
/// @return void
void waltz_melody(void)
{
  sound_freq_play(Cookoo[sound_seq][0],Cookoo[sound_seq][1]);
  if(++sound_seq > 22)
  {
    if(--sound_cnt == 0)
      sound_freq_stop();
    else
      sound_seq = 0;
  }
}


/// @brief 옹달샘 소리를 출력하기 위한 함수.
/// @param  void
/// @return void
void ongdalsaem_melody(void)
{
  sound_freq_play(OngDalSaem[sound_seq][0],OngDalSaem[sound_seq][1]);
  if(++sound_seq > 42)
  {
    if(--sound_cnt == 0)
      sound_freq_stop();
    else
      sound_seq = 0;
  }
}


/// @brief 곰세마리 소리를 출력하기 위한 함수.
/// @param  void
/// @return void
void threebear_melody(void)
{
  sound_freq_play(ThreeBear[sound_seq][0],ThreeBear[sound_seq][1]);
  if(++sound_seq > 48)
  {
    if(--sound_cnt == 0)
      sound_freq_stop();
    else
      sound_seq = 0;
  }
}


/// @brief sound_play
/// @param  void
/// @return void
void sound_play(U16 type, U16 cnt)
{
  sound_seq = CLEAR;        
  sound_type = type;              // sound type select
  sound_cnt = cnt;
  sound_play_16ms = 0;
  SpeakerOnOff( SPK_ONOFF_ON, SPK_ONOFF_MELODY);
  //jin  BEEP = CLEAR;                // Low - Initial Value
}


/// @brief Sound Play Function
/// @param[in] freq : 주파수 입력
/// @param[in] endur : 주기 입력
/// @return void
void sound_freq_play(U16 freq, U16 endur)
{
   U16 a,b;
  //trcsr &= 0x8e;
  
  a = (freq);                /* period : 50ns * 2000 = 100us */
  b = (freq>>1);
  TMH1_PWM_DISABLE();
  trcsr = 0x00;
  trc = 0x0000;
  trcgra = a;//a;                /* period : 50ns * 2000 = 100us */
  trcgrc = b;
  trcsr = 0x00;
  TMH1_PWM_ENABLE();              // PWM Enable
  sound_play_16ms = endur;

}


/// @brief PWM을 정지시키는 함수
/// @param  void
/// @return void
void sound_freq_stop(void)
{
  SpeakerOnOff( SPK_ONOFF_OFF, SPK_ONOFF_MELODY);
  TMH1_PWM_DISABLE();
  trcgra = 0;//freq-1;                /* period : 50ns * 2000 = 100us */
  trcgrc = 0;//(freq/2)-1;    
  //TMH1_PWM_DISABLE();             // PWM Disable
  sound_type = sound_seq = sound_cnt = sound_play_16ms =  CLEAR;
}


/// @brief Sound mode에 따른 동작을 위한 함수
/// @param  void
/// @return void
void InterruptSoundMode(void)
{

  if(sound_play_16ms){
    sound_play_16ms--;
    return;
  }

  switch(sound_type)
  {
    case NO_SOUND:
      //sound_freq_stop();
      break;
    case KEY_TONE:
      key_tone();
      break;
    case BUSY_TONE:
      busy_tone();
      break;
    case ERROR_TONE:
      error_tone();
      break;
    case SUCCESS_TONE:
      success_tone();
      break;
    case END_TONE_2:
      success_tone2();
      break;
    case INITIAL_TONE:
      initial_tone();
      break;
    case OPEN_TONE:
      open_tone();
      break;
    case MAIN_BELL:
      //chime_bell();
      //tel_bell();
      //elize_melody();
      waltz_melody();
      break;
    case CHIME_BELL:
      chime_bell();
      break;
    case TEL_BELL:
      tel_bell();
      break;
    case SIREN_MELODY:
      siren_melody();
      break;
    case ELIZE_MELODY:
      elize_melody();
      break;
    case WALTZ_MELODY:
      waltz_melody();
      break;
    case ONGDALSAEM_MELODY:
      ongdalsaem_melody();
      break;
    case THREEBEAR_MELODY:
      threebear_melody();
      break;
    default:
      break;
  }
}


/// @brief 소리 선택을 하기 위한 함수.
/// @param  void
/// @return void
void sound_effect(U16 sound, U16 tCnt )
{
  if ( gLobbyHandle.BeepSoundMute == true ) return;
  sound_play(sound, tCnt);
  if(sound == NO_SOUND) sound_freq_stop();
  //else sound_mode();
}


U8 mlobbyTalkFlg = false;
U8 mMelodyFlg = false;

/// @brief 스피커를 On OFF 설정 하기 위한 함수
/// @param  tIsOn \n SPK_ONOFF_ON = On \n SPK_ONOFF_OFF = Off
/// @param  tIsLobbyTalk \n SPK_ONOFF_LOBBY = Mic \n SPK_ONOFF_MELODY = Melody
/// @return void
void SpeakerOnOff( U8 tIsOn, U8 tIsLobbyTalk )
{
  if ( tIsLobbyTalk )
  {
    mlobbyTalkFlg = tIsOn;
  }
  else
  {
    mMelodyFlg = tIsOn;
  }
  IsSpeakerOnOff();
}


/// @brief 실제 스피커를 On Off 하는 함수
/// @param  void
/// @return void
void IsSpeakerOnOff( void )
{

  if ( NULL == mlobbyTalkFlg + mMelodyFlg )
  {
    nMUTE_SPK = false;
  }
  else
  {
    nMUTE_SPK = true;
  }
}


