//---------------------------------------------------------------------------
#ifndef PwdUtilH
#define PwdUtilH

#include "APUtil.h"
#include "crypto.hpp"
//---------------------------------------------------------------------------

// ���ͤ@�շs���K�X
AnsiString CreateCGSPwd( const char * old_pwd ) ;
// �K�X�s�X
AnsiString citi_encode(const char* psource ) ;
// �K�X�ѽX
AnsiString citi_decode(const char* psource ) ;

// 3DES�K�X�ѽX
AnsiString TripleDES_decode( char* psource ) ;

//---------------------------------------------------------------------------

#endif
