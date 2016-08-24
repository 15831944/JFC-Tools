// Grille_Graphe.cpp : implementation file
//

#include "stdafx.h"
#include "cinema.h"
extern CCinemaApp theApp;
#include "Grille_Graphe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGrille_Graphe

CGrille_Graphe::CGrille_Graphe()
{
	m_Style=-1;
	m_Part=-1;

	m_LargColone=-1;
	m_NbColone=-1;
	m_HauteurGraphe=-1;
	m_OrigineY=-1;
	m_HautLigneTitre=-1;
	m_PosAscH=-1;
}

CGrille_Graphe::~CGrille_Graphe()
{
}


BEGIN_MESSAGE_MAP(CGrille_Graphe, CStatic)
	//{{AFX_MSG_MAP(CGrille_Graphe)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGrille_Graphe message handlers


void CGrille_Graphe::DessineBitmapACCOGRAP(HDC hDC,RECT R,HBITMAP hBmp)
{
	HBITMAP hbmpOld;
	BITMAP bm;
	HDC hdcMemory;
	int x,y;

	GetObject(hBmp, sizeof(BITMAP), &bm);

	hdcMemory = CreateCompatibleDC(hDC);
	hbmpOld = (HBITMAP)SelectObject(hdcMemory, hBmp);

	// on centre la bitmap dans le rectangle donné
	x=R.left+max(R.right-R.left-bm.bmWidth,0)/2;
	y=R.top+max(R.bottom-R.top-bm.bmHeight,0)/2;

	BitBlt(hDC,x,y, bm.bmWidth, bm.bmHeight, hdcMemory, 0, 0, SRCCOPY);
	SelectObject(hdcMemory, hbmpOld);

	DeleteDC(hdcMemory);
}

void CGrille_Graphe::SetTextACCOGRAP(HDC hDC, char * txt , RECT  R){
	::DrawText(hDC,txt,strlen(txt),&R,DT_SINGLELINE|DT_CENTER|DT_VCENTER|DT_NOPREFIX|DT_NOCLIP);
}


