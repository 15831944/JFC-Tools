// stdafx.h : Fichier Include pour les fichiers Include syst�me standard,
// ou les fichiers Include sp�cifiques aux projets qui sont utilis�s fr�quemment,
// et sont rarement modifi�s

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclure les en-t�tes Windows rarement utilis�s
#endif

// Modifiez les d�finitions suivantes si vous devez cibler une plate-forme avant celles sp�cifi�es ci-dessous.
// Reportez-vous � MSDN pour obtenir les derni�res informations sur les valeurs correspondantes pour les diff�rentes plates-formes.
#ifndef WINVER				// Autorise l'utilisation des fonctionnalit�s sp�cifiques � Windows�95 et Windows NT�4 ou version ult�rieure.
#define WINVER 0x0400		// Attribuez la valeur appropri�e � cet �l�ment pour cibler Windows�98 et Windows�2000 ou version ult�rieure.
#endif

#ifndef _WIN32_WINNT		// Autorise l'utilisation des fonctionnalit�s sp�cifiques � Windows NT�4 ou version ult�rieure.
#define _WIN32_WINNT 0x0400		// Attribuez la valeur appropri�e � cet �l�ment pour cibler Windows�98 et Windows�2000 ou version ult�rieure.
#endif						

#ifndef _WIN32_WINDOWS		// Autorise l'utilisation des fonctionnalit�s sp�cifiques � Windows�98 ou version ult�rieure.
#define _WIN32_WINDOWS 0x0410 // Attribuez la valeur appropri�e � cet �l�ment pour cibler Windows�Me ou version ult�rieure.
#endif

#ifndef _WIN32_IE			// Autorise l'utilisation des fonctionnalit�s sp�cifiques � IE�4.0 ou version ult�rieure.
#define _WIN32_IE 0x0400	// Attribuez la valeur appropri�e � cet �l�ment pour cibler IE�5.0 ou version ult�rieure.
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// certains constructeurs CString seront explicites

// d�sactive le masquage MFC de certains messages d'avertissement courants et par ailleurs souvent ignor�s
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // composants MFC principaux et standard
#include <afxext.h>         // extensions MFC
#include <afxdisp.h>        // Classes MFC Automation

#include <afxdtctl.h>		// Prise en charge des MFC pour les contr�les communs Internet Explorer 4
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// Prise en charge des MFC pour les contr�les communs Windows
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxtempl.h>       // MFC templates


#include "JLib.h"

//#include "JFCApp.h"
//#include "JFCFrame.h"
#include "JFCSplash.h"
#include "JFCDialog.h"
//#include "JFCPane.h"
//#include "JFCSplitterPane.h"
//#include "SplitterWndEx.h"

#include "JFCUpdate.h"
//#include "JFCDocument.h"

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
//#include "JFCWave.h"
#include "JFCGrid.h"


// Everything from Items
#include "ItemsConstruits.h"

// Everything for the Tables
//#include "JSrcPresse.h"

// Everything  for Source
#include "Terrain.h"

// Colors for all the apps
#include "ATPColors.h"
#include "FormatStr.h"


#include <afx.h>

// Objective Grid
#include "grid\gxall.h"

