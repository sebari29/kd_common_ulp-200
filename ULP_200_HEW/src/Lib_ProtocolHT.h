/// @file   Lib_KdProtocol.h
/// @date   2012/03/12
/// @author jinhwa Lee
/// @brief  경동 프로토콜 라이브러리

#ifndef ___LIB_PROTOCOL_HT_H___
#define ___LIB_PROTOCOL_HT_H___

// 라이브러리 버전 관리
// 120312 1.0 초기 작성

typedef struct
{
  U8 ID;
  U8 Command;
  U8 PathInfo;
  U8 LobbyID;
  U8 EOD;
  U8 _reserve1;
  U8 _reserve2;
  U8 _reserve3;
}typeHtProtocolFormat5;


typedef struct
{
  U8 ID;
  U8 Command;
  U8 PathInfo;
  U8 DongNoH;
  U8 DongNoL;
  U8 HooNoH;
  U8 HooNoL;
  U8 EOD;
}typeHtProtocolFormat8;

void Ht_SetupChannel( U8 tChannel, U8 tThisID, 
                      void (*tGPrt_485TxEnabletion)(void), void (*tRx485EnableFunction)(void),
                      void (*tTxBufferRegister)(U16));
void Ht_TimerInterrupt3ms( void );
void Ht_ReceiveData( U8 tChannel, U8 tStart, U8 tRxData );
void Ht_TransmitDataL5( U8 tChannel, U8 tID, U8 tCommand, U8 tPathInfo, U8 tLobbyID );
void Ht_TransmitDataL8( U8 tChannel, U8 tID, U8 tCommand, U8 tPathInfo, U8 tDongNoH, U8 tDongNoL, U8 tHooNoH, U8 tHooNoL );



/////////////////////////////////////////////////////////////////////////////////////////////
///// Command list for 현대통신 주장치 연동  system communication ------------
/////////////////////////////////////////////////////////////////////////////////////////////
          
#define HT_GR_CALL_OK        0x2c
#define HT_INTC_ON_HOOK      0x2d    //rx, room to room(indirect) talk on-hk
#define HT_GR_CALL_ON        0x2e    //rx/S-tx, guard calling
#define HT_GR_CALL_OFF       0x2f    //rx/S-tx, guard call cancel
#define HT_GR_OFF_HOOK       0x30    //rx, guard off-hk
#define HT_GR_ON_HOOK        0x31    //rx/S-tx, guard on-hk
#define HT_GR_CBUSY          0x33    //rx, guard talk busy
#define HT_GR_LBUSY          0x34    //rx, guard line busy
#define HT_GR_HOLD_ON        0x35    //rx, guard talk hold
#define HT_GR_HOLD_OFF       0x36    //rx, guard talk hold release
#define HT_GR_INT_COM        0x37    //rx, room to room talk start
#define HT_GR_INST_ADDR      0x38    //rx, room number setting from guard
#define HT_GR_INST_OK        0x39
#define HT_GR_CHK            0x3a    //rx, guard test room
#define HT_GR_CHK_OK         0x3b    //tx, room test acknowledge

#define HT_RG_CALL_ON        0x3c    //tx, room to guard calling
#define HT_RG_CALL_OK        0x3d    //rx, room calling acknowledge
#define HT_RG_CALL_OFF       0x3e    //tx, room to guard calling cancel
#define HT_RG_OFF_HOOK       0x3f    //S-rx/tx, room talk off-hk to guard
#define HT_RG_ON_HOOK        0x40    //S-rx/tx, room talk on-hk to guard
#define HT_RG_CBUSY          0x41    //tx, room talk busy to guard
#define HT_RG_FIRE           0x42    //tx, room fire alarm
#define HT_RG_GAS            0x43    //tx, room gas alarm
#define HT_RG_BURG1          0x44    //tx, room burglar1 alarm
#define HT_RG_EMER           0x45    //S-rx/tx, room/sub-phone emergency alarm
#define HT_RG_EXIT_ON        0x46    //tx, room exit on
#define HT_RG_EXIT_OFF       0x47    //tx, room exit off
#define HT_RG_HOLD_OFF       0x48    //tx, room hold release by on-hk
#define HT_RG_LBUSY          0x49
#define HT_RG_EXIT           0x4a    //tx, room exit by guard calling

#define HT_LG_CALL_ON        0x4b
#define HT_LG_CALL_OK        0x4c
#define HT_LG_CALL_OFF       0x4d
#define HT_LG_CBUSY          0x4e
#define HT_LG_ON_HOOK        0x01   //????????????

#define HT_GL_OFF_HOOK       0x50   // == 0x50
#define HT_GL_ON_HOOK        0x51
#define HT_GL_DOPEN          0x52
#define HT_GL_CALL_ON        0x53
#define HT_GL_CALL_OFF       0x54
#define HT_GL_CBUSY          0x55
#define HT_GL_LBUSY          0x56
        
#define HT_RG_FIRE_CNCL      0x8c    //tx, room fire alarm cancel(RG_FIRE_END)  
#define HT_RG_GAS_CNCL       0x8d    //tx, room gas alarm cancel(RG_GAS_END)
#define HT_RG_BURG1_CNCL     0x8e    //tx, room burglar1 alarm cancel(RG_BURG1_END)
#define HT_RG_BURG2          0x8f    //tx, room burglar2 alarm
#define HT_RG_BURG2_CNCL     0x90    //tx, room burglar2 alarm cancel(RG_BURG2_END) 
#define HT_RG_EMER_CNCL      0x91    //tx/S-tx, room emergency alarm cancel   
#define HT_RG_BURG1_ARM      0x92    //tx, room burglar1 arming(RG_BURG1_EN)
#define HT_RG_BURG1_DISARM   0x93    //tx, room burglar1 disarming(RG_BURG1_DIS)
#define HT_RG_BURG2_ARM      0x94    //tx, room burglar2 arming(RG_BURG2_EN)
#define HT_RG_BURG2_DISARM   0x95    //tx, room burglar2 disarming(RG_BURG2_DIS)

/******************************************************************************/


#endif  // #ifndef ___LIB_PROTOCOL_HT_H___

