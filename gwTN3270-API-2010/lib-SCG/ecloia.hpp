//-------------------------------------------------------------------------------
// Module:  ecloia.hpp
//-------------------------------------------------------------------------------
//
// Description:  ECLOIA class definitions.
//
//-------------------------------------------------------------------------------
// Copyright Notice: IBM Personal Communication/3270 Version 4.2
//                   (C) COPYRIGHT IBM CORP. 1989,1997 - PROGRAM PROPERTY
//                   OF IBM ALL RIGHTS RESERVED
//-------------------------------------------------------------------------------

#ifndef _ECLOIA_HPP_
#define _ECLOIA_HPP_

#include "eclbase.hpp"
#include "eclcinfo.hpp"
#include "eclnotfy.hpp"                     // Event class headers

// Enumeration for InputInhibited method
enum INHIBIT_REASON {
  NotInhibited   = 0,
  SystemWait     = 1,
  CommCheck      = 2,
  ProgCheck      = 3,
  MachCheck      = 4,
  OtherInhibit   = 5
};

// Definition of GetStatusFlags bit flags
const ULONG OIAFLAG_ALPHANUM = 0x80000000;
const ULONG OIAFLAG_APL      = 0x40000000;
const ULONG OIAFLAG_KATAKANA = 0x20000000;
const ULONG OIAFLAG_HIRAGANA = 0x10000000;
const ULONG OIAFLAG_DBCS     = 0x08000000;
const ULONG OIAFLAG_UPSHIFT  = 0x04000000;
const ULONG OIAFLAG_NUMERIC  = 0x02000000;  
const ULONG OIAFLAG_CAPSLOCK = 0x01000000;
const ULONG OIAFLAG_INSERT   = 0x00800000;
const ULONG OIAFLAG_COMMERR  = 0x00400000;
const ULONG OIAFLAG_MSGWAIT  = 0x00200000;
const ULONG OIAFLAG_INHIBMASK= 0x0000000F;  // Mask for inhibit reason

// Forward declarations 
class ECLSession;
class ECLErr;


class ECLOIAData;                           // Foreward declaration 
class DllExport ECLOIA : public ECLConnection
{
  private: // Class private data
    ECLOIAData   *pd;                           

  private: // Constructors and operators not allowed on this object
    ECLOIA();
    ECLOIA(const ECLOIA &From);
    ECLOIA& operator= (const ECLOIA &From);

  public:

    // Constructor/destructor
    ECLOIA(char ConnName);
    ECLOIA(long ConnHandle);
    ~ECLOIA();

    // ECLOIA property access methods
    BOOL IsAlphanumeric() const;
    BOOL IsAPL() const;
    BOOL IsKatakana() const;
    BOOL IsHiragana() const;
    BOOL IsDBCS() const;
    BOOL IsUpperShift() const;
    BOOL IsNumeric() const;
    BOOL IsCapsLock() const;
    BOOL IsInsertMode() const;
    BOOL IsCommErrorReminder() const;
    BOOL IsMessageWaiting() const;
    INHIBIT_REASON InputInhibited() const;
    ULONG GetStatusFlags() const;

};

typedef ECLOIA * PECLOIA;

#endif //_ECLOIA_HPP_
