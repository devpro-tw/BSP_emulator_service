// EhllapiImpl.cpp: implementation of the EhllapiImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "EhllapiImpl.h"
#include "Ehlapi32.h"
#include ".\ehllapiimpl.h"
#include "APUtil.h"
#include <string>
#include "sysutils.hpp"
#include <assert.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
using namespace std;

namespace HACL	// Host Access Class Library
{
//////////////////////////////////////////////////////////////////////
// EhllapiImpl Class
//////////////////////////////////////////////////////////////////////

// Initialize api library
bool EhllapiImpl::Initialize( char * dllpath )
	{
            DevproDebugString( AnsiString().sprintf("EhllapiImpl::EhllapiImpl DLLPath name = %s " , dllpath ).c_str());
            if ( hLib == NULL ) {

                //sprintf( szDllFileName , "%s\\%s" , dllpath , "PCSHLL32.DLL") ;
                //sprintf( szDllName , "%s\\PCtmp%04X.dll" , dllpath , GetCurrentThreadId() ) ;
                //CopyFile( szDllFileName , szTempDllName , false ) ;
                strcpy( szTempDllName , "c:\\tn3270nf\\PCSHLL32.DLL" );
                hLib = LoadLibrary( szTempDllName );
                if ( hLib != NULL ) {
                        assert(hLib);
                        vx = (_vx)GetProcAddress(hLib, "hllapi");
                        assert(vx);
                }
            }
            DevproDebugString( AnsiString().sprintf("Load DLL name = %s , Lib = %x , apiaddr = %x" , szTempDllName , hLib , vx ).c_str());

            return hLib != NULL;
	}

	// release api library
	void EhllapiImpl::UnInitialize(void)
	{
        if ( hLib != NULL ) {
            FreeLibrary(hLib);
            DeleteFile( szTempDllName ) ;
			hLib = NULL;
			vx = NULL;
        }   
	}

	// Construction
	EhllapiImpl::EhllapiImpl( char*dllpath  = "c:\\TN3270NF" )
		:m_IsConnect(false), m_SessionId(0)
	{
                DevproDebugString( "EhllapiImpl::EhllapiImpl" ) ;
                prevtick = 0 ;

		//strcpy(szDllPath,dllpath);
                hLib = NULL ;
                vx = NULL ;
		Initialize( dllpath );
	}

	// Destruction
	EhllapiImpl::~EhllapiImpl()
	{
        UnInitialize();
	}

	void EhllapiImpl::SessionId(char id)
	{
		m_SessionId = id;
	}

	char EhllapiImpl::SessionId()
	{
		return m_SessionId;
	}

	// call hllapi
	int EhllapiImpl::call_api()
	{
        bool bDebug = false ;
        AnsiString sFileName ;
        if ( bDebug ) {       
            sFileName.sprintf( "./log/emutrace-%s.log" , m_SessionStr ) ;
        }
        DWORD tick = GetTickCount() ;
        DWORD idletick = tick - prevtick ;
		try{
            if ( vx ) {
			    (vx)(&m_nFuncCode, (LPSTR)m_szStr, (LPINT)&m_nLength, (LPINT)&m_nRetCode);
                if (m_nFuncCode != QUERY_CURSOR_LOC)
                    m_szStr[m_nLength] = 0 ;
            }
			//return (xhllapi)(&m_nFuncCode, (LPSTR)m_szStr, (LPINT)&m_nLength, (LPINT)&m_nRetCode);
		}
		catch (...) {
			//throw EhllapiError(m_nFuncCode, m_nRetCode, "unknown exception [hllapi]");
            // MessageBox( NULL , "EHLLAPI Exception" ,  "Exception" , MB_OK ) ;
            LastErrMsg = "EHLLAPI Exception" ;
            DWORD exectick = GetTickCount() ;
            m_nRetCode = -1 ;
            ECommLog::sWriteLog( sFileName.c_str() , "call hllapi exception , enter :\t%d\t , leave:\t%d\t , prev idle time :\t%u\t, spend time :\t%d\t,FuncCode:\t%d\t,RetCode:\t%04d " , tick , exectick , idletick , exectick-tick , m_nFuncCode , m_nRetCode ) ;
		}

        if ( bDebug ) {
            DWORD exectick = GetTickCount() ;
            ECommLog::sWriteLog( sFileName.c_str() , "call hllapi , enter :\t%d\t , leave:\t%d\t , prev idle time :\t%u\t, spend time :\t%d\t,FuncCode:\t%d\t,RetCode:\t%04d " , tick , exectick , idletick , exectick-tick , m_nFuncCode , m_nRetCode ) ;
            prevtick = GetTickCount();
        }
        return m_nRetCode ;
	}

