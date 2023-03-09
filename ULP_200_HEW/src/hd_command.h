
/// @file   hd_command.h
/// @date   2012/02/14
/// @author jinhwa Lee
/// @brief  프로토콜 커멘드 정의 파일


#ifndef __HD_COMMAND_H__
#define __HD_COMMAND_H__

/////////////////////////////////////////////////////////////////////////////////////////////
///// Command list for 대양디앤티 빌라형 로비폰  system communication ------------
/////////////////////////////////////////////////////////////////////////////////////////////

#define KD__GR_LBUSY          52    //rx, guard line busy
// 0x01 ~ 0x0a        //blank  
#define KD__GR_CALL_ON        0x0b    //rx/S-tx, guard calling
#define KD__GR_CALL_OFF       0x0c    //rx/S-tx, guard call cancel
#define KD__GR_CALL_OK        0x0d
#define KD__GR_OFF_HOOK       0x0e    //rx, guard off-hk
#define KD__GR_ON_HOOK        0x0f    //rx/S-tx, guard on-hk
#define KD__GR_CBUSY          0x10    //rx, guard talk busy
#define KD__GR_HOLD_ON        0x11    //rx, guard talk hold
#define KD__GR_HOLD_OFF       0x12    //rx, guard talk hold release
#define KD__GR_INT_COM        0x13    //rx, room to room talk start
#define KD__GR_MSG_ON         0x14    //rx, guard message waiting
#define KD__GR_MSG_OFF        0x15    
#define KD__GR_INST_ADDR      0x16    //rx, room number setting from guard
#define KD__GR_CHK            0x17    //rx, guard test room
#define KD__GR_CHK_OK         0x18    //tx, room test acknowledge
          
#define KD__RG_CALL_ON        0x1f    //tx, room to guard calling
#define KD__RG_CALL_OFF       0x20    //tx, room to guard calling cancel
#define KD__RG_OFF_HOOK       0x21    //S-rx/tx, room talk off-hk to guard
#define KD__RG_ON_HOOK        0x22    //S-rx/tx, room talk on-hk to guard
#define KD__RG_CALL_OK        0x23    //rx, room calling acknowledge
#define KD__RG_CBUSY          0x24    //tx, room talk busy to guard
#define KD__RG_HOLD_OFF       0x25    //tx, room hold release by on-hk
#define KD__RG_EXIT_ON        0x26    //tx, room exit on
#define KD__RG_EXIT_OFF       0x27    //tx, room exit off
#define KD__RG_FIRE           0x28    //tx, room fire alarm
#define KD__RG_GAS            0x29    //tx, room gas alarm
#define KD__RG_EMER           0x2a    //S-rx/tx, room/sub-phone emergency alarm
#define KD__RG_BURG1          0x2b    //tx, room burglar1 alarm
#define KD__RG_BURG2          0x2c    //tx, room burglar2 alarm
#define KD__RG_EXIT           0x37    //tx, room exit by guard calling
          
#define KD__INTC_ON_HOOK      0x38    //rx, room to room(indirect) talk on-hk
          
#define KD__LR_CALL_ON        0x3d    //rx/S-tx, lobby calling
#define KD__LR_CALL_OFF       0x3e    //rx/S-tx, lobby call cancel
#define KD__LR_CALL_OK        0x3f
#define KD__LR_ON_HOOK        0x40    //S-tx, lobby talk time over
#define KD__S_LR_ON_HOOK      0x41    //S-tx, S_ lobby talk time over
          
#define KD__RL_OFF_HOOK       0x44    //S-rx/tx, room talk off-hk to lobby
#define KD__RL_ON_HOOK        0x45    //S-rx/tx, room talk on-hk to lobby
#define KD__RL_CBUSY          0x46 
#define KD__RL_DOPEN          0x47    //S-rx/tx, room lobby open request
          
#define KD__LL_CALL_ON        0x4c
#define KD__LL_CALL_OFF       0x4d
#define KD__LL_ON_HOOK        0x4e
          
#define KD__LG_CALL_ON        0x51
#define KD__LG_CALL_OFF       0x52
#define KD__LG_CALL_OK        0x53
#define KD__LG_CBUSY          0x54
#define KD__S_LG_CALL_ON      0x55
#define KD__S_LG_CALL_OFF     0x56
#define KD__LG_ON_HOOK        0x57
          
#define KD__GL_OFF_HOOK       0x5b
#define KD__GL_ON_HOOK        0x5c
#define KD__GL_CBUSY          0x5d
#define KD__GL_DOPEN          0x5e    //off hook 상태 중 
#define KD__GL_ONE_OPEN       0x5f    //on hook 상태 중 
#define KD__GL_ONE_CLOSE      0x60    //on hook 상태 중 
#define KD__GL_ALL_OPEN       0x61    //all door 
#define KD__GL_ALL_CLOSE      0x62    //all door 
#define KD__GL_COMM_ON        0x63 
          
