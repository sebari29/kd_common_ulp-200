
/// @file   dev_uart.c
/// @date   2012/02/14
/// @author jinhwa Lee
/// @brief  Uart 관련 설정 디바이스 드라이브 파일


#include "dev_uart.h"


/// @brief      Uart0 설정 함수.
///             (19200bps Console Uart)
/// @param      void
/// @return     void
void __InitializeUart0_19200(void)
{
  u0mr = 0x00;

  u0c0 = 0x00;
  u0c1 = 0x00;
  
  u0tb = 0x0000;    // tx buffer clear
  u0rb = 0x0000;    // rx buffer clear

  txd0sel0 = 1;    //TxD0 : pd1_4
  rxd0sel0 = 1;    //RxD0 : pd1_5

  s0tic = 0x00;    // interrupt level 0, interrupt request clear, tx interrupt disable
  s0ric = 0x00;    //rx interrupt Diable    

  /* 4MHz/(16( u0brg + 1 )) = 19230.769bps */  
  u0brg = 12;
  
  u0mr = 0x05;

  u0c0 = 0x08;    //txept_u0c0 = 1, transmission completed  
  u0c1 = 0x15;      //rx enable, tx enable, Transmission completed interrupt
  
  s0ric = 0x04;    // interrupt level 3, interrupt request clear, rx interrupt enable
  s0tic = 0x04;    // interrupt level 3, interrupt request clear, tx interrupt enable
  
}

// @brief      Uart1 설정 함수.
///             (9600bps 경비실기 간 통신)
/// @param      void
/// @return     void
void __InitializeUart1_9600(void)
{

  u1mr = 0x00;
  u1c0 = 0x00;
  u1c1 = 0x00;
  
  u1tb = 0x0000;    // tx buffer clear
  u1rb = 0x0000;    // rx buffer clear

  /* TXD1 pin select bit */
  txd1sel0 = 0;    //TxD1 : pd6_3
  txd1sel1 = 1;
  /* RXD1 pin select bit */
  rxd1sel0 = 0;    //RxD1 : pd6_4
  rxd1sel1 = 1;    

  /* UART0 transmit interrupt control register */
  s1tic = 0x00;    //tx interrupt Diable
  /* UART0 receive interrupt control register */
  s1ric = 0x00;    // interrupt level 0, interrupt request clear, rx interrupt disable  

  //4MHz/(16( u2brg + 1 )) = 3906.25bps   
  u1brg = (U8)(26 - 1);
  /* Set 9-bit transfer data length
     set internal clock source
     set 1 stop bit
     set no parity */
  u1mr = 0x05;

  /* Set F1 - source, TXD0 - CMOS output, LSB - first */
  u1c0 = 0x08;    //txept_u1c0 = 1, transmission completed  
  /* Set reception/transmission - enable */
  u1c1 = 0x15;    //rx enable, tx enable, Transmission completed interrupt
  
  /* UART1 receive interrupt control register */
  s1ric = 0x06;    // interrupt level 4, interrupt request clear, rx interrupt enable
  /* UART1 transmit interrupt control register */
  //s1tic = 0x07;    // interrupt level 4, interrupt request clear, tx interrupt enable
}


/// @brief      Uart1 설정 함수.
///             (3900bps 경비실기 간 통신)
/// @param      void
/// @return     void
void __InitializeUart1_3906(void)
{

  u1mr = 0x00;
  u1c0 = 0x00;
  u1c1 = 0x00;
  
  u1tb = 0x0000;    // tx buffer clear
  u1rb = 0x0000;    // rx buffer clear

  /* TXD1 pin select bit */
  txd1sel0 = 0;    //TxD1 : pd6_3
  txd1sel1 = 1;
  /* RXD1 pin select bit */
  rxd1sel0 = 0;    //RxD1 : pd6_4
  rxd1sel1 = 1;    

  /* UART0 transmit interrupt control register */
  s1tic = 0x00;    //tx interrupt Diable
  /* UART0 receive interrupt control register */
  s1ric = 0x00;    // interrupt level 0, interrupt request clear, rx interrupt disable  

  //4MHz/(16( u2brg + 1 )) = 3906.25bps   
  u1brg = (U8)(64 - 1);
  /* Set 9-bit transfer data length
     set internal clock source
     set 1 stop bit
     set no parity */
  u1mr = 0x06;

  /* Set F1 - source, TXD0 - CMOS output, LSB - first */
  u1c0 = 0x08;    //txept_u1c0 = 1, transmission completed  
  /* Set reception/transmission - enable */
  u1c1 = 0x15;    //rx enable, tx enable, Transmission completed interrupt
  
  /* UART1 receive interrupt control register */
  s1ric = 0x06;    // interrupt level 4, interrupt request clear, rx interrupt enable
  /* UART1 transmit interrupt control register */
  //s1tic = 0x07;    // interrupt level 4, interrupt request clear, tx interrupt enable
}


