/*****************************************************************************/
/*                                                                           */
/*  Module Name     : UUCCPR32.H                                             */
/*                                                                           */
/*  Description     : 32 Bit CPRB Structure, Return Codes and Routines       */
/*                                                                           */
/*  Copyright Notice: IBM Personal Communication/3270 Version 4.1            */
/*                    (C) COPYRIGHT IBM CORP. 1989,1996 - PROGRAM PROPERTY   */
/*                    OF IBM ALL RIGHTS RESERVED                             */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*  Function:                                                                */
/*                                                                           */
/*    Define the CPRB structure, return code constants and the external      */
/*    routines init_send_req_parms and send_request.                         */
/*                                                                           */
/*  Remarks:                                                                 */
/*                                                                           */
/*    * 32 bit entry point is init_send_req_parms and send_request           */
/*                                                                           */
/*****************************************************************************/

/*---------------------------------------------------------------------------*/
/*  CPR32 Structure                                                          */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus                     /* C++ compiler                       */
  extern "C" {
#endif

#ifndef  SRPI_C_INCLUDED
#define  SRPI_C_INCLUDED
#endif

typedef
struct
{
  unsigned char  *     uerserver;     /* ascii name of server                */
  unsigned short       uerfunct;      /* function id                         */
  unsigned char        uerrsvd1[2];   /* RESERVED                            */

  unsigned short       uerqparml;     /* request parameters length           */
  unsigned char        uerrsvd2[2];   /* RESERVED                            */
  unsigned char  *     uerqparmad;    /* request parameters address          */
  unsigned short       uerqdatal;     /* request data length                 */
  unsigned char        uerrsvd3[2];   /* RESERVED                            */
  unsigned char  *     uerqdataad;    /* request data address                */

  unsigned short       uerrparml;     /* reply parameters length             */
  unsigned char        uerrsvd4[2];   /* RESERVED                            */
  unsigned char  *     uerrparmad;    /* reply parameters address            */
  unsigned short       uerrdatal;     /* reply data length                   */
  unsigned char        uerrsvd5[2];   /* RESERVED                            */
  unsigned char  *     uerrdataad;    /* reply data address                  */

  unsigned long        uerretcode;    /* return code                         */
  unsigned long        uerservrc;     /* server return code                  */
  unsigned short       uerrepldplen;  /* replied paramters length            */
  unsigned short       uerreplddlen;  /* replied data data length            */

  unsigned long        uer3270ind;    /* 3270 screen update notify           */

  char                 uerrsvd6[148]; /* reserved field                      */

} UERCPRB;

/*---------------------------------------------------------------------------*/
/*  Parameters for 3270 screen update notify                                 */
/*---------------------------------------------------------------------------*/

#define UER3270NOTIFY 0x0000000000    /* notify user of 3270 screen update   */
#define UER3270DISABL 0XFFFFFFFFFF    /* do not notify user of screen update */

/*---------------------------------------------------------------------------*/
/*  Return Codes                                                             */
/*---------------------------------------------------------------------------*/

#define UERERROK        0x00000000    /* successful                          */

/* Type 1 Errors */

#define UERERRT1START   0x01000402    /* not started                         */
#define UERERRT1LOAD    0x01000404    /* not loaded                          */
#define UERERRT1BUSY    0x01000408    /* busy (not used)                     */
#define UERERRT1VER     0x0100040A    /* unsupported version id              */
#define UERERRT1EMU     0x0100040C    /* emulator not loaded                 */
#define UERERRT1ROUT    0x0100040E    /* undefined server name               */
#define UERERRT1COMMR   0x01000410    /* resource not available              */
#define UERERRT1REST    0x01000412    /* emulator restarted since last use   */
#define UERERRT1INUSE   0x01000414    /* session in use by file transfer     */
#define UERERRT1QPLEN   0x01000602    /* req. parm. length too large         */
#define UERERRT1RPLEN   0x01000604    /* reply parm. length too large        */
#define UERERRT1VERB    0x01000606    /* invalid verb type                   */
#define UERERRT1SERV    0x01000608    /* invalid server name                 */
#define UERERRT1QPAD    0x0100060C    /* invalid request parameters address  */
#define UERERRT1QDAD    0x0100060E    /* invalid request data address        */
#define UERERRT1RPAD    0x01000610    /* invalid reply parameters address    */
#define UERERRT1RDAD    0x01000612    /* invalid reply data address          */
#define UERERRT1TOPV    0x01000616    /* TOPVIEW not supported (not used)    */
#define UERERRT1INV3270 0x01000622    /* invalid 3270 screen update notify   */
#define UERERRT1INVCPRB 0x01000624    /* invalid CPRB segment                */
#define UERERRT1CNCL    0x01000802    /* cancelled by host                   */
#define UERERRT1CONV    0x01000C00    /* unable to maintain conversation     */
#define UERERRT1ISE     0x01000C02    /* internal software error             */
#define UERERRT1PROT    0x01000C04    /* protocol violation                  */
#define UERERRT1SYIN    0x01000C06    /* system inconsistency                */

/* Type 3 Errors - Process Errors */

#define UERERRT3NORES   0x03041D00    /* host resource not available         */
#define UERERRT3NOSER   0x03041E00    /* server unknown at host              */
#define UERERRT3UNSER   0x03041F00    /* server not available at host        */
#define UERERRT3TERMS   0x03042200    /* server ended normally, but no reply */
#define UERERRT3ABNDS   0x03042300    /* server ended abnormally, no reply   */

/*  For type 2 and type 3 errors the most significant byte of the return     */
/*  code is 0x02 and 0x03 respectively. The 3 remaining bytes are the        */
/*  exception class, exception code, and exception object sent or received   */
/*  in ackknowledge.                                                         */

#define UERERRT2        0x02          /* error type 2 - acknowledge sent     */
#define UERERRT3        0x03          /* error type 3 - acknowledge received */

/*---------------------------------------------------------------------------*/
/*  Interface Routines                                                       */
/*---------------------------------------------------------------------------*/

#define ord_init_send_req_parms  102
#define ord_send_request         103

void     WINAPI init_send_req_parms(UERCPRB * );
long int WINAPI send_request(UERCPRB * );

#ifdef __cplusplus
  }
#endif
