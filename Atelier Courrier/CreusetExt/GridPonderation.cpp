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
// Retour �dition d'une cellule
void CGridPonderation::OnSpinEditCellEdited (ROWCOL nRow, ROWCOL nCol)
{
	// R�cup�re la valeur de la cellule
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

	// Envoi �v�nement changement de valeur � la boite de dialogue
	m_pDlgParent->UpdateValue(nRow, nCol);
}

////////////////////////////////////////////////////////////////////////
// Passe la fen�tre parent
void CGridPonderation::SetDlgParent(CDlgCiblePondere * pDlgParent)
{
	m_pDlgParent = pDlgParent;
}