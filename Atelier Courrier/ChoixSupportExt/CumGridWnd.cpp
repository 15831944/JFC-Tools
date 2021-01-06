#include "StdAfx.h"
#include "CumGridWnd.h"
#include "DlgCumulativite.h"
#include "AllTypeOfIterator.h"
#include "AllTypeOfVisitor.h"
#include "IDLimits.h"
#include <afxadv.h>            // For CSharedFile


#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////////////////////////////
// Constructeur
CCumGridWnd::CCumGridWnd(void)
{
	// Mode clipboard
	m_nClipboardFlags = GX_DNDTEXT|GX_DNDCOLHEADER|GX_DNDROWHEADER ;

	// D�shinibe la souris
	m_bDisableMouseMoveOver = FALSE;

	// Init colonne tri
	m_NoColTri = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Destructeur
CCumGridWnd::~CCumGridWnd(void)
{
}

/* A REMETTRE
extern "C" static int __cdecl __compare(const void* p1, const void* p2) 
{
	//Redefinition de la m�thode de classement utilis� par le qsort de la grille

	CGXSortKey* pKey1 = *((CGXSortKey**) p1);
	CGXSortKey* pKey2 = *((CGXSortKey**) p2);
	const CGXSortInfoArray* pSortInfo = pKey1->pSortInfo;

	for (int i = 0; i < pSortInfo->GetSize(); i++)
	{
		const CGXSortInfo& keyinfo = pSortInfo->GetAt(i);

		if (keyinfo.sortType == CGXSortInfo::date)
		{
			// date comparison

#ifdef OLEDATE_SUPPORT
			COleDateTime d1 = pKey1->darray[i];
			COleDateTime d2 = pKey2->darray[i];
#else
			ASSERT(0); // for 16-Bit this should be alphanumeric sort
			double d1 = 0, d2 = 0;
#endif

			if (d1 == d2)
				continue;

			if (d1 > d2)
				return keyinfo.sortOrder == CGXSortInfo::ascending ? 1 : -1;
			else
				return keyinfo.sortOrder == CGXSortInfo::ascending ? -1 : 1;
		}
		else if (keyinfo.sortType == CGXSortInfo::numeric)
		{
			// numeric comparison
			double d1 = pKey1->darray[i];
			double d2 = pKey2->darray[i];

			if (d1 == d2)
				continue;

			if (d1 > d2)
				return keyinfo.sortOrder == CGXSortInfo::ascending ? 1 : -1;
			else
				return keyinfo.sortOrder == CGXSortInfo::ascending ? -1 : 1;
		}
		else
		{
			// alphanumeric comparison
			int n = 0;
			n = CCompareTitre::CompareTitre(pKey1->sKeys[i], pKey2->sKeys[i]);
//
//#if _MFC_VER >= 0x0400
//			if (keyinfo.bCase)
//				// TODO: maybe add flag for "NLS" sort, e.g. keinfo.bNLS
//				// instead of commenting out Compare and CompareNoCase
//				// n = pKey1->sKeys[i].Compare(pKey2->sKeys[i]);
//				n = _tcscoll(pKey1->sKeys[i], pKey2->sKeys[i]);
//			else
//				// n = pKey1->sKeys[i].CompareNoCase(pKey2->sKeys[i]);
//				n = _tcsicoll(pKey1->sKeys[i], pKey2->sKeys[i]);
//#else
//			if (keyinfo.bCase)
//				n = pKey1->sKeys[i].Compare(pKey2->sKeys[i]);
//			else
//				n = pKey1->sKeys[i].CompareNoCase(pKey2->sKeys[i]);
//#endif
//
			if (n != 0)
				return keyinfo.sortOrder == CGXSortInfo::ascending ? n : -n;
		}
	}

	// values are equal - return ids

	return (int) (pKey1->nId - pKey2->nId);
}

void CCumGridWnd::SortKeys(const CGXSortInfoArray& sortInfo, CPtrArray& sortKeyArray)
{

	//Red�finition de la m�those de classement de la grille
	//qsort
	//	CGXGridWnd::SortKeys(sortInfo, sortKeyArray);


#if _MFC_VER < 0x0400
	CGXPtrArray* ap = (CGXPtrArray*) &sortKeyArray;
	qsort(ap->GetData(), ap->GetSize(), sizeof(void*), __compare);
#else
	qsort(sortKeyArray.GetData(), sortKeyArray.GetSize(), sizeof(void*), __compare);
#endif	
}
*/

//////////////////////////////////////////////////////////////////////////////////////////////
// Init document choix support associ�
JBool CCumGridWnd::OnInitDocument(JATPDocChoixSupport *pDoc, CDlgCumulativite *pDlg, JUnt32 &NoColIdSupport)
{
	// R�cup�re la fen�tre m�re
	m_pDlgGrille		 = pDlg;

	// Test si valide
	if (OnInitDoc(pDoc, NoColIdSupport) && m_pDlgGrille != NULL)
		return true;
	else
		return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Gestion s�lection cellule du grid
BOOL CCumGridWnd::OnLButtonClickedRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt)
{
	// Nombre de lignes header
	JUnt32 NbRowHeader = m_pDlgGrille->GetNbHeaderRow();

	if ((nRow == 0) && (nCol == m_pDlgGrille->m_NoColSelSupport-1))
	{
		// V�rifie la s�lection courante
		CRowColArray awCols;
		ROWCOL nCount = GetSelectedCols(awCols);
		
		if (nCount == GetColCount())
		{
			// D�validation de toutes les cellules
			SelectRange(CGXRange(0,0,GetRowCount(),GetColCount()), FALSE, TRUE);
		}
		else
		{
			// Validation de toutes les cellules en vues d'un clipboard global
			SelectRange(CGXRange(0,0,GetRowCount(),GetColCount()), TRUE, TRUE);
		}
	}
	
	/*
	// Test si on est sur une cellule format support
	else if (nRow >= NbRowHeader && nCol == m_pDlgGrille->m_NoColFormat-1)
	{
		LockUpdate(false);
		return false;
	}
	*/

	//Test si c'est une ligne support
	else if (nRow >= NbRowHeader)
	{
		// Ident support de la ligne
		JUnt32 IdSupport = (JUnt32) atoi(GetValueRowCol(nRow, m_pDlgGrille->m_NoColIdSupport-1));

		// Va sur le support concern�
		m_pDocChoixSupport->m_MapIdSupportsSel.MoveTo(IdSupport);
		m_pDocChoixSupport->m_MapIdColorSupports.MoveTo(IdSupport);

		if (m_pDocChoixSupport->m_MapIdSupportsSel.IsValid())
		{
			// Test si click sur case � cocher s�lection
			if (nCol == (m_pDlgGrille->m_NoColSupport-1) || nCol == (m_pDlgGrille->m_NoColSelSupport-1))
			{
				// R�cup�re ligne et colonne s�lectionn�es (niveau s�lection)
				JBool &EtatSupport = m_pDocChoixSupport->m_MapIdSupportsSel.GetItem();
				EtatSupport = !EtatSupport;

				// Mise � jour des supports s�lectionn�s
				m_pDocChoixSupport->UpdateAllViews(UPDATE_SEL_SUPPORTS); 

				// Le document a chang�
				m_pDocChoixSupport->SetModifiedFlag(TRUE); 
			}

			else if (nCol == m_pDlgGrille->m_NoColColorGraphe-1)
			{
				// S�lection ou d�selection couleur
				MajColorGraphe(nRow,nCol);
			}	
		}
	}

	// S�lection ou d�selection globale de tous les supports
	else if (nRow == NbRowHeader - 2 &&  nCol == m_pDlgGrille->m_NoColSelSupport-1)
	{
		// Validation de toutes les cellules en vues d'un clipboard global
		SelectRange(CGXRange(0,0,GetRowCount(),GetColCount()), FALSE, FALSE);

		// R�cup�re �tat du 1er support
		m_pDocChoixSupport->m_MapIdSupportsSel.MoveFirst();
		if (m_pDocChoixSupport->m_MapIdSupportsSel.IsValid())
		{
			// Attente mise � jour
			LockUpdate(true);

			// Etat du 1er support
			JBool &EtatSupport = m_pDocChoixSupport->m_MapIdSupportsSel.GetItem();

			// Test si valide
			if (!EtatSupport)
			{
				// On balaye tous les supports et on valide leur s�lection
				for (m_pDocChoixSupport->m_MapIdSupportsSel.MoveFirst();
					 m_pDocChoixSupport->m_MapIdSupportsSel.IsValid();
					 m_pDocChoixSupport->m_MapIdSupportsSel.MoveNext())
				{
					// R�cup�re �tat du support
					JBool &EtatSupport = m_pDocChoixSupport->m_MapIdSupportsSel.GetItem();
	
					// force la s�lection
					EtatSupport = true;
				}
		
				// Mise � jour des supports s�lectionn�s
				m_pDocChoixSupport->UpdateAllViews(UPDATE_SEL_SUPPORTS); 
		
				// Le document a chang�
				m_pDocChoixSupport->SetModifiedFlag(TRUE); 
			}
			else
			{
				// On balaye tous les supports et on d�valide leur s�lection
				for (m_pDocChoixSupport->m_MapIdSupportsSel.MoveFirst();
					 m_pDocChoixSupport->m_MapIdSupportsSel.IsValid();
					 m_pDocChoixSupport->m_MapIdSupportsSel.MoveNext())
				{
					// R�cup�re �tat du support
					JBool &EtatSupport = m_pDocChoixSupport->m_MapIdSupportsSel.GetItem();
		
					// force la d�s�lection
					EtatSupport = false;
				}
		
				// Mise � jour des supports s�lectionn�s
				m_pDocChoixSupport->UpdateAllViews(UPDATE_SEL_SUPPORTS); 
		
				// Le document a chang�
				m_pDocChoixSupport->SetModifiedFlag(TRUE); 
			}

			// Attente mise � jour
			LockUpdate(false);
			Redraw();
		}
	}

	//////////////////////////////////// TRI DES COLONNES ///////////////////////////////////////////
	else
	{
		// Si on clicke sur la colonne ligne NbRowHeader-1
		if (nRow == NbRowHeader - 1 && nCol != (m_pDlgGrille->m_NoColColorGraphe-1))
		{
			// Attente mise � jour
			LockUpdate(true);

			// Flag affichage prct
			JBool AffichPrct = false;

			// Modifie le contenu pour trier
			FormatCellTri(nCol, m_pDlgGrille->GetNbHeaderRow(), m_pDlgGrille->m_NoColPeriodicite, AffichPrct);

			// Tri de la colonne
			TrierColonne(nCol, true);

			// Remet affichage de base
			FormatCellFinal(nCol, m_pDlgGrille->GetNbHeaderRow(), m_pDlgGrille->m_NoColPeriodicite, AffichPrct);

			// Attente mise � jour
			LockUpdate(false);
			Redraw();
		}

		else if (nRow < NbRowHeader)
		{
			// Devalidation de toutes les cellules
			SelectRange(CGXRange(0,0,NbRowHeader-1,GetColCount()), FALSE, FALSE);
			Redraw();
		}
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Gestion Bouton Droit pour Menu PopPup
BOOL CCumGridWnd::OnRButtonClickedRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt)
{
	//que si on clique sur une case libell� support
	if (nRow >= m_pDlgGrille->GetNbHeaderRow() && nCol == (m_pDlgGrille->m_NoColSupport-1))
	{
		
		// R�cup�ration de l'id support s�lectionn�
		JUnt32 IdSupport = (JUnt32) atoi(GetValueRowCol(nRow, m_pDlgGrille->m_NoColIdSupport-1));

		// Test si ligne correspond � 1 support
		if (IdSupport == 0) return FALSE;

		// On se positionne sur ce support dans le map g�n�ral des supports
		m_pDocChoixSupport->m_MapIdSupportsSel.MoveTo(IdSupport);

		// R�cup coordonn�e pour menu pop pup
		ClientToScreen(&pt);

		//si on clik sur un element de plateau
		if (m_pDocChoixSupport->m_MapIdSupportsSel.IsValid() && m_pDocChoixSupport->m_MapIdSupportsSel.GetItem() == 1)
		{
			// Menu popup avec acc�s familles support
			AffichPopupMenu(IdSupport, pt, true);
		}
		else
		{
			// Menu popup avec acc�s familles support
			AffichPopupMenu(IdSupport, pt, false);
		}
	}
	return true;
}

void CCumGridWnd::OnModifyCell (ROWCOL nRow, ROWCOL nCol)
{
	// Nombre de lignes header
	JUnt32 NbRowHeader = m_pDlgGrille->GetNbHeaderRow();

	/*
	// Test si on est sur une cellule format support
	if (nRow >= NbRowHeader && nCol == m_pDlgGrille->m_NoColFormat-1)
	{
		CGXControl *pCtrl = GetControl(nRow,nCol);
		if(pCtrl && pCtrl->IsKindOf(RUNTIME_CLASS(CGXComboBoxWnd)))
		{
			// Ident support de la ligne
			JUnt32 IdSupport = (JUnt32) atoi(GetValueRowCol(nRow, m_pDlgGrille->m_NoColIdSupport-1));

			// Index de l'�l�ment format click�
			CString LibFormat = "";
			if (pCtrl->GetSelectedText(LibFormat))
			{
				// Balaye le map des formats possibles
				JUnt32 IdFormatSel = m_pDocChoixSupport->GetIdFormat(LibFormat,true);

				// On se positionne sur le map au niveau du support
				m_pDocChoixSupport->m_MapIdFormatSpecifCum.MoveTo(IdSupport);
				if (m_pDocChoixSupport->m_MapIdFormatSpecifCum.IsValid())
				{
					// On modifie l'ident format associ� au support
					JUnt32 &IdFormat = m_pDocChoixSupport->m_MapIdFormatSpecifCum.GetItem();
					IdFormat = IdFormatSel;
				}
			}
		
			// Le document a chang�
			m_pDocChoixSupport->SetModifiedFlag(TRUE); 

			// Mise � jour des tarifs supports et r�sultats aff�rant aux tarifs
			m_pDocChoixSupport->UpdateAllViews(UPDATE_SEL_FORMATSUPPORT);
		}
	}
	*/
}

//////////////////////////////////////////////////////////////////////////////
// Tri sur une colonne du grille cumulativit�
JVoid CCumGridWnd::SortColonne(ROWCOL nCol, JBool PermutTri)
{
	// Flag affichage prct
	JBool AffichPrct = false;

	// Modifie le contenu pour trier
	FormatCellTri(nCol, m_pDlgGrille->GetNbHeaderRow(), m_pDlgGrille->m_NoColPeriodicite, AffichPrct);

	// Tri de la colonne
	TrierColonne(nCol,PermutTri);

	// Remet affichage de base
	FormatCellFinal(nCol, m_pDlgGrille->GetNbHeaderRow(), m_pDlgGrille->m_NoColPeriodicite, AffichPrct);

	// Le document a chang�
	m_pDocChoixSupport->SetModifiedFlag(TRUE); 

	// Mise � jour no colonne tri courant
	m_NoColTri = nCol;
}

//////////////////////////////////////////////////////////////////////////////
// Tri sur une colonne du grille cumulativit�
JVoid CCumGridWnd::SortColonneCourante(JBool PermutTri)
{
	// Flag affichage prct
	JBool AffichPrct = false;

	// Modifie le contenu pour trier
	FormatCellTri(m_NoColTri, m_pDlgGrille->GetNbHeaderRow(), m_pDlgGrille->m_NoColPeriodicite, AffichPrct);

	// Tri de la colonne
	TrierColonne(m_NoColTri,PermutTri);

	// Remet affichage de base
	FormatCellFinal(m_NoColTri, m_pDlgGrille->GetNbHeaderRow(), m_pDlgGrille->m_NoColPeriodicite, AffichPrct);

	// Le document a chang�
	m_pDocChoixSupport->SetModifiedFlag(TRUE); 
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Tri d'une colonne
void CCumGridWnd::TrierColonne (JUnt32 NoColonne, JBool PermutOrdre)
{

	// Debloque modif cellules
	SetReadOnly(FALSE); 

	// Le document a chang�
	m_pDocChoixSupport->SetModifiedFlag(TRUE); 

	// Colonne � trier
	CRowColArray ArrayCols;
	ROWCOL nCount = GetSelectedCols(ArrayCols);

	if (nCount)
	{
		// Specifie information tri
		CGXSortInfoArray sortInfo;
		sortInfo.SetSize(ArrayCols.GetCount());						// keys only (you can also have more keys)

		// Info Tri pr�c�dent
		CInfoTriCol &InfoTriCol = m_pDocChoixSupport->m_TabInfoTriCol[m_pDocChoixSupport->ModuleCum];

		// Ordre de tri � effectuer
		bool Croissant = true;
		if (NoColonne == InfoTriCol.NoCol)
		{
			if (PermutOrdre == true)
			{
				// On permute l'ordre de tri
				if (InfoTriCol.TriCroissant == true)
					Croissant = false;
				else
					Croissant = true;
			}
			else
			{
				// On garde le m�me ordre (pour mise � jour uniquement)
				if (InfoTriCol.TriCroissant == true)
					Croissant = true;
				else
					Croissant = false;

			}
		}

		// Sauve nouvelle information tri
		InfoTriCol.NoCol		= NoColonne;
		InfoTriCol.TriCroissant = Croissant; 

		// Mise en place param�tre tri 
		for (ROWCOL n = 0; n < nCount; n++)
		{
			sortInfo[n].nRC			= ArrayCols[n]; 				// column nCol is the key

			if (NoColonne < m_pDlgGrille->m_NoColPeriodicite)
				// Tri automatique
				sortInfo[n].sortType	= CGXSortInfo::autodetect;		
	
			else
				// Tri alpha-num�rique
				sortInfo[n].sortType	= CGXSortInfo::alphanumeric;		
			
			// Tri croissant ou d�croissant selon info tri courant
			if (Croissant)
				sortInfo[n].sortOrder	= CGXSortInfo::ascending;		// sort ascending
			else
				sortInfo[n].sortOrder	= CGXSortInfo::descending;		// sort descending

		}

		// Appel tri automatique du grid
		SortRows(CGXRange().SetTable(), sortInfo,GX_UPDATENOW );
	}

	// R�cup derniere colonne tri�e
	m_NoColTri = NoColonne;

	// Mise � jour des supports s�lectionn�s
	m_pDocChoixSupport->UpdateAllViews(UPDATE_VISIBLE_SUPPORTS); 

	// Debloque modif cellules
	SetReadOnly(TRUE); 
}

//////////////////////////////////////////////////////////////////////////////////////////////
// S�lection ou d�selection couleur
void CCumGridWnd::	MajColorGraphe(ROWCOL nRow, ROWCOL nCol)
{
	// Nombre de lignes header
	JUnt32 NbRowHeader = m_pDlgGrille->GetNbHeaderRow();

	//Test si c'est une ligne support
	if (nRow >= NbRowHeader)
	{
		// Ident support de la ligne
		JUnt32 IdSupport = (JUnt32) atoi(GetValueRowCol(nRow, m_pDlgGrille->m_NoColIdSupport-1));

		// Va sur le support concern�
		m_pDocChoixSupport->m_MapIdSupportsSel.MoveTo(IdSupport);
		m_pDocChoixSupport->m_MapIdColorSupports.MoveTo(IdSupport);

		if (m_pDocChoixSupport->m_MapIdSupportsSel.IsValid())
		{
			// Test si click sur colonne support pour validation support dans graphe
			if (nCol == m_pDlgGrille->m_NoColColorGraphe-1) 
			{
				// R�cup�re couleur support en cours
				JUnt32 &ColorSupport = m_pDocChoixSupport->m_MapIdColorSupports.GetItem();

				// Test couleur support
				if (ColorSupport > 0)
				{
					// D�j� s�lectionner , on le d�selection en mettant la couleur � 0
					ColorSupport = 0;

					// Mise � jour d�s�lection couleur supports
					//m_pDocChoixSupport->UpdateAllViews(UPDATE_SEL_SUPPORTS);
					m_pDocChoixSupport->UpdateAllViews(UPDATE_SEL_COLOR_GRAPHSUPP);

					// Le document a chang�
					m_pDocChoixSupport->SetModifiedFlag(TRUE); 

					// Recalcul la trame horizontale du graphe
					m_pDlgGrille->UpdateGraphe();
				}

				else
				{
					if (m_pDocChoixSupport->m_IdColorCum > 0)
					{
						// Affecte la nouvelle couleur
						ColorSupport = m_pDocChoixSupport->m_IdColorCum;

						// Mise � jour des supports s�lectionn�s (niveau couleur)
						m_pDocChoixSupport->UpdateAllViews(UPDATE_SEL_COLOR_GRAPHSUPP);

						// Le document a chang�
						m_pDocChoixSupport->SetModifiedFlag(TRUE); 

						// Recalcul la trame horizontale du graphe
						m_pDlgGrille->UpdateGraphe();
					}
					else
						// Attention aucune couleur s�lectionn�e
						AfxMessageBox("Attention s�lectionner d'abord une couleur",MB_ICONINFORMATION);
				}					
			}				
		}
	}
}

/*
//////////////////////////////////////////////////////////////////////////////////////////////
// Gestion mouvement mouse sur le grid
BOOL CCumGridWnd::OnMouseMoveOver(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt)
{
	CString MessRowCol;
	MessRowCol.Format("%s %d %s %d","Mouse on Ligne",nRow," / Colonne",nCol);  
	AfxMessageBox(MessRowCol);

	if (nRow < GetRowCount() && nCol < GetColCount())
		return TRUE;
	else
		return FALSE;
}
*/

///////////////////////////////////////////////////////////////////////////////////////
// Bidouille blocage 1ere colonne
BOOL CCumGridWnd::OnMouseMoveOver(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt)
{
	if (nCol < 2)
		this->HideCols(0,0);
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copie des informations grid cumulativit�
BOOL CCumGridWnd::Copy( )
{
	CWaitCursor wait;

	// S�lection lignes et colonnes
	CRowColArray Rows,Cols;
	ROWCOL NbCols = GetSelectedCols(Cols,false,false);
	ROWCOL NbRows = GetSelectedRows(Rows,false,false);

	// Pas de s�lection valide, on sort
	if(NbRows == 0 && NbCols == 0)
	{	
		AfxMessageBox("Pas de copier-coller possible sans s�lection",MB_ICONINFORMATION);
		return false;
	}
	
	// Fichier temporaire clipboard
	CSharedFile clipb (GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);

	// Entete Information
	CString ClipString		= _T(" ");
	m_pDlgGrille->RecupEnteteExport(ClipString); 
	clipb.Write(ClipString.GetBuffer(),ClipString.GetLength());

	// Export Grille Cumulation
	ClipString = "BLOC 2\t";

	// Type de r�sultat
	CString TxtResultat = "";
	m_pDocChoixSupport->m_MapIdGrandCumSel.MoveFirst();
	if (m_pDocChoixSupport->m_MapIdGrandCumSel.IsValid())
	{
		TxtResultat = m_pDocChoixSupport->GetGrandeur(m_pDocChoixSupport->ModuleCum,m_pDocChoixSupport->m_MapIdGrandCumSel.GetItem());  
	}
	ClipString += TxtResultat + "\n";

	clipb.Write(ClipString.GetBuffer(),ClipString.GetLength());
	ClipString	= _T(" ");
	m_pDlgGrille->RecupGrilleExport(ClipString); 
	clipb.Write(ClipString.GetBuffer(),ClipString.GetLength());

	// R�cup�ration du bloc infos terrains / cibles
	ClipString	= _T(" ");
	m_pDlgGrille->RecupBlocTerrainCible(ClipString);
	clipb.Write(ClipString.GetBuffer(),ClipString.GetLength());

	// Passe � la ligne suivante
	ClipString = _T("\n");
	clipb.Write(ClipString.GetBuffer(),ClipString.GetLength());

	// Bloc n� 4 : D�tail groupes
	// Si on est en mode group�, export contenu des groupes
	if (m_pDocChoixSupport->m_ModeColorGraphe == 1)
	{
		ClipString	= _T(" ");
		m_pDlgGrille->RecupContenuGroupe(ClipString);
		clipb.Write(ClipString.GetBuffer(),ClipString.GetLength());
	}

	// Test op�ration Clipboard
	VERIFY(OpenClipboard());
	BOOL bEmpty = EmptyClipboard();
	VERIFY(CloseClipboard());

	// Lance la copie clipboard
	HGLOBAL hData = 0;
	HANDLE hResult = 0;
	hData = clipb.Detach();

	// Now, hand over memory block to clipboard
	if (bEmpty && hData)
	{
		VERIFY(OpenClipboard());
		hResult = SetClipboardData(CF_TEXT, hData);
		VERIFY(CloseClipboard());
	}

	return true;
}
