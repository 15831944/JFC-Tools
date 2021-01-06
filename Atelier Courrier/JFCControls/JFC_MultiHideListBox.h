/*
//	JFC Informatique et Média
//	2005
//
//	Auteur:
//	Eddie GENT (Template de la listbox Respect)
//	Jérémy DEVILERS (Selection JFC)
//
//
//	Classe:
//	JFC_MultiHideListBox<T>
//
//	Descriptif:
//	ListBox selection JFC prenant des T avec des methodes
//	JInt32 GetID(); et JString GetLabel();
*/


#pragma once
/*
#if _MFC_VER < 0x0700
	// For MFC6.0
	#ifdef _AFXDLL
		#define BEGIN_TEMPLATED_MESSAGE_MAP1(theClass, baseClass) \
		template <class T>  const AFX_MSGMAP* PASCAL theClass<T>::_GetBaseMessageMap() { return &baseClass::messageMap; } \
		template <class T>  AFX_COMDAT AFX_DATADEF const AFX_MSGMAP theClass<T>::messageMap = { &theClass::_GetBaseMessageMap, &theClass::_messageEntries[0] }; \
		template <class T>  const AFX_MSGMAP* theClass<T>::GetMessageMap() const { return &theClass::messageMap; } \
		template <class T>  AFX_COMDAT const AFX_MSGMAP_ENTRY theClass<T>::_messageEntries[] = {
	#else
		#define BEGIN_TEMPLATED_MESSAGE_MAP1(theClass, baseClass) \
		template <class T>  AFX_COMDAT AFX_DATADEF const AFX_MSGMAP theClass<T>::messageMap = { &baseClass::messageMap, &theClass::_messageEntries[0] }; \
		template <class T>  const AFX_MSGMAP* theClass<T>::GetMessageMap() const { return &theClass::messageMap; } \
		template <class T>  AFX_COMDAT const AFX_MSGMAP_ENTRY theClass<T>::_messageEntries[] = {
	#endif
#else
	// For MFC 7.0
	#ifdef _AFXDLL
		#define BEGIN_TEMPLATED_MESSAGE_MAP1(theClass,theBase) \
		template <class T> const AFX_MSGMAP* PASCAL theClass<T>::GetThisMessageMap() \
			{ return &theClass::messageMap; } \
		template <class T> const AFX_MSGMAP* theClass<T>::GetMessageMap() const \
			{ return &theClass::messageMap; } \
		template <class T> const AFX_MSGMAP theClass<T>::messageMap = \
		{ &theBase::GetThisMessageMap, &theClass<T>::_messageEntries[0] }; \
		template <class T> const AFX_MSGMAP_ENTRY theClass<T>::_messageEntries[] = \
		{ \

	#else
		#define BEGIN_TEMPLATED_MESSAGE_MAP1(theClass) \
		template <class T> const AFX_MSGMAP* theClass<T>::GetMessageMap() const \
			{ return &theClass::messageMap; } \
		template <class T> const AFX_MSGMAP theClass<T>::messageMap = \
		{ &theBase::messageMap, &theClass<BASE>::_messageEntries[0] }; \
		template <class T> const AFX_MSGMAP_ENTRY theClass<T>::_messageEntries[] = \
		{ \

	#endif
#endif
*/
//#define WM_USER_RIGHTBUTTONDOWN (WM_USER + 102)

//Enumeration des différents modes de sélection
//#ifndef JFCSELMODE
//	typedef enum JFCSELMODE {NORMAL, PREGNANCE, OPPOSITION} JFCSELMODE;
//#endif

/**
 * \ingroup JFCControls
 * This is a list box with multi line features and multi selection features
 * which take classes the have a GetLabel method.
 *
 *
 * \par requirements
 * win98 or later\n
 * win2k or later\n
 * MFC\n
 *
 * \version 1.0
 * first version
 *
 * \date 07-10-2003
 *
 * \author eddie
 *
 * \par license
 * This code is the property of JFC Informatique & Media\n
 * Unauthorized copying or use is strictly forbiden.
 * 
 * \todo 
 *
 * \bug 
 *
 */


