// ECLEmulator.h: interface for the ECLEmulator class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __ECLEMULATOR_H__
#define __ECLEMULATOR_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <windows.h>
namespace HACL	// Host Access Class Library
{
	class ECLEmulator
	{
	private:
		HWND m_hWKS;

	public:
		std::string m_ClassName;	// ClassName
		std::string m_ApplName;	// ApplicationName
		std::string m_CmdLine;	// CommandLine
		std::string m_ExecDir;	// CurrentDirectory
		std::string m_CfgName;	// Config Name

	public:
		BOOL Disconnect();
		BOOL Connect();
		BOOL Close();
		BOOL Create(const char *lpCfgName);
		ECLEmulator();
		virtual ~ECLEmulator();
		BOOL FindWindow();
	};
}	// namespace HACL
#endif // __ECLEMULATOR_H__
