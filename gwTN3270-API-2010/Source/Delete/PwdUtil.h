//---------------------------------------------------------------------------
#ifndef PwdUtilH
#define PwdUtilH

#include "APUtil.h"
#include "crypto.hpp"
//---------------------------------------------------------------------------

// 產生一組新的密碼
AnsiString CreateCGSPwd( const char * old_pwd ) ;
// 密碼編碼
AnsiString citi_encode(const char* psource ) ;
// 密碼解碼
AnsiString citi_decode(const char* psource ) ;

// 3DES密碼解碼
AnsiString TripleDES_decode( char* psource ) ;

//---------------------------------------------------------------------------

#endif
