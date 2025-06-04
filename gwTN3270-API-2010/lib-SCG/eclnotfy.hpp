//-------------------------------------------------------------------------------
// Module:  eclnotfy.hpp
//-------------------------------------------------------------------------------
//
// Description:  ECL event class header file.  
//
//-------------------------------------------------------------------------------
// Copyright Notice: IBM Personal Communication/3270 Version 4.2
//                   (C) COPYRIGHT IBM CORP. 1989,1997 - PROGRAM PROPERTY
//                   OF IBM ALL RIGHTS RESERVED
//-------------------------------------------------------------------------------

#ifndef _ECLNOTIFY_HPP_
#define _ECLNOTIFY_HPP_

#include "eclbase.hpp"                      // Need ECLBase class definitions
#include "eclerr.hpp"                       // ECLErr object

// Forward declarations of classes referenced here
class ECLPS;
class ECLConnection;
class ECLSession;
class ECLConnMgr;

//----------------- Enumerations ------------------
enum UPDATETYPE {
  PSUpdate = 'P',                           // Just PS update
  OIAUpdate= 'O',                           // Just OIA update
  PSOIAUpdate = 'B'                         // PS and OIA update
};


//-------------------------------------------------------------------------------
// Base class for all ECL event classes.
//-------------------------------------------------------------------------------
class DllExport ECLNotify: public ECLBase
{

  public:
    ECLNotify();
    ~ECLNotify();

};


//-------------------------------------------------------------------------------
// Keystroke event class (implemented by user application).  User application
// must implement all functions.  Register an object of this class with the
// ECLPS object to be notified of keystrokes.
//-------------------------------------------------------------------------------
class DllExport ECLKeyNotify: public ECLNotify
{

  public:
    ECLKeyNotify();
    ~ECLKeyNotify();

    // Pure virtual functions all derived classes must implement
    virtual int  NotifyEvent (ECLPS *PSObj, char const KeyType[2], const char * const KeyString) = 0;

    // Virtual functions derived classes can override
    virtual void NotifyError (ECLPS *PSObj, ECLErr ErrObject); // Displays error message
    virtual void NotifyStop  (ECLPS *PSObj, int Reason); // Does nothing

};


//-------------------------------------------------------------------------------
// Dis/connect event class (implemented by user application).  User application
// must implement all functions.  Register an object of this class with the
// ECLConnection object to be notified of host communication connect/disconnects.
//-------------------------------------------------------------------------------
class DllExport ECLCommNotify: public ECLNotify
{

  public:
    ECLCommNotify();
    ~ECLCommNotify();

    // Pure virtual functions all derived classes must implement
    virtual void NotifyEvent (ECLConnection *ConnObj, BOOL Connected) = 0;

    // Virtual functions derived classes can override
    virtual void NotifyError (ECLConnection *ConnObj, ECLErr ErrObject); // Displays error message
    virtual void NotifyStop  (ECLConnection *ConnObj, int Reason); // Does nothing

};


//-------------------------------------------------------------------------------
// Session start/stop event class (implemented by user application).  User application
// must implement all functions.  Register an object of this class with the
// ECLConnMgr object to be notified of all connection start/stop events.
//-------------------------------------------------------------------------------
class DllExport ECLStartNotify: public ECLNotify
{

  public:
    ECLStartNotify();
    ~ECLStartNotify();

    // Pure virtual functions all derived classes must implement
    virtual void NotifyEvent (ECLConnMgr *CMObj, long ConnHandle, BOOL Started) = 0;

    // Virtual functions derived classes can override
    virtual void NotifyError (ECLConnMgr *CMObj, long ConnHandle, ECLErr ErrObject); // Displays error message
    virtual void NotifyStop  (ECLConnMgr *CMObj, int Reason); // Does nothing

};


//-------------------------------------------------------------------------------
// Update event class (implemented by user application).  User application
// must implement all functions.  Register an object of this class with the
// ECLSession object to be notified of PS and/or OIA updates.
//-------------------------------------------------------------------------------
class DllExport ECLUpdateNotify: public ECLNotify
{
  public:
    ECLUpdateNotify();
    ~ECLUpdateNotify();

    // Pure virtual functions all derived classes must implement
    virtual void NotifyEvent (ECLSession *SessObj, UPDATETYPE Type) = 0;

    // Virtual functions derived classes can override
    virtual void NotifyError (ECLSession *SessObj, ECLErr ErrObject); // Displays error message
    virtual void NotifyStop  (ECLSession *SessObj, int Reason); // Does nothing

};

#endif // _ELCNOTIFY_HPP_