	// CONNECT_PS
	bool EhllapiImpl::connect()
	{
		m_nFuncCode = CONNECT_PS;
		//m_szStr[0] = m_SessionId;
		//m_szStr[1] = 0;
        strcpy( m_szStr , m_SessionStr ) ;
        m_nLength = 2 ;
		int ret = call_api();
		return (m_nRetCode == SUCCESS) ;
	}

	// connect
	bool EhllapiImpl::Connect()
	{
		bool bOk = connect();
		SetParams("NWAIT");
		SetParams("SRCHFROM");
        //SetParams("NORESET");
        return bOk ;
	}

	bool EhllapiImpl::Connect(char *id)
	{
		// TODO : //SessionId(id);
        strcpy( m_SessionStr , id ) ;
		return Connect();
	}

	// DISCONNECT_PS
	void EhllapiImpl::Disconnect()
	{
		m_nFuncCode = DISCONNECT_PS;
		int ret = call_api();
		m_IsConnect = false;
	}

	// SET_SESSION_PARMS
	void EhllapiImpl::SetParams(const char *param)
	{
		m_nFuncCode = SET_SESSION_PARMS;
		strcpy((char*)m_szStr, param);
		m_nLength = strlen((const char*)m_szStr);
		int ret = call_api();
		//if (m_nRetCode != SUCCESS)
		//	throw EhllapiError(m_nFuncCode, m_nRetCode, "EhllapiImpl::SetParams()");
	}

	// CONVERT ROW&COLUMN TO POSITION VALUE
	int EhllapiImpl::ConvertRowColToPos(int row, int col)
	{
		if (((row >= 1) && (row <= 24)) && ((col >= 1) && (col <= 80)))
			return ((row - 1) * 80 + col);
		//throw EhllapiError(CONVERT_POS_ROW_COL, 9999, "EhllapiImpl::ConvertColToPos()");
	}

	// SET_CURSOR
	void EhllapiImpl::SetCursorPos(int pos)
	{
		m_nFuncCode = SET_CURSOR;
		m_nRetCode = pos;
		int ret = call_api();
		//if (m_nRetCode != SUCCESS)
			//throw EhllapiError(m_nFuncCode, m_nRetCode, "EhllapiImpl::SetCursorPos()");
	}

	// SET_CURSOR
	void EhllapiImpl::SetCursorPos(int row, int col)
	{
		SetCursorPos(ConvertRowColToPos(row, col));
	}

	// QUERY_CURSOR_LOC
	int EhllapiImpl::GetCursorPos()
	{
		m_nFuncCode = QUERY_CURSOR_LOC;
		int ret = call_api();
		if (m_nRetCode != SUCCESS)
		{
			ret = -1;
			//throw EhllapiError(m_nFuncCode, m_nRetCode, "EhllapiImpl::GetCursorPos()");
		}
		else
			ret = m_nLength;
		return (ret);
	}

	// SENDKEY
	void EhllapiImpl::SendKeys(const char *lpstr)
	{
		m_nFuncCode = SENDKEY;
		strcpy((char*)m_szStr, lpstr);
		m_nLength = strlen((const char*)m_szStr);
		int ret = call_api();
		//if (m_nRetCode != SUCCESS)
		//	throw EhllapiError(m_nFuncCode, m_nRetCode, "EhllapiImpl::SendKeys()");
	}

	// SENDKEY
	void EhllapiImpl::SendKeys(const char *lpstr, int pos)
	{
		SetCursorPos(pos);
		SendKeys(lpstr);
	}

	// SENDKEY
	void EhllapiImpl::SendKeys(const char *lpstr, int row, int col)
	{
		SetCursorPos(ConvertRowColToPos(row, col));
		SendKeys(lpstr);
	}

	// COPY_STR_TO_PS
	void EhllapiImpl::SetString(const string& str, int row, int col)
	{
		m_nFuncCode = COPY_STR_TO_PS;
		strcpy((char*)m_szStr, str.c_str());
		m_nLength = strlen((const char*)m_szStr);
		m_nRetCode = ConvertRowColToPos(row, col);
		if (m_nLength > 0)
		{
			int ret = call_api();
			//if (m_nRetCode != SUCCESS)
				//throw EhllapiError(m_nFuncCode, m_nRetCode, "EhllapiImpl::String()");
		}
	}
	
