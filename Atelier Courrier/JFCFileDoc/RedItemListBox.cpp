#include "StdAfx.h"
#include "RedItemListBox.h"
#include "DlgCible.h"
#include "JATPOffre.h"

CRedItemListBox::CRedItemListBox(void)
{
	m_pchTip = NULL;
	m_pwchTip = NULL;
}

CRedItemListBox::~CRedItemListBox(void)
{
}

BEGIN_MESSAGE_MAP(CRedItemListBox, JFC_MultiListBox<IItem*>)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Renvoi niveau d'interdiction d'un élément (item ou palette) à l'utilisation
// 0 - Aucune interdiction
// 1 - Interdiction partielle (seulement interdite sur certains terrains
// 2 - Interdiction totale (interdit sur tous les terrains)
JUnt32 CRedItemListBox::DegreInterdit(int ID) const
{
	JUnt32 DegreInterdit = CDlgCible::Item_Interdit_Aucun;
	
	if (m_pDlgParent->m_pItems->GetIItemByID(ID)->IsPalette())
		DegreInterdit = DegrePaletteInterdit(ID);
	else
		DegreInterdit = DegreItemInterdit(ID);

	return DegreInterdit;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Renvoi niveau d'interdiction de la palette à l'utilisation
// 0 - Aucune interdiction
// 1 - Interdiction partielle (seulement interdite sur certains terrains
// 2 - Interdiction totale (interdit sur tous les terrains)
JUnt32 CRedItemListBox::DegrePaletteInterdit(int ID) const
{
	// Par défaut palette OK
	JUnt32 DegreInterdit = CDlgCible::Item_Interdit_Aucun;

	// Récupère la palette
	CPalettes * palette = m_pDlgParent->m_pItems->GetIItemByID(ID)->AsPalette();
	if(palette)
	{
		// Init le nombre d'interdictions partielles trouvées et le nombre d'interdictions totales
		JUnt32 NbInterditPartiel	= 0;
		JUnt32 NbInterditTotal		= 0;

		// Récupère les composants items de la palette
		JList<JInt32> idList;
		palette->GetItems(idList);
		for (idList.MoveFirst(); idList.IsValid(); idList.MoveNext())
		{
			// Test si item composant avec interdiction partielle
			if (DegreItemInterdit(idList.GetItem()) == CDlgCible::Item_Interdit_Partiel)
			{
				NbInterditPartiel++;
			}
			// Test si item composant avec interdiction totale
			else if (DegreItemInterdit(idList.GetItem()) == CDlgCible::Item_Interdit_Total)
			{
				NbInterditTotal++; 
			}
		}

		// Test si interdiction totale
		if (NbInterditTotal == idList.GetCount())
		{
			// Cas où tous les items composants ont une interdiction totale
			DegreInterdit = CDlgCible::Item_Interdit_Total;
		}

		// Test si interdiction partielle
		if (NbInterditPartiel > 0)
		{
			// Cas où au moins 1 composant item a une interdiction totale
			DegreInterdit = CDlgCible::Item_Interdit_Partiel;
		}
	}

	// Revoi degré interdiction palette trouvé
	return DegreInterdit;
}

/* En attente !!!!
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Idem mais en retournant en +, la liste des terrains interdits (par composants items)
JUnt32 CRedItemListBox::DegrePaletteInterdit(int ID, JMap <JUnt32, JMap<JUnt32, JUnt32> > & MapTerrainsInterdits) const
{
	// Par défaut palette OK
	JUnt32 DegreInterdit = CDlgCible::Item_Interdit_Aucun;

	// Récupère la palette
	CPalettes * palette = m_pDlgParent->m_pItems->GetIItemByID(ID)->AsPalette();
	if(palette)
	{
		// Init le nombre d'interdictions partielles trouvées et le nombre d'interdictions totales
		JUnt32 NbInterditPartiel	= 0;
		JUnt32 NbInterditTotal		= 0;

		// Récupère les composants items de la palette
		JList<JInt32> idList;
		palette->GetItems(idList);
		for (idList.MoveFirst(); idList.IsValid(); idList.MoveNext())
		{
			// Test si item composant avec interdiction partielle
			if (DegreItemInterdit(idList.GetItem(), MapTerrainsInterdits) == CDlgCible::Item_Interdit_Partiel)
			{
				NbInterditPartiel++;
			}
			// Test si item composant avec interdiction totale
			else if (DegreItemInterdit(idList.GetItem(), MapTerrainsInterdits) == CDlgCible::Item_Interdit_Total)
			{
				NbInterditTotal++; 
			}
		}

		// Test si interdiction totale
		if (NbInterditTotal == idList.GetCount())
		{
			// Cas où tous les items composants ont une interdiction totale
			DegreInterdit = CDlgCible::Item_Interdit_Total;
		}

		// Test si interdiction partielle
		if (NbInterditPartiel > 0)
		{
			// Cas où au moins 1 composant item a une interdiction totale
			DegreInterdit = CDlgCible::Item_Interdit_Partiel;
		}
	}

	// Revoi degré interdiction palette trouvé
	return DegreInterdit;
}
*/

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Renvoi niveau d'interdiction de l'item à l'utilisation
// 0 - Aucune interdiction
// 1 - Interdiction partielle (seulement interdite sur certains terrains
// 2 - Interdiction totale (interdit sur tous les terrains)
JUnt32 CRedItemListBox::DegreItemInterdit(int ID) const
{
	// Par défaut pas d'interdiction	
	JUnt32 DegreInterdit = CDlgCible::Item_Interdit_Aucun;
	
	// On se positionne sur ma map item parametres
	m_pDlgParent->m_mapItemParams.MoveTo(ID);
	if(m_pDlgParent->m_mapItemParams.IsValid())
	{
		/*
		if (m_pDlgParent->m_mapItemParams.GetItem()->GetNbCas() < AfxGetApp()->GetProfileInt("Moteur", "Seuil_Limite", 100))
		{
			DegreInterdit = Item_Interdit_Total;
		}
		*/
		// Init nombre d'interdiction par terrain
		JUnt32 NbInterdit = 0;

		// Boucle sur les infos seuils terrains
		JSRCTables * pSrcTables = (JSRCTables*)& JSRCTables::GetInstance();

		// Boucle sur les terrains
		for (m_pDlgParent->m_LstIdTerrains.MoveFirst(); m_pDlgParent->m_LstIdTerrains.IsValid(); m_pDlgParent->m_LstIdTerrains.MoveNext())
		{
			// Récup l'info terrain
			JUnt32 IdTerrain = m_pDlgParent->m_LstIdTerrains.GetItem();
			const JTerrain * pTerrain	= pSrcTables->m_TBLTerrains.GetItem(m_pDlgParent->m_IdSrcAudience, IdTerrain);

			// On se positionne sur les infos items pour ce terrain
			m_pDlgParent->m_mapItemParams.GetItem().MoveTo(IdTerrain); 

			if (m_pDlgParent->m_mapItemParams.GetItem().IsValid()) 
			{
				// Test nombre de cas item via les seuils du terrain
				JInt32 NbCasItem = m_pDlgParent->m_mapItemParams.GetItem().GetItem()->GetNbCas();
				if (NbCasItem < pTerrain->m_SeuilRedhibitoire)
				{
					// Incrémente le nombre de terrains interdits
					NbInterdit++; 
				}
			}
		}

		if (NbInterdit != 0)
		{
			// Si nombre d'interdit = nombre de terrains sélectionnés, alors cet item est inutilisable
			if (NbInterdit == m_pDlgParent->m_LstIdTerrains.GetCount())
				DegreInterdit = CDlgCible::Item_Interdit_Total; 

			// Si au moins 1 interdit , alors cet item est partiellement utilisable
			else if (NbInterdit > 0)
				DegreInterdit = CDlgCible::Item_Interdit_Partiel; 
		}
	}

	// Renvoi le degré d'interdiction de l'item
	return DegreInterdit;
}

int CRedItemListBox::OnToolHitTest(CPoint point, TOOLINFO * pTI) const
{
	int row;
	RECT cellrect;   // cellrect		- to hold the bounding rect
	BOOL tmp = FALSE;
	row  = ItemFromPoint(point,tmp);  //we call the ItemFromPoint function to determine the row,
	if ( row == -1 ) 
		return -1;

	//set up the TOOLINFO structure. GetItemRect(row,&cellrect);
	GetItemRect(row,&cellrect);
	pTI->rect = cellrect;
	pTI->hwnd = m_hWnd;
	pTI->uId = (UINT)((row));   //The ‘uId’ is assigned a value according to the row value.
	pTI->lpszText = LPSTR_TEXTCALLBACK;
	return (int)pTI->uId;

}


//Define OnToolTipText(). This is the handler for the TTN_NEEDTEXT notification from 
//support ansi and unicode 
BOOL CRedItemListBox::OnToolTipText( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{
	// need to handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	CString strTipText;
	UINT nID = (UINT)pNMHDR->idFrom;

	::SendMessage(pNMHDR->hwndFrom, TTM_SETMAXTIPWIDTH, 0, 300);

	// Init texte tooltip
	strTipText = "";

	// Ident de l'item survolé
	JInt32 ID =  ((IItem*)GetItemData(nID))->GetID();

	// Est-ce une palette
	CPalettes * palette = m_pDlgParent->m_pItems->GetIItemByID(ID)->AsPalette();

	if(palette)
	//////////////////////////////////////////////////// Palettes d'items ///////////////////////////////////////////////////
	{
		JList<JInt32> idList;
		palette->GetItems(idList);
		for (idList.MoveFirst(); idList.IsValid(); idList.MoveNext())
		{
			m_pDlgParent->m_mapItemParams.MoveTo(idList.GetItem());
			if(	m_pDlgParent->m_mapItemParams.IsValid())
			{
				IItem * item = m_pDlgParent->m_pItems->GetIItemByID( idList.GetItem() );
				
				// Le libellé
				strTipText.AppendFormat("%s %s \n", "Item",item->GetLabel().AsJCharPtr());

				// Boucle sur les infos seuils terrains
				JSRCTables * pSrcTables = (JSRCTables*)& JSRCTables::GetInstance();

				// Récup le source
				const JSource * ptSrc		= pSrcTables->m_TBLSources.GetItem(m_pDlgParent->m_IdSrcAudience);

				// Boucle sur les terrains
				for (m_pDlgParent->m_LstIdTerrains.MoveFirst(); m_pDlgParent->m_LstIdTerrains.IsValid(); m_pDlgParent->m_LstIdTerrains.MoveNext())
				{
					// Récup l'info terrain
					JUnt32 IdTerrain = m_pDlgParent->m_LstIdTerrains.GetItem();
					const JTerrain * pTerrain	= pSrcTables->m_TBLTerrains.GetItem(m_pDlgParent->m_IdSrcAudience, IdTerrain);

					// On se positionne sur les infos items pour ce terrain
					m_pDlgParent->m_mapItemParams.GetItem().MoveTo(IdTerrain); 

					if (m_pDlgParent->m_mapItemParams.GetItem().IsValid()) 
					{
						// Nombre de cas sur ce terrain
						CString TxtNbCas;
						TxtNbCas.Format("%s%s %s (%s %d)","  - ",ptSrc->m_Libelle.AsJCharPtr(), pTerrain->m_Libelle.AsJCharPtr(),"Nb cas =", m_pDlgParent->m_mapItemParams.GetItem().GetItem()->GetNbCas());

						// Ajoute au tooltip texte
						strTipText.Append(TxtNbCas);
						strTipText += "\n";
					}
				}	
			}
		}
		strTipText.Trim();
		strTipText.AppendChar('\0');
	}
	else
	///////////////////////////////////////////////////////////// Items simples /////////////////////////////////////////////////
	{
		m_pDlgParent->m_mapItemParams.MoveTo(ID);
		if(	m_pDlgParent->m_mapItemParams.IsValid())
		{
			// Boucle sur les infos seuils terrains
			JSRCTables * pSrcTables = (JSRCTables*)& JSRCTables::GetInstance();

			// Récup le source
			const JSource * ptSrc		= pSrcTables->m_TBLSources.GetItem(m_pDlgParent->m_IdSrcAudience);

			// Boucle sur les terrains
			for (m_pDlgParent->m_LstIdTerrains.MoveFirst(); m_pDlgParent->m_LstIdTerrains.IsValid(); m_pDlgParent->m_LstIdTerrains.MoveNext())
			{
				// Récup l'info terrain
				JUnt32 IdTerrain = m_pDlgParent->m_LstIdTerrains.GetItem();
				const JTerrain * pTerrain	= pSrcTables->m_TBLTerrains.GetItem(m_pDlgParent->m_IdSrcAudience, IdTerrain);

				// On se positionne sur les infos items pour ce terrain
				m_pDlgParent->m_mapItemParams.GetItem().MoveTo(IdTerrain); 

				if (m_pDlgParent->m_mapItemParams.GetItem().IsValid()) 
				{
					CCalcItemParams *pParams =  m_pDlgParent->m_mapItemParams.GetItem().GetItem(); 

					// Nombre de cas sur ce terrain
					CString TxtNbCas;
					TxtNbCas.Format("%s%s %s (%s %d)","  - ",ptSrc->m_Libelle.AsJCharPtr(), pTerrain->m_Libelle.AsJCharPtr(),"Nb cas =", m_pDlgParent->m_mapItemParams.GetItem().GetItem()->GetNbCas());

					// Ajoute au tooltip texte
					strTipText.Append(TxtNbCas);
					strTipText += "\n";
				}
			}
			
		}
		strTipText.AppendChar('\0');
	}

#ifndef _UNICODE
	if(pNMHDR->code == TTN_NEEDTEXTA)
	{
		if(m_pchTip)
			delete m_pchTip;
		
		m_pchTip = new TCHAR[strTipText.GetLength()];
		lstrcpyn(m_pchTip, strTipText, strTipText.GetLength());
		pTTTW->lpszText = (WCHAR*)m_pchTip;
	}
	if(pNMHDR->code == TTN_NEEDTEXTW)
	{
		if(m_pwchTip)
			delete m_pwchTip;
		
		m_pwchTip = new WCHAR[strTipText.GetLength()];
		_mbstowcsz(m_pwchTip, strTipText, strTipText.GetLength());
		pTTTW->lpszText = (WCHAR*)m_pwchTip;
	}
#else
	if(pNMHDR->code == TTN_NEEDTEXTA)
	{
		if(m_pchTip)
			delete m_pchTip;
		
		m_pchTip = new TCHAR[strTipText.GetLength()];
		_wcstombsz(m_pchTip, strTipText, strTipText.GetLength());
		pTTTA->lpszText = (LPTSTR)m_pchTip;
	}
	if(pNMHDR->code == TTN_NEEDTEXTW)
	{
		if(m_pwchTip)
			delete m_pwchTip;
		
		m_pwchTip = new WCHAR[strTipText.GetLength()];
		lstrcpyn(m_pwchTip, strTipText, strTipText.GetLength());
		pTTTA->lpszText = (LPTSTR) m_pwchTip;
	}
#endif

	*pResult = 0;

	return TRUE;    
}


void CRedItemListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// If there are no list box items, skip this message. 
	if (lpDrawItemStruct->itemID == -1) 
		return; 

	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	
	IItem* lpItem	= (IItem*) lpDrawItemStruct->itemData;
	CString sLabel	= lpItem->GetLabel().AsJCharPtr();
	JInt32 ItemID	= lpItem->GetID();

	// Save these values to restore them when done drawing.
	COLORREF crOldTextColor = pDC->GetTextColor();
	COLORREF crOldBkColor	= pDC->GetBkColor();

	// item selected
	COLORREF colorref;
	if ((lpDrawItemStruct->itemAction | ODA_SELECT) &&
		(lpDrawItemStruct->itemState & ODS_SELECTED))
	{
		pDC->SetTextColor(m_ColorTxtSel);
		colorref = m_ColorNORMALSel;
		pDC->SetBkColor(colorref);
	}

	else// item deselected
	{
		colorref = crOldBkColor;
		if ( IsAttribue(ItemID) )
			pDC->SetTextColor( m_ColorAttribue );

		if (DegreInterdit(ItemID) == CDlgCible::Item_Interdit_Partiel)
		{
			// Interdiction partielle
			pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));

			colorref = RGB(255, 200, 145);
			pDC->SetBkColor(colorref);
		}

		else if (DegreInterdit(ItemID) == CDlgCible::Item_Interdit_Total)
		{
			// Interdiction totale
			pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));

			// colorref = RGB(255,132,193);
			colorref = RGB(255,102,102);
			pDC->SetBkColor(colorref);
		}
	}

	//si cet ID est sur-selectionné
	if ( ItemID == m_IDJFCSelection )
	{
		pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		if( m_JFCSelectionMode == PREGNANCE )
		{
			pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
			colorref = m_ColorPREGNANCESel;
			pDC->SetBkColor(colorref);
		}
		if( m_JFCSelectionMode == OPPOSITION )
		{
			// On colore pour indiquer non sélectionabilité
			pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
			colorref = m_ColorOPPOSITIONSel;
			pDC->SetBkColor(colorref);
		}
	}

	// Remplissage de la zone 
	pDC->FillSolidRect(&lpDrawItemStruct->rcItem, colorref);

	RECT Rect = lpDrawItemStruct->rcItem;
	Rect.left+=5; 
	Rect.right-=5;
	pDC->DrawText( sLabel, -1, &Rect, DT_WORDBREAK | DT_NOPREFIX );

	// Reset the background color and the text color back to their
	// original values.
	pDC->SetTextColor(crOldTextColor);
	pDC->SetBkColor(crOldBkColor);
}
