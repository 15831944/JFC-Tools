// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__5ACA002B_8B80_4A3A_B368_DDE9DFE3A8DB__INCLUDED_)
#define AFX_STDAFX_H__5ACA002B_8B80_4A3A_B368_DDE9DFE3A8DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#include <direct.h>
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls


#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxtempl.h>

// composant JFC
#include "obj_lbox.h"
#include "obj_gray.h"

// Définition des couleurs
#define RGB_BleuClairJFC RGB(176,248,255)
#define RGB_BleuSpeJFC	 RGB (244,242,242)
#define RGB_BleuJFC      RGB(0,92,141)

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__5ACA002B_8B80_4A3A_B368_DDE9DFE3A8DB__INCLUDED_)
