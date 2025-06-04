/*****************************************************************************/
/*                                                                           */
/*  Module Name     : EHLAPI32.H                                             */
/*    Define the 32 Bit HLLAPI structure, return code constants and          */
/*    the external routines HLLAPI.                                          */
/*                                                                           */
/*****************************************************************************/
#ifdef __cplusplus
  extern "C" {
#endif

#ifndef  HLLAPI_C_INCLUDED
#define  HLLAPI_C_INCLUDED
#endif

#define ord_hllapi  3


long WINAPI hllapi(LPINT lpiFun, LPSTR lpStr, LPINT lpiLength , LPINT lpiReturnCode );

extern "C" __declspec(dllexport)
WORD WinHLLAPI(int iFun, LPSTR lpStr, LPINT lpiLength , LPINT lpiReturnCode );

/*********************************************************************/
/**************** EHLLAPI FUNCTION NUMBERS ***************************/
/*********************************************************************/

#define CONNECT_PS            1      /* 000 Connect PS            */
                                        /* function number.          */

#define DISCONNECT_PS         2      /* 000 Disconnect PS         */
                                        /* function number.          */

#define SENDKEY               3      /* 000 Sendkey function      */
                                        /* number.                   */

#define WAIT                  4      /* 000 Wait function         */
                                        /* number.                   */

#define COPY_PS               5      /* 000 Copy PS function      */
                                        /* number.                   */

#define SEARCH_PS             6      /* 000 Search PS function    */
                                        /* number.                   */

#define QUERY_CURSOR_LOC      7      /* 000 Query Cursor          */
                                        /* Location function         */
                                        /* number.                   */

#define COPY_PS_TO_STR        8      /* 000 Copy PS to String     */
                                        /* function number.          */

#define SET_SESSION_PARMS     9      /* 000 Set Session           */
                                        /* Parameters function       */
                                        /* number.                   */

#define QUERY_SESSIONS        10     /* 000 Query Sessions        */
                                        /* function number.          */

#define RESERVE               11     /* 000 Reserve function      */
                                        /* number.                   */

#define RELEASE               12     /* 000 Release function      */
                                        /* number.                   */


#define COPY_OIA              13     /* 000 Copy OIA function     */
                                        /* number.                   */

#define QUERY_FIELD_ATTR      14     /* 000 Query Field           */
                                        /* Attribute function        */
                                        /* number.                   */

#define COPY_STR_TO_PS        15     /* 000 Copy string to PS     */
                                        /* function number.          */

#define STORAGE_MGR           17     /* 000 Storage Manager       */
                                        /* function number.          */

#define PAUSE                 18     /* 000 Pause function        */
                                        /* number.                   */

#define QUERY_SYSTEM          20     /* 000 Query System          */
                                        /* function number.          */

#define RESET_SYSTEM          21     /* 000 Reset System          */
                                        /* function number.          */

#define QUERY_SESSION_STATUS  22     /* 000 Query Session         */
                                        /* Status function           */
                                        /* number.                   */

#define START_HOST_NOTIFY     23     /* 000 Start Host            */
                                        /* Notification function     */
                                        /* number.                   */

#define QUERY_HOST_UPDATE     24     /* 000 Query Host Update     */
                                        /* function number.          */

#define STOP_HOST_NOTIFY      25     /* 000 Stop Host             */
                                        /* Notification function     */
                                        /* number.                   */

#define SEARCH_FIELD          30     /* 000 Search Field          */
                                        /* function number.          */

#define FIND_FIELD_POS        31     /* 000 Find Field            */
                                        /* Position function         */
                                        /* number.                   */

#define FIND_FIELD_LEN        32     /* 000 Find Field Length     */
                                        /* function number.          */

#define COPY_STR_TO_FIELD     33     /* 000 Copy String to        */
                                        /* Field function number.    */

#define COPY_FIELD_TO_STR     34     /* 000 Copy Field to         */
                                        /* String function           */
                                        /* number.                   */

#define SET_CURSOR            40     /* 000 Set Cursor            */
                                        /* function number.          */

#define START_CLOSE_INTERCEPT 41     /* 000 Start Close Intercept */
                                        /* function number.          */

#define QUERY_CLOSE_INTERCEPT 42     /* 000 Query Close Intercept */
                                        /* function number.          */

#define STOP_CLOSE_INTERCEPT  43     /* 000 Stop Close Intercept  */
                                        /* function number.          */

#define START_KEY_INTERCEPT   50     /* 000 Start Keystroke       */
                                        /* Intercept function        */
                                        /* number.                   */

#define GET_KEY               51     /* 000 Get Key function      */
                                        /* number.                   */

#define POST_INTERCEPT_STATUS 52     /* 000 Post Intercept        */
                                        /* Status function           */
                                        /* number.                   */

#define STOP_KEY_INTERCEPT    53     /* 000 Stop Keystroke        */
                                        /* Intercept function        */
                                        /* number.                   */

#define LOCK_PS               60     /* 000 Lock Presentation     */
                                        /* Space function            */
                                        /* number.                   */

#define LOCK_PMSVC            61     /* 000 Lock PM Window        */
                                        /* Services function         */
                                        /* number.                   */

#define SEND_FILE             90     /* 000 Send File function    */
                                        /* number.                   */

#define RECEIVE_FILE          91     /* 000 Receive file          */
                                        /* function number.          */
#define SEND_FILE_NOWAIT             96     /* 000 Send File function    */
                                        /* number.                   */

#define RECEIVE_FILE_NOWAIT          97     /* 000 Receive file          */
                                        /* function number.          */

#define RESTART_SESSION         60      // 99.04.29 Eddy Add
// 98.08.26 Eddy Add
#define EHL_XF_PARM             92      // File Transfer Configuration Paremeter
#define EHL_XF_LENGTH           93      // File Transfer Bytes Counter
#define EHL_XF_STATUS           94      // File Transfer Bytes Counter
#define EHL_XF_CANCEL           95      // Cancel File Transfer
//------------------------------------------------------------------------------------


#define CONVERT_POS_ROW_COL   99     /* 000 Convert Position      */
                                        /* or Row Column function    */
                                        /* number.                   */

#define CONNECT_PM_SRVCS     101     /* 000 Connect For           */
                                        /* Presentation Manager      */
                                        /* Window Services function  */
                                        /* number                    */

#define DISCONNECT_PM_SRVCS  102     /* 000 Disconnect From       */
                                        /* Presentation Manager      */
                                        /* Window Services function  */
                                        /* number                    */

#define QUERY_WINDOW_COORDS  103     /* 000 Query Presentation    */
                                        /* Manager Window            */
                                        /* Coordinates function      */
                                        /* number                    */

#define PM_WINDOW_STATUS     104     /* 000 PM Window Status      */
                                        /* function number           */

#define CHANGE_SWITCH_NAME   105     /* 000 Change Switch List    */
                                        /* Logical Terminal name     */
                                        /* function number           */

#define CHANGE_WINDOW_NAME   106     /* 000 Change PS Window      */
                                        /* Name function number      */

#define START_PLAYING_MACRO  110     /* 000 Start playing macro   */
                                        /* function                  */

#define START_STRUCTURED_FLD 120     /* 000 Start Structured      */
                                        /* Field function number     */

#define STOP_STRUCTURED_FLD  121     /* 000 Stop Structured       */
                                        /* Field function number     */

#define QUERY_BUFFER_SIZE    122     /* 000 Query Communications  */
                                        /* Buffer Size function      */
                                        /* number                    */

#define ALLOCATE_COMMO_BUFF  123     /* 000 Allocate              */
                                        /* Communications Buffer     */
                                        /* function number           */

#define FREE_COMMO_BUFF      124     /* 000 Free Communications   */
                                        /* Buffer function number    */

#define GET_ASYNC_COMPLETION 125     /* 000 Get Asynchronous      */
                                        /* Completion Request        */
                                        /* function                  */

#define READ_STRUCTURED_FLD  126     /* 000 Read Structured Field */
                                        /* function number           */

#define WRITE_STRUCTURED_FLD 127     /* 000 Write Structured      */
                                        /* Field function number     */

#define EHL_XF_PARM             92      // File Transfer Configuration Paremeter
#define EHL_XF_LENGTH           93      // File Transfer Bytes Counter
#define EHL_XF_STATUS           94      // File Transfer Bytes Counter
#define EHL_XF_CANCEL           95      // Cancel File Transfer

//98.10.02 Eddy Add
#define EHL_SHOW_SESSION           225 // 98.10.02 Eddy Add for follow W16 Special
#define EHL_HIDE_SESSION           226 // 98.10.02 Eddy Add for follow W16 Special
//----------------------------------------------------------------------

/*********************************************************************/
/******************** EHLLAPI RETURN CODES ***************************/
/*********************************************************************/


#define SUCCESS               0    /* 000 Good return code.     */

#define HARC99_INVALID_INP         0    /* 000 Incorrect input       */
                                        /* for Convert Position      */
                                        /* or RowCol(99).            */

#define INVALID_PS            1    /* 000 Invalid PS, Not       */
                                        /* connected return code.    */

#define BAD_PARM              2    /* 000 Bad parameter, or     */
                                        /* verb not supported        */
                                        /* return code.              */

#define BUSY                  4    /* 000 PS is busy return     */
                                        /* code.                     */

#define LOCKED                5    /* 000 PS is LOCKed, or      */
                                        /* invalid keystroke         */
                                        /* return code.              */

#define TRUNCATION            6    /* 000 Truncation            */
                                        /* occured, or invalid       */
                                        /* length return code.       */

#define INVALID_PS_POS        7    /* 000 Invalid PS            */
                                        /* position return code.     */

#define NO_PRIOR_START        8    /* 000 No prior start        */
                                        /* keystroke int. or host    */
                                        /* notify return code.       */

#define SYSTEM_ERROR          9    /* 000 A system error        */
                                        /* occured return code.      */

#define UNSUPPORTED           10   /* 000 Invalid or            */
                                        /* unsupported function      */
                                        /* number return code.       */

#define UNAVAILABLE           11   /* 000 Resource is           */
                                        /* unavalible at this        */
                                        /* time return code.         */

#define SESSION_STOPPED       12   /* 000 Session has           */
                                        /* been stopped              */

#define BAD_MNEMONIC          20   /* 000 Illegal mnemonic      */
                                        /* return code.              */

#define OIA_UPDATE            21   /* 000 A OIA update          */
                                        /* occurred return code.     */

#define PS_UPDATE             22   /* 000 A PS update           */
                                        /* occurred return code.     */

#define PS_AND_OIA_UPDATE     23   /* A PS and OIA update       */
                                        /* occurred return code.     */

#define STR_NOT_FOUND_UNFM_PS 24   /* 000 String not found,     */
                                        /* or Unformated PS          */
                                        /* return code.              */

#define NO_KEYS_AVAIL         25   /* 000 No keys available     */
                                        /* return code.              */

#define HOST_UPDATE           26   /* 000 A HOST update         */
                                        /* occurred return code.     */

#define FIELD_LEN_ZERO        28   /* 000 Field length = 0      */
                                        /* return code.              */

#define QUEUE_OVERFLOW        31   /* 000 Keystroke queue       */
                                        /* overflow return code.     */

#define ANOTHER_CONNECTION    32   /* 000 Successful. Another   */
                                        /* Structured Field          */
                                        /* connection to this        */
                                        /* session concurrently      */
                                        /* exists.                   */

#define INBOUND_CANCELLED     34   /* 000 Inbound structured    */
                                        /* field cancelled.          */

#define OUTBOUND_CANCELLED    35   /* 000 Outbound structured   */
                                        /* field cancelled.          */

#define CONTACT_LOST          36   /* 000 Contact with the      */
                                        /* Host was lost.            */

#define INBOUND_DISABLED      37   /* 000 Host structured field */
                                        /* state is inbound disabled.*/

#define FUNCTION_INCOMPLETE   38   /* 000 Requested Asynchronous*/
                                        /* function has not completed*/

#define DDM_ALREADY_EXISTS    39   /* 000 Request for DDM       */
                                        /* structured field connect  */
                                        /* failed because another    */
                                        /* DDM connection to this    */
                                        /* session already exists.   */

#define ASYNC_REQUESTS_OUT    40   /* 000 Disconnect successful.*/
                                        /* Pending asynchronous      */
                                        /* requests pending.         */

#define MEMORY_IN_USE         41   /* 000 Memory cannot be freed*/
                                        /* because it is in use.     */

#define NO_MATCH              42   /* 000 No pending            */
                                        /* asynchronous request meet */
                                        /* supplied criterion.       */

#define OPTION_INVALID        43   /* 000 Option requested is   */
                                        /* invalid at this time.     */

#define CALL_REENTRY          99

#define HARC99_INVALID_PS        9998   /* 000 An invalid PS id      */
                                        /* was specified, or PS      */
                                        /* was never connected, or   */
                                        /* System Error occured      */
                                        /* for Convert Position      */
                                        /* or RowCol(99).            */

#define HARC99_INVALID_CONV_OPT  9999   /* 000 Invalid convert       */
                                        /* option was specified.     */
                                        /* for Convert Position      */
                                        /* or RowCol(99).            */

/*********************************************************************/
/******************* EHLLAPI FUNCTION STRUCTURES *********************/
/*********************************************************************/

typedef union   _wait_object            /*                           */
{
  HANDLE      * hev_semaphore;          /* 000 Event semaphore       */
  HWND          window_handle;          /* 000 Window handle         */
  WORD          wTaskID;                /* 000 TaskID returned       */
} WAIT_OBJECT
;

#define  sthn_asem   sthn_wait.hev_semaphore
#define  stci_asem   stci_wait.hev_semaphore
#define  stsf_asem   stsf_wait.hev_semaphore
#define  rdsf_asem   rdsf_wait.hev_semaphore
#define  wrsf_asem   wrsf_wait.hev_semaphore

#define  stps_struct HLDConnectPS
#define  qses_struct HLDQuerySessions
#define  coia_struct HLDCopyOIA
#define  stor_struct HLDStorageMgr
#define  paus_struct HLDPause
#define  qsys_struct HLDQuerySystem
#define  qsst_struct HLDQuerySessionStatus
#define  sthn_struct HLDStartHostNotify
#define  qyhn_struct HLDQueryHostUpdate
#define  sphn_struct HLDStopHostNotify
#define  stci_struct HLDStartCloseIntercept
#define  qyci_struct HLDQueryCloseIntercept
#define  spci_struct HLDStopCloseIntercept
#define  stki_struct HLDStartKeyIntercept
#define  gkey_struct HLDGetKey
#define  pist_struct HLDPostInterceptStatus
#define  spki_struct HLDStopKeyIntercept
#define  lkps_struct HLDLockPS
#define  lkpm_struct HLDLockPMSVC
#define  cvrc_struct HLDConvertPosRowCol
#define  stpm_struct HLDConnectPM
#define  sppm_struct HLDDisconnectPM
#define  gcor_struct HLDQueryWindowCoords
#define  cwin_struct HLDPMWindowStatus
#define  xwin_struct HLDPMWindowStatusExt
#define  chsw_struct HLDChangeSwitchName
#define  chlt_struct HLDChangeWindowName
#define  stsf_struct HLDConnectSF
#define  spsf_struct HLDDisconnectSF
#define  qbuf_struct HLDQueryBufferSize
#define  abuf_struct HLDAllocateCommBuff
#define  fbuf_struct HLDFreeCommBuff
#define  gcmp_struct HLDGetAsyncCompletion
#define  rdsf_struct HLDReadSF
#define  wrsf_struct HLDWriteSF

/*********************************************************************/
/*                     Connect for Presentation Services (1)         */
/*********************************************************************/

struct HLDConnectPS                     /* 000 Connect For           */
                                        /* Presentation              */
                                        /* Services structure        */
{
  unsigned char stps_shortname;         /* 000 Session Shortname     */
  unsigned char stps_extendedname[3];   /* RESERVED                  */
}
;                                       /* 000                       */

/*********************************************************************/
/*                     Query Sessions (10)                           */
/*********************************************************************/

struct HLDQuerySessions                 /* 000 Query Sessions        */
                                        /* structure                 */
{
  unsigned char qses_shortname;         /* 000 Session Shortname     */
  unsigned char qses_extendedname[3];   /* RESERVED                  */
  unsigned char qses_longname[8];       /* 000 Session Longname      */
  unsigned char qses_sestype;           /* 000 Session Type          */
  unsigned char qses_reserved1;         /* 000 RESERVED              */
  unsigned short int qses_pssize;       /* 000 Presentation Space    */
                                        /* Size.                     */
}
;                                       /* 000                       */

/*********************************************************************/
/*                         Copy OIA (13)                             */
/*********************************************************************/

struct HLDCopyOIA                       /* 000 Copy OIA structure    */
{
  unsigned char coia_format;            /* 000 The OIA Format        */
                                        /* Byte for the 3270 PC.     */
  unsigned char coia_image[80];         /* 000 The OIA image         */
  unsigned char coia_group[22];         /* 000 The OIA group         */
  unsigned char coia_reserved;          /* RESERVED                  */
}
;                                       /* 000                       */

/*********************************************************************/
/*                     Storage Manager (17)                          */
/*********************************************************************/

struct HLDStorageMgr                    /* 000 Storage Manager       */
                                        /* structure.                */
{
  unsigned char *     stor_ptr;         /* 000 4 byte pointer to     */
                                        /* memory.                   */
}
;                                       /* 000                       */

/*********************************************************************/
/*                     Pause (18)                                    */
/*********************************************************************/

struct HLDPause                         /* 000 Pause                 */
                                        /* structure.                */
{
  unsigned char paus_shortname;         /* 000 Session Shortname     */
  unsigned char paus_extendedname[3];   /* RESERVED                  */
  unsigned char paus_singleses;         /* 000 Single session        */
                                        /* indicater                 */
  unsigned char paus_reserved[3];       /* RESERVED                  */
}
;                                       /* 000                       */

/*********************************************************************/
/*                     Query System (20)                             */
/*********************************************************************/

struct HLDQuerySystem                   /* 000 Query System          */
                                        /* structure                 */
{
  unsigned char qsys_hllapi_ver;        /* 000 Ehllapi version       */
  unsigned char qsys_hllapi_lvl[2];     /* 000 Ehllapi level         */
  unsigned char qsys_hllapi_date[6];    /* 000 Ehllapi release       */
                                        /* date                      */
  unsigned char qsys_lim_ver;           /* 000 LIM version           */
  unsigned char qsys_lim_lvl[2];        /* 000 LIM level             */
  unsigned char qsys_hardware_base;     /* 000 Hardware base         */
  unsigned char qsys_ctrl_prog_type;    /* 000 Control program       */
                                        /* type                      */
  unsigned char qsys_seq_num[2];        /* 000 Sequence number       */
  unsigned char qsys_ctrl_prog_ver[2];  /* 000 Control program       */
                                        /* version                   */
  unsigned char qsys_pc_sname;          /* 000 Base pc session       */
                                        /* name                      */
  unsigned char qsys_err1[4];           /* 000 System error words    */
                                        /* and 2                     */
  unsigned char qsys_err2[4];           /* 000 System error words    */
                                        /* and 4                     */
  unsigned char qsys_sys_model;         /* 000 System model          */
  unsigned char qsys_sys_submodel;      /* 000 System submodel       */
  unsigned char qsys_reserved1;         /* 000 RESERVED              */
  unsigned short qsys_pc_nls;           /* 000 Pc code page          */
  unsigned char qsys_monitor_type;      /* 000 Monitor type          */
  unsigned char qsys_reserved2[3];      /* 000 RESERVED              */
}
;                                       /* 000                       */

/*********************************************************************/
/*                     Query Session Status (22)                     */
/*********************************************************************/

struct HLDQuerySessionStatus            /* 000 Query Session         */
                                        /* Status structure          */
{
  unsigned char qsst_shortname;         /* 000 Session Shortname     */
  unsigned char qsst_extendedname[3];   /* RESERVED                  */
  unsigned char qsst_longname[8];       /* 000 Session longname      */
  unsigned char qsst_sestype;           /* 000 Session type          */
  unsigned char qsst_char;              /* 000 Session               */
                                        /* characteristics           */
  unsigned short int qsst_ps_rows;      /* 000 Number of rows in     */
                                        /* presentation space        */
  unsigned short int qsst_ps_cols;      /* 000 Number of columns     */
                                        /* in presentation space     */
  unsigned short int qsst_host_nls;     /* 000 Host code page        */
}
;                                       /* 000                       */

/*********************************************************************/
/*                     Start Host Notification (23)                  */
/*********************************************************************/

struct HLDStartHostNotify               /* 000 Start Host            */
                                        /* Notification              */
                                        /* structure.                */
{
  unsigned char sthn_shortname;         /* 000 Session Shortname     */
  unsigned char sthn_extendedname[3];   /* RESERVED                  */
  unsigned char sthn_event_opt;         /* 000 Update event          */
                                        /* option                    */
  unsigned char sthn_reserved1[3];      /* RESERVED                  */
  WAIT_OBJECT        sthn_wait;         /* wait_object union         */
                                        /*                           */
  unsigned char sthn_aevent_opt;        /* 000 Async update event    */
                                        /* option                    */
  unsigned char sthn_reserved2[3];      /* RESERVED                  */
}
;                                       /* 000                       */

/*********************************************************************/
/*                     Query Host Update (24)                        */
/*********************************************************************/

struct HLDQueryHostUpdate               /* 000 Query Host Update     */
                                        /* structure                 */
{
  unsigned char qyhn_shortname;         /* 000 Session Shortname     */
  unsigned char qyhn_extendedname[3];   /* RESERVED                  */
}
;                                       /* 000                       */

/*********************************************************************/
/*                     Stop Host Notification (25)                   */
/*********************************************************************/

struct HLDStopHostNotify                /* 000 Stop Host Notification*/
                                        /* structure                 */
{
  unsigned char sphn_shortname;         /* 000 Session Shortname     */
  unsigned char sphn_extendedname[3];   /* RESERVED                  */
}
;                                       /* 000                       */

/*********************************************************************/
/*                     Start Close Click Intercept (41)              */
/*********************************************************************/

struct HLDStartCloseIntercept           /* 000 Start Close Click     */
                                        /* Intercept structure       */
{
  unsigned char stci_shortname;         /* 000 Session Shortname     */
  unsigned char stci_extendedname[3];   /* RESERVED                  */
  unsigned char stci_close_opt;         /* 000 Type of notification  */
  unsigned char stci_reserved[3];       /* RESERVED                  */
  WAIT_OBJECT        stci_wait;         /* wait_object union         */
                                        /*                           */
}
;                                       /* 000                       */

/*********************************************************************/
/*                     Query Close Click Intercept (42)              */
/*********************************************************************/

struct HLDQueryCloseIntercept           /* 000 Query Close Click     */
                                        /* Intercept structure       */
{
  unsigned char qyci_shortname;         /* 000 Session Shortname     */
  unsigned char qyci_extendedname[3];   /* RESERVED                  */
}
;                                       /* 000                       */

/*********************************************************************/
/*                     Stop  Close Click Intercept (43)              */
/*********************************************************************/

struct HLDStopCloseIntercept            /* 000 Stop Close Click      */
                                        /* Intercept structure       */
{
  unsigned char spci_shortname;         /* 000 Session Shortname     */
  unsigned char spci_extendedname[3];   /* RESERVED                  */
}
;                                       /* 000                       */

/*********************************************************************/
/*                     Start Keystroke Intercept (50)                */
/*********************************************************************/

struct HLDStartKeyIntercept             /* 000 Start Keystroke       */
                                        /* Intercept structure       */
{
  unsigned char stki_shortname;         /* 000 Session Shortname     */
  unsigned char stki_extendedname[3];   /* RESERVED                  */
  unsigned char stki_keytyp_opt;        /* 000 Type of keys to       */
                                        /* intercept                 */
  unsigned char stki_reserved1[3];      /* RESERVED                  */
  WAIT_OBJECT        stki_wait;         /* wait_object union         */
                                        /*                           */
  unsigned char stki_akeytyp_opt;       /* 000 async type of keys to */
                                        /* intercept                 */
  unsigned char stki_reserved2[3];      /* RESERVED                  */
}
;                                       /* 000                       */

/*********************************************************************/
/*                     Get Key (51)                                  */
/*********************************************************************/

struct HLDGetKey                        /* 000 Get Key structure.    */
{
  unsigned char gkey_shortname;         /* 000 Session Shortname     */
  unsigned char gkey_extendedname[3];   /* RESERVED                  */
  unsigned char gkey_keytype;           /* 000 Type of key           */
                                        /* intercepted               */
  unsigned char gkey_key[6];            /* 000 Ascii or ascii        */
                                        /* mnemonic                  */
  unsigned char gkey_reserved;          /* RESERVED                  */
}
;                                       /* 000                       */

/*********************************************************************/
/*                     Post Intercept Status (52)                    */
/*********************************************************************/

struct HLDPostInterceptStatus           /* 000 Post Intercept        */
                                        /* Status structure          */
{
  unsigned char pist_shortname;         /* 000 Session Shortname     */
  unsigned char pist_extendedname[3];   /* RESERVED                  */
  unsigned char pist_post_opt;          /* 000 Key accepted or       */
                                        /* rejected                  */
  unsigned char pist_reserved[3];       /* RESERVED                  */
}
;                                       /* 000                       */

/*********************************************************************/
/*                     Stop  Keystroke Intercept (53)                */
/*********************************************************************/

struct HLDStopKeyIntercept              /* 000 Stop Intercept        */
                                        /* Status structure          */
{
  unsigned char spki_shortname;         /* 000 Session Shortname     */
  unsigned char spki_extendedname[3];   /* RESERVED                  */
}
;                                       /* 000                       */

/*********************************************************************/
/*                     Lock Presentation Space API (60)              */
/*********************************************************************/

struct HLDLockPS                        /* 000 LockPS structure     */
{
  unsigned char lkps_shortname;         /* 000 Session Shortname     */
  unsigned char lkps_extendedname[3];   /* RESERVED                  */
  unsigned char lkps_lockopt;           /* 000 Lock/unlock option    */
  unsigned char lkps_queueopt;          /* 000 Queue option          */
  unsigned char lkps_reserved[2];       /* RESERVED                  */
}
;                                       /* 000                       */

/*********************************************************************/
/*                     Lock PMSVC API (61)                           */
/*********************************************************************/

struct HLDLockPMSVC                     /* 000 LockPMSVC structure   */
{
  unsigned char lkpm_shortname;         /* 000 Session Shortname     */
  unsigned char lkpm_extendedname[3];   /* RESERVED                  */
  unsigned char lkpm_lockopt;           /* 000 Lock/unlock option    */
  unsigned char lkpm_queueopt;          /* 000 Queue option          */
  unsigned char lkpm_reserved[2];       /* RESERVED                  */
}
;                                       /* 000                       */

/*********************************************************************/
/*                     Convert Position or Row Column (99)           */
/*********************************************************************/

struct HLDConvertPosRowCol              /* 000 Convert Position      */
                                        /* or Row Column             */
                                        /* structure                 */
{
  unsigned char cvrc_shortname;         /* 000 Session Shortname     */
  unsigned char cvrc_extendedname[3];   /* RESERVED                  */
  unsigned char cvrc_opt;               /* 000 Convert option        */
  unsigned char cvrc_reserved[3];       /* RESERVED                  */
}
;                                       /* 000                       */

/*********************************************************************/
/*                     Connect for PM Services (101)                 */
/*********************************************************************/

struct HLDConnectPM                     /* 000 Connect For           */
                                        /* Presentation Manager      */
                                        /* Services structure        */
{
  unsigned char stpm_shortname;         /* 000 Session Shortname     */
  unsigned char stpm_extendedname[3];   /* RESERVED                  */
}
;                                       /* 000                       */

/*********************************************************************/
/*                     Disconnect from PM Services (102)             */
/*********************************************************************/

struct HLDDisconnectPM                  /* 000 Disconnect From       */
                                        /* Presentation Manager      */
                                        /* Services structure        */
{
  unsigned char sppm_shortname;         /* 000 Session Shortname     */
  unsigned char sppm_extendedname[3];   /* RESERVED                  */
}
;                                       /* 000                       */

/*********************************************************************/
/*                       Query PM Window Coordinates (103)           */
/*********************************************************************/

struct HLDQueryWindowCoords             /* 000 Query PM Window       */
                                        /* Coordinates structure     */
{
  unsigned char gcor_shortname;         /* 000 Session Shortname     */
  unsigned char gcor_extendedname[3];   /* RESERVED                  */
  long int      gcor_xLeft;             /* 000 Left X coordinate     */
  long int      gcor_yBottom;           /* 000 Bottom Y coordinate   */
  long int      gcor_xRight;            /* 000 Right X coordinate    */
  long int      gcor_yTop;              /* 000 Top Y coordinate      */
}
;                                       /* 000                       */

/*********************************************************************/
/*                       PM Window Status (104)                      */
/*********************************************************************/

struct HLDPMWindowStatus                /* 000 PM Window Status      */
                                        /* structure                 */
{
  unsigned char cwin_shortname;         /* 000 Session Shortname     */
  unsigned char cwin_extendedname[3];   /* RESERVED                  */
  unsigned char cwin_option;            /* 000 Set/Query option      */
  unsigned char cwin_reserved;          /* RESERVED                  */
  unsigned short int cwin_flags;        /* 000 Window flags          */
  long int      cwin_xpos;              /* 000 X coordinate position */
  long int      cwin_ypos;              /* 000 Y coordinate position */
  unsigned long int cwin_xsize;         /* 000 X axis window size    */
  unsigned long int cwin_ysize;         /* 000 Y axis window size    */
  unsigned long int cwin_behind;        /* 000 Relative Window       */
                                        /*     placement             */
}
;                                       /* 000                       */

struct HLDPMWindowStatusExt             /* 000 PM Window Status for  */
                                        /* extended structure        */
{
  unsigned char xwin_shortname;         /* 000 Session Shortname     */
  unsigned char xwin_extendedname[3];   /* RESERVED                  */
  unsigned char xwin_option;            /* 000 Set/Query option      */
  unsigned char xwin_reserved1;         /* RESERVED                  */
  unsigned short int xwin_flags;        /* 000 Window flags          */
  unsigned short int xwin_fontx;        /* 000 X font size           */
  unsigned short int xwin_fonty;        /* 000 Y font size           */
  unsigned short int xwin_offsetx;      /* 000 X offset              */
  unsigned short int xwin_offsety;      /* 000 Y offset              */
  unsigned short int xwin_firstrow;     /* 000 first row             */
  unsigned short int xwin_firstcol;     /* 000 first col             */
  HWND          xwin_handle;            /* 000 Window handle         */
}
;                                       /* 000                       */

/*********************************************************************/
/*                Change Switch List Name (105)                      */
/*********************************************************************/

struct HLDChangeSwitchName              /* 000 Change Switch List    */
                                        /* Name structure            */
{
  unsigned char chsw_shortname;         /* 000 Session Shortname     */
  unsigned char chsw_extendedname[3];   /* RESERVED                  */
  unsigned char chsw_option;            /* 000 Set/Reset option      */
  unsigned char chsw_swname[61];        /* 000 Switch List name      */
  unsigned char chsw_reserved[2];       /* RESERVED                  */
}
;                                       /* 000                       */

/*********************************************************************/
/*                Change LT Window Name (106)                        */
/*********************************************************************/

struct HLDChangeWindowName              /* 000 Change LT Window      */
                                        /* Name structure            */
{
  unsigned char chlt_shortname;         /* 000 Session Shortname     */
  unsigned char chlt_extendedname[3];   /* RESERVED                  */
  unsigned char chlt_option;            /* 000 Set/Reset option      */
  unsigned char chlt_ltname[61];        /* 000 LT Window Name        */
  unsigned char chlt_reserved[2];       /* RESERVED                  */
}
;                                       /* 000                       */

/*********************************************************************/
/*                Start Structured Field           (120)             */
/*********************************************************************/

struct HLDConnectSF                     /* 000 Connect SF structure  */
{
  unsigned char stsf_shortname;         /* 000 Session Shortname     */
  unsigned char stsf_extendedname[3];   /* RESERVED                  */
  unsigned char *    stsf_query;        /* 000 4 byte pointer to     */
                                        /* 000 Query reply           */
  unsigned short int stsf_doid;         /* 000 Destination/Orgin id  */
  unsigned char stsf_reserved[2];       /* RESERVED                  */
  WAIT_OBJECT        stsf_wait;         /* wait_object union         */
}
;                                       /* 000                       */

/*********************************************************************/
/*                Stop  Structured Field           (121)             */
/*********************************************************************/

struct HLDDisconnectSF                  /* 000 Disconnect SF         */
                                        /* structure                 */
{
  unsigned char spsf_shortname;         /* 000 Session Shortname     */
  unsigned char spsf_extendedname[3];   /* RESERVED                  */
  unsigned short int spsf_doid;         /* 000 Destination/Orgin id  */
  unsigned char spsf_reserved[2];       /* RESERVED                  */
}
;                                       /* 000                       */

/*********************************************************************/
/*                Query Communications buffer sizes(122)             */
/*********************************************************************/

struct HLDQueryBufferSize               /* 000 Query Communication   */
                                        /* buffer size structure     */
{
  unsigned char qbuf_shortname;         /* 000 Session Shortname     */
  unsigned char qbuf_extendedname[3];   /* RESERVED                  */
  unsigned long int  qbuf_opt_inbound;  /* 000 Optimal inbound buffer*/
                                        /* 000 length                */
  unsigned long int  qbuf_max_inbound;  /* 000 Maximum inbound buffer*/
                                        /* 000 length                */
  unsigned long int  qbuf_opt_outbound; /* 000 Optiomal outbound     */
                                        /* 000 buffer length         */
  unsigned long int  qbuf_max_outbound; /* 000 Maximum outbound      */
                                        /* 000 buffer length         */
}
;                                       /* 000                       */

/*********************************************************************/
/*                Allocate communications buffer   (123)             */
/*********************************************************************/

struct HLDAllocateCommBuff              /* 000 Allocate Communication*/
                                        /* buffer structure          */
{
  unsigned long int abuf_length;        /* 000 request buffer length */
  unsigned char *   abuf_address;       /* 000 4 byte pointer to     */
                                        /* 000 return buffer address */
}
;                                       /* 000                       */

/*********************************************************************/
/*                Free communications buffer       (124)             */
/*********************************************************************/

struct HLDFreeCommBuff                  /* 000 Free Communication    */
                                        /* buffer structure          */
{
  unsigned long int fbuf_length;        /* 000 buffer length to free */
  unsigned char *   fbuf_address;       /* 000 4 byte buffer addr    */
}
;                                       /* 000                       */

/*********************************************************************/
/*                Get Async Request completion     (125)             */
/*********************************************************************/

struct HLDGetAsyncCompletion            /* 000 Get Async Request     */
                                        /* Completion structure      */
{
  unsigned char gcmp_shortname;         /* 000 Session Shortname     */
  unsigned char gcmp_extendedname[3];   /* RESERVED                  */
  unsigned char gcmp_option;            /* 000 Wait option           */
  unsigned char gcmp_reserved[3];       /* RESERVED                  */
  unsigned short int gcmp_requestid;    /* 000 request id            */
  unsigned short int gcmp_ret_functid;  /* 000 returned function id  */
  unsigned char  *   gcmp_ret_datastr;  /* 000 4 byte pointer to     */
                                        /* 000 returned datastring   */
  unsigned long  int gcmp_ret_length;   /* 000 returned length       */
  unsigned long  int gcmp_ret_retcode;  /* 000 returned return code  */
}
;                                       /* 000                       */

/*********************************************************************/
/*                Read  Structured Field            (126)            */
/*********************************************************************/

struct HLDReadSF                        /* 000 Read SF structure     */
{
  unsigned char rdsf_shortname;         /* 000 Session Shortname     */
  unsigned char rdsf_extendedname[3];   /* RESERVED                  */
  unsigned char rdsf_option;            /* 000 Wait option           */
  unsigned char rdsf_reserved1;         /* RESERVED                  */
  unsigned short int rdsf_doid;         /* 000 destination/orgin id  */
  unsigned char  *   rdsf_buffer;       /* 000 4 byte pointer to     */
                                        /* 000 read buffer           */
  unsigned short int rdsf_requestid;    /* 000 request id            */
  unsigned char rdsf_reserved2[2];      /* RESERVED                  */
  WAIT_OBJECT        rdsf_wait;         /* wait_object union         */
                                        /*                           */
}
;                                       /* 000                       */

/*********************************************************************/
/*               Write Structured Field            (127)             */
/*********************************************************************/

struct HLDWriteSF                       /* 000 Write SF structure    */
{
  unsigned char wrsf_shortname;         /* 000 Session Shortname     */
  unsigned char wrsf_extendedname[3];   /* RESERVED                  */
  unsigned char wrsf_option;            /* 000 Wait option           */
  unsigned char wrsf_reserved1;         /* RESERVED                  */
  unsigned short int wrsf_doid;         /* 000 destination/orgin id  */
  unsigned char  *   wrsf_buffer;       /* 000 4 byte pointer to     */
                                        /* 000 write buffer          */
  unsigned short int wrsf_requestid;    /* 000 request id            */
  unsigned char wrsf_reserved2[2];      /* RESERVED                  */
  WAIT_OBJECT        wrsf_wait;         /* wait_object union         */
                                        /*                           */
}
;                                       /* 000                       */



#ifdef __cplusplus
  }
#endif
