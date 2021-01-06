// JFCSpinEditGrid.cpp�: fichier d'impl�mentation
//

#include "stdafx.h"
#include "JFCSpinEditGrid.h"
#include "JFCSpinEditGrid.h"
#include "GXSpinEditCell.h"
#include "JFCNumericCell.h"

#if !defined(BUILD_FOR_ATELIER_INTERNET)

// Gestionnaires de messages de JFCSpinEditGrid
BEGIN_MESSAGE_MAP(JFCSpinEditGrid, CGXGridWnd)
	ON_WM_SETFOCUS()
	ON_MESSAGE(CGXSpinEditCell::SPINEDITCELL_END_EDIT, SpinEditCellEdited)
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////
// le constructeur
JFCSpinEditGrid::JFCSpinEditGrid(const int iNumLines, const int iNumDecimales, eSpinEditGridMode eMode)
	:	m_iNumRows				(iNumLines)
	,	m_iGlobalDecimalesNum	(iNumDecimales)
	,	m_Mode					(eMode)
{
	m_DecimalesVec.resize(iNumLines, iNumDecimales);
}

////////////////////////////////////////////////////////////
// le destructeur
JFCSpinEditGrid::~JFCSpinEditGrid()
{

}

////////////////////////////////////////////////////////////
void JFCSpinEditGrid::SetHeaderText(eSpinEditGridColumnIndex eColIndex, LPCTSTR sTitre)
{
	// on inhibe la lecture seule sur toute la grille
	GetParam()->SetLockReadOnly(FALSE);
	
	// on d�finit les titres
	SetValueRange(CGXRange(0,eColIndex), sTitre);

	// on reinstaure la lecture seule sur toute la grille
	GetParam()->SetLockReadOnly(FALSE);
}

////////////////////////////////////////////////////////////
void JFCSpinEditGrid::SetCellText(const int iLineIdx, const eSpinEditGridColumnIndex eColIndex, LPCTSTR lpNewValue)
{
	// on inhibe la lecture seule sur toute la grille
	GetParam()->SetLockReadOnly(FALSE);

	// on d�finit les titres
	SetStyleRange(CGXRange(iLineIdx, eColIndex), CGXStyle().SetValue(lpNewValue));

	// on reinstaure la lecture seule sur toute la grille
	GetParam()->SetLockReadOnly(FALSE);
}

////////////////////////////////////////////////////////////
void JFCSpinEditGrid::SetCellText(const int iLineIdx, const eSpinEditGridColumnIndex eColIndex, double dNewValue)
{
	// on inhibe la lecture seule sur toute la grille
	GetParam()->SetLockReadOnly(FALSE);

	ASSERT((unsigned int)iLineIdx <= m_DecimalesVec.size());

	// on formatte avec le nombre de d�cimales de cette ligne
	CString sTmp; sTmp.Format("%.*f", m_DecimalesVec[iLineIdx - 1], dNewValue);

	// on d�finit les titres
	SetStyleRange(CGXRange(iLineIdx, eColIndex), CGXStyle().SetValue(sTmp));

	// on reinstaure la lecture seule sur toute la grille
	GetParam()->SetLockReadOnly(FALSE);
}

////////////////////////////////////////////////////////////
// Les fonctions pour r�cup�rer les valeurs d'une cellule
LPCSTR JFCSpinEditGrid::GetCellText(const int iLineIdx, const eSpinEditGridColumnIndex eColIndex)
{
	// V�rification index ligne correct
	ASSERT((unsigned int)iLineIdx <= m_DecimalesVec.size());

	// R�cup�re la valeur texte
	return (LPCSTR)GetValueRowCol(iLineIdx, eColIndex);

}

////////////////////////////////////////////////////////////
double JFCSpinEditGrid::GetCellValue(const int iLineIdx, const eSpinEditGridColumnIndex eColIndex)
{
	// V�rification index ligne correct
	ASSERT((unsigned int)iLineIdx <= m_DecimalesVec.size());

	return (atof(GetValueRowCol(iLineIdx, eColIndex)));
}

