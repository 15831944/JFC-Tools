// stdafx.h : Fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets qui sont utilisés fréquemment,
// et sont rarement modifiés

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclure les en-têtes Windows rarement utilisés
#endif

// Modifiez les définitions suivantes si vous devez cibler une plate-forme avant celles spécifiées ci-dessous.
// Reportez-vous à MSDN pour obtenir les dernières informations sur les valeurs correspondantes pour les différentes plates-formes.
#ifndef WINVER				// Autorise l'utilisation des fonctionnalités spécifiques à Windows 95 et Windows NT 4 ou version ultérieure.
#define WINVER 0x0400		// Attribuez la valeur appropriée à cet élément pour cibler Windows 98 et Windows 2000 ou version ultérieure.
#endif

#ifndef _WIN32_WINNT		// Autorise l'utilisation des fonctionnalités spécifiques à Windows NT 4 ou version ultérieure.
#define _WIN32_WINNT 0x0400		// Attribuez la valeur appropriée à cet élément pour cibler Windows 98 et Windows 2000 ou version ultérieure.
#endif						

#ifndef _WIN32_WINDOWS		// Autorise l'utilisation des fonctionnalités spécifiques à Windows 98 ou version ultérieure.
#define _WIN32_WINDOWS 0x0410 // Attribuez la valeur appropriée à cet élément pour cibler Windows Me ou version ultérieure.
#endif

#ifndef _WIN32_IE			// Autorise l'utilisation des fonctionnalités spécifiques à IE 4.0 ou version ultérieure.
#define _WIN32_IE 0x0400	// Attribuez la valeur appropriée à cet élément pour cibler IE 5.0 ou version ultérieure.
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// certains constructeurs CString seront explicites

// désactive le masquage MFC de certains messages d'avertissement courants et par ailleurs souvent ignorés
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // composants MFC principaux et standard
#include <afxext.h>         // extensions MFC
#include <afxdisp.h>        // Classes MFC Automation

#include <afxdtctl.h>		// Prise en charge des MFC pour les contrôles communs Internet Explorer 4
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// Prise en charge des MFC pour les contrôles communs Windows
#endif // _AFX_NO_AFXCMN_SUPPORT

// Cette macro est semblable à IMPLEMENT_OLECREATE, à la différence qu'elle passe au constructeur COleObjectFactory
// la valeur TRUE pour le paramètre bMultiInstance.
// Une instance distincte de cette application doit être démarrée pour
// chaque objet proxy Automation demandé par les contrôleurs Automation.
#ifndef IMPLEMENT_OLECREATE2
#define IMPLEMENT_OLECREATE2(class_name, external_name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
	AFX_DATADEF COleObjectFactory class_name::factory(class_name::guid, \
		RUNTIME_CLASS(class_name), TRUE, _T(external_name)); \
	const AFX_DATADEF GUID class_name::guid = \
		{ l, w1, w2, { b1, b2, b3, b4, b5, b6, b7, b8 } };
#endif // IMPLEMENT_OLECREATE2

#include "JLib.h"
#include <afxtempl.h>       // MFC templates
