// ListPoint.h: interface for the CListPoint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LISTPOINT_H__5BD886A5_F3B5_45D3_92CD_87BAB0D9921B__INCLUDED_)
#define AFX_LISTPOINT_H__5BD886A5_F3B5_45D3_92CD_87BAB0D9921B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "JFCArray.h"
#include "JATPDocChoixSupport.h"
//#include "DlgMapping.h"

class CDlgMapping;

//class AFX_EXT_CLASS CListPoint  
class CListPoint  
{
private:
	class Point
	{
	public:
		double  m_XElt;			// la position en x			 de l'element
		double  m_YElt;			// la position en y			 de l'element
		bool    m_IsSelElt;		// la selection				 de l'element
		bool    m_IsShowElt;	// la visibilite			 de l'element
		bool    m_IsCbElt;		// la possession � la courbe de l'element
		double  m_XLib;			// la position en x			 du libell�
		double  m_YLib;			// la position en y			 du libell�
		bool    m_IsSelLib;		// la selection				 du libell�
		bool    m_IsShowLib;	// la visibilite			 du libell�
		int     m_IdentElt;		// l'identificateur          de l'�l�ment
	};

public:
	CListPoint();

	// Fonction de r�cup�ration document
	void SetDocument(JATPDocChoixSupport* pDoc,CDlgMapping *pDlg);

	// Update liste des points mapping
	bool Update(JBool InitPos);

	// Taille
	long GetTaille();
	void SetTaille(long nb);

	// visibilite d'un element
	bool GetShowElt(long indice);
	void SetShowElt(long indice, bool etat);
	void SetShowEltById(int IdElmt, bool etat);
	
	// position d'un element
	double GetdXPosElt(long indice);
	double GetdYPosElt(long indice);
	void SetdXPosElt(long indice, double X);	
	void SetdYPosElt(long indice, double Y);

	// selection d'un point
	bool GetSelElt(long indice);
	void SetSelElt(long indice, bool etat);

	// mise � jour s�lection point
	void MajSelElt();

	// mise � jour libell�s des points
	void MajLibelleElt();

	// l'identificateur de l'�l�ment
	int GetIdent(long indice);
	void SetIdent(long indice, int ident);

	// visibilite d'un libelle
	bool GetShowLib(long indice);
	void SetShowLib(long indice, bool etat);

	// Position d'un libell�
	double GetdXPosLib(long indice);
	double GetdYPosLib(long indice);
	void SetdXPosLib(long indice, double X);	
	void SetdYPosLib(long indice, double Y);

	// Nouvelle position d'un libell� 
	void SetPosLib(long indice, double X, double Y);
	
	// Position courante en X et Y d'un libell� 
	double GetXPosLib(long indice);
	double GetYPosLib(long indice);
	
	// selection d'un libele
	bool GetSelLib(long indice);
	void SetSelLib(long indice, bool etat);

	// appartenance a la courbe generale de degr� 2 d'un element
	bool GetIsCourbeElt(long indice);
	void SetIsCourbeElt(long indice, bool etat);

	// R�cup�ration libell� du point
	CString GetLibPoint(long indice);

	// R�cup�ration libell� grandeur en abscisse
	CString GetLibAbscisse();

	// R�cup�ration libell� grandeur en ordonn�e
	CString GetLibOrdonnee();

	// r�cup�ration �tat affichage libell�s
	bool GetLibAfficher();

	// positionne �tat d'affichage des libell�s
	void SetLibAfficher(bool etat);

	// D�fini le nombre de points support  afficher
	long GetNbPointValid();

	// Affichage uniquement des points s�lectionn�s
	JVoid ShowSelItem();

	// Affichage uniquement des points non s�lectionn�s
	JVoid HideSelItem();

	// Affichage de tous les points
	JVoid ShowAllItem();
	
	// Destructeur liste des points
	~CListPoint();

protected:
	// Liste des points � traiter
	Array<Point> m_Liste;

	// Liste des idents �lmts correspondants (comme un ItemData)
	// Array<JUnt32> m_ListeIdElmt;
	JMap <JUnt32,JUnt32> m_MapIdElmtMapping;

	// Pointeur sur document en cours
	JATPDocChoixSupport *m_pDocChoixSupport;

	// Pointeur sur dialogue Mapping
	CDlgMapping *m_pDlgMapping;

	// Liste des infos points affich�s
	JMap <long,CString> m_MapInfoPoint;

	// Mode affichage libell� des points (visible ou invisible);
	bool LibelleVisible;

};

#endif // !defined(AFX_LISTPOINT_H__5BD886A5_F3B5_45D3_92CD_87BAB0D9921B__INCLUDED_)
