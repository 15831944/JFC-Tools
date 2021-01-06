// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H____INCLUDED_)
#define AFX_STDAFX_H____INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef WINVER				// Allow use of features specific to Windows 95 and Windows NT 4 or later.
#define WINVER 0x0400		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows NT 4 or later.
#define _WIN32_WINNT 0x0400	// Change this to the appropriate value to target Windows 2000 or later.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 4.0 or later.
#define _WIN32_IE 0x0400	// Change this to the appropriate value to target IE 5.0 or later.
#endif

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

// désactive le masquage MFC de certains messages d'avertissement courants et par ailleurs souvent ignorés
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#include <afxtempl.h>       // MFC templates
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

//Utilisation du GDI+
#include <gdiplus.h>
using namespace Gdiplus;

#include "resource.h"

#include "JLib.h"

#include "JFCApp.h"
#include "JFCFrame.h"
#include "JFCSplash.h"
#include "JFCDialog.h"
#include "JFCPane.h"
#include "JFCSplitterPane.h"
#include "SplitterWndEx.h"

#include "JFCUpdate.h"

#include "JFControl.h"
#include "JFCViewport.h"
#include "JFCViewportSelect.h"
#include "JFCViewportSelectEx.h"

#include "JFCScroll.h"
#include "JFCHorzSplit.h"
#include "JFCVertSplit.h"

#include "JFCTitle.h"
#include "JFCStatic.h"
#include "JFCButton.h"
#include "JFCListBoxS.h"
#include "JFCListBoxM.h"
#include "JFCCalendar.h"
#include "JFCWave.h"
#include "JFCGrid.h"

// Everything from Supports
#include "AllTypeOfNode.h"

// Everything from Items
#include "ItemsConstruits.h"
#include "ItemsConstruitsManager.h"

// Everything for the Tables
#include "JSrcPresse.h"
#include "JSRCTables.h"

// Everything for the Tarifs
#include "Tarifs.h"
#include "KeyTarifManuel.h"

// Everything  for Source
#include "Terrain.h"

// Colors for all the apps
#include "ATPColors.h"
#include "FormatStr.h"

// Pour les messages
#include "ATPMessages.h"

//Le Moteur
#include <afx.h>

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H____INCLUDED_)