void CGrille_Graphe::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	RECT R,Rbase,R2;
	RECT Rdest=dc.m_ps.rcPaint;

	HDC hDC;
	HBITMAP hBmpNew,hBmpOld;
	int l,h;
	POINT *TabPoint;
	int NbPoint;
	int NbColone;
	int PosY;

	// pour le calendier
	int AnneePrecedente,MoisPrecedent,PosMoisPrecedent;

	int i,iV,j;


	// Construction de la copie mémoire bitmap de la grille
	hDC=CreateCompatibleDC(dc.m_hDC);
	GetClientRect(&R);
	OffsetRect(&R,-R.left,-R.top);
	Rbase=R;
	l=R.right-R.left;
	h=R.bottom-R.top;
	hBmpNew=CreateCompatibleBitmap(dc.m_hDC,l,h);
	hBmpOld=(HBITMAP)SelectObject(hDC,hBmpNew);

	SetBkMode(hDC,TRANSPARENT);

	char tamponalloc[1000];

	switch(m_Part){
		case 0: // le calendrier
			FillRect(hDC,&R,theApp.hBrush[CLR_GRISC]);
//			FrameRect(hDC,&R,hBrush[CLR_NOIR]);
//			R.top+=2;
//			FrameRect(hDC,&R,hBrush[CLR_NOIR]);
			SelectObject(hDC,theApp.hFont[FNT_SMALL7]);
			if(m_pCampagne->m_SetEnCours)break; // on s'arrête là.

			// la 2 ligne= jour
			R=Rbase;
			R.bottom=R.top+m_pCampagne->m_HautLigneTitre;
			for(i=0;i<m_pCampagne->m_NbColone;i++){
				iV=i+m_PosAscH;
				::MoveToEx(hDC,i*m_pCampagne->m_LargColone-1,R.top,NULL);
				::LineTo(hDC,i*m_pCampagne->m_LargColone-1,R.bottom);
				if(iV>=m_pCampagne->m_NbSemaineCalcul)continue;
				if(m_Style==1)continue;
				R.left=i*m_pCampagne->m_LargColone;
				R.right=R.left+m_pCampagne->m_LargColone-1;
				sprintf(tamponalloc,"%d",m_pCampagne->m_NrSemaine[iV]);
//				sprintf(tamponalloc,"%d",Date_Decode(m_pCampagne->DateDebut+iV*7)/10000);
				SetTextACCOGRAP(hDC,tamponalloc,R);
			}

			// le trait horizontal qui sépare les 2 lignes
			::MoveToEx(hDC,Rbase.left,R.bottom,NULL);
			::LineTo(hDC,Rbase.right,R.bottom);


			// la 1 ligne (mois variables)
			R=Rbase;
			R.top=m_pCampagne->m_HautLigneTitre+1;

			AnneePrecedente=m_pCampagne->m_NrAnnee[m_PosAscH]%100;
			MoisPrecedent=m_pCampagne->m_NrMois[m_PosAscH];
			PosMoisPrecedent=0;
			for(i=0;i<m_pCampagne->m_NbColone;i++){
				iV=i+m_PosAscH;
				if(iV>=m_pCampagne->m_NbSemaineCalcul)break;
				if(m_pCampagne->m_NrMois[iV]==MoisPrecedent)continue;
				R.left=PosMoisPrecedent*m_pCampagne->m_LargColone+1;
				R.right=(i)*m_pCampagne->m_LargColone;
				::MoveToEx(hDC,i*m_pCampagne->m_LargColone-1,R.top,NULL);
				::LineTo(hDC,i*m_pCampagne->m_LargColone-1,R.bottom);

				if(i-PosMoisPrecedent>=3){
					if(m_Style==1)sprintf(tamponalloc,"%d-%d",MoisPrecedent,AnneePrecedente);
					else sprintf(tamponalloc,"%s %d",theApp.m_LibMois[MoisPrecedent-1],AnneePrecedente);
					SetTextACCOGRAP(hDC,tamponalloc,R);
				}
				MoisPrecedent=m_pCampagne->m_NrMois[iV];
				AnneePrecedente=m_pCampagne->m_NrAnnee[iV]%100;
				PosMoisPrecedent=i;
			}
			// on rajoute le dernier mois
			::MoveToEx(hDC,i*m_pCampagne->m_LargColone,R.top,NULL);
			::LineTo(hDC,i*m_pCampagne->m_LargColone,R.bottom);
			R.left=PosMoisPrecedent*m_pCampagne->m_LargColone+1;
			R.right=i*m_pCampagne->m_LargColone;
			if(i-PosMoisPrecedent>=3){
				if(m_Style==1)sprintf(tamponalloc,"%d-%d",MoisPrecedent,AnneePrecedente);
				else sprintf(tamponalloc,"%s %d",theApp.m_LibMois[MoisPrecedent-1],AnneePrecedente);
				SetTextACCOGRAP(hDC,tamponalloc,R);
			}
		break;

		case 1: // un TOM
			FillRect(hDC,&R,theApp.hBrush[CLR_GRISC]);
			if(m_pCampagne->m_SetEnCours)break;
			DessineBitmapACCOGRAP(hDC,Rbase,theApp.hBmpTom);
		break;

		case 2: // l'echelle ****************
			FillRect(hDC,&R,theApp.hBrush[CLR_GRISC]);
//			SelectObject(hDC,hFont[FNT_SMALL7]);
			SelectObject(hDC,theApp.hFont[FNT_HELV8]);

			if(m_pCampagne->m_SetEnCours)break;

			R=Rbase;
			R.top=m_OrigineY-m_NbCran*m_NbPixCran;
			R.bottom=m_OrigineY;

			::MoveToEx(hDC,POSXECHELLE_ACCOGRAP,R.top,NULL);
			::LineTo(hDC,POSXECHELLE_ACCOGRAP,R.bottom);
			R2.left=0;
			R2.right=POSXECHELLE_ACCOGRAP-NBPIXLEFTCRAN_ACCOGRAP;
			for(i=0;i<=m_NbCran;i++){
				::MoveToEx(hDC,POSXECHELLE_ACCOGRAP-NBPIXLEFTCRAN_ACCOGRAP,R.bottom-i*m_NbPixCran,NULL);
				::LineTo(hDC,POSXECHELLE_ACCOGRAP+NBPIXRIGHTCRAN_ACCOGRAP,R.bottom-i*m_NbPixCran);
				R2.top=R.bottom-i*m_NbPixCran-10;
				R2.bottom=R.bottom-i*m_NbPixCran+10;
				sprintf(tamponalloc,"%.1f%%",i*m_Echelle/m_NbCran);
				if(i==0)OffsetRect(&R2,0,-5);
				if(i==m_NbCran)OffsetRect(&R2,0,+3);
				SetTextACCOGRAP(hDC,tamponalloc,R2);
				for(j=1;j<=m_NbInterCran;j++){
					::MoveToEx(hDC,POSXECHELLE_ACCOGRAP-NBPIXLEFTCRANI_ACCOGRAP,R.bottom-i*m_NbPixCran-j*m_NbPixCran/(m_NbInterCran+1),NULL);
					::LineTo(hDC,POSXECHELLE_ACCOGRAP+NBPIXRIGHTCRANI_ACCOGRAP,R.bottom-i*m_NbPixCran-j*m_NbPixCran/(m_NbInterCran+1));
				}
			}

		break;

		case 3: // le coeur de la grille
			BitBlt(hDC,0,0,l,h,0,0,0,WHITENESS);
//			FrameRect(hDC,&R,hBrush[CLR_NOIR]);

			if(m_pCampagne->m_SetEnCours)break;
			NbColone=min(m_NbColone,m_pCampagne->m_NbSemaineCalcul);

			if(m_pCampagne->fZeroBinomeSelected){
				R=Rbase;
				R.top=70;
				R.left+=35;
				R.right-=35;
				::SetTextColor(hDC,theApp.rgb[CLR_ROUGE]);
				strcpy(tamponalloc,"Pour visualiser les courbes, sélectionnez au moins 1 binôme !");
				DrawText(hDC,tamponalloc,strlen(tamponalloc),&R,DT_NOPREFIX|DT_WORDBREAK|DT_CENTER);
				break;
			}
			R=Rbase;
			// la courbe active est dessinée en surface
			TabPoint=new POINT[10+NbColone];

			// POUR AVOIR LA ZONE COMPLETE en gris+++++++++++++++++++
			NbPoint=1;
			TabPoint[0].x=0;
			TabPoint[0].y=m_OrigineY;
			if(m_PosAscH>0){
				NbPoint=2;
				TabPoint[1].x=0;
				TabPoint[1].y=m_OrigineY-m_CoeffY*m_pCampagne->MemoTot[0][m_pCampagne->m_HypotheseActive][m_PosAscH-1];
			}
			for(i=0;i<NbColone;i++){
				iV=i+m_PosAscH;
				TabPoint[NbPoint].y=m_OrigineY-m_CoeffY*m_pCampagne->MemoTot[0][m_pCampagne->m_HypotheseActive][iV];
				TabPoint[NbPoint].x=(1+i)*m_LargColone-1;
				NbPoint++;
			}
			TabPoint[NbPoint].y=m_OrigineY;
			TabPoint[NbPoint].x=TabPoint[NbPoint-1].x;
			NbPoint++;

			SelectObject(hDC,theApp.hBrush[CLR_FONDGRILLEINACTIF]);
			Polygon(hDC,TabPoint,NbPoint);


			// POUR AVOIR LA ZONE comprise dans la base de calcul uniquement
			if(!(m_pCampagne->fCurCalend==1 || m_pCampagne->fCurCalend==2)){
				NbPoint=1;
				TabPoint[0].x=0;
				TabPoint[0].y=m_OrigineY;
				if(m_PosAscH>0 && m_pCampagne->m_DebutCalend<=m_PosAscH){
					NbPoint=2;
					TabPoint[1].x=0;
					TabPoint[1].y=m_OrigineY-m_CoeffY*m_pCampagne->MemoTot[0][m_pCampagne->m_HypotheseActive][m_PosAscH-1];
				}

				for(i=0;i<NbColone;i++){
					iV=i+m_PosAscH;
					if(iV== m_pCampagne->m_DebutCalend){
						TabPoint[NbPoint].y=m_OrigineY;
						if(i)TabPoint[NbPoint].x=(i)*m_LargColone-1;
						else TabPoint[NbPoint].x=0;
						NbPoint++;
						if(iV>0){
							TabPoint[NbPoint].y=m_OrigineY-m_CoeffY*m_pCampagne->MemoTot[0][m_pCampagne->m_HypotheseActive][iV-1];
							if(i)TabPoint[NbPoint].x=(i)*m_LargColone-1;
							else TabPoint[NbPoint].x=0;
							NbPoint++;
						}
					}
					if(iV>=m_pCampagne->m_DebutCalend && iV<=m_pCampagne->m_FinCalend){
						TabPoint[NbPoint].y=m_OrigineY-m_CoeffY*m_pCampagne->MemoTot[0][m_pCampagne->m_HypotheseActive][iV];
						TabPoint[NbPoint].x=(1+i)*m_LargColone-1;
						NbPoint++;
					}
					if(iV==m_pCampagne->m_FinCalend){
						TabPoint[NbPoint].y=m_OrigineY;
						TabPoint[NbPoint].x=(1+i)*m_LargColone-1;
						NbPoint++;
					}
				}
				TabPoint[NbPoint].y=m_OrigineY;
				TabPoint[NbPoint].x=TabPoint[NbPoint-1].x;
				NbPoint++;

				SelectObject(hDC,theApp.hBrush[CLR_FONDHYP1+m_pCampagne->m_HypotheseActive]);

				if(NbPoint>=4)Polygon(hDC,TabPoint,NbPoint);
			}
			delete [] TabPoint;

			// le cadrillage verticale
			R=Rbase;
			SelectObject(hDC,theApp.hPen[CLR_GRISC]);
			for(i=1;i<m_NbColone;i++){
				::MoveToEx(hDC,i*m_LargColone-1,R.top,NULL);
				::LineTo(hDC,i*m_LargColone-1,R.bottom);
			}
			// le cadrillage horizontal
			for(i=1;i<m_NbCran;i++){
				::MoveToEx(hDC,R.left,m_OrigineY-i*m_NbPixCran,NULL);
				::LineTo(hDC,R.right,m_OrigineY-i*m_NbPixCran);
			}


			// les traits de la base de mémorisation
			SelectObject(hDC,theApp.hPen[CLR_TRAITSELECTCALEND]);
			if(!(m_pCampagne->fCurCalend==1 || m_pCampagne->fCurCalend==2)){
				i=m_pCampagne->m_DebutCalend-m_PosAscH;
				if(i>=0 && i<m_NbColone){
					::MoveToEx(hDC,i*m_LargColone-2,R.top,NULL);
					::LineTo(hDC,i*m_LargColone-2,R.bottom);
				}
			}

			if(!(m_pCampagne->fCurCalend==1 || m_pCampagne->fCurCalend==2)){
				i=m_pCampagne->m_FinCalend-m_PosAscH;
				if(i>=0 && i<m_NbColone){
					i++;
					::MoveToEx(hDC,i*m_LargColone-2,R.top,NULL);
					::LineTo(hDC,i*m_LargColone-2,R.bottom);
				}
			}

			if(m_pCampagne->fCurCalend!=3){
				i=m_pCampagne->m_NrSemaineX-m_PosAscH;
				if(i>=0 && i<m_NbColone){
					i++;
					SelectObject(hDC,theApp.hPen[CLR_TRAITNRSEMAINEX]);
					::MoveToEx(hDC,i*m_LargColone,R.top,NULL);
					::LineTo(hDC,i*m_LargColone,R.bottom);
				}
			}



			// les hypothese a visualiser
			for(h=0;h<NBHYPOTHESE;h++){
				if(!m_pCampagne->m_VisuHypothese[h])continue;
				if(h==m_pCampagne->m_HypotheseActive)continue;
				SelectObject(hDC,theApp.hPen[CLR_HYPOTHESE1+h]);
				if(m_PosAscH>0)::MoveToEx(hDC,0,m_OrigineY-m_CoeffY*m_pCampagne->MemoTot[0][h][m_PosAscH-1],NULL);
				else ::MoveToEx(hDC,0,m_OrigineY,NULL);
				for(i=0;i<NbColone;i++){
					iV=i+m_PosAscH;
					::LineTo(hDC,m_LargColone*(1+i)-1,m_OrigineY-m_CoeffY*m_pCampagne->MemoTot[0][h][iV]);
				}
			}

			// toujours en dernier, l'hypothese active
			h=m_pCampagne->m_HypotheseActive;
			SelectObject(hDC,theApp.hPen[CLR_HYPOTHESE1+h]);
			if(m_PosAscH>0)::MoveToEx(hDC,0,m_OrigineY-m_CoeffY*m_pCampagne->MemoTot[0][h][m_PosAscH-1],NULL);
			else ::MoveToEx(hDC,0,m_OrigineY,NULL);
			for(i=0;i<NbColone;i++){
				iV=i+m_PosAscH;
				::LineTo(hDC,m_LargColone*(1+i)-1,m_OrigineY-m_CoeffY*m_pCampagne->MemoTot[0][h][iV]);
			}

			// la mémo moyenne de l'hypothèse valide
			if(!(m_pCampagne->fCurCalend==1 || m_pCampagne->fCurCalend==2)){
				SelectObject(hDC,theApp.hPen[CLR_HYPOTHESE1+h]);
				R=Rbase;
				PosY=m_CoeffY*m_pCampagne->m_Resultat.MemoMoy[0][h]/100.0;
				i=m_pCampagne->m_DebutCalend-m_PosAscH;
				if(i>m_NbColone)goto PasDeMoyenne;
				if(i>0)R.left=m_LargColone*i-1;
				i=1+m_pCampagne->m_FinCalend-m_PosAscH;
				if(i<=0)goto PasDeMoyenne;
				if(i<m_NbColone)R.right=m_LargColone*i-1;
				::MoveToEx(hDC,R.left,m_OrigineY-PosY,NULL);
				::LineTo(hDC,R.right,m_OrigineY-PosY);
				if(PosY>0){
					PosY--;
					::MoveToEx(hDC,R.left,m_OrigineY-PosY,NULL);
					::LineTo(hDC,R.right,m_OrigineY-PosY);
				}
			}
			PasDeMoyenne:

			// la ligne en cours de séparation
			if(m_pCampagne->fCurCalend){
				if(m_pCampagne->fCurCalend==3)SelectObject(hDC,theApp.hPen[CLR_TRAITNRSEMAINEX]);
				else SelectObject(hDC,theApp.hPen[CLR_TRAITSELECTCALEND]);
				R=Rbase;
				::MoveToEx(hDC,m_pCampagne->PosXCurCalend-1,R.top,NULL);
				::LineTo(hDC,m_pCampagne->PosXCurCalend-1,R.bottom);
				::MoveToEx(hDC,m_pCampagne->PosXCurCalend,R.top,NULL);
				::LineTo(hDC,m_pCampagne->PosXCurCalend,R.bottom);
			}


			// les traits noir de haut et bas
			R=Rbase;
			SelectObject(hDC,theApp.hPen[CLR_NOIR]);
			::MoveToEx(hDC,R.left,m_OrigineY,NULL);
			::LineTo(hDC,R.right,m_OrigineY);
			::MoveToEx(hDC,R.left,m_OrigineY-m_NbCran*m_NbPixCran,NULL);
			::LineTo(hDC,R.right,m_OrigineY-m_NbCran*m_NbPixCran);
		break;
	}


	// copy de la mem sur ecran
	BitBlt(dc.m_hDC,Rdest.left,Rdest.top,Rdest.right,Rdest.bottom,hDC,Rdest.left,Rdest.top,SRCCOPY);
	SelectObject(hDC,hBmpOld);
	DeleteObject(hBmpNew);
	DeleteDC(hDC);
	
}