/// @brief      Uart1 설정 함수.
///             (3900bps 경비실기 간 통신)
/// @param      void
/// @return     void
void __InitializeUart1_Disable(void)
{

  u1mr = 0x00;
  u1c0 = 0x00;
  u1c1 = 0x00;
  
  u1tb = 0x0000;    // tx buffer clear
  u1rb = 0x0000;    // rx buffer clear

  /* TXD1 pin select bit */
  txd1sel0 = 0;    //TxD1 : pd6_3
  txd1sel1 = 0;
  /* RXD1 pin select bit */
  rxd1sel0 = 0;    //RxD1 : pd6_4
  rxd1sel1 = 0;    

  /* UART0 transmit interrupt control register */
  s1tic = 0x00;    //tx interrupt Diable
  /* UART0 receive interrupt control register */
  s1ric = 0x00;    // interrupt level 0, interrupt request clear, rx interrupt disable  

  //4MHz/(16( u2brg + 1 )) = 3906.25bps   
  u1brg = 0;
  /* Set 9-bit transfer data length
     set internal clock source
     set 1 stop bit
     set no parity */
  u1mr = 0;

  /* Set F1 - source, TXD0 - CMOS output, LSB - first */
  u1c0 = 0;    //txept_u1c0 = 1, transmission completed  
  /* Set reception/transmission - enable */
  u1c1 = 0;    //rx enable, tx enable, Transmission completed interrupt
  
  /* UART1 receive interrupt control register */
  s1ric = 0;    // interrupt level 4, interrupt request clear, rx interrupt enable
  /* UART1 transmit interrupt control register */
  //s1tic = 0x07;    // interrupt level 4, interrupt request clear, tx interrupt enable
}


/// @brief      Uart2 설정 함수.
///             (9600bps 세대기 / 로비 간 통신)
/// @param      void
/// @return     void
void __InitializeUart2_9600(void)  // 세대기 통신 3906.25bps
{
  u2mr = 0x00;
  u2c0 = 0x00;      //txept_u1c0 = 1, transmission completed  
  u2c1 = 0x00;      //rx enable, tx enable, Transmission completed interrupt

  
  u2tb = 0x0000;    // tx buffer clear
  u2rb = 0x0000;    // rx buffer clear

  txd2sel0 = 1;      //TxD2 : pd6_6
  txd2sel1 = 0;
  txd2sel2 = 1;

  rxd2sel0 = 1;      //RxD2 : pd4_5
  rxd2sel1 = 1;    

  s2tic = 0x00;      //tx interrupt Diable
  s2ric = 0x00;      // interrupt level 0, interrupt request clear, rx interrupt disable  

   //4MHz/(16( u2brg + 1 )) = 3906.25bps   
  u2brg = (U8)(26 - 1);

   //4MHz/(16( u0brg + 1 )) = 19230.769bps   
  //u2brg = 12;
  
  u2mr = 0x05;

  u2c0 = 0x08;      //txept_u1c0 = 1, transmission completed  
  u2c1 = 0x15;      //rx enable, tx enable, Transmission completed interrupt
  
  s2ric = 0x06;      // interrupt level 7, interrupt request clear, rx interrupt enable
  //s2tic = 0x07;      // interrupt level 7, interrupt request clear, tx interrupt enable

  mp = mpie = 0;    // Multiprocessor Communication enable bit
}


/// @brief      Uart2 설정 함수.
///             (3900bps 세대기 / 로비 간 통신)
/// @param      void
/// @return     void
void __InitializeUart2_3906(void)  // 세대기 통신 3906.25bps
{
  u2mr = 0x00;
  u2c0 = 0x00;      //txept_u1c0 = 1, transmission completed  
  u2c1 = 0x00;      //rx enable, tx enable, Transmission completed interrupt

  
  u2tb = 0x0000;    // tx buffer clear
  u2rb = 0x0000;    // rx buffer clear

  txd2sel0 = 1;      //TxD2 : pd6_6
  txd2sel1 = 0;
  txd2sel2 = 1;

  rxd2sel0 = 1;      //RxD2 : pd4_5
  rxd2sel1 = 1;    

  s2tic = 0x00;      //tx interrupt Diable
  s2ric = 0x00;      // interrupt level 0, interrupt request clear, rx interrupt disable  

   //4MHz/(16( u2brg + 1 )) = 3906.25bps   
  u2brg = (U8)(64 - 1);

   //4MHz/(16( u0brg + 1 )) = 19230.769bps   
  //u2brg = 12;
  
  u2mr = 0x06;

  u2c0 = 0x08;      //txept_u1c0 = 1, transmission completed  
  u2c1 = 0x15;//0x04      //rx enable, tx enable, Transmission completed interrupt
  
  s2ric = 0x06;      // interrupt level 7, interrupt request clear, rx interrupt enable
  //s2tic = 0x07;      // interrupt level 7, interrupt request clear, tx interrupt enable

  mp = mpie = SET;    // Multiprocessor Communication enable bit
}


/// @brief      Uart2 설정 함수.
///             (3900bps 세대기 / 로비 간 통신)
/// @param      void
/// @return     void
void __InitializeUart2_Disable(void)  // 세대기 통신 3906.25bps
{
  u2mr = 0x00;
  u2c0 = 0x00;      //txept_u1c0 = 1, transmission completed  
  u2c1 = 0x00;      //rx enable, tx enable, Transmission completed interrupt

  
  u2tb = 0x0000;    // tx buffer clear
  u2rb = 0x0000;    // rx buffer clear

  txd2sel0 = 0;      //TxD2 : pd6_6
  txd2sel1 = 0;
  txd2sel2 = 0;

  rxd2sel0 = 0;      //RxD2 : pd4_5
  rxd2sel1 = 0;    

  s2tic = 0x00;      //tx interrupt Diable
  s2ric = 0x00;      // interrupt level 0, interrupt request clear, rx interrupt disable  

  u2brg = 0;

  u2mr = 0;

  u2c0 = 0;      //txept_u1c0 = 1, transmission completed  
  u2c1 = 0;      //rx enable, tx enable, Transmission completed interrupt
  
  s2ric = 0;      // interrupt level 7, interrupt request clear, rx interrupt enable

  mp = mpie = 0;    // Multiprocessor Communication enable bit
}


