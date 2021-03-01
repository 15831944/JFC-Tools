#include "StdAfx.h"
#include ".\ancrosstable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	TTS_BALLOON		0x40

// Constructeur
CAnCrossTable::CAnCrossTable(void)
{
	m_pItemTable = 0;
	m_nClipboardFlags = GX_DNDTEXT|GX_DNDCOLHEADER|GX_DNDROWHEADER ;
	m_bDisableMouseMoveOver = FALSE;

	// A VOIR ToolTip Grid Analyseur
	this->EnableToolTips(true); 

	// No tooltip created
	m_ToolTip.m_hWnd = NULL;
	m_dwToolTipStyle = TTS_ALWAYSTIP;

	// Ligne et colonne courante
	m_RowColChange      =   false;
	m_CurRow			=	-1;
	m_CurCol			=	-1;

	// Init chrono
	m_LastTimeUpdate = 0;

	// Init invalidate global
	m_InvalidateAllOk = true;

}

// Destructeur
CAnCrossTable::~CAnCrossTable(void)
{
}

BEGIN_MESSAGE_MAP(CAnCrossTable, CGXGridWnd)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
	ON_WM_SETCURSOR()
	ON_WM_TIMER()
END_MESSAGE_MAP()


//On tool tip test
// int CAnCrossTable::OnToolHitTest(CPoint point, TOOLINFO * pTI) const
int CAnCrossTable::OnToolHitTest(CPoint point, TOOLINFO * pTI)
{
	LVHITTESTINFO lvhitTestInfo;
	lvhitTestInfo.pt = point;
		
	this->HitTest(point);

	return -1;
}

//On tooltip text
BOOL CAnCrossTable::OnToolTipText(UINT /*id*/, NMHDR *pNMHDR, LRESULT *pResult)
{
	UINT nID = pNMHDR->idFrom;
		
	// check if this is the automatic tooltip of the control
	if (nID == 0) 
		return TRUE;	// do not allow display of automatic tooltip,
						// or our tooltip will disappear

	// handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	
	*pResult = 0;
	
	// get the mouse position
	const MSG* pMessage;
	pMessage = GetCurrentMessage();
	ASSERT(pMessage);
	CPoint pt;
	pt = pMessage->pt;		// get the point from the message
	ScreenToClient(&pt);	// convert the point's coords to be relative to this control
	
	// A FAIRE
	//
		
	return FALSE;	// we didn't handle the message, let the 
					// framework continue propagating the message
}


bool CAnCrossTable::SetIItemTable(IItemTable * pTable)
{
	m_pItemTable = pTable;

	// Init Timer
	SetTimer(999, 50, NULL);

	return true;
}

// Supply data (and other formats) from external
// data source
BOOL CAnCrossTable::GetStyleRowCol(ROWCOL nRow,
                          ROWCOL nCol, CGXStyle& style,
                          GXModifyType mt, int nType)
{
	if(nType >= GX_VALUE_TEXT && m_pItemTable)
	{
		if(m_pItemTable->GetStyleRowCol(nRow,nCol,style,mt,nType))
			return true;
	}
	return CGXGridWnd::GetStyleRowCol(nRow, nCol, style, mt, nType);
}

