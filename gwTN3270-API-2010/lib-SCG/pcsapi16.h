/*****************************************************************************/
/*                                                                           */
/*  Module Name     : PCSAPI16.H                                             */
/*                                                                           */
/*  Description     : 16 Bit PCSAPI Return Codes and Routines                */
/*                                                                           */
/*  Copyright Notice: IBM Personal Communication/3270 Version 4.1            */
/*                    (C) COPYRIGHT IBM CORP. 1984,1996 - PROGRAM PROPERTY   */
/*                    OF IBM ALL RIGHTS RESERVED                             */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*  Function:                                                                */
/*                                                                           */
/*    Define the PCSAPI return code constants and the external               */
/*    routines.                                                              */
/*                                                                           */
/*****************************************************************************/

#ifdef __cplusplus                     /* C++ compiler                       */
  extern "C" {
#endif

/*--------------------------------------------------------------------------*/
/*  General Purpose Definition                                              */
/*--------------------------------------------------------------------------*/
#define MAXPATHLEN                   100

/*--------------------------------------------------------------------------*/
/*  Definition for Query Session Status                                     */
/*--------------------------------------------------------------------------*/

#define PCS_SESSION_STARTED          0x0001
#define PCS_SESSION_ONLINE           0x0002
#define PCS_SESSION_API_ENABLED      0x0004

/*--------------------------------------------------------------------------*/
/*  Definition for Start Session                                            */
/*--------------------------------------------------------------------------*/

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
/*  Function Prototypes                                                     */
/*--------------------------------------------------------------------------*/

extern WORD far pascal pcsQueryEmulatorStatus(char);
extern BOOL far pascal pcsQueryWorkstationProfile(char, LPSTR);
extern WORD far pascal pcsStartSession(LPSTR, char, WORD);
extern BOOL far pascal pcsStopSession(char, WORD);
extern BOOL far pascal pcsConnectSession(char);
extern BOOL far pascal pcsDisconnectSession(char);

#ifdef __cplusplus
  }
#endif
