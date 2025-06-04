#pragma once

#ifndef __winboj_base_hpp
#define __winboj_base_hpp

#pragma comment(compiler)

#ifndef __cplusplus
#error require C++ 
#endif

#ifndef _M_IX86
#error Intel X86 is required
#endif

#ifndef _WIN32_WINNT
#error Windows 2000 is required
#elif _WIN32_WINNT < 0x0500
#error Windows 2000 is required
#endif

#include <tchar.h>
#include <stdio.h>

#define dbgLINE2(x) #x
#define dbgLINE(x) dbgLINE2(x)
#define dbgMsg(desc) message(__FILE "(" dbgLINE( __LINE__ ) "):" #desc)

inline void dbgMB(LPCTSTR s) {
	_TCHAR t[256];
	GetModuleFileName(0, t, sizeof(t)/sizeof(_TCHAR));
	HWND h = GetActiveWindow();
	MessageBox(h, s, t, MB_OK | ((h == 0) ? MB_SERVICE_NOTIFICATION  : 0));
}

inline void dbgMBBreak(LPCTSTR s) {
	dbgMB(s);
	#ifndef __BORLANDC__
    _asm { int 3 }
    #endif
}

inline void dbgASSERTFAIL(LPCTSTR f, int l, LPCTSTR s) {
	_TCHAR t[256];
	_stprintf(t, "File %s, line %d : %s", f, l, s);
	dbgMBBreak(t);
}

#ifdef _DEBUG
#define dbgASSERT(x) if (!(x)) dbgASSERTFAIL(__FILE__,__LINE__, #x)
#define dbgFAIL() dbgASSERTFAIL(__FILE__,__LINE__, "")
#define dbgVERIFY(x) dbgASSERT(x)
#else
#define dbgASSERT(x)
#define dbgFAIL()
#define dbgVERIFY(x) (x)
#endif

inline void win9xNotSupported() {
	OSVERSIONINFO vi = { sizeof(vi) };
	GetVersionEx(&vi);
	if (vi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) {
		dbgMB("Window NT or 2000 required.");
		ExitProcess(0);
	}
}

inline void win2000Required() {
	OSVERSIONINFO vi = { sizeof(vi) };
   GetVersionEx(&vi);
   if ((vi.dwPlatformId != VER_PLATFORM_WIN32_NT) && (vi.dwMajorVersion < 5)) {
      dbgMB("This application requires Windows 2000.");
      ExitProcess(0);
	}
}

#endif
