// ListBox_Complexe.cpp : Defines the class behaviors for the application.
//
#include "stdafx.h"
#include "ListBox_Complexe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
ListBox_Complexe::ListBox_Complexe()
{
	// Par défaut on prend les anciennes valeurs Atelier Cinema
	InfoColListe Info;
	m_TabListe.SetSize(3);
	Info.TailleCol = 207; m_TabListe.Add(Info);		
	Info.TailleCol = 25 ; m_TabListe.Add(Info);
	
	// Par défaut, texte aligné à gauche
	m_AlignType=DT_LEFT;
}

ListBox_Complexe::~ListBox_Complexe()
{
}

//////////////////////////////////////////////////////////////////////////////
// Définition nombre de colonnes
bool ListBox_Complexe::InitNbColonne(int NbColonne)
{
	try
	{
		// Dimensionnement tableau info taille colonnes
		m_TabListe.RemoveAll();
		m_TabListe.SetSize(NbColonne);
		return(true);
	}

	// Problème initialisation taille colonne
	catch(...)
	{
		CString txt;
		txt.Format("Initialisation impossible <InitSizeColonne> ListBox_Complexe");
		AfxMessageBox(txt);
		return(false);
	}
}

//////////////////////////////////////////////////////////////////////////////
// Définition taille des colonnes
bool ListBox_Complexe::InitSizeColonne(int InxCol,int SizeCol)
{
	try
	{
		m_TabListe[InxCol-1].TailleCol = SizeCol;		
		return(true);
	}

	// Problème initialisation taille colonne
	catch(...)
	{
		CString txt;
		txt.Format("Initialisation impossible <InitSizeColonne> ListBox_Complexe");
		AfxMessageBox(txt);
		return(false);
	}

}


BEGIN_MESSAGE_MAP(ListBox_Complexe, CListBox)
	//{{AFX_MSG_MAP(ListBox_Complexe)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ListBox_Complexe message handlers

void ListBox_Complexe::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CObj_LBox::OnLButtonDown(nFlags, point);
}


void ListBox_Complexe::OnMouseMove(UINT nFlags, CPoint point) 
{
	CObj_LBox::OnMouseMove(nFlags, point);
}

#if(0)
void ListBox_Complexe::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC *dc=CDC::FromHandle(lpDrawItemStruct->hDC);

	// Separation des substrings et recuperation
	CString txt;
	int index=lpDrawItemStruct->itemID;
	if(index>=0)
		CListBox::GetText(index,txt);
	
	int n = txt.Find('\t', 0); 
	CString txtleft;
	CString txtmidlle;
	CString txtright;
	if (n == -1 )
	{
		txtleft = txt;
	} 
	else 
	{
		txtleft = txt.Left(n);
		txt = txt.Mid(n+1);
		int m = txt.Find('\t',0);
		if (m == -1)
		{
			txtmidlle = txt;
		}
		else
		{
			txtmidlle = txt.Left(m);
			txtright = txt.Mid(m+1);
		}
	}


	// Récupération du rectangle
	CRect Rect(lpDrawItemStruct->rcItem);

	int etat = GetItemData(lpDrawItemStruct->itemID);

	if(lpDrawItemStruct->itemState&ODS_SELECTED)
	{
		CBrush Brush;
		Brush.CreateSolidBrush(GetSysColor(COLOR_HIGHLIGHT));
		dc->FillRect(Rect,&Brush);
		dc->SetBkMode(TRANSPARENT);
		dc->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
	}
	if(etat==1)
	{
		CBrush Brush;
		Brush.CreateSolidBrush(GetSysColor(COLOR_HIGHLIGHT));
		dc->FillRect(Rect,&Brush);
		dc->SetBkMode(TRANSPARENT);
		dc->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
	}	
	else
	{
		if(lpDrawItemStruct->itemState!=ODS_SELECTED)
		{
			CBrush Brush;
			Brush.CreateSolidBrush(GetSysColor(COLOR_WINDOW));
			dc->FillRect(Rect,&Brush);
			dc->SetBkMode(TRANSPARENT);
			dc->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
		}
	}
	// Affichage des substrings dans les rectangles avec décalage
	//int offsetHor1 = 207;
	//int offsetHor2 = 25;
	int offsetHor1 = m_TabListe[0].TailleCol;
	int offsetHor2 = m_TabListe[1].TailleCol;

	Rect.OffsetRect(5,0);
	dc->DrawText(txtleft,Rect,DT_SINGLELINE|DT_LEFT);

	Rect.OffsetRect(offsetHor1,0);
	dc->DrawText(txtmidlle,Rect,DT_SINGLELINE|DT_LEFT);

	Rect.OffsetRect(offsetHor2,0);
	dc->DrawText(txtright,Rect,DT_SINGLELINE|DT_LEFT);
	
	// Affichage des lignes de séparation
	//int lineposX1=202;
	//int lineposX2=232;
	int lineposX1 = m_TabListe[0].TailleCol - 3;
	int lineposX2 = m_TabListe[0].TailleCol + m_TabListe[1].TailleCol;

	GetClientRect(Rect);

	dc->MoveTo( lineposX1, Rect.top);
	dc->LineTo( lineposX1, Rect.bottom);

	dc->MoveTo( lineposX2, Rect.top);
	dc->LineTo( lineposX2, Rect.bottom);
}
#endif

