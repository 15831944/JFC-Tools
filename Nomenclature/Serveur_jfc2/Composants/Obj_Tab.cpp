// Obj_Tab.cpp : implementation file
//

#include "stdafx.h"
#include "Obj_Tab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CObj_Tab

CObj_Tab::CObj_Tab()
{
	m_Style=0;
	m_NbLig=0;
	m_NbCol=0;
	m_nbLigVisu=0;
	m_nbColVisu=0;
	m_PosActiveCol=0;
	m_PosActiveLig=0;
	m_pScrollH=NULL;
	m_pScrollV=NULL;
	m_fCapture=false;
	m_RectTab.SetRectEmpty();
	m_itTableau.SetSize(3);
	m_sSelection.RemoveAll();

	m_PGrisF=new CPen(PS_SOLID,1,RGB(128,128,128));
	m_PBlanc=new CPen(PS_SOLID,1,RGB(255,255,255));
	m_PNoir=new CPen(PS_SOLID,1,RGB(0,0,0));
	m_PGrisC=new CPen(PS_SOLID,1,RGB(192,192,192));

	m_pMSSansSerif=new CFont;
	m_pMSSansSerif->CreatePointFont(80, "MS Sans Serif", NULL);
	m_pTimesNewRoman=new CFont;
	m_pTimesNewRoman->CreatePointFont(120,"Times New Roman",NULL);
	m_pMsgFont=NULL;

}

CObj_Tab::~CObj_Tab()
{
	if(m_pMSSansSerif)		delete m_pMSSansSerif;
	if(m_pTimesNewRoman)	delete m_pTimesNewRoman;

	if(m_PGrisF)delete m_PGrisF;
	if(m_PBlanc)delete m_PBlanc;
	if(m_PNoir)	delete m_PNoir;
	if(m_PGrisC)delete m_PGrisC;
	LibereObjet();
}

void CObj_Tab::LibereObjet()
{
	if(m_itTableau[TITRE_LIG].m_InfoCel)	delete [] m_itTableau[TITRE_LIG].m_InfoCel;
	m_itTableau[TITRE_LIG].m_InfoCel=NULL;
	if(m_itTableau[TITRE_COL].m_InfoCel)	delete [] m_itTableau[TITRE_COL].m_InfoCel;
	m_itTableau[TITRE_COL].m_InfoCel=NULL;
	if(m_itTableau[CORPS].m_InfoCel)	delete [] m_itTableau[CORPS].m_InfoCel;
	m_itTableau[CORPS].m_InfoCel=NULL;
}

void CObj_Tab::Initialise(short Style,short Format,short DataType,short dxCel,short dyCel,
						  short nLigVisu,short nColVisu,CScrollBar * ScrollH,
						  CScrollBar * ScrollV,CFont * pFont,COLORREF TabColorBg)
{
	m_Style=Style;
	m_Format=Format;
	m_pScrollH=ScrollH;
	m_pScrollV=ScrollV;
	if(TabColorBg)	m_itTableau[CORPS].m_ColorBg=TabColorBg;
	m_itTableau[CORPS].m_dx=dxCel;
	m_itTableau[CORPS].m_dy=dyCel;
	m_itTableau[CORPS].m_pFont=pFont;
	if(m_itTableau[CORPS].m_pFont==NULL)	m_itTableau[CORPS].m_pFont=m_pMSSansSerif;
	m_itTableau[CORPS].m_DataType=DataType;
	m_nbLigVisu=nLigVisu;
	m_nbColVisu=nColVisu;
}

void CObj_Tab::InitialiseTitre(short TypeTitre, short dxTitre, short dyTitre, CFont * pFont, COLORREF TitreColorBg)
{
	switch(TypeTitre)
	{
	case TITRE_LIG:
	case TITRE_COL:
		if(TitreColorBg)	m_itTableau[TypeTitre].m_ColorBg=TitreColorBg;
		m_itTableau[TypeTitre].m_dx=dxTitre;
		m_itTableau[TypeTitre].m_dy=dyTitre;
		m_itTableau[TypeTitre].m_pFont=pFont;
		if(m_itTableau[TypeTitre].m_pFont==NULL)	m_itTableau[TypeTitre].m_pFont=m_pMSSansSerif;
		break;
	default:break;
	}
}

void CObj_Tab::SetTitre(short TypeTitre, CStringArray * TabTitre)
{
	if(TypeTitre==TITRE_COL || TypeTitre==TITRE_LIG)
	{
		m_itTableau[TypeTitre].m_fInit=false;
		if(m_itTableau[TypeTitre].m_InfoCel)	delete [] m_itTableau[TypeTitre].m_InfoCel;
		m_itTableau[TypeTitre].m_InfoCel=new CInfoCellule[1];
		if(!TabTitre->GetSize()){
			InvalidateRect(NULL, 0);
			return;
		}
	}
	switch(TypeTitre)
	{
	case TITRE_LIG:
		m_NbLig=TabTitre->GetSize();
		break;
	case TITRE_COL:
		m_NbCol=TabTitre->GetSize();
		break;
	default:break;
	}
	if(TypeTitre==TITRE_COL || TypeTitre==TITRE_LIG)
	{
		m_itTableau[TypeTitre].m_InfoCel[0].SetSize(TabTitre->GetSize());
		for(short i=0 ; i<TabTitre->GetSize() ; i++)
		{
			m_itTableau[TypeTitre].m_InfoCel[0][i].m_strCel=TabTitre->GetAt(i);
			m_itTableau[TypeTitre].m_InfoCel[0][i].m_RGBCel=m_itTableau[TypeTitre].m_ColorBg;
			m_itTableau[TypeTitre].m_InfoCel[0][i].m_pFont=m_itTableau[TypeTitre].m_pFont;
			m_itTableau[TypeTitre].m_InfoCel[0][i].m_dx=m_itTableau[TypeTitre].m_dx;
			m_itTableau[TypeTitre].m_InfoCel[0][i].m_dy=m_itTableau[TypeTitre].m_dy;
			m_itTableau[TypeTitre].m_InfoCel[0][i].m_Orientation=DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX;
		}
		m_itTableau[TypeTitre].m_fInit=true;
		m_PosActiveLig=0;
		m_PosActiveCol=0;
	}
	InvalidateRect(NULL, 0);
}