template <class T>
class JFC_MultiHideListBox : public CListBox
{
// Construction
public:
	enum JFCSELMODE {NORMAL, PREGNANCE, OPPOSITION};

	JFC_MultiHideListBox()
	{
		m_Rect.left		= 0;
		m_Rect.top		= 0;
		m_Rect.right	= 0;
		m_Rect.bottom	= 0;
		m_ActifItems.Reset();
		m_RightClick	= false;

		//Ajout Jérémy
		m_JFCSelectionMode = NORMAL;
		m_bMouseMove = false;
		m_idxFirst = m_idxPrec = -1;
		m_IDJFCSelection = -1;

		m_ColorNORMALSel = RGB(0, 140, 120);//RGB(146, 186, 173);//::GetSysColor(COLOR_HIGHLIGHT);//bleu si theme par défaut
		m_ColorPREGNANCESel = RGB(100,100,100);//gris
		m_ColorOPPOSITIONSel = RGB(255,0,0);//rouge
		m_ColorAttribue = ::GetSysColor(COLOR_WINDOWTEXT);
	};


// Attributes
protected:
	RECT		m_Rect;
	JMap<JInt32,JBool> m_ActifItems;
	//L'ID de l'item sur-sélectionné
	JInt32 m_IDJFCSelection;
	COLORREF m_ColorAttribue;

private:
	//pour le OnMouseMove()
	bool m_bMouseMove;
	int m_idxPrec, m_idxFirst, m_idxHaut, m_idxBas;
	bool m_bFirstSel;
	//Le mode de selection
	JFCSELMODE m_JFCSelectionMode;
	//Les couleurs des selections
	COLORREF m_ColorNORMALSel;
	COLORREF m_ColorPREGNANCESel;
	COLORREF m_ColorOPPOSITIONSel;
	bool	m_RightClick;

	virtual bool IsAttribue(int ID) const { return false; }

// Operations
public:

	bool IsRightClick() { return m_RightClick; }

	bool	ToggleSelection()
	{
		if(!GetCount())
			return false;

		if(GetStyle()&(LBS_MULTIPLESEL|LBS_EXTENDEDSEL))
		{
			int NbSel		= GetSelCount();
			bool Select		= false;

			if(NbSel == 0)
				Select = true;

			if(GetCount() == 1)
				SetSel(0,Select);
			else
				SelItemRange( Select,0,GetCount()-1);
			return true;
		}

		return false;
	};

	// Overrides reset content
	void ResetContent()
	{
		CListBox::ResetContent();
		m_ActifItems.Reset();
	};

	/// Adds a new item with the object provided
	int AddItem(T pData)
	{
		int Pos = AddString((LPCSTR)pData);
		return Pos;
	};

	/// Adds a new item with the object provided
	int AddItems(JList<T> & Data)
	{
		for(Data.MoveFirst(); Data.IsValid(); Data.MoveNext())
		{
			AddString((LPCSTR)Data.GetItem());
		}
		return 0;
	};

	/// Removes an item with the ID
	bool RemoveItem(int ID)
	{
		int Index = FindItemIndex(ID);

		if(Index != LB_ERR)
		{
			DeleteString(Index);
		}
		return false;
	};

	void SetActifItems(JList<JInt32> & Items)
	{
		m_ActifItems.Reset();
		for(Items.MoveFirst(); Items.IsValid(); Items.MoveNext())
		{
			m_ActifItems.Add(Items.GetItem()) = true;
		}
		Invalidate();
		UpdateWindow(); 
	}

	void SelectFirstActive()
	{
		if(m_ActifItems.GetCount())
		{
			for(int x=0; x  < GetCount(); x++)
			{
				T lpItem = (T)	GetItemData(x);
				m_ActifItems.MoveTo(lpItem->GetID());
				if(m_ActifItems.IsValid())
				{
					if(GetStyle()&(LBS_MULTIPLESEL|LBS_EXTENDEDSEL))
					{
						SelItemRange( false,0,GetCount()-1);
						SetSel(x,true);
						SetCaretIndex(x,false);
					}
					else
						SetCurSel(x);
					break;
				}
			}
		}
		else
		{
			if(GetStyle()&(LBS_MULTIPLESEL|LBS_EXTENDEDSEL))
			{
				SelItemRange( false,0,GetCount()-1);
				SetSel(0,true);
				SetCaretIndex(0,false);
			}
			else
				SetCurSel(0);
		}
	}

