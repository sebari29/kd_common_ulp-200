
/// @file   func_protocol.h
/// @date   2012/02/14
/// @author jinhwa Lee
/// @brief  Protocol 관련 파일

#ifndef __FUNC_PROTOCOL_H__
#define __FUNC_PROTOCOL_H__

#define PSS_KD_PROTOCOL               1
#define PSS_HT_PROTOCOL               2
#define PSS_HT_PROTOCOL_2ND           3
#define PSS_ANAM_PROTOCOL             4



#define PROTOCOL_GUARD_CH0          0
#define PROTOCOL_HOME_CH1           1
#define PROTOCOL_LOBBY_CH1          2



#define PROTOCOL_SIZE_5BYTE     5
#define PROTOCOL_SIZE_8BYTE     8

#define DEF_EOD                  0xEE        //end of data

/// Device code
#define  MASTER_LOBBY       0x01
#define  GUARD_PHONE        0x02
#define  VIDEO_PHONE        0x03
#define  SLAVE_LOBBY        0x04

/// Protocol CMD
#define  CMD_CALL           0x10
#define  STATUS_CALL_ON     0x00
#define  STATUS_CALL_OFF    0x01

#define  CMD_CALL_ACK       0x11
#define  STATUS_NORMAL      0x00
#define  STATUS_BUSY        0x01

#define  CMD_TALK           0x12
#define  STATUS_OFF_HOOK    0x00
#define  STATUS_ON_HOOK     0x01

#define  CMD_OPEN           0x13
#define  STATUS_OPEN        0x00
#define  STATUS_OPEN_ACK    0x01

#define  CMD_STATUS_REQ     0x20
#define  CMD_STATUS_REQ_ACK 0x21
/*
#define  STATUS...
*/

#define  CMD_VERSION_REQ      0x22
#define  CMD_VERSION_REQ_ACK  0x23

#define  CMD_EMER             0x30
#define  CMD_EMER_ACK         0x31


#define RECEIVE_LINE_HOUSE            0
#define RECEIVE_LINE_LOBBY            1
#define RECEIVE_LINE_GUARD            2
#define RECEIVE_LINE_KEY              3
#define RECEIVE_LINE_TIMEOUT          10
#define TRANSMIT_LINE_HOUSE           4
#define TRANSMIT_LINE_LOBBY           5
#define TRANSMIT_LINE_GUARD           6

#define UART_BYTE_INTERVAL_10ms       10
#define UART_PROTOCOL_INTERVAL_20ms   20

void TransmitProtocolLobby(U8 tLobbyID, U8 tCmd, U8 tPath, U8  tDongNoH, U8 tDongNoL, U8 tHooNoH, U8 tHooNoL );
void TransmitProtocolGuardSwitch(U8 tGuardId, U8 tCmd, U8 tPath, U8  tDongNoH, U8 tDongNoL, U8 tHooNoH, U8 tHooNoL );
void TransmitProtocolHouse(U8 tId, U8 tCmd, U8 tPath, U8 tLobbyID );
void TransmitProtocolGuard(U8 tGuardId, U8 tCmd, U8 tPath, U8  tDongNoH, U8 tDongNoL, U8 tHooNoH, U8 tHooNoL );
void InitializeUart2_3906(void);
void SendMsgReceiveProtocol ( U8 *tRXData );
void SendMsgReceiveProtocol_Echo ( U8 *tRXData );
void TransmitProtocolHouseSwitch( U8 tId, U8 tCmd, U8 tPath, U8 tLobbyID );
void TransmitProtocolLobbySwitch( U8 tLobbyID, U8 tCmd, U8 tPath, U8  tDongNoH, U8 tDongNoL, U8 tHooNoH, U8 tHooNoL );

void TransmitGuard( U8 tData );
void TransmitGuardCntrRx( void );
void TransmitGuardCntrTx( void );
void TransmitHouseLobby( U8 tData );
void TransmitHouseLobbyCntrRx( void );
void TransmitHouseLobbyCntrTx( void );


#endif    //#ifndef __FUNC_PROTOCOL_H__