#define KD__RR_CALL_ON        0x6f    //tx/rx, room to room calling
#define KD__RR_CALL_OFF       0x70    //tx/rx, room to room calling cancel
#define KD__RR_CALL_OK        0x71    //rx, room to room call OK(reserved)
#define KD__RR_OFF_HOOK       0x72    //tx/rx, room to room talk start
#define KD__RR_ON_HOOK        0x73    //tx/rx, room to room talk end
#define KD__RR_CBUSY          0x74    //rx, room to room call busy
          
#define KD__RL_OPENOK         0x75    //rx, lobby dopen ack
          
#define KD__CR_CALL_ON        0x76    //rx/S-tx, central calling
#define KD__CR_CALL_OK        0x77
#define KD__CR_CALL_OFF       0x78    //rx/S-tx, central call cancel
#define KD__CR_ON_HOOK        0x79    //rx/S-tx, central talk on-hk
          
#define KD__RC_OFF_HOOK       0x7a    //S-rx/tx, room talk off-hk to Central
#define KD__RC_ON_HOOK        0x7b    //S-rx/tx, room talk on-hk to central
#define KD__RC_CBUSY          0x7c    //tx, room talk busy by guard/central
#define KD__RC_EXIT           0x7d    //tx, room exit to central
          
#define KD__RG_FIRE_CNCL      0x8c    //tx, room fire alarm cancel(RG_FIRE_END)
#define KD__RG_GAS_CNCL       0x8d    //tx, room gas alarm cancel(RG_GAS_END)
#define KD__RG_BURG1_CNCL     0x8e    //tx, room burglar1 alarm cancel(RG_BURG1_END)
#define KD__RG_BURG2_CNCL     0x90    //tx, room burglar2 alarm cancel(RG_BURG2_END)
#define KD__RG_EMER_CNCL      0x91    //tx/S-tx, room emergency alarm cancel
#define KD__RG_IN_ARM         0x92    //tx, room in arming(RG_IN_ON)
#define KD__RG_IN_DISARM      0x93    //tx, room in disarming(RG_IN_DISARM)
#define KD__RG_BURG2_ARM      0x94    //tx, room burglar2 arming(RG_BURG2_EN)
#define KD__RG_BURG2_DISARM   0x95    //tx, room burglar2 disarming(RG_BURG2_DIS)
          
//일본향 추가(050327)
#define KD__RG_BURG3          150    //tx, room burglar3 alarm
#define KD__RG_BURG3_CNCL     151    //tx, room burglar3 alarm cancel(RG_BURG3_END)
#define KD__RG_BURG4          152    //tx, room burglar4 alarm
#define KD__RG_BURG4_CNCL     153    //tx, room burglar4 alarm cancel(RG_BURG3_END)
#define KD__RN_DOPEN          154    //N-rx, ha door open tx (=0x9a)
#define KD__RN_EXIT_STS       155    //N-tx, ha exit on status tx (=0x9b)
#define KD__RN_IDLE_STS       156    //N-tx, ha idle status tx (=0x9c)
#define KD__LR_VOFF           157    //HEF-rx, ha lobby video off(=0x9d)
#define KD__LR_VOFFOK         158    //HEF-rx, ha lobby video off ack(=0x9e)
          
#define KD__RL_CALL_REQUEST   0xF0
#define KD___ON_HOOK          0xF1
#define KD__LR_ECHO           0xFE



/////////////////////////////////////////////////////////////////////////////////////////////
///// Command list for 현대통신 주장치 연동  system communication ------------
/////////////////////////////////////////////////////////////////////////////////////////////
          
          
#define HD__GR_CALL_OK        0x2c
#define HD__INTC_ON_HOOK      0x2d    //rx, room to room(indirect) talk on-hk
#define HD__GR_CALL_ON        0x2e    //rx/S-tx, guard calling
#define HD__GR_CALL_OFF       0x2f    //rx/S-tx, guard call cancel
#define HD__GR_OFF_HOOK       0x30    //rx, guard off-hk
#define HD__GR_ON_HOOK        0x31    //rx/S-tx, guard on-hk
#define HD__GR_CBUSY          0x33    //rx, guard talk busy
#define HD__GR_LBUSY          0x34    //rx, guard line busy
#define HD__GR_HOLD_ON        0x35    //rx, guard talk hold
#define HD__GR_HOLD_OFF       0x36    //rx, guard talk hold release
#define HD__GR_INT_COM        0x37    //rx, room to room talk start
#define HD__GR_INST_ADDR      0x38    //rx, room number setting from guard
#define HD__GR_INST_OK        0x39
#define HD__GR_CHK            0x3a    //rx, guard test room
#define HD__GR_CHK_OK         0x3b    //tx, room test acknowledge