	JInt32 GetSelectedID()
	{
		int CurSel = GetCurSel();

		if(CurSel != LB_ERR)
		{
			// Single selection listbox with a current selection
			T lpItem = (T) GetItemData(CurSel);
			return lpItem->GetID();
		}
		else
			return CurSel;
	}

	void GetSelectedID(JList<JInt32> & ID)
	{
		ID.Reset();
		int NbSel		= GetSelCount();
		int CurSel		= GetCurSel();

		// Could be multi selection list box or no selection
		if(GetStyle()&(LBS_MULTIPLESEL|LBS_EXTENDEDSEL))
		{
			if(NbSel && NbSel !=LB_ERR)
			{
				// Multiselection with selected items
				int *pSel = new int[NbSel];;
				GetSelItems(NbSel,pSel);
				int x=0;
				for(x=0; x < NbSel; x++)
				{
					T lpItem = (T) GetItemData(pSel[x]);
					ID.AddTail() = lpItem->GetID();
				}
				delete [] pSel;
			}
		}
		else
		{
			if(CurSel != LB_ERR)
			{
				// Single selection listbox with a current selection
				T lpItem = (T) GetItemData(CurSel);

				ID.AddTail() = lpItem->GetID();
			}
		}
	}

	void SetSelectedID(JList<JInt32> & ID)
	{
		int NbSel		= GetSelCount();
		int CurSel		= GetCurSel();

		int index = GetTopIndex();
		// Could be multi selection list box or no selection
		if(GetStyle()&(LBS_MULTIPLESEL|LBS_EXTENDEDSEL))
		{
			EnableWindow(false);
			if(NbSel && NbSel!=LB_ERR)
			{
				SelItemRange( false,0,GetCount()-1);
			}

			// Set the new selection
			for(ID.MoveFirst(); ID.IsValid(); ID.MoveNext())
			{
				int Index = FindItemIndex(ID.GetItem());
				if(Index != LB_ERR)
					SetSel(Index,true);
			}
			SetTopIndex(index); 
			EnableWindow(true);
			Invalidate();
			UpdateWindow(); 
		}
		else
		{
			EnableWindow(false);
			for(ID.MoveFirst(); ID.IsValid(); ID.MoveNext())
			{
				int Index = FindItemIndex(ID.GetItem());
				if(Index != LB_ERR)
					SetCurSel(Index);
			}
			SetTopIndex(index); 
			EnableWindow(true);
			Invalidate();
			UpdateWindow(); 
		}
	}

	void SetSelectedID(JInt32 ID)
	{
		int NbSel		= GetSelCount();
		int CurSel		= GetCurSel();
		int index = GetTopIndex();

		// Could be multi selection list box or no selection
		if(GetStyle()&(LBS_MULTIPLESEL|LBS_EXTENDEDSEL))
		{
			if(NbSel && NbSel!=LB_ERR)
			{
				SelItemRange( false,0,GetCount()-1);
			}

			// Set the new selection
			int Index = FindItemIndex(ID);
			if(Index != LB_ERR)
			{
				SetSel(Index,true);
				SetTopIndex(index); 
			}
			Invalidate();
			UpdateWindow(); 
		}
		else
		{
			int Index = FindItemIndex(ID);
			if(Index != LB_ERR)
			{
				SetCurSel(Index);
				SetTopIndex(index); 
			}
			Invalidate();
			UpdateWindow(); 
		}
	}


	int	FindItemIndex(int ID)
	{
		int NbItems = GetCount();
		if(NbItems && NbItems != LB_ERR)
		{
			for(int x = 0 ; x < NbItems ; x++)
			{
				T lpItem = (T) GetItemData(x);
				if(lpItem->GetID() == ID)
					return x;
			}
		}
		return LB_ERR;
	}

public:

	virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct)
	{
		T lpItem1 = (T)lpCompareItemStruct->itemData1;
		T lpItem2 = (T)lpCompareItemStruct->itemData2;

		if(lpItem1->GetLabel() < lpItem2->GetLabel())
			// return -1 = item 1 sorts before item 2
			return -1;
		
		if(lpItem1->GetLabel() > lpItem2->GetLabel())
			// return 1 = item 1 sorts after item 2
			return 1;
	
		// return 0 = item 1 and item 2 sort the same
		return 0;

	};

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
	{
		// If there are no list box items, skip this message. 

		if (lpDrawItemStruct->itemID == -1) 
			return; 

		CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
		
		T lpItem = (T) lpDrawItemStruct->itemData;
		CString sLabel = lpItem->GetLabel().AsJCharPtr();
		JInt32 ItemID = lpItem->GetID();

		// Save these values to restore them when done drawing.
		COLORREF crOldTextColor = pDC->GetTextColor();
		COLORREF crOldBkColor = pDC->GetBkColor();

		// item selected
		COLORREF colorref;
		if ((lpDrawItemStruct->itemAction | ODA_SELECT) &&
			(lpDrawItemStruct->itemState & ODS_SELECTED))
		{
			pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
			colorref = m_ColorNORMALSel;
			pDC->SetBkColor(colorref);
		}
		else// item deselected
			colorref = crOldBkColor;
	
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
				pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
				colorref = m_ColorOPPOSITIONSel;
				pDC->SetBkColor(colorref);
			}
		}

		//Si l'item est attribue (méthode à masquer dans une classe fille)
		if ( IsAttribue(ItemID) )
			pDC->SetTextColor( m_ColorAttribue );

		pDC->FillSolidRect(&lpDrawItemStruct->rcItem, colorref);

		m_ActifItems.MoveTo(ItemID);
		if(m_ActifItems.IsValid())
			pDC->SetTextColor(RGB(0,128,0));
		RECT Rect = lpDrawItemStruct->rcItem;
		Rect.left+=5; 
		Rect.right-=5;
		pDC->DrawText( sLabel, -1, &Rect,  DT_RIGHT | DT_WORDBREAK | DT_NOPREFIX );

		if ((lpDrawItemStruct->itemAction | ODA_FOCUS) &&
			(lpDrawItemStruct->itemState & ODS_FOCUS))
		{
			// Draw the focus rect 
			pDC->DrawFocusRect(&(lpDrawItemStruct->rcItem)); 
		}

		// Reset the background color and the text color back to their
		// original values.
		pDC->SetTextColor(crOldTextColor);
		pDC->SetBkColor(crOldBkColor);
	};

	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
	{

		T lpItem = (T) lpMeasureItemStruct->itemData;

		int nItem = lpMeasureItemStruct->itemID;
		CPaintDC dc(this);
		CString sLabel = lpItem->GetLabel().AsJCharPtr();
		CRect rcLabel;

		GetItemRect(nItem, rcLabel);
		// Make left margin of 5 pixels
		rcLabel.left+=5;
		// Make right margin 5 pixels
		rcLabel.right-=5;
		// Calculate the required rectangle for the text and set the item height for this 
		// specific item based on the return value (new height).
		dc.SelectObject(GetFont()); 
		lpMeasureItemStruct->itemHeight = dc.DrawText( sLabel, -1, rcLabel, 
												 DT_RIGHT |DT_WORDBREAK | DT_CALCRECT | DT_NOPREFIX );
	};


// Implementation
public:

	virtual ~JFC_MultiHideListBox() {};

protected:

	afx_msg void OnSize(UINT nType, int cx, int cy)
	{
		CListBox::OnSize(nType, cx, cy);
		RECT Rect;
		GetClientRect(&Rect);
		
		// If the width is the same then nothing to do
		if(m_Rect.right == cx)
			return;

		m_Rect = Rect;

		CPaintDC dc(this);
		CRect rcLabel;
		for(int x=0; x  < GetCount(); x++)
		{
			T lpItem = (T)	GetItemData(x);
			CString sLabel = lpItem->GetLabel().AsJCharPtr();
			GetItemRect(x, rcLabel);
			// Make left margin of 5 pixels
			rcLabel.left+=5;
			// Make right margin 5 pixels
			rcLabel.right-=5;
			// Calculate the required rectangle for the text and set the item height for this 
			// specific item based on the return value (new height).
			dc.SelectObject(GetFont()); 
			int itemHeight = dc.DrawText( sLabel, -1, rcLabel, 
													 DT_RIGHT | DT_WORDBREAK | DT_CALCRECT | DT_NOPREFIX );
			SetItemHeight(x,itemHeight);
		}
	}

