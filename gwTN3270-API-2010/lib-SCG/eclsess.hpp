//-------------------------------------------------------------------------------
// Module:  eclsess.hpp
//-------------------------------------------------------------------------------
//
// Description:  Declaration of ECLSession class 
//
//-------------------------------------------------------------------------------
// Copyright Notice: IBM Personal Communication/3270 Version 4.2
//                   (C) COPYRIGHT IBM CORP. 1989,1997 - PROGRAM PROPERTY
//                   OF IBM ALL RIGHTS RESERVED
//-------------------------------------------------------------------------------

#ifndef _ECLSESS_HPP_
#define _ECLSESS_HPP_

#include "eclcinfo.hpp"
#include "eclwmet.hpp"
#include "eclps.hpp"
#include "ecloia.hpp"
#include "eclxfer.hpp"
#include "eclnotfy.hpp"

class ECLSessionData;                       // Forward declaration 
class DllExport ECLSession : public ECLConnection
{

  private: // Class private data
    ECLSessionData   *pd;                    

  private: // Constructors and operators not allowed on this object
    ECLSession();
    ECLSession(const ECLSession &From);
    ECLSession& operator= (const ECLSession &From);

  public:
    // Constructor/destructor
    ECLSession(char ConnName);
    ECLSession(long ConnHandle);
    ~ECLSession();

    // ECLSession property access methods
    ECLWinMetrics  *GetWinMetrics() const;
    ECLPS          *GetPS() const;
    ECLOIA         *GetOIA() const;
    ECLXfer        *GetXfer() const;

    // Events
    void RegisterUpdateEvent(UPDATETYPE Type, ECLUpdateNotify *UpdateNotifyClass, BOOL InitEvent=TRUE);
    void UnregisterUpdateEvent(ECLUpdateNotify *UpdateNotifyClass);

};

typedef ECLSession * PECLSession;

#endif //_ECLSESS_HPP_
