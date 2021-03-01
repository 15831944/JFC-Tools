#if !defined(AFX_ATPCreusetHypo_C2_H__0AF4083E_A4A9_4F7B_B144_8919950208B4__INCLUDED_)
#define AFX_ATPCreusetHypo_C2_H__0AF4083E_A4A9_4F7B_B144_8919950208B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les définitions nécessaires
#include "JATPDocCreuset.h"
#include "JFCWave.h"
#include "Insertion.h"
#include "JFCGridMultiCell.h"
class CATPCreusetHypo;

/////////////////////////////////////////////////////////////////////////////
// CATPCreusetHypo_C2

class CATPCreusetHypo_C2 : public JFCGridMultiCell
{
public:
	// le constructeur
	CATPCreusetHypo_C2();
	// le destructeur
	virtual ~CATPCreusetHypo_C2();

	// la fonction pour définir le document
	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);
	virtual JVoid SetParent(CATPCreusetHypo* pATPCreusetHypo);

	// la fonction pour mettre à jour le composant
	virtual JVoid Update();



protected:
	// la fonction pour dessiner l'intérieur des cellules
	void OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG ii, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD);

	// les fonctions pour capter les clicks de la souris
	void OnLButtonDown(LONG i, LONG ii,LONG j);
	void OnRButtonDown(LONG i, LONG ii,LONG j);
	// la fonction pour capter les déplacements de la souris
	void OnMouseMove(LONG i, LONG ii, LONG j);

protected:
	// les fonctions pour éditer les éléments
	virtual bool OnBeginEditing(CString &CurString, LONG i, LONG j);
	virtual bool OnEditItem(const TCHAR* pString, LONG i, LONG j);
	virtual void OnCancelEditing();
	virtual void OnEndEditing();

protected:
	// la fonction pour appliquer les mises à jour
	JVoid OnUpdate();

	// Ajout d'un support au plateau pour synchro Plan Calendaire <> Plan global
	JBool AjoutSupportPlateau(CKeySupport KeySup);

protected:
	//{{AFX_VIRTUAL(CATPCreusetHypo_C2)
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	//{{AFX_MSG(CATPCreusetHypo_C2)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	JATPDocCreuset* m_pATPDocCreuset; // le document
	CATPCreusetHypo* m_pATPCreusetHypo; // le parent

protected:

	void ComputeContrib(LONG i, LONG ii, LONG j) ; 
	// les valeurs précédentes de i et j pour OnMouseMove
	LONG m_prev_i; 
	LONG m_prev_ii; 
	LONG m_prev_j;

	int ConvertICellToFormat(int iCell);
	JMap<int,JInt32> m_mapICellFormat;

	// Ajoute nb insertion dans la cellule correspondante
	void AjoutInsertionCellule(LONG NoHypo, LONG NoFormat, LONG NoSupport, int NbAjoutIns);

	// Suppression nb insertion dans la cellule correspondante
	void SuppInsertionCellule(LONG NoHypo, LONG NoFormat, LONG NoSupport, int NbSuppIns);

	// Ajout ou supp insertions par bloc de lignes
	void AjoutOuSuppInsertionBlocRow(LONG Col, LONG RowDeb, LONG RowFin, LONG NoFormatCur, int NbFinalIns);

	// Récupère le nombre d'insertion courant sur ce support
	JUnt32 GetNbInsertion(LONG NoHypo, LONG NoFormat, LONG NoSupport);

	// Indices cellule éditer
	LONG m_RowCellEdit;
	LONG m_ColCellEdit;

	// Nb insertions en cours sur la cellule édition
	JUnt32 m_NbInsCurEdit;

	// No du format en cours
	LONG m_NoColFormatCur;

	// Maximum insertion autorisé
	static const int MaxInsertion = 100;
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPCreusetHypo_C2_H__0AF4083E_A4A9_4F7B_B144_8919950208B4__INCLUDED_)
