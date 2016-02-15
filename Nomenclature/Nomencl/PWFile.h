// PWFile.h: interface for the CPWFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PWFILE_H__68928160_BD0E_11D3_8718_005004F93C23__INCLUDED_)
#define AFX_PWFILE_H__68928160_BD0E_11D3_8718_005004F93C23__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPWFile : public CStdioFile   // avant private CStdioFile  
{
public:
	// le constructeur
	CPWFile();

	// la fonction pour ouvrir le fichier
	BOOL OpenFile( LPCSTR nom );

	// la fonction pour lire la ligne suivante du ficher
	BOOL ReadString( CString *chaine );

	// la fonction pour decoder le nombre d'elements du fichier
	BOOL GetNbItems( CString *chaine, INT *nombre, TCHAR separateur = '#' );

	// la fonction pour decomposer une ligne du fichier
	BOOL GetStringItems( CString *chaine, CStringArray *elements, TCHAR separateur = ',' );

	// la fonction pour fermer le fichier
	VOID CloseFile();

	// le destructeur
	virtual ~CPWFile();
};

#endif // !defined(AFX_PWFILE_H__68928160_BD0E_11D3_8718_005004F93C23__INCLUDED_)
