/*****************************************************************************/
/*                                                                           */
/*  Module Name     : WHLLAPI.H                                              */
/*                                                                           */
/*  Description     : WinHLLAPI Structure, Return Codes and Routines         */
/*                                                                           */
/*  Copyright Notice: IBM Personal Communication/3270 Version 4.3            */
/*                    (C) COPYRIGHT IBM CORP. 1989,1995 - PROGRAM PROPERTY   */
/*                    OF IBM ALL RIGHTS RESERVED                             */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*  Function:                                                                */
/*                                                                           */
/*    Define the WinHLLAPI structure, return code constants and              */
/*    the external routines for WinHLLAPI.                                   */
/*                                                                           */
/*****************************************************************************/
#ifndef WHLLAPI_HEAD
#define WHLLAPI_HEAD

#ifdef __cplusplus
  extern "C" {
#endif

/*************************************************************************/
/******************** WinHLLAPI STRUCTURE DEFINITIONS ********************/
/*************************************************************************/
#define  WHLLDESCRIPTION_LEN            127

typedef struct tagWHLLAPIDATA
{
    WORD    wVersion;
    char    szDescription[WHLLDESCRIPTION_LEN + 1];
} WHLLAPIDATA, * PWHLLAPIDATA, FAR * LPWHLLAPIDATA;

/*************************************************************************/
/******************** WinHLLAPI FUNCTION PROTOTYPES **********************/
/*************************************************************************/
extern void far pascal     WinHLLAPI(LPWORD , LPBYTE , LPWORD , LPWORD );
extern HANDLE far pascal   WinHLLAPIAsync(HWND, LPWORD, LPBYTE, LPWORD, LPWORD);
extern HANDLE far pascal   WinAsyncHLLAPI(HWND, LPWORD, LPBYTE, LPWORD, LPWORD);
extern BOOL far pascal     WinHLLAPICancelAsyncRequest(WORD, WORD);
extern int far pascal      WinHLLAPICancelBlockingCall(void);
extern int far pascal      WinHLLAPICleanup(void);
extern BOOL far pascal     WinHLLAPIIsBlocking(void);
extern FARPROC far pascal  WinHLLAPISetBlockingHook(FARPROC);
extern int far pascal      WinHLLAPIStartup(WORD, LPWHLLAPIDATA);
extern BOOL far pascal     WinHLLAPIUnhookBlockingHook(void);

#include "hllstr16.h"                    /* WinHLLAPI struct definitions */

/*************************************************************************/
/******************** WinHLLAPI FUNCTION NUMBERS *************************/
/*************************************************************************/
#define  CONNECTPS                        1 /* 000 Connect PS            */
#define  DISCONNECTPS                     2 /* 000 Disconnect PS         */
#define  SENDKEY                          3 /* 000 Sendkey function      */
#define  WAIT                             4 /* 000 Wait function         */
#define  COPYPS                           5 /* 000 Copy PS function      */
#define  SEARCHPS                         6 /* 000 Search PS function    */
#define  QUERYCURSORLOC                   7 /* 000 Query Cursor          */
#define  COPYPSTOSTR                      8 /* 000 Copy PS to String     */
#define  SETSESSIONPARAMETERS             9 /* 000 Set Session           */
#define  QUERYSESSIONS                   10 /* 000 Query Sessions        */
#define  RESERVE                         11 /* 000 Reserve function      */
#define  RELEASE                         12 /* 000 Release function      */
#define  COPYOIA                         13 /* 000 Copy OIA function     */
#define  QUERYFIELDATTRIBUTE             14 /* 000 Query Field           */
#define  COPYSTRTOPS                     15 /* 000 Copy string to PS     */
#define  STORAGEMGR                      17 /* 000 Storage Manager       */
#define  PAUSE                           18 /* 000 Pause function        */
#define  QUERYSYSTEM                     20 /* 000 Query System          */
#define  RESETSYSTEM                     21 /* 000 Reset System          */
#define  QUERYSESSIONSTATUS              22 /* 000 Query Session         */
#define  STARTHOSTNOTIFICATION           23 /* 000 Start Host            */
#define  QUERYHOSTUPDATE                 24 /* 000 Query Host Update     */
#define  STOPHOSTNOTIFICATION            25 /* 000 Stop Host             */
#define  SEARCHFIELD                     30 /* 000 Search Field          */
#define  FINDFIELDPOSITION               31 /* 000 Find Field            */
#define  FINDFIELDLENGTH                 32 /* 000 Find Field Length     */
#define  COPYSTRINGTOFIELD               33 /* 000 Copy String to        */
#define  COPYFIELDTOSTRING               34 /* 000 Copy Field to         */
#define  SETCURSOR                       40 /* 000 Set Cursor            */
#define  STARTCLOSEINTERCEPT             41 /* 000 Start Close Intercept */
#define  QUERYCLOSEINTERCEPT             42 /* 000 Query Close Intercept */
#define  STOPCLOSEINTERCEPT              43 /* 000 Stop Close Intercept  */
#define  STARTKSINTERCEPT                50 /* 000 Start Keystroke       */
#define  GETKEY                          51 /* 000 Get Key function      */
#define  POSTINTERCEPTSTATUS             52 /* 000 Post Intercept        */
#define  STOPKSINTERCEPT                 53 /* 000 Stop Keystroke        */
#define  LOCKPSAPI                       60 /* 000 Lock Presentation     */
#define  LOCKWINDOWSERVICESAPI           61 /* 000 Lock PM Window        */
#define  SENDFILE                        90 /* 000 Send File function    */
#define  RECEIVEFILE                     91 /* 000 Receive file          */
#define  CONVERT                         99 /* 000 Convert Position      */
#define  CONNECTWINDOWSERVICES          101 /* 000 Connect For           */
#define  DISCONNECTWINDOWSERVICES       102 /* 000 Disconnect From       */
#define  QUERYWINDOWCOORDINATES         103 /* 000 Query Presentation    */
#define  WINDOWSTATUS                   104 /* 000 PM Window Status      */
#define  CHANGEPSNAME                   105 /* 000 Change Switch List    */
#define  CHANGEWINDOWNAME               106 /* 000 Change PS Window      */
#define  STARTPLAYINGMACRO              110 /* 000 Start playing macro   */
#define  CONNECTSTRFLDS                 120 /* 000 Start Structured      */
#define  DISCONSTRFLDS                  121 /* 000 Stop Structured       */
#define  QUERYCOMMBUFSIZ                122 /* 000 Query Communications  */
#define  ALLOCCOMMBUFF                  123 /* 000 Allocate              */
#define  FREECOMMBUFF                   124 /* 000 Free Communications   */
#define  GETREQUESTCOMP                 125 /* 000 Get Asynchronous      */
#define  READSTRFLDS                    126 /* 000 Read Structured Field */
#define  WRITESTRFLDS                   127 /* 000 Write Structured      */

//=============================================================================
// SetSessionParameters definitions
//=============================================================================
#define  WHLL_SSP_NEWRET                0x00000001
#define  WHLL_SSP_OLDRET                0x00000002
#define  WHLL_SSP_ATTRB                 0x00000004
#define  WHLL_SSP_NOATTRB               0x00000008
#define  WHLL_SSP_NWAIT                 0x00000010
#define  WHLL_SSP_LWAIT                 0x00000020
#define  WHLL_SSP_TWAIT                 0x00000040
#define  WHLL_SSP_EAB                   0x00000080
#define  WHLL_SSP_NOEAB                 0x00000100
#define  WHLL_SSP_AUTORESET             0x00000200
#define  WHLL_SSP_NORESET               0x00000400
#define  WHLL_SSP_SRCHALL               0x00001000
#define  WHLL_SSP_SRCHFROM              0x00002000
#define  WHLL_SSP_SRCHFRWD              0x00004000
#define  WHLL_SSP_SRCHBKWD              0x00008000
#define  WHLL_SSP_FPAUSE                0x00010000
#define  WHLL_SSP_IPAUSE                0x00020000

//=============================================================================
// Convert Row or Column Flags
//=============================================================================
#define  WHLL_CONVERT_POSITION          'P'
#define  WHLL_CONVERT_ROW               'R'

//=============================================================================
// Storage Manager Sub-Function Values
//=============================================================================
#define  WHLL_GETSTORAGE                1
#define  WHLL_FREESTORAGE               2
#define  WHLL_FREEALLSTORAGE            3
#define  WHLL_QUERYFREESTORAGE          4

//=============================================================================
// Change PS Name Values
//=============================================================================
#define  WHLL_CHANGEPSNAME_SET          0X01
#define  WHLL_CHANGEPSNAME_RESET        0X02

//=============================================================================
// Window Status Values
//=============================================================================
#define  WHLL_WINDOWSTATUS_SET          0X01
#define  WHLL_WINDOWSTATUS_RESET        0X02

#define  WHLL_WINDOWSTATUS_NULL         0X0000
#define  WHLL_WINDOWSTATUS_SIZE         0X0001
#define  WHLL_WINDOWSTATUS_MOVE         0X0002
#define  WHLL_WINDOWSTATUS_ZORDER       0X0004
#define  WHLL_WINDOWSTATUS_SHOW         0X0008
#define  WHLL_WINDOWSTATUS_HIDE         0X0010
#define  WHLL_WINDOWSTATUS_ACTIVATE     0X0080
#define  WHLL_WINDOWSTATUS_DEACTIVATE   0X0100
#define  WHLL_WINDOWSTATUS_MINIMIZE     0X0400
#define  WHLL_WINDOWSTATUS_MAXIMIZE     0X0800
#define  WHLL_WINDOWSTATUS_RESTORE      0X1000

#define  WHLL_WINDOWSTATUS_FRONT        0X00000003
#define  WHLL_WINDOWSTATUS_BACK         0X00000004

/*********************************************************************/
/******************** WinHLLAPI RETURN CODES *************************/
/*********************************************************************/
#define  WHLLOK                         0
#define  WHLLNOTCONNECTED               1
#define  WHLLBLOCKNOTAVAIL              1
#define  WHLLPARAMETERERROR             2
#define  WHLLBLOCKIDINVALID             2
#define  WHLLFTXCOMPLETE                3
#define  WHLLFTXSEGMENTED               4
#define  WHLLPSBUSY                     4
#define  WHLLINHIBITED                  5
#define  WHLLTRUNCATED                  6
#define  WHLLPOSITIONERROR              7
#define  WHLLNOTAVAILABLE               8
#define  WHLLSYSERROR                   9
#define  WHLLWOULDBLOCK                 10
#define  WHLLUNAVAILABLE                11
#define  WHLLPSENDED                    12
#define  WHLLUNDEFINEDKEY               20
#define  WHLLOIAUPDATE                  21
#define  WHLLPSUPDATE                   22
#define  WHLLBOTHUPDATE                 23
#define  WHLLNOFIELD                    24
#define  WHLLNOKEYSTROKES               25
#define  WHLLPSCHANGED                  26
#define  WHLLFTXABORTED                 27
#define  WHLLZEROLENFIELD               28
#define  WHLLINVALIDTYPE                30
#define  WHLLKEYOVERFLOW                31
#define  WHLLSFACONN                    32
#define  WHLLTRANCANCLI                 34
#define  WHLLTRANCANCL                  35
#define  WHLLHOSTCLOST                  36
#define  WHLLOKDISABLED                 37
#define  WHLLNOTCOMPLETE                38
#define  WHLLSFDDM                      39
#define  WHLLSFDPEND                    40
#define  WHLLBUFFINUSE                  41
#define  WHLLNOMATCH                    42
#define  WHLLFTXCOMPLETECICS            101

#define  WHLLINVALIDFUNCTIONNUM         301
#define  WHLLFILENOTFOUND               301
#define  WHLLACCESSDENIED               305
#define  WHLLMEMORY                     308
#define  WHLLINVALIDENVIRONMENT         310
#define  WHLLINVALIDFORMAT              311

#define  WHLLINVALIDPSID                9998
#define  WHLLINVALIDRC                  9999

//=============================================================================
// Windows HLLAPI Extension Return Codes
//=============================================================================
#define  WHLLALREADY                    0xF000
#define  WHLLINVALID                    0xF001
#define  WHLLCANCEL                     0xF002
#define  WHLLSYSNOTREADY                0xF003
#define  WHLLVERNOTSUPPORTED            0xF004

#ifdef __cplusplus
  }
#endif

#endif // #ifdef WHLLAPI_HEAD
