// GrilleFilm_Performances.cpp : implementation file
//

#include "stdafx.h"
#include "cinema.h"
#include "GrilleFilm_Performances.h"
extern CCinemaApp theApp;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGrilleFilm_Performances

CGrilleFilm_Performances::CGrilleFilm_Performances()
{
}

CGrilleFilm_Performances::~CGrilleFilm_Performances()
{
}


BEGIN_MESSAGE_MAP(CGrilleFilm_Performances, CStatic)
	//{{AFX_MSG_MAP(CGrilleFilm_Performances)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGrilleFilm_Performances message handlers

void CGrilleFilm_Performances::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	int NbCol=2,NbLig=15;
	int x[]={0+8,1+8+98};
	int iCib=m_NrCible;

	int lig,col;
	RECT R;

	HDC hDC;
	HBITMAP hBmpNew,hBmpOld;
	int l,h;
	
	CString Unite="€";

	// Construction de la copie mémoire bitmap de la grille
	hDC=CreateCompatibleDC(dc.m_hDC);
	GetClientRect(&R);
	l=R.right-R.left;
	h=R.bottom-R.top;
	hBmpNew=CreateCompatibleBitmap(dc.m_hDC,l,h);
	hBmpOld=(HBITMAP)SelectObject(hDC,hBmpNew);
	// **********************************

	::FillRect(hDC,&R,theApp.hBrush[CLR_PAILLE]);

	SelectObject(hDC,theApp.hFont[FNT_COURIERD]);
	SetBkMode(hDC,TRANSPARENT);

	char tamponalloc[500];

	for(lig=0;lig<NbLig;lig++){
		SetTextColor(hDC,theApp.rgb[CLR_NOIR]);
		for(col=0;col<NbCol;col++){
			tamponalloc[0]=0;
			switch(lig){
				case 0: // Budget brut
					sprintf(tamponalloc,"%8.2f %s",m_pCampagneFilm->m_BudgetBrut,Unite);
				break;

				case 1: // budget net
					sprintf(tamponalloc,"%5ld.00 %s",m_pCampagneFilm->m_BudgetNet,Unite);
				break;

				case 2: // Nombre de contacts distribués
					sprintf(tamponalloc,"%8.0f",m_pCampagneFilm->m_NbContacts);
				break;

				case 3: // GRP
					if(m_pCampagneFilm->m_Population<=0)break;
					sprintf(tamponalloc,"%8.2f",m_pCampagneFilm->m_GRP);
				break;

				case 4: // CPM brut
					if(m_pCampagneFilm->m_NbContacts<=0)break;
					sprintf(tamponalloc,"%8.2f %s",m_pCampagneFilm->m_CPMbrut,Unite);
				break;

				case 5: // CPM net
					if(m_pCampagneFilm->m_NbContacts<=0)break;
					sprintf(tamponalloc,"%8.2f %s",m_pCampagneFilm->m_CPMnet,Unite);
				break;

				case 6: // Population de la cible
					sprintf(tamponalloc,"%8.0f",m_pCampagneFilm->m_Population);
				break;

				case 7: // NbIndiv touchés
					sprintf(tamponalloc,"%8.0f",m_pCampagneFilm->m_NbIndiv);
				break;

				case 8: // C% cumulée
					if(m_pCampagneFilm->m_Population<=0)break;
					sprintf(tamponalloc,"%8.2f %%",m_pCampagneFilm->m_Couverture);
				break;

				case 9: // Répétition
					if(m_pCampagneFilm->m_NbIndiv<=0)break;
					sprintf(tamponalloc,"%8.2f",m_pCampagneFilm->m_Repetition);
				break;

				case 10: // NbIndiv ayant mémorisé
					sprintf(tamponalloc,"%8.0f",m_pCampagneFilm->NbIndivMemoTot);
				break;

				case 11: // mémorisation moyenne
					sprintf(tamponalloc,"%8.2f %%",m_pCampagneFilm->m_MemoMoy);
				break;

				case 12: // Mémo à la semaine X
					SetTextColor(hDC,theApp.rgb[CLR_TRAITNRSEMAINEX]);
					// attention dernière semaine de calcul
					sprintf(tamponalloc,"%8.2f %%",m_pCampagneFilm->MemoTot[m_pCampagneFilm->m_NrSemaineX]);
				break;

				case 13: // points de mémo
					SetTextColor(hDC,theApp.rgb[CLR_HYPOTHESE1]);
					sprintf(tamponalloc,"%8.0f",m_pCampagneFilm->PtMemoCampTot);
				break;

				case 14: // RM
					if(m_pCampagneFilm->m_NbIndiv<=0)break;
					sprintf(tamponalloc,"%8.2f",m_pCampagneFilm->m_RatioMemo);
				break;
			}
			// Ecriture du chiffre dans la grille
			TextOut(hDC,x[col],1+lig*16,tamponalloc,strlen(tamponalloc));
		}
	}

	// on dessine les traits à la main, c'est plus joli
	for(col=1;col<NbCol;col++){
		::MoveToEx(hDC,x[col]-8-col,0,NULL);
		::LineTo(hDC,x[col]-8-col,NbLig*16);
	}

	// les traits horizontaux
	::MoveToEx(hDC,0,16*2-1,NULL);
	::LineTo(hDC,200,16*2-1);
	::MoveToEx(hDC,0,16*6-1,NULL);
	::LineTo(hDC,200,16*6-1);
	::MoveToEx(hDC,0,16*10-1,NULL);
	::LineTo(hDC,200,16*10-1);


//	if(FenetreActive->NrDessin)DessineBitmapBD(hDC,hBmpWait,50,100,FenetreActive->NrDessin-1,10);


	// copy de la mem sur ecran
	BitBlt(dc.m_hDC,0,0,l,h,hDC,0,0,SRCCOPY);
	SelectObject(hDC,hBmpOld);
	DeleteObject(hBmpNew);
	DeleteDC(hDC);
}