void CObj_Tab::SetTableau(short NbLig, CStringArray * TabElements)
{
	if(!m_itTableau[TITRE_LIG].m_fInit)	m_NbLig=0;
	if(!m_itTableau[TITRE_COL].m_fInit)	m_NbCol=0;
	m_PosActiveLig=0;
	m_PosActiveCol=0;
	m_itTableau[CORPS].m_fInit=false;
	if(!TabElements)
	{
		if((m_Style & 0x0400)==OPT_TAB_TRI_COL)
			for(short i=0 ; i<m_NbCol ; i++)m_itTableau[TITRE_COL].m_InfoCel[0][i].m_fSelect=false;
		InvalidateRect(NULL, 0);
		m_sSelection.RemoveAll();
		return;
	}
	m_NbLig = NbLig;
	if(m_itTableau[CORPS].m_InfoCel)	delete [] m_itTableau[CORPS].m_InfoCel;
	if(m_NbLig > 0)
	{
		m_itTableau[CORPS].m_InfoCel = new CInfoCellule[NbLig];
		for(short i=0 ; i<NbLig ; i++)
		{
			if(!TabElements[i].GetSize())	continue;
			m_NbCol=TabElements[i].GetSize();
			m_itTableau[CORPS].m_InfoCel[i].SetSize(m_NbCol);
			for(short j=0 ; j<TabElements[i].GetSize() ; j++)
			{
				m_itTableau[CORPS].m_InfoCel[i][j].m_strCel=TabElements[i][j];
				m_itTableau[CORPS].m_InfoCel[i][j].m_RGBCel=m_itTableau[CORPS].m_ColorBg;
				m_itTableau[CORPS].m_InfoCel[i][j].m_pFont=m_itTableau[CORPS].m_pFont;
				m_itTableau[CORPS].m_InfoCel[i][j].m_DataType=m_itTableau[CORPS].m_DataType;
				m_itTableau[CORPS].m_InfoCel[i][j].m_dx=m_itTableau[CORPS].m_dx;
				m_itTableau[CORPS].m_InfoCel[i][j].m_dy=m_itTableau[CORPS].m_dy;
				m_itTableau[CORPS].m_InfoCel[i][j].m_Orientation=DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX;
			}
		}
		m_itTableau[CORPS].m_fInit=true;
	}
	else
	{
		m_itTableau[CORPS].m_InfoCel = NULL;
	}
	InvalidateRect(NULL, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CObj_Tab, CStatic)
	//{{AFX_MSG_MAP(CObj_Tab)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObj_Tab message handlers

void CObj_Tab::OnPaint() 
{
	if(!m_itTableau[CORPS].m_fInit
	&& !m_itTableau[TITRE_LIG].m_fInit
	&& !m_itTableau[TITRE_COL].m_fInit)
		return;
	CPaintDC dc(this); // device context for painting

	CRect R, Rdest;
	CBitmap hBmpNew, * hBmpOld;
	CDC DC;
	long l, h;

	// copie de l'écran en mémoire ****************
	GetClientRect(&R);

	DC.CreateCompatibleDC(&dc);

	l=R.right-R.left;
	h=R.bottom-R.top;
	hBmpNew.CreateCompatibleBitmap(&dc, l,h);
	hBmpOld=(CBitmap *)DC.SelectObject(&hBmpNew);
	DC.BitBlt(0,0,l,h,0,0,0,WHITENESS);
	// *********************************************
	DC.SetBkMode(TRANSPARENT);
	CBrush brush(m_itTableau[CORPS].m_ColorBg);	
	DC.FillRect(&R, &brush);

	//message dans le fond
	if(!m_Message.IsEmpty())
	{
		DC.SelectObject(m_pMsgFont);
		COLORREF oldRGB=DC.GetTextColor();
		DC.SetTextColor(m_MsgColor);
		DC.DrawText(m_Message, &R, DT_CENTER|DT_NOPREFIX|DT_CALCRECT|DT_WORDBREAK);
		DC.SetTextColor(oldRGB);
	}

	//Calcul  des dimensions pour chaque sections
	SetDimensionsTab(&DC,&R);
	
	//cas ou ya - d'elements que d'espace
//	if(m_nbColVisu>m_NbCol)	m_nbColVisu=m_NbCol;
//	if(m_nbLigVisu>m_NbLig)	m_nbLigVisu=m_NbLig;

	//initialisation des ascenseurs
	InitAscenseurs(m_nbColVisu,m_nbLigVisu);

	//dessin des sections
	if(m_itTableau[CORPS].m_fInit)	    DessineTableau(&DC);
	if(m_itTableau[TITRE_COL].m_fInit)	DessineTitre(TITRE_COL, m_NbCol, &DC);
	if(m_itTableau[TITRE_LIG].m_fInit)	DessineTitre(TITRE_LIG, m_NbLig, &DC);

	//cadre entier
	GetClientRect(&R);
	DC.MoveTo(R.left, R.top);
	DC.LineTo(R.left, R.bottom-1);
	DC.LineTo(R.right-1,R.bottom-1);
	DC.LineTo(R.right-1,R.top);
	DC.LineTo(R.left, R.top);

	// copy de la mem sur ecran
	Rdest=dc.m_ps.rcPaint;
	dc.BitBlt(Rdest.left,Rdest.top,Rdest.right,Rdest.bottom,&DC,Rdest.left,Rdest.top,SRCCOPY);
	dc.SelectObject(hBmpOld);
	DC.DeleteDC();
	// ************************	
	// Do not call CStatic::OnPaint() for painting messages
}


void CObj_Tab::DessineTitre(short titre, short Nb, CDC * dc)
{
	dc->SelectObject(m_itTableau[titre].m_pFont);
	CBrush brush(m_itTableau[titre].m_ColorBg);
	dc->FillRect(&m_itTableau[titre].m_RSection, &brush);

	//cadre
	dc->MoveTo(m_itTableau[titre].m_RSection.left, m_itTableau[titre].m_RSection.top);
	dc->LineTo(m_itTableau[titre].m_RSection.left, m_itTableau[titre].m_RSection.bottom);
	dc->LineTo(m_itTableau[titre].m_RSection.right, m_itTableau[titre].m_RSection.bottom);
	dc->LineTo(m_itTableau[titre].m_RSection.right, m_itTableau[titre].m_RSection.top);
	dc->LineTo(m_itTableau[titre].m_RSection.left, m_itTableau[titre].m_RSection.top);

	short nActive=0, nb=0;
	switch(titre)
	{
	case TITRE_LIG:
		nActive=m_PosActiveLig;
		nb=min(Nb, m_PosActiveLig+m_nbLigVisu);
		break;
	case TITRE_COL:
		nActive=m_PosActiveCol;
		nb=min(Nb, m_PosActiveCol+m_nbColVisu);
		break;
	}
	//titre
	//Pour dessiner la couleur de sélection dans la barre de titre...mais à quoi ca sert???
	for(short i=nActive ; i<nb ; i++)
	{
		CRect R=m_itTableau[titre].m_InfoCel[0][i].m_RCellule;
		for(short s=0 ; s<m_sSelection.GetSize() ; s++)
		{
			switch(m_sSelection[s].m_TypeTitre)
			{
			case TITRE_COL:{
				if(m_itTableau[TITRE_COL].m_fInit)
				{
					if(m_sSelection[s].m_X==i)
					{
						if(m_sSelection[s].m_Color){
							CBrush brush(m_sSelection[s].m_Color);
//							dc->FillRect(&R, &brush);
						}
					}
				}}break;
			case TITRE_LIG:{
				if(m_itTableau[TITRE_LIG].m_fInit)
				{
					if(m_sSelection[s].m_Y==i)
					{
						if(m_sSelection[s].m_Color){
							CBrush brush(m_sSelection[s].m_Color);
							dc->FillRect(&R, &brush);
						}
					}
				}}break;
			}
		}

		switch(m_Format&0x01)
		{
		case FORMAT_TAB_BTN:
			{
			if(m_itTableau[titre].m_InfoCel[0][i].m_Orientation&DT_LEFT)
				R.OffsetRect(2,0);
			else
			if(m_itTableau[titre].m_InfoCel[0][i].m_Orientation&DT_RIGHT)
				R.OffsetRect(-2,0);
			else
				R.OffsetRect(1,0);
			CBrush brush(RGB(192,192,192));
			dc->FillRect(&R, &brush);
			if((m_Style & 0x0c00))
			{
				if(m_itTableau[titre].m_InfoCel[0][i].m_fSelect)
				{
					DessineRect2Pen(dc, &R, m_PNoir, m_PBlanc);
					R.OffsetRect(1,1);
					dc->DrawText(m_itTableau[titre].m_InfoCel[0][i].m_strCel, &R, m_itTableau[titre].m_InfoCel[0][i].m_Orientation);
				}
				else
				{
					DessineRect2Pen(dc, &R, m_PBlanc, m_PNoir);
					dc->DrawText(m_itTableau[titre].m_InfoCel[0][i].m_strCel, &R, m_itTableau[titre].m_InfoCel[0][i].m_Orientation);
				}
			}
			else{
				DessineRect2Pen(dc, &R, m_PBlanc, m_PGrisF);
				dc->DrawText(m_itTableau[titre].m_InfoCel[0][i].m_strCel, &R, m_itTableau[titre].m_InfoCel[0][i].m_Orientation);
			}
			break;
			}
		default:
			dc->MoveTo(R.right, R.top);
			dc->LineTo(R.right, R.bottom);
			//texte
			dc->DrawText(m_itTableau[titre].m_InfoCel[0][i].m_strCel, &R, m_itTableau[titre].m_InfoCel[0][i].m_Orientation);
			break;
		}
		//grille du tableau
		R=m_itTableau[titre].m_InfoCel[0][i].m_RCellule;
		switch(titre)
		{
		case TITRE_LIG:	//ligne
			if(!(m_Style & 0x0020) && (m_Style & 0x1000)){
				dc->MoveTo(R.right, R.top);
				dc->LineTo(m_RectTab.right, R.top);
			}
			break;
		case TITRE_COL:	//colonne
			if(!(m_Style & 0x0010) && (m_Style & 0x1000)){
				dc->MoveTo(R.right, R.bottom);
				dc->LineTo(R.right, m_RectTab.bottom);
			}
			break;
		}
	}
}

void CObj_Tab::DessineTableau(CDC * dc)
{
	dc->SelectObject(m_itTableau[CORPS].m_pFont);
	//couleur
	CBrush brush(m_itTableau[CORPS].m_ColorBg);
	dc->FillRect(&m_itTableau[CORPS].m_RSection, &brush);

	//grille
	for(short i=m_PosActiveLig ; i<min(m_NbLig, m_PosActiveLig+m_nbLigVisu) ; i++)
	{
		for(short j=m_PosActiveCol ; j<min(m_NbCol, m_PosActiveCol+m_nbColVisu) ; j++)
		{
			CRect R=m_itTableau[CORPS].m_InfoCel[i-m_PosActiveLig][j-m_PosActiveCol].m_RCellule;
			//selection
			if(!(m_Style & 0x0020))	R.bottom-=1;
			if(!(m_Style & 0x0010))	R.left+=1;
			for(short s=0 ; s<m_sSelection.GetSize() ; s++)
			{
				switch(m_sSelection[s].m_TypeTitre)
				{
				case CORPS:{
					if(m_sSelection[s].m_X>=0 && m_sSelection[s].m_Y<0)	//colonne entiere
					{
						if(m_sSelection[s].m_X==j)
						{
							if(m_sSelection[s].m_Color){
								CBrush brush(m_sSelection[s].m_Color);
								dc->FillRect(&R, &brush);
							}
						}
					}
					else
					if(m_sSelection[s].m_X<0 && m_sSelection[s].m_Y>=0)	//ligne entiere
					{
						if(m_sSelection[s].m_Y==i)
						{
							if(m_sSelection[s].m_Color){
								CBrush brush(m_sSelection[s].m_Color);
								dc->FillRect(&R, &brush);
							}
						}
					}
					else
					if(m_sSelection[s].m_X>=0 && m_sSelection[s].m_Y>=0)	//celulle particuliere
					{
						if(m_sSelection[s].m_X==j && m_sSelection[s].m_Y==i)
						{
							if(m_sSelection[s].m_Color){
								CBrush brush(m_sSelection[s].m_Color);
								dc->FillRect(&R, &brush);
							}
						}
					}
					else	//tout
					{
						if(m_sSelection[s].m_Color){
							CBrush brush(m_sSelection[s].m_Color);
							dc->FillRect(&R, &brush);
						}
					}}break;
				}
			}
			if(!(m_Style & 0x0020))	R.bottom+=1;
			if(!(m_Style & 0x0010))	R.left-=1;

			if(!(m_Style & 0x0020)){//ligne
				dc->MoveTo(R.left, R.top);
				dc->LineTo(m_RectTab.right, R.top);
			}
			if(!(m_Style & 0x0010)){//colonne
				dc->MoveTo(R.right, R.top);
				dc->LineTo(R.right, m_RectTab.bottom);
			}
			//texte
			UINT nFormat=DT_NOPREFIX|DT_SINGLELINE;
			if(m_itTableau[TITRE_LIG].m_fInit && m_itTableau[TITRE_COL].m_fInit)
			{
				if(m_itTableau[TITRE_LIG].m_InfoCel[0][i].m_Orientation!=m_itTableau[TITRE_COL].m_InfoCel[0][j].m_Orientation)
				{
					nFormat|=m_itTableau[TITRE_LIG].m_InfoCel[0][i].m_Orientation & (DT_TOP|DT_BOTTOM|DT_VCENTER);
					nFormat|=m_itTableau[TITRE_COL].m_InfoCel[0][j].m_Orientation & (DT_LEFT|DT_RIGHT|DT_CENTER);
				}
				else
					nFormat=m_itTableau[TITRE_COL].m_InfoCel[0][j].m_Orientation;
			}
			else
				nFormat=m_itTableau[CORPS].m_InfoCel[i][j].m_Orientation;
			if(nFormat & DT_RIGHT)	//droite
				R.OffsetRect(-4,0);
			else
			if(!(nFormat & DT_CENTER))	//gauche (DL_LEFT vaut 0..pas pratique!)
				R.OffsetRect(4,0);
			dc->DrawText(m_itTableau[CORPS].m_InfoCel[i][j].m_strCel, &R, nFormat);
		}
	}
	//cadre
	if(!(m_Style & 0x1000))
	{
		dc->MoveTo(m_itTableau[CORPS].m_RSection.left, m_itTableau[CORPS].m_RSection.top);
		dc->LineTo(m_itTableau[CORPS].m_RSection.left, m_itTableau[CORPS].m_RSection.bottom);
		dc->LineTo(m_itTableau[CORPS].m_RSection.right, m_itTableau[CORPS].m_RSection.bottom);
		dc->LineTo(m_itTableau[CORPS].m_RSection.right, m_itTableau[CORPS].m_RSection.top);
		dc->LineTo(m_itTableau[CORPS].m_RSection.left, m_itTableau[CORPS].m_RSection.top);
	}
}

// c'est un double cadre de 4 couleurs
void CObj_Tab::Dessine2Rect4Pen(CDC * DC, CRect R, CPen * hP1, CPen * hP2, CPen * hP3, CPen * hP4)
{
	CPen * OldPen;
	// pour faire comme un FrameRect
	R.right--;
	R.bottom--;
	// Dessin...
	OldPen=DC->SelectObject(hP1);
	DC->MoveTo(R.left,R.top);
	DC->LineTo(R.right,R.top);
	DC->MoveTo(R.left,R.top);
	DC->LineTo(R.left,R.bottom+1);

	DC->SelectObject(hP2);
	DC->MoveTo(R.left+1,R.top+1);
	DC->LineTo(R.right,R.top+1);
	DC->MoveTo(R.left+1,R.top+1);
	DC->LineTo(R.left+1,R.bottom);

	DC->SelectObject(hP3);
	DC->MoveTo(R.left+1,R.bottom-1);
	DC->LineTo(R.right-1,R.bottom-1);
	DC->MoveTo(R.right-1,R.bottom-1);
	DC->LineTo(R.right-1,R.top);

	DC->SelectObject(hP4);
	DC->MoveTo(R.left,R.bottom);
	DC->LineTo(R.right,R.bottom);
//	DC->MoveTo(R.right,R.bottom);
	DC->LineTo(R.right,R.top-1);
	DC->SelectObject(OldPen);
}

// c'est un double cadre de deux couleurs
void CObj_Tab::Dessine2Rect2Pen(CDC * DC, CRect R, CPen * hP1, CPen * hP2)
{
	CPen * OldPen;
	// pour faire comme un FrameRect
	R.right--;
	R.bottom--;

	// Dessin...
	OldPen=DC->SelectObject(hP1);
	DC->MoveTo(R.left,R.top);
	DC->LineTo(R.right,R.top);
	DC->MoveTo(R.left,R.top+1);
	DC->LineTo(R.right-1,R.top+1);
	DC->MoveTo(R.left,R.top);
	DC->LineTo(R.left,R.bottom);
	DC->MoveTo(R.left+1,R.top);
	DC->LineTo(R.left+1,R.bottom-1);

	DC->SelectObject(hP2);
	DC->MoveTo(R.right,R.top);
	DC->LineTo(R.right,R.bottom+1);
	DC->MoveTo(R.right-1,R.top+1);
	DC->LineTo(R.right-1,R.bottom+1);
	DC->MoveTo(R.left,R.bottom);
	DC->LineTo(R.right+1,R.bottom);
	DC->MoveTo(R.left+1,R.bottom-1);
	DC->LineTo(R.right+1,R.bottom-1);
	DC->SelectObject(OldPen);
}

// c'est un simple cadre de deux couleurs
void CObj_Tab::DessineRect2Pen(CDC * DC, CRect R, CPen * hP1, CPen * hP2)
{
	CPen * OldPen;
	// pour faire comme un FrameRect
	R.right--;
	R.bottom--;

	// Dessin...
	OldPen=DC->SelectObject(hP1);
	DC->MoveTo(R.left,R.top);
	DC->LineTo(R.right,R.top);
	DC->MoveTo(R.left,R.top);
	DC->LineTo(R.left,R.bottom);

	DC->SelectObject(hP2);
	DC->MoveTo(R.right,R.top);
	DC->LineTo(R.right,R.bottom+1);
	DC->MoveTo(R.left,R.bottom);
	DC->LineTo(R.right+1,R.bottom);
	DC->SelectObject(OldPen);
}
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
void CObj_Tab::InitAscenseurs(short nbColVisu, short nbLigVisu)
{
	//init de l'ascenseur Hor...
	if(m_pScrollH)
	{
		if(m_NbCol<=nbColVisu)	m_pScrollH->EnableScrollBar(ESB_DISABLE_BOTH);
		else
		{
			m_pScrollH->EnableScrollBar(ESB_ENABLE_BOTH);
			m_pScrollH->SetScrollRange(0, m_NbCol-min(m_NbCol, nbColVisu), 0);
			m_pScrollH->SetScrollPos(m_PosActiveCol,TRUE);
		}
	}

	//init de l'ascenseur Ver...
	if(m_pScrollV)
	{
		if(m_NbLig<=nbLigVisu)	m_pScrollV->EnableScrollBar(ESB_DISABLE_BOTH);
		else
		{
			m_pScrollV->EnableScrollBar(ESB_ENABLE_BOTH);
			m_pScrollV->SetScrollRange(0, m_NbLig-min(m_NbLig, nbLigVisu), 0);
			m_pScrollV->SetScrollPos(m_PosActiveLig,TRUE);
		}
	}
}

short CObj_Tab::MAJPosScrollV(UINT nSBCode, UINT nPos, short flag)
{
	int i,mini,maxi;

	i=m_pScrollV->GetScrollPos();
	m_pScrollV->GetScrollRange(&mini,&maxi);

	switch(nSBCode) {
		case SB_BOTTOM:		m_pScrollV->SetScrollPos(maxi,1); break;
		case SB_ENDSCROLL:	break;
		case SB_LINEDOWN:	m_pScrollV->SetScrollPos(min(i+1, maxi),1); break;
		case SB_LINEUP:		m_pScrollV->SetScrollPos(max(i-1, mini),1); break;
		case SB_PAGEDOWN:	m_pScrollV->SetScrollPos(min((i+min(m_NbLig, m_nbLigVisu)), maxi),1); break;
		case SB_PAGEUP:		m_pScrollV->SetScrollPos(max((i-min(m_NbLig, m_nbLigVisu)), mini),1); break;
		case SB_TOP:		m_pScrollV->SetScrollPos(mini,1); break;
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:	m_pScrollV->SetScrollPos(nPos,1); break;

	}
	short pos=m_pScrollV->GetScrollPos();
	if(flag){
		m_PosActiveLig=pos;
		InvalidateRect(NULL,0);
	}
	return pos;
}

short CObj_Tab::MAJPosScrollH(UINT nSBCode, UINT nPos, short flag)
{
	int i,mini,maxi;

	i=m_pScrollH->GetScrollPos();
	m_pScrollH->GetScrollRange(&mini,&maxi);

	switch(nSBCode) {
		case SB_LEFT:		m_pScrollH->SetScrollPos(mini,1); break;
		case SB_ENDSCROLL:break;
		case SB_LINELEFT:	m_pScrollH->SetScrollPos(max(i-1, mini),1); break;
		case SB_LINERIGHT:	m_pScrollH->SetScrollPos(min(i+1, maxi),1); break;
		case SB_PAGELEFT:	m_pScrollH->SetScrollPos(max((i-min(m_NbCol, m_nbColVisu)), mini),1); break;
		case SB_PAGERIGHT:	m_pScrollH->SetScrollPos(min((i+min(m_NbCol, m_nbColVisu)), maxi),1); break;
		case SB_RIGHT:		m_pScrollH->SetScrollPos(maxi,1); break;
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:	m_pScrollH->SetScrollPos(nPos,1); break;
	}

	short pos=m_pScrollH->GetScrollPos();
	if(flag){
		m_PosActiveCol=pos;
		InvalidateRect(NULL,0);
	}
	return(pos);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void CObj_Tab::SetDimensionsTab(CDC * dc, CRect * pR)
{
	CSize sztmp=0, size=0;
	m_RectTab=pR;
	//titre ligne gauche/groite
	//	 ______ _______ ______
	//	|______|_col1__|_col2_.......
	//	|_lig1_|_______|______......
	//	<- dx ->
	if(m_itTableau[TITRE_LIG].m_fInit)
	{
		dc->SelectObject(m_itTableau[TITRE_LIG].m_pFont);
		m_itTableau[TITRE_LIG].m_RSection.left=pR->left+1;
		if(m_itTableau[TITRE_LIG].m_dx){
			m_itTableau[TITRE_LIG].m_RSection.right=pR->left+m_itTableau[TITRE_LIG].m_dx;
		}
		else
		{
			for(short i=0 ; i<m_NbLig ; i++)
			{
				sztmp=dc->GetTextExtent(m_itTableau[TITRE_LIG].m_InfoCel[0][i].m_strCel);
				if(sztmp.cx+MARGE_TAB_X>m_itTableau[TITRE_LIG].m_dx)
					m_itTableau[TITRE_LIG].m_dx=(short)sztmp.cx+MARGE_TAB_X;
				if(sztmp.cx+MARGE_TAB_X>m_itTableau[TITRE_LIG].m_InfoCel[0][i].m_dx)
					m_itTableau[TITRE_LIG].m_InfoCel[0][i].m_dx=(short)sztmp.cx+MARGE_TAB_X;
			}
			m_itTableau[TITRE_LIG].m_RSection.right=pR->left+m_itTableau[TITRE_LIG].m_dx;
		}
	}

	//titre colonne gauche/groite
	//	 ______ _______ ____	  ______
	//	|_####_|_col1__|_col2....|_col3_|
	//	|_lig1_|_______|____.....|______|
	//		   <----------- dx --------->
	if(m_itTableau[TITRE_COL].m_fInit)
	{
		sztmp=0; size=0;
		dc->SelectObject(m_itTableau[TITRE_COL].m_pFont);
		if(m_itTableau[TITRE_LIG].m_fInit)
			m_itTableau[TITRE_COL].m_RSection.left=m_itTableau[TITRE_LIG].m_RSection.right;
		else
			m_itTableau[TITRE_COL].m_RSection.left=pR->left;
		if(m_itTableau[TITRE_COL].m_dx){
			m_itTableau[TITRE_COL].m_RSection.right=m_itTableau[TITRE_COL].m_RSection.left+m_itTableau[TITRE_COL].m_dx;
		}
		else
		{
			for(short i=0 ; i<m_NbCol ; i++)
			{
				sztmp=dc->GetTextExtent(m_itTableau[TITRE_COL].m_InfoCel[0][i].m_strCel);
				if(sztmp.cx+MARGE_TAB_X>m_itTableau[TITRE_COL].m_InfoCel[0][i].m_dx){
					m_itTableau[TITRE_COL].m_InfoCel[0][i].m_dx=(short)sztmp.cx+MARGE_TAB_X;
					size.cx+=sztmp.cx+MARGE_TAB_X;
				}
				else size.cx+=m_itTableau[TITRE_COL].m_InfoCel[0][i].m_dx;
			}
			m_itTableau[TITRE_COL].m_dx=(short)size.cx;
			m_itTableau[TITRE_COL].m_RSection.right=m_itTableau[TITRE_COL].m_RSection.left+size.cx;
		}
		//pas de cadre en dehors des limites si < au rect
		if(!(m_Style & 0x1000))	m_RectTab.right=m_itTableau[TITRE_COL].m_RSection.right;

		//titre colonne haut/bas
		//	 ______ _______ ____	  ______
		//dy|_####_|_col1__|_col2....|_col3_|
		//	|_lig1_|_______|____.....|______|
		//
		sztmp=0; size=0;
		m_itTableau[TITRE_COL].m_RSection.top=pR->top+1;
		if(m_itTableau[TITRE_COL].m_dy){
			m_itTableau[TITRE_COL].m_RSection.bottom=pR->top+m_itTableau[TITRE_COL].m_dy;
		}
		else
		{
			for(short i=0 ; i<m_NbCol ; i++)
			{
				sztmp=dc->GetTextExtent(m_itTableau[TITRE_COL].m_InfoCel[0][i].m_strCel);
				if(sztmp.cy+MARGE_TAB_Y>m_itTableau[TITRE_COL].m_dy)
					m_itTableau[TITRE_COL].m_dy=(short)sztmp.cy+MARGE_TAB_Y;
				if(sztmp.cy+MARGE_TAB_Y>m_itTableau[TITRE_COL].m_InfoCel[0][i].m_dy)
					m_itTableau[TITRE_COL].m_InfoCel[0][i].m_dy=(short)sztmp.cy+MARGE_TAB_Y;
			}
			m_itTableau[TITRE_COL].m_RSection.bottom=pR->top+m_itTableau[TITRE_COL].m_dy;
		}
	}

	//titre ligne haut/bas
	//	 ______ _______ ____	  ______
	//	|_####_|_col1__|_col2....|_col3_|
	//  |_lig1_|_______|____.....|______|
	//dy|_lig2_|_______|____.....|______|
	//  |_lig3_|_______|____.....|______|
	//
	if(m_itTableau[TITRE_LIG].m_fInit)
	{
		sztmp=0; size=0;
		dc->SelectObject(m_itTableau[TITRE_LIG].m_pFont);
		if(m_itTableau[TITRE_COL].m_fInit)
			m_itTableau[TITRE_LIG].m_RSection.top=m_itTableau[TITRE_COL].m_RSection.bottom;
		else{
			m_itTableau[TITRE_LIG].m_RSection.top=pR->top;
		}
		if(m_itTableau[TITRE_LIG].m_dy){
			m_itTableau[TITRE_LIG].m_RSection.bottom=m_itTableau[TITRE_LIG].m_RSection.top+m_itTableau[TITRE_LIG].m_dy;
		}
		else
		{
			for(short i=0 ; i<m_NbLig ; i++)
			{
				sztmp=dc->GetTextExtent(m_itTableau[TITRE_LIG].m_InfoCel[0][i].m_strCel);
				if(sztmp.cy+MARGE_TAB_Y>m_itTableau[TITRE_LIG].m_InfoCel[0][i].m_dy){
					m_itTableau[TITRE_LIG].m_InfoCel[0][i].m_dy=(short)sztmp.cy+MARGE_TAB_Y;
					size.cy+=sztmp.cy+MARGE_TAB_Y;
				}
				else size.cy+=m_itTableau[TITRE_LIG].m_InfoCel[0][i].m_dy;
			}
			m_itTableau[TITRE_LIG].m_dy=(short)size.cy;
			m_itTableau[TITRE_LIG].m_RSection.bottom=m_itTableau[TITRE_LIG].m_RSection.top+size.cy;
		}
	}

	//tableau colonne
	//	 ______ _______ ____	  ______
	//	|_####_|_col1__|_col2....|_col3_|
	//  |_lig1_|_______|____.....|______|
	//  |_lig2_|_______|____.....|______|
	//  |_lig3_|_______|____.....|______|
	//		   <----------- dx --------->
	if(m_itTableau[CORPS].m_fInit)
	{
		sztmp=0; size=0;
		dc->SelectObject(m_itTableau[CORPS].m_pFont);
		if(m_itTableau[TITRE_COL].m_fInit)
			m_itTableau[CORPS].m_RSection.left=m_itTableau[TITRE_COL].m_RSection.left;
		else if(m_itTableau[TITRE_LIG].m_fInit)
			m_itTableau[CORPS].m_RSection.left=m_itTableau[TITRE_LIG].m_RSection.right+1;
		else m_itTableau[CORPS].m_RSection.left=pR->left;
		if(m_itTableau[CORPS].m_dx){
			m_itTableau[CORPS].m_RSection.right=m_itTableau[CORPS].m_RSection.left+m_itTableau[CORPS].m_dx;
			if(m_itTableau[TITRE_COL].m_fInit)
				m_itTableau[TITRE_COL].m_RSection.right=m_itTableau[CORPS].m_RSection.right;
		}
		else
		{
			CSize sz=0;
			for(short i=0 ; i<m_NbCol ; i++)
			{
				sztmp=0; size=0;
				for(short j=0 ; j<m_NbLig ; j++)
				{
					sztmp=dc->GetTextExtent(m_itTableau[CORPS].m_InfoCel[j][i].m_strCel);
					if(sztmp.cx+MARGE_TAB_X>size.cx)	size.cx=sztmp.cx+MARGE_TAB_X;
				}
				if(m_itTableau[TITRE_COL].m_fInit)
				{
					if(m_itTableau[TITRE_COL].m_InfoCel[0][i].m_dx>(short)size.cx)
					{
						for(j=0 ; j<m_NbLig ; j++)
							m_itTableau[CORPS].m_InfoCel[j][i].m_dx=m_itTableau[TITRE_COL].m_InfoCel[0][i].m_dx;
					}
					else
					{
						for(j=0 ; j<m_NbLig ; j++)
							m_itTableau[CORPS].m_InfoCel[j][i].m_dx=(short)size.cx;
						m_itTableau[TITRE_COL].m_InfoCel[0][i].m_dx=(short)size.cx;
					}
				}
				else
				{
					for(j=0 ; j<m_NbLig ; j++)
						m_itTableau[CORPS].m_InfoCel[j][i].m_dx=(short)size.cx;
				}
				sz.cx+=size.cx;
			}
			if(m_itTableau[TITRE_COL].m_fInit)
				m_itTableau[CORPS].m_RSection.right=m_itTableau[TITRE_COL].m_RSection.right;
			else m_itTableau[CORPS].m_RSection.right=m_itTableau[CORPS].m_RSection.left+sz.cx;
		}	
		
		//tableau ligne
		//	 ______ _______ ____	  ______
		//	|_####_|_col1__|_col2....|_col3_|
		//  |_lig1_|_______|____.....|______| /|\
		//  |_lig2_|_______|____.....|______| dy
		//  |_lig3_|_______|____.....|______| \|/
		//	
		sztmp=0; size=0;
		if(m_itTableau[TITRE_LIG].m_fInit)
			m_itTableau[CORPS].m_RSection.top=m_itTableau[TITRE_LIG].m_RSection.top;
		else if(m_itTableau[TITRE_COL].m_fInit)
			m_itTableau[CORPS].m_RSection.top=m_itTableau[TITRE_COL].m_RSection.bottom+1;
		else m_itTableau[CORPS].m_RSection.top=pR->top;
		if(m_itTableau[CORPS].m_dy)
		{
			m_itTableau[CORPS].m_RSection.bottom=m_itTableau[CORPS].m_RSection.top+m_itTableau[CORPS].m_dy;
			if(m_itTableau[TITRE_LIG].m_fInit)
				m_itTableau[TITRE_LIG].m_RSection.bottom=m_itTableau[CORPS].m_RSection.bottom;
		}
		else
		{
			CSize sz=0;
			for(short i=0 ; i<m_NbLig ; i++)
			{
				sztmp=0; size=0;
				for(short j=0 ; j<m_NbCol ; j++)
				{
					sztmp=dc->GetTextExtent(m_itTableau[CORPS].m_InfoCel[i][j].m_strCel);
					if(sztmp.cy>size.cy)	size.cy=sztmp.cy;
				}
				if(m_itTableau[TITRE_LIG].m_fInit)
				{
					if(m_itTableau[TITRE_LIG].m_InfoCel[0][i].m_dy>(short)size.cy)
					{
						for(j=0 ; j<m_NbCol ; j++)
							m_itTableau[CORPS].m_InfoCel[i][j].m_dy=m_itTableau[TITRE_LIG].m_InfoCel[0][i].m_dy;
					}
					else
					{
						for(j=0 ; j<m_NbCol ; j++)
							m_itTableau[CORPS].m_InfoCel[i][j].m_dy=(short)size.cy;
						m_itTableau[TITRE_LIG].m_InfoCel[0][i].m_dy=(short)size.cy;
					}
				}
				else
				{
					for(j=0 ; j<m_NbCol ; j++)
						m_itTableau[CORPS].m_InfoCel[i][j].m_dy=(short)size.cy;
				}
				sz.cy+=size.cy;
			}
			if(m_itTableau[TITRE_LIG].m_fInit)
				m_itTableau[CORPS].m_RSection.bottom=m_itTableau[TITRE_LIG].m_RSection.bottom;
			else m_itTableau[CORPS].m_RSection.bottom=m_itTableau[CORPS].m_RSection.top+sz.cy;
		}
	}

	//Rect des cellules titreS et tableau
	//	 ______ _______ ____	  ______
	//dy|_####_|_col1__|_col2....|_col3_|
	//  |_lig1_|_______|____.....|______|
	//  |_lig2_|_______|____.....|______|
	//  |_lig3_|_______|____.....|______|dy
	//	<- dx ->				 <- dx ->
	//
	short dy=0;
	if(m_itTableau[CORPS].m_fInit)
		dy=(short)m_itTableau[CORPS].m_RSection.top;
	else if(m_itTableau[TITRE_LIG].m_fInit)
		dy=(short)m_itTableau[TITRE_LIG].m_RSection.top;

	int nblig=0;
	if(m_itTableau[CORPS].m_fInit)			nblig=m_NbLig;
	else if(m_itTableau[TITRE_LIG].m_fInit)	nblig=m_NbLig;
	else if(m_itTableau[TITRE_COL].m_fInit)	nblig=1;
	for(short i=0 ; i<nblig ; i++)
	{
		short dx=0;
		if(m_itTableau[CORPS].m_fInit)
			dx=(short)m_itTableau[CORPS].m_RSection.left;
		else if(m_itTableau[TITRE_COL].m_fInit)
			dx=(short)m_itTableau[TITRE_COL].m_RSection.left;

		//titre ligne
		if(m_itTableau[TITRE_LIG].m_fInit)
		{
			m_itTableau[TITRE_LIG].m_InfoCel[0][i].m_RCellule.left=m_itTableau[TITRE_LIG].m_RSection.left;
			m_itTableau[TITRE_LIG].m_InfoCel[0][i].m_RCellule.right=m_itTableau[TITRE_LIG].m_RSection.right;
			m_itTableau[TITRE_LIG].m_InfoCel[0][i].m_RCellule.top=dy;
		}
		int nbcol=0;
		if(m_itTableau[CORPS].m_fInit)			nbcol=m_NbCol;
		else if(m_itTableau[TITRE_LIG].m_fInit)	nbcol=1;
		else if(m_itTableau[TITRE_COL].m_fInit)	nbcol=m_NbCol;
		for(short j=0 ; j<nbcol ; j++)
		{
			//tableau
			if(m_itTableau[CORPS].m_fInit)
				m_itTableau[CORPS].m_InfoCel[i][j].m_RCellule.left=dx;
			if(m_itTableau[TITRE_COL].m_fInit)
				m_itTableau[TITRE_COL].m_InfoCel[0][j].m_RCellule.left=dx;
			if(m_itTableau[CORPS].m_fInit)
				dx+=m_itTableau[CORPS].m_InfoCel[i][j].m_dx;
			else if(m_itTableau[TITRE_COL].m_fInit)
				dx+=m_itTableau[TITRE_COL].m_InfoCel[0][j].m_dx;
			if(m_itTableau[CORPS].m_fInit)
				m_itTableau[CORPS].m_InfoCel[i][j].m_RCellule.right=dx;
			if(m_itTableau[TITRE_COL].m_fInit)
				m_itTableau[TITRE_COL].m_InfoCel[0][j].m_RCellule.right=dx;
			
			if(m_itTableau[CORPS].m_fInit){
				m_itTableau[CORPS].m_InfoCel[i][j].m_RCellule.top=dy;
				m_itTableau[CORPS].m_InfoCel[i][j].m_RCellule.bottom=dy+m_itTableau[CORPS].m_InfoCel[i][j].m_dy;
			}

			//titre colonne
			if(m_itTableau[TITRE_COL].m_fInit){
				m_itTableau[TITRE_COL].m_InfoCel[0][j].m_RCellule.top=m_itTableau[TITRE_COL].m_RSection.top;
				m_itTableau[TITRE_COL].m_InfoCel[0][j].m_RCellule.bottom=m_itTableau[TITRE_COL].m_RSection.bottom;
				m_itTableau[TITRE_COL].m_InfoCel[0][j].m_dx=m_itTableau[TITRE_COL].m_InfoCel[0][j].m_RCellule.Width();
				m_itTableau[TITRE_COL].m_InfoCel[0][j].m_dy=m_itTableau[TITRE_COL].m_InfoCel[0][j].m_RCellule.Height();
			}
		}
		if(m_itTableau[CORPS].m_fInit)
			m_itTableau[CORPS].m_RSection.right=dx;
		if(m_itTableau[TITRE_COL].m_fInit)
			m_itTableau[TITRE_COL].m_RSection.right=dx;
		//titre ligne
		if(m_itTableau[TITRE_LIG].m_fInit)
		{
			m_itTableau[TITRE_LIG].m_InfoCel[0][i].m_RCellule.bottom=dy+m_itTableau[CORPS].m_InfoCel[i][0].m_dy;
			m_itTableau[TITRE_LIG].m_InfoCel[0][i].m_dx=m_itTableau[TITRE_LIG].m_InfoCel[0][i].m_RCellule.Width();
			m_itTableau[TITRE_LIG].m_InfoCel[0][i].m_dy=m_itTableau[TITRE_LIG].m_InfoCel[0][i].m_RCellule.Height();
		}
		if(m_itTableau[CORPS].m_fInit)
			dy+=m_itTableau[CORPS].m_InfoCel[i][j-1].m_dy;
		else if(m_itTableau[TITRE_LIG].m_fInit)
			dy+=m_itTableau[TITRE_LIG].m_InfoCel[0][i].m_dy;
	}
	if(m_itTableau[CORPS].m_fInit)
		m_itTableau[CORPS].m_RSection.bottom=dy;
	if(m_itTableau[TITRE_LIG].m_fInit)
		m_itTableau[TITRE_LIG].m_RSection.bottom=dy;

	if(m_itTableau[TITRE_COL].m_fInit){
		if(!(m_Style & 0x1000))	m_RectTab.right=m_itTableau[TITRE_COL].m_RSection.right;
	}
	else if(m_itTableau[CORPS].m_fInit){
		if(!(m_Style & 0x1000))	m_RectTab.right=m_itTableau[CORPS].m_RSection.right;
	}
	
	if(m_itTableau[TITRE_LIG].m_fInit){
		if(!(m_Style & 0x1000))	m_RectTab.bottom=m_itTableau[TITRE_LIG].m_RSection.bottom;
	}
	else if(m_itTableau[CORPS].m_fInit){
		if(!(m_Style & 0x1000))	m_RectTab.bottom=m_itTableau[CORPS].m_RSection.bottom;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void CObj_Tab::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
//	CObj_Tab::OnLButtonDown(nFlags, point);
	CStatic::OnLButtonDblClk(nFlags, point);
}

void CObj_Tab::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(!m_itTableau[CORPS].m_fInit)	return;
	SInfoSelection Select;
	Select.m_Color=CLR_TAB_SELECTION;
	CRect Rhit;
	if(m_itTableau[CORPS].m_RSection.PtInRect(point))
	{
		Rhit=m_itTableau[CORPS].m_RSection;
		Select.m_TypeTitre=CORPS;
		m_fCapture=true;
	}
	if(m_itTableau[TITRE_LIG].m_fInit)
	{
		if(m_itTableau[TITRE_LIG].m_RSection.PtInRect(point))
		{
			Rhit=m_itTableau[TITRE_LIG].m_RSection;
			Select.m_TypeTitre=TITRE_LIG;
			m_fCapture=true;
			for(short i=0 ; i<m_NbLig ; i++)
				m_itTableau[TITRE_LIG].m_InfoCel[0][i].m_fSelect=false;
		}
	}
	if(m_itTableau[TITRE_COL].m_fInit)
	{
		if(m_itTableau[TITRE_COL].m_RSection.PtInRect(point))
		{
			Rhit=m_itTableau[TITRE_COL].m_RSection;
			Select.m_TypeTitre=TITRE_COL;
			m_fCapture=true;
			for(short i=0 ; i<m_NbCol ; i++)
				m_itTableau[TITRE_COL].m_InfoCel[0][i].m_fSelect=false;
		}
	}

	switch(m_Style&0x0001)
	{
	case STYLE_TAB_DEFAUT:
		switch(Select.m_TypeTitre)
		{
		case TITRE_LIG:
			{
				if((m_Style & 0x0800)==OPT_TAB_TRI_LIG)
				{
					for(short i=m_PosActiveLig;i<min(m_NbLig, m_PosActiveLig+m_nbLigVisu);i++)
					{
						if(m_itTableau[Select.m_TypeTitre].m_InfoCel[i][0].m_RCellule.PtInRect(point))
						{
							Select.m_X=-1;
							Select.m_Y=i;
							m_sSelection.Add(Select);
							m_itTableau[Select.m_TypeTitre].m_InfoCel[i][0].m_fSelect=true;
							OnSort(Select.m_TypeTitre, Select.m_X);
						}
					}
				}
			}
			break;
		case TITRE_COL:
			{
				if((m_Style & 0x0400)==OPT_TAB_TRI_COL)
				{
					for(short i=m_PosActiveCol;i<min(m_NbCol,m_PosActiveCol+m_nbColVisu);i++)
					{
						if(m_itTableau[Select.m_TypeTitre].m_InfoCel[0][i].m_RCellule.PtInRect(point))
						{
							Select.m_X=i;
							Select.m_Y=-1;
							m_sSelection.Add(Select);
							m_itTableau[Select.m_TypeTitre].m_InfoCel[0][i].m_fSelect=true;
							OnSort(Select.m_TypeTitre, Select.m_X);
						}
					}
				}
			}
			break;
		case CORPS:
			if((m_Style & 0x0080) == OPT_TAB_SELECT_LIG)
			{
				for(short i = 0 ; i < min(m_NbLig, m_nbLigVisu) ; i++)
				{
					for(short c = 0 ; c < min(m_NbCol,m_nbColVisu) ; c++)
					{
						if(m_itTableau[Select.m_TypeTitre].m_InfoCel[i][c].m_RCellule.PtInRect(point))
						{
							OnLeftClick(i + m_PosActiveLig, c + m_PosActiveCol);
							break;
						}
					}
					if(c < m_nbColVisu)
					{
						short nb = m_sSelection.GetSize();
						for(short s = 0 ; s < nb ; s++)
						{
							if(m_sSelection[s].m_Y == i + m_PosActiveLig)
							{
								for(short c = 0 ; c < m_NbCol ; c++)
								{
									m_itTableau[Select.m_TypeTitre].m_InfoCel[m_sSelection[s].m_Y][c].m_fSelect=false;
								}
								m_sSelection.RemoveAt(s);
								s = -1;
								break;
							}
						}
						if(s >= 0)
						{
							Select.m_X = -1;
							Select.m_Y = i + m_PosActiveLig;
							m_sSelection.Add(Select);
							for(short c = 0 ; c < m_NbCol ; c++)
							{
								m_itTableau[Select.m_TypeTitre].m_InfoCel[Select.m_Y][c].m_fSelect = true;
							}
						}
					}
				}
			}
			break;
		}
	}
	InvalidateRect(NULL, 0);
	CStatic::OnLButtonDown(nFlags, point);
}

void CObj_Tab::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(!m_itTableau[CORPS].m_fInit)	return;
	if(!m_fCapture)	return;
	m_fCapture=false;
	InvalidateRect(NULL, 0);
	CStatic::OnLButtonUp(nFlags, point);
}

void CObj_Tab::OnSort(short TypeTitre, short NroTitre)
{
	CWaitCursor wait;
	InvalidateRect(NULL, 0);
}

void CObj_Tab::SetFormatTitre(short TypeTitre, short NroTitre, short dxTitre, short dyTitre, UINT Centrage, CFont * pFont, COLORREF ColorCel, bool flag)
{
	if(!m_itTableau[TITRE_LIG].m_fInit && !m_itTableau[TITRE_COL].m_fInit)	return;
	short n=NroTitre;
	m_itTableau[TypeTitre].m_InfoCel[0][n].m_dx=dxTitre;
	m_itTableau[TypeTitre].m_InfoCel[0][n].m_dy=dyTitre;

	UINT nFormat;
/*f(Centrage & GAUCHE){
		nFormat|=DT_LEFT;
		if(Centrage & CENTRE)	nFormat|=DT_VCENTER;
	}
	if(Centrage & DROITE){
		nFormat|=DT_RIGHT;
		if(Centrage & CENTRE)	nFormat|=DT_VCENTER;
	}
	if(Centrage & HAUT){
		nFormat|=DT_TOP;
		if(Centrage & CENTRE)	nFormat|=DT_CENTER;
	}
	if(Centrage & BAS){
		nFormat|=DT_BOTTOM;
		if(Centrage & CENTRE)	nFormat|=DT_CENTER;
	}
	if(Centrage==CENTRE)	nFormat|=DT_CENTER|DT_VCENTER;
*/
	if(!Centrage)	nFormat=DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX;
	else nFormat=Centrage;
	m_itTableau[TypeTitre].m_InfoCel[0][n].m_Orientation=nFormat;
	if(pFont)		m_itTableau[TypeTitre].m_InfoCel[0][n].m_pFont=pFont;
	if(ColorCel)	m_itTableau[TypeTitre].m_InfoCel[0][n].m_RGBCel=ColorCel;
	if(flag)	InvalidateRect(NULL,0);
}

void CObj_Tab::SelectTitre(short TypeTitre, short NroTitre, bool flag)
{
	SInfoSelection Select;
	Select.m_X=NroTitre;
	Select.m_Y=-1;
	Select.m_TypeTitre=TypeTitre;
	Select.m_Color=0;
	m_sSelection.Add(Select);
	if((m_Style & 0x0400)==OPT_TAB_TRI_COL)
	{
		for(short i=0 ; i<m_NbCol ; i++){
			if(i==NroTitre)	m_itTableau[TypeTitre].m_InfoCel[0][NroTitre].m_fSelect=true;
			else			m_itTableau[TITRE_COL].m_InfoCel[0][i].m_fSelect=false;
		}
	}
	if(flag)	InvalidateRect(NULL,0);
}

void CObj_Tab::PutMsgInToTab(CString Msg, CFont * pFont, COLORREF TxtColor, bool flag)
{
	m_Message=Msg;
	if(!pFont)	m_pMsgFont=m_pMSSansSerif;
	else		m_pMsgFont=pFont;
	m_MsgColor=TxtColor;
	if(flag)	InvalidateRect(NULL,0);
}

void CObj_Tab::RAZSelection(short TypeTitre, short CelX, short CelY, bool fUpdate)
{
	short n=m_sSelection.GetSize();
	for(short i=n-1;i>=0;i--)
	{
		switch(TypeTitre)
		{
			case TITRE_COL:
				if(CelX==-1 && CelY==-1){
					m_sSelection.RemoveAll();
					break;
				}
				if(m_sSelection[i].m_X==CelX)		m_sSelection.RemoveAt(i);
				m_itTableau[TypeTitre].m_InfoCel[0][CelX].m_fSelect=false;
				break;
			case TITRE_LIG:
				if(CelX==-1 && CelY==-1){
					m_sSelection.RemoveAll();
					break;
				}
				if(m_sSelection[i].m_Y==CelY)		m_sSelection.RemoveAt(i);
				m_itTableau[TypeTitre].m_InfoCel[CelY][0].m_fSelect=false;
				break;
			case CORPS:
				if(CelX==-1 && CelY==-1){
					m_sSelection.RemoveAll();
					break;
				}
				if(m_sSelection[i].m_X==CelX && m_sSelection[i].m_Y==CelY)
				{
					m_sSelection.RemoveAt(i);
					if(CelX==-1 && CelY>=0){
						for(short c=0;c<m_NbCol;c++)
							m_itTableau[TypeTitre].m_InfoCel[i][c].m_fSelect=false;
					}
					if(CelY==-1 && CelX>=0){
						for(short l=0;l<m_NbLig;l++)
							m_itTableau[TypeTitre].m_InfoCel[l][CelX].m_fSelect=false;
					}
					if(CelY>=0 && CelX>=0)	m_itTableau[TypeTitre].m_InfoCel[CelY][CelX].m_fSelect=false;
				}
				break;
		}
	}
	if(fUpdate)	InvalidateRect(NULL,0);
}

void CObj_Tab::SetSelection(short TypeTitre, short CelX, short CelY, COLORREF SelColor, bool fUpdate)
{
	SInfoSelection Select;
	switch(TypeTitre)
	{
		case TITRE_COL:
			Select.m_TypeTitre=TypeTitre;
			Select.m_X=CelX;
			Select.m_Y=-1;
			Select.m_Color=SelColor;
			m_sSelection.Add(Select);
			break;
		case TITRE_LIG:
			Select.m_TypeTitre=TypeTitre;
			Select.m_X=-1;
			Select.m_Y=CelY;
			Select.m_Color=SelColor;
			m_sSelection.Add(Select);
			break;
		case CORPS:
			Select.m_TypeTitre=TypeTitre;
			Select.m_X=CelX;
			Select.m_Y=CelY;
			Select.m_Color=SelColor;
			m_sSelection.Add(Select);
			break;
	}
	if(fUpdate)	InvalidateRect(NULL,0);
}

void CObj_Tab::SetFormatCellule(short CelX, short CelY, short Format, short DataType, UINT Orientation, CFont * pFont, COLORREF ColorCel, bool fUpdate)
{
	if(CelX>=0 && CelY>=0)	//cellule particulière
	{
		if(ColorCel)m_itTableau[CORPS].m_InfoCel[CelY][CelX].m_RGBCel=ColorCel;
		if(pFont)	m_itTableau[CORPS].m_InfoCel[CelY][CelX].m_pFont=pFont;
		m_itTableau[CORPS].m_InfoCel[CelY][CelX].m_DataType=DataType;
		if(Orientation)	m_itTableau[CORPS].m_InfoCel[CelY][CelX].m_Orientation=Orientation;
	}
	else
	if(CelX>=0 && CelY<0)	//colonne X
	{
		for(short i=0 ; i<m_NbLig ; i++)
		{
			if(ColorCel)m_itTableau[CORPS].m_InfoCel[i][CelX].m_RGBCel=ColorCel;
			if(pFont)	m_itTableau[CORPS].m_InfoCel[i][CelX].m_pFont=pFont;
			m_itTableau[CORPS].m_InfoCel[i][CelX].m_DataType=DataType;
			if(Orientation)	m_itTableau[CORPS].m_InfoCel[i][CelX].m_Orientation=Orientation;
		}
	}
	else
	if(CelX<0 && CelY>=0)	//ligne Y
	{
		for(short j=0 ; j<m_NbCol ; j++)
		{
			if(ColorCel)m_itTableau[CORPS].m_InfoCel[CelY][j].m_RGBCel=ColorCel;
			if(pFont)	m_itTableau[CORPS].m_InfoCel[CelY][j].m_pFont=pFont;
			m_itTableau[CORPS].m_InfoCel[CelY][j].m_DataType=DataType;
			if(Orientation)	m_itTableau[CORPS].m_InfoCel[CelY][j].m_Orientation=Orientation;
		}
	}
	else	//tous les éléments du tableau
	{
		for(short i=0 ; i<m_NbLig ; i++)
		{
			for(short j=0 ; j<m_NbCol ; j++)
			{
				if(ColorCel)m_itTableau[CORPS].m_InfoCel[i][j].m_RGBCel=ColorCel;
				if(pFont)	m_itTableau[CORPS].m_InfoCel[i][j].m_pFont=pFont;
				m_itTableau[CORPS].m_InfoCel[i][j].m_DataType=DataType;
				if(Orientation)	m_itTableau[CORPS].m_InfoCel[i][j].m_Orientation=Orientation;
			}
		}
	}
	if(fUpdate)	InvalidateRect(NULL,0);
}

void CObj_Tab::SetPosition(short TypeTitre, short CelX, short CelY, bool fUpdate)
{
	if(CelX>m_NbCol-m_nbColVisu)	CelX=m_NbCol-m_nbColVisu;
	if(CelX<0)	CelX=0;
	if(CelY>m_NbLig-m_nbLigVisu)	CelY=m_NbLig-m_nbLigVisu;
	if(CelY<0)	CelY=0;
	switch(TypeTitre)
	{
		case TITRE_COL:
			m_PosActiveCol=CelX;
			break;
		case TITRE_LIG:
			m_PosActiveLig=CelY;
			break;
		case CORPS:
			m_PosActiveLig=CelY;
			m_PosActiveCol=CelX;
			break;
	}
	if(m_pScrollH)	m_pScrollH->SetScrollPos(m_PosActiveCol,TRUE);
	if(m_pScrollV)	m_pScrollV->SetScrollPos(m_PosActiveLig,TRUE);
	if(fUpdate)	InvalidateRect(NULL,0);
}

void CObj_Tab::ChangeBgColor(COLORREF ColorBg)
{
	m_itTableau[CORPS].m_ColorBg=ColorBg;
	InvalidateRect(NULL,0);
}

short * CObj_Tab::GetSelection(void)
{
	short * Selection=NULL;
	if((m_Style & 0x0080)==OPT_TAB_SELECT_LIG)
	{
		Selection=new short[m_sSelection.GetSize()];
		for(short s=0;s<m_sSelection.GetSize();s++)
			Selection[s]=m_sSelection[s].m_Y;
	}
	return Selection;
}

short CObj_Tab::GetNbSelection(void)
{
	short nbSel=0;
	if((m_Style & 0x0080)==OPT_TAB_SELECT_LIG)
		nbSel=m_sSelection.GetSize();
	return nbSel;
}

void CObj_Tab::OnLeftClick(long Lig, long Col)
{
}

void CObj_Tab::OnLeftDblClick(long Lig, long Col)
{
}
