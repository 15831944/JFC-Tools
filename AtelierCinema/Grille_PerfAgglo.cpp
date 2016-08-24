// Grille_PerfAgglo.cpp : implementation file
//

#include "stdafx.h"
#include "cinema.h"
extern CCinemaApp theApp;
#include "Grille_PerfAgglo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGrille_PerfAgglo

CGrille_PerfAgglo::CGrille_PerfAgglo()
{
}

CGrille_PerfAgglo::~CGrille_PerfAgglo()
{
}


BEGIN_MESSAGE_MAP(CGrille_PerfAgglo, CStatic)
	//{{AFX_MSG_MAP(CGrille_PerfAgglo)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGrille_PerfAgglo message handlers

void CGrille_PerfAgglo::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	int NbCol=3,NbLig=15;
	int x[]={0+8,1+8+98,208};
	int iCib=0;

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
			Hyp=m_pCampagne->m_HypotheseActive;
			CResultatPerformance * R;

			if(col==0)
			{
				R = &m_pCampagne->m_Resultat;
			}

			if(col==1)
			{
				R = &m_pCampagne->m_ResultatAgglo;
			}
			if(col==2)
			{
				R = &m_pCampagne->m_ResultatMigre;
			}
			tamponalloc[0]=0;
			switch(lig){
				case 0: // Budget brut
					if(m_pCampagne->EtatCalcul[Hyp]<1)break;
					if(col==2)break;
					sprintf(tamponalloc,"%8.2f %s",R->BudgetTot[Hyp],Unite);
				break;

				case 1: // budget net
					if(col==2)break;
					if(m_pCampagne->EtatCalcul[Hyp]<1)break;
					sprintf(tamponalloc,"%5ld.00 %s",R->BudgetNet[Hyp],Unite);
				break;

				case 2: // Nombre de contacts distribués
					if(m_pCampagne->EtatCalcul[Hyp]<1)break;
					sprintf(tamponalloc,"%8.0f",1000*R->NbContactTot[iCib][Hyp]);
				break;

				case 3: // GRP
					if(m_pCampagne->EtatCalcul[Hyp]<1)break;
					if(R->BasePopulation[iCib]<=0)break;
					sprintf(tamponalloc,"%8.2f",100000*R->NbContactTot[iCib][Hyp]/R->BasePopulation[iCib]);
				break;

				case 4: // CPM brut
					if(col==2)break;
					if(m_pCampagne->EtatCalcul[Hyp]<1)break;
					if(R->NbContactTot[iCib][Hyp]<=0)break;
					sprintf(tamponalloc,"%8.2f %s",R->BudgetTot[Hyp]/R->NbContactTot[iCib][Hyp],Unite);
				break;

				case 5: // CPM net
					if(col==2)break;
					if(m_pCampagne->EtatCalcul[Hyp]<1)break;
					if(R->NbContactTot[iCib][Hyp]<=0)break;
					sprintf(tamponalloc,"%8.2f %s",R->BudgetNet[Hyp]/R->NbContactTot[iCib][Hyp],Unite);
				break;

				case 6: // Population de la cible
					// CALCUL FRED: on affiche tous les résulats de l'hypothese valide
					//if(m_pCampagne->EtatCalcul[Hyp]<2)break;
					if(m_pCampagne->EtatCalcul[Hyp]<1)break;

					sprintf(tamponalloc,"%8.0f",R->BasePopulation[iCib]);
				break;

				case 7: // NbIndiv touchés
					//if(m_pCampagne->EtatCalcul[Hyp]<2)break;
					if(m_pCampagne->EtatCalcul[Hyp]<1)break;

					sprintf(tamponalloc,"%8.0f",1000*R->NbIndivTouchesTot[iCib][Hyp]);
				break;

				case 8: // C% cumulée
					//if(m_pCampagne->EtatCalcul[Hyp]<2)break;
					if(m_pCampagne->EtatCalcul[Hyp]<1)break;
					if(R->BasePopulation[iCib]<=0)break;
					sprintf(tamponalloc,"%8.2f %%",100000*R->NbIndivTouchesTot[iCib][Hyp]/R->BasePopulation[iCib]);
				break;

				case 9: // Répétition
				//	if(m_pCampagne->EtatCalcul[Hyp]<2)break;
					if(m_pCampagne->EtatCalcul[Hyp]<1)break;
					if(R->NbIndivTouchesTot[iCib][Hyp]<=0)break;
					sprintf(tamponalloc,"%8.2f",R->NbContactTot[iCib][Hyp]/R->NbIndivTouchesTot[iCib][Hyp]);
				break;

				case 10: // NbIndiv ayant mémorisé
//					if(m_pCampagne->EtatCalcul[Hyp]<3)break;
					if(m_pCampagne->EtatCalcul[Hyp]<1)break;

					sprintf(tamponalloc,"%8.0f",1000*R->NbIndivMemoTot[iCib][Hyp]);
				break;

				case 11: // mémorisation moyenne
					//if(m_pCampagne->EtatCalcul[Hyp]<3)break;
					if(col==2)break;
					if(m_pCampagne->EtatCalcul[Hyp]<1)break;

					sprintf(tamponalloc,"%8.2f %%",R->MemoMoy[iCib][Hyp]);
				break;

				case 12: // Mémo à la semaine X
					//if(m_pCampagne->EtatCalcul[Hyp]<3)break;
					if(m_pCampagne->EtatCalcul[Hyp]<1)break;

					if(col==2)break;
					SetTextColor(hDC,theApp.rgb[CLR_TRAITNRSEMAINEX]);
					sprintf(tamponalloc,"%8.2f %%",R->MemoNrSemaineX[iCib][Hyp]);
				break;

				case 13: // points de mémo
					//if(m_pCampagne->EtatCalcul[Hyp]<3)break;
					if(col==2)break;
					if(m_pCampagne->EtatCalcul[Hyp]<1)break;
					SetTextColor(hDC,theApp.rgb[CLR_HYPOTHESE1+Hyp]);
					sprintf(tamponalloc,"%8.0f",R->PtMemoCampTot[iCib][Hyp]);
				break;

				case 14: // RM
					//if(m_pCampagne->EtatCalcul[Hyp]<3)break;
					if(col==2)break;
					if(m_pCampagne->EtatCalcul[Hyp]<1)break;
					if(R->NbIndivTouchesTot[iCib][Hyp]<=0)break;
					sprintf(tamponalloc,"%8.2f",R->NbIndivMemoTot[iCib][Hyp]/R->NbIndivTouchesTot[iCib][Hyp]);
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
	::LineTo(hDC,300,16*2-1);
	::MoveToEx(hDC,0,16*6-1,NULL);
	::LineTo(hDC,300,16*6-1);
	::MoveToEx(hDC,0,16*10-1,NULL);
	::LineTo(hDC,300,16*10-1);


//	if(FenetreActive->NrDessin)DessineBitmapBD(hDC,hBmpWait,50,100,FenetreActive->NrDessin-1,10);


	// copy de la mem sur ecran
	BitBlt(dc.m_hDC,0,0,l,h,hDC,0,0,SRCCOPY);
	SelectObject(hDC,hBmpOld);
	DeleteObject(hBmpNew);
	DeleteDC(hDC);
	// ************************	
	// Do not call CStatic::OnPaint() for painting messages
}