////////////////////////////////////////////////////////////////

	// Selectionne ou deselectionne un item sauf s'il est sur-selectionné
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point)
	{
		m_RightClick = false;
		BOOL bOutside;
		m_idxHaut = m_idxBas = m_idxFirst = ItemFromPoint(point, bOutside);
		if(!bOutside)
		{
			CRect Rect;
			GetItemRect(m_idxFirst, &Rect);
			if(GetStyle()&(LBS_MULTIPLESEL|LBS_EXTENDEDSEL) && Rect.PtInRect(point) && m_idxFirst != FindItemIndex(m_IDJFCSelection) )
			{
				SetSel(m_idxFirst, !GetSel(m_idxFirst) );
				
				m_bFirstSel = false;
				if ( GetSel(m_idxFirst) > 0)
					m_bFirstSel = true;
				
				//deselection calcul des bornes
				if ( m_bFirstSel == false)
				{
					do
						m_idxHaut--;
					while ( m_idxHaut > -1 && GetSel(m_idxHaut) != GetSel(m_idxFirst) );
					do
						m_idxBas++;
					while ( m_idxBas < GetCount() && GetSel(m_idxBas) != GetSel(m_idxFirst) );
				}

				m_bMouseMove = TRUE;
				SetCapture();
			}
			else
			{
				if(Rect.PtInRect(point))
					SetCurSel(m_idxFirst);
			}
		}
		
		SetFocus();
		m_idxPrec = m_idxFirst;
		//Puisqu'on redéfinie totalement la selection on ne peux pas y faire appel
		//CListBox::OnLButtonDown(nFlags, point);
	}

	// Selectionne ou deselectionne un item sauf s'il est sur-selectionné
	afx_msg void OnMouseMove(UINT nFlags, CPoint point)
	{
		CListBox::OnMouseMove(nFlags, point);
		if ( m_bMouseMove && GetStyle()&(LBS_MULTIPLESEL|LBS_EXTENDEDSEL) )
		{

			BOOL bOutside;
			int idx = ItemFromPoint(point, bOutside);
			
			//scroll	
			if ( bOutside&& idx == GetTopIndex() ) //on remonte
					SetCaretIndex(idx-1);

			if ( idx !=  m_idxPrec )
			{
				if (m_bFirstSel)
				{
					//deselection
					SelectItemRange( false, m_idxFirst, m_idxPrec );
					//selection de la nouvelle zone
					SelectItemRange( true,  m_idxFirst, idx );
				}
				else
				{
					//on deselectionne tout
					SelItemRange( true, m_idxHaut+1, m_idxBas-1 );
					//on est au dessus
					if (idx > m_idxFirst)
						SelItemRange( false,  m_idxFirst , min(idx, m_idxBas) );
					//on est au dessous
					if (idx < m_idxFirst)
						SelItemRange( false, max(idx, m_idxHaut), m_idxFirst );
					//on est revenu au point de départ
					if (idx == m_idxFirst)
						SetSel( m_idxFirst,  false );
				}

				SetCaretIndex(idx);

				//On reselectionne l'item surSelec
				if (m_IDJFCSelection != -1)
					SetSel( FindItemIndex(m_IDJFCSelection), TRUE );
		
				m_idxPrec = idx;

			}
		}

	}

	afx_msg void OnLButtonUp(UINT nFlags, CPoint point)
	{
		ReleaseCapture();
		NotifySelChanged();
		m_bMouseMove = FALSE;
		m_idxFirst = m_idxPrec = -1;
		CListBox::OnLButtonUp(nFlags, point);
	}

	void SelectItemRange(bool bSel, int idxDeb, int idxFin)
	{
		if (idxDeb == idxFin)
			SetSel(idxDeb, bSel);
		else
			SelItemRange( bSel, min(idxDeb, idxFin), max(idxDeb, idxFin) );
	}

	//Notify parent que la selection à changée
	void NotifySelChanged()
	{
		WPARAM wparam = MAKEWPARAM( GetDlgCtrlID(), LBN_SELCHANGE);
		CWnd* wnd = GetParent();
		if (wnd)
			wnd->PostMessage(WM_COMMAND, wparam, (LPARAM)m_hWnd);
	}


	//Sur-selection que si on a une liste à sel multiple
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point)
	{
		BOOL bOutside;
		m_RightClick = true;
		int idx = ItemFromPoint(point, bOutside);
		if( GetStyle()&(LBS_MULTIPLESEL|LBS_EXTENDEDSEL) && !bOutside)
		{
			int index = GetTopIndex();
			EnableWindow(false);
			if ( m_JFCSelectionMode == NORMAL)
			{
				SelItemRange( false,0,GetCount()-1);
				SetSel(idx);
			}
			else
			{
				T lpItem = (T) GetItemData(idx);
				SetJFCSelection( lpItem->GetID() );
			}
			SetTopIndex(index);
			EnableWindow (true);
		}
		else
			SetCurSel(idx);
		//La selection à changée
		NotifySelChanged();
	}