#define HD__RG_CALL_ON        0x3c    //tx, room to guard calling
#define HD__RG_CALL_OK        0x3d    //rx, room calling acknowledge
#define HD__RG_CALL_OFF       0x3e    //tx, room to guard calling cancel
#define HD__RG_OFF_HOOK       0x3f    //S-rx/tx, room talk off-hk to guard
#define HD__RG_ON_HOOK        0x40    //S-rx/tx, room talk on-hk to guard
#define HD__RG_CBUSY          0x41    //tx, room talk busy to guard
#define HD__RG_FIRE           0x42    //tx, room fire alarm
#define HD__RG_GAS            0x43    //tx, room gas alarm
#define HD__RG_BURG1          0x44    //tx, room burglar1 alarm
#define HD__RG_EMER           0x45    //S-rx/tx, room/sub-phone emergency alarm
#define HD__RG_EXIT_ON        0x46    //tx, room exit on
#define HD__RG_EXIT_OFF       0x47    //tx, room exit off
#define HD__RG_HOLD_OFF       0x48    //tx, room hold release by on-hk
#define HD__RG_LBUSY          0x49
#define HD__RG_EXIT           0x4a    //tx, room exit by guard calling

#define HD__LG_CALL_ON        0x4b
#define HD__LG_CALL_OK        0x4c
#define HD__LG_CALL_OFF       0x4d
#define HD__LG_CBUSY          0x4e
//       0x4f
#define HD__LG_ON_HOOK        0x01   //????????????

#define HD__GL_OFF_HOOK       0x50   // == 0x50
#define HD__GL_ON_HOOK        0x51
#define HD__GL_DOPEN          0x52
#define HD__GL_CALL_ON        0x53
#define HD__GL_CALL_OFF       0x54
#define HD__GL_CBUSY          0x55
#define HD__GL_LBUSY          0x56
//       0x57
//       0x58
//       0x59

/* Command list for 현대통신 빌라형 로비폰  system communication ------------------------------*/
// TX : Lobby -> Room
#define HD__LR_CALL_ON        0x5a    //rx/S-tx, lobby calling
#define HD__LR_CALL_OFF       0x5c    //rx/S-tx, lobby call cancel
#define HD__LR_ON_HOOK        0x5d    //S-tx, lobby talk time over
//       0x5e
// RX : Room -> Lobby
#define HD__RL_OFF_HOOK       0x5f    //S-rx/tx, room talk off-hk to lobby
#define HD__RL_ON_HOOK        0x60    //S-rx/tx, room talk on-hk to lobby
#define HD__LR_CALL_OK        0x5b
#define HD__RL_DOPEN          0x61    //S-rx/tx, room lobby open request
#define HD__RL_CBUSY          0x62    //S-rx/tx, room busy to lobby
#define HD__RL_LBUSY          0x63    //S-rx/tx, room over room path to lobby
/*========================================================================*/
//       0x64
//       0x65
//       0x66
//       0x67
//       0x68
#define HD__LL_CALL_ON        0x69
#define HD__LL_CALL_OFF       0x6a
#define HD__LL_ON_HOOK        0x6b
#define HD__S_LR_CALL_ON      0x6c
#define HD__S_LR_CALL_OFF     0x6d
        
#define HD__CR_CALL_ON        0x6e    //rx/S-tx, central calling  
#define HD__CR_CALL_OK        0x6f    //        
#define HD__CR_CALL_OFF       0x70    //rx/S-tx, central call cancel 
#define HD__CR_ON_HOOK        0x71    //rx/S-tx, central talk on-hk
//       0x72
#define HD__RC_OFF_HOOK       0x73    //S-rx/tx, room talk off-hk to Central
#define HD__RC_ON_HOOK        0x74    //S-rx/tx, room talk on-hk to central
#define HD__RC_CBUSY          0x75    //tx, room talk busy by guard/central
#define HD__RC_EXIT           0x76    //tx, room exit to central
//       0x77
        
#define HD__MC_DONG_DATA      0x78
#define HD__MC_GDAPP_DATA     0x79
#define HD__MC_LOBY_DATA      0x7a
#define HD__MC_ROOM_DATA      0x7b
#define HD__MC_ROOM_DIAGS     0x7c
#define HD__CM_DONG_SHOW      0x7d
#define HD__CM_GDAPP_SHOW     0x7e
#define HD__CM_LOBY_SHOW      0x7f
#define HD__CM_ROOM_SHOW      0x80    
//       0x81            
                         
