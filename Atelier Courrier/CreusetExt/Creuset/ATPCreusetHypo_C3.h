#if !defined(AFX_ATPCreusetHypo_C3_H__F55C2542_8361_4923_825F_2D90ACFCC1D5__INCLUDED_)
#define AFX_ATPCreusetHypo_C3_H__F55C2542_8361_4923_825F_2D90ACFCC1D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les définitions nécessaires
#include "JATPDocCreuset.h"
#include "JFCWave.h"
#include "JFCGridMultiCell.h"

/////////////////////////////////////////////////////////////////////////////
// CATPCreusetHypo_C3

class CATPCreusetHypo_C3 : public JFCGridMultiCell
{
public:
	// le constructeur
	CATPCreusetHypo_C3();

	// la fonction pour définir le document
	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);

	// la fonction pour mettre à jour le composant
	virtual JVoid Update();

	// le destructeur
	virtual ~CATPCreusetHypo_C3();

protected:
	// la fonction pour appliquer les mises à jour
	JVoid OnUpdate();

	// la fonction pour dessiner l'intérieur des cellules
	JVoid OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG ii, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD);


	// Ajoute ou retranche x insertions sur l'hypothèse courante pour les formats actifs
	JBool OnInsertionsHyp(JInt32 NbInsertions,JUnt32 NoHypothese, int idFormat);


	// les fonctions pour capter les clicks de la souris
	JVoid OnLButtonDown(LONG i, LONG ii, LONG j);
	JVoid OnRButtonDown(LONG i, LONG ii, LONG j);

protected:
	// les fonctions pour éditer les éléments
	virtual bool OnBeginEditing(CString &CurString, LONG i, LONG j);
	virtual bool OnEditItem(const TCHAR* pString, LONG i, LONG j);
	virtual void OnCancelEditing();
	virtual void OnEndEditing();

private:
	int ConvertICellToFormat(int iCell);
	JMap<int,JInt32> m_mapICellFormat;

protected:

	// Ajoute nb insertion sur tous les supports
	void AjoutInsertionAllSupport(LONG NoHypo, LONG NoFormat, int NbAjoutIns);

	// Suppression nb insertion sur tous les supports
	void SuppInsertionAllSupport(LONG NoHypo, LONG NoFormat, int NbSuppIns);

	// Récupère le nombre d'insertion total sur hypothese
	JUnt32 GetNbInsertion(LONG NoHypo, LONG NoFormat);

	// Indices cellule éditer
	LONG m_RowCellEdit;
	LONG m_ColCellEdit;

	// Nb insertions en cours sur la cellule édition
	JUnt32 m_NbInsCurEdit;

	// No du format en cours
	LONG m_NoColFormatCur;

	// Maximum insertion autorisé
	static const int MaxInsertion = 100;

protected:
	//{{AFX_VIRTUAL(CATPCreusetHypo_C3)
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CATPCreusetHypo_C3)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	JATPDocCreuset* m_pATPDocCreuset; // le document
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPCreusetHypo_C3_H__F55C2542_8361_4923_825F_2D90ACFCC1D5__INCLUDED_)
