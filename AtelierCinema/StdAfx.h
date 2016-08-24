// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__33A58746_0AED_11D4_989E_004095420987__INCLUDED_)
#define AFX_STDAFX_H__33A58746_0AED_11D4_989E_004095420987__INCLUDED_

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

// Composants Stingray
#include <ot_dockingwindows.h>		// Objective Toolkit Docking Windows
#include <ot_toolbar.h>				// Objective Toolkit Customizable Toolbar/Menubar

#include <gxall.h> // Grid


// Includes 
#include <afxtempl.h>
#include <math.h>

// Composants jfc
#include "obj_gray.h"
#include "obj_lbox.h"
#include "obj_btn.h"
#include "obj_printer.h"

#include "other\\gradientprogressctrl.h"

// Définition des couleurs
#define RGB_BleuClairJFC RGB(176,248,255)
#define RGB_BleuSpeJFC	 RGB (244,242,242)
//#define RGB_BleuJFC      RGB(0,92,141)
#define RGB_BleuJFC      RGB(8,121,165)





//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__33A58746_0AED_11D4_989E_004095420987__INCLUDED_)