#define HD__CM_DONG_DATA      0x82
#define HD__CM_GDAPP_DATA     0x83
#define HD__CM_LOBY_DATA      0x84
#define HD__CM_ROOM_DATA      0x85
#define HD__CM_ROOM_DIAGS     0x86
//       0x87
//       0x88
//       0x89
//       0x8a
//       0x8b
        
#define HD__RG_FIRE_CNCL      0x8c    //tx, room fire alarm cancel(RG_FIRE_END)  
#define HD__RG_GAS_CNCL       0x8d    //tx, room gas alarm cancel(RG_GAS_END)
#define HD__RG_BURG1_CNCL     0x8e    //tx, room burglar1 alarm cancel(RG_BURG1_END)
#define HD__RG_BURG2          0x8f    //tx, room burglar2 alarm
#define HD__RG_BURG2_CNCL     0x90    //tx, room burglar2 alarm cancel(RG_BURG2_END) 
#define HD__RG_EMER_CNCL      0x91    //tx/S-tx, room emergency alarm cancel   
#define HD__RG_BURG1_ARM      0x92    //tx, room burglar1 arming(RG_BURG1_EN)
#define HD__RG_BURG1_DISARM   0x93    //tx, room burglar1 disarming(RG_BURG1_DIS)
#define HD__RG_BURG2_ARM      0x94    //tx, room burglar2 arming(RG_BURG2_EN)
#define HD__RG_BURG2_DISARM   0x95    //tx, room burglar2 disarming(RG_BURG2_DIS)
        
        
#define HD__RD_DOPEN          0xb4    //S-rx, sub-phone door open request    
#define HD__DR_CALL_ON        0xb5    //S-tx, door call wait to sub-phone    
#define HD__DR_CALL_OFF       0xb6    //S-tx, door call cancel to sub-phone   
#define HD__RD_OFF_HOOK       0xb7    //S-rx, sub-phone door talk start    
#define HD__RD_ON_HOOK        0xb8    //S-rx/tx, sub-phone door talk end    
#define HD__RD_MNT_ON         0xb9    //S-rx, sub-phone door monitoring    
#define HD__RD_MNT_OFF        0xba    //S-rx, sub-phone monitoring end    
#define HD__DR_ON_HOOK        0xbb    //S-tx, sub door talk time over     
//       0xbc
//       0xbd
        
#define HD__RR_CALL_ON        0xbe    //tx/rx, room to room calling     
           // 본, 명령은 세대통화를 요청할 때도 세대통화가 걸려 왔을 때도 통용되는 명령임
           // (앞에 //tx/rx 표기가 상기 의미를 갖고 있음.)
#define HD__RR_CALL_OFF       0xbf    //tx/rx, room to room calling cancel   
#define HD__RR_OFF_HOOK       0xc0    //tx/rx, room to room talk start    
#define HD__RR_ON_HOOK        0xc1    //tx/rx, room to room talk end
#define HD__RR_CBUSY          0xc2    //rx, room to room call busy     
#define HD__RR_CALL_OK        0xc3    //rx, room to room call OK(reserved)
        
//************************************************************************************
//;196d - 199d ; 무인택배 연동 프로토콜 (2005년 3월 7일 HYJ 추가)
//************************************************************************************
        
#define HD__CR_ATB_ARRIVE     0xc4       // 택배 도착 알림        
#define HD__CR_ATB_TAKE       0xc5       // 택배 가져감 알림        
#define HD__CR_ATB_STINIT     0xc6       // 세대 일괄 전송(이것은 동단위로 처리 해야 함)  == 0xc6
#define HD__RC_ATB_CMDACK     0xc7       // 택배 커맨드에 대한 응답      
        
//************************************************************************************
//200d - 204d ; 주차시스템 연동 프로토콜 (2005년 3월 7일 HYJ 추가)
//************************************************************************************
#define HD__RC_APK_CMDACK     0xc8               //            
#define HD__RC_APK_STREQ      0xc9               // 세대에서 현재 차량 상태 요구(?)              
#define HD__CR_APK_STINIT     0xca               // 차량 일괄 전송                               
#define HD__CR_APK_CARIN      0xcb               // 차량 도착 알림                               
#define HD__CR_APK_CAROUT     0xcc               //          
                         
#define HD__RL_CALL_REQUEST   0xF0
#define HD___ON_HOOK          0xF1
#define HD__LR_ECHO           0xFE

////////////////////////////////////////////////////////////////////////////////////////////

#endif    //#ifndef __FUNC_PROTOCOL_H__
