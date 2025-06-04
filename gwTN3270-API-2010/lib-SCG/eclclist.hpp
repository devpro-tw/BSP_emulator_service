//-------------------------------------------------------------------------------
// Module:  eclclist.hpp
//-------------------------------------------------------------------------------
//
// Description:  ECLConnList class header file.  This class provides an 
//               enumeration of the currently available host sessions.  When 
//               created and when the Refresh() function is invoked, a snap-shot 
//               of the currently available sessions is made.  The connection 
//               list is not updated if connections are started or stopped.  When 
//               destroyed, all the ECLConnection objects created by this class 
//               are deleted.
//
//-------------------------------------------------------------------------------
// Copyright Notice: IBM Personal Communication/3270 Version 4.2
//                   (C) COPYRIGHT IBM CORP. 1989,1997 - PROGRAM PROPERTY
//                   OF IBM ALL RIGHTS RESERVED
//-------------------------------------------------------------------------------

#ifndef _ECLCLIST_HPP_
#define _ECLCLIST_HPP_

#include "eclbase.hpp"    
#include "eclcinfo.hpp"   

class ECLConnListData;                      // Forward declaration 
class DllExport ECLConnList: public ECLBase
{
  private: // Class private data
    ECLConnListData *pd;                    

  private: // Constructors and operators not allowed on this object
    ECLConnList(const ECLConnList &From);
    ECLConnList& operator= (const ECLConnList &From);

  public:
    ECLConnList();                          // Constructor
    ~ECLConnList();                         // Destructor

    ECLConnection *GetFirstConnection() const;    // Get first connection in the list; reset enumeration
    ECLConnection *GetNextConnection(ECLConnection *Prev) const; // Get next connection in the enumeration
    ECLConnection *FindConnection(long ConnHandle) const; // Find connection by handle
    ECLConnection *FindConnection(char ConnName) const; // Find connection by name
    ULONG GetCount() const;                       // Number of connections in the list
    void Refresh();                               // Get new list of connections and reset enumeration

};

typedef ECLConnList * PECLConnList;

#endif //_ECLCLIST_HPP_
