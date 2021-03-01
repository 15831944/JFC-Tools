// on inclut les définitions nécessaires
#include "stdafx.h"
#include "JFCScheme.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

////////////////////////////////////////////////
// la fonction pour récupérer le nom de la fonte

const TCHAR* JFCScheme::GetFontName() { return (_T("MS Shell Dlg")); }

///////////////////////////////////////////////////////
// les fonctions pour récupérer les tailles de la fonte

LONG JFCScheme::GetFontSizeS() { return ( 80); }
LONG JFCScheme::GetFontSizeM() { return ( 90); }
LONG JFCScheme::GetFontSizeL() { return (100); }
LONG JFCScheme::GetFontSizeX() { return (110); }

/////////////////////////////////////////////////////////////////////
// les fonctions pour récupérer les couleurs systèmes des bordures 3D

COLORREF JFCScheme::Get3DSystemF() { return (::GetSysColor(COLOR_3DFACE)); }
COLORREF JFCScheme::Get3DSystemS() { return (::GetSysColor(COLOR_3DSHADOW)); }
COLORREF JFCScheme::Get3DSystemD() { return (::GetSysColor(COLOR_3DDKSHADOW)); }
COLORREF JFCScheme::Get3DSystemH() { return (::GetSysColor(COLOR_3DHILIGHT)); }

//////////////////////////////////////////////////////////////////
// les fonctions pour récupérer les couleurs des composants Chorus

#if(0)
COLORREF JFCScheme::GetChorusA() { return (RGB(202, 86, 12)); } // Orange
COLORREF JFCScheme::GetChorusB() { return (RGB(248,150, 86)); }
COLORREF JFCScheme::GetChorusC() { return (RGB(240,184,124)); }
COLORREF JFCScheme::GetChorusD() { return (RGB(250,234,208)); }
#endif

COLORREF JFCScheme::GetChorusA() { return (RGB(124, 96,242)); } // Bleu
COLORREF JFCScheme::GetChorusB() { return (RGB(160,176,250)); }
COLORREF JFCScheme::GetChorusC() { return (RGB(198,196,246)); }
COLORREF JFCScheme::GetChorusD() { return (RGB(222,234,252)); }

/////////////////////////////////////////////////////////////
// les fonctions pour récupérer les niveaux de gris standards

COLORREF JFCScheme::GetBlack() { return (RGB(  0,  0,  0)); }
COLORREF JFCScheme::GetGreyD() { return (RGB( 96, 96, 96)); }
COLORREF JFCScheme::GetGreyM() { return (RGB(128,128,128)); }
COLORREF JFCScheme::GetGreyL() { return (RGB(192,192,192)); }
COLORREF JFCScheme::GetWhite() { return (RGB(255,255,255)); }
