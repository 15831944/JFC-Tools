///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// Idem CThemeTree mais n'affichera que les questions quantivatives du questionnaire, en enlevant également tous
// les chapitres ne contenant aucun elmt quantitatifs
// 
// On affichera ici aussi toutes les branches-enfants correspondant aux questions 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "themetree.h"
#include "DlgItems.h"

class AFX_EXT_CLASS CThemeTreeQuanti : public CThemeTree
{
public:
	// Constructeur
	CThemeTreeQuanti();

	// Destructeur
	~CThemeTreeQuanti(void);


protected:
	//{{AFX_MSG(CThemeTree)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:

	// Remplissage du tree des questions quantitatives
	virtual void SetData(CTerrain *pTerrain, CItemsConstruitsBase * pIConst,CString & sFilter, int iModality, bool UpdateNow = false);

	// Update des sélections
	virtual void ToggleItemState(HTREEITEM hti, bool WithSelection = true);

	// Affiche les questions pré-sélectionnées
	virtual void AffichPreselectQu();
	virtual void AffichPreselectQu(HTREEITEM hti);

	// Complete les dernières termes avec les questions associées
	void FillWithAllQuestions(HTREEITEM hti);

	// Liste des questions quantitatives sélectionnées
	JMap <JInt32,JInt32> m_MapSelQuestQuanti;

	// Recup liste des questions à mettre à jour
	const JList <JInt32> * GetLstQuestions()
	{
		return &m_LstBlockId_Sel;
	};

protected:

	// Ajoute ou supprime les questions de cette branche
	void ActiveQuestions(HTREEITEM hti, JBool ItemSelect);

	// Liste des questions résultantes à mettre à jour
	JList <JInt32> m_LstBlockId_Sel;

};