	// COPY_STR_TO_PS
	void EhllapiImpl::SetString(const char *lpstr, int pos)
	{
		m_nFuncCode = COPY_STR_TO_PS;
		strcpy((char*)m_szStr, lpstr);
		m_nLength = strlen((const char*)m_szStr);
		m_nRetCode = pos;
		if (m_nLength > 0)
		{
			int ret = call_api();
			if (m_nRetCode != SUCCESS)
			{
				//throw EhllapiError(m_nFuncCode, m_nRetCode, "EhllapiImpl::SetString()");
			}
		}
	}

	// COPY_STR_TO_PS
	void EhllapiImpl::SetString(const char *lpstr, int row, int col)
	{
		SetString(lpstr, ConvertRowColToPos(row, col));
	}

	// COPY_PS_TO_STR
	void EhllapiImpl::GetString(char *lpstr, int length, int pos)
	{
		m_nFuncCode = COPY_PS_TO_STR;
		m_nLength = length;
		m_nRetCode = pos;
		int ret = call_api();
		if ((m_nRetCode == SUCCESS) || (m_nRetCode == BUSY) || (m_nRetCode == LOCKED)) {
			m_szStr[length] = 0;
			strcpy(lpstr, m_szStr);
		}
		//else
			//throw EhllapiError(m_nFuncCode, m_nRetCode, "EhllapiImpl::GetString()");
	}

	// COPY_PS_TO_STR
	void EhllapiImpl::GetString(char *lpstr, int length, int row, int col)
	{
		GetString(lpstr, length, ConvertRowColToPos(row, col));
	}

	// COPY_PS_TO_STR
	void EhllapiImpl::GetString(char *lpstr, int length)
	{
		GetString(lpstr, length, GetCursorPos());
	}

	// COPY_PS
	int EhllapiImpl::CopyPS()
	{
		m_nFuncCode = COPY_PS;
		memset(m_szStr, 0, sizeof(m_szStr));
		m_nLength = sizeof(m_szStr);
		int ret = call_api();
        return ret ;
		//if (m_nRetCode != SUCCESS)
		//	throw EhllapiError(m_nFuncCode, m_nRetCode, "EhllapiImpl::CopyPS()");
	}

	// SEARCH_PS
	int EhllapiImpl::SearchString(const char *lpstr, int pos)
	{
		m_nFuncCode = SEARCH_PS;
		strcpy((char*)m_szStr, lpstr);
		m_nLength = strlen((const char*)m_szStr);
		m_nRetCode = pos;
		int ret = call_api();
		if (m_nRetCode == SUCCESS)
			return (m_nLength);
		else if ((m_nRetCode == STR_NOT_FOUND_UNFM_PS) ||
					(m_nRetCode == BUSY) ||
					(m_nRetCode == LOCKED))
			return (0);
		//else
			//throw EhllapiError(m_nFuncCode, m_nRetCode, "EhllapiImpl::SearchString()");
	}

	// SEARCH_PS
	int EhllapiImpl::SearchString(const char *lpstr, int row, int col)
	{
		return (SearchString(lpstr, ConvertRowColToPos(row, col)));
	}

	// SEARCH_PS
	bool EhllapiImpl::MatchString(const char *lpstr, int pos)
	{
		if (pos == SearchString(lpstr, pos))
			return true;
		return false;
	}

	// SEARCH_PS
	bool EhllapiImpl::MatchString(const char *lpstr, int row, int col)
	{
		int pos = ConvertRowColToPos(row, col);
		return (MatchString(lpstr, pos));
	}

	// WAIT
	int EhllapiImpl::WaitForInput()
	{
		m_nFuncCode = WAIT;
		int ret = call_api();
        return m_nRetCode ;
        /*
		if ((m_nRetCode == BUSY) || (m_nRetCode == LOCKED))
			return false;
		if (m_nRetCode == SUCCESS)
			return true;  */
		//throw EhllapiError(m_nFuncCode, m_nRetCode, "EhllapiImpl::WaitForInput()");
	}

	bool EhllapiImpl::IsConnect()
	{
		return (m_IsConnect);
	}

