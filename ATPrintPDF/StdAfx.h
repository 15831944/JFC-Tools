// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__33409515_429C_490C_A314_EDBD62CDFA50__INCLUDED_)
#define AFX_STDAFX_H__33409515_429C_490C_A314_EDBD62CDFA50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

// les macro pour les mises à jour de la vue
#define UPDATE_PRINT_CURRENT_PAGE	1
#define UPDATE_PRINT_ALL_PAGE		2
#define UPDATE_FIRST_PAGE		3
#define UPDATE_PREV_PAGE		4
#define UPDATE_PAGE_CHANGED		5
#define UPDATE_NEXT_PAGE		6
#define UPDATE_LAST_PAGE		7
#define UPDATE_ZOOM_OUT			8
#define UPDATE_ZOOM_IN			9
#define UPDATE_ZOOM_CHANGED		10
#define UPDATE_PDF_CHANGED		11
#define UPDATE_CONTRASTE_CHANGED	12
#define UPDATE_DOC_CHANGED 		13

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__33409515_429C_490C_A314_EDBD62CDFA50__INCLUDED_)
