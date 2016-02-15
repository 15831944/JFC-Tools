// Obj_Printer.cpp: implementation of the CObj_Printer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Obj_Printer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CObj_Printer::CObj_Printer()
{
	m_Orientation = 0;
	m_NbLignePage=0;
	m_NbLigneCorps=0;
	m_HLig=0;
	m_LCol=0;
	m_NbColPage=0;
	m_NbPage=0;
	m_Mapping.RemoveAll();
	m_Bitmaps.SetSize( MAXBITMAP );
}

CObj_Printer::~CObj_Printer()
{
}

/////////////////////////////////////////////////////////////////////////////////
// VIRTUALS FONCTION
/////////////////////////////////////////////////////////////////////////////////
void CObj_Printer::SetDocument(CDocument * Doc)
{
//	m_pDoc=Doc;	//pour l'héritier
}

bool CObj_Printer::OnPreparePrinting(CPrintInfo* pInfo)
{
	//c'est ici que l'on peux modifier les options lié à l'imprimante (ex=l'orientation)
	return true;
}

void CObj_Printer::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	int larg = pDC->GetDeviceCaps(HORZSIZE);
	int haut = pDC->GetDeviceCaps(VERTSIZE);
	m_Orientation = (haut>=larg) ? PORTRAIT : PAYSAGE;
	m_pInfo=pInfo;
	Initialise(pDC, &m_Init);
	CalculNbPage(pDC, pInfo);
}

void CObj_Printer::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{	
}

void CObj_Printer::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	// prise en compte des marges
	pInfo->m_rectDraw.top=pInfo->m_rectDraw.top+(pInfo->m_rectDraw.Height()*m_Init.m_Marge[0]/100);
	pInfo->m_rectDraw.bottom=pInfo->m_rectDraw.bottom-(pInfo->m_rectDraw.Height()*m_Init.m_Marge[1]/100);
	pInfo->m_rectDraw.left=pInfo->m_rectDraw.left+(pInfo->m_rectDraw.Width()*m_Init.m_Marge[2]/100);
	pInfo->m_rectDraw.right=pInfo->m_rectDraw.right-(pInfo->m_rectDraw.Width()*m_Init.m_Marge[3]/100);

	m_HLig=pInfo->m_rectDraw.Height()/m_NbLignePage;
	m_LCol=pInfo->m_rectDraw.Width()/m_NbColPage;
	CRect r=pInfo->m_rectDraw;
	short NrPage=pInfo->m_nCurPage-1,NrLig,i,idx,maxi;

	pDC->SetBkMode(TRANSPARENT);
	DrawBk(pDC,&r);
	
	maxi=m_Mapping.GetSize();
	
	// Lancement des dessins de l'entete
	NrLig=0;
	int FirstLine=min(NrPage*m_NbLigneCorps,maxi-1);
	int LastLine=min(((NrPage+1)*m_NbLigneCorps)-1,maxi-1);
	for(i=0;i<m_Init.m_NbLigneTete;i++,NrLig++)
	{
		r.top=pInfo->m_rectDraw.top+m_HLig*NrLig;
		r.bottom=r.top+m_HLig;
		DrawTete(pDC,&r,NrPage,i,m_Mapping[FirstLine].m_Tab,m_Mapping[LastLine].m_Tab);
	}

	// Lancement des dessins du corps
	for(i=0;i<m_NbLigneCorps;i++,NrLig++)
	{
		r.top=pInfo->m_rectDraw.top+m_HLig*NrLig;
		r.bottom=r.top+m_HLig;
		idx=NrPage*m_NbLigneCorps+i;
		if(idx>=maxi)break;
		int tab=m_Mapping[idx].m_Tab;
		switch(m_Init.m_InfoTab[tab].m_fColonne)
		{
		case 0:	//tableau ligne/ligne
			DrawTab(pDC,&r,tab,m_Mapping[idx].m_Type,m_Mapping[idx].m_Lig);
			break;
		case 1:{	//tableau colonne/ligne
			int nb,d;
			nb=m_Mapping[idx].m_NbCol;
			d=m_Mapping[idx].m_NrCol;
			DrawCol(pDC,&r,tab,m_Mapping[idx].m_Type,m_Mapping[idx].m_Lig,d,nb);
			}break;
		case 2:{	//tableau ligne/colonne
			// les cas des colones
//			tab=m_Mapping[idx].m_Tab;
			int nb,c,d;
			nb=m_Init.m_InfoTab[tab].m_NbCarGauche;
			r.left=pInfo->m_rectDraw.left;
			r.right=r.left+nb*m_LCol;
			if(nb>0){ // la colonne de gauche
				DrawCol(pDC,&r,tab,m_Mapping[idx].m_Type,m_Mapping[idx].m_Lig,-1,-1);
			}
			nb=m_Mapping[idx].m_NbCol;
			d=m_Mapping[idx].m_NrCol;
			if(nb>0){ // les colonnes du centre
				for(c=0;c<nb;c++){
					r.left=r.right;
					r.right=r.left+m_LCol*m_Init.m_InfoTab[tab].m_CarColonne[c+d];
					if(DrawCol(pDC,&r,tab,m_Mapping[idx].m_Type,m_Mapping[idx].m_Lig,c+d,nb))break;
				}
			}
			nb=m_Init.m_InfoTab[tab].m_NbCarDroit;
			r.left=pInfo->m_rectDraw.right-(nb*m_LCol);
			r.right=pInfo->m_rectDraw.right;
			if(nb>0){ // la colonne de droite
				DrawCol(pDC,&r,tab,m_Mapping[idx].m_Type,m_Mapping[idx].m_Lig,999,999);
			}
			}break;
		}

		if(m_Mapping[idx].m_Rappel){ // Cas des traits verticaux à dessiner
			CRect rh=r;
			rh.top=pInfo->m_rectDraw.top+m_HLig*(m_Mapping[idx].m_Rappel-1+m_Init.m_NbLigneTete);
			rh.bottom=rh.top+m_HLig;
			DrawVTab(pDC,&rh, &r,tab);
		}
	}

