
/// @file   Lib_AnamProtocol.h
/// @date   2012/07/20
/// @author jinhwa Lee
/// @brief  Anam Protocol 관련 파일

#ifndef __LIB_ANAM_PROTOCOL_H___
#define __LIB_ANAM_PROTOCOL_H___


#define ANAM_GUARD_CODE_HOME_TO_MASTER  0x40
#define ANAM_GUARD_CODE_MASTER_TO_HOME  0x30

#define ANAM_FUNC_CODE_GUARD_CALL       0x20
#define ANAM_FUNC_CODE_GUARD_CALL_OK    0x21
#define ANAM_FUNC_CODE_GUARD_TALK       0x22
#define ANAM_FUNC_CODE_GUARD_OFF        0x23


#define ANAM_FUNC_CODE_LOBBY_CALL       0x70
#define ANAM_FUNC_CODE_LOBBY_CALL_OK    0x71
#define ANAM_FUNC_CODE_LOBBY_TALK       0x72
#define ANAM_FUNC_CODE_LOBBY_OFF        0x73

#define ANAM_FUNC_CODE_DOOR_OPEN        0x50


void Anam_TransmitData( U8 *tData );
void Anam_TimerInterrutp100us(void);
U8 Anam_TransmitProtocolHouse(U8 tId, U8 tCmd, U8 tLobbyID );
void Anam_TransmitProtocolLobby(U8 tLobbyID, U8 tCmd, U8 tPath, U8  tDongNoH, U8 tDongNoL, U8 tHooNoH, U8 tHooNoL );
U8 Anam_ReceiveProtocolLobby(void);


#endif  // #ifndef __LIB_ANAM_PROTOCOL_H___

