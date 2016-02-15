// ARPrintCmdLine.h: interface for the CARPrintCmdLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARPRINTCMDLINE_H__5A4E1721_FA23_434C_B942_706A0A7F4780__INCLUDED_)
#define AFX_ARPRINTCMDLINE_H__5A4E1721_FA23_434C_B942_706A0A7F4780__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CARPrintCmdLine : public CCommandLineInfo  
{
public:
	// le constructeur
	CARPrintCmdLine();

	// la fonction pour parser la ligne de commande
	virtual void ParseParam( LPCTSTR lpszParam, BOOL bFlag, BOOL bLast );

	// le destructeur
	virtual ~CARPrintCmdLine();

	// l'orientation de la page
	long m_Orientation;

	// le handle du lanceur
	long m_HLanceur;

	// le numéro du paramètre
	long m_NoParam;
};

#endif // !defined(AFX_ARPRINTCMDLINE_H__5A4E1721_FA23_434C_B942_706A0A7F4780__INCLUDED_)
