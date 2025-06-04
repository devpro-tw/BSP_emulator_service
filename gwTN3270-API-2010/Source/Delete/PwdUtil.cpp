//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "PwdUtil.h"
#include <stdio.h>
#include "crypto.hpp"

using namespace winobj;
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
// Private Functions
//---------------------------------------------------------------------------

int hx2int(unsigned char ch)
{
	switch(ch) {
	case 0x30:  return 0;

	case 0x31:
		return 1;

	case 0x32:
		return 2;

	case 0x33:
		return 3;

	case 0x34:
		return 4;

	case 0x35:
		return 5;

	case 0x36:
		return 6;

	case 0x37:
		return 7;

	case 0x38:
		return 8;

	case 0x39:
		return 9;

	case 0x61:
		return 10;

	case 0x62:
		return 11;

	case 0x63:
		return 12;

	case 0x64:
		return 13;

	case 0x65:
		return 14;

	case 0x66:
		return 15;

	case 0x41:
		return 10;

	case 0x42:
		return 11;

	case 0x43:
		return 12;

	case 0x44:
		return 13;

	case 0x45:
		return 14;

	case 0x46:
		return 15;

	default:
		return 0;
	}
}

AnsiString hx_encode(unsigned char key, const char* psource, char* pecode, int nbuflen)
{
	int slen;	// length of source string
	slen = strlen(psource);

	unsigned char e_tmp[4];
	unsigned char e_code[4];
	for (int i = 0; i < slen; i++)
	{
		sprintf((char*)e_tmp, "%02x", (unsigned char)psource[i]);
		e_code[0] = e_tmp[0] ^ key;
		e_code[1] = e_tmp[1] ^ key;
		sprintf((char*)(pecode + i * 4), "%02x%02x", e_code[0], e_code[1]);
	}
    
	return AnsiString( pecode );
}

AnsiString hx_decode(unsigned char key, const char* psource, char* pcode, int nbuflen)
{
	int slen, n;
	slen = strlen(psource) / 4;

	unsigned char e_code[8];

	for (int i = 0; i < slen; i++)
	{
		strncpy((char*)e_code, (const char*)(psource + i * 4), 4);
		for (n = 0; n < 2; n++)
		{
			e_code[n] = 16 * hx2int(e_code[n * 2]) + hx2int(e_code[n * 2 + 1]);
		}
		e_code[0] = e_code[0] ^ key;
		e_code[1] = e_code[1] ^ key;
		unsigned char d = 16 * hx2int(e_code[0]) + hx2int(e_code[1]);
		pcode[i] = d;
	}
	pcode[slen] = 0;
    
	return AnsiString( pcode );
}


//---------------------------------------------------------------------------
// Public Functions
//---------------------------------------------------------------------------

// 產生一組新的密碼
AnsiString CreateCGSPwd( const char * old_pwd )
{
    char new_pwd[255] ;
    int nIndex , nPwdLen , a_cnt = 0 , n_cnt = 0 ;

    strcpy( new_pwd , old_pwd ) ;
    // 重取亂數種子
    randomize() ;
    nPwdLen = strlen( new_pwd );

    for( nIndex = 0 ; nIndex < nPwdLen ; nIndex ++ ) {
        // 將所有密碼值加一
        switch( new_pwd[nIndex] ) {
            case 'Z':
            case 'z':
                    new_pwd[nIndex] = '0' ;
                    break;

            case '9':
                    if( rand() % 2 )
                        new_pwd[nIndex] = 'a' ;
                    else
                        new_pwd[nIndex] = 'A' ;
                    break;
            default:
                    new_pwd[nIndex] = new_pwd[nIndex] + 1 ;
                    break;
        } // switch

        // 如果與前一碼相同，重新加一
        if( ( nIndex > 0 ) && ( new_pwd[nIndex] == new_pwd[nIndex-1] ) ) {
            nIndex-- ;
        } // if
        // 如果是數字
        else if( ( new_pwd[nIndex] >= '0' ) && ( new_pwd[nIndex] <= '9' ) ) {
            n_cnt ++ ;
        } // else if
        else {
            a_cnt ++ ;
        } // else
    } //  for

    // 如果沒有數字
    if( (nPwdLen >= 2) && (!n_cnt) ) {
		new_pwd[0] = '0';
		new_pwd[nPwdLen - 1] = '1';
    } // if
    // 如果沒有英文字
    else if( (nPwdLen >= 4) && (!a_cnt) ) {
        strncpy( new_pwd , "aZpQ" , 4 );
    } // else if

    return AnsiString( new_pwd ) ;
}

