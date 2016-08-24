// GrilleFilm_Graphe.cpp : implementation file
//

#include "stdafx.h"
#include "cinema.h"
#include "Grille_Graphe.h"
#include "GrilleFilm_Graphe.h"
extern CCinemaApp theApp;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGrilleFilm_Graphe

CGrilleFilm_Graphe::CGrilleFilm_Graphe()
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

CGrilleFilm_Graphe::~CGrilleFilm_Graphe()
{
}

void CGrilleFilm_Graphe::DessineBitmapACCOGRAP(HDC hDC,RECT R,HBITMAP hBmp)
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

void CGrilleFilm_Graphe::SetTextACCOGRAP(HDC hDC, char * txt , RECT  R){
	::DrawText(hDC,txt,strlen(txt),&R,DT_SINGLELINE|DT_CENTER|DT_VCENTER|DT_NOPREFIX|DT_NOCLIP);
}

void CGrilleFilm_Graphe::CalculEchelleACCOGRAP()
{

	double ech,EcartMax;
	int NbPuiss10,PPval,p;

	// les Bonnes valeurs pour l'échelle
	// MODIF FRED
	// Ajout de deux nouvelles échelles 0.5 et 1.0 pour les nouvelles valeurs de couverture mémorisée.
	int NbBonneVal=8;
	double BonneVal[]={0.5,1.0,1.5,2,3,5,8,10};
	int NbCranDeVal[]={5,5,3,4,3,5,4,5};
	int NbInterCranDeVal[]={4,4,4,4,4,4,3,3};
	double Puiss10;
	
	// on initialise les résultats en cas de rupture de cette fonction
	m_Echelle=100;
	m_NbCran=5;
	m_NbPixCran=m_HauteurGraphe/m_NbCran;
	m_NbInterCran=1;
	m_CoeffY=m_NbPixCran*m_NbCran;

	// recherche du plus gros chiffre
	EcartMax=0.0;
	
	// recherche du plus gros chiffre
	EcartMax=0.0;
	for(int s=0;s<m_pCampagneFilm->m_NbSemaineCalcul;s++)
	{
		if(m_pCampagneFilm->MemoTot[s]>EcartMax)EcartMax=m_pCampagneFilm->MemoTot[s];
	}

	if(EcartMax<=0)return;

	// MODIF FRED
	// Nouvelles mémorisations

	ech=EcartMax/*100*/;

	// calcul de l'echelle à utiliser
	for(NbPuiss10=0;ech>10;ech/=10,NbPuiss10++);
	for(PPval=0;PPval<NbBonneVal;PPval++)if(ech<=BonneVal[PPval])break;

	for(Puiss10=1.0,p=0;p<NbPuiss10;p++)Puiss10*=10.0;
	m_Echelle=BonneVal[PPval]*Puiss10;
	m_NbCran=NbCranDeVal[PPval];
	m_NbInterCran=NbInterCranDeVal[PPval];
	m_NbPixCran=m_HauteurGraphe/m_NbCran;
	m_CoeffY=m_NbPixCran*m_NbCran*100/m_Echelle;

}

BEGIN_MESSAGE_MAP(CGrilleFilm_Graphe, CStatic)
	//{{AFX_MSG_MAP(CGrilleFilm_Graphe)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGrilleFilm_Graphe message handlers