/*	for(i=0;i<m_NbLigneCorps;i++,NrLig++)
	{
		r.top=pInfo->m_rectDraw.top+m_HLig*NrLig;
		r.bottom=r.top+m_HLig;
		idx=NrPage*m_NbLigneCorps+i;
		if(idx>=maxi)break;
		int tab=m_Mapping[idx].m_Tab;
		DrawTab(pDC,&r,tab,m_Mapping[idx].m_Type,m_Mapping[idx].m_Lig);
		if(m_Init.m_InfoTab[tab].m_fColonne==1){
			int nb,d;
			nb=m_Mapping[idx].m_NbCol;
			d=m_Mapping[idx].m_NrCol;
			DrawCol(pDC,&r,tab,m_Mapping[idx].m_Type,m_Mapping[idx].m_Lig,d,nb);
		}
		if(m_Init.m_InfoTab[tab].m_fColonne==2){
			// les cas des colones
			tab=m_Mapping[idx].m_Tab;
			int nb,c,d;
			nb=m_Init.m_InfoTab[tab].m_NbCarGauche;
			if(nb>0){ // la colonne de gauche
				r.left=pInfo->m_rectDraw.left;
				r.right=r.left+nb*m_LCol;
				DrawCol(pDC,&r,tab,m_Mapping[idx].m_Type,m_Mapping[idx].m_Lig,-1,-1);
			}
			nb=m_Mapping[idx].m_NbCol;
			d=m_Mapping[idx].m_NrCol;
			if(nb>0){ // les colonnes du centre
				for(c=0;c<nb;c++){
					r.left=r.right;
					r.right=r.left+m_LCol*m_Init.m_InfoTab[tab].m_CarColonne[c+d];
					if(DrawCol(pDC,&r,tab,m_Mapping[idx].m_Type,m_Mapping[idx].m_Lig,c+d,nb))break;
				}
			}
			nb=m_Init.m_InfoTab[tab].m_NbCarDroit;
			if(nb>0){ // la colonne de droite
				r.left=r.right;
				r.right=pInfo->m_rectDraw.right;
				DrawCol(pDC,&r,tab,m_Mapping[idx].m_Type,m_Mapping[idx].m_Lig,999,999);
			}
		}

		if(m_Mapping[idx].m_Rappel){ // Cas des traits verticaux à dessiner
			CRect rh=r;
			rh.top=pInfo->m_rectDraw.top+m_HLig*(m_Mapping[idx].m_Rappel-1+m_Init.m_NbLigneTete);
			rh.bottom=rh.top+m_HLig;
			DrawVTab(pDC,&rh, &r);
		}
	}
*/
	// Lancement des dessins du pieds de page
	NrLig=m_NbLignePage-m_Init.m_NbLignePieds;
	for(i=0;i<m_Init.m_NbLignePieds;i++,NrLig++)
	{
		r.top=pInfo->m_rectDraw.top+m_HLig*NrLig;
		r.bottom=r.top+m_HLig;
		DrawPieds(pDC,&r,NrPage,i);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
void CObj_Printer::Initialise(CDC * pDC, SIniPrint * pInit)
{
	pInit->m_NbLigneTete=20;
	pInit->m_NbLignePieds=10;
	pInit->m_Marge[0]=2;	//haut
	pInit->m_Marge[1]=2;	//bas
	pInit->m_Marge[2]=5;	//gauche
	pInit->m_Marge[3]=5;	//droite
	LoadFont(pDC, FONT_TIME10);
//	pInit->m_PoliceDef=&m_Font[FONT_TIME12];
	pInit->m_fOrientation=0;		//0:libre 2:Paysage 1:Portrait

	SInfoTab InfoTab;
	InfoTab.m_NbLigneTitre=5;
	InfoTab.m_NbLigneTete=3;
	InfoTab.m_NbLigneCorps=5;
	InfoTab.m_NbLignePieds=3;
	InfoTab.m_NbLigneTotal=0;
	InfoTab.m_NbLigneInterLigne=3;
	pInit->m_InfoTab.Add(InfoTab);
}

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
//Il y a 2 fonctions DrawTete pour permettre aux versions precedentes d'être compilé

bool CObj_Printer::DrawTete(CDC * pDC, CRect * pR, short NrPage, short NrLig, short PremierNrTab, short DernierNrTab)
{
	//La nouvelle fonction DrawTete lance l'ancienne version
	DrawTete(pDC, pR, NrPage, NrLig);
	return 1;
}

bool CObj_Printer::DrawTete(CDC * pDC, CRect * pR, short NrPage, short NrLig)
{
	pDC->SelectObject(&m_Font[FONT_TIME10]);
	pDC->DrawText("Tete de PAGE", pR, DT_CENTER|DT_VCENTER);
	return 1;
}

bool CObj_Printer::DrawPieds(CDC * pDC, CRect * pR, short NrPage, short NrLig)
{
	pDC->DrawText("Pieds de PAGE", pR, DT_CENTER|DT_VCENTER);
	return 1;
}

bool CObj_Printer::DrawTab(CDC * pDC, CRect * pR, short NrTab, short NrType, short NrLig)
{
	pDC->DrawText("Corps de PAGE", pR, DT_CENTER|DT_VCENTER);
	return 1;
}


bool CObj_Printer::DrawCol(CDC * pDC, CRect * pR, short NrTab, short NrType, short NrLig,short NrCol,short NbCol)
{
	return 1;
}


bool CObj_Printer::DrawVTab(CDC * pDC, CRect * pR_haut, CRect * pR_bas,short NrTab)
{
	return 1;
}

bool CObj_Printer::DrawBk(CDC * pDC, CRect * pR)
{
	return(0);
}

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
void CObj_Printer::LoadFont(CDC * pDC, short Nr)
{
	long lfheight=0;
	switch(Nr)
	{
	case FONT_SERIF10:
		lfheight=-MulDiv(10, pDC->GetDeviceCaps(LOGPIXELSY), 72);
		m_Font[FONT_SERIF10].CreateFont(lfheight, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, OUT_DEFAULT_PRECIS, 0, DEFAULT_QUALITY, DEFAULT_PITCH|FF_SWISS, "MS Sans Serif");
		break;
	case FONT_SERIF8:
		lfheight=-MulDiv(8, pDC->GetDeviceCaps(LOGPIXELSY), 72);
		m_Font[FONT_SERIF8].CreateFont(lfheight, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, OUT_DEFAULT_PRECIS, 0, DEFAULT_QUALITY, DEFAULT_PITCH|FF_SWISS, "MS Sans Serif");
		break;
	case FONT_TIME14:
		lfheight=-MulDiv(14, pDC->GetDeviceCaps(LOGPIXELSY), 72);
		m_Font[FONT_TIME14].CreateFont(lfheight, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, OUT_DEFAULT_PRECIS, 0, DEFAULT_QUALITY, DEFAULT_PITCH|FF_ROMAN, "Times New Roman");
		break;
	case FONT_TIME14BI:
		lfheight=-MulDiv(14, pDC->GetDeviceCaps(LOGPIXELSY), 72);
		m_Font[FONT_TIME14BI].CreateFont(lfheight, 0, 0, 0, FW_BOLD, 1, 0, 0, 0, OUT_DEFAULT_PRECIS, 0, DEFAULT_QUALITY, DEFAULT_PITCH|FF_ROMAN, "Times New Roman");
		break;
	case FONT_TIME12:
		lfheight=-MulDiv(12, pDC->GetDeviceCaps(LOGPIXELSY), 72);
		m_Font[FONT_TIME12].CreateFont(lfheight, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, OUT_DEFAULT_PRECIS, 0, DEFAULT_QUALITY, DEFAULT_PITCH|FF_ROMAN, "Times New Roman");
		break;
	case FONT_TIME12BU:
		lfheight=-MulDiv(12, pDC->GetDeviceCaps(LOGPIXELSY), 72);
		m_Font[FONT_TIME12BU].CreateFont(lfheight, 0, 0, 0, FW_BOLD, 0, 1, 0, 0, OUT_DEFAULT_PRECIS, 0, DEFAULT_QUALITY, DEFAULT_PITCH|FF_ROMAN, "Times New Roman");
		break;
	case FONT_TIME12B:
		lfheight=-MulDiv(12, pDC->GetDeviceCaps(LOGPIXELSY), 72);
		m_Font[FONT_TIME12B].CreateFont(lfheight, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, OUT_DEFAULT_PRECIS, 0, DEFAULT_QUALITY, DEFAULT_PITCH|FF_ROMAN, "Times New Roman");
		break;
	case FONT_TIME12BI:
		lfheight=-MulDiv(12, pDC->GetDeviceCaps(LOGPIXELSY), 72);
		m_Font[FONT_TIME12BI].CreateFont(lfheight, 0, 0, 0, FW_BOLD, 1, 0, 0, 0, OUT_DEFAULT_PRECIS, 0, DEFAULT_QUALITY, DEFAULT_PITCH|FF_ROMAN, "Times New Roman");
		break;
	case FONT_TIME10:
		lfheight=-MulDiv(10, pDC->GetDeviceCaps(LOGPIXELSY), 72);
		m_Font[FONT_TIME10].CreateFont(lfheight, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, OUT_DEFAULT_PRECIS, 0, DEFAULT_QUALITY, DEFAULT_PITCH|FF_ROMAN, "Times New Roman");
		break;
	case FONT_TIME10B:
		lfheight=-MulDiv(10, pDC->GetDeviceCaps(LOGPIXELSY), 72);
		m_Font[FONT_TIME10B].CreateFont(lfheight, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, OUT_DEFAULT_PRECIS, 0, DEFAULT_QUALITY, DEFAULT_PITCH|FF_ROMAN, "Times New Roman");
		break;
	case FONT_TIME6:
		lfheight=-MulDiv(6, pDC->GetDeviceCaps(LOGPIXELSY), 72);
		m_Font[FONT_TIME6].CreateFont(lfheight, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, OUT_DEFAULT_PRECIS, 0, DEFAULT_QUALITY, DEFAULT_PITCH|FF_ROMAN, "Times New Roman");
		break;
	case FONT_TIME7:
		lfheight=-MulDiv(7, pDC->GetDeviceCaps(LOGPIXELSY), 72);
		m_Font[FONT_TIME7].CreateFont(lfheight, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, OUT_DEFAULT_PRECIS, 0, DEFAULT_QUALITY, DEFAULT_PITCH|FF_ROMAN, "Times New Roman");
		break;
	case FONT_TIME7I:
		lfheight=-MulDiv(7, pDC->GetDeviceCaps(LOGPIXELSY), 72);
		m_Font[FONT_TIME7I].CreateFont(lfheight, 0, 0, 0, FW_NORMAL, 1, 0, 0, 0, OUT_DEFAULT_PRECIS, 0, DEFAULT_QUALITY, DEFAULT_PITCH|FF_ROMAN, "Times New Roman");
		break;
	case FONT_TIME8:
		lfheight=-MulDiv(8, pDC->GetDeviceCaps(LOGPIXELSY), 72);
		m_Font[FONT_TIME8].CreateFont(lfheight, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, OUT_DEFAULT_PRECIS, 0, DEFAULT_QUALITY, DEFAULT_PITCH|FF_ROMAN, "Times New Roman");
		break;
	case FONT_TIME8B:
		lfheight=-MulDiv(8, pDC->GetDeviceCaps(LOGPIXELSY), 72);
		m_Font[FONT_TIME8B].CreateFont(lfheight, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, OUT_DEFAULT_PRECIS, 0, DEFAULT_QUALITY, DEFAULT_PITCH|FF_ROMAN, "Times New Roman");
		break;
	case FONT_TIME8I:
		lfheight=-MulDiv(8, pDC->GetDeviceCaps(LOGPIXELSY), 72);
		m_Font[FONT_TIME8I].CreateFont(lfheight, 0, 0, 0, FW_NORMAL, 1, 0, 0, 0, OUT_DEFAULT_PRECIS, 0, DEFAULT_QUALITY, DEFAULT_PITCH|FF_ROMAN, "Times New Roman");
		break;
	case FONT_TIME8U:
		lfheight=-MulDiv(8, pDC->GetDeviceCaps(LOGPIXELSY), 72);
		m_Font[FONT_TIME8U].CreateFont(lfheight, 0, 0, 0, FW_NORMAL, 0, 1, 0, 0, OUT_DEFAULT_PRECIS, 0, DEFAULT_QUALITY, DEFAULT_PITCH|FF_ROMAN, "Times New Roman");
		break;
	case FONT_TIME8BI:
		lfheight=-MulDiv(8, pDC->GetDeviceCaps(LOGPIXELSY), 72);
		m_Font[FONT_TIME8BI].CreateFont(lfheight, 0, 0, 0, FW_BOLD, 1, 0, 0, 0, OUT_DEFAULT_PRECIS, 0, DEFAULT_QUALITY, DEFAULT_PITCH|FF_ROMAN, "Times New Roman");
		break;
	case FONT_TIME8BU:
		lfheight=-MulDiv(8, pDC->GetDeviceCaps(LOGPIXELSY), 72);
		m_Font[FONT_TIME8BU].CreateFont(lfheight, 0, 0, 0, FW_BOLD, 0, 1, 0, 0, OUT_DEFAULT_PRECIS, 0, DEFAULT_QUALITY, DEFAULT_PITCH|FF_ROMAN, "Times New Roman");
		break;
	case FONT_TIME8IU:
		lfheight=-MulDiv(8, pDC->GetDeviceCaps(LOGPIXELSY), 72);
		m_Font[FONT_TIME8IU].CreateFont(lfheight, 0, 0, 0, FW_NORMAL, 1, 1, 0, 0, OUT_DEFAULT_PRECIS, 0, DEFAULT_QUALITY, DEFAULT_PITCH|FF_ROMAN, "Times New Roman");
		break;
	case FONT_TIME8BIU:
		lfheight=-MulDiv(8, pDC->GetDeviceCaps(LOGPIXELSY), 72);
		m_Font[FONT_TIME8BIU].CreateFont(lfheight, 0, 0, 0, FW_BOLD, 1, 1, 0, 0, OUT_DEFAULT_PRECIS, 0, DEFAULT_QUALITY, DEFAULT_PITCH|FF_ROMAN, "Times New Roman");
		break;
	default:
		AfxMessageBox("Obj_Printer (LoadFont) : Ce numéro de font n'est pas encode gérée.");
		break;
	}
}

void CObj_Printer::LoadPen(CDC * pDC, short Nr)
{
	switch(Nr)
	{
	case PEN_1NOIR:
		m_Pen[PEN_1NOIR].CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		break;
	case PEN_2NOIR:
		m_Pen[PEN_2NOIR].CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
		break;
	case PEN_4NOIR:
		m_Pen[PEN_4NOIR].CreatePen(PS_SOLID, 4, RGB(0, 0, 0));
		break;
	case PEN_8NOIR:
		m_Pen[PEN_8NOIR].CreatePen(PS_SOLID, 8, RGB(0, 0, 0));
		break;
	case PEN_1GRISC:
		m_Pen[PEN_1GRISC].CreatePen(PS_SOLID, 1, RGB(128, 128, 128));
		break;
	}
}

void CObj_Printer::LoadBitmap(CDC* pDC, CString filename, short Nr)
{
	// on teste le numero
	if ( Nr < 0 || Nr >= m_Bitmaps.GetSize() )
		return;

	// on teste si le numero est deja utilise
	if ( TRUE == m_Bitmaps[Nr].IsValid() )
		return;

	// on charge la bitmap
	m_Bitmaps[Nr].Load( filename );

	// on quitte
	return;
}

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
bool CObj_Printer::YAPasPLace(short reel,short place)
{
	short r=m_NbLigneCorps-reel;
	if(r<=place)return true;
	else return false;
}

bool CObj_Printer::NouvellePage(short &reel)
{
	if(reel>=m_NbLigneCorps){
		reel=0;
		return true;
	}
	else return false;
}

void CObj_Printer::CalculNbPage(CDC * pDC, CPrintInfo* pInfo)
{
	// en fc du type (portrait 
	switch(m_Orientation)
	{
	case PORTRAIT:
		m_NbLignePage=NBLIGNES_PORTRAIT;
		m_NbColPage=NBCOLONNES_PORTRAIT;
		break;
	case PAYSAGE:
		m_NbLignePage=NBLIGNES_PAYSAGE;
		m_NbColPage=NBCOLONNES_PAYSAGE;
		break;
	default:
		m_NbLignePage=NBLIGNES_PORTRAIT;
		m_NbColPage=NBCOLONNES_PORTRAIT;
		break;
	}
	m_NbLigneCorps=m_NbLignePage-(m_Init.m_NbLignePieds+m_Init.m_NbLigneTete);

	if(!m_Init.m_InfoTab.GetSize())
	{
		pInfo->SetMaxPage(1);
		return;
	}
	
	SMappTab M;
	short reel=0, tab=0, stab=0, lig=0;
	short i,idx=0, MemoLigCorps=0,MemoLigDebut,place,memoidx,ligdebut;
	bool fCoupureCorps=false,fCoupureColonne=false;
	bool fNouvellePage;
	m_NbPage=1;
	while(1)
	{
		M.m_Rappel=0;
		M.m_NbLig=0;
		//titre
		lig=0;
		for(i=0 ; i<m_Init.m_InfoTab[tab].m_NbLigneTitre ; i++){
			M.m_Reel=reel++;
			M.m_Tab=tab;
			M.m_Type=PRINT_TITRE;
			M.m_Lig=lig++;
			m_Mapping.Add(M);
		}
		
		// cas du multicolonne
multicolonne:
		if(fCoupureColonne==false){
			M.m_NrCol=0;
			M.m_NbCol=0;
			MemoLigCorps=0;
		}
		else {
			fCoupureColonne=false;
			M.m_NrCol+=M.m_NbCol;
		}
multicolonne2:
		if(m_Init.m_InfoTab[tab].m_CarColonne.GetSize()>=1){
			int taille=m_Init.m_InfoTab[tab].m_NbCarGauche+m_Init.m_InfoTab[tab].m_NbCarDroit;
			for(i=M.m_NrCol;i<m_Init.m_InfoTab[tab].m_CarColonne.GetSize();i++){
				if(taille+m_Init.m_InfoTab[tab].m_CarColonne[i]>m_NbColPage+1){
					fCoupureColonne=true;
					break;
				}
				taille+=m_Init.m_InfoTab[tab].m_CarColonne[i];
			}
			M.m_NbCol=i-M.m_NrCol;
		}

tete:
		if(m_NbPage>500)
		{
			AfxMessageBox("Cet état de sortie est trop volumineux.\nSeul les 500 premières pages seront imprimées.");
			m_NbPage=500;
			break;
		}

		// Tete de tableau
		lig=0;
		MemoLigDebut=reel+1;
		for(i=0 ; i<m_Init.m_InfoTab[tab].m_NbLigneTete ; i++){
			M.m_Reel=reel++;
			M.m_Tab=tab;
			M.m_Type=PRINT_TETE;
			M.m_Lig=lig++;
			idx=m_Mapping.Add(M);
		}
		
		//corps
		lig=MemoLigCorps;
		fCoupureCorps=false;
		memoidx=-1;
		M.m_NbLig=0;
		for(i=MemoLigCorps ; i<m_Init.m_InfoTab[tab].m_NbLigneCorps ; i+=m_Init.m_InfoTab[tab].m_NbLigneGarantie){
			for(int subline=0;subline<m_Init.m_InfoTab[tab].m_NbLigneGarantie;subline++){
				// on ajoute les lignes par paquets
				M.m_Reel=reel++;
				M.m_Tab=tab;
				M.m_Type=PRINT_CORPS;
				M.m_Lig=lig++;
				idx=m_Mapping.Add(M);
				if(memoidx<0){
					memoidx=idx;
					ligdebut=lig-1;
				}
			}
			if(lig<m_Init.m_InfoTab[tab].m_NbLigneCorps){
				place=m_Init.m_InfoTab[tab].m_NbLignePieds+m_Init.m_InfoTab[tab].m_NbLigneTotal;
				place+=m_Init.m_InfoTab[tab].m_NbLigneGarantie-1;
				if(YAPasPLace(reel,place)){
					place=m_Init.m_InfoTab[tab].m_NbLignePieds;
					place+=m_Init.m_InfoTab[tab].m_NbLigneGarantie-1;
					if(YAPasPLace(reel,place)){
						MemoLigCorps=lig;
						fCoupureCorps=true;
						break;
					}
				}
			}
		}

		// place pour le total ?
		if(i==m_Init.m_InfoTab[tab].m_NbLigneCorps && !fCoupureCorps){
			place=m_Init.m_InfoTab[tab].m_NbLigneTotal;
					if(place!=0 && YAPasPLace(reel,place)){
						MemoLigCorps=lig;
						fCoupureCorps=true;
					}
		}

		// pour le multicolonne (type 1)
		if(memoidx>=0){
			M=m_Mapping[memoidx];
			M.m_NbLig=lig-ligdebut-1;
			m_Mapping.SetAt(memoidx, M);
		}

		//pieds
		lig=0;
		for(i=0 ; i<m_Init.m_InfoTab[tab].m_NbLignePieds ; i++){
			M.m_Reel=reel++;
			M.m_Tab=tab;
			M.m_Type=PRINT_PIEDS;
			M.m_Lig=lig++;
			idx=m_Mapping.Add(M);
		}

		if(fCoupureCorps){
			m_Mapping[idx].m_Rappel=MemoLigDebut;
			
			//  (25/9/00) InterLigne dans les cas des tableaux avec des 
			// garanties; il est possible que le tableau, même coupé,
			// n'atteigne pas exactement le bas de la page.
			// Il faut donc dans ce cas ajouter des interlignes
			lig=0;
			while(1){
				if(NouvellePage(reel))break;
				M.m_Reel=reel++;
				M.m_Tab=tab;
				M.m_Type=PRINT_INTERLIGNE;
				M.m_Lig=lig++;
				m_Mapping.Add(M);
			}
			// fin de l'insertion des interlignes (25/9/00)

			m_NbPage++;
			if(fCoupureColonne){
//				MemoLigCorps-=M.m_NbLig;
				MemoLigCorps-=M.m_NbLig+1;
				goto multicolonne;
			}
			else{
				M.m_NrCol=0;
				M.m_NbCol=0;
				goto multicolonne2;
			}
			goto tete;
		}

		//Total
		lig=0;
		for(i=0 ; i<m_Init.m_InfoTab[tab].m_NbLigneTotal ; i++){
			M.m_Reel=reel++;
			M.m_Tab=tab;
			M.m_Type=PRINT_TOTAL;
			M.m_Lig=lig++;
			idx=m_Mapping.Add(M);
		}
		m_Mapping[idx].m_Rappel=MemoLigDebut;

		//InterLigne
		lig=0;
		fNouvellePage=0;
		for(i=0 ; i<m_Init.m_InfoTab[tab].m_NbLigneInterLigne ; i++){
			if(NouvellePage(reel)){
				m_NbPage++;
				fNouvellePage=1;
				break; // Fin des interlignes
			}
			M.m_Reel=reel++;
			M.m_Tab=tab;
			M.m_Type=PRINT_INTERLIGNE;
			M.m_Lig=lig++;
			m_Mapping.Add(M);
		}

		if(fCoupureColonne){
			if(fCoupureCorps)	MemoLigCorps=0;
		}
		else{
			tab++; // On passe au tableau suivant
			MemoLigCorps = 0;
			fCoupureColonne = false;
			if(tab>=m_Init.m_InfoTab.GetSize()){
				// Si on avais prévue une nouvelle page, on l'annule
				if(fNouvellePage)m_NbPage--;
				break; // Fin
			}
		}

		// le prochain tableau tiend-t-il sur cette page ?
		place=m_Init.m_InfoTab[tab].m_NbLigneTitre+
			  m_Init.m_InfoTab[tab].m_NbLigneTete+
              (m_Init.m_InfoTab[tab].m_NbLigneCorps-MemoLigCorps)+
			  m_Init.m_InfoTab[tab].m_NbLignePieds+
			  m_Init.m_InfoTab[tab].m_NbLigneTotal;
		
		if(YAPasPLace(reel,place) && reel != 0){
			//InterLigne
			lig=0;
			while(1){
				if(NouvellePage(reel))break;
				M.m_Reel=reel++;
				M.m_Tab=tab;
				M.m_Type=PRINT_INTERLIGNE;
				M.m_Lig=lig++;
				m_Mapping.Add(M);
			}
			m_NbPage++;
		}
	} // while(1)
	
	// Calcul du nombre de pages
	//m_NbPage=1+m_Mapping.GetSize()/m_NbLigneCorps;
	pInfo->SetMaxPage(m_NbPage);
}

CRect CObj_Printer::CalcRect(CRect * pR, short Debut, short Fin)
{
	CRect R=*pR;
	R.left=pR->left+(pR->Width()*Debut/100);
	R.right=pR->left+(pR->Width()*Fin/100);
	return R;
}

void CObj_Printer::CalcBitmapRect(CRect* pR, DWORD largeur, DWORD hauteur, DWORD resoX, DWORD resoY)
{
	int    larg;
	int    haut;
	int    width;
	int    height;
	double RapX;
	double RapY;
	double RapR;

	// on recupere les dimensions du rectangle
	width  = pR->Width();
	height = pR->Height();

	// on calcule le rapport des resolutions
	RapR = (double)resoX / (double)resoY;

	// on calcule les rapports de mise a l'echelle
	RapX = (double)width  / ( (double)largeur * RapR );
	RapY = (double)height / (double)hauteur;

	// on teste le rapport que l'on doit utiliser
	if ( RapX < RapY )
	{
		// on calcule les dimensions de l'image
		larg = (int)( RapX * (double)largeur * RapR );
		haut = (int)( RapX * (double)hauteur );
	}
	else
	{
		// on calcule les dimensions de l'image
		larg = (int)( RapY * (double)largeur * RapR );
		haut = (int)( RapY * (double)hauteur );
	}

	// on redimensionne le rectangle de la bitmap
	pR->left += ( width - larg ) / 2;
	pR->right = pR->left + larg;
	pR->top   = pR->bottom - haut;

	// on quitte
	return;
}

CPoint CObj_Printer::CalcPointMilieuG(CRect * pR)
{
	CPoint pt(pR->left, pR->top+pR->Height()/2);
	return pt;
}

CPoint CObj_Printer::CalcPointMilieuD(CRect * pR)
{
	CPoint pt(pR->right, pR->top+pR->Height()/2);
	return pt;
}

/////////////////////////////////////////////////////////////////////////////////
// FONCTIONS DE DESSINS
/////////////////////////////////////////////////////////////////////////////////
void CObj_Printer::TraitH(CDC * pDC, CRect * pR, short PDebut, short PFin, CPen * pPen)
{
	CPen * penold;
	if(pPen)penold=pDC->SelectObject(pPen);
	CPoint Pg, Pd;
	Pg=CalcPointMilieuG(&CalcRect(pR, PDebut, PFin));
	Pd=CalcPointMilieuD(&CalcRect(pR, PDebut, PFin));
	pDC->MoveTo(Pg);
	pDC->LineTo(Pd);
	if(pPen)pDC->SelectObject(penold);
}

void CObj_Printer::TraitHtop(CDC * pDC, CRect * pR, short PDebut, short PFin, CPen * pPen)
{
	CPen * penold;
	if(pPen)penold=pDC->SelectObject(pPen);
	CPoint Pg, Pd;
	Pg=CalcPointMilieuG(&CalcRect(pR, PDebut, PFin));
	Pd=CalcPointMilieuD(&CalcRect(pR, PDebut, PFin));
	Pg.y=Pd.y=pR->top;
	pDC->MoveTo(Pg);
	pDC->LineTo(Pd);
	if(pPen)pDC->SelectObject(penold);
}

void CObj_Printer::TraitHbottom(CDC * pDC, CRect * pR, short PDebut, short PFin, CPen * pPen)
{
	CPen * penold;
	if(pPen)penold=pDC->SelectObject(pPen);
	CPoint Pg, Pd;
	Pg=CalcPointMilieuG(&CalcRect(pR, PDebut, PFin));
	Pd=CalcPointMilieuD(&CalcRect(pR, PDebut, PFin));
	Pg.y=Pd.y=pR->bottom;
	pDC->MoveTo(Pg);
	pDC->LineTo(Pd);
	if(pPen)pDC->SelectObject(penold);
}

void CObj_Printer::TraitDoubleH(CDC * pDC, CRect * pR, short PDebut, short PFin, CPen * pPen)
{
	CPen * penold;
	if(pPen)penold=pDC->SelectObject(pPen);
	CPoint Pg, Pd;
	Pg=CalcPointMilieuG(&CalcRect(pR, PDebut, PFin));
	Pd=CalcPointMilieuD(&CalcRect(pR, PDebut, PFin));
	Pg.y-=5;	Pd.y-=5;
	pDC->MoveTo(Pg);
	pDC->LineTo(Pd);
	Pg.y+=10;	Pd.y+=10;
	pDC->MoveTo(Pg);
	pDC->LineTo(Pd);
	if(pPen)pDC->SelectObject(penold);
}

void CObj_Printer::TraitV(CDC * pDC, CRect * pR_haut, CRect * pR_bas, short PDebut, short PFin, CPen * pPen)
{
	TraitVG(pDC, pR_haut, pR_bas, PDebut, PFin, pPen);
	TraitVD(pDC, pR_haut, pR_bas, PDebut, PFin, pPen);
}

void CObj_Printer::TraitVfull(CDC * pDC, CRect * pR_haut, CRect * pR_bas, short PDebut, short PFin, CPen * pPen)
{
	TraitVGfull(pDC, pR_haut, pR_bas, PDebut, PFin, pPen);
	TraitVDfull(pDC, pR_haut, pR_bas, PDebut, PFin, pPen);
}

void CObj_Printer::TraitDoubleV(CDC * pDC, CRect * pR_haut, CRect * pR_bas, short Position, CPen * pPen)
{
	CPen * penold;
	if(pPen)penold=pDC->SelectObject(pPen);
	CPoint Ph, Pb;
	Ph=CalcPointMilieuG(&CalcRect(pR_haut, Position,  Position));
	Pb=CalcPointMilieuG(&CalcRect(pR_bas, Position, Position));
	Ph.x-=5;
	Pb.x-=5;
	pDC->MoveTo(Ph);
	pDC->LineTo(Pb);
	Ph.x+=10;
	Pb.x+=10;
	pDC->MoveTo(Ph);
	pDC->LineTo(Pb);
	if(pPen)pDC->SelectObject(penold);
}

void CObj_Printer::TraitDoubleVfull(CDC * pDC, CRect * pR_haut, CRect * pR_bas, short Position, CPen * pPen)
{
	CPen * penold;
	if(pPen)penold=pDC->SelectObject(pPen);
	CPoint Ph, Pb;
	Ph=CalcPointMilieuG(&CalcRect(pR_haut, Position,  Position));
	Pb=CalcPointMilieuG(&CalcRect(pR_bas, Position, Position));
	Ph.y=pR_haut->top;
	Pb.y=pR_bas->bottom;
	Ph.x-=5;
	Pb.x-=5;
	pDC->MoveTo(Ph);
	pDC->LineTo(Pb);
	Ph.x+=10;
	Pb.x+=10;
	pDC->MoveTo(Ph);
	pDC->LineTo(Pb);
	if(pPen)pDC->SelectObject(penold);
}

void CObj_Printer::TraitVG(CDC * pDC, CRect * pR_haut, CRect * pR_bas, short PDebut, short PFin, CPen * pPen)
{
	CPen * penold;
	if(pPen)penold=pDC->SelectObject(pPen);
	CPoint Ph, Pb;
	Ph=CalcPointMilieuG(&CalcRect(pR_haut, PDebut, PFin));
	Pb=CalcPointMilieuG(&CalcRect(pR_bas, PDebut, PFin));
	pDC->MoveTo(Ph);
	pDC->LineTo(Pb);
	if(pPen)pDC->SelectObject(penold);
}

void CObj_Printer::TraitVGfull(CDC * pDC, CRect * pR_haut, CRect * pR_bas, short PDebut, short PFin, CPen * pPen)
{
	CPen * penold;
	if(pPen)penold=pDC->SelectObject(pPen);
	CPoint Ph, Pb;
	Ph=CalcPointMilieuG(&CalcRect(pR_haut, PDebut, PFin));
	Pb=CalcPointMilieuG(&CalcRect(pR_bas, PDebut, PFin));
	Ph.y=pR_haut->top;
	Pb.y=pR_bas->bottom;
	pDC->MoveTo(Ph);
	pDC->LineTo(Pb);
	if(pPen)pDC->SelectObject(penold);
}

void CObj_Printer::TraitVD(CDC * pDC, CRect * pR_haut, CRect * pR_bas, short PDebut, short PFin, CPen * pPen)
{
	CPen * penold;
	if(pPen)penold=pDC->SelectObject(pPen);
	CPoint Ph, Pb;
	Ph=CalcPointMilieuD(&CalcRect(pR_haut, PDebut, PFin));
	Pb=CalcPointMilieuD(&CalcRect(pR_bas, PDebut, PFin));
	pDC->MoveTo(Ph);
	pDC->LineTo(Pb);
	if(pPen)pDC->SelectObject(penold);
}

void CObj_Printer::TraitVDfull(CDC * pDC, CRect * pR_haut, CRect * pR_bas, short PDebut, short PFin, CPen * pPen)
{
	CPen * penold;
	if(pPen)penold=pDC->SelectObject(pPen);
	CPoint Ph, Pb;
	Ph=CalcPointMilieuD(&CalcRect(pR_haut, PDebut, PFin));
	Pb=CalcPointMilieuD(&CalcRect(pR_bas, PDebut, PFin));
	Ph.y=pR_haut->top;
	Pb.y=pR_bas->bottom;
	pDC->MoveTo(Ph);
	pDC->LineTo(Pb);
	if(pPen)pDC->SelectObject(penold);
}

void CObj_Printer::EcritG(CDC * pDC, CString txt, CRect * pR, short PDebut, short PFin, short Offset, CFont * pFont, COLORREF TxtRGB)
{
	CFont * fontold;
	COLORREF oldrgb;
	if(pFont)fontold=pDC->SelectObject(pFont);
	if(TxtRGB)oldrgb=pDC->SetTextColor(TxtRGB);
	CRect R=*pR;
	R.left=pR->left+(pR->Width()*Offset/100)+(pR->Width()*PDebut/100);
	R.right=pR->left+(pR->Width()*PFin/100);
	pDC->DrawText(txt, &R, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
	if(pFont)pDC->SelectObject(fontold);
	if(TxtRGB)pDC->SetTextColor(oldrgb);
}

void CObj_Printer::EcritD(CDC * pDC, CString txt, CRect * pR, short PDebut, short PFin, short Offset, CFont * pFont, COLORREF TxtRGB)
{
	CFont * fontold;
	COLORREF oldrgb;
	if(pFont)fontold=pDC->SelectObject(pFont);
	if(TxtRGB)oldrgb=pDC->SetTextColor(TxtRGB);
	CRect R=*pR;
	R.left=pR->left+(pR->Width()*PDebut/100);
	R.right=pR->left-(pR->Width()*Offset/100)+(pR->Width()*PFin/100);
	pDC->DrawText(txt, &R, DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);	
	if(pFont)pDC->SelectObject(fontold);
	if(TxtRGB)pDC->SetTextColor(oldrgb);
}

void CObj_Printer::EcritC(CDC * pDC, CString txt, CRect * pR, short Pour100_Debut, short Pour100_Fin, CFont * pFont, COLORREF TxtRGB)
{
	CFont * fontold;
	COLORREF oldrgb;
	if(pFont)fontold=pDC->SelectObject(pFont);
	if(TxtRGB)oldrgb=pDC->SetTextColor(TxtRGB);
	CRect R=*pR;
	R.left=pR->left+(pR->Width()*Pour100_Debut/100);
	R.right=pR->left+(pR->Width()*Pour100_Fin/100);
	pDC->DrawText(txt, &R, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
	if(pFont)pDC->SelectObject(fontold);
	if(TxtRGB)pDC->SetTextColor(oldrgb);
}

void CObj_Printer::DessineBitmap(CDC * pDC,CRect * pR,CBitmap * pBmp, short PDebut, short PFin)
{
	CBitmap * hbmpOld;
	BITMAP bm;
	CDC hdcMemory;
	int x,y;

	pBmp->GetObject(sizeof(BITMAP), &bm);
	hdcMemory.CreateCompatibleDC(pDC);
	hbmpOld=hdcMemory.SelectObject(pBmp);
	CRect R=&CalcRect(pR, PDebut, PFin);
	// on centre la bitmap dans le rectangle donné
	x=pR->left+max(pR->right-pR->left-R.Width(),0)/2;
	y=pR->top+max(pR->bottom-pR->top-R.Height(),0)/2;

//	x=pR->left+max(pR->right-pR->left-(bm.bmWidth+pR->Width()*50/100),0)/2;
//	y=pR->top+max(pR->bottom-pR->top-(bm.bmHeight+pR->Height()*50/100),0)/2;

	pDC->StretchBlt(x,y, R.Width(), R.Height(), &hdcMemory, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
//	pDC->BitBlt(x,y, (bm.bmWidth+pR->Width()*20/100), (bm.bmHeight+pR->Height()*20/100), &hdcMemory, 0, 0, SRCCOPY);
	hdcMemory.SelectObject(hbmpOld);
	hdcMemory.DeleteDC();
}

void CObj_Printer::DessineBitmap(CDC* pDC, short Nr, CRect* pR, short PDebut, short PFin)
{
	long  caps;
	HDC   hdc;
	DWORD resoX;
	DWORD resoY;

	// on teste le numero
	if ( Nr < 0 || Nr >= m_Bitmaps.GetSize() )
		return;

	// on verifie que la bitmap existe
	if ( m_Bitmaps[Nr].IsValid() == FALSE )
		return;

	// on recupere la taille de la bitmap
	const BITMAPINFO* pInfos = m_Bitmaps[Nr].GetBitmapInfos();
	if ( pInfos == NULL )
		return;

	// on recupere la taille de la bitmap
	const VOID* pBits = m_Bitmaps[Nr].GetBitmapBits();
	if ( pBits == NULL )
		return;

	// on recupere le handle du dc
	hdc = pDC->GetSafeHdc();
	if ( hdc == NULL )
		return;

	// on recupere les capacites raster du dc
	caps = ::GetDeviceCaps( hdc, RASTERCAPS );

	// on teste si le peripherique supporte les operations BitBlt
	if ( ( caps & RC_BITBLT ) != 0 )
	{
		// on recupere la resolution du peripherique
		resoX = (DWORD)::GetDeviceCaps( hdc, LOGPIXELSX );
		resoY = (DWORD)::GetDeviceCaps( hdc, LOGPIXELSY );

		// on calcule le rectangle d'affichage
		CRect R = &CalcRect(pR, PDebut, PFin);

		// on calcule le rectangle de la bitmap
		CalcBitmapRect( &R, pInfos->bmiHeader.biWidth, pInfos->bmiHeader.biHeight, resoX, resoY );

		// on recopie les bits
		int Ret = ::StretchDIBits( hdc, R.left, R.top, R.Width(), R.Height(), 0, 0, pInfos->bmiHeader.biWidth, pInfos->bmiHeader.biHeight, pBits, pInfos, DIB_RGB_COLORS, SRCCOPY); 
	}

	// on quitte
	return;
}

void CObj_Printer::RemplisRect(CDC* pDC, CRect * pR, COLORREF rgb, short PDebut, short PFin)
{
	CBrush brush(rgb);
	pDC->FillRect(&CalcRect(pR, PDebut, PFin), &brush);
}

void CObj_Printer::ShadowRect(CDC* pDC, CRect * pR_haut, CRect * pR_bas, COLORREF rgb, short PDebut, short PFin)
{
	CBrush brush(rgb);
	CRect r1=CalcRect(pR_haut, PDebut, PFin);
	CRect r2=CalcRect(pR_bas, PDebut, PFin);
	CPoint Ph;
	Ph=CalcPointMilieuD(&r1);
	CRect r=r2;
	r.top=Ph.y;
	pDC->FrameRect(&r, &brush);
	r.OffsetRect(10, 10);
	r.top=r.bottom-10;
	pDC->FillRect(&r, &brush);
	r=r2;
	r.top=Ph.y;
	r.OffsetRect(10, 10);
	r.left=r.right-10;
	pDC->FillRect(&r, &brush);
}

void CObj_Printer::DessineCadre(CDC* pDC, CRect * pR_haut, CRect * pR_bas, short PDebut, short PFin, CPen * pPen)
{
	TraitH(pDC, pR_haut, PDebut, PFin, pPen);
	TraitH(pDC, pR_bas, PDebut, PFin, pPen);
	TraitV(pDC, pR_haut, pR_bas, PDebut, PFin, pPen);
}

void CObj_Printer::DessineCadre2(CDC* pDC, CRect * pR_haut, CRect * pR_bas, short PDebut, short PFin, CPen * pPen)
{
	CBrush brush(RGB(0,0,0));
	CPen * penold;
	if(pPen)penold=pDC->SelectObject(pPen);
	CRect r1=CalcRect(pR_haut, PDebut, PFin);
	CRect r2=CalcRect(pR_bas, PDebut, PFin);
	CPoint Ph;
	Ph=CalcPointMilieuD(&r1);
	CRect r=r2;
	r.top=Ph.y;
	pDC->FrameRect(&r, &brush);
	if(pPen)pDC->SelectObject(penold);
}
