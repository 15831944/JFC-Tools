// ARMLLogo.h: interface for the CLogo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARMLLOGO_H__0BF04B85_F724_4E5B_8B08_C22971DEF658__INCLUDED_)
#define AFX_ARMLLOGO_H__0BF04B85_F724_4E5B_8B08_C22971DEF658__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les fichiers n�cessaires
#include "./../Composite/JFCDrawComposite.h"

class StateFooter;

class CLogo  
{
	friend class StateFooter;
public:
	// le constructeur
	CLogo();

	// le constructeur de recopie
	CLogo(const CLogo & source);

	// l'op�rateur d'�galit�
	CLogo & operator=(const CLogo & operande);

	// la fonction pour fixer le chemin du logo
	void SetPath(const char * logopath);

	// la fonction pour fixer le nombre de lignes
	void SetStartPosX(long startposX);
	void SetEndPosX(long endposX);
	void SetStartPosY(long startposY);
	void SetEndPosY(long endposY);
	
	// la fonction pour cr�er le logo
	void CreateLogo(DrawComposite * composant);

	// le destructeur
	virtual ~CLogo();
protected:
	// le chemin du logo
	CString m_LogoPath;
	// les coordonn�es maximales du cadre
	long m_StartPosX;
	long m_EndPosX;
	long m_StartPosY;
	long m_EndPosY;
};

#endif // !defined(AFX_ARMLLOGO_H__0BF04B85_F724_4E5B_8B08_C22971DEF658__INCLUDED_)
