// Grille_Performances.cpp : implementation file
//

#include "stdafx.h"
#include "cinema.h"
extern CCinemaApp theApp;
#include "Grille_Performances.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGrille_Performances

CGrille_Performances::CGrille_Performances()
{
	m_NrCible=0;
}

CGrille_Performances::~CGrille_Performances()
{
}


BEGIN_MESSAGE_MAP(CGrille_Performances, CStatic)
	//{{AFX_MSG_MAP(CGrille_Performances)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGrille_Performances message handlers

void CGrille_Performances::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	int NbCol=2,NbLig=15;
	int x[]={0+8,1+8+98};
	int iCib=m_NrCible;

	int lig,col;
	RECT R;
	int Hyp;

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
	SetTextColor(hDC,theApp.rgb[CLR_NOIR]);
	SetBkMode(hDC,TRANSPARENT);

	char tamponalloc[500];

	for(lig=0;lig<NbLig;lig++){
		SetTextColor(hDC,theApp.rgb[CLR_NOIR]);
		for(col=0;col<NbCol;col++){
			if(col){
				if(m_pCampagne->m_HypotheseActive==m_pCampagne->m_HypotheseValide)break;
				Hyp=m_pCampagne->m_HypotheseActive;
			}
			else Hyp=m_pCampagne->m_HypotheseValide;
			tamponalloc[0]=0;
			switch(lig){
				case 0: // Budget brut
					if(m_pCampagne->EtatCalcul[Hyp]<1)break;
					sprintf(tamponalloc,"%8.2f %s",m_pCampagne->m_Resultat.BudgetTot[Hyp],Unite);
				break;

				case 1: // budget net
					if(m_pCampagne->EtatCalcul[Hyp]<1)break;
					sprintf(tamponalloc,"%5ld.00 %s",m_pCampagne->m_Resultat.BudgetNet[Hyp],Unite);
				break;

				case 2: // Nombre de contacts distribués
					if(m_pCampagne->EtatCalcul[Hyp]<1)break;
					sprintf(tamponalloc,"%8.0f",1000*m_pCampagne->m_Resultat.NbContactTot[iCib][Hyp]);
				break;

				case 3: // GRP
					if(m_pCampagne->EtatCalcul[Hyp]<1)break;
					if(m_pCampagne->m_Resultat.BasePopulation[iCib]<=0)break;
					sprintf(tamponalloc,"%8.2f",100000*m_pCampagne->m_Resultat.NbContactTot[iCib][Hyp]/m_pCampagne->m_Resultat.BasePopulation[iCib]);
				break;

				case 4: // CPM brut
					if(m_pCampagne->EtatCalcul[Hyp]<1)break;
					if(m_pCampagne->m_Resultat.NbContactTot[iCib][Hyp]<=0)break;
					sprintf(tamponalloc,"%8.2f %s",m_pCampagne->m_Resultat.BudgetTot[Hyp]/m_pCampagne->m_Resultat.NbContactTot[iCib][Hyp],Unite);
				break;

				case 5: // CPM net
					if(m_pCampagne->EtatCalcul[Hyp]<1)break;
					if(m_pCampagne->m_Resultat.NbContactTot[iCib][Hyp]<=0)break;
					sprintf(tamponalloc,"%8.2f %s",m_pCampagne->m_Resultat.BudgetNet[Hyp]/m_pCampagne->m_Resultat.NbContactTot[iCib][Hyp],Unite);
				break;

				case 6: // Population de la cible
					// CALCUL FRED: on affiche tous les résulats de l'hypothese valide
					//if(m_pCampagne->EtatCalcul[Hyp]<2)break;
					if(m_pCampagne->EtatCalcul[Hyp]<1)break;

					sprintf(tamponalloc,"%8.0f",m_pCampagne->m_Resultat.BasePopulation[iCib]);
				break;

				case 7: // NbIndiv touchés
					//if(m_pCampagne->EtatCalcul[Hyp]<2)break;
					if(m_pCampagne->EtatCalcul[Hyp]<1)break;

					sprintf(tamponalloc,"%8.0f",1000*m_pCampagne->m_Resultat.NbIndivTouchesTot[iCib][Hyp]);
				break;

				case 8: // C% cumulée
					//if(m_pCampagne->EtatCalcul[Hyp]<2)break;
					if(m_pCampagne->EtatCalcul[Hyp]<1)break;
					if(m_pCampagne->m_Resultat.BasePopulation[iCib]<=0)break;
					sprintf(tamponalloc,"%8.2f %%",100000*m_pCampagne->m_Resultat.NbIndivTouchesTot[iCib][Hyp]/m_pCampagne->m_Resultat.BasePopulation[iCib]);
				break;

				case 9: // Répétition
				//	if(m_pCampagne->EtatCalcul[Hyp]<2)break;
					if(m_pCampagne->EtatCalcul[Hyp]<1)break;
					if(m_pCampagne->m_Resultat.NbIndivTouchesTot[iCib][Hyp]<=0)break;
					sprintf(tamponalloc,"%8.2f",m_pCampagne->m_Resultat.NbContactTot[iCib][Hyp]/m_pCampagne->m_Resultat.NbIndivTouchesTot[iCib][Hyp]);
				break;

				case 10: // NbIndiv ayant mémorisé
//					if(m_pCampagne->EtatCalcul[Hyp]<3)break;
					if(m_pCampagne->EtatCalcul[Hyp]<1)break;

					sprintf(tamponalloc,"%8.0f",1000*m_pCampagne->m_Resultat.NbIndivMemoTot[iCib][Hyp]);
				break;

				case 11: // mémorisation moyenne
					//if(m_pCampagne->EtatCalcul[Hyp]<3)break;
					if(m_pCampagne->EtatCalcul[Hyp]<1)break;

					sprintf(tamponalloc,"%8.2f %%",m_pCampagne->m_Resultat.MemoMoy[iCib][Hyp]);
				break;

				case 12: // Mémo à la semaine X
					//if(m_pCampagne->EtatCalcul[Hyp]<3)break;
					if(m_pCampagne->EtatCalcul[Hyp]<1)break;

					SetTextColor(hDC,theApp.rgb[CLR_TRAITNRSEMAINEX]);
					sprintf(tamponalloc,"%8.2f %%",m_pCampagne->m_Resultat.MemoNrSemaineX[iCib][Hyp]);
				break;

				case 13: // points de mémo
					//if(m_pCampagne->EtatCalcul[Hyp]<3)break;
					if(m_pCampagne->EtatCalcul[Hyp]<1)break;
					SetTextColor(hDC,theApp.rgb[CLR_HYPOTHESE1+Hyp]);
					sprintf(tamponalloc,"%8.0f",m_pCampagne->m_Resultat.PtMemoCampTot[iCib][Hyp]);
				break;

				case 14: // RM
					//if(m_pCampagne->EtatCalcul[Hyp]<3)break;
					if(m_pCampagne->EtatCalcul[Hyp]<1)break;
					if(m_pCampagne->m_Resultat.NbIndivTouchesTot[iCib][Hyp]<=0)break;
					sprintf(tamponalloc,"%8.2f",m_pCampagne->m_Resultat.NbIndivMemoTot[iCib][Hyp]/m_pCampagne->m_Resultat.NbIndivTouchesTot[iCib][Hyp]);
				break;
			}
			
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
	// ************************

	// Do not call CStatic::OnPaint() for painting messages
}
