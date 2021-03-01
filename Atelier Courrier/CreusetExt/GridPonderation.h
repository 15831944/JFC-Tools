///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		    Classe d�riv�e de JFCSpinEditGrid : grille pond�ration pour les cibles pond�r�es
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

	// Retour �dition d'une cellule
	void OnSpinEditCellEdited (ROWCOL nRow, ROWCOL nCol);

	// Passe la fen�tre parent
	void SetDlgParent(CDlgCiblePondere * pDglParent);

private:
	CDlgCiblePondere * m_pDlgParent;
};
