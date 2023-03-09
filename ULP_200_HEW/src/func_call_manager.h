
/// @file func_call_manager.h
/// @date 2012/02/14
/// @author jinhwa Lee
/// @brief  ��ȭ ���� ��� ���

#ifndef ___FUNC_CALL_MANAGER_H___
#define ___FUNC_CALL_MANAGER_H___

#define LOBBY_ID_MASK     0x40
#define MASTER_LOBBY_ID   0x41
#define SLAVE_LOBBY1_ID   0x42
#define SLAVE_LOBBY2_ID   0x43
#define SLAVE_LOBBY3_ID   0x44

// ����ġ
#define TALK_TIME_OUT_66SEC         660 //*60*10 //600 = 1min , 600*60*5 = 5hour
#define CALL_TIME_OUT_36SEC         360 //300

// ����̽�
#define TALK_TIME_OUT_63SEC         630 //*60*10 //600 = 1min , 600*60*5 = 5hour
#define CALL_TIME_OUT_33SEC         330 //300
#define TALK_TIME_OUT_60SEC       600 //*60*10 //600 = 1min , 600*60*5 = 5hour
#define CALL_TIME_OUT_30SEC       300 //300

#define CALL_OK_TIME_OUT_3SEC    35 //30


#define MAX_CHANNEL    2
#define MAX_MUX        2
#define MAX_LINE       2

#define MUX_1 0
#define MUX_2 1

#define PATH_1  0
#define PATH_2  1

#define CHANNEL_1 0
#define CHANNEL_2 1

#define PATH_NAME_NULL           0
#define PATH_NAME_MASTER_LOBBY   1
#define PATH_NAME_SUB_LOBBY_1    2
#define PATH_NAME_SUB_LOBBY_2    3
#define PATH_NAME_SUB_LOBBY_3    4
#define PATH_NAME_SUB_LOBBY_NEXT 5
#define PATH_NAME_HOUSE           6
#define PATH_NAME_GUARD          7

#define INFO_TALK_MAX 2
#define INFO_TALK_CH0 0
#define INFO_TALK_CH1 1

#define CALL_DIRECTION_GR 1
#define CALL_DIRECTION_RG 2
#define CALL_DIRECTION_SLR 3
#define CALL_DIRECTION_SLG 4
#define CALL_DIRECTION_MLR 5
#define CALL_DIRECTION_MLG 6
#define CALL_DIRECTION_SLMLR 7
#define CALL_DIRECTION_SLMLG 8

#define INFO_TALK_STATUS_NORMAL NULL
#define INFO_TALK_STATUS_CALL_OK    1
#define INFO_TALK_STATUS_CALL       2
#define INFO_TALK_STATUS_TALK       3
#define INFO_TALK_STATUS_HOLD       4


/// @brief ���� �������� ������ ����ü
typedef struct{
  U8 DeviceID;        ///< Device ID
  U8 Cmd;             ///< Command
  U8 Path;            ///< ��ȭ Path
  U8 DongNoH_LobbyID; ///< Dong High or LobbyID
  U8 DongNoL;         ///< Dong Low
  U8 HooNoH;          ///< Hoo High
  U8 HooNoL;          ///< Hoo Low
  U8 Eof;             ///< EOF
  U8 ReceiveLine;     ///< ���ŵ� ��� ����
}typeProtocolData;


/// @brief ��ȭ ���ҽ� ���� ����ü
typedef struct{
  U8 Use;         ///< ä�� ��� ����
  U8 Channel;     ///< ä�� ��ȣ
  U8 Status;      ///< ��ȭ ����
  U8 Direction;   ///< ��ȭ ����
  U8 HouseID;     ///< ���� ID
  U8 LobbyID;     ///< �κ� ID
  U8 GuardID;     ///< ��� ID
  U8 SrcPath;     ///< �ɾ�� �� ��ȭ��
  U8 DistPath;    ///< ���� ��ȭ��
  U8 DongNoH;     ///< �� ��ȣ ����
  U8 DongNoL;     ///< �� ��ȣ ����
  U8 HooNoH;      ///< ȣ ��ȣ ����
  U8 HooNoL;      ///< ȣ ��ȣ ����
  U8 PathName[MAX_MUX];   ///< ��ȭ�� �̸�
  U8 PathNumber[MAX_MUX]; ///< ��ȭ�� ��ȣ
  //U8 GuardPath;           ///< ���� ��ȭ��
}typeInfoTalk;

////////////////////////////////////////////

