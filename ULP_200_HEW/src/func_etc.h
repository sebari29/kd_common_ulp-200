
/// @file   func_etc.h
/// @date   2012/02/14
/// @author jinhwa Lee
/// @brief  Door RFID 관련 파일 헤더

#ifndef ___FUNC_ETC_H___
#define ___FUNC_ETC_H___

#define RF_READER_CMD_NORMAL_MODE               0x10
#define RF_READER_CMD_NORMAL_SUCCESS            0x11
#define RF_READER_CMD_NORMAL_FAIL               0x12
#define RF_READER_CMD_NORMAL_MASTERCARD         0x13

#define RF_READER_CMD_HOUSE_REGISTER_MODE       0x20
#define RF_READER_CMD_HOUSE_REGISTER_SUCCESS    0x21
#define RF_READER_CMD_HOUSE_REGISTER_FAIL       0x22

#define RF_READER_CMD_MASTER_REGISTER_MODE      0x30
#define RF_READER_CMD_MASTER_REGISTER_SUCCESS   0x31
#define RF_READER_CMD_MASTER_REGISTER_FAIL      0x32

#define RF_READER_CMD_HOUSE_DELETE_MODE         0x40
#define RF_READER_CMD_HOUSE_DELETE_SUCCESS      0x41
#define RF_READER_CMD_HOUSE_DELETE_FAIL         0x42


#define RF_READER_CMD_SUCCESS_MARK              0x01
#define RF_READER_CMD_FAIL_MARK                 0x02

#define RF_READER_MODE_NORMAL   0
#define RF_READER_MODE_WAIT     1
#define RF_READER_MODE_HOUSE    2
#define RF_READER_MODE_DELETE   3
#define RF_READER_MODE_MASTER   4


#define RF_READER_CMD_INFOMATION_MODE         0x80
#define RF_READER_CMD_INFOMATION_L            0x90
#define RF_READER_CMD_INFOMATION_C            0xA0
#define RF_READER_CMD_INFOMATION_H            0xB0
#define RF_READER_CMD_INFOMATION_VER          0xC0

#define RF_READER_CMD_SOUND_ON                0x50
#define RF_READER_CMD_SOUND_OFF               0x60
#define RF_READER_CMD_ALL_DELETE              0x70

void ThreadDoorOpen(void);
void SendMsgRFCard( U8 tIsRX, U8 *tData );
void ThreadRFCard(void);
void InitalizeRFCard(void);

U8  Chr2Num( U8 tChr );
U16 Dec2Hex( U8 tDec );
U8 Dec2HalfHex( U16 tDec, U16 *tHalfHex );
void HalfHex2Dec( U16 bHalfHex, U16 *bDec );
void MsgDoorOpenfunc(void);
U8 ProtocolSpecSetting(void);

#endif  // #ifndef ___FUNC_MESSAGE_H___

