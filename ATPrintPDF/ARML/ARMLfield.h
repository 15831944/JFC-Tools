// armlfield.h: interface for the CField class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARMLFIELD_H__2B8F302A_5714_43B7_A4FB_232B2BCCE150__INCLUDED_)
#define AFX_ARMLFIELD_H__2B8F302A_5714_43B7_A4FB_232B2BCCE150__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CField  
{
public:
	// le constructeur
	CField();

	// le constructeur de recopie
	CField(const CField & source);

	// l'opérateur d'égalité
	CField & operator=(const CField & operande);

	// la fonction pour fixer la position en X
	void SetPosX(long posX);
	
	// la fonction pour fixer la position en Y
	void SetPosY(long posY);

	// la fonction pour fixer le paramètre de gras
	void SetBold(bool bold);

	// la fonction pour fixer la justification
	void SetJustification(char justification);

	// la fonction pour fixer le texte
	void SetText(const CString & texte);

	// le destructeur
	virtual ~CField();
public:
	// la ligne
	long m_PosY;
	// la position
	long m_PosX;
	// la justification
	long m_Justification;
	// le texte
	CString m_Texte;
	// le flag gras
	bool m_Bold;
};

#endif // !defined(AFX_ARMLFIELD_H__2B8F302A_5714_43B7_A4FB_232B2BCCE150__INCLUDED_)
