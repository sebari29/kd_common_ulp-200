
/// @file   func_message.c
/// @date   2012/02/14
/// @author jinhwa Lee
/// @brief  ť / �޽��� ��� ���� ����


#include "main.h"


/*******************************************/
/*************** RingBuffer ****************/
/*******************************************/


/// @brief �� ���� �ʱ�ȭ �Լ�
/// @param pQue �ʱ�ȭ�� ������ ������
/// @param tData �ʱ�ȭ �� ������ ������ ������
/// @param tBufferLength ������ ������ �迭 ����
/// @return void
void InitializeCircularBuffer( typeCircularBuffer* pQue, U8* tData, U8 tBufferLength )
{
  pQue->Data = tData;
  pQue->BufferLength = tBufferLength;
  pQue->WritePointer = 0;
  pQue->ReadPointer  = 0;
  pQue->Lock = false;

}

/*
void ClearCircularBuffer( typeCircularBuffer* pQue )
{
  U8 i;
  for ( i = 0; i < pQue->BufferLength ; i++ )
  {
    pQue->Data[i] = 0;
  }
  pQue->WritePointer = 0;
  pQue->ReadPointer  = 0;

}
*/


/// @brief �����۰� �� á���� �����ϴ� �Լ�
/// @param que ������ �� ���� ������
/// @return True = Buffer Full \n False = Free
U8 IsFullCircularBuffer( typeCircularBuffer* que )
{
    return (U8)(((que->WritePointer + 1) % que->BufferLength) == que->ReadPointer);
}


/// @brief �����۰� ��� �ִ��� �����ϴ� �Լ�
/// @param que ������ �� ���� ������
/// @return True = Buffer Empty \n False = Buffer�� ������ ����
U8 IsEmptyCircularBuffer( typeCircularBuffer* que )
{
    return (U8)(que->ReadPointer == que->WritePointer);
}


/// @brief �� ���� ������ �Է� �Լ�
/// @param que �ʱ�ȭ�� ������ ������
/// @param tdata �Է��� ������
/// @return True = Buffer Full Error \n False = Success
U8 EnqueCircularBuffer( typeCircularBuffer* que, U8 tdata )
{
    U8 isFull;
    
    wdt_reset();

    while ( que->Lock ){
    };

    que->Lock = true;
    
    isFull = IsFullCircularBuffer(que);
    if(!isFull)
    {
        que->Data[que->WritePointer] = tdata;
        que->WritePointer++;
        que->WritePointer %= que->BufferLength;
    }

    que->Lock = false;
    
    return isFull;
}


/// @brief �� ���� ������ ���
/// @param[in] que �ʱ�ȭ�� ������ ������
/// @param[out] tdata ��µ� ������ ������
/// @return True = Buffer Empty Error \n False = Success
U8 DequeCircularBuffer(typeCircularBuffer* que, U8* tdata)
{
    U8 isEmpty;

    if ( que->Lock ) return true;

    que->Lock = true;

    isEmpty =  IsEmptyCircularBuffer(que);
    if(!isEmpty)
    {
        *tdata = que->Data[que->ReadPointer];
        que->ReadPointer++;
        que->ReadPointer %= que->BufferLength;
    }

    que->Lock = false;
    return(isEmpty);
}
 



/*******************************************/
/*************** RingBuffer ****************/
/*******************************************/
//////////////// message queue ////////////////////
/*
typedef struct 
{
  //U8  MsgIndex[MSG_BOX_LENGTH];   // �޽��� ������ ���� �Ѵ�. �Ʒ� ������� �޸� ������ ������ �Ѵ�.
  U8  DstID[MSG_BOX_LENGTH];      // �� ������ ���� �������� ���� �ȴ�.
  U8  SrcID[MSG_BOX_LENGTH];      // �� ������ ���� �������� ���� �ȴ�.
  U8  Buffer[MSG_BOX_LENGTH][MSG_BUFFER_LENGTH];  // �� ������ ���� �������� ���� �ȴ�.
} tMsgBox;
*/
///////////////////////////////