////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
void JFCSpinEditGrid::OnInitialUpdate()
{
	CGXGridWnd::OnInitialUpdate();

	// on cr�e l'instance du contr�le d�riv�e du spin
	CGXSpinEditCell * pNewControl = new CGXSpinEditCell(this, GXIDS_JFC_SPINEDIT_CTRL);

	// on enregistre ce nouveu contr�le dans la ControlFactory
	RegisterControl(GXIDS_JFC_SPINEDIT_CTRL, pNewControl);
	((CGXStatic*)pNewControl)->ImplementCellTips();

	// on cr�e l'instance du contr�le edit normal
	JFCNumericCell * pNewControl2 = new JFCNumericCell(this, GXIDS_JFC_EDIT_CTRL);

	// on enregistre ce nouveu contr�le dans la ControlFactory
	RegisterControl(GXIDS_JFC_EDIT_CTRL, pNewControl2);
	((CGXStatic*)pNewControl2)->ImplementCellTips();

	WORD CONTROL_ID = -1;
	switch (m_Mode)
	{
		case semSpinEdit:	CONTROL_ID = GXIDS_JFC_SPINEDIT_CTRL;
			break;
		case semEdit:		CONTROL_ID = GXIDS_JFC_EDIT_CTRL;
			break;
		default:throw new std::exception;
			break;
	}


	// on bloque l'undo le temps de d�finir le nombre de lignes et de colonnes
	GetParam()->EnableUndo(FALSE);

	// on d�finit le nombre de lignes et de colonnes
	SetRowCount(m_iNumRows);
	SetColCount(segPondere);

	// on d�finit la largeur de la 1�re colonne
	SetColWidth(segLibelle, segLibelle, 90);

	// on d�finit la hauteur de toutes les lignes
	SetRowHeight(0, m_iNumRows, 20);

	// on d�finit la 1�re colonne comme n'etant pas une colonne de titre
	ChangeRowHeaderStyle(StandardStyle());

	// on d�finit le style de la 1�re colonne
	CGXRange col1_2Rng; col1_2Rng.SetCols(segLibelle, segReel);
	CGXRange colLibelleRng; colLibelleRng.SetCols(segLibelle);

	// on emp�che l'utilisateur de redimensionner les lignes et les colonnes
	GetParam()->EnableTrackColWidth(FALSE);
	GetParam()->EnableTrackRowHeight(FALSE);

	// on d�finit en contr�le static et readonly les 2 premi�res colonnes
	SetStyleRange(col1_2Rng, CGXStyle() .SetControl(GX_IDS_CTRL_STATIC)
										.SetReadOnly(TRUE));

	// on d�finit le style de toute la grille (centrage)
	CGXRange _AllTableRng; _AllTableRng.SetTable();
	SetStyleRange(_AllTableRng, CGXStyle().SetHorizontalAlignment(DT_CENTER)
										  .SetVerticalAlignment(DT_VCENTER)
										  .SetReadOnly(TRUE)
										  .SetBorders(gxBorderAll, CGXPen().SetStyle(PS_SOLID)));

	// on d�finit le style de la colonne contenant les spinedit d�riv�s
	CGXRange _2ndColRange; _2ndColRange.SetCols(segPondere);
	SetStyleRange(_2ndColRange, CGXStyle()	 .SetControl(CONTROL_ID)
											 .SetVerticalAlignment(DT_VCENTER)
											 .SetHorizontalAlignment(DT_CENTER)
											 .SetDraw3dFrame(gxFrameRaised)
											 .SetFloatCell(TRUE)
											 .SetValueType(GX_VT_NUMERIC )
											 .SetFormat(GX_FMT_FIXED).SetPlaces(2)
											 .SetReadOnly(FALSE));

	
	// on r�cup�re le style de la seconde colonne (celle des spinEdit)		
	GetColStyle(segPondere, m_SpinColStyle);


	m_SpinColStyle.SetPlaces(m_iGlobalDecimalesNum);
	SetStyleRange(_2ndColRange, m_SpinColStyle);


	// on d�bloque l'Undo
	GetParam()->EnableUndo(TRUE);

}

