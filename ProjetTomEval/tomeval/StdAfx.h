// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//



#if !defined(AFX_STDAFX_H__E843E169_9722_11D1_A42E_004005327F70__INCLUDED_)
#define AFX_STDAFX_H__E843E169_9722_11D1_A42E_004005327F70__INCLUDED_


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers



#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

//#define  _CRTDBG_MAP_ALLOC

//#include <afxdisp.h>        // MFC OLE automation classes
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT



#include <afxtempl.h>
#include "tomprot.h"		// prototypes structures et classes
#include "stdfonc.h"		// fonctions standards JFC
#ifdef OPTION_JFC
#include "tom_msg.h"		// Messages pour dialoguer avec TOM
#endif
#ifdef OPTION_CHECKUP
#include "tom2cup.h"		// Messages pour dialoguer avec TOM
#endif
#ifdef OPTION_CARAT
#include "tomcarat.h"		// Messages pour dialoguer avec TOM
#endif
#ifdef OPTION_MEDIATOP
#include "mediatop.h"		// Messages pour dialoguer avec TOM
#endif
#ifdef OPTION_MEDIACONSULT
#include "mediaconsult.h"		// Messages pour dialoguer avec TOM
#endif
#ifdef OPTION_ZOOMRADIOTELE
#include "tom_msg.h"		// Messages pour dialoguer avec TOM
#endif
#ifdef OPTION_TOPCHRONO
#include "tom_msg.h"		// Messages pour dialoguer avec TOM
#endif


//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__E843E169_9722_11D1_A42E_004005327F70__INCLUDED_)