void CGrille_Graphe::CalculEchelleACCOGRAP()
{

//  *** Echelle fixe à 100 % ***
//	pData->Echelle=100;
//	pData->NbCran=5;
//	pData->NbPixCran=pData->HauteurGraphe/pData->NbCran;
//	pData->NbInterCran=1;
//	pData->CoeffY=pData->NbPixCran*pData->NbCran;


//	int v,NbVagueValide;
//	double moy,mini,maxi;
	double ech,EcartMax;
	int NbPuiss10,PPval,p;
	// le Bonnes valeurs pour l'échelle
	int NbBonneVal=6;
	double BonneVal[]={1.5,2,3,5,8,10};
	int NbCranDeVal[]={3,4,3,5,4,5};
	int NbInterCranDeVal[]={4,4,4,4,3,3};
//	int NbPixelsParDemi;
	double Puiss10;
	int h,s;

	// on initialise les résultats en cas de rupture de cette fonction
	m_Echelle=100;
	m_NbCran=5;
	m_NbPixCran=m_HauteurGraphe/m_NbCran;
	m_NbInterCran=1;
	m_CoeffY=m_NbPixCran*m_NbCran;

	// recherche du plus gros chiffre
	EcartMax=0.0;
	for(h=0;h<NBHYPOTHESE;h++){
		if(!m_pCampagne->m_VisuHypothese[h])continue;
		for(s=0;s<m_pCampagne->m_NbSemaineCalcul;s++){
			if(m_pCampagne->MemoTot[0][h][s]>EcartMax)EcartMax=m_pCampagne->MemoTot[0][h][s];
		}
	}

	if(EcartMax<=0)return;
	ech=EcartMax*100;

	// calcul de l'echelle à utiliser
	for(NbPuiss10=0;ech>10;ech/=10,NbPuiss10++);
	for(PPval=0;PPval<NbBonneVal;PPval++)if(ech<=BonneVal[PPval])break;

	for(Puiss10=1.0,p=0;p<NbPuiss10;p++)Puiss10*=10.0;
	m_Echelle=BonneVal[PPval]*Puiss10;
	m_NbCran=NbCranDeVal[PPval];
	m_NbInterCran=NbInterCranDeVal[PPval];
	m_NbPixCran=m_HauteurGraphe/m_NbCran;
	m_CoeffY=m_NbPixCran*m_NbCran*100/m_Echelle;
	//NbPixelsParDemi=pData->NbPixCran*pData->NbCran;
}


	
