//-------------------------------------------------------------------------------
// Module:  eclopsys.hpp
//-------------------------------------------------------------------------------
//
// Description:  Operating system compatibility definitions and macros.  Allows
//               common ECL header files across platforms.
//
//-------------------------------------------------------------------------------
// Copyright Notice: IBM Personal Communication/3270 Version 4.2
//                   (C) COPYRIGHT IBM CORP. 1989,1997 - PROGRAM PROPERTY
//                   OF IBM ALL RIGHTS RESERVED
//-------------------------------------------------------------------------------
#if !defined(ECLOPSYS_INCLUDED)
#define ECLOPSYS_INCLUDED

#if defined(__OS2__)  // OS/2 compiler?
  #define OS2
#else         
  #define WIN
#endif

// Load operating system headers

#if defined(WIN)
  #include <windows.h>
#endif

#if defined(OS2)
  #define INCL_DOSMODULEMGR
  #define INCL_DOSERRORS
  #include <os2.h>
#endif

#endif // ECLOPSYS_INCLUDED