public:

	// Permet de choisir le mode de sur-selection
	void SetJFCSelectionMode( JFCSELMODE selmode )
	{
		m_JFCSelectionMode = selmode;
	}

	// Retour du mode actuel
	JFCSELMODE GetJFCSelectionMode() const
	{
		return m_JFCSelectionMode;
	}
	
	// Retourne l'ID de l'item sur selectionné
	JInt32 GetJFCSelection() const
	{
		return m_IDJFCSelection;
	}

	// Sur-Selection d'un Item par son ID 
	int SetJFCSelection( JInt32 ID )
	{
		if( GetStyle()&(LBS_MULTIPLESEL|LBS_EXTENDEDSEL) )
		{
			//Sur-selection que s'il est déjà sélectionné
			if ( m_JFCSelectionMode != NORMAL && GetSel( FindItemIndex(ID) ) > 0 )
			{
				//si l'element est déjà sur-sélectionné on le desélectionne
				if ( ID == m_IDJFCSelection )
					m_IDJFCSelection = LB_ERR;
				else	//si ID == -1 on deselectionne car LB_ERR = -1
					m_IDJFCSelection = ID; 

				//On redessinne les items
				Invalidate();
				UpdateWindow();
				return TRUE;
			}
		}

		// Si on est en mode normal on desélectionne et on retourne l'erreur
		m_IDJFCSelection = LB_ERR;
		return m_IDJFCSelection;
	}

	//Permet de choisir les couleus d'affichage des différents types de selection
	void SetSelectionColor( JFCSELMODE selmode, COLORREF color )
	{
		switch(selmode)
		{
		case NORMAL:
			m_ColorNORMALSel = color;
			break;
		case PREGNANCE:
			m_ColorPREGNANCESel = color;
			break;
		case OPPOSITION:
			m_ColorOPPOSITIONSel = color;
			break;
		}
	}

	DECLARE_MESSAGE_MAP()
};


/*	MODIF Aurelien RAINONE : 06.02.2008, changement de macros pour templates
	
BEGIN_TEMPLATED_MESSAGE_MAP1(JFC_MultiHideListBox,CListBox)
	//{{AFX_MSG_MAP(CDlgItems)
		ON_WM_SIZE()
		ON_WM_LBUTTONDOWN()
		ON_WM_MOUSEMOVE()
		ON_WM_LBUTTONUP()
		ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
*/

BEGIN_TEMPLATE_MESSAGE_MAP(JFC_MultiHideListBox, T, CListBox)
	//{{AFX_MSG_MAP(CDlgItems)
		ON_WM_SIZE()
		ON_WM_LBUTTONDOWN()
		ON_WM_MOUSEMOVE()
		ON_WM_LBUTTONUP()
		ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

