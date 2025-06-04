//-------------------------------------------------------------------------------
// Module:  eclps.hpp
//-------------------------------------------------------------------------------
//
// Description:  ECLPS class header file
//
//-------------------------------------------------------------------------------
// Copyright Notice: IBM Personal Communication/3270 Version 4.2
//                   (C) COPYRIGHT IBM CORP. 1989,1997 - PROGRAM PROPERTY
//                   OF IBM ALL RIGHTS RESERVED
//-------------------------------------------------------------------------------

#ifndef _CHOSTPS_HPP_
#define _CHOSTPS_HPP_

#include "eclbase.hpp"
#include "eclopsys.hpp"
#include "eclcinfo.hpp"
#include "eclnotfy.hpp"                     // Event class headers


#define  FATTR_MDT                0x01                                   
#define  FATTR_PEN_MASK           0x0C                                   
#define  FATTR_BRIGHT             0x08                                   
#define  FATTR_DISPLAY            0x0C                                   
#define  FATTR_ALPHA              0x10                                   
#define  FATTR_NUMERIC            0x10                                   
#define  FATTR_PROTECTED          0x20                                   
#define  FATTR_PRESENT            0x80                                   

#define  FATTR_52_BRIGHT          0x10                                   
#define  FATTR_52_DISP            0x40                                   

#define  FATTR_52_MASK            0x0E                                   
                                            // 5250 Field attribute format:
                                            // 0000 xxx0  bit 4-6
                                            //
                                            // 000 = 0 = alpha shift
                                            // 001 = 1 = alpha only
                                            // 010 = 2 = numeric shift
                                            // 011 = 3 = numeric only
                                            // 100 = 4 = katakana shift
                                            // 101 = 5 = digits only
                                            // 110 = 6 = i/o - mag stripe
                                            // 111 = 7 = signed numeric

//------------------- Enumerations
enum PS_PLANE {                             // Presentation Space data planes
  NoPlane      = 0x00,                      // No data
  TextPlane    = 0x01,                      // Text plane (0x00 terminated)
  ColorPlane   = 0x02,                      // Color plane
  FieldPlane   = 0x04,                      // Field attribute plane
  ExfieldPlane = 0x08,                      // Extended field (character) attribute plane
  DBCSPlane    = 0x10,                      // DBCS character plane
  GridPlane    = 0x20,                      // DBCS grid line plane
  AllPlanes    = 0x3F                       // All planes
};                                                                               
                                                                                 
enum PS_DIR {                               // Search direction                  
  SrchForward,                              // Search forward                    
  SrchBackward                              // Search backward                   
};

// Forward declarations
class ECLFieldList;
class ELCFieldInfo;

class ECLPSData;                            // Foreward declaration 
class DllExport ECLPS: public ECLConnection
{
  private: // Class private data
    ECLPSData   *pd;                            

  private: // Constructors and operators not allowed on this object
    ECLPS();
    ECLPS(const ECLPS &From);
    ECLPS& operator= (const ECLPS &From);

  public:

    // Constructor/destructor
    ECLPS(char ConnName);
    ECLPS(long ConnHandle);
    ECLPS& operator= (const char * const String);  // String assignment
    ECLPS& operator= (const LONG Value);           // Numeric assignment
    ~ECLPS();

    ULONG            GetSize() const;
    ULONG            GetSizeRows() const;
    ULONG            GetSizeCols() const;
    void             GetSize(ULONG * Row, ULONG * Col) const;

    ULONG            GetCursorPos() const;
    ULONG            GetCursorPosRow() const;
    ULONG            GetCursorPosCol() const;
    void             GetCursorPos(ULONG * Row, ULONG * Col) const;

    void             SetCursorPos(ULONG Pos);
    void             SetCursorPos(ULONG Row, ULONG Col);

    void             SendKeys(const char * const text);
    void             SendKeys(const char * const text, ULONG AtPos);
    void             SendKeys(const char * const text, ULONG AtRow, ULONG AtCol);
    void             StartMacro(const char * const MacroName);

    ULONG            SearchText(const char * const text, PS_DIR Dir=SrchForward, BOOL FoldCase=FALSE) const;
    ULONG            SearchText(const char * const text, ULONG StartPos, PS_DIR Dir=SrchForward, BOOL FoldCase=FALSE) const;
    ULONG            SearchText(const char * const text, ULONG StartRow, ULONG StartCol, PS_DIR Dir=SrchForward, BOOL FoldCase=FALSE) const;

    ULONG            GetScreen(char * Buff, ULONG BuffLen, PS_PLANE Plane=TextPlane) const;
    ULONG            GetScreen(char * Buff, ULONG BuffLen, ULONG StartPos, ULONG Length, PS_PLANE Plane=TextPlane) const;
    ULONG            GetScreen(char * Buff, ULONG BuffLen, ULONG StartRow, ULONG StartCol, ULONG Length, PS_PLANE Plane=TextPlane) const;
    ULONG            GetScreenRect(char * Buff, ULONG BuffLen, ULONG StartPos, ULONG EndPos, PS_PLANE Plane=TextPlane) const;
    ULONG            GetScreenRect(char * Buff, ULONG BuffLen, ULONG StartRow, ULONG StartCol, ULONG EndRow, ULONG EndCol, PS_PLANE Plane=TextPlane) const;

    void             SetText(const char * const text);  
    void             SetText(const char * const text, ULONG AtPos);  
    void             SetText(const char * const text, ULONG AtRow, ULONG AtCol);  

    ULONG            ConvertPosToRow(ULONG Pos) const;
    ULONG            ConvertPosToCol(ULONG Pos) const;
    void             ConvertPosToRowCol(ULONG Pos, ULONG * Row, ULONG * Col) const;
    ULONG            ConvertRowColToPos(ULONG Row, ULONG Col) const;

    ECLFieldList *   GetFieldList() const;

    // Functions to register interest in various events
    void RegisterKeyEvent(ECLKeyNotify *NotifyObject);
    void UnregisterKeyEvent(ECLKeyNotify *NotifyObject);

};

typedef ECLPS * PECLPS;

#endif //_CHOSTPS_HPP_