typeMsgBox  gMsgBox[MSG_BOX_LENGTH];

/// @brief �޽��� �ڽ� �ʱ�ȭ �Լ�
/// @param void
/// @return void
void InitializeMessageBox(void)
{
  U8 i,j;
  for( i = 0 ; i < MSG_BOX_LENGTH ; i++ )
  {
    gMsgBox[i].DstID = MSG_NULL;
    gMsgBox[i].SrcID = MSG_NULL;

    for( j = 0; j < (MSG_BUFFER_LENGTH); j++)
    {
      gMsgBox[i].Buf.Byte[j] = 0;
    }
  }
}

void ResetMessageBox(void)
{
  InitializeMessageBox();
}

/// @brief �޽��� �ڽ��� �޽��� ���� �Լ�
/// @param[in] tSendMsg ������ �޽���
/// @return True = Success \n False = Error
U8 bSendMessageBox( typeMsgBox* tSendMsg )
{
  U8 i,j;

  for( i = 0; i < MSG_BOX_LENGTH; i++)
  {
    if( gMsgBox[i].DstID == MSG_NULL ) // ���۰� ��� �ֳ�?
    {
      gMsgBox[i].DstID = tSendMsg->DstID;
      gMsgBox[i].SrcID = tSendMsg->SrcID;
      gMsgBox[i].Cmd = tSendMsg->Cmd ;
      for( j = 0; j < (MSG_BUFFER_LENGTH / 2) ; j++ )
      {
        gMsgBox[i].Buf.Word[j] = tSendMsg->Buf.Word[j];
      }
      return true;
    }
  }
  //while(1);
  ResetMessageBox();
  
  return FALSE;
}


/*
U8 bSendMessageBoxData( U8 tDstID, U8 tSrcID, U8 tCmd, U8 tMsgData0, U8 tMsgData1, U8 tMsgData2, U8 tMsgData3 )
{
  typeMsgBox tMsgBuffer;
  tMsgBuffer.DstID = tDstID;
  tMsgBuffer.SrcID = tSrcID;
  tMsgBuffer.Cmd  = tCmd;
  tMsgBuffer.Buf.Byte[0] = tMsgData0;
  tMsgBuffer.Buf.Byte[1] = tMsgData1;
  tMsgBuffer.Buf.Byte[2] = tMsgData2;
  tMsgBuffer.Buf.Byte[3] = tMsgData3;
  bSendMessageBox( &tMsgBuffer );
}
*/


/// @brief �޽��� �ڽ� ���� Ȯ�� �Լ�
/// @param[in] ID �޽��� ���� ID
/// @param[out] tMsgBox ���ŵ� �޽����� ����Ǵ� ���� ������
/// @return True = �޽��� ���� \n False = �޽��� ����
U8 bReceiveMessageBox( U8 ID, typeMsgBox* tMsgBox )
{
  U8 i;
  U8 tMsgIndex;

  for ( tMsgIndex = 0 ; tMsgIndex < MSG_BOX_LENGTH ; tMsgIndex++ )
  {
    if ( gMsgBox[tMsgIndex].DstID != ID ) continue;

    tMsgBox->DstID = gMsgBox[tMsgIndex].DstID;
    tMsgBox->SrcID = gMsgBox[tMsgIndex].SrcID;
    tMsgBox->Cmd   = gMsgBox[tMsgIndex].Cmd;

    for( i = 0; i < (MSG_BUFFER_LENGTH / 2); i++)
    {
      tMsgBox->Buf.Word[i] = gMsgBox[tMsgIndex].Buf.Word[i];
      gMsgBox[tMsgIndex].Buf.Word[i] = 0;
    }
    gMsgBox[tMsgIndex].DstID = MSG_NULL;
    gMsgBox[tMsgIndex].SrcID = MSG_NULL;
    gMsgBox[tMsgIndex].Cmd = MSG_NULL;

    return true;
  }
  return false;
  
}