/// @brief �ӽ� ��ȭ ���ҽ� ���� ����ü
typedef struct{
  U8 Channel;   ///< ä�� ��ȣ
  U8 HouseID;   ///< ���� ID
  U8 LobbyID;   ///< �κ� ID
  U8 GuardID;   ///< ��� ID
  U8 DongNoH;   ///< �� ��ȣ ����
  U8 DongNoL;   ///< �� ��ȣ ����
  U8 HooNoH;    ///< ȣ ��ȣ ����
  U8 HooNoL;    ///< ȣ ��ȣ ����
  U8 PathName[MAX_MUX];   ///< ��ȭ�� �̸�
  U8 PathNumber[MAX_MUX]; ///< ��ȭ�� ��ȣ
  //U8 GuardPath;           ///< ���� ��ȭ��
}typeUserCallResource;

////////////////////////////////////////////

/// @brief �ϵ����� 1:1�� ��Ī �Ǵ� ��ȭ ȸ�� ���� ����ü
typedef struct{
  U8 House[MAX_LINE];   ///< �����
  U8 MasterLobby;       ///< ������ �κ�
  U8 SubLobby[MAX_LINE];///< �����̺� �κ�
  U8 Guard[MAX_LINE];   ///< ���Ǳ�
  U8 IsHold;            ///< ���ҽ� ���
}typeLineResource;

////////////////////////////////////////////

#define DIR_MUX_HOUSE          0
#define DIR_MUX_SUBLOBBY       2
#define DIR_MUX_GUARD          4
#define DIR_MUX_MASTERLOBBY    6
#define DIR_MUX_NULL           7


void InitializeResourceCall(void);
U8 Anam_AssignResourceCall( typeUserCallResource *tUserCallResource, U8 tHomePath  );
U8 Anam_AssignResourceLine ( typeUserCallResource *tUserCallResource, U8 tHomePath );
U8 AssignResourceCall( typeUserCallResource *tUserCallResource );
U8 AssignResourceLine ( typeUserCallResource *tUserCallResource );
U8 ReturnResourceCall( U8 tChannel );
U8 ReturnResourceLine ( typeUserCallResource *tUserCallResource );

void SelectVideoMux( void );
void SelectCallMux( typeUserCallResource *tUserCallResource );

void SelectCameraMic( void );
void SelectRelay(void);

void ThreadAnalysisProtocol(void);
U8 CallManagerRepeater( typeProtocolData *tBuffer ); // 20ms 

void InitializeCallManager(void);
U8 CallManagerRepeaterOnHook( U8 tInfoTalkChannel ); // 20ms 
U8 CallManagerTimeOut(void);

U8 CallManagerRepeater_SubLobby( typeProtocolData *tBuffer ); // 20ms 
U8 CallManagerRequest_SubLobby( typeProtocolData *tBuffer ); // 20ms 

U8 CallManagerRepeater_GR( typeProtocolData *tPrtBuf , U8 tIndex );
U8 CallManagerRepeater_RG( typeProtocolData *tPrtBuf , U8 tIndex );
U8 CallManagerRepeater_SLR( typeProtocolData *tPrtBuf , U8 tIndex );
U8 CallManagerRepeater_SLG( typeProtocolData *tPrtBuf , U8 tIndex );
U8 CallManagerRepeater_MLR( typeProtocolData *tPrtBuf , U8 tIndex );
U8 CallManagerRepeater_MLG( typeProtocolData *tPrtBuf , U8 tIndex );
U8 CallManagerRepeater_MLR_SubLobby( typeProtocolData *tPrtBuf , U8 tIndex );
U8 CallManagerRepeater_MLG_SubLobby( typeProtocolData *tPrtBuf , U8 tIndex );

U8 TransmitProtocol_RL_CALL_REQUEST(U8 tID, U8 tCmd, U8 tPath, U8 tLobbyID );
U8 CallTest_RL_CALL_REQUEST( U8 tStart, typeProtocolData *tData );

void CallManagerRequest_RG_CALL_ON( typeUserCallResource *tUserCallResource, typeProtocolData *tBuffer );
void CallManagerRequest_GR_CALL_ON( typeUserCallResource *tUserCallResource, typeProtocolData *tBuffer );
void CallManagerRequest_LR_CALL_ON( typeUserCallResource *tUserCallResource, typeProtocolData *tBuffer );
void CallManagerRequest_LG_CALL_ON( typeUserCallResource *tUserCallResource, typeProtocolData *tBuffer );
void CallManagerRequest_GR_CALL_ON( typeUserCallResource *tUserCallResource, typeProtocolData *tBuffer );
void CallManagerRequest_ReturnBusy(typeProtocolData *tBuffer, U8 tCallDirection );

void CallManagerRequest_RegisterInfoTalk( typeUserCallResource *tUserCallResource,U8 tCallDirection );


#endif  // #ifndef ___FUNC_CALL_MANAGER_H___

