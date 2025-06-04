// EhllapiError.h: interface for the EhllapiError class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __EHLLAPIERROR_H__
#define __EHLLAPIERROR_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <exception>
#include <string>
#include <stdio.h>
#include <vcl.h>
using namespace std;
namespace HACL	// Host Access Class Library
{
	class EhllapiError : public exception  
	{
	private:
		int m_nFuncCode;	// reference to EhllapiImpl::m_nFuncCode
		int m_nErrNumber;	// reference to EhllapiImpl::m_nRetCode
		std::string m_what;	// Error message

	private:
		void _Debug_Construct(const char* log) {
				char sztmp[128];
				sprintf(sztmp, "[%x]-%s", this, log);
				//OutputDebugString(sztmp);
			}

	public:
		explicit EhllapiError(const char *_Message = "unknown") 
			:m_what(_Message) {
				_Debug_Construct("EhllapiError()");
			}

		EhllapiError(int nFuncCode, int nErrNumber, const char *what) 
			:m_nFuncCode(nFuncCode), m_nErrNumber(nErrNumber), m_what(what) {
				_Debug_Construct("EhllapiError(int nFuncCode, int nErrNumber, const char *what)");
			}

		EhllapiError(const EhllapiError& err)
			:m_nFuncCode(err.m_nFuncCode), m_nErrNumber(err.m_nErrNumber), m_what(err.m_what) {
				_Debug_Construct("EhllapiError(const EhllapiError& err)");
			}

		EhllapiError& operator=(const EhllapiError& err) {
				m_nFuncCode = err.m_nFuncCode;
				m_nErrNumber = err.m_nErrNumber;
				m_what = err.m_what;
				_Debug_Construct("EhllapiError& operator=(const EhllapiError& err)");
				return (*this);
			}

		virtual ~EhllapiError() {
				char sztmp[32];
				sprintf(sztmp, "~EhllapiError(%x)", this);
				//OutputDebugString(sztmp);
			}

		int FuncCode(void) {
				return (m_nFuncCode);
			}

		int ErrNumber(void) {
				return (m_nErrNumber);
			}

		const std::string& GetMsg(void) {
				return (m_what);
			}

		const char* what() const {
			return (m_what.c_str());
		}
	};
}	// namespace HACL

#endif // __EHLLAPIERROR_H__
