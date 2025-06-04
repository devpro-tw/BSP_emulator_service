//-------------------------------------------------------------------------------
// Module:  eclxfer.hpp
//-------------------------------------------------------------------------------
//
// Description:  Declaration of the ECLXfer class
//
//-------------------------------------------------------------------------------
// Copyright Notice: IBM Personal Communication/3270 Version 4.2
//                   (C) COPYRIGHT IBM CORP. 1989,1997 - PROGRAM PROPERTY
//                   OF IBM ALL RIGHTS RESERVED
//-------------------------------------------------------------------------------

#ifndef _ECLXFER_HPP_
#define _ECLXFER_HPP_

#include "eclbase.hpp"
#include "eclcinfo.hpp"

class ECLXferData;                              // Foreward declaration 
class DllExport ECLXfer : public ECLConnection
{
  private: // Class private data
    ECLXferData   *pd;                          

  private: // Constructors and operators not allowed on this object
    ECLXfer();
    ECLXfer(const ECLXfer &From);
    ECLXfer& operator= (const ECLXfer &From);

  public:

    // Constructor/destructor
    ECLXfer(char ConnName);
    ECLXfer(long ConnHandle);
    ~ECLXfer();

    // ECLXfer methods
    int SendFile(const char * const LocalFileName,
                 const char * const HostFileName,
                 const char * const SendParms);

    int ReceiveFile(const char * const LocalFileName,
                    const char * const HostFileName,
                    const char * const RecvParms);

};

typedef ECLXfer * PECLXfer;

#endif //_ECLXFER_HPP_