/*
void ListBox_Complexe::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC *dc=CDC::FromHandle(lpDrawItemStruct->hDC);
	int etat;

	// Separation des substrings et recuperation
	CString txt;
	int index=lpDrawItemStruct->itemID;
	if(index>=0)
		CListBox::GetText(index,txt);
	
	int n = txt.Find('\t', 0); 
	CString txtleft;
	CString txtmidlle;
	CString txtright;
	if (n == -1 )
	{
		txtleft = txt;
	} 
	else 
	{
		txtleft = txt.Left(n);
		txt = txt.Mid(n+1);
		int m = txt.Find('\t',0);
		if (m == -1)
		{
			txtmidlle = txt;
		}
		else
		{
			txtmidlle = txt.Left(m);
			txtright = txt.Mid(m+1);
		}
	}


	// Récupération du rectangle
	CRect Rect(lpDrawItemStruct->rcItem);

	if((lpDrawItemStruct->itemState & ODS_SELECTED) != 0)
	{
		CBrush Brush;
		Brush.CreateSolidBrush(GetSysColor(COLOR_HIGHLIGHT));
		dc->FillRect(Rect,&Brush);
		dc->SetBkMode(TRANSPARENT);
		dc->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
	}
	else
	{
		CBrush Brush;
		Brush.CreateSolidBrush(GetSysColor(COLOR_WINDOW));
		dc->FillRect(Rect,&Brush);
		dc->SetBkMode(TRANSPARENT);
		dc->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
	}
	// Affichage des substrings dans les rectangles avec décalage
	//int offsetHor1 = 207;
	//int offsetHor2 = 25;
	int offsetHor1 = m_TabListe[0].TailleCol;
	int offsetHor2 = m_TabListe[1].TailleCol;

	Rect.OffsetRect(5,0);
	dc->DrawText(txtleft,Rect,DT_SINGLELINE|DT_LEFT);

	Rect.OffsetRect(offsetHor1,0);
	dc->DrawText(txtmidlle,Rect,DT_SINGLELINE|DT_LEFT);

	Rect.OffsetRect(offsetHor2,0);
	dc->DrawText(txtright,Rect,DT_SINGLELINE|DT_LEFT);
	
	// Affichage des lignes de séparation
	//int lineposX1=202;
	//int lineposX2=232;
	int lineposX1 = m_TabListe[0].TailleCol - 3;
	int lineposX2 = m_TabListe[0].TailleCol + m_TabListe[1].TailleCol;

	GetClientRect(Rect);

	dc->MoveTo( lineposX1, Rect.top);
	dc->LineTo( lineposX1, Rect.bottom);

	dc->MoveTo( lineposX2, Rect.top);
	dc->LineTo( lineposX2, Rect.bottom);
}
*/

