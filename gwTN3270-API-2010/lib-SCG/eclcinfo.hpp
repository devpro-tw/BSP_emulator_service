//-------------------------------------------------------------------------------
// Module:  eclcinfo.hpp
//-------------------------------------------------------------------------------
//
// Description:  ECLConnection class header file.  Base class for all PCOMM 
//               session-specific objects (e.g. PS, OIA, etc).  Can also be 
//               used stand-alone to query the status of a particular host 
//               connection.
//
//-------------------------------------------------------------------------------
// Copyright Notice: IBM Personal Communication/3270 Version 4.2
//                   (C) COPYRIGHT IBM CORP. 1989,1997 - PROGRAM PROPERTY
//                   OF IBM ALL RIGHTS RESERVED
//-------------------------------------------------------------------------------
#ifndef _HSINFO_HPP_
#define _HSINFO_HPP_

#include "eclbase.hpp"                      // Need HostBase class definitions
#include "eclnotfy.hpp"                     // Notify classes

class ECLConnData;                          // Forward declaration 
class DllExport ECLConnection: public ECLBase
{

  private: // Class private data
    ECLConnData *ConnPd;                


  private: // Constructors and operators not allowed on this object
    ECLConnection();
    ECLConnection(const ECLConnection &From);
    ECLConnection& operator= (const ECLConnection &From);

  public:

    ECLConnection(long ConnHandle);         // Construct from connection handle
    ECLConnection(char ConnName);           // Construct from connection name
    ~ECLConnection();

    // Information functions
    long  GetHandle() const;                // Connection handle
    int   GetConnType() const;              // Connection type (HOSTTYPE_* constant)
    char  GetName() const;                  // EHLLAPI short name
    unsigned int GetCodePage() const;       // Connection code page
    BOOL  IsStarted() const;                // Connection is running?
    BOOL  IsCommStarted() const;            // Connection connected to a host?
    BOOL  IsAPIEnabled() const;             // Connection is enabled for APIs?
    BOOL  IsReady() const;                  // Started & CommStarted & APIEnabled?
    BOOL  IsDBCSHost() const;               // Host code page is known DBCS code page

    // Communications functions
    void StartCommunication();              // Start communications with host
    void StopCommunication();               // Stop communications with host
    void RegisterCommEvent(ECLCommNotify *CommNotifyClass, BOOL InitEvent=TRUE);
    void UnregisterCommEvent(ECLCommNotify *CommNotifyClass);

};

typedef ECLConnection * PECLConnection;

#endif //_HSINFO_HPP_
