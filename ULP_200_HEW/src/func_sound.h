
/// @file   func_sound.h
/// @date   2012/02/14
/// @author jinhwa Lee
/// @brief  ∫Ò«¡¿Ω √‚∑¬ ∞¸∑√ ∆ƒ¿œ

#ifndef ___FUNC_SOUND_H___
#define ___FUNC_SOUND_H___

#include "dev_pwm.h"

#define TMH1_PWM_ENABLE()   __TMH1_PWM_ENABLE() 
#define TMH1_PWM_DISABLE()  __TMH1_PWM_DISABLE()

#define NO_SOUND          0
#define KEY_TONE          1
#define ERROR_TONE        2
#define SUCCESS_TONE      3
#define INITIAL_TONE      4
#define CHIME_BELL        5
#define TEL_BELL          6
#define SIREN_MELODY      7
#define ELIZE_MELODY      8
#define WALTZ_MELODY      9
#define ONGDALSAEM_MELODY 10
#define THREEBEAR_MELODY  11
#define BUSY_TONE         13
#define OPEN_TONE         14
#define MAIN_BELL         15
#define END_TONE_2     16

#define BATCH_MULT_VAL  8
#define BATCH_MULT_VAL2 1


// CMP01 = (4MHz(64∫–¡÷) 62,500Hz) / melody Hz 
//2ø¡≈∏∫Í 
#define VLRA_F    568 * BATCH_MULT_VAL
#define VLRI_F    536 * BATCH_MULT_VAL
#define VLSI_F    506 * BATCH_MULT_VAL
//3ø¡≈∏∫Í 
#define LDO_F    480 * BATCH_MULT_VAL
#define LDI_F    453 * BATCH_MULT_VAL
#define LRE_F    428 * BATCH_MULT_VAL
#define LRI_F    403 * BATCH_MULT_VAL
#define LMI_F    381 * BATCH_MULT_VAL
#define LFA_F    359 * BATCH_MULT_VAL
#define LFI_F    338 * BATCH_MULT_VAL
#define LSOL_F    319 * BATCH_MULT_VAL
#define LSIL_F    301 * BATCH_MULT_VAL
#define LLA_F    284 * BATCH_MULT_VAL
#define LLI_F    268 * BATCH_MULT_VAL
#define LSI_F    253 * BATCH_MULT_VAL
//4ø¡≈∏∫Í 
#define MDO_F    239 * BATCH_MULT_VAL
#define MDI_F    225 * BATCH_MULT_VAL
#define MRE_F    213 * BATCH_MULT_VAL
#define MRI_F    201 * BATCH_MULT_VAL
#define MMI_F    190 * BATCH_MULT_VAL
#define MFA_F    179 * BATCH_MULT_VAL
#define MFI_F    169 * BATCH_MULT_VAL
#define MSOL_F    159 * BATCH_MULT_VAL
#define MSIL_F    150 * BATCH_MULT_VAL
#define MLA_F    142 * BATCH_MULT_VAL
#define MLI_F    134 * BATCH_MULT_VAL
#define MSI_F    127 * BATCH_MULT_VAL
// 5ø¡≈∏∫Í
#define HDO_F    119 * BATCH_MULT_VAL
#define HDI_F    113 * BATCH_MULT_VAL
#define HRE_F    106 * BATCH_MULT_VAL
#define HRI_F    100 * BATCH_MULT_VAL
#define HMI_F    95 * BATCH_MULT_VAL
#define HFA_F    89 * BATCH_MULT_VAL
#define HFI_F    84 * BATCH_MULT_VAL
#define HSOL_F    80 * BATCH_MULT_VAL
#define HSIL_F    75 * BATCH_MULT_VAL
#define HLA_F    71 * BATCH_MULT_VAL
#define HLI_F    67 * BATCH_MULT_VAL
#define HSI_F    63 * BATCH_MULT_VAL
// 6ø¡≈∏∫Í
#define VHDO_F    60 * BATCH_MULT_VAL
#define VHDI_F    56 * BATCH_MULT_VAL
#define VHRE_F    53 * BATCH_MULT_VAL
#define VHRI_F    50 * BATCH_MULT_VAL
#define VHMI_F    47 * BATCH_MULT_VAL
#define VHFA_F    45 * BATCH_MULT_VAL
#define VHFI_F    42 * BATCH_MULT_VAL
#define VHSOL_F    40 * BATCH_MULT_VAL
#define VHSIL_F    38 * BATCH_MULT_VAL
#define VHLA_F    36 * BATCH_MULT_VAL
#define VHLI_F    34 * BATCH_MULT_VAL
#define VHSI_F    32 * BATCH_MULT_VAL

// 7ø¡≈∏∫Í
#define VVHDO_F    30 * BATCH_MULT_VAL





#define NOTE16    8 * BATCH_MULT_VAL2
#define NOTE12   12 * BATCH_MULT_VAL2
#define NOTE8    16 * BATCH_MULT_VAL2
#define NOTE6    24 * BATCH_MULT_VAL2
#define NOTE4    32 * BATCH_MULT_VAL2
#define NOTE3    48 * BATCH_MULT_VAL2
#define NOTE2    64 * BATCH_MULT_VAL2
#define NOTE1    96 * BATCH_MULT_VAL2

#define NOTE_SHORT 12




/* sound.c  extern Function definition  -------------------------------*/
void sound_stop(void);
void sound_play(U16 type, U16 cnt);
void sound_freq_play(U16 freq, U16 endur);
void sound_freq_stop(void);
void key_tone(void);
void error_tone(void);
void success_tone(void);
void initial_tone(void);
void tel_bell(void);
void siren_melody(void);
void elize_melody(void);
void waltz_melody(void);
void ongdalsaem_melody(void);
void threebear_melody(void);
void InterruptSoundMode(void);
void sound_effect(U16 sound, U16 tCnt );


#define SPK_ONOFF_ON  1
#define SPK_ONOFF_OFF 0
#define SPK_ONOFF_LOBBY  1
#define SPK_ONOFF_MELODY 0 

void SpeakerOnOff( U8 tIsOn, U8 tIsLobbyTalk );
void IsSpeakerOnOff(void);
void InitializeSound(void);


#endif  // #ifndef ___FUNC_SOUND_H___