////////////////////////////////////////////////////////////
void JFCSpinEditGrid::OnModifyCell (ROWCOL nRow, ROWCOL nCol)
{
	CString msg; msg.Format("JFCSpinEditGrid::OnModifyCell(nRow : %d, nCol : %d)\n", nRow, nCol);
	OutputDebugString(msg);

	//si on a click� sur la colonne des SpinEditCells
	//if (nCol == segPondere)
	//{
	//	// a conserver : pour VS 2005
	//	//INumericCell * pNumericCell = dynamic_cast<INumericCell*>(GetControl(nRow, nCol));

	//	INumericCell * pNumericCell = static_cast<INumericCell*>((INumericCell*)GetControl(nRow, nCol));

	//	ASSERT(nRow <= m_DecimalesVec.size());
	//	if (pNumericCell)
	//	{
	//		int ValDec = m_DecimalesVec[nRow - 1];

	//		pNumericCell->SetNumberOfDecimals(m_DecimalesVec[nRow - 1]);
	//		pNumericCell->SetMinValue(m_dMinVal);
	//		pNumericCell->SetMaxValue(m_dMaxVal);
	//	}
	//}

	// on enregistre les coordonn�es de la cellule en train d'�tre �dit�e
	m_nRow = nRow; m_nCol = nCol;

	CGXGridCore::OnModifyCell(nRow, nCol);
}

////////////////////////////////////////////////////////////
BOOL JFCSpinEditGrid::AddLines (int iInsertAt, int iNumNewLines)
{
	// on d�termine ou ajouter la position de la derni�re ligne
	const int iRowCount = GetRowCount() + 1;

	// on v�rifie le nombre de lignes du vecteur
	ASSERT(m_DecimalesVec.size() == (size_t) GetRowCount());

	// check du nombre de lignes � inserer/ajouter
	ASSERT (iNumNewLines >= 0);

	if (iNumNewLines == 0) return FALSE;

	// si on ajoute en fin de grille
	if (iInsertAt < 0)
	{	
		// on tente d'ajouter la/les ligne(s)
		if (InsertRows(iRowCount, iNumNewLines))
		{
			for (int iNumElems = 0; iNumElems < iNumNewLines; ++iNumElems)
				m_DecimalesVec.push_back(m_iGlobalDecimalesNum);
			return TRUE;
		}
	}
	// si il n'y a pas autant de lignes dans la grille
	else
	{
		// on borne la ligne d'insertion
		iInsertAt = min(iRowCount, iInsertAt);

		// on tente d'ajouter la/les lignes(s)
		if (InsertRows(iInsertAt, iNumNewLines))
		{
			// vecteur : on cherche la position ou ajouter les nouvelles lignes
			size_t iLine = 1;
			vNumDecimalesVec::iterator dec_it = m_DecimalesVec.begin();
			while(dec_it != m_DecimalesVec.end())
			{
				if (iLine == iInsertAt)
				{
					break;
				}
				++dec_it; ++iLine;
			}

			if (dec_it != m_DecimalesVec.begin())
			{
				ASSERT(dec_it != m_DecimalesVec.end());
			}

			// on efface la ligne en question du vecteur
			m_DecimalesVec.insert(dec_it, iNumNewLines, m_iGlobalDecimalesNum);
			return TRUE;
		}
	}

	return FALSE;
}

