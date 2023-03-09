/// @file   Lib_KdProtocol.h
/// @date   2012/03/12
/// @author jinhwa Lee
/// @brief  �浿 �������� ���̺귯��

#ifndef ___LIB_PROTOCOLKD_H___
#define ___LIB_PROTOCOLKD_H___

// ���̺귯�� ���� ����
// 120312 1.0 �ʱ� �ۼ�

// -----------------------------------------------------------------------------
// Queue -----------------------------------------------------------------------
// -----------------------------------------------------------------------------

/*
����>
*/


  // KD Protocol //
  ///////////////////////////////////////////////////////////////////////////////////////////
  //        HEADER = 5             /           ID = 10           /    INFO = 2  / DATA ... //
  //---------------------------------------------------------------------------------------//
  // STX / DEV / SubID / CMD / Len / TXDEV / RXDEV / TXID / RXID / CHAN / VEDIO / DATA ... //
  //---------------------------------------------------------------------------------------//
  //  1  /  1  /   1   /  1  /  1  /   1   /   1   /  4   /   4  /   1  /   1   / LEN  ... //
  ///////////////////////////////////////////////////////////////////////////////////////////



#define COUNT_CHANNEL_KDP        3
#define KD_DATA_FIELD_SIZE      20

typedef struct 
{
  U8 Device;
  U8 SubID;
  U8 Command; 
  U8 Length;
} __typeKdProtocolHeader;

typedef struct
{
  U8 TxDevice;
  U8 RxDevice;
  
  U8 TxDongIDHi;
  U8 TxDongIDLo;
  U8 TxHooIDHi;
  U8 TxHooIDLo;
  
  U8 RxDongIDHi;
  U8 RxDongIDLo;
  U8 RxHooIDHi;
  U8 RxHooIDLo;
} __typeKdProtocolID;

typedef struct
{
  U8 SlotID_Channel;
  U8 Video;
} __typeKdProtocolChannel;


typedef struct
{
  __typeKdProtocolHeader  Header;
  __typeKdProtocolID      ID;
  __typeKdProtocolChannel Channel;
  U8 Data[KD_DATA_FIELD_SIZE];
} typeKdProtocol;


void Kd_Initialize(void);
void Kd_SetupChannel( U8 tChannel, U16 tBaudrate,
                      U8 tThisDevice, U8 tThisSubID, 
                      void (*tTx485Enable)(void), void (*tRx485Enable)(void),
                      void (*tTxBufferReg)(U8));

U8 Kd_TransmitData( U8 tChannel, U8 tCommand, U8 *tBuffer, U8 tLength, U8 tWaitTime );
U8 Kd_TransmitDataRawData(U8 tChannel,U8 tDevice, U8 tSubID, U8 tCommand, U8 *tBuffer, U8 tLength, U8 tWaitTime100us );
void Kd_ReceiveData( U8 tChannel, U8 tRxData );
void Kd_TimerInterrupt300us( void );



///////////////////////////////////////////////////////////////////////////////
// Protocol Command
////////////////////////////////////////////////////////////////////////////////

/* Device ID Define */
#define KD_PRT_DEVICE_MAINDEVICE            0x00    // ����ġ ����̽� ���̵�
#define KD_PRT_DEVICE_HOUSE                 0x01    // ����� ����̽� ���̵�
#define KD_PRT_DEVICE_LOBBY                 0x02    // �κ� ����̽� ���̵�
#define KD_PRT_DEVICE_GUARD                 0x03    // ��� ����̽� ���̵�
#define KD_PRT_DEVICE_ALL                   0x04    // ��� ����̽� ���̵�
#define KD_PRT_DEVICE_PC                    0x30    // PC

/****************************** Command Define ********************************/
/* Device Comunication */
#define KD_PRT_CMD_CALL_ON                  0x01    // ȣ�� ����
#define KD_PRT_CMD_CALL_OK                  0x02    // ȣ�� ����
#define KD_PRT_CMD_CALL_BUSY                0x03    // ��ȭ��
#define KD_PRT_CMD_TALK_ON                  0x04    // ��ȭ ����
#define KD_PRT_CMD_ALL_OFF                  0x05    // ȣ��/��ȭ ����
#define KD_PRT_CMD_DIRECT_TALK_CONVERSION_TX     0x06 // ������ȭ ��ȯ ����
#define KD_PRT_CMD_GUARD_CONVERSION_CALL_REQUEST 0x07 // ��� ��ȯ ȣ�� ��û
#define KD_PRT_CMD_TALK_WAIT                0x08 // ��ȭ ���
#define KD_PRT_CMD_CALL_BUSY_ETC            0x09 // ��Ÿ ��Ȳ ��ȭ��
#define KD_PRT_CMD_LINE_TEST_ON             0x0F


/* Function & Error */
#define KD_PRT_CMD_DOOR_OPEN                0x11    // ������
#define KD_PRT_CMD_FIRE_DOOR_OPEN           0x12    // ȭ�� ������
#define KD_PRT_CMD_FIRE_DOOR_CLOSE          0x13    // ȭ�� ������
#define KD_PRT_CMD_AUTO_CALL                0x14    // ���� �� ��û
#define KD_PRT_CMD_TEMINAL_USE_REQUEST      0x15    // ����̺� ��û
#define KD_PRT_CMD_TEMINAL_USE_DATA_TX      0x16    // ����̺� ����
#define KD_PRT_CMD_TEMINAL_SETUP_TX         0x17    // �ܸ��� ���� ��ȸ
#define KD_PRT_CMD_TEMINAL_SETUP_INQUIRY    0x18    // �ܸ��� ���� ����
#define KD_PRT_CMD_TEMINAL_SETUP_CHANGE     0x19    // �ܸ��� ���� ����
#define KD_PRT_CMD_CHANNEL_TEST             0x1A    // ä�� TEST
#define KD_PRT_CMD_SECURITY_DATA_COM_REQUEST 0x1B    // ���� DATA
#define KD_PRT_CMD_SECURITY_DATA_RESULT_TX  0x1C    // ���� DATA 
#define KD_PRT_CMD_ERROR                    0x1F    // ����

/* Event */
#define KD_PRT_CMD_OUT                      0x21    // ����
#define KD_PRT_CMD_HOME                     0x22    // ����
#define KD_PRT_CMD_FIRE                     0x23    // ȭ��
#define KD_PRT_CMD_GAS                      0x24    // ����
#define KD_PRT_CMD_EMERGENCY                0x25    // ���
#define KD_PRT_CMD_BURGALR1                 0x26    // ���1
#define KD_PRT_CMD_BURGALR2                 0x27    // ���2
#define KD_PRT_CMD_BURGALR3                 0x28    // ���3
#define KD_PRT_CMD_BURGALR4                 0x29    // ���4

/* Setup & Delete & Info 1 */
#define KD_PRT_CMD_EPASS_SETUP              0x31    // ������ P/W ���� �� �ʱ�ȭ
#define KD_PRT_CMD_CPASS_SETUP              0x32    // ���� P/W ���� �� �ʱ�ȭ
#define KD_PRT_CMD_RF_SETUP                 0x33    // RFī�� ���� �� ����/�ʱ�ȭ
#define KD_PRT_CMD_PHONE_SETUP              0x34    // ��ȭ��ȣ ���� �� �ʱ�ȭ
#define KD_PRT_CMD_SECURITY_INFO_INQUIRY    0x35    // ���� ���� ��ȸ
#define KD_PRT_CMD_SECURITY_INFO_TX         0x36    // ���� ���� ����
#define KD_PRT_CMD_MAIN_VERSION_INQUIRY     0x37    // ����ġ ���� ��ȸ
#define KD_PRT_CMD_MAIN_VERSION_TX          0x38    // ����ġ ���� ����
#define KD_PRT_CMD_EVENT_INFO_INQUIRY       0x39    // �̺�Ʈ ���� ��ȸ
#define KD_PRT_CMD_EVENT_INFO_DELETE        0x3A    // �̺�Ʈ ���� ����
#define KD_PRT_CMD_EVENT_INFO_TX            0x3B    // �̺�Ʈ ���� ����

/* Setup & Delete & Info 2 */
#define KD_PRT_CMD_APT_SETUP                0x41    // ���� ����
#define KD_PRT_CMD_APT_INQUIRY              0x42    // ���� ��ȸ
#define KD_PRT_CMD_APT_INFO_TX              0x43    // ���� ���� ����
#define KD_PRT_CMD_MAIN_INFO_SETUP          0x44    // ����ġ ���� ���� �� ����
#define KD_PRT_CMD_MAIN_INFO_INQUIRY        0x45    // ����ġ ���� ��ȸ
#define KD_PRT_CMD_MAIN_INFO_TX             0x46    // ����ġ ���� ����
#define KD_PRT_CMD_MAIN_BACKUP_INFO_INQUIRY 0x47    // ����ġ ��� ���� ��ȸ
#define KD_PRT_CMD_MAIN_BACKUP_INFO_TX      0x48    // ����ġ ��� ���� ����
#define KD_PRT_CMD_GUARD_CON_INQUIRY        0x49    // ���� ��ȯ ��ȸ
#define KD_PRT_CMD_GUARD_CON_INQUIRY_TX     0x4A    // ���� ��ȯ ��ȸ ����
#define KD_PRT_CMD_SETUP_OK                 0x4B    // ���� OK
#define KD_PRT_CMD_DELETE_OK                0x4C    // ���� OK
#define KD_PRT_CMD_SETUP_FAIL               0x4D    // ���� FAIL
#define KD_PRT_CMD_DELETE_FAIL              0x4E    // ���� FAIL
#define KD_PRT_CMD_INQUIRY_FAIL             0x4F    // ��ȸ FAIL

/* Etc */
#define KD_PRT_CMD_DONG_INFO_REQUEST        0x51    // �� ���� ��û/ ����ī�� ���� ����
#define KD_PRT_CMD_DONG_INFO_TX             0x52    // �� ���� ����
#define KD_PRT_CMD_CH_INFO_REQUEST          0x53    // ä�� ���� ��û
#define KD_PRT_CMD_CH_INFO_TX               0x54    // ä�� ���� ����
#define KD_PRT_CMD_MONITER_REQUEST          0x55    // ����� ��û
#define KD_PRT_CMD_MONITER_DATA_TX          0x56    // ����� ������ ����
#define KD_PRT_CMD_TIME_INQ                 0x57    // �ð� ��û 
#define KD_PRT_CMD_TIME_SETUP               0x58    // �ð� ����

////////////////////////////////////////////////////////////////////////////////
U8 KDP_TransmitProtocol(U8 tId, U8 tCmd, U8 tChannel, U8 DongNoH_LobbyID, U8 DongNoL, U8 HooNoH, U8 HooNoL);
void KDP_LobbySetupChannel(void);

#endif  // #ifndef ___LIB_PROTOCOLKD_H___