/*
// 密碼編碼
AnsiString pwd_encode(const char* pwd )
{
    char e_str[1024] , * p_str ;
	unsigned char seed , e_tmp[4], e_code[4];
    int nLen , i ;

    p_str = e_str ;
    // 重取亂數種子
    randomize() ;

	seed = rand() % 256;
	sprintf(p_str, "%04d", seed );

    nLen = strlen( pwd ) ;
	for( i = 0 ; i < nLen ; i++ ) {
        p_str += 4 ;
		sprintf((char*)e_tmp, "%02x", (unsigned char)pwd[i]);
		e_code[0] = e_tmp[0] ^ seed;
		e_code[1] = e_tmp[1] ^ seed;
		sprintf( p_str , "%02x%02x", e_code[0], e_code[1]);
	} // if


	return AnsiString( e_str ) ;
}

// 密碼解碼
AnsiString pwd_decode( const char * e_str )
{
    char pwd[1024] , * p_str ;
	unsigned char seed , e_code[8] ;
    int nLen , i , n ;

    p_str = (char *)e_str ;
	seed = AnsiString( p_str ).SubString(1,4).ToIntDef(0);
    p_str += 4 ;

    nLen = strlen( p_str ) ;
	for( i = 0 ; i < nLen ; i++ ) {
		strncpy((char*)e_code, (const char*)(p_str + i * 4), 4);
		for( n = 0 ; n < 2 ; n++ ) {
			e_code[n] = 16 * hx2int(e_code[n * 2]) + hx2int(e_code[n * 2 + 1]);
		} // for
		e_code[0] = e_code[0] ^ seed;
		e_code[1] = e_code[1] ^ seed;
		unsigned char d = 16 * hx2int(e_code[0]) + hx2int(e_code[1]);
		pwd[i] = d;
	} // for
	pwd[nLen] = 0;

	return AnsiString( pwd ) ;
}
*/

// 密碼編碼
AnsiString citi_encode(const char* psource )
{
    char pecode[1024] ;

	char szseed[5];
	unsigned char seed;
    // 重取亂數種子
    randomize() ;
//	srand((unsigned)time(NULL));
	seed = rand() % 256;
	sprintf(szseed, "%04d", seed);
	strcpy(pecode, szseed);
	hx_encode(seed, psource, (char*)(pecode + 4), sizeof(pecode) - 4);
    return AnsiString( pecode );
}

// 密碼解碼
AnsiString citi_decode(const char* psource )
{
    char pcode[1024] ;

	char szseed[5];
	unsigned char seed;
	strncpy(szseed, psource, 4);
	szseed[4] = 0;
	seed = atoi(szseed);
	return hx_decode(seed, (const char*)(psource + 4), (char*)pcode, sizeof(pcode) );
}


AnsiString TripleDES_decode( char* psource )
{
    char 		l_Buffer[ 512 ],
    			l_Hex[ 1024 ];
    DWORD		l_Length;

    try
    {
        AnsiString error_msg;
        AnsiString FileName;

        FileName = ExtractFilePath( Application->ExeName ) + "Configure.Ini";

	    l_Length = HexToBin( psource, l_Buffer, sizeof( l_Buffer ) );
       	try
   		{
            error_msg = "TripleDesCtx ";
    		TripleDesCtx l_tdc( "3DES_Engsound" );
			TripleDesKey l_tdk( &l_tdc );

            error_msg = "loadKey ";
            l_tdk.loadKey( "UMSInfo", "BLOB", FileName.c_str() );
            error_msg = "decrypt ";
			l_Length = l_tdk.decrypt(l_Buffer, l_Length, true);
        	l_Buffer[ l_Length ] = 0;
            return AnsiString( l_Buffer );
        }
		catch (crypt_exception& e)
        {
		    g_CommLog->WriteLog( "[3DES Decode Exception]: %s" , error_msg + e.what() );
        }
		catch (...)
		{
			g_CommLog->WriteLog( "[3DES Decode Exception]: Unknown Exception" );
		}
        return "";
	}
    catch( ... )
    {
    }
    return "";
}