////////////////////////////////////////////////////////////
// la fonction pour supprimer une ligne
BOOL JFCSpinEditGrid::DeleteLines (int iLineToDelete)
{
	// on d�termine ou ajouter la position de la derni�re ligne
	const int iRowCount = GetRowCount() + 1;

	if (iRowCount == 1) return FALSE;

	// on v�rifie le nombre de lignes du vecteur
	ASSERT(m_DecimalesVec.size() == (size_t) GetRowCount());

	if (iLineToDelete > 0 && iLineToDelete <= iRowCount)
	{
		// on tente de supprimer la ligne	
		if (RemoveRows(iLineToDelete, iLineToDelete))
		{			
			// on cherche la ligne � effacer
			size_t iLine = 1;
			vNumDecimalesVec::iterator dec_it = m_DecimalesVec.begin();
			while(dec_it != m_DecimalesVec.end())
			{
				if (iLine == iLineToDelete)
				{
					break;
				}
				++dec_it; ++iLine;
			}

			if (dec_it != m_DecimalesVec.begin())
			{
				ASSERT(dec_it != m_DecimalesVec.end());
			}

			// on efface la ligne en question du vecteur
			m_DecimalesVec.erase(dec_it);

			return TRUE;
		}
	}
	else if (iLineToDelete == -1)
	{
		// on tente de supprimer toutes les lignes
		if (RemoveRows(1, iRowCount))
		{
			// on supprime le vecteur de d�cimales
			m_DecimalesVec.clear();
		
			return TRUE;
		}
	}

	// on ne peut pas suppprimer la ligne de titres ni une ligne inexistante !!
	return FALSE;
}

////////////////////////////////////////////////////////////
void JFCSpinEditGrid::SetSpinCellsNumDecimales (const int iNumDecimales, const int iSpinLine)
{
	CGXRange ModifiedRange;	
	
	// le nombre pass� est global
	if (iSpinLine == -1)
	{
		// on d�finit le range comme �tant toute la colonne des SpinEditCell
		ModifiedRange.SetCols(segPondere);

		// le nombre par d�faut de d�cimales devient iNumDecimales
		m_iGlobalDecimalesNum = iNumDecimales;

		// on red�finit le nombre de lignes dans le vecteur
		m_DecimalesVec.resize((size_t)GetRowCount());

		// on change �galement le nbr de d�cimales de toutes les lignes
		vNumDecimalesVec::iterator dec_it = m_DecimalesVec.begin();
		while (dec_it != m_DecimalesVec.end())
		{
			(*dec_it) = iNumDecimales;
			++dec_it;
		}
	}
	// le nombre pass� est sp�cifique � une SpinCell
	else
	{
		// on d�finit le range comme �tant l'unique cellule � modifier
		ModifiedRange.SetCells(iSpinLine, segPondere);

		// on red�finit le nbr de d�cimales de cette ligne
		m_DecimalesVec[iSpinLine - 1] = iNumDecimales;
	}


	// on d�finit �galement le nombre de d�cimales lorsque le spin est activ�
	SetStyleRange(ModifiedRange, CGXStyle().SetPlaces(iNumDecimales));
}

////////////////////////////////////////////////////////////
// la fonction pour d�finir les bornes des cellules de la colonne des SpinEdit
void JFCSpinEditGrid::SetSpinCellsBounds (const double dMinVal, const double dMaxVal)
{
	m_dMinVal = dMinVal;
	m_dMaxVal = dMaxVal;
}


////////////////////////////////////////////////////////////
// callback appel� lorsqu'une cellule est modifi�e
LRESULT JFCSpinEditGrid::SpinEditCellEdited(WPARAM wParam, LPARAM lParam)
{
	OnSpinEditCellEdited(m_nRow, m_nCol);
	return 0;	
}

////////////////////////////////////////////////////////////
// callback appel� lorsqu'une cellule est modifi�e
void JFCSpinEditGrid::OnSpinEditCellEdited (ROWCOL nRow, ROWCOL nCol)
{
	// on ne fait rien
}
#endif //!defined(BUILD_FOR_ATELIER_INTERNET)
