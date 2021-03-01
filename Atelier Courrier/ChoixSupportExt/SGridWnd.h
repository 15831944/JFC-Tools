#pragma once
#include <grid\gxwnd.h>
#include "JATPDocChoixSupport.h"
#include "JLib.h"

class CSGridWnd : public CGXGridWnd
{
	DECLARE_DYNAMIC(CSGridWnd)

public:
	// Constructeur
	CSGridWnd(void);

	// Destructeur
	virtual ~CSGridWnd(void);

	// Init document choix support
	virtual JBool OnInitDoc(JATPDocChoixSupport *pDoc, JUnt32 NoColIdSupport=0);

	// Modifie le contenu pour trier une colonne d'un grid
	virtual void FormatCellTri(JUnt32 NoColonne, JUnt32 NbHeaderRow, JUnt32 NoColDeb, JBool &AffichPrct);

	// Remet affichage de base
	virtual void FormatCellFinal(JUnt32 NoColonne, JUnt32 NbHeaderRow, JUnt32 NoColDeb, JBool &AffichPrct);

	// Menu popup sans accés familles support
	virtual void AffichPopupMenu(JUnt32 IdSupport, CPoint pt, JBool AffichPopupFamille);

	// Blocage des colonnes normales (en dehors des combobox, editbox etc....)
	virtual void SetColReadOnly(JUnt32 NbHeaderRow, JUnt32 NoColIdSupport,JUnt32 NoColSupport, JUnt32 NoColPeriodicite);

	// Type de formatage chaine cellule grid
	enum {FMT_TEXT, FMT_TEXT_MILLIERS, FMT_INT, FMT_FLOAT_1DEC, FMT_FLOAT_2C};

	void SetTooltipText(int nText, BOOL bActivate = TRUE);
	void SetTooltipText(LPCTSTR lpszText, BOOL bActivate = TRUE);

	void ActivateTooltip(BOOL bEnable = TRUE);

	// Affichage en mode tooltip info cellule
	void AfficheInfoCellule(ROWCOL nRow, ROWCOL nCol, CString ValReelle, bool AffichOk);

	// Affichage composants famille
	bool VisuCompoFamille(JUnt32 IdFamille, ROWCOL nRow, ROWCOL nCol);

protected :

	// Gestion menu popup
	void InsertMenu(INode* nd, CMenu & menu, int & i );
	JMap<JUnt32, JUnt32> m_mapMenu;

	// le document
	JATPDocChoixSupport *m_pDocChoixSupport;	

	// Info colonne n° id support
	JUnt32 m_NoColIdSupport;

private :
	// Complète chaine numérique avec des caractères devant (pour palier au problème tri)
	void CompleteString(CString &Txt, JUnt32 NbTotCar);

	// Recompose la chaine selon le formatage demandée
	void RebuildString(CString &Txt, JUnt32 ModeRebuild, JUnt32 NbDecimale);

	// Découpage chaine par milliers
	void FormatString(CString &Txt, JUnt32 PasDecoup);

// ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAnCrossTable)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

public:
	// Commandes Menu Popup
	DECLARE_MESSAGE_MAP()
	afx_msg void OnFamilleCentre(UINT nID);
	afx_msg void OnInfoTitre();
	afx_msg void OnCacheSupport();
	afx_msg void OnAllSupportVisible();
	afx_msg void OnOnlySupportSelVisible();
	afx_msg void OnOnlySupportNonSelVisible();
	afx_msg void OnModifTarif();

	afx_msg BOOL OnToolTipText(UINT id, NMHDR * pNMHDR, LRESULT * pResult);
	virtual int OnToolHitTest(CPoint point, TOOLINFO * pTI);

private:
	void InitToolTip();
	CToolTipCtrl m_ToolTip;
	DWORD		m_dwToolTipStyle;	///< Style of tooltip control


};