	// COPY_OIA
	int EhllapiImpl::CopyOIA()
	{
		m_nFuncCode = COPY_OIA;
		memset(m_szStr, 0, sizeof(m_szStr));
		//memset(m_szOIA, 0, sizeof(m_szOIA));
		m_nLength = 103; // sizeof(m_szStr);
		int ret = call_api();
        
		if ((m_nRetCode == SUCCESS) ||
			(m_nRetCode == BUSY) ||
			(m_nRetCode == LOCKED))
		{
			memcpy(m_szOIA, m_szStr, 103);
			m_szOIA[103] = 0;
			if (strncmp(m_szOIA + 36, "COMM", 4) == 0)
				m_nCommError = true;
			else
				m_nCommError = false;
		}
		//else
			//throw EhllapiError(m_nFuncCode, m_nRetCode, "EhllapiImpl::CopyOIA()");
        return m_nRetCode ;
	}

	// QUERY_SYSTEM
	void EhllapiImpl::QuerySystem(void)
	{
		m_nFuncCode = QUERY_SYSTEM;
		memset(m_szStr, 0, sizeof(m_szStr));
		int ret = call_api();
		if (ret != 0)
			DevproDebugString("CGSAPI:QUERY_SYSYEM FAIL");
	}

	// START_HOST_NOTIFY
	void EhllapiImpl::StartHostNotify(char param)
	{
		m_nFuncCode = START_HOST_NOTIFY;
		m_szStr[0] = m_SessionId;
		m_szStr[1] = param;
		m_szStr[2] = 0;
		int ret = call_api();
		//if (m_nRetCode != SUCCESS)
			//throw EhllapiError(m_nFuncCode, m_nRetCode, "EhllapiImpl::StartHostNotify()");
	}

	// QUERY_HOST_UPDATE
	int EhllapiImpl::QueryHostUpdate(void)
	{
		m_nFuncCode = QUERY_HOST_UPDATE;
		m_szStr[0] = m_SessionId;
		m_szStr[1] = 0;
		int ret = call_api();
		return m_nRetCode;
	}

	// STOP_HOST_NOTIFY
	void EhllapiImpl::StopHostNotify(void)
	{
		m_nFuncCode = STOP_HOST_NOTIFY;
		m_szStr[0] = m_SessionId;
		m_szStr[1] = 0;
		int ret = call_api();
		//if (m_nRetCode != SUCCESS)
			//throw EhllapiError(m_nFuncCode, m_nRetCode, "EhllapiImpl::StopHostNotify()");
	}

	// QUERY_SESSION_STATUS
	void EhllapiImpl::QuerySessionStatus(void)
	{
		m_nFuncCode = QUERY_SESSION_STATUS;
		memset(m_szStr, 0, sizeof(m_szStr));
		m_szStr[0] = m_SessionId;
		m_szStr[1] = 0;
		m_nLength = 64;
		int ret = call_api();
		//if (m_nRetCode != SUCCESS)
			//throw EhllapiError(m_nFuncCode, m_nRetCode, "EhllapiImpl::QuerySessionStatus()");
	}

	// QUERY_SESSIONS
	void EhllapiImpl::QuerySessions(void)
	{
		m_nFuncCode = QUERY_SESSIONS;
		memset(m_szStr, 0, sizeof(m_szStr));
		m_nLength = 256;
		int ret = call_api();
		//if (m_nRetCode != SUCCESS)
			//throw EhllapiError(m_nFuncCode, m_nRetCode, "EhllapiImpl::QuerySessions()");
	}

	// RESERVE
	void EhllapiImpl::LockKeyboard(void)
	{
		m_nFuncCode = RESERVE;
		int ret = call_api();
		//if (m_nRetCode != SUCCESS)
			//throw EhllapiError(m_nFuncCode, m_nRetCode, "EhllapiImpl::LockKeyboard()");
	}

	// RELEASE
	void EhllapiImpl::UnlockKeyboard(void)
	{
		m_nFuncCode = RELEASE;
		int ret = call_api();
		//if (m_nRetCode != SUCCESS)
			//throw EhllapiError(m_nFuncCode, m_nRetCode, "EhllapiImpl::UnlockKeyboard()");
	}

	// QUERY_FIELD_ATTR
	int EhllapiImpl::FieldAttribute(int row, int col)
	{
		m_nFuncCode = QUERY_FIELD_ATTR;
		m_nRetCode = ConvertRowColToPos(row, col);
		int ret = call_api();
		if (m_nRetCode == SUCCESS)
			return m_nLength;
		//else
			//throw EhllapiError(m_nFuncCode, m_nRetCode, "EhllapiImpl::FieldAttribute()");
	}

}	// namespace HACL

