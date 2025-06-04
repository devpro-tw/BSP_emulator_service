//-------------------------------------------------------------------------------
// Module:  eclflist.hpp
//-------------------------------------------------------------------------------
//
// Description:  ECLFieldList class header file.
//
//-------------------------------------------------------------------------------
// Copyright Notice: IBM Personal Communication/3270 Version 4.2
//                   (C) COPYRIGHT IBM CORP. 1989,1997 - PROGRAM PROPERTY
//                   OF IBM ALL RIGHTS RESERVED
//-------------------------------------------------------------------------------

#ifndef _ECLFLIST_HPP_
#define _ECLFLIST_HPP_

#include "eclbase.hpp"
#include "eclps.hpp"                        // Need PS object definitions

// Forward declarations
class ECLField;
class ECLPS;

// Flags for field attributes in GetFirstField() and GetNextField().
// These can be logically OR-ed to get fields with only the specified
// set of attributes (e.g. "GetDisplay|GetUnprotected" would return
// ONLY the fields which are visible input fields -- e.g. meet all criteria
// specified by the set of bit flags).
const unsigned int GetAll         = 0x0000; // Get all fields

const unsigned int GetModified    = 0x0001;
const unsigned int GetDisplay     = 0x0040;
const unsigned int GetProtected   = 0x0020;
const unsigned int GetHilight     = 0x0010;

const unsigned int GetUnmodified  = 0x0100; // "Not" flags are shifted to upper byte
const unsigned int GetNondisplay  = 0x4000;
const unsigned int GetUnprotected = 0x2000;
const unsigned int GetLolight     = 0x1000;


class ECLFieldListData;                     // Forward declaration 
class DllExport ECLFieldList: public ECLBase
{

  private: // Class private data
    ECLFieldListData *pd;                    

  public:

    // Constructor/destructor -- note that a copy constructor and
    // assignment operator are define for this object.  See the
    // ECL documentation for proper usage.

    ECLFieldList();                  // Default constructor
    ECLFieldList(const ECLFieldList &From);  // Copy constructor
    ECLFieldList& operator= (const ECLFieldList &From); // Assignment operator
    ~ECLFieldList();                 // Destructor          

    void        Refresh(PS_PLANE Planes=TextPlane);
    ULONG       GetFieldCount() const;
    ECLField    *GetFirstField(unsigned int Attrb = GetAll) const;
    ECLField    *GetNextField(ECLField *PrevField, unsigned int Attrib = GetAll) const;
    ECLField    *FindField(ULONG Pos) const;                           
    ECLField    *FindField(ULONG Row, ULONG Col) const;  
    ECLField    *FindField(const char * const text, PS_DIR Dir=SrchForward) const; 
    ECLField    *FindField(const char * const text, ULONG Pos, PS_DIR Dir=SrchForward) const;
    ECLField    *FindField(const char * const text, ULONG Row, ULONG Col,  PS_DIR Dir=SrchForward) const;

};

typedef ECLFieldList * PECLFieldList;

#endif //_ECLFLIST_HPP_