void ListBox_Complexe::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC *dc=CDC::FromHandle(lpDrawItemStruct->hDC);
	int etat;

	// Separation des substrings et recuperation
	CString txt;
	int index=lpDrawItemStruct->itemID;
	if(index>=0)
		CListBox::GetText(index,txt);
	
	int n = txt.Find('\t', 0); 
	CString txtleft;
	CString txtmidlle;
	CString txtright;
	if (n == -1 )
	{
		txtleft = txt;
	} 
	else 
	{
		txtleft = txt.Left(n);
		txt = txt.Mid(n+1);
		int m = txt.Find('\t',0);
		if (m == -1)
		{
			txtmidlle = txt;
		}
		else
		{
			txtmidlle = txt.Left(m);
			txtright = txt.Mid(m+1);
		}
	}


	// Récupération du rectangle
	CRect Rect(lpDrawItemStruct->rcItem);

	/*
	if((lpDrawItemStruct->itemState & ODS_SELECTED) != 0)
	{
		CBrush Brush;
		Brush.CreateSolidBrush(GetSysColor(COLOR_HIGHLIGHT));
		dc->FillRect(Rect,&Brush);
		dc->SetBkMode(TRANSPARENT);
		dc->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
	}
	else
	{
		CBrush Brush;
		Brush.CreateSolidBrush(GetSysColor(COLOR_WINDOW));
		dc->FillRect(Rect,&Brush);
		dc->SetBkMode(TRANSPARENT);
		dc->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
	}
	*/

	if(lpDrawItemStruct->itemState&ODS_SELECTED)
	{
		CBrush Brush;
		Brush.CreateSolidBrush(GetSysColor(COLOR_HIGHLIGHT));
		dc->FillRect(Rect,&Brush);
		dc->SetBkMode(TRANSPARENT);
		dc->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
	}	

	etat = GetItemData(lpDrawItemStruct->itemID); 

	if(etat==1)
	{
		CBrush Brush;
		Brush.CreateSolidBrush(GetSysColor(COLOR_HIGHLIGHT));
		dc->FillRect(Rect,&Brush);
		dc->SetBkMode(TRANSPARENT);
		dc->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
	}
	else
	{
		if(lpDrawItemStruct->itemState!=ODS_SELECTED)
		{
			CBrush Brush;
			Brush.CreateSolidBrush(GetSysColor(COLOR_WINDOW));
			dc->FillRect(Rect,&Brush);
			dc->SetBkMode(TRANSPARENT);
			dc->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
		}
	}

	// Affichage des substrings dans les rectangles avec décalage
	//int offsetHor1 = 207;
	//int offsetHor2 = 25;
	int offsetHor1 = m_TabListe[0].TailleCol;
	int offsetHor2 = m_TabListe[1].TailleCol;

	Rect.OffsetRect(5,0);
	dc->DrawText(txtleft,Rect,DT_SINGLELINE|DT_LEFT);

	Rect.OffsetRect(offsetHor1,0);
	dc->DrawText(txtmidlle,Rect,DT_SINGLELINE|DT_LEFT);

	Rect.OffsetRect(offsetHor2,0);
	dc->DrawText(txtright,Rect,DT_SINGLELINE|DT_LEFT);
	
	// Affichage des lignes de séparation
	//int lineposX1=202;
	//int lineposX2=232;
	int lineposX1 = m_TabListe[0].TailleCol - 3;
	int lineposX2 = m_TabListe[0].TailleCol + m_TabListe[1].TailleCol;

	GetClientRect(Rect);

	dc->MoveTo( lineposX1, Rect.top);
	dc->LineTo( lineposX1, Rect.bottom);

	dc->MoveTo( lineposX2, Rect.top);
	dc->LineTo( lineposX2, Rect.bottom);
}

/////////////////////////////////////////////////////////////////////////////////////
// Lecture libelle liste (nIndex : index liste, ColIndex : index colonne)
void ListBox_Complexe::GetText(int nIndex, int ColIndex,CString &rString) const
{
	CString txt,tmptxt;
	int NbTabul;
	CArray<CString,CString> TabString;

	//Récupération de la chaine complète
	CListBox::GetText(nIndex,txt);		

	// Décomposition string via tabulation
	NbTabul = 0;
	tmptxt = txt;
	while (tmptxt != "")
	{
		int n = tmptxt.Find('\t', 0);				
		if (n != -1)
		{
			// 1 tabulation trouvée, on continue le décorticage
			NbTabul++;
			tmptxt = tmptxt.Mid(n+1);
		}
		else
			// plus de tabulation
			tmptxt = "";
	}	

	// Renvoi la string positionné en ColIndex
	if (NbTabul == 0 || ColIndex > NbTabul+1)

		// renvoi d'office toute la chaine
		rString = txt;

	else
	{
		// récup tab chaine 
		TabString.SetSize(NbTabul+1);
		tmptxt = txt;
		for (int i=0;i<NbTabul+1;i++)
		{
			int n = tmptxt.Find('\t', 0);			
			if (n != -1)
			{
				// récup string entre tabulations
				TabString[i] = tmptxt.Left(n);
				tmptxt = tmptxt.Mid(n+1);
			}
			else
				// fin de tabulation
				TabString[i] = tmptxt;
		}

		// renvoi la chaine en position ColIndex (attention 1ere colonne = 1)
		rString = TabString[ColIndex-1];
	}

	/*
	//On garde que le nom du complexe
	int n = txt.Find('\t', 0);			
	
	if (n == -1 )
	{
		rString = txt;
	} 
	else 
		rString = txt.Left(n);
	*/

}

int ListBox_Complexe::GetSelCount()
{
	return CObj_LBox::GetSelCount();
}

int ListBox_Complexe::GetSelItems(int nMaxItems, LPINT rgIndex )
{
	return CObj_LBox::GetSelItems( nMaxItems,rgIndex);
}

