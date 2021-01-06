///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		    Classe dérivée de JFCSpinEditGrid : grille pondération pour les cibles pondérées
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "JFCSpinEditGrid.h"
class CDlgCiblePondere;

class CGridPonderation : public JFCSpinEditGrid
{
public:
	// Constructeur
	CGridPonderation(void);

	// Destructeur
	~CGridPonderation(void);

	// Retour édition d'une cellule
	void OnSpinEditCellEdited (ROWCOL nRow, ROWCOL nCol);

	// Passe la fenêtre parent
	void SetDlgParent(CDlgCiblePondere * pDglParent);

private:
	CDlgCiblePondere * m_pDlgParent;
};
