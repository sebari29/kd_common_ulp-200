
/// @file   func_sound.c
/// @date   2012/02/14
/// @author jinhwa Lee
/// @brief  ������ ��� ���� ����

#include "main.h"

#define ELIZE_COUNT     81
#define NOTE8_2         18
#define BUSY_DELAY      28

const U16 FurElize[ELIZE_COUNT][2]=  
{
  HMI_F,  NOTE8_2,  // ��    1
  HRI_F,  NOTE8_2,  // �޷�
  HMI_F,  NOTE8_2,  // ��    2
  HRI_F,  NOTE8_2,  // �޷�
  HMI_F,  NOTE8_2,  // ��
  MSI_F,  NOTE8_2,  // �Ʒ� ��
  HRE_F,  NOTE8_2,  // ��
  HDO_F,  NOTE8_2,  // ��
  MLA_F,  NOTE8_2*2,// �Ʒ� ��  3
  0,  NOTE8_2,      // ��ǥ
  MDO_F,  NOTE8_2,  // �Ʒ� ��
  MMI_F,  NOTE8_2,  // �Ʒ� ��
  MLA_F,  NOTE8_2,  // �Ʒ� ��
  MSI_F,  NOTE8_2*2,// �Ʒ� ��  4
  0,  NOTE8_2,      // ��ǥ
  MMI_F,  NOTE8_2,  // �Ʒ� ��
  MSIL_F,  NOTE8_2, // �Ʒ� �޼�
  MSI_F,  NOTE8_2,  // �Ʒ� ��
  HDO_F,  NOTE8_2*2,// ��    5
  0,  NOTE8_2,      // ��ǥ
  MMI_F,  NOTE8_2,  // �Ʒ� ��
  HMI_F,  NOTE8_2,  // ��
  HRI_F,  NOTE8_2,  // �� ��
  HMI_F,  NOTE8_2,  // ��    6
  HRI_F,  NOTE8_2,  // �� ��
  HMI_F,  NOTE8_2,  // ��
  MSI_F,  NOTE8_2,  // �Ʒ� ��
  HRE_F,  NOTE8_2,  // ��
  HDO_F,  NOTE8_2,  // ��
  MLA_F,  NOTE8_2*2,// �Ʒ� ��  7
  0,  NOTE8_2,      // ��ǥ
  MDO_F,  NOTE8_2,  // �Ʒ� ��
  MMI_F,  NOTE8_2,  // �Ʒ� ��
  MLA_F,  NOTE8_2,  // �Ʒ� ��
  MSI_F,  NOTE8_2*2,// �Ʒ� ��  8
  0,  NOTE8_2,      // ��ǥ
  MMI_F,  NOTE8_2,  // �Ʒ� ��
  HDO_F,  NOTE8_2,  // ��
  MSI_F,  NOTE8_2,  // �Ʒ� ��
  MLA_F,  NOTE8_2*4,// �Ʒ� ��  9
  0,  NOTE8_2*2,    // ��ǥ
  HMI_F,  NOTE8_2,  // ��    10
  HRI_F,  NOTE8_2,  // �޷�
  HMI_F,  NOTE8_2,  // ��
  MSI_F,  NOTE8_2,  // �Ʒ� ��
  HRE_F,  NOTE8_2,  // ��
  HDO_F,  NOTE8_2,  // ��
  MLA_F,  NOTE8_2*2,// �Ʒ� ��  11
  0,  NOTE8_2,      // ��ǥ
  MDO_F,  NOTE8_2,  // �Ʒ� ��
  MMI_F,  NOTE8_2,  // �Ʒ� ��
  MLA_F,  NOTE8_2,  // �Ʒ� ��
  MSI_F,  NOTE8_2*2,// �Ʒ� ��  12
  0,  NOTE8_2,      // ��ǥ
  MMI_F,  NOTE8_2,  // �Ʒ� ��
  MSIL_F,  NOTE8_2, // �Ʒ� �޼�
  MSI_F,  NOTE8_2,  // �Ʒ� ��
  HDO_F,  NOTE8_2*2,// ��    13
  0,  NOTE8_2,      // ��ǥ
  MMI_F,  NOTE8_2,  // �Ʒ� ��
  HMI_F,  NOTE8_2,  // ��
  HRI_F,  NOTE8_2,  // �� ��
  HMI_F,  NOTE8_2,  // ��    14
  HRI_F,  NOTE8_2,  // �� ��
  HMI_F,  NOTE8_2,  // ��
  MSI_F,  NOTE8_2,  // �Ʒ� ��
  HRE_F,  NOTE8_2,  // ��
  HDO_F,  NOTE8_2,  // ��
  MLA_F,  NOTE8_2*2,// �Ʒ� ��  15
  0,  NOTE8_2,      // ��ǥ
  MDO_F,  NOTE8_2,  // �Ʒ� ��
  MMI_F,  NOTE8_2,  // �Ʒ� ��
  MLA_F,  NOTE8_2,  // �Ʒ� ��
  MSI_F,  NOTE8_2*2,// �Ʒ� ��  16
  0,  NOTE8_2,      // ��ǥ
  MMI_F,  NOTE8_2,  // �Ʒ� ��
  HDO_F,  NOTE8_2,  // ��
  MSI_F,  NOTE8_2,  // �Ʒ� ��
  MLA_F,  NOTE8_2*4,// �Ʒ� ��  17  // 78
  0,  NOTE8_2       // ��ǥ
/*
  NOTE16,  NOTE8_2, // ��ǥ
  MSI_F,  NOTE8_2,  // �Ʒ� ��
  HDO_F,  NOTE8_2,  // ��
  HRE_F,  NOTE8_2,  // ��    // 82
*/
/*
  HMI_F,  NOTE8_2*3,// ��    18
  MSI_F,  NOTE8_2,  // �Ʒ� ��
  HFA_F,  NOTE8_2,  // ��
  HMI_F,  NOTE8_2,  // ��

  HRE_F,  NOTE8_2*3,  //      19
  MFA_F,  NOTE8_2,
  HMI_F,  NOTE8_2,
  HRE_F,  NOTE8_2,

  HDO_F,  NOTE8_2*3,  //      20
  MMI_F,  NOTE8_2,
  HRE_F,  NOTE8_2,
  HDO_F,  NOTE8_2,

  MSI_F,  NOTE8_2*2,  //      21
  NOTE16,  NOTE8_2,  // ��ǥ
  MMI_F,  NOTE8_2,
  HMI_F,  NOTE8_2,
  NOTE16,  NOTE8_2,  // ��ǥ

  NOTE16,  NOTE8_2,  // ��ǥ    22
  HMI_F,  NOTE8_2,
  VHMI_F,  NOTE8_2,
  NOTE16,  NOTE8_2,  // ��ǥ
  NOTE16,  NOTE8_2,  // ��ǥ
  HRI_F,  NOTE8_2,

  HRE_F,  NOTE8_2*2,  //      23
  NOTE16,  NOTE8_2,  // ��ǥ
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
  0,         48  // ��ǥ
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


/// @brief  ������ ��� �Լ� Initialize
/// @param  void
/// @return void
void InitializeSound(void)
{
   __InitializePWM();
   SpeakerOnOff( SPK_ONOFF_ON, SPK_ONOFF_MELODY);
}


/// @brief  ��ư�� ��� �Լ�
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


/// @brief  ������ ��� �Լ�
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


/// @brief  ������ ��� �Լ�
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


/// @brief  ������ ��� �Լ�
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


/// @brief  ������2 ��� �Լ�
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


/// @brief �� ~ �� ~ �� ~�� �� ����ϱ� ���� �Լ�.
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


/// @brief ���� ������ ���� �Ҹ��� ����ϱ� ���� �Լ�.
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



/// @brief ������  �Ҹ��� ����ϱ� ���� �Լ�.
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


/// @brief ��ȭ �� �Ҹ��� ����ϱ� ���� �Լ�.
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


/// @brief ����� �Ҹ��� ����ϱ� ���� �Լ�.
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



/// @brief Elize �Ҹ��� ����ϱ� ���� �Լ�.
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


/// @brief waltz �Ҹ��� ����ϱ� ���� �Լ�.
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


/// @brief �˴޻� �Ҹ��� ����ϱ� ���� �Լ�.
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


/// @brief �������� �Ҹ��� ����ϱ� ���� �Լ�.
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
/// @param[in] freq : ���ļ� �Է�
/// @param[in] endur : �ֱ� �Է�
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


/// @brief PWM�� ������Ű�� �Լ�
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


/// @brief Sound mode�� ���� ������ ���� �Լ�
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


/// @brief �Ҹ� ������ �ϱ� ���� �Լ�.
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

/// @brief ����Ŀ�� On OFF ���� �ϱ� ���� �Լ�
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


/// @brief ���� ����Ŀ�� On Off �ϴ� �Լ�
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


