//-------------------------------------------------------------------------------
// Module:  eclerr.hpp
//-------------------------------------------------------------------------------
//
// Description:  ECLErr class definitions.
//
//-------------------------------------------------------------------------------
// Copyright Notice: IBM Personal Communication/3270 Version 4.2
//                   (C) COPYRIGHT IBM CORP. 1989,1997 - PROGRAM PROPERTY
//                   OF IBM ALL RIGHTS RESERVED
//-------------------------------------------------------------------------------

#ifndef _ECLERR_HPP_
#define _ECLERR_HPP_

#include "eclbase.hpp"
#include "errorids.hpp"


class ECLErrData;                          // Forward declaration 
class DllExport ECLErr : public ECLBase
{
  private: // Class private data
    ECLErrData *pd;                       // Class private data 

  public:

    // Constructor/destructor
    ECLErr();                                 // Default constructor
    ECLErr(const ECLErr &From);               // Copy constructor  
    ECLErr& operator= (const ECLErr &From);   // Assignment operator
    ~ECLErr();                                // Destructor

    // ECLErr access functions
    const int GetMsgNumber() const;
    const int GetReasonCode() const;
    const char *GetMsgText();
};

typedef ECLErr * PECLErr;

#endif //_ECLERR_HPP_
