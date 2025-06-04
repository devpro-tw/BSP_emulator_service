// ECLEmulator.cpp: implementation of the ECLEmulator class.
//
//////////////////////////////////////////////////////////////////////

#include "ECLEmulator.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace std;
namespace HACL	// Host Access Class Library
{
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
	ECLEmulator::ECLEmulator() : m_hWKS(NULL)
	{

	}

	ECLEmulator::~ECLEmulator()
	{

	}

	BOOL ECLEmulator::Create(const char *lpCfgName)
	{
		char szCmdLine[512];
		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		m_CfgName = lpCfgName;
		m_CmdLine = m_ApplName;
		m_CmdLine += " ";
		m_CmdLine += lpCfgName;

		strcpy(szCmdLine, m_CmdLine.c_str());

		m_hWKS = ::FindWindow(m_ClassName.c_str(), lpCfgName);
		if (m_hWKS)
			return TRUE;

		GetStartupInfo(&si);
		if (!CreateProcess(NULL,
						szCmdLine,
						NULL,
						NULL,
						FALSE,
						DETACHED_PROCESS,
						NULL,
						m_ExecDir.c_str(),
						&si,
						&pi))
			return FALSE;
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		while ((m_hWKS = ::FindWindow(m_ClassName.c_str(), lpCfgName)) == NULL)
			Sleep(500);
		return TRUE;
	}

	BOOL ECLEmulator::Close()
	{
		if (!m_hWKS)
		{
			m_hWKS = ::FindWindow(m_ClassName.c_str(), m_CfgName.c_str());
			if (!m_hWKS)
				return FALSE;
		}
		if (!PostMessage(m_hWKS, WM_SETFOCUS, NULL, NULL))
			return TRUE;
		return (PostMessage(m_hWKS, WM_COMMAND, 0x0000E141, 0x000104DC));
	}

	BOOL ECLEmulator::Connect()
	{
		if (!PostMessage(m_hWKS, WM_SETFOCUS, NULL, NULL))
			return FALSE;
		return (::PostMessage(m_hWKS, WM_COMMAND, 0x0000846D, 0x0L));
	}

	BOOL ECLEmulator::Disconnect()
	{
		if (!PostMessage(m_hWKS, WM_SETFOCUS, NULL, NULL))
			return FALSE; 
		return (::PostMessage(m_hWKS, WM_COMMAND, 0x0000846E, 0x0L));
	}

	BOOL ECLEmulator::FindWindow()
	{
		HWND h;
		h = ::FindWindow(m_ClassName.c_str(), m_CfgName.c_str());
		if (h) {
			m_hWKS = h;
			return TRUE;
		}
		return FALSE;
	}
}	// namespace HACL