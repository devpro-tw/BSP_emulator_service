//-------------------------------------------------------------------------------
// Module:  eclall.hpp
//-------------------------------------------------------------------------------
//
// Description:  Includes all Emulator Class Library header files.
//
//-------------------------------------------------------------------------------
// Copyright Notice: IBM Personal Communication/3270 Version 4.2
//                   (C) COPYRIGHT IBM CORP. 1989,1997 - PROGRAM PROPERTY
//                   OF IBM ALL RIGHTS RESERVED
//-------------------------------------------------------------------------------

#ifndef _ECLALL_HPP_
#define _ECLALL_HPP_

#include "eclbase.hpp"        // ECLBase class (base class for all ECL classes)
#include "eclnotfy.hpp"       // ECLNotify class (ECL event classes)
#include "eclcmgr.hpp"        // ECLConnMgr class (Connection Manager)
#include "eclclist.hpp"       // ECLConnList class (a collection of connections)
#include "eclcinfo.hpp"       // ECLConnection class (an ECL connection)
#include "eclps.hpp"          // ECLPS class (presentation space)
#include "ecloia.hpp"         // ECLOIA class (OIA area)
#include "eclwmet.hpp"        // ECLWinMetrics class (window metrics)
#include "eclxfer.hpp"        // ECLXfer class (file transfer)
#include "eclsess.hpp"        // ECLSession class (contains PS, OIA, WinMetrics, and Xfer objects)
#include "eclflist.hpp"       // ECLFieldList class (a collection of PS fields)
#include "eclfinfo.hpp"       // ECLField class (a PS field)
#include "eclerr.hpp"         // ECLErr class (error messages)
#include "errorids.hpp"       // ECL error defines 

#endif //_ECLALL_HPP_
