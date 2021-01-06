// JFCScheme.h: interface for the JFCScheme class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JFCSCHEME_H__DF85DA20_DBA0_4755_985C_87B0EA17A0E2__INCLUDED_)
#define AFX_JFCSCHEME_H__DF85DA20_DBA0_4755_985C_87B0EA17A0E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class JFCScheme  
{
public:
	// la fonction pour r�cup�rer le nom de la fonte
	static const TCHAR* GetFontName();

public:
	// les fonctions pour r�cup�rer les tailles de la fonte
	static LONG GetFontSizeS();
	static LONG GetFontSizeM();
	static LONG GetFontSizeL();
	static LONG GetFontSizeX();

public:
	// les fonctions pour r�cup�rer les couleurs syst�mes des bordures 3D
	static COLORREF Get3DSystemF();
	static COLORREF Get3DSystemS();
	static COLORREF Get3DSystemD();
	static COLORREF Get3DSystemH();

public:
	// les fonctions pour r�cup�rer les couleurs des composants Chorus
	static COLORREF GetChorusA();
	static COLORREF GetChorusB();
	static COLORREF GetChorusC();
	static COLORREF GetChorusD();

public:
	// les fonctions pour r�cup�rer les niveaux de gris standards
	static COLORREF GetBlack();
	static COLORREF GetGreyD();
	static COLORREF GetGreyM();
	static COLORREF GetGreyL();
	static COLORREF GetWhite();
};

#endif // !defined(AFX_JFCSCHEME_H__DF85DA20_DBA0_4755_985C_87B0EA17A0E2__INCLUDED_)
