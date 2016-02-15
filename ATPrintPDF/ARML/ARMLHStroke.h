// ARMLHStroke.h: interface for the CHStroke class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARMLHSTROKE_H__EB8EA6D0_42F3_4DFD_A286_664E2AAB418E__INCLUDED_)
#define AFX_ARMLHSTROKE_H__EB8EA6D0_42F3_4DFD_A286_664E2AAB418E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CHStroke  
{
public:
	// le constructeur
	CHStroke();

	// le constructeur de recopie
	CHStroke(const CHStroke & source);

	// les fonctions pour modifier les paramètres
	void SetStartPosX(long startposx);
	void SetEndPosX(long endposx);
	void SetPosY(long posy);
	void SetStrokeStyle(long strokestyle);

	// l'opérateur d'égalité
	CHStroke & operator = (const CHStroke & source);

	// le destructeur
	virtual ~CHStroke();
public:
	// la position X de départ
	long m_StartPosX;
	// la position X d'arrivée
	long m_EndPosX;
	// la position verticale
	long m_PosY;
	// le style de trait
	long m_StrokeStyle;
};

#endif // !defined(AFX_ARMLHSTROKE_H__EB8EA6D0_42F3_4DFD_A286_664E2AAB418E__INCLUDED_)
