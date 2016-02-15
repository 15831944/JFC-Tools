// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__4757152C_6F92_4906_9DFA_834D8D26A15A__INCLUDED_)
#define AFX_STDAFX_H__4757152C_6F92_4906_9DFA_834D8D26A15A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "locale.h"
//#include "filesys.h"
#include "PWFile.h"
#include "Noeud.h"
#include "TreeCtrlX.h"
// #include "MoveManager.h"
#include "Obj_Printer.h"
#include "Obj_Gray.h"
#include "Time.h"

#define RGB_OrangeFormulaire RGB(255,172,89)
#define RGB_MarronCacadoie RGB(131,102,5)
#define RGB_VertEmeraude RGB(147,195,147)
#define RGB_BleuJFC RGB(0,92,141)


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__4757152C_6F92_4906_9DFA_834D8D26A15A__INCLUDED_)
