#include "StdAfx.h"
#include ".\gridponderation.h"

#include "DlgCiblePondere.h"

CGridPonderation::CGridPonderation(void)
	:	JFCSpinEditGrid(0, 2, JFCSpinEditGrid::semEdit)	
{
}

CGridPonderation::~CGridPonderation(void)
{
}

////////////////////////////////////////////////////////////////////////
// Retour édition d'une cellule
void CGridPonderation::OnSpinEditCellEdited (ROWCOL nRow, ROWCOL nCol)
{
	// Récupère la valeur de la cellule
	if (nCol <= segLibelle )
	{
		// En format texte
		CString Txt;
		Txt = this->GetCellText(nRow, (JFCSpinEditGrid::eSpinEditGridColumnIndex)nCol); 
	}
	else
	{
		// En format Float
		JFlt64 Valeur;
		Valeur = this->GetCellValue(nRow, (JFCSpinEditGrid::eSpinEditGridColumnIndex)nCol); 
	}

	// Envoi événement changement de valeur à la boite de dialogue
	m_pDlgParent->UpdateValue(nRow, nCol);
}

////////////////////////////////////////////////////////////////////////
// Passe la fenêtre parent
void CGridPonderation::SetDlgParent(CDlgCiblePondere * pDlgParent)
{
	m_pDlgParent = pDlgParent;
}