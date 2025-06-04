/*****************************************************************************/
/*                                                                           */
/*  Module Name     : PCSAPI32.H                                             */
/*                                                                           */
/*  Description     : 32 Bit PCSAPI Return Codes and Routines                */
/*                                                                           */
/*  Copyright Notice: IBM eNetwork Personal Communication                    */
/*                    (C) COPYRIGHT IBM CORP. 1984,1997 - PROGRAM PROPERTY   */
/*                    OF IBM ALL RIGHTS RESERVED                             */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*  Function:                                                                */
/*                                                                           */
/*    Define the PCSAPI return code constants and the external               */
/*    routines.                                                              */
/*                                                                           */
/*****************************************************************************/
/* V4.2 changes:                                                             */
/*   - Added pcsQuerySessionList() function and data structures              */
/*   - Added ordinal entry point values for dynamic loading of PCSAPI32.DLL  */
/*   - Added #define's for pcsStartSession() window-status parameter.        */
/*****************************************************************************/

#ifdef __cplusplus                     /* C++ compiler                       */
  extern "C" {
#endif

/*--------------------------------------------------------------------------*/
/*  General Purpose Definition                                              */
/*--------------------------------------------------------------------------*/
#define MAXPATHLEN                    256

/*--------------------------------------------------------------------------*/
/*  Definition for Query Session Status                                     */
/*--------------------------------------------------------------------------*/

#define PCS_SESSION_STARTED          0x00000001
#define PCS_SESSION_ONLINE           0x00000002
#define PCS_SESSION_API_ENABLED      0x00000004

/*--------------------------------------------------------------------------*/
/*  Definition for Start Session                                            */
/*--------------------------------------------------------------------------*/

#define PCS_HIDE                     0
#define PCS_SHOW                     1
#define PCS_MINIMIZE                 2
#define PCS_MAXIMIZE                 3

#define PCS_SUCCESSFUL               0
#define PCS_INVALID_ID               1
#define PCS_USED_ID                  2
#define PCS_INVALID_PROFILE          3
#define PCS_SYSTEM_ERROR             9

/*--------------------------------------------------------------------------*/
/*  Definition for Stop Session                                             */
/*--------------------------------------------------------------------------*/

#define PCS_SAVE_AS_PROFILE          0
#define PCS_SAVE_ON_EXIT             1
#define PCS_NOSAVE_ON_EXIT           2

/*--------------------------------------------------------------------------*/
/*  Definition for Query Session List (added V4.2)                          */
/*--------------------------------------------------------------------------*/

typedef union _SESSNAME {   // Name field of SessInfo structure
  char ShortName;           // Short session ID (A-Z)
  ULONG Handle;             // Session handle
  } SESSNAME;

typedef struct _SESSINFO {  // Description of a single session
  SESSNAME Name;            // Session name (ID or handle)
  ULONG    Status;          // Session status (PCS_SESSION_* bit flags)
  } SESSINFO;

/*--------------------------------------------------------------------------*/
/*  Function Prototypes                                                     */
/*--------------------------------------------------------------------------*/
#if defined(__OS2__)
  #define PCSAPI_ENTRY APIENTRY
#else
  #define PCSAPI_ENTRY WINAPI
#endif

ULONG PCSAPI_ENTRY pcsQueryEmulatorStatus(char);
BOOL  PCSAPI_ENTRY pcsQueryWorkstationProfile(char, PSZ);
ULONG PCSAPI_ENTRY pcsStartSession(PSZ, char, USHORT);
BOOL  PCSAPI_ENTRY pcsStopSession(char, USHORT);
BOOL  PCSAPI_ENTRY pcsConnectSession(char);
BOOL  PCSAPI_ENTRY pcsDisconnectSession(char);
ULONG PCSAPI_ENTRY pcsQuerySessionList(ULONG Count, SESSINFO *SessList); // Added V4.2


/*--------------------------------------------------------------------------*/
/*  Ordinal numbers for function entry points                               */
/*--------------------------------------------------------------------------*/

#define ord_pcsQueryEmulatorStatus     3
#define ord_pcsQueryWorkstationProfile 4
#define ord_pcsStartSession            5
#define ord_pcsStopSession             6
#define ord_pcsConnectSession          7
#define ord_pcsDisconnectSession       8
#define ord_pcsQuerySessionList        9

#ifdef __cplusplus
  }
#endif
