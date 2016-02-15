// JFCExport.h: interface for the CJFCExport class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JFCEXPORT_H__BF365AA0_4388_11D3_A1B9_0050048B3CF9__INCLUDED_)
#define AFX_JFCEXPORT_H__BF365AA0_4388_11D3_A1B9_0050048B3CF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/*
JFCEXPORT VERSION 2
REECRITURE du protocole le 14 Février 2001
par REMI EQUOY

                 *** NOTICE ***

Il reste à re-écrire les fonctions qui risquent des plantages
de pointeurs puisqu'elles travaillent sur des LPCSTR
alors qu'il serait su simple d'utiliser des CStrings !

Toutes les fonctions SetTYPE() ont été remplacées par Write()

Il est possible de déterminer globalement le type de séparateur
et la précision des réels

Il est possible d'envoyer l'export dans le presse-papiers

Il est possible d'utiliser l'opérateur <<

Pour plus de lisibilité, il est possible de sauter des lignes
sans utiliser la fc WriteCRLF():

export << "coucou" << '\n'

Dans ce cas SPECIFIQUE et EXCLUSIF le caractère \n déclenche un saut de ligne
*/


#include <time.h>
#include <afxadv.h>            // For CSharedFile

class CJFCExport
{
#define JFCEX_TEXT			1

public:
	bool OpenJFCExport(LPCSTR NomFic,char separator);
	CJFCExport();
	virtual ~CJFCExport();

	// Choix du fichier pour un export vers un fichier
	// ATTENTION: Ne pas utiliser ces fonctions pour un export dans le presse-papiers
	virtual int DoJFCExport(void);
	virtual int DoJFCExport(int Type,			//Type de l'export (text, html, xls, rtf, ...)
							LPCSTR File,		//Fichier de destination
							bool Erase=true);	//Supprime (true) ou ajoute (false) les informations au fichier
	// ----------------------------------------------------------------

	// Ouverture du fichier d'export ou du presse-papiers
	bool OpenJFCExport(bool flagPressePapiers=0);
	// ----------------------------------------------------------------

	// Définition du séparateur (par défaut: tabulation)
	void SetSeparator(TCHAR separator){m_Separator=separator;}
	
	// Définition de la précision des réels (par défaut 2 décimales)
	void SetPrecision(TCHAR precision){m_Precision=precision;}
	// ----------------------------------------------------------------
	
	// Fermeture de l'export (automatique dans le destructeur)
	bool CloseJFCExport(void);
	// ----------------------------------------------------------------

	
	// Exports explicite
	bool Write(LPCTSTR String);
	bool Write(const TCHAR Caractere);
	bool Write(const int Number,LPCTSTR Type=NULL);
	bool Write(const long Number,LPCTSTR Type=NULL);
	bool Write(const DWORD Number,LPCTSTR Type=NULL){return(Write((long) Number,Type));};
	bool Write(const WORD Number,LPCTSTR Type=NULL){return(Write((long) Number,Type));};
	bool Write(const double Number, const short Precision=-1,LPCTSTR Type=NULL);
	bool Write(const float Number, const short Precision=-1,LPCTSTR Type=NULL){return(Write((double) Number,Precision,Type));};
	bool Write(const struct tm * Time);
	bool Write(const COleDateTime * Time);
	// ----------------------------------------------------------------

	// Ajout de n séparateur
	bool WriteSeparator(const short nbSeparator=1);

	// Ajout de n sout de ligne
	bool WriteCRLF(const short nbLineFeed=1);
	// ----------------------------------------------------------------

	// L'opérateur <<
	CJFCExport & operator <<(LPCTSTR String){Write(String);return *this;};
	CJFCExport & operator <<(const TCHAR Caractere){Write(Caractere);return *this;};
	CJFCExport & operator <<(const int i){Write(i);return *this;};
	CJFCExport & operator <<(const long i){Write(i);return *this;};
	CJFCExport & operator <<(const DWORD i){Write(i);return *this;};
	CJFCExport & operator <<(const WORD i){Write(i);return *this;};
	CJFCExport & operator <<(const double i){Write(i);return *this;};
	CJFCExport & operator <<(const float i){Write(i);return *this;};
	CJFCExport & operator <<(const struct tm * Time){Write(Time);return *this;};
	CJFCExport & operator <<(const COleDateTime * Time){Write(Time);return *this;};
	// ----------------------------------------------------------------

private:
	bool m_ExportPressePapiers;
	LPCTSTR SetType(LPCTSTR String, LPCTSTR Type);
	short GetStrSize(LPCTSTR String);
	int m_Type;								//Type de l'export (text, html, xls, rtf, ...)	
	CString m_FileName;							//Fichier de destination
	CFile * m_pFile;
	TCHAR m_Separator;
	short m_Precision;
};

#endif // !defined(AFX_JFCEXPORT_H__BF365AA0_4388_11D3_A1B9_0050048B3CF9__INCLUDED_)