// Supply data (and other formats) from external
// data source
BOOL CAnCrossTable::GetTotStyleRowCol(ROWCOL nRowBegin, ROWCOL nRowEnd,
									  ROWCOL nColBegin, ROWCOL nColEnd,  CGXStyle& style,
								      GXModifyType mt, int nType)
{
	if(nType >= GX_VALUE_TEXT && m_pItemTable)
	{
		if(m_pItemTable->GetTotStyleRowCol(nRowBegin, nRowEnd, nColBegin, nColEnd,style,mt,nType))
			return true;
	}
	return CGXGridWnd::GetStyleRowCol(nRowBegin, nColBegin, style, mt, nType);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Indique s'il faut rendre invisible certaines lignes moyennes
bool CAnCrossTable::SomeRowToHide()
{
	CGXStyle style;
	GXModifyType mt = gxCopy;
	int nType		= 0;

	CELLTYPE Rt,Ct;
	int Ri,Ru,Rc,Rv;

	// Init map info cache lignes
	m_MapRowMoyenneAffich.Reset();

	// Boucle sur toutes les lignes et colonnes
	for (int nRow = 1; nRow <= GetRowCount(); ++nRow)
	{
		// Par défaut la ligne n'est pas affichable tant qu'il n'y a pas de valeur définie
		m_MapRowMoyenneAffich.Add(nRow) = false;

		// Récup information type ligne
		Rt = m_pItemTable->GetRowCoordinates(nRow,Ri,Ru,Rc,Rv);

		if (Rt == CTYPE_ITEMCELL)
		{
			// Test s'il s'agit d'une ligne moyenne non calculée, auquel cas on la cache
			m_MapRowMoyenneAffich.MoveTo(nRow);
			bool &MoyenneAffich = m_MapRowMoyenneAffich.GetItem();
			if (!MoyenneAffich)
			{
				return true;
			}
		}
	}

	/*
	// Coordonnées ligne colonne
	CELLTYPE Rt,Ct;
	int Ri,Ru,Rc,Rv;
	int Ci,Cu,Cc,Cv;

	// Rend invisible les lignes non définies
	for (int nRow = 0; nRow <= GetRowCount(); ++nRow)
	{
		// Récup information type ligne
		Rt = m_pItemTable->GetRowCoordinates(nRow,Ri,Ru,Rc,Rv);

		if (Rt == CTYPE_ITEMCELL)
		{
			// Test s'il s'agit d'une ligne moyenne non calculée, auquel cs on la cache
			m_MapRowMoyenneAffich.MoveTo(nRow);
			bool &MoyenneAffich = m_MapRowMoyenneAffich.GetItem();
			if (!MoyenneAffich)
			{
				return true;
			}
		}
	}
	*/

	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Redessin systématique de toute la grille
void CAnCrossTable::UpdateAllRowsCols()
{
	// Lock update grid during resize row
	this->LockUpdate(true);

	CGXStyle style;
	GXModifyType mt = gxCopy;
	int nType		= 0;

	// Init map info cache lignes
	m_MapRowMoyenneAffich.Reset();

	// Boucle sur toutes les lignes et colonnes
	for (int nRow = 1; nRow <= GetRowCount(); ++nRow)
	{
		// Par défaut la ligne n'est pas affichable tant qu'il n'y a pas de valeur définie
		m_MapRowMoyenneAffich.Add(nRow) = false;

		for (int nCol = 1; nCol <= GetColCount() ; ++nCol)
		{
			// Redessine couleur cellule
			GetStyleRowCol(nRow , nCol , style, mt, nType);
		}
	}
	
	/* AVOIR
	// OPTIM 2.7
	// Init Map Row Moyennable
	for (int nRow = 1; nRow <= GetRowCount(); ++nRow)
	{
		// Par défaut la ligne n'est pas affichable tant qu'il n'y a pas de valeur définie
		m_MapRowMoyenneAffich.Add(nRow) = false;
	}

	// OPTIM 2.7 Compacte appel GetStyleRowCol();
	GetTotStyleRowCol(1,GetRowCount(), 1, GetColCount(), style, mt, nType);
	*/

	// Coordonnées ligne colonne
	CELLTYPE Rt,Ct;
	int Ri,Ru,Rc,Rv;
	int Ci,Cu,Cc,Cv;

	// Rend invisible les lignes non définies
	for (int nRow = 0; nRow <= GetRowCount(); ++nRow)
	{
		// Récup information type ligne
		Rt = m_pItemTable->GetRowCoordinates(nRow,Ri,Ru,Rc,Rv);

		if (Rt == CTYPE_ITEMCELL)
		{
			// Test s'il s'agit d'une ligne moyenne non calculée, auquel cas on la cache
			m_MapRowMoyenneAffich.MoveTo(nRow);
			bool &MoyenneAffich = m_MapRowMoyenneAffich.GetItem();
			if (!MoyenneAffich)
			{
				this->SetRowHeight(nRow, nRow, 0);
			}
		}
		else
		{
			// Remet la taille origine de la ligne
			this->SetRowHeight(nRow, nRow, m_pItemTable->GetRowHdrHeight(nRow),0,GX_INVALIDATE);
		}
	}

	// Grid can be updated
	this->LockUpdate(false);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Redessin systématique de toute la grille seulement pour taille des cellules
void CAnCrossTable::UpdateAllRowsColsSize()
{
	// Lock update grid during resize row
	this->LockUpdate(true);

	CGXStyle style;
	GXModifyType mt = gxCopy;
	int nType		= 0;

	// Init map info cache lignes
	m_MapRowMoyenneAffich.Reset();
	
	// Coordonnées ligne colonne
	CELLTYPE Rt,Ct;
	int Ri,Ru,Rc,Rv;
	int Ci,Cu,Cc,Cv;

	// Rend invisible les lignes non définies
	for (int nRow = 0; nRow <= GetRowCount(); ++nRow)
	{
		for (int nCol = 1; nCol <= GetColCount() ; ++nCol)
		{
			// Est ce une ligne moyennable ??
			if (m_pItemTable->TypeRowMoyennable(nRow, nCol))
			{
				this->SetRowHeight(nRow, nRow, 0);
				break;
			}
		}
	}

	// Grid can be updated
	this->LockUpdate(false);
}

void CAnCrossTable::OnModifyCell (ROWCOL nRow, ROWCOL nCol)
{
	if(m_pItemTable && nCol==0)
	{
		CGXControl *pCtrl = GetControl(nRow,nCol);
		if(pCtrl &&pCtrl->IsKindOf(RUNTIME_CLASS(CGXComboBoxWnd)))
		{
			// Récupération de la valeur saisie page
			CString Val;
			CGXComboBoxWnd * pCombo = (CGXComboBoxWnd *)pCtrl;
			pCtrl->GetValue(Val);
			long CurPage = atol(Val);
			m_pItemTable->PageChanged(CurPage);

			// Le timer repart 
			SetTimer(999, 50, NULL);

			// On remet le focus sur le grid en désactivant le controle combo page
			pCtrl->SetActive(TRUE); 
			// this->SetFocus(); 
		}
	}
}

BOOL CAnCrossTable::OnLButtonClickedRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt)
{
	CWaitCursor wait;
	return m_pItemTable->OnLButtonClickedRowCol(nRow,nCol,nFlags,pt);
}

BOOL CAnCrossTable::OnRButtonClickedRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt)
{
	return m_pItemTable->OnRButtonClickedRowCol(nRow,nCol,nFlags,pt);
}

BOOL CAnCrossTable::OnTrackColWidth(ROWCOL nCol)
{
	return m_pItemTable->OnTrackColWidth(nCol);
}

BOOL CAnCrossTable::OnTrackRowHeight(ROWCOL nRow)
{
	return m_pItemTable->OnTrackRowHeight(nRow);
}

void CAnCrossTable::OnChangedSelection(const CGXRange* pRange, BOOL bIsDragging, BOOL bKey)
{
	if (bIsDragging)
	{	
		// this->SetStyleRange(*pRange, CGXStyle().SetValue(_T("X")));
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Timer pour gestion affichage déplacement de la souris sur le grille 
void CAnCrossTable::OnTimer(UINT nIDEvent)
{
	
	// Par défaut invalidate global non à effectuer
	JBool InvalidateAll = false;

	// Par défaut Combo Page Non Active 
	JBool ComboPageActive = false;

	// Lignes et colonnes sélectionnées
	CRowColArray TSelCol;
	CRowColArray TSelRow;
	this->GetSelectedCols(TSelCol);
	this->GetSelectedRows(TSelRow);

	// Lignes début et fin bloc sélectionnés
	int RowDebSel = 0;
	int RowFinSel = 0;

	// Colonnes début et fin sélectionnées
	int ColDebSel = 0;
	int ColFinSel = 0;

	
	
	// Récupération bornes en ligne de la partie sélectionnée
	if (TSelCol.GetCount() > 0)
	{
		RowDebSel = TSelRow.GetAt(0);
		RowFinSel = TSelRow.GetAt(TSelRow.GetCount()-1);
	}

	// Récupération bornes en colonne de la partie sélectionnée
	if (TSelCol.GetCount() > 0)
	{
		ColDebSel = TSelCol.GetAt(0);
		ColFinSel = TSelCol.GetAt(TSelCol.GetCount()-1);
	}

	JBool ZoneMarque = false;
	if (TSelCol.GetCount() > 1 || TSelRow.GetCount() > 1)
		ZoneMarque = true;
	
	// Temps écoulé depuis dernier rafraichissement
	LONG Time1		= GetTickCount();
	LONG TimeEcoule = Time1 - m_LastTimeUpdate;

	// Si temps écoulé excessif, on redessine tout
	// OPTIM 2.7
	if (TimeEcoule > 50 && !m_InvalidateAllOk)
	{
		InvalidateAll = true;
		m_LastTimeUpdate = Time1;
	}		

	// Nb cols ou rows avec terrains et garndeurs
	JUnt32 NbHeaderColTerrain  =  m_pItemTable->Get_NbHeaderColTerrain();
	JUnt32 NbHeaderColGrandeur =  m_pItemTable->Get_NbHeaderColGrandeur(); 
	JUnt32 NbHeaderRowTerrain  =  m_pItemTable->Get_NbHeaderRowTerrain(); 
	JUnt32 NbHeaderRowGrandeur =  m_pItemTable->Get_NbHeaderRowGrandeur(); 

	// On vérifie que la grid existe avec au moins 1 ligne ou 1 colonne bien
	JUnt32 NbRow = m_pItemTable->GetItemRowCount(); 
	JUnt32 NbCol = m_pItemTable->GetItemColCount(); 
	if (NbRow == 0 || NbCol == 0) 
		return;

	CGXControl *pCtrl = GetControl(1 + NbHeaderRowTerrain, 0);
	if(pCtrl &&pCtrl->IsKindOf(RUNTIME_CLASS(CGXComboBoxWnd)))
	{
		// if (pCtrl->IsActive() && m_pItemTable->GetRowCurHeader() < (2 + NbHeaderRowTerrain + NbHeaderRowGrandeur - 1) && m_pItemTable->GetColCurHeader() < (2 + NbHeaderColTerrain + NbHeaderColGrandeur - 1))
		// 	ComboPageActive = true;

		if (pCtrl->IsActive())
			ComboPageActive = true;
	}

	int RowEnCours = m_pItemTable->GetRowCurHeader();
	int ColEnCours = m_pItemTable->GetColCurHeader();

	// Attention à la dimension page, générer le suivi mouse uniquement pour les lignes après la ligne combobox page
	if (RowEnCours > 0)
	{
		if (RowEnCours > (1 + NbHeaderRowTerrain) || ColEnCours != 0)
		{
			ComboPageActive = false;
		}
	}

	if (InvalidateAll)
	{
		// Validation globale ou partielle (sans le bloc sélectionné)
		if (nIDEvent == 999)
		{
			if (!ComboPageActive)
			{
				if ( (!ZoneMarque || m_CurRow < RowDebSel || m_CurRow > RowFinSel || m_CurCol < ColDebSel || m_CurCol > ColFinSel) ||
					(ZoneMarque && m_CurRow >= RowDebSel && m_CurRow <= RowFinSel && m_CurCol >= ColDebSel && m_CurCol <= ColFinSel))	
				{
					// (Pas de zone marqué ou zone marquée et on se balade en dehors) ou (zone marquée et on se balade dedans)
					//
					// Rectangle du grille
					CRect RectGrille;
					CRgn rgn;
					this->GetClientRect(&RectGrille);

					// Calcul partie header ligne à redessiner
					CRect Rect;
					ROWCOL nRowHeader1	= 2 + NbHeaderRowTerrain + NbHeaderRowGrandeur - 1;
					Rect				= CalcRectFromRowCol(nRowHeader1, 3);
					Rect.right			+= RectGrille.right;
					
					// Redessine la zone header row 1
					rgn.CreateRectRgnIndirect(&Rect);
					InvalidateRgn(&rgn); 
					rgn.DeleteObject();

					// Calcul partie header colonne à redessiner
					ROWCOL nColHeader1	= 2 + NbHeaderColTerrain + NbHeaderColGrandeur - 1;
					Rect				= CalcRectFromRowCol(3, nColHeader1);
					Rect.bottom			+= RectGrille.bottom;

					// Redessine la zone header1 col
					rgn.CreateRectRgnIndirect(&Rect);
					InvalidateRgn(&rgn); 
					rgn.DeleteObject();

					// Invalidate();
					m_LastTimeUpdate = GetTickCount();
					m_InvalidateAllOk = true;
				}
				else
				{
					m_LastTimeUpdate = GetTickCount();
					m_InvalidateAllOk = true;
				}
			}
			else
			{
				m_LastTimeUpdate = GetTickCount();
				m_InvalidateAllOk = true;
			}
		}
	}
	else
	{
		// Validation des zones uniquement modifiées
		if (nIDEvent == 999)
		{
			if (!ComboPageActive)
			{
				ROWCOL nOldRow = 0;
				ROWCOL nOldCol = 0;

				// Validité colonne et ligne
				CPoint point;
					
				// on récupère la position du curseur
				DWORD pos = ::GetMessagePos();

				// on récupère les coordonnées du curseur
				point.x = (LONG)(SHORT)LOWORD(pos);
				point.y = (LONG)(SHORT)HIWORD(pos);

				// on convertit la position du curseur dans le repère de la fenêtre
				this->ScreenToClient(&point);

				// Calcul ligne et colonne correspondant au point survolé
				ROWCOL nRow;
				ROWCOL nCol;
				int Retour = this->HitTest(point, &nRow, &nCol); 

				// Lignes début et fin cellule
				int nRowDebCell = 0;
				int nRowFinCell = 0;
				int RowStartItem = 2;

				// Colonnes début et fin cellule
				int nColDebCell = 0;
				int nColFinCell = 0;
				int ColStartItem =  2;

			// Calcul nb ligne résultat en igne et en colonne pour une cellule
			JUnt32 ResultRowCount;
			JUnt32 ResultColCount;
			m_pItemTable->GetResultRowColCount(ResultRowCount, ResultColCount);

				if (ResultRowCount != 0 && ResultColCount != 0)
				{
			// Calcul des limites bloc cellule - Récupère ligne entete courante
			nRowDebCell =  nRow;
			if (nRowDebCell > 0)
			{
				nRowDebCell = ((nRowDebCell - (RowStartItem + NbHeaderRowTerrain + NbHeaderRowGrandeur) ) / ResultRowCount) * ResultRowCount + RowStartItem + NbHeaderRowTerrain + NbHeaderRowGrandeur;  
				nRowFinCell = nRowDebCell + ResultRowCount - 1;
			}

			// Récupère colonne entete courante
			nColDebCell = nCol;
			if (m_ItemColCount > 0)
			{	
				nColDebCell = ((nColDebCell - (ColStartItem + NbHeaderColTerrain + NbHeaderColGrandeur) ) / ResultColCount) * ResultColCount + ColStartItem + NbHeaderColTerrain + NbHeaderColGrandeur;  
				nColFinCell = nColDebCell + ResultColCount - 1;
					}

					// Test si mouse dans le grid
					if (Retour == 0)                      
					{
						nOldRow = m_CurRow;
						nOldCol = m_CurCol;

						// Initialisation de la ligne et de la colonnne courante
						if (m_CurRow != -1 || m_CurCol != -1)
						{
							m_CurRow = -1;
							m_CurCol = -1;
							m_RowColChange = true;
						}
					}
					// else if (m_CurRow != nRow)
					else if (m_CurRow < nRowDebCell || m_CurRow > nRowFinCell)
					{
						m_CurRow = nRow;
						m_CurCol = nCol;
						m_RowColChange = true;
					}
					// else if (m_CurCol != nCol)
					else if (m_CurCol < nColDebCell || m_CurCol > nColFinCell)
					{
						m_CurRow = nRow;
						m_CurCol = nCol;
						m_RowColChange = true;
					}

					// On invalidate la zone grille ssi ligne et/ou colonne change
					if (m_RowColChange)
					{
						// Coordonnées ligne colonne
						CELLTYPE Rt,Ct;
						int Ri,Ru,Rc,Rv;
						int Ci,Cu,Cc,Cv;


						Rt = m_pItemTable->GetRowCoordinates(m_CurRow,Ri,Ru,Rc,Rv);
						Ct = m_pItemTable->GetColCoordinates(m_CurCol,Ci,Cu,Cc,Cv);

						if (Rt == CTYPE_ITEMCELL  && Ct == CTYPE_ITEMCELL)
						{
							// Nombre de colonnes et de lignes sur une même cellule
							m_ItemColCount = m_pItemTable->GetItemColCount();
							m_ItemRowCount = m_pItemTable->GetItemRowCount();

							// Surlignage des headeres row et col associés à la cellule courante
							CGXStyle style;
							GXModifyType mt = gxCopy;
							int nType		= 0;
							GetStyleRowCol(m_CurRow , m_CurCol , style, mt, nType);
							
							// Mse à jour uniquement des zones à rafraichir
							CRgn rgn;
							CRect Rect;
							Rect = CalcRectFromRowCol(m_CurRow , m_CurCol );

							// puis on calcule la zone maxi à rafraichir pour la cellule
							int HtCell	= GetRowHeight(m_CurRow);
							int LgCell	= GetColWidth(m_CurCol); 

							if (!ZoneMarque || m_CurRow < RowDebSel || m_CurRow > RowFinSel || m_CurCol < ColDebSel || m_CurCol > ColFinSel)
							{
								Rect.top	-= HtCell * (m_ItemRowCount);
								Rect.bottom += HtCell * (m_ItemRowCount);
								Rect.left   -= LgCell * (m_ItemColCount);
								Rect.right  += LgCell * (m_ItemColCount);
								rgn.CreateRectRgnIndirect(&Rect);
								InvalidateRgn(&rgn); 

								// on redessine la fenêtre
								this->InvalidateRect(Rect, FALSE);
								rgn.DeleteObject();
							}
													
							// puis on calcule a zone maxi à rafraichir pour la zone header ligne
							ROWCOL nColColor = 1 + NbHeaderColGrandeur + NbHeaderColTerrain - 1;
							Rect		= CalcRectFromRowCol(m_CurRow, nColColor);
							Rect.top	-= HtCell * m_ItemRowCount;
							Rect.bottom += HtCell * m_ItemRowCount;
							rgn.CreateRectRgnIndirect(&Rect);
							InvalidateRgn(&rgn);

							// on redessine la fenêtre
							this->InvalidateRect(Rect, FALSE);
							rgn.DeleteObject();
							
							// puis on calcule a zone maxi à rafraichir pour la zone header colonne
							ROWCOL nRowColor = 1 + NbHeaderRowGrandeur + NbHeaderRowTerrain - 1;
							Rect = CalcRectFromRowCol(nRowColor, m_CurCol);
							Rect.left  -= LgCell * m_ItemColCount;
							Rect.right += LgCell * m_ItemColCount;
							rgn.CreateRectRgnIndirect(&Rect);
							InvalidateRgn(&rgn); 

							// on redessine la fenêtre
							this->InvalidateRect(Rect, FALSE);
							rgn.DeleteObject();
						}
				
						// Indique changement surlignage effectué
						m_RowColChange = false;

						// Mise à jour dernier changement
						m_LastTimeUpdate = GetTickCount();

						m_InvalidateAllOk = false;
					}
				}
			}
		}
		else
		{
			m_LastTimeUpdate = GetTickCount();
			m_InvalidateAllOk = false;
		}
	}
}


BOOL CAnCrossTable::OnMouseMoveOver(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt)
{
	// Appel mouse move composant
	m_pItemTable->OnMouseMoveOver(nRow, nCol, nFlags, pt);
	
	return CGXGridWnd::OnMouseMoveOver(nRow, nCol, nFlags, pt);
}


BOOL CAnCrossTable::OnGridKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	return CGXGridWnd::OnGridKeyDown(nChar, nRepCnt, nFlags);
}


void CAnCrossTable::AfficheInfoCellule(ROWCOL nRow, ROWCOL nCol, CString ValReelle, bool AffichOk)
{
	
	// Modif Juillet 2013
	// Attention, l'affichage du tooltip génére des scintillements  (A VOIR OPTIM 2.7)
	this->ActivateTooltip(FALSE);
	if (AffichOk)
	{
		// Affichage info supplémentaire cellule (par exemple marge d'erreur réelle)
		this->ActivateTooltip(TRUE);
		this->SetTooltipText(ValReelle);
		this->EnableBalloonTooltip();
		// this->ActivateTooltip(TRUE);
	}
	else
	{
		// Désaffiche le tooltip
		this->ActivateTooltip(FALSE);
	}
}

////////////////////////////////////////////////////////////////////////////
// Renvoi coordonnées basses via ligne
void CAnCrossTable::CalcLimitTopBottomRow(ROWCOL nRow, int &Top, int &Bottom)
{
	Top		= this->CalcTopFromClientRow(nRow); 
	Bottom  = Top + this->GetRowHeight(nRow); 
}

BOOL CAnCrossTable::Copy( )
{
	CWaitCursor wait;

	//Implementer ici une barre de défilement
	return m_pItemTable->CopyToClipboard();
}

void CAnCrossTable::InitToolTip()
{
	if (m_ToolTip.m_hWnd == NULL)
	{
		// Create ToolTip control
		m_ToolTip.Create(this, m_dwToolTipStyle);
		// Create inactive
		m_ToolTip.Activate(FALSE);
		// Enable multiline
		m_ToolTip.SendMessage(TTM_SETMAXTIPWIDTH, 0, 400);
		m_ToolTip.SetDelayTime(300);
	} 
}

// This function sets the text to show in the button tooltip.
//
// Parameters:
//		[IN]	lpszText
//				Pointer to a null-terminated string containing the text to show.
//		[IN]	bActivate
//				If TRUE the tooltip will be created active.
//
void CAnCrossTable::SetTooltipText(LPCTSTR lpszText, BOOL bActivate)
{
	// We cannot accept NULL pointer
	if (lpszText == NULL) return;

	// Initialize ToolTip
	InitToolTip();

	// If there is no tooltip defined then add it
	if (m_ToolTip.GetToolCount() == 0)
	{
		CRect rectBtn; 
		GetClientRect(rectBtn);
		m_ToolTip.AddTool(this, lpszText, rectBtn, 1);
	}

	// Set text for tooltip
	m_ToolTip.UpdateTipText(lpszText, this, 1);
	m_ToolTip.Activate(bActivate);
} 

// This function sets the text to show in the button tooltip.
//
// Parameters:
//		[IN]	nText
//				ID number of the string resource containing the text to show.
//		[IN]	bActivate
//				If TRUE the tooltip will be created active.
//
void CAnCrossTable::SetTooltipText(int nText, BOOL bActivate)
{
	CString sText;

	// Load string resource
	sText.LoadString(nText);
	// If string resource is not empty
	if (sText.IsEmpty() == FALSE) SetTooltipText((LPCTSTR)sText, bActivate);
} 


// This function enables or disables the button tooltip.
//
// Parameters:
//		[IN]	bActivate
//				If TRUE the tooltip will be activated.
//
void CAnCrossTable::ActivateTooltip(BOOL bActivate)
{
	// If there is no tooltip then do nothing
	if (m_ToolTip.GetToolCount() == 0) return;

	// Activate tooltip
	m_ToolTip.Activate(bActivate);
} 

// This function enables the tooltip to be displayed using the balloon style.
// This function must be called before any call to SetTooltipText is made.
//
// Return value:
//		BTNST_OK
//			Function executed successfully.
//
DWORD CAnCrossTable::EnableBalloonTooltip()
{
	m_dwToolTipStyle |= TTS_BALLOON;
	return true;
} 

BOOL CAnCrossTable::PreTranslateMessage(MSG* pMsg) 
{
	InitToolTip();
	m_ToolTip.RelayEvent(pMsg);
	
	if (pMsg->message == WM_LBUTTONDBLCLK)
		pMsg->message = WM_LBUTTONDOWN;

	return CGXGridWnd::PreTranslateMessage(pMsg);
} // End of PreTranslateMessage


BOOL CAnCrossTable::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	return CGXGridWnd::OnSetCursor(pWnd, nHitTest, message);
}