void CGrilleFilm_Graphe::OnPaint() 
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
	// Sélection du nouvel objet bitmap
	hBmpOld=(HBITMAP)SelectObject(hDC,hBmpNew);
	// Mode de coloris pour le fond
	SetBkMode(hDC,TRANSPARENT);

	char tamponalloc[1000];

	NbColone=min(m_NbColone,m_pCampagneFilm->m_NbSemaineCalcul);

	switch(m_Part){
		case 0: // le calendrier
			FillRect(hDC,&R,theApp.hBrush[CLR_GRISC]);
			SelectObject(hDC,theApp.hFont[FNT_SMALL7]);
			if(m_pCampagneFilm->m_SetEnCours)break; // on s'arrête là.

			
			GetWindowRect(&R);
			m_pCampagneFilm->m_HautLigneTitre=(R.bottom-R.top)/2;



			// la 2 ligne= jour
			R=Rbase;
			R.bottom=R.top+m_pCampagneFilm->m_HautLigneTitre;
			for(i=0;i<NbColone;i++){
				iV=i+m_PosAscH;
				::MoveToEx(hDC,i*m_pCampagneFilm->m_LargColone-1,R.top,NULL);
				::LineTo(hDC,i*m_pCampagneFilm->m_LargColone-1,R.bottom);
				if(iV>=m_pCampagneFilm->m_NbSemaineCalcul)continue;
				if(m_Style==1)continue;
				R.left=i*m_pCampagneFilm->m_LargColone;
				R.right=R.left+m_pCampagneFilm->m_LargColone-1;
				sprintf(tamponalloc,"%d",m_pCampagneFilm->m_NrSemaine[iV]);
//				sprintf(tamponalloc,"%d",Date_Decode(m_pCampagne->DateDebut+iV*7)/10000);
				SetTextACCOGRAP(hDC,tamponalloc,R);
			}

			// le trait horizontal qui sépare les 2 lignes
			::MoveToEx(hDC,Rbase.left,R.bottom,NULL);
			::LineTo(hDC,Rbase.right,R.bottom);


			// la 1 ligne (mois variables)
			R=Rbase;
			R.top=m_pCampagneFilm->m_HautLigneTitre+1;

			AnneePrecedente=m_pCampagneFilm->m_NrAnnee[m_PosAscH]%100;
			MoisPrecedent=m_pCampagneFilm->m_NrMois[m_PosAscH];
			PosMoisPrecedent=0;
			for(i=0;i<NbColone;i++){
				iV=i+m_PosAscH;
				if(iV>=m_pCampagneFilm->m_NbSemaineCalcul)break;
				if(m_pCampagneFilm->m_NrMois[iV]==MoisPrecedent)continue;
				R.left=PosMoisPrecedent*m_pCampagneFilm->m_LargColone+1;
				R.right=(i)*m_pCampagneFilm->m_LargColone;
				::MoveToEx(hDC,i*m_pCampagneFilm->m_LargColone-1,R.top,NULL);
				::LineTo(hDC,i*m_pCampagneFilm->m_LargColone-1,R.bottom);

				if(i-PosMoisPrecedent>=3){
					if(m_Style==1)sprintf(tamponalloc,"%d-%d",MoisPrecedent,AnneePrecedente);
					else sprintf(tamponalloc,"%s %d",theApp.m_LibMois[MoisPrecedent-1],AnneePrecedente);
					SetTextACCOGRAP(hDC,tamponalloc,R);
				}
				MoisPrecedent=m_pCampagneFilm->m_NrMois[iV];
				AnneePrecedente=m_pCampagneFilm->m_NrAnnee[iV]%100;
				PosMoisPrecedent=i;
			}
			// on rajoute le dernier mois
			::MoveToEx(hDC,i*m_pCampagneFilm->m_LargColone,R.top,NULL);
			::LineTo(hDC,i*m_pCampagneFilm->m_LargColone,R.bottom);
			R.left=PosMoisPrecedent*m_pCampagneFilm->m_LargColone+1;
			R.right=i*m_pCampagneFilm->m_LargColone;
			if(i-PosMoisPrecedent>=3){
				if(m_Style==1)sprintf(tamponalloc,"%d-%d",MoisPrecedent,AnneePrecedente);
				else sprintf(tamponalloc,"%s %d",theApp.m_LibMois[MoisPrecedent-1],AnneePrecedente);
				SetTextACCOGRAP(hDC,tamponalloc,R);
			}
		break;

		case 1: // un TOM
			FillRect(hDC,&R,theApp.hBrush[CLR_GRISC]);
			if(m_pCampagneFilm->m_SetEnCours)break;
			DessineBitmapACCOGRAP(hDC,Rbase,theApp.hBmpTom);
		break;

		case 2: // l'echelle ****************
			FillRect(hDC,&R,theApp.hBrush[CLR_GRISC]);
//			SelectObject(hDC,hFont[FNT_SMALL7]);
			SelectObject(hDC,theApp.hFont[FNT_HELV8]);

			if(m_pCampagneFilm->m_SetEnCours)break;

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

			if(m_pCampagneFilm->m_SetEnCours)break;

			R=Rbase;
			// la courbe active est dessinée en surface
			TabPoint=new POINT[10+m_NbColone];

			// POUR AVOIR LA ZONE COMPLETE en gris+++++++++++++++++++
			NbPoint=1;
			TabPoint[0].x=0;
			TabPoint[0].y=m_OrigineY;
			if(m_PosAscH>0){
				NbPoint=2;
				TabPoint[1].x=0;
				// MODIF FRED: Nouvelles mémorisations
				TabPoint[1].y=m_OrigineY-m_CoeffY*m_pCampagneFilm->MemoTot[m_PosAscH-1]/100;
			}
			for(i=0;i<NbColone;i++){
				iV=i+m_PosAscH;

				// MODIF FRED: Nouvelles mémorisations
				TabPoint[NbPoint].y=m_OrigineY-m_CoeffY*m_pCampagneFilm->MemoTot[iV]/100;
				TabPoint[NbPoint].x=(1+i)*m_LargColone-1;
				NbPoint++;
			}
			TabPoint[NbPoint].y=m_OrigineY;
			TabPoint[NbPoint].x=TabPoint[NbPoint-1].x;
			NbPoint++;

			SelectObject(hDC,theApp.hBrush[CLR_FONDGRILLEINACTIF]);
			Polygon(hDC,TabPoint,NbPoint);


			// POUR AVOIR LA ZONE comprise dans la base de calcul uniquement
			NbPoint=1;
			TabPoint[0].x=0;
			TabPoint[0].y=m_OrigineY;
			if(m_PosAscH>0 && m_pCampagneFilm->m_DebutCalend<=m_PosAscH){
				NbPoint=2;
				TabPoint[1].x=0;

				// MODIF FRED: Nouvelles mémorisations
				TabPoint[1].y=m_OrigineY-m_CoeffY*m_pCampagneFilm->MemoTot[m_PosAscH-1]/100;
			}

			for(i=0;i<NbColone;i++){
				iV=i+m_PosAscH;
				if(iV== m_pCampagneFilm->m_DebutCalend){
					TabPoint[NbPoint].y=m_OrigineY;
					if(i)TabPoint[NbPoint].x=(i)*m_LargColone-1;
					else TabPoint[NbPoint].x=0;
					NbPoint++;
					if(iV>0){

						// MODIF FRED: Nouvelles mémorisations
						TabPoint[NbPoint].y=m_OrigineY-m_CoeffY*m_pCampagneFilm->MemoTot[iV-1]/100;
						if(i)TabPoint[NbPoint].x=(i)*m_LargColone-1;
						else TabPoint[NbPoint].x=0;
						NbPoint++;
					}
				}
				if(iV>=m_pCampagneFilm->m_DebutCalend && iV<=m_pCampagneFilm->m_FinCalend){

					// MODIF FRED: Nouvelles mémorisations
					TabPoint[NbPoint].y=m_OrigineY-m_CoeffY*m_pCampagneFilm->MemoTot[iV]/100;
					TabPoint[NbPoint].x=(1+i)*m_LargColone-1;
					NbPoint++;
				}
				if(iV==m_pCampagneFilm->m_FinCalend){
					TabPoint[NbPoint].y=m_OrigineY;
					TabPoint[NbPoint].x=(1+i)*m_LargColone-1;
					NbPoint++;
				}
			}
			TabPoint[NbPoint].y=m_OrigineY;
			TabPoint[NbPoint].x=TabPoint[NbPoint-1].x;
			NbPoint++;

			SelectObject(hDC,theApp.hBrush[CLR_FONDHYP1]);

			if(NbPoint>=4)Polygon(hDC,TabPoint,NbPoint);

			delete [] TabPoint;

			// le cadrillage vertical
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
			i=m_pCampagneFilm->m_DebutCalend-m_PosAscH;
			if(i>=0 && i<m_NbColone){
				::MoveToEx(hDC,i*m_LargColone-2,R.top,NULL);
				::LineTo(hDC,i*m_LargColone-2,R.bottom);
			}

			i=m_pCampagneFilm->m_FinCalend-m_PosAscH;
			if(i>=0 && i<m_NbColone){
				i++;
				::MoveToEx(hDC,i*m_LargColone-2,R.top,NULL);
				::LineTo(hDC,i*m_LargColone-2,R.bottom);
			}

			i=m_pCampagneFilm->m_NrSemaineX-m_PosAscH;
			if(i>=0 && i<m_NbColone){
				i++;
				SelectObject(hDC,theApp.hPen[CLR_TRAITNRSEMAINEX]);
				::MoveToEx(hDC,i*m_LargColone,R.top,NULL);
				::LineTo(hDC,i*m_LargColone,R.bottom);
			}



			// les hypothese a visualiser
			SelectObject(hDC,theApp.hPen[CLR_HYPOTHESE1]);
			// MODIF FRED: Nouvelles mémorisations /100
			if(m_PosAscH>0)::MoveToEx(hDC,0,m_OrigineY-m_CoeffY*m_pCampagneFilm->MemoTot[m_PosAscH-1]/100,NULL);
			else ::MoveToEx(hDC,0,m_OrigineY,NULL);
			for(i=0;i<NbColone;i++){
				iV=i+m_PosAscH;
				// MODIF FRED: Nouvelles mémorisations /100
				::LineTo(hDC,m_LargColone*(1+i)-1,m_OrigineY-m_CoeffY*m_pCampagneFilm->MemoTot[iV]/100);
			}
			// toujours en dernier, l'hypothese active
			SelectObject(hDC,theApp.hPen[CLR_HYPOTHESE1]);
			// MODIF FRED: Nouvelles mémorisations /100
			if(m_PosAscH>0)::MoveToEx(hDC,0,m_OrigineY-m_CoeffY*m_pCampagneFilm->MemoTot[m_PosAscH-1]/100,NULL);
			else ::MoveToEx(hDC,0,m_OrigineY,NULL);
			for(i=0;i<NbColone;i++){
				iV=i+m_PosAscH;
				// MODIF FRED: Nouvelles mémorisations /100
				::LineTo(hDC,m_LargColone*(1+i)-1,m_OrigineY-m_CoeffY*m_pCampagneFilm->MemoTot[iV]/100);
			}

			// la mémo moyenne de l'hypothèse valide
			SelectObject(hDC,theApp.hPen[CLR_HYPOTHESE1+h]);
			R=Rbase;
			PosY=m_CoeffY*m_pCampagneFilm->m_MemoMoy/100.0;
			i=m_pCampagneFilm->m_DebutCalend-m_PosAscH;
			if(i>m_NbColone)goto PasDeMoyenne;
			if(i>0)R.left=m_LargColone*i-1;
			i=1+m_pCampagneFilm->m_FinCalend-m_PosAscH;
			if(i<=0)goto PasDeMoyenne;
			if(i<m_NbColone)R.right=m_LargColone*i-1;
			::MoveToEx(hDC,R.left,m_OrigineY-PosY,NULL);
			::LineTo(hDC,R.right,m_OrigineY-PosY);
			if(PosY>0){
				PosY--;
				::MoveToEx(hDC,R.left,m_OrigineY-PosY,NULL);
				::LineTo(hDC,R.right,m_OrigineY-PosY);
			}
			PasDeMoyenne:

			// la ligne en cours de séparation
			if(m_pCampagneFilm->fCurCalend){
				if(m_pCampagneFilm->fCurCalend==3)SelectObject(hDC,theApp.hPen[CLR_TRAITNRSEMAINEX]);
				else SelectObject(hDC,theApp.hPen[CLR_TRAITSELECTCALEND]);
				R=Rbase;
				::MoveToEx(hDC,m_pCampagneFilm->PosXCurCalend-1,R.top,NULL);
				::LineTo(hDC,m_pCampagneFilm->PosXCurCalend-1,R.bottom);
				::MoveToEx(hDC,m_pCampagneFilm->PosXCurCalend,R.top,NULL);
				::LineTo(hDC,m_pCampagneFilm->PosXCurCalend,R.bottom);
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
