#include "StdAfx.h"
#include "Dupgridwnd.h"
#include "DlgDuplication.h"
#include "AllTypeOfIterator.h"
#include "AllTypeOfVisitor.h"
#include "IDLimits.h"
#include <afxadv.h>            // For CSharedFile


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////////////////////////
// Constructeur
CDupGridWnd::CDupGridWnd(void)
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
CDupGridWnd::~CDupGridWnd(void)
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


void CDupGridWnd::SortKeys(const CGXSortInfoArray& sortInfo, CPtrArray& sortKeyArray)
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
JBool CDupGridWnd::OnInitDocument(JATPDocChoixSupport *pDoc, CDlgDuplication *pDlg, JUnt32 &NoColIdSupport)
{
	// R�cup�re la fen�tre m�re
	m_pDlgGrille	   = pDlg;

	// Test si valide
	if (OnInitDoc(pDoc, NoColIdSupport) && m_pDlgGrille != NULL)
		return true;
	else
		return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Gestion s�lection cellule du grid
BOOL CDupGridWnd::OnLButtonClickedRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt)
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

	// Test si on est sur une cellule format support
	else if (nRow >= NbRowHeader && nCol == m_pDlgGrille->m_NoColFormat-1)
	{
		LockUpdate(false);
		return false;
	}

	// Test si c'est une ligne support (donc ligne >= m_pDlgGrille->m_NbHeaderRow && col == 0)
	else if (nRow >= NbRowHeader  && 
		    (nCol == (m_pDlgGrille->m_NoColSelSupport-1) || nCol == (m_pDlgGrille->m_NoColSupport-1)))
	{
		// Ident support de la ligne
		JUnt32 IdSupportRow = (JUnt32) atoi(GetValueRowCol(nRow, m_pDlgGrille->m_NoColIdSupport-1));

		// On se positionne sur l'�lmt map s�lection support
		m_pDocChoixSupport->m_MapIdSupportsSel.MoveTo(IdSupportRow);

		// Si valide, on change �tat s�lection
		if (m_pDocChoixSupport->m_MapIdSupportsSel.IsValid())
		{
			// R�cup�re ligne et colonne s�lectionn�es
			JBool &EtatSupport = m_pDocChoixSupport->m_MapIdSupportsSel.GetItem();
			EtatSupport = !EtatSupport;

			// Mise � jour des supports s�lectionn�s
			m_pDocChoixSupport->UpdateAllViews(UPDATE_SEL_SUPPORTS); 
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

	// Si on clicke sur la colonne ligne NbRowHeader-1 >> Tri des colonnes
	else if (nRow == NbRowHeader - 1)
	{
		// Tri sur la colonne (par d�faut avec permutation via tri pr�c�dent)
		SortColonne(nCol);
	}

	else if (nRow < NbRowHeader)
	{
		// Devalidation de toutes les cellules
		SelectRange(CGXRange(0,0,NbRowHeader-1,GetColCount()), FALSE, FALSE);
		Redraw();
	}
	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Gestion Bouton Droit pour Menu PopPup
BOOL CDupGridWnd::OnRButtonClickedRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt)
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

//////////////////////////////////////////////////////////////////////////////
// Tri sur une colonne du grille �chelle
JVoid CDupGridWnd::SortColonne(ROWCOL nCol, JBool PermutTri)
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
// Tri sur une colonne du grille duplication
JVoid CDupGridWnd::SortColonneCourante(JBool PermutTri)
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
void CDupGridWnd::TrierColonne(JUnt32 NoColonne, JBool PermutOrdre)
{
	// Debloque modif cellules
	SetReadOnly(FALSE); 

	// Le document a chang�
	m_pDocChoixSupport->SetModifiedFlag(TRUE); 

	// Colonne � trier
	CRowColArray ArrayCols;

	// Ajoute No Colonne en tant que colonne de tri
	ArrayCols.Add(NoColonne);
	ROWCOL nCount = 1;

	// On tri selon s�lection colonne en cours
	if (nCount)
	{
		// Specifie information tri
		CGXSortInfoArray sortInfo;
		sortInfo.SetSize(ArrayCols.GetCount());							// keys only (you can also have more keys)

		// Info Tri pr�c�dent
		CInfoTriCol &InfoTriCol = m_pDocChoixSupport->m_TabInfoTriCol[m_pDocChoixSupport->ModuleDup];

		// Ordre de tri � effectuer
		bool Croissant = true;
		// if (NoColonne == InfoTriCol.NoCol)
		int NoCol = ArrayCols[0];

		if (ArrayCols[0] == InfoTriCol.NoCol)
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
			sortInfo[n].nRC			= ArrayCols[n]; 					// column nCol is the key

			if (NoColonne == m_pDlgGrille->m_NoColSelSupport || NoColonne == m_pDlgGrille->m_NoColSupport)
			{	
				// Tri alphab�tique
				sortInfo[n].sortType	= CGXSortInfo::autodetect;
			}
			else
			{
				// tri num�rique
				sortInfo[n].sortType	= CGXSortInfo::alphanumeric;
			}

			// Tri croissant
			if (Croissant == true)
				sortInfo[n].sortOrder	= CGXSortInfo::ascending;		// sort ascending
			else
				sortInfo[n].sortOrder	= CGXSortInfo::descending;		// sort ascending
		}


		// Appel tri automatique du grid
		SortRows(CGXRange().SetTable(), sortInfo,GX_UPDATENOW );
	}

	// Debloque modif cellules
	SetColReadOnly(m_pDlgGrille->GetNbHeaderRow(), m_pDlgGrille->m_NoColIdSupport,m_pDlgGrille->m_NoColSupport, m_pDlgGrille->m_NoColPeriodicite);

	// R�cup derniere colonne tri�e
	m_NoColTri = NoColonne;

	// Mise � jour des supports s�lectionn�s
	m_pDocChoixSupport->UpdateAllViews(UPDATE_VISIBLE_SUPPORTS); 
		
	// Attente mise � jour
	LockUpdate(false);
	Redraw();
}

void CDupGridWnd::OnModifyCell (ROWCOL nRow, ROWCOL nCol)
{
	// Nombre de lignes header
	JUnt32 NbRowHeader = m_pDlgGrille->GetNbHeaderRow();

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
				m_pDocChoixSupport->m_MapIdFormatSpecifDup.MoveTo(IdSupport);
				if (m_pDocChoixSupport->m_MapIdFormatSpecifDup.IsValid())
				{
					// On modifie l'ident format associ� au support
					JUnt32 &IdFormat = m_pDocChoixSupport->m_MapIdFormatSpecifDup.GetItem();
					IdFormat = IdFormatSel;
				}
			}
		
			// Le document a chang�
			m_pDocChoixSupport->SetModifiedFlag(TRUE); 

			// Mise � jour des tarifs supports et r�sultats aff�rant aux tarifs
			m_pDocChoixSupport->UpdateAllViews(UPDATE_SEL_FORMATSUPPORT);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copie des informations grid duplication
BOOL CDupGridWnd::Copy( )
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

	// Export Grille Duplication
	ClipString = "BLOC 2\t";

	// Type de r�sultat
	CString TxtResultat = "";
	m_pDocChoixSupport->m_MapIdGrandDupSel.MoveFirst();
	if (m_pDocChoixSupport->m_MapIdGrandDupSel.IsValid())
	{
		TxtResultat = m_pDocChoixSupport->GetGrandeur(m_pDocChoixSupport->ModuleDup,m_pDocChoixSupport->m_MapIdGrandDupSel.GetItem());  
	}
	ClipString += TxtResultat + "\n";
	
	// Export du grille r�sultat
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

