// Grille.cpp : implementation file
//

#include "stdafx.h"
#include "cinema.h"
extern CCinemaApp theApp;
#include "Grille.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGrille

CGrille::CGrille()
{
	m_fCapture=0;
	m_Part=-1;
	m_pCampagne=NULL;
}

CGrille::~CGrille()
{
	if(m_pCampagne && m_pCampagne->m_hBmpOldspeed)
	{
		SelectObject(m_pCampagne->m_hDCspeed,m_pCampagne->m_hBmpOldspeed);
		DeleteDC(m_pCampagne->m_hDCspeed);
		DeleteObject(m_pCampagne->m_hBmpNewspeed);
		m_pCampagne->m_hBmpOldspeed=0;
	}

}


BEGIN_MESSAGE_MAP(CGrille, CStatic)
	//{{AFX_MSG_MAP(CGrille)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGrille message handlers


void CGrille::DessineBlocDeplacementACCOGRIL(HDC hDC)
{
	int p,s;
	int posx,posy;
	RECT R;

	for(p=0;p<m_pCampagne->m_NbP;p++){
		posy=m_pCampagne->m_CoinDebutY+p-m_pCampagne->m_PosAscV;
		if(posy<0 || posy>=m_pCampagne->m_NbLigne)continue;
		for(s=0;s<m_pCampagne->m_NbS;s++){
			posx=m_pCampagne->m_CoinDebutX+s-m_pCampagne->m_PosAscH;
			if(posx<0 || posx>=m_pCampagne->m_NbColone)continue;
			if(!m_pCampagne->m_BlocPass[p][s][m_pCampagne->m_NbFormat])continue;
			R.left=posx*m_pCampagne->m_LargColone+2;
			R.right=R.left+PASSAGEPIX_ACCOGRIL-3;
			R.top=posy*m_pCampagne->m_HautLigne+2;
			R.bottom=R.top+PASSAGEPIX_ACCOGRIL-3;
			FrameRect(hDC,&R,theApp.hBrush[CLR_FONDPASSENDPL]);
		}
	}
}


void CGrille::ConstruitAccelerateurACCOGRIL()
{
	int f;
	for(f=0;f<=m_pCampagne->m_NbFormat;f++)DessineCelluleACCOGRIL(m_pCampagne->m_hDCspeed,f*m_pCampagne->m_LargColone,0,f);
	for(f=0;f<=m_pCampagne->m_NbFormat;f++)DessineCelluleACCOGRIL(m_pCampagne->m_hDCspeed,f*m_pCampagne->m_LargColone,m_pCampagne->m_HautLigne,100+f);
	for(f=0;f<=m_pCampagne->m_NbFormat;f++)DessineCelluleACCOGRIL(m_pCampagne->m_hDCspeed,f*m_pCampagne->m_LargColone,2*m_pCampagne->m_HautLigne,203+f);

	DessineCelluleACCOGRIL(m_pCampagne->m_hDCspeed,0,3*m_pCampagne->m_HautLigne,200);
	DessineCelluleACCOGRIL(m_pCampagne->m_hDCspeed,1*m_pCampagne->m_LargColone,4*m_pCampagne->m_HautLigne,201);
	DessineCelluleACCOGRIL(m_pCampagne->m_hDCspeed,1*m_pCampagne->m_LargColone,5*m_pCampagne->m_HautLigne,202);


}


void CGrille::DessineCelluleSpeedACCOGRIL(HDC hDCdest,HDC hDCspeed,int posx,int posy,int TC)
{
	int x,y;
	if(TC>=203)
	{ 
		x=LARGCOLONE_ACCOGRIL*(TC-203);
		y=2*HAUTLIGNE_ACCOGRIL;
	}
	else if(TC>=202)
	{
		x=LARGCOLONE_ACCOGRIL*1;
		y=5*HAUTLIGNE_ACCOGRIL;
	}
	else if(TC>=201)
	{
		// non-achetable
		x=LARGCOLONE_ACCOGRIL*1;
		y=4*HAUTLIGNE_ACCOGRIL;
	}
	else if(TC==200){ // le fond est présent
		x=0;
		y=3*HAUTLIGNE_ACCOGRIL;
	}
	else if(TC>=100){
		x=LARGCOLONE_ACCOGRIL*(TC-100);
		y=HAUTLIGNE_ACCOGRIL;
	}
	else {
		x=LARGCOLONE_ACCOGRIL*TC;
		y=0;
	}
	
	
	BitBlt(hDCdest,posx,posy,LARGCOLONE_ACCOGRIL-1,HAUTLIGNE_ACCOGRIL-1,hDCspeed,x,y,SRCCOPY);
}



// retourne un code pour savoir ce que l'on va dessiner dans une cellule
int CGrille::TypeDeCelluleACCOGRIL(int NrParc,int NrSemaine)
{
	int f;
	int NrFormat=-1;
	char fMS=0;
	char fFond=0;
	// gestion de l'offre figée
	int ligne;
	bool fige=0;
	CString code;
	code = m_pCampagne->m_Selection.GetCodeFromParc(NrParc);
	// MODIF FRED: 15/05/2002 bac régie
	// Le tarif d'un bac ne change pas au niveau de sa régie.
	//code = code.Left(code.GetLength()-1); 
	if((code[0]=='B')&&(code.GetLength()==6))code+="P";
	if(m_pCampagne->m_Tarif.GetTarif(code,NrSemaine,NrParc%2)<0)
	{
		return(201);
	}

	// le produit est il figé?
	ligne = m_pCampagne->m_Selection.GetNrDirectBinome(NrParc);		
	// la case sélectionnée appartient elle à une ligne de produit figé. 		
	if((m_pCampagne->m_Selection.GetType(ligne)==tagSelection::PRODUITFIGE))		
	{		
		fige=1;		
	}		

	for(f=0;f<m_pCampagne->m_NbFormat;f++){
		if(m_pCampagne->m_FlagFormat[f]==0)continue;
		if(m_pCampagne->m_FlagFormat[f]&1)
		{
			if(m_pCampagne->m_MapInterdit[NrSemaine][NrParc][f])
			{
				NrFormat=201;
			}
			if(!m_pCampagne->m_Passage[m_pCampagne->m_HypotheseActive][NrParc][NrSemaine][f])continue;
			if(NrFormat<0)	
			{ 	
				if(fige)	
				{	
					NrFormat=203+f;	
				}	
				else 	
				{	
					NrFormat=f;	
				}	
			}	
			else	
			{	
				fMS=1;	
			}	
			if((fige)&&!((m_pCampagne->m_Passage[m_pCampagne->m_HypotheseActive][NrParc][NrSemaine][f]==1)&&(NrParc%2==0)))	
			{	
				return(202);	

			}


		}
		else if(m_pCampagne->m_FlagFormat[f]&2)
		{
			if(m_pCampagne->m_Passage[m_pCampagne->m_HypotheseActive][NrParc][NrSemaine][f]>=1)
			{
				return(200);
			}

		}
		
	}

	if(fMS)NrFormat=m_pCampagne->m_NbFormat;
	if(fFond){
	   if(NrFormat<0)
	   {
		   return(200);
	   }
	   else
	   {
		 if(fige)
		 {
			 NrFormat-=203;
		 }
		 return(100+NrFormat);

	   }
	}
	return(NrFormat);
}


void CGrille::DessineCelluleACCOGRIL(HDC hDC,int posx,int posy,int TC)
{
	RECT R;

	char fMS=0;
	char fFonf=0;

	R.left=posx;R.right=posx+PASSAGEPIX_ACCOGRIL;
	R.top=posy;R.bottom=posy+PASSAGEPIX_ACCOGRIL;
	if(TC>=203)
	{
		FillRect(hDC,&R,theApp.hBrush[CLR_PRODUITFIGE]);
		FrameRect(hDC,&R,theApp.hBrush[CLR_BORDPASSAGE]);
		R.right--;
		TC-=203;
	}
	else
	{

		if(TC>=202){
			R.left=posx;R.right=posx+PASSAGEPIX_ACCOGRIL+1;
			R.top=posy;R.bottom=posy+PASSAGEPIX_ACCOGRIL+1;
			FillRect(hDC,&R,theApp.hBrush[CLR_PRODUITFIGE]);
			return;

		}
		else if(TC>=201){
			R.left=posx;R.right=posx+PASSAGEPIX_ACCOGRIL+1;
			R.top=posy;R.bottom=posy+PASSAGEPIX_ACCOGRIL+1;
			FillRect(hDC,&R,theApp.hBrush[CLR_CELLULEGRISEE]);
			return;
		}
		else if(TC>=100){
			R.left=posx;R.right=posx+PASSAGEPIX_ACCOGRIL+1;
			R.top=posy;R.bottom=posy+PASSAGEPIX_ACCOGRIL+1;
			FillRect(hDC,&R,theApp.hBrush[CLR_FONDPRESENT]);
			TC-=100;
		}
		if(TC>=100)return;
		FillRect(hDC,&R,theApp.hBrush[CLR_FONDPASSAGE]);
		FrameRect(hDC,&R,theApp.hBrush[CLR_BORDPASSAGE]);
	
		R.right--;

	}
	if(TC>=m_pCampagne->m_NbFormat)
	{
		SetTextACCOGRIL(hDC,"@",R);
	}
	else
	{		
			SetTextACCOGRIL(hDC,m_pCampagne->m_LibFormat[TC],R);
	}

}


// fonction à mettre en standard
void CGrille::DrawRectPlisse(HDC hDC,RECT R){

	FillRect(hDC,&R,theApp.hBrush[CLR_BLANC]);

	SelectObject(hDC,theApp.hPen[CLR_GRISF]);
	::MoveToEx(hDC,R.left,R.bottom,NULL);
	::LineTo(hDC,R.left,R.top);
	::LineTo(hDC,R.right,R.top);

	SelectObject(hDC,theApp.hPen[CLR_NOIR]);
	::MoveToEx(hDC,R.left+1,R.bottom-1,NULL);
	::LineTo(hDC,R.left+1,R.top+1);
	::LineTo(hDC,R.right-1,R.top+1);

	SelectObject(hDC,theApp.hPen[CLR_BLANC]);
	::MoveToEx(hDC,R.left,R.bottom,NULL);
	::LineTo(hDC,R.right,R.bottom);
	::LineTo(hDC,R.right,R.top-1);

	SelectObject(hDC,theApp.hPen[CLR_GRISC]);
	::MoveToEx(hDC,R.left+1,R.bottom-1,NULL);
	::LineTo(hDC,R.right-1,R.bottom-1);
	::LineTo(hDC,R.right-1,R.top);
}

void CGrille::SetTextACCOGRIL(HDC hDC,LPCSTR txt , RECT  R){
	DrawText(hDC,txt,strlen(txt),&R,DT_SINGLELINE|DT_CENTER|DT_VCENTER|DT_NOPREFIX|DT_NOCLIP);
}


void CGrille::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	
	RECT R,Rbase,Rgrise;
	RECT Rdest=dc.m_ps.rcPaint;
	int i,c,b,p;
	int iV,iB;
	int TC; // Type de Cellule
	int idxB; // nrindex du binome

	HDC hDC;
	HBITMAP hBmpNew,hBmpOld;
	int l,h;

	// pour le calendier
	int AnneePrecedente,MoisPrecedent,PosMoisPrecedent;
	int NbPassage;
	int posx;

	// pour les produits figé
	int ligne;
	bool fige=0;


	// Construction de la copie mémoire bitmap de la grille
	HDC hDCdest=dc.m_hDC;
	hDC=CreateCompatibleDC(hDCdest);
	GetWindowRect(&R);
	OffsetRect(&R,-R.left,-R.top);
	Rbase=R;
	l=R.right-R.left;
	h=R.bottom-R.top;
	hBmpNew=CreateCompatibleBitmap(hDCdest,l,h);
	hBmpOld=(HBITMAP)SelectObject(hDC,hBmpNew);

	SetBkMode(hDC,TRANSPARENT);
	CString txt;

	switch(m_Part){
		case 0: // le calendrier
			if(m_pCampagne->m_SetEnCours){
				FillRect(hDC,&R,theApp.hBrush[CLR_GRISC]);
				break; // on s'arrête là.
			}

//			BitBlt(hDC,0,0,l,h,0,0,0,WHITENESS);
			FillRect(hDC,&R,theApp.hBrush[CLR_GRISC]);
//			FrameRect(hDC,&R,hBrush[CLR_NOIR]);
//			R.top+=2;
//			FrameRect(hDC,&R,hBrush[CLR_NOIR]);
			SelectObject(hDC,theApp.hFont[FNT_SMALL7]);

			// la 1 ligne (mois variables)
			R=Rbase;
			R.bottom=R.top+m_pCampagne->m_HautLigneTitre;

			AnneePrecedente=m_pCampagne->m_NrAnnee[m_pCampagne->m_PosAscH]%100;
			MoisPrecedent=m_pCampagne->m_NrMois[m_pCampagne->m_PosAscH];
			PosMoisPrecedent=0;
			for(i=0;i<m_pCampagne->m_NbColone;i++){
				iV=i+m_pCampagne->m_PosAscH;
				if(iV>=m_pCampagne->m_NbSemaine)break;
				if(m_pCampagne->m_NrMois[iV]==MoisPrecedent)continue;
				R.left=PosMoisPrecedent*m_pCampagne->m_LargColone+1;
				R.right=(i)*m_pCampagne->m_LargColone;
				::MoveToEx(hDC,i*m_pCampagne->m_LargColone-1,R.top,NULL);
				::LineTo(hDC,i*m_pCampagne->m_LargColone-1,R.bottom);

				if(i-PosMoisPrecedent>=3){
					CString txt;
					txt.Format("%s %d",theApp.m_LibMois[MoisPrecedent-1],AnneePrecedente);
					SetTextACCOGRIL(hDC,txt,R);
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
				txt.Format("%s %d",theApp.m_LibMois[MoisPrecedent-1],AnneePrecedente);
				SetTextACCOGRIL(hDC,txt,R);
			}


			// le trait horizontal qui sépare les 2 lignes
//			MoveTo(hDC,Rbase.left,R.bottom);
//			LineTo(hDC,Rbase.right,R.bottom);

			// le rectangle plissé
			R=Rbase;
			R.top=m_pCampagne->m_HautLigneTitre-2;
			InflateRect(&R,-1,-1);
			if(m_pCampagne->m_NbSemaine<m_pCampagne->m_NbColone)R.right=m_pCampagne->m_NbSemaine*m_pCampagne->m_LargColone;
			DrawRectPlisse(hDC,R);

			// la 2 ligne= jour
			R=Rbase;
			R.top=m_pCampagne->m_HautLigneTitre+1;
			R.bottom-=2;
			SelectObject(hDC,theApp.hPen[CLR_NOIR]);
			for(i=0;i<m_pCampagne->m_NbColone;i++){
				iV=i+m_pCampagne->m_PosAscH;
				if(iV>=m_pCampagne->m_NbSemaine)continue;
				if(i){
					::MoveToEx(hDC,i*m_pCampagne->m_LargColone-1,R.top+10,NULL);
					::LineTo(hDC,i*m_pCampagne->m_LargColone-1,R.bottom);
				}
				if(!i){
					R.left=3;
					R.right=R.left+m_pCampagne->m_LargColone;
				}
				else if(i==m_pCampagne->m_NbColone-1){
					R.left=i*m_pCampagne->m_LargColone;
					R.right=Rbase.right-2;
				}
				else {
					R.left=i*m_pCampagne->m_LargColone;
					R.right=R.left+m_pCampagne->m_LargColone;
				}

				if((m_pCampagne->fCurCalend==0 || m_pCampagne->fCurCalend==3) && m_pCampagne->SelectCalend[iV])FillRect(hDC,&R,theApp.hBrush[CLR_FONDSELECTCALEND]);

				txt.Format("%d",m_pCampagne->m_NrSemaine[iV]);

				R.right--;
				SetTextACCOGRIL(hDC,txt,R);
			}

			// la fleche de début
			posx=m_pCampagne->m_DebutCalend-m_pCampagne->m_PosAscH;
			if(posx==0)::DrawIcon(hDC,-1,R.top,theApp.hIcoFleche);
			else if(posx>0 && posx <m_pCampagne->m_NbColone){
				if(m_pCampagne->fCurCalend!=1)DrawIcon(hDC,posx*m_pCampagne->m_LargColone-4,R.top,theApp.hIcoFleche);
			}

			// la fleche de fin
			posx=m_pCampagne->m_FinCalend-m_pCampagne->m_PosAscH;
			if(m_pCampagne->fCurCalend!=2){
				if(posx>0 && posx <m_pCampagne->m_NbColone-1)DrawIcon(hDC,(1+posx)*m_pCampagne->m_LargColone-4,R.top,theApp.hIcoFleche);
				else if(posx==m_pCampagne->m_NbColone-1)DrawIcon(hDC,(1+posx)*m_pCampagne->m_LargColone-6,R.top,theApp.hIcoFleche);
			}

			// la fleche du NrSemaineX
			posx=m_pCampagne->m_NrSemaineX-m_pCampagne->m_PosAscH;
			if(m_pCampagne->fCurCalend!=3){
				if(posx>0 && posx <m_pCampagne->m_NbColone-1)DrawIcon(hDC,(1+posx)*m_pCampagne->m_LargColone-4,R.top,theApp.hIcoFleche2);
				else if(posx==m_pCampagne->m_NbColone-1)DrawIcon(hDC,(1+posx)*m_pCampagne->m_LargColone-6,R.top,theApp.hIcoFleche2);
			}

			// une fleche en déplacement ...
			if(m_pCampagne->fCurCalend==3)DrawIcon(hDC,m_pCampagne->PosXCurCalend-3,R.top,theApp.hIcoFleche2);
			else if(m_pCampagne->fCurCalend)DrawIcon(hDC,m_pCampagne->PosXCurCalend-3,R.top,theApp.hIcoFleche);



		break;

		case 1: // les binomes
			if(m_pCampagne->m_SetEnCours){
				FillRect(hDC,&R,theApp.hBrush[CLR_GRISC]);
				break;
			}
//			BitBlt(hDC,0,0,l,h,0,0,0,WHITENESS);
			FillRect(hDC,&R,theApp.hBrush[CLR_GRISC]);
//			FrameRect(hDC,&R,hBrush[CLR_NOIR]);
			SelectObject(hDC,theApp.hFont[FNT_HELV8]);

			R=Rbase;

		for(i=0;i<m_pCampagne->m_NbLigne;i+=2)
		{
				b=i/2+m_pCampagne->m_PosAscV;
				if(b>=m_pCampagne->m_Selection.GetSize())break;

				idxB=b;
				R.top=i*m_pCampagne->m_HautLigne+1;
				R.bottom=(i+m_pCampagne->m_Selection.GetNbParc(b))*m_pCampagne->m_HautLigne-1;
				// SetTextACCOGRIL(hDC,m_pCampagne->Binome[b].Nom,R);
				if(m_pCampagne->m_Selection.SelectBinome[idxB]){
					R.top--;
					FillRect(hDC,&R,theApp.hBrush[CLR_FONDSELECTBINOME]);
					R.top++;
				}
				else {
					R.top--;
					// FillRect(hDC,&R,hBrush[CLR_GRISC]);
					SelectObject(hDC,theApp.hPen[CLR_BLANC]);
					::MoveToEx(hDC,R.left,R.top,NULL);
					::LineTo(hDC,R.right,R.top);
					::MoveToEx(hDC,R.left,R.top,NULL);
					::LineTo(hDC,R.left,R.bottom);
					R.top++;
				}
				SelectObject(hDC,theApp.hPen[CLR_NOIR]);
				txt = m_pCampagne->m_Selection.GetLibelle(idxB);
				
				RECT Rtext;
				Rtext.left = R.left + 3;
				Rtext.top = R.top;
				Rtext.bottom = R.bottom;
				Rtext.right = R.right;

				::DrawText(hDC,txt,strlen(txt),&/*R*/Rtext,DT_NOPREFIX|DT_WORDBREAK/*|DT_CENTER*/);
				::MoveToEx(hDC,0,R.bottom,NULL);
				::LineTo(hDC,R.right,R.bottom);
		}

		break;

		case 2: // les passages
			FillRect(hDC,&R,theApp.hBrush[CLR_PAILLE]);
			SelectObject(hDC,theApp.hFont[FNT_SMALL7]);

			if(m_pCampagne->m_SetEnCours)break;

			R=Rbase;
			for(i=0;i<m_pCampagne->m_NbLigne ;i+=2)
			{

				int ip;
				b=i/2+m_pCampagne->m_PosAscV;
				if(b>=m_pCampagne->m_Selection.GetSize())break;
				idxB=b;
				ip=m_pCampagne->m_Selection.GetNrDirectParc(idxB,0);
				NbPassage=m_pCampagne->m_NbPassage[m_pCampagne->m_HypotheseActive][ip];
				for(p=1;p<m_pCampagne->m_Selection.GetNbParc(idxB);p++){
					if(NbPassage!=m_pCampagne->m_NbPassage[m_pCampagne->m_HypotheseActive][ip+p])break;
				}
				if(p>=m_pCampagne->m_Selection.GetNbParc(idxB)){
					// tous identique
					SelectObject(hDC,theApp.hFont[FNT_HELV8]);
					if(NbPassage)SetTextColor(hDC,theApp.rgb[CLR_VERT]);
					else SetTextColor(hDC,theApp.rgb[CLR_NOIR]);
					R.top=i*m_pCampagne->m_HautLigne+1;
					R.bottom=(i+m_pCampagne->m_Selection.GetNbParc(b))*m_pCampagne->m_HautLigne-1;
					txt.Format("%2d",NbPassage);
					SetTextACCOGRIL(hDC,txt,R);
				}
				else {
					// on donne le détail
					SelectObject(hDC,theApp.hFont[FNT_SMALL7]);
					SetTextColor(hDC,theApp.rgb[CLR_ROUGE]);
					for(p=0;p<m_pCampagne->m_Selection.GetNbParc(idxB);p++){
						R.top=(p+i)*m_pCampagne->m_HautLigne;
						R.bottom=(1+p+i)*m_pCampagne->m_HautLigne;
						txt.Format("%2d",m_pCampagne->m_NbPassage[m_pCampagne->m_HypotheseActive][ip+p]);
						SetTextACCOGRIL(hDC,txt,R);
					}
				}
				::MoveToEx(hDC,R.left,(i+m_pCampagne->m_Selection.GetNbParc(b))*m_pCampagne->m_HautLigne-1,NULL);
				::LineTo(hDC,R.right,(i+m_pCampagne->m_Selection.GetNbParc(b))*m_pCampagne->m_HautLigne-1);
			}
		break;

		case 3: // le coeur de la grille
			BitBlt(hDC,0,0,l,h,0,0,0,WHITENESS);
//			FrameRect(hDC,&R,hBrush[CLR_NOIR]);

			if(m_pCampagne->m_SetEnCours)break;

			// on n'utilise pas toujours tout !
			Rgrise=Rbase;
			if(Rbase.bottom>m_pCampagne->m_Selection.GetNbParcTotal()*m_pCampagne->m_HautLigne){

				Rbase.bottom=m_pCampagne->m_Selection.GetNbParcTotal()*m_pCampagne->m_HautLigne;

				Rgrise.top=Rbase.bottom;
			}
			else Rgrise.top=0;
			if(Rgrise.top>0)FillRect(hDC,&Rgrise,theApp.hBrush[CLR_FONDGRILLEINACTIF]);

			// à droite aussi il peut y avoir du grisé
			if(m_pCampagne->m_NbSemaine<m_pCampagne->m_NbColone){
				R=Rbase;
				R.left=m_pCampagne->m_NbSemaine*m_pCampagne->m_LargColone;
				FillRect(hDC,&R,theApp.hBrush[CLR_FONDGRILLEINACTIF]);
			}

			if(m_pCampagne->m_SetEnCours)break;

			SelectObject(hDC,theApp.hFont[FNT_SMALL7]);

			// le cadrillage de la grille
			R=Rbase;
			SelectObject(hDC,theApp.hPen[CLR_GRISF]);
			for(i=1;i<m_pCampagne->m_NbColone;i++){
				iV=i+m_pCampagne->m_PosAscH;

				::MoveToEx(hDC,i*m_pCampagne->m_LargColone-1,R.top,NULL);
				::LineTo(hDC,i*m_pCampagne->m_LargColone-1,Rgrise.bottom);
			}

			// les lignes des parcs
			SelectObject(hDC,theApp.hPen[CLR_GRISC]);
			for(i=0;i<m_pCampagne->m_NbLigne;i+=2){
				iB=i+m_pCampagne->m_PosAscV;
				if(iB>=m_pCampagne->m_Selection.GetNbParcTotal())break;

				::MoveToEx(hDC,R.left,(1+i)*m_pCampagne->m_HautLigne-1,NULL);
				::LineTo(hDC,R.right,(1+i)*m_pCampagne->m_HautLigne-1);
			}

			// les lignes des binômes
			SelectObject(hDC,theApp.hPen[CLR_NOIR]);
			for(i=1;i<m_pCampagne->m_NbLigne;i+=2){
				iB=i+m_pCampagne->m_PosAscV;
				if(iB>=m_pCampagne->m_Selection.GetNbParcTotal())break; 

				::MoveToEx(hDC,R.left,(1+i)*m_pCampagne->m_HautLigne-1,NULL);
				::LineTo(hDC,R.right,(1+i)*m_pCampagne->m_HautLigne-1);
			}

			ConstruitAccelerateurACCOGRIL();

			
			// le contenu de la grille
			
			SelectObject(hDC,theApp.hPen[CLR_NOIR]);
			for(c=0;c<m_pCampagne->m_NbColone;c++){
				iV=c+m_pCampagne->m_PosAscH;
				if(iV>=m_pCampagne->m_NbSemaine)break;
				for(i=0;i<m_pCampagne->m_NbLigne;i++){
					fige=0;
					iB=i+m_pCampagne->m_PosAscV*2;
					if(iB>=m_pCampagne->m_Selection.GetNbParcTotal())break; 
					
					if(m_pCampagne->m_Passage[m_pCampagne->m_HypotheseActive][iB][iV]==0)continue;
					if((TC=TypeDeCelluleACCOGRIL(iB,iV))<0)continue;
					// le produit est il figé?
					ligne = m_pCampagne->m_Selection.GetNrDirectBinome(iB);
					// la case sélectionnée appartient elle à une ligne de produit figé. 
					//DessineCelluleACCOGRIL(hDC,c*m_pCampagne->m_LargColone,i*m_pCampagne->m_HautLigne,TC);
					// Accélérateur à remettre...
					DessineCelluleSpeedACCOGRIL(hDC,m_pCampagne->m_hDCspeed,c*m_pCampagne->m_LargColone,i*m_pCampagne->m_HautLigne,TC);
				}
			}


			// les lignes de sélection pour le calcul de la base de mémo
			SelectObject(hDC,theApp.hPen[CLR_TRAITSELECTCALEND]);
			if(m_pCampagne->fCurCalend!=1){
				i=m_pCampagne->m_DebutCalend-m_pCampagne->m_PosAscH;
				if(i>=0 && i<m_pCampagne->m_NbColone){
					::MoveToEx(hDC,i*m_pCampagne->m_LargColone,R.top,NULL);
					::LineTo(hDC,i*m_pCampagne->m_LargColone,R.bottom);
				}
			}
			if(m_pCampagne->fCurCalend!=2){
				i=m_pCampagne->m_FinCalend-m_pCampagne->m_PosAscH;
				if(i>=0 && i<m_pCampagne->m_NbColone){
					i++;
					::MoveToEx(hDC,i*m_pCampagne->m_LargColone,R.top,NULL);
					::LineTo(hDC,i*m_pCampagne->m_LargColone,R.bottom);
				}
			}
			if(m_pCampagne->fCurCalend!=3){
				SelectObject(hDC,theApp.hPen[CLR_TRAITNRSEMAINEX]);
				i=m_pCampagne->m_NrSemaineX-m_pCampagne->m_PosAscH;
				if(i>=0 && i<m_pCampagne->m_NbColone){
					i++;
					::MoveToEx(hDC,i*m_pCampagne->m_LargColone-2,R.top,NULL);
					::LineTo(hDC,i*m_pCampagne->m_LargColone-2,R.bottom);
				}
			}



			// le rectangle de sélection
			if(m_pCampagne->fBloc){
				R=m_pCampagne->RBloc;
				FrameRect(hDC,&R,theApp.hBrush[CLR_ROUGE]);
				InflateRect(&R,1,1);
				FrameRect(hDC,&R,theApp.hBrush[CLR_JAUNE]);
				InflateRect(&R,1,1);
				FrameRect(hDC,&R,theApp.hBrush[CLR_ROUGE]);
				if(m_pCampagne->fBloc==2){
					DessineBlocDeplacementACCOGRIL(hDC);
				}
			}

			// la ligne en cours de séparation
			if(m_pCampagne->fCurCalend){
				if(m_pCampagne->fCurCalend==3)SelectObject(hDC,theApp.hPen[CLR_TRAITNRSEMAINEX]);
				else SelectObject(hDC,theApp.hPen[CLR_TRAITSELECTCALEND]);
				R=Rbase;
				::MoveToEx(hDC,m_pCampagne->PosXCurCalend,R.top,NULL);
				::LineTo(hDC,m_pCampagne->PosXCurCalend,R.bottom);
				::MoveToEx(hDC,m_pCampagne->PosXCurCalend+1,R.top,NULL);
				::LineTo(hDC,m_pCampagne->PosXCurCalend+1,R.bottom);
			}

			// on en finit avec le zones grises (inactives)
			if(m_pCampagne->m_NbSemaine<m_pCampagne->m_NbColone){
				SelectObject(hDC,theApp.hPen[CLR_NOIR]);
				R=Rbase;
				R.left=m_pCampagne->m_NbSemaine*m_pCampagne->m_LargColone-1;
				//FillRect(hDC,&R,hBrush[CLR_FONDGRILLEINACTIF]);
				::MoveToEx(hDC,R.left,R.top,NULL);
				::LineTo(hDC,R.left,R.bottom);
			}

		break;

		case 4: // le  bouton en bas
			FillRect(hDC,&R,theApp.hBrush[CLR_GRISC]);
		break;

	}


	// copy de la mem sur ecran

	BitBlt(hDCdest,Rdest.left,Rdest.top,Rdest.right,Rdest.bottom,hDC,Rdest.left,Rdest.top,SRCCOPY);
// 	pour voir l'accelerateur graphique:
//	BitBlt(hDCdest,Rdest.left,Rdest.top,Rdest.right,Rdest.bottom,m_pCampagne->hDCspeed,Rdest.left,Rdest.top,SRCCOPY);
	SelectObject(hDC,hBmpOld);
	DeleteObject(hBmpNew);
	DeleteDC(hDC);
	// ************************
	
	// Do not call CButton::OnPaint() for painting messages
}

void CGrille::Init(int part,CCampagne * campagne)
{
	m_Part=part;
	m_pCampagne=campagne;
	campagne->m_Btn[part]=this;
}

int CGrille::TraitSousCurseurACCOGRIL(POINT pt)
{
	int col;

	col=m_pCampagne->m_PosAscH+(pt.x+6)/m_pCampagne->m_LargColone;
	if(col>m_pCampagne->m_NbSemaine)return(0);

	// a-t-on un curseur sous la souris ?
	if(pt.y<m_pCampagne->m_HautLigneTitre || pt.y>m_pCampagne->m_HautLigneTitre+8)return(0);
	if(((pt.x+6)%m_pCampagne->m_LargColone)>=11)return(0);

	if(m_pCampagne->m_NrSemaineX==col-1)return(3);
	
	if(m_pCampagne->m_DebutCalend==col)return(1);
	if(m_pCampagne->m_FinCalend==col-1)return(2);
	return(0);
}


void CGrille::MajAffichageACCOGRIL(char fColoneBinome)
{
	m_pCampagne->CalculPassageACCOGRIL();
	if(fColoneBinome)
	{
		m_pCampagne->m_Btn[1]->InvalidateRect(NULL,0);
	}
	m_pCampagne->m_Btn[2]->InvalidateRect(NULL,0);
	m_pCampagne->m_Btn[3]->InvalidateRect(NULL,0);
	if(fColoneBinome)
	{
		m_pCampagne->m_Btn[1]->UpdateWindow();
	}
	m_pCampagne->m_Btn[2]->UpdateWindow();
	m_pCampagne->m_Btn[3]->UpdateWindow();
}

void CGrille::LiberationBlocPassACCOGRIL()
{
	int p,s;

	if(m_pCampagne->m_BlocPass==NULL)return;
	for(p=0;p<m_pCampagne->m_NbP;p++){
		for(s=0;s<m_pCampagne->m_NbS;s++){
			delete [] m_pCampagne->m_BlocPass[p][s];
		}
		delete [] m_pCampagne->m_BlocPass[p];
	}
	delete [] m_pCampagne->m_BlocPass;
	m_pCampagne->m_BlocPass=NULL;
}

void CGrille::ConstruitBlocPassACCOGRIL(int SDebut,int PDebut,char fEfface)
{
	int p,s,f,NrParc;

	LiberationBlocPassACCOGRIL();
	m_pCampagne->m_BlocPass=new char ** [m_pCampagne->m_NbP];
	for(p=0;p<m_pCampagne->m_NbP;p++){
		//NrParc=m_pCampagne->NrParcVisu[PDebut+p];
		NrParc = PDebut+p;

		m_pCampagne->m_BlocPass[p]=new char* [m_pCampagne->m_NbS];
		for(s=0;s<m_pCampagne->m_NbS;s++){
			m_pCampagne->m_BlocPass[p][s]=new char [m_pCampagne->m_NbFormat+1];
			for(f=0;f<m_pCampagne->m_NbFormat;f++){
				if(m_pCampagne->m_FlagFormat[f]&1 && m_pCampagne->m_Passage[m_pCampagne->m_HypotheseActive][NrParc][SDebut+s][f]){
					if(fEfface)m_pCampagne->m_Passage[m_pCampagne->m_HypotheseActive][NrParc][SDebut+s][f]=0;
					m_pCampagne->m_BlocPass[p][s][f]=1;
					m_pCampagne->m_BlocPass[p][s][m_pCampagne->m_NbFormat]++;
				}
			}
		}
	}
}

void CGrille::MajAffichageCalendACCOGRIL()
{
	m_pCampagne->m_Btn[0]->InvalidateRect(NULL,0);
	m_pCampagne->m_Btn[3]->InvalidateRect(NULL,0);
	m_pCampagne->m_Btn[0]->UpdateWindow();
	m_pCampagne->m_Btn[3]->UpdateWindow();
}


void CGrille::OnLButtonDown(UINT nFlags, CPoint point) 
{

	RECT R;
	int lineVisu,lineGrille,col;
	int NrParc,NewFlag,f,i,periode,ligne;
	
	// pour la gestion de l'offre figée
	int demipark1,demipark2;
	bool fige=0,passage=0;

	// Fc de gestion du clic gauche avec glissé laché
	m_fCapture=0;
	POINT pt;//ptf;
	pt.x=point.x; //LOWORD(lParam);
	pt.y=point.y; //HIWORD(lParam);
	char fShiftPressed,fCtrlPressed;
	if(nFlags&MK_SHIFT)fShiftPressed=1; else fShiftPressed=0;
	if(nFlags&MK_CONTROL)fCtrlPressed=1; else fCtrlPressed=0;

	switch(m_Part){
		case 0: // dans le calendrier
		/*
			col=m_pCampagne->m_PosAscH+(pt.x+3)/m_pCampagne->m_LargColone;
			if(col>m_pCampagne->m_NbSemaine)return(0);

			// a-t-on un curseur sour la souris ?
			if(pt.y>m_pCampagne->m_HautLigneTitre && pt.y<m_pCampagne->m_HautLigneTitre+8)
			if(((pt.x+5)%m_pCampagne->m_LargColone)<11)

			if(*m_pCampagne->ptrNrSemaineX==col-1){
				m_pCampagne->fCurCalend=3;
			}
			else if(*m_pCampagne->ptrDebutCalend==col){
				m_pCampagne->fCurCalend=1;
			}
			else if(*m_pCampagne->ptrFinCalend==col-1){
				m_pCampagne->fCurCalend=2;
			}*/
			m_pCampagne->fCurCalend=TraitSousCurseurACCOGRIL(pt);
			if(m_pCampagne->fCurCalend){
				m_fCapture=1;
				m_pCampagne->PosXCurCalend=pt.x;
				SetCapture();
				::SetCursor(theApp.hCurGlissement);
				GetWindowRect(&m_BtnRect);
				SetTimer(0,100,NULL);  //100 milli seconde en glissé laché
				InvalidateRect(NULL,0);
				UpdateWindow();
				break;
			}

			// rien, alors on regarde l'outils

		break;

		case 1: // selection des binôme pour
			lineVisu=m_pCampagne->m_PosAscV+pt.y/(2*m_pCampagne->m_HautLigne);
			if(lineVisu>=m_pCampagne->m_Selection.GetSize()*2)return;

			NrParc=lineVisu;


			// Sélection unique ou multiple avec Ctrl ou shift
//					if(!(fShiftPressed || fCtrlPressed))for(i=0;i<m_pCampagne->NbBinome;i++)m_pCampagne->SelectBinome[i]=0;
//					m_pCampagne->SelectBinome[line]=1;

//					C'etait losque l'on voulait des agrégats exclusifs
//					if(FenetreActive->fAffBinome){
//						int i;  					// pour les agrégats 1 seul !
//						for(i=0;i<m_pCampagne->NbBinomeVisu;i++)m_pCampagne->SelectBinome[m_pCampagne->NrBinomeVisu[i]]=0;
//					}

			// Un click à l'extérieur de la sélection
			if(NrParc>=m_pCampagne->m_Selection.SelectBinome.GetSize())return;
			// Sélection multiple
			m_pCampagne->m_Selection.SelectBinome[NrParc]=1-m_pCampagne->m_Selection.SelectBinome[NrParc];
			// CALCUL FRED
			// reconstruction de la liste des complexes et des bacs actifs
			m_pCampagne->m_Selection.ConstruitListeBacComplexeActifs();
			//-----------------------------------------------------
			
			// préparation du vecteur
			m_pCampagne->LanceCalcul(CCampagne::CIBLEUTILE);
			InvalidateRect(NULL,0);
			UpdateWindow();
			LanceCalcul(CCampagne::HYPOTHESESPOP);
		break;

		case 3: // coeur de grille
			SetModifiedFlag();
			

			col=m_pCampagne->m_PosAscH+pt.x/m_pCampagne->m_LargColone;
			if(col>=m_pCampagne->m_NbSemaine)return;
			lineGrille=pt.y/m_pCampagne->m_HautLigne;
			lineVisu=2*m_pCampagne->m_PosAscV+pt.y/m_pCampagne->m_HautLigne;
			if(lineVisu>=m_pCampagne->m_Selection.GetNbParcTotal())return;

			NrParc=lineVisu;
			periode=1;
			ligne = m_pCampagne->m_Selection.GetNrDirectBinome(NrParc);
			// la case sélectionnée appartient elle à une ligne de produit figé. 
			if(m_pCampagne->m_Selection.GetType(ligne)==tagSelection::PRODUITFIGE)
			{
				CTableProduit * TabProduit = theApp.m_TableCinema.GetTableProduit();
				int index = m_pCampagne->m_Selection.GetIndex(ligne);
				periode=TabProduit->GetAt(index).GetDuree(); 
				fige=1;
			}
			switch(m_pCampagne->OutilsActif){
				case 0: // Ajout-Supprime 1 passage

					if(fige)
					{
						for(f=0;f<m_pCampagne->m_NbFormat;f++)if((m_pCampagne->m_FlagFormat[f]&1) && m_pCampagne->m_Passage[m_pCampagne->m_HypotheseActive][NrParc][col][f]==0)break;
						if((f<m_pCampagne->m_NbFormat))NewFlag=1;
						else NewFlag=0;

						// Est il possible d'ajouter un nouveau produit figé?
						// Toutes les cases de la période sont elles libres?

						if(NewFlag) 
						{
							for(i=0;i<periode;i++)
							{
								if((col+periode)>m_pCampagne->m_NbSemaine)
								{
									NewFlag=0;
									MessageBeep(0xFFFFFFFF);
									break;
								}
								if((m_pCampagne->m_Passage[m_pCampagne->m_HypotheseActive][NrParc][col+i][f]>0)||(m_pCampagne->m_MapInterdit[col+i][NrParc][f]==1))
								{
									// mécanique d'alternance: le passage est il autorisé ?
									MessageBeep(0xFFFFFFFF);
									NewFlag=0;
									break;
								}

							}
						}
					}
					else
					{
						for(f=0;f<m_pCampagne->m_NbFormat;f++)if((m_pCampagne->m_FlagFormat[f]&1) && m_pCampagne->m_Passage[m_pCampagne->m_HypotheseActive][NrParc][col][f]==0)break;
						if(f<m_pCampagne->m_NbFormat)NewFlag=1;
						else NewFlag=0;

					}
					if(NrParc%2==0)	
					{
						// si on clique sur le premier demi park
						demipark1=NrParc;			
						demipark2=NrParc+1;			
					}
					else
					{
						// on clique sur le deuxième
						demipark1=NrParc-1;
						demipark2=NrParc;
					}
					for(f=0;f<m_pCampagne->m_NbFormat;f++)		
					{		
						if(m_pCampagne->m_FlagFormat[f]&1)		
						{	// si le produit est figé, on écrit dans les deux demi-park
							if(fige)		
							{		
								if(NewFlag)		
								{	// ajout	
									for(i=0;i<periode;i++)
									{	
										m_pCampagne->m_Passage[m_pCampagne->m_HypotheseActive][demipark1][col+i][f]=i+1;		
										m_pCampagne->m_Passage[m_pCampagne->m_HypotheseActive][demipark2][col+i][f]=i+1;		
										
									}		
								}		
								else{		
									// effacement		
									if(m_pCampagne->m_Passage[m_pCampagne->m_HypotheseActive][NrParc][col][f]!=0)
									{
										int pos = col - (m_pCampagne->m_Passage[m_pCampagne->m_HypotheseActive][NrParc][col][f]-1);		
										for(i=pos;i<pos+periode;i++)
										{		
											// controle du dépassement de l'exercice
											if(i>m_pCampagne->m_NbSemaine)break;
												m_pCampagne->m_Passage[m_pCampagne->m_HypotheseActive][demipark1][i][f]=0;		
												m_pCampagne->m_Passage[m_pCampagne->m_HypotheseActive][demipark2][i][f]=0;		
										}
									}
								}
									
							}		
							else		
							{		
								for(i=0;i<periode;i++)		
								{	
									// mécanique d'alternance: le passage est il autorisé ?
									if(m_pCampagne->m_MapInterdit[col+i][NrParc][f]==0)
									{
										m_pCampagne->m_Passage[m_pCampagne->m_HypotheseActive][NrParc][col+i][f]=NewFlag;
									}
								}		
							}
						}
					}
					MajAffichageACCOGRIL(0);
					// CALCUL FRED: on recalcule l'hypothese active
					//LanceCalcul(m_pCampagne->m_Selection.GetNrDirectBinome(NrParc));
					LanceCalcul(CCampagne::BUDGETPERF);
				break;
				case 1: // Déplacement
				
					for(f=0;f<m_pCampagne->m_NbFormat;f++)if((m_pCampagne->m_FlagFormat[f]&1) && m_pCampagne->m_Passage[m_pCampagne->m_HypotheseActive][NrParc][col][f])break;
					if(f>=m_pCampagne->m_NbFormat)return;
					
					if(!fige)
					{
						for(f=0;f<m_pCampagne->m_NbFormat;f++){
							if(!(m_pCampagne->m_FlagFormat[f]&1))continue;
							m_pCampagne->m_PassageTempo[f]=m_pCampagne->m_Passage[m_pCampagne->m_HypotheseActive][NrParc][col][f];
							m_pCampagne->m_Passage[m_pCampagne->m_HypotheseActive][NrParc][col][f]=0;
						}
					
						m_pCampagne->m_Btn[3]->InvalidateRect(NULL,0);
						m_pCampagne->m_Btn[3]->UpdateWindow();
						m_fCapture=1;
						SetCapture();
						::SetCursor(theApp.hCurMove);
						GetWindowRect(&R);
						R.bottom=R.top+=8+lineGrille*m_pCampagne->m_HautLigne;
						R.top--;
						SetCursorPos(R.left+m_pCampagne->m_LargColone*(col-m_pCampagne->m_PosAscH)+7,R.top-1);
						R.left-=5;
						R.right+=5;
						ClipCursor(&R);
						GetWindowRect(&m_BtnRect);
						SetTimer(0,100,NULL);  //100 milli seconde en glissé laché
					}
					else
					{
						::SetCursor(theApp.hCurInterdit);
					}

				break;
				/*case 5: // Bloc Déplacer
				case 6: // Bloc Copier Coller
						if(m_pCampagne->fBloc==1){
						ptf.x=pt.x;//+m_pCampagne->m_PosAscH*m_pCampagne->m_LargColone;
						ptf.y=pt.y;//+m_pCampagne->m_PosAscV*m_pCampagne->m_HautLigne;
						if(PtInRect(&m_pCampagne->RDebut,ptf)){
							m_pCampagne->m_OffsetX=ptf.x-m_pCampagne->RDebut.left;
							m_pCampagne->m_OffsetY=ptf.y-m_pCampagne->RDebut.top;
							OffsetRect(&m_pCampagne->RDebut,-m_pCampagne->RDebut.left-m_pCampagne->m_OffsetX,-m_pCampagne->RDebut.top-m_pCampagne->m_OffsetY);

							m_pCampagne->fBloc=2;
							if(m_pCampagne->m_SDebut>=0)ConstruitBlocPassACCOGRIL(m_pCampagne->m_SDebut,m_pCampagne->m_PDebut,m_pCampagne->OutilsActif==5);
							m_pCampagne->m_SDebut=-100;

							goto DebutCapture;
						}
						LiberationBlocPassACCOGRIL();
						// ça continue ...
					}*/
				case 2: // Bloc construire tempo
				case 3: // Bloc effacer
				case 4: // Bloc changer le format
					m_pCampagne->fBloc=1;
					m_pCampagne->CaseS=col;
					m_pCampagne->CaseP=lineVisu;
					m_pCampagne->RDebut.top=m_pCampagne->CaseP*m_pCampagne->m_HautLigne;
					m_pCampagne->RDebut.bottom=m_pCampagne->RDebut.top+m_pCampagne->m_HautLigne-1;
					m_pCampagne->RDebut.left=m_pCampagne->CaseS*m_pCampagne->m_LargColone;
					m_pCampagne->RDebut.right=m_pCampagne->RDebut.left+m_pCampagne->m_LargColone-1;

					m_pCampagne->RBloc=m_pCampagne->RDebut;
					OffsetRect(&m_pCampagne->RBloc,-m_pCampagne->m_LargColone*m_pCampagne->m_PosAscH,-m_pCampagne->m_HautLigne*m_pCampagne->m_PosAscV*2);

					m_pCampagne->m_Btn[3]->InvalidateRect(NULL,0);
					m_pCampagne->m_Btn[3]->UpdateWindow();
					//DebutCapture: // ************
					m_fCapture=1;
					SetCapture();
					::SetCursor(theApp.hCurBlocMove);
					GetWindowRect(&m_BtnRect);
					SetTimer(0,100,NULL);  //100 milli seconde en glissé laché
				break;
			}
		break;
	}
	return;
//	CButton::OnLButtonDown(nFlags, point);
}

void CGrille::LanceCalcul(int type)
{
	m_pCampagne->LanceCalcul(type);	
}

void CGrille::SetModifiedFlag(bool flag)
{
	m_pCampagne->m_pDoc->SetModifiedFlag(flag);  
}

BOOL CGrille::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	POINT pt;
	int line,ligne,col,NrParc,f;
	bool fige=0;
	if(m_Part==0){
		::GetCursorPos(&pt);
		ScreenToClient(&pt);
		if(TraitSousCurseurACCOGRIL(pt)){
			::SetCursor(theApp.hCurGlissement);
			return(0);
		}
	}
	else if(m_Part==3) // Coeur de grille
	{
		GetCursorPos(&pt);			
		ScreenToClient(&pt);			
		col=m_pCampagne->m_PosAscH+pt.x/m_pCampagne->m_LargColone;			
		line=m_pCampagne->m_PosAscV*2+pt.y/m_pCampagne->m_HautLigne;			
		if((line<0)||(col<0))return FALSE;
		NrParc=line;			
		int periode=0; // Periode du produit fige	

		if(NrParc>=m_pCampagne->m_Selection.GetNbParcTotal())return FALSE;
		ligne = m_pCampagne->m_Selection.GetNrDirectBinome(NrParc);					

		// la case sélectionnée appartient elle à une ligne de produit figé. 	
		if(m_pCampagne->m_Selection.GetType(ligne)==tagSelection::PRODUITFIGE)	
		{	
			// code du produit	
			CTableProduit * TabProduit = theApp.m_TableCinema.GetTableProduit();	
			int index = m_pCampagne->m_Selection.GetIndex(ligne);	
			periode=TabProduit->GetAt(index).GetDuree(); 	
			fige=1;	
		}
		
		if((col<m_pCampagne->m_NbSemaine)&&(line<m_pCampagne->m_Selection.GetNbParcTotal()-1))
		{
			for(f=0;f<m_pCampagne->m_NbFormat;f++)if(m_pCampagne->m_FlagFormat[f]&1)break;

			if(m_pCampagne->m_MapInterdit[col][line][f]==1)
			{
				::SetCursor(theApp.hCurInterdit);
				return(TRUE);
			}
			if(fige)
			{
				if(col+(periode-1)<m_pCampagne->m_NbSemaine)
				{
					// Si le produit figé est trop pret
					if(m_pCampagne->m_Passage[m_pCampagne->m_HypotheseActive][line][col+periode-1][f]>0)
					{
							::SetCursor(theApp.hCurInterdit);
							return(TRUE);
					}
					// Pour l'ajout,on controle qu'il n'y a pas de cases interdites indésirables
					else
					{
						// on boucle sur les cases des deux demi-park
						for(int colfige=0;colfige<col+periode;colfige++)
						{
							if((m_pCampagne->m_MapInterdit[colfige][line][f])||(m_pCampagne->m_MapInterdit[colfige][line+1][f]))
							{
								::SetCursor(theApp.hCurInterdit);
								return(TRUE);
							}
						}
					}
				}
				// Ou qu'il n'y a plus la place d'ajouter un produit fige!!
				else
				{
						::SetCursor(theApp.hCurInterdit);
						return(TRUE);

				}
			}
			// TODO FRED: curseur interdit
			/*else
			{
				if((m_pCampagne->m_Passage[m_pCampagne->m_HypotheseActive][line][col][f]==0)&&(m_pCampagne->OutilsActif==4))
				{
						// Y a t'il un passage pour le format a changer (orange).
						/*for(int k=0;k<m_pCampagne->m_NbFormat;k++)
						{
							if((!(m_pCampagne->m_FlagFormat[k]&1))&&(!(m_pCampagne->m_FlagFormat[k]&3)))break;
						}
						if (!m_pCampagne->m_Passage[m_pCampagne->m_HypotheseActive][line][col][k])
						{
							::SetCursor(theApp.hCurInterdit);
							return(TRUE);
						//}
				}
			}*/
		}	

		switch(m_pCampagne->OutilsActif){
			case 0:
				::SetCursor(theApp.hCurAjoutSupp);
			return(TRUE);
			case 1:
				if(m_fCapture)::SetCursor(theApp.hCurMove);
				else {
				if(col>=m_pCampagne->m_NbSemaine)break;			
				if(line>=m_pCampagne->m_Selection.GetNbParcTotal())break;			

					for(f=0;f<m_pCampagne->m_NbFormat;f++){
						if((m_pCampagne->m_FlagFormat[f]&1) && (m_pCampagne->m_Passage[m_pCampagne->m_HypotheseActive][NrParc][col][f]))break;
					}
					if(f>=m_pCampagne->m_NbFormat)
					{
						::SetCursor(theApp.hCurInterdit);
					}
					else if(fige)
					{
						::SetCursor(theApp.hCurInterdit);

					}
					else
					{
						::SetCursor(theApp.hCurGlissement);
					}
				}
			return(TRUE);
			case 2:
					if(fige)
					{
						::SetCursor(theApp.hCurInterdit);

					}
					else
					{
						::SetCursor(theApp.hCurBlocTempo);
					}
			return(TRUE);
			case 3:
				::SetCursor(theApp.hCurBlocEfface);
			return(TRUE);
			case 4:
				// TODO FRED: curseur interdit
				/*for(int k=0;k<m_pCampagne->m_NbFormat;k++)
				{
					if(m_pCampagne->m_FlagFormat[k]&2)break;
				}
				if((k<m_pCampagne->m_NbFormat)&&(m_pCampagne->m_Passage[m_pCampagne->m_HypotheseActive][line][col][k]))
				{*/
					::SetCursor(theApp.hCurBlocChange);
					return(TRUE);
				//}
		
			/*case 5:
					::SetCursor(theApp.hCurInterdit);
			return(TRUE);
			case 6:
				::SetCursor(theApp.hCurInterdit);
			return(TRUE);*/
		}
	}
	
//	return CButton::OnSetCursor(pWnd, nHitTest, message);
	return(0);
}


void CGrille::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(!m_fCapture)return;
	POINT pt;
	int posx,posy;

	pt.x=point.x; //LOWORD(lParam);
	pt.y=point.y; //HIWORD(lParam);
	//ClientToScreen(hBtn,&pt);

	// faut-il bouger l'ascenceur ?
	bool fAscHBouge=0,fAscVBouge=0;
//			if(wParam==999){ // on est obligé d'enlever ce contrôl->Saturation !
		// On bouge l'ascenseur horizontal
		posx=pt.x/m_pCampagne->m_LargColone;
		if(pt.x<0 && m_pCampagne->m_PosAscH>0){
			m_pCampagne->m_PosAscH--;
			::SetScrollPos(m_pCampagne->m_hAscH,SB_CTL,m_pCampagne->m_PosAscH,TRUE);
			fAscHBouge=1;
		}
		else if(posx>=m_pCampagne->m_NbColone && m_pCampagne->m_PosAscH<m_pCampagne->m_PosAscHmax){
			m_pCampagne->m_PosAscH++;
			::SetScrollPos(m_pCampagne->m_hAscH,SB_CTL,m_pCampagne->m_PosAscH,TRUE);
			fAscHBouge=1;
		}

		// On bouge l'ascenseur vertical
		posy=pt.y/m_pCampagne->m_HautLigne;
		if(pt.y<0 && m_pCampagne->m_PosAscV>0){
			m_pCampagne->m_PosAscV--;
			::SetScrollPos(m_pCampagne->m_hAscV,SB_CTL,m_pCampagne->m_PosAscV,TRUE);
			fAscVBouge=1;
		}
		else if(posy>=m_pCampagne->m_NbLigne && m_pCampagne->m_PosAscV<m_pCampagne->m_PosAscVmax){
			m_pCampagne->m_PosAscV++;
			::SetScrollPos(m_pCampagne->m_hAscV,SB_CTL,m_pCampagne->m_PosAscV,TRUE);
			fAscVBouge=1;
		}
	
//			}

	if(m_Part==0 && m_pCampagne->fCurCalend){
		int px=pt.x;
		if(px<0)px=0;
		int maxx=m_pCampagne->m_LargColone*min(m_pCampagne->m_NbColone,m_pCampagne->m_NbSemaine);
		if(px>maxx)px=maxx;
		m_pCampagne->PosXCurCalend=px;
	}

	if(m_Part==3 && m_pCampagne->fBloc)
	{
		int px=pt.x;
		if(px<1)px=1;
		if(px>=m_pCampagne->m_LargColone*m_pCampagne->m_NbColone-1)px=m_pCampagne->m_LargColone*m_pCampagne->m_NbColone-2;
		px+=m_pCampagne->m_PosAscH*m_pCampagne->m_LargColone;

		int py=pt.y;
		if(py<1)py=1;
		if(py>=m_pCampagne->m_HautLigne*m_pCampagne->m_NbLigne-1)py=m_pCampagne->m_HautLigne*m_pCampagne->m_NbLigne-2;
		py+=2*m_pCampagne->m_PosAscV*m_pCampagne->m_HautLigne;
		

		if(m_pCampagne->fBloc==1){
			m_pCampagne->RBloc=m_pCampagne->RDebut;
			if(px<m_pCampagne->RBloc.left)m_pCampagne->RBloc.left=px;
			else if(px>m_pCampagne->RBloc.right)m_pCampagne->RBloc.right=px;
			if(py<m_pCampagne->RBloc.top)m_pCampagne->RBloc.top=py;
			else if(py>m_pCampagne->RBloc.bottom)m_pCampagne->RBloc.bottom=py;
			OffsetRect(&m_pCampagne->RBloc,-m_pCampagne->m_LargColone*m_pCampagne->m_PosAscH,-m_pCampagne->m_HautLigne*m_pCampagne->m_PosAscV*2);
			}
		else if(m_pCampagne->fBloc==2){
			m_pCampagne->RBloc=m_pCampagne->RDebut;
			OffsetRect(&m_pCampagne->RBloc,px-m_pCampagne->m_LargColone*m_pCampagne->m_PosAscH,py-m_pCampagne->m_HautLigne*m_pCampagne->m_PosAscV*2);

			int CoinY=py-m_pCampagne->m_OffsetY;
			m_pCampagne->m_CoinDebutY=CoinY/m_pCampagne->m_HautLigne;
			if(CoinY%m_pCampagne->m_HautLigne>m_pCampagne->m_HautLigne/2)m_pCampagne->m_CoinDebutY++;
			int CoinX=px-m_pCampagne->m_OffsetX;
			m_pCampagne->m_CoinDebutX=CoinX/m_pCampagne->m_LargColone;
			if(CoinX%m_pCampagne->m_LargColone>m_pCampagne->m_LargColone/2)m_pCampagne->m_CoinDebutX++;
		}
	}

	bool dessinercentre=m_pCampagne->fBloc!=0;
	if(fAscHBouge || m_pCampagne->fCurCalend)
	{
		dessinercentre=0;
		MajAffichageCalendACCOGRIL();
	}
	if(fAscVBouge)
	{
		dessinercentre=0;
		MajAffichageACCOGRIL(1);
	}
	if(dessinercentre)
	{
		m_pCampagne->m_Btn[3]->InvalidateRect(NULL,0);
		m_pCampagne->m_Btn[3]->UpdateWindow();
	}

	if(fAscHBouge)
	{
		// Ce truc, c'était pour mettre l'ascenceur du graphe
		// en phase.
		// On enlève ce truc pour la version 32 bits
		//SetAscenceurEnPhase(m_pCampagne->hAscH);

	}

	return;
	
	//CButton::OnMouseMove(nFlags, point);
}

void CGrille::OnTimer(UINT nIDEvent) 
{
	POINT pt;
	GetCursorPos(&pt);
	if(!PtInRect(&m_BtnRect,pt)){
		ScreenToClient(&pt);
		SendMessage(WM_MOUSEMOVE,999,MAKELPARAM(pt.x,pt.y));
	}
	return;

//	CButton::OnTimer(nIDEvent);
}

void CGrille::CopyBlocPassACCOGRIL(int SDebut,int PDebut)
{
	int p,s,f;
	int NrParc;
	for(p=0;p<m_pCampagne->m_NbP;p++){
		//if(PDebut+p<0 || PDebut+p>=m_pCampagne->m_NbParcVisu)continue;
		if(PDebut+p<0 || PDebut+p>=m_pCampagne->m_Selection.GetNbParcTotal())continue;

		//NrParc=m_pCampagne->NrParcVisu[PDebut+p];
		NrParc=PDebut+p;

		for(s=0;s<m_pCampagne->m_NbS;s++){
			for(f=0;f<m_pCampagne->m_NbFormat;f++){
				if(SDebut+s<0 || SDebut+s>=m_pCampagne->m_NbSemaine)continue;
				if(m_pCampagne->m_BlocPass[p][s][f])m_pCampagne->m_Passage[m_pCampagne->m_HypotheseActive][NrParc][SDebut+s][f]=1;
			}
		}
	}
}


void CGrille::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(!m_fCapture)return;

	// on met fin à la capture
	ReleaseCapture();
	KillTimer(0);
	m_fCapture=0;
//	int A_FAIRE;
	//SetCursor(GDerCur);
	ClipCursor(NULL);

	POINT pt;
	pt.x=point.x; //LOWORD(lParam);
	pt.y=point.y; //HIWORD(lParam);

	int line,col,NrParc;
	int SDebut,SFin,PDebut,PFin;
	int CoinX,CoinY;
	int b,f,p,s,t,Dephasage;
	int flagbin;
	RECT R;

	// gestion de l'offre figée
	int periode,ligne,i,demipark1,demipark2;
	bool fige=0;
	
	switch(m_Part){
		int NrCalculALancer;
		case 0: // Calendrier  **************************************************
			if(pt.x<0)pt.x=0;
			if(pt.x>m_pCampagne->m_NbColone*m_pCampagne->m_LargColone)pt.x=m_pCampagne->m_NbColone*m_pCampagne->m_LargColone;
			col=m_pCampagne->m_PosAscH+(pt.x+(m_pCampagne->m_LargColone/2))/m_pCampagne->m_LargColone;
			if(col>m_pCampagne->m_NbSemaine)col=m_pCampagne->m_NbSemaine;

			if(m_pCampagne->fCurCalend==1)m_pCampagne->m_DebutCalend=col;
			else if(m_pCampagne->fCurCalend==2)m_pCampagne->m_FinCalend=col-1;
			else if(m_pCampagne->fCurCalend==3)m_pCampagne->m_NrSemaineX=col-1;

			if(m_pCampagne->fCurCalend==3)NrCalculALancer=-7;
			else NrCalculALancer=-4;
			m_pCampagne->fCurCalend=0;

			m_pCampagne->MajInfoCalendACCOGRIL();
			MajAffichageCalendACCOGRIL();
			LanceCalcul(NrCalculALancer);
		break;
		case 3: // Coeur de grille **************************************************
			if(pt.x<0)pt.x=0;
			if(pt.x>m_pCampagne->m_NbColone*m_pCampagne->m_LargColone)pt.x=m_pCampagne->m_NbColone*m_pCampagne->m_LargColone;
			col=m_pCampagne->m_PosAscH+pt.x/m_pCampagne->m_LargColone;
			if(col>=m_pCampagne->m_NbSemaine)col=m_pCampagne->m_NbSemaine-1;

			if(pt.y<0)pt.y=0;
			if(pt.y>m_pCampagne->m_NbLigne*m_pCampagne->m_HautLigne)pt.y=m_pCampagne->m_NbLigne*m_pCampagne->m_HautLigne;
			line=(m_pCampagne->m_PosAscV*2)+pt.y/m_pCampagne->m_HautLigne;
			if(line>=m_pCampagne->m_Selection.GetSize()*2)line=m_pCampagne->m_Selection.GetNbParcTotal()-1;

			NrParc=line;

			if(m_pCampagne->fBloc==1){
				// description du bloc
				if(col<m_pCampagne->CaseS){
					SDebut=col;SFin=m_pCampagne->CaseS;
					if(pt.x%m_pCampagne->m_LargColone>=m_pCampagne->m_LargColone/2)SDebut++;
				}
				else {
					SDebut=m_pCampagne->CaseS;SFin=col;
					if(col>m_pCampagne->CaseS){
						if(pt.x%m_pCampagne->m_LargColone<=m_pCampagne->m_LargColone/2)SFin--;
					}
				}
				if(line<m_pCampagne->CaseP){
					PDebut=line;PFin=m_pCampagne->CaseP;
					if(pt.y%m_pCampagne->m_HautLigne>=m_pCampagne->m_HautLigne/2)PDebut++;
				}
				else {
					PDebut=m_pCampagne->CaseP;PFin=line;
					if(line>m_pCampagne->CaseP){
						if(pt.y%m_pCampagne->m_HautLigne<=m_pCampagne->m_HautLigne/2)PFin--;
					}
				}
			}
			if(m_pCampagne->fBloc==2){
				CoinY=pt.y-m_pCampagne->m_OffsetY;
				PDebut=m_pCampagne->m_PosAscV+CoinY/m_pCampagne->m_HautLigne;
				PFin=PDebut+m_pCampagne->m_NbP-1;
				if(CoinY%m_pCampagne->m_HautLigne>m_pCampagne->m_HautLigne/2){
					PDebut++;PFin++;
				}
				CoinX=pt.x-m_pCampagne->m_OffsetX;
				SDebut=m_pCampagne->m_PosAscH+CoinX/m_pCampagne->m_LargColone;
				SFin=SDebut+m_pCampagne->m_NbS-1;
				if(CoinX%m_pCampagne->m_LargColone>m_pCampagne->m_LargColone/2){
					SDebut++;SFin++;
				}
			}

			if(m_pCampagne->OutilsActif<5)m_pCampagne->fBloc=0;

			// récupération de la table des tempos
			//CTableTempo * TabTempo = theApp.m_TableCinema.GetTableTempo();  

		switch(m_pCampagne->OutilsActif){
				case 1: // déplacement d'un bloc
					for(f=0;f<m_pCampagne->m_NbFormat;f++)if(m_pCampagne->m_PassageTempo[f])m_pCampagne->m_Passage[m_pCampagne->m_HypotheseActive][NrParc][col][f]=1;
						GetWindowRect(&R);
						SetCursorPos(R.left+m_pCampagne->m_LargColone*(col-m_pCampagne->m_PosAscH)+7,R.top+(line-m_pCampagne->m_PosAscV*2)*m_pCampagne->m_HautLigne+11);
				break;

				case 2: // un "TEMPO"
				{
		
					Dephasage=(1+SFin-SDebut)%(theApp.m_TableCinema.GetTableTempo()->GetPeriode(m_pCampagne->NrTempoActif));

					if(Dephasage>0)SFin+=theApp.m_TableCinema.GetTableTempo()->GetPeriode(m_pCampagne->NrTempoActif)-Dephasage;
					if(SFin>=m_pCampagne->m_NbSemaine-1)SFin=m_pCampagne->m_NbSemaine-1;

					for(b=m_pCampagne->m_Selection.GetNrDirectBinome(PDebut);b<=m_pCampagne->m_Selection.GetNrDirectBinome(PFin);b++){						for(p=0;p<m_pCampagne->m_Selection.GetNbParc(b);p++){
							flagbin=1<<p;
							ligne = b;				
							// la case sélectionnée appartient elle à une ligne de produit figé. 			
							if(m_pCampagne->m_Selection.GetType(ligne)==tagSelection::PRODUITFIGE)				
							{				
								fige=1;				
							}				
							for(f=0;f<m_pCampagne->m_NbFormat;f++){
								if(!(m_pCampagne->m_FlagFormat[f]&1))continue;
								for(s=SDebut;s<=SFin;s++){
									t=(s-SDebut)%theApp.m_TableCinema.GetTableTempo()->GetPeriode(m_pCampagne->NrTempoActif);
									if((theApp.m_TableCinema.GetTableTempo()->GetAt(m_pCampagne->NrTempoActif).GetPassage(t,flagbin-1))&&(!fige))
									{
										m_pCampagne->m_Passage[m_pCampagne->m_HypotheseActive][m_pCampagne->m_Selection.GetNrDirectParc(b,p)][s][f]=1;
									}
								}
							}
						}
					}
				}
				break;

				case 3: // GOMME
					
					for(s=SDebut;s<=SFin;s++){
						for(p=PDebut;p<=PFin;p++){
							if(p%2==0)
							{
								// si on clique sur le premier demi-park
								demipark1 = p;
								demipark2 = p+1;
							}
							else
							{
								// ou sur le deuxième
								demipark1 = p-1;
								demipark2 = p;
							}
							for(f=0;f<m_pCampagne->m_NbFormat;f++){
								// pour les produits figés, on efface tout le contenu
								// en ne sélectionnant qu'une case
								periode=1;		
								ligne = m_pCampagne->m_Selection.GetNrDirectBinome(p);		

								// la case sélectionnée appartient elle à une ligne de produit figé. 		
								if(m_pCampagne->m_Selection.GetType(ligne)==tagSelection::PRODUITFIGE)		
								{		
									CTableProduit * TabProduit = theApp.m_TableCinema.GetTableProduit();		
									int index = m_pCampagne->m_Selection.GetIndex(ligne);
									periode = TabProduit->GetAt(index).GetDuree();		
									fige=1;		
								}
								if((fige)&&(m_pCampagne->m_Passage[m_pCampagne->m_HypotheseActive][p][s][f]!=0))
								{
										int pos = s - (m_pCampagne->m_Passage[m_pCampagne->m_HypotheseActive][p][s][f]-1);
										for(i=pos;i<pos+periode;i++)
										{
											m_pCampagne->m_Passage[m_pCampagne->m_HypotheseActive][demipark1][i][f]=0;
											m_pCampagne->m_Passage[m_pCampagne->m_HypotheseActive][demipark2][i][f]=0;

										}
								}
								else
								{
									if(!fige)
									{
										if(m_pCampagne->m_FlagFormat[f]&1){
											m_pCampagne->m_Passage[m_pCampagne->m_HypotheseActive][p][s][f]=0;
										}
									}
								}
							}
						}
					}
				break;

				case 4: // changement de format
					for(s=SDebut;s<=SFin;s++){
						for(p=PDebut;p<=PFin;p++){
							NrParc=p;
							periode=1;		
							ligne = m_pCampagne->m_Selection.GetNrDirectBinome(p);		

							// la case sélectionnée appartient elle à une ligne de produit figé. 			
							if(m_pCampagne->m_Selection.GetType(ligne)==tagSelection::PRODUITFIGE)			
							{			
								CTableProduit * TabProduit = theApp.m_TableCinema.GetTableProduit();			
								int index = m_pCampagne->m_Selection.GetIndex(ligne);	
										
								periode = TabProduit->GetAt(index).GetDuree();			
								fige=1;			
							}
							if(fige)
							{
								for(f=0;f<m_pCampagne->m_NbFormat;f++)if(m_pCampagne->m_FlagFormat[f]&2)break;				
								if(m_pCampagne->m_FlagFormat[f]&2)
								{
									for(int k=0;k<m_pCampagne->m_NbFormat;k++)if(m_pCampagne->m_FlagFormat[k]&1)break;	
									int pos = s - (m_pCampagne->m_Passage[m_pCampagne->m_HypotheseActive][p][s][f]-1);

									if (pos+periode>m_pCampagne->m_NbSemaine) break;

									for(int j=0;j<periode;j++)
									{
										m_pCampagne->m_Passage[m_pCampagne->m_HypotheseActive][p][pos+j][f]=0;
										m_pCampagne->m_Passage[m_pCampagne->m_HypotheseActive][p+1][pos+j][f]=0;
										m_pCampagne->m_Passage[m_pCampagne->m_HypotheseActive][p][pos+j][k]=j+1;
										m_pCampagne->m_Passage[m_pCampagne->m_HypotheseActive][p+1][pos+j][k]=j+1;
									}
								}
							}
							else
							{
								for(f=0;f<m_pCampagne->m_NbFormat;f++)if(m_pCampagne->m_FlagFormat[f]&2 && m_pCampagne->m_Passage[m_pCampagne->m_HypotheseActive][NrParc][s][f])break;
								if(f>=m_pCampagne->m_NbFormat)continue;
								for(f=0;f<m_pCampagne->m_NbFormat;f++){
									if(m_pCampagne->m_FlagFormat[f]&1)m_pCampagne->m_Passage[m_pCampagne->m_HypotheseActive][NrParc][s][f]=1;
									else if(m_pCampagne->m_FlagFormat[f]&2)m_pCampagne->m_Passage[m_pCampagne->m_HypotheseActive][NrParc][s][f]=0;
								}
							}
						}
					}
				break;

				// déplacement 
				case 5:
					if(m_pCampagne->fBloc==2){
						CopyBlocPassACCOGRIL(SDebut,PDebut);
					}

					m_pCampagne->RDebut.top=PDebut*m_pCampagne->m_HautLigne;
					m_pCampagne->RDebut.bottom=(1+PFin)*m_pCampagne->m_HautLigne-1;
					m_pCampagne->RDebut.left=SDebut*m_pCampagne->m_LargColone;
					m_pCampagne->RDebut.right=(1+SFin)*m_pCampagne->m_LargColone-1;
					OffsetRect(&m_pCampagne->RDebut,-m_pCampagne->m_PosAscH*m_pCampagne->m_LargColone,0);
					m_pCampagne->RBloc=m_pCampagne->RDebut;
					m_pCampagne->m_NbS=SFin-SDebut+1;
					m_pCampagne->m_NbP=PFin-PDebut+1;

					if(m_pCampagne->fBloc==2){
						m_pCampagne->fBloc=1;
						if(m_pCampagne->OutilsActif==5){
							m_pCampagne->m_SDebut=SDebut;
							m_pCampagne->m_PDebut=PDebut;
						}
					}
					else {
						// ConstruitBlocPassACCOGRIL(m_pCampagne,SDebut,PDebut,0);
						m_pCampagne->m_SDebut=SDebut;
						m_pCampagne->m_PDebut=PDebut;
					}
				break;
			}
			MajAffichageACCOGRIL(0);
			// les calculs pour les courbes de mémo
			/*switch(m_pCampagne->OutilsActif){
				case 1: LanceCalcul(m_pCampagne->m_Selection.GetNrDirectBinome(NrParc)); break;
				// a améliorer pour gagner du temps de réponse :
				default: LanceCalcul(-1); break;
			}*/
			// CALCUL FRED
			LanceCalcul(CCampagne::BUDGETPERF);
		break;
	}
	return;
	
//	CButton::OnLButtonUp(nFlags, point);
}

void CGrille::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	OnLButtonDown(nFlags,point);	
//	CButton::OnLButtonDblClk(nFlags, point);
}

void CGrille::InitDessin()
{
	RECT R;

	// préparation des données pour dessiner
	GetWindowRect(&R);
	m_pCampagne->m_NbColone=(R.right-R.left+1)/m_pCampagne->m_LargColone;
	m_pCampagne->m_NbLigne=(R.bottom-R.top)/m_pCampagne->m_HautLigne;

	InitAscenceur();
}

void CGrille::InitAscenceur()
{
	// ascenceur horizontal
	m_pCampagne->m_PosAscH=0;
	m_pCampagne->m_PosAscHmax=max(0,m_pCampagne->m_NbSemaine-m_pCampagne->m_NbColone);

	::SetScrollRange(m_pCampagne->m_hAscH,SB_CTL,0,m_pCampagne->m_PosAscHmax,1);

	// ascenceur vertical
	m_pCampagne->m_PosAscV=0;
	m_pCampagne->m_PosAscVmax=max(0,m_pCampagne->m_Selection.GetSize()-(m_pCampagne->m_NbLigne/2));

	::SetScrollRange(m_pCampagne->m_hAscV,SB_CTL,0,m_pCampagne->m_PosAscVmax,1);
	
}

void CGrille::InitAccelerateur()
{
	RECT R;
	// preparation de l'accelerateur graphique
	HDC hDC=::GetDC(this->m_hWnd);
	m_pCampagne->m_hDCspeed=CreateCompatibleDC(hDC);
	GetClientRect(&R);
	int l=R.right-R.left;
	int h=R.bottom-R.top;
	m_pCampagne->m_hBmpNewspeed=CreateCompatibleBitmap(hDC,l,h);
	m_pCampagne->m_hBmpOldspeed=(HBITMAP)SelectObject(m_pCampagne->m_hDCspeed,m_pCampagne->m_hBmpNewspeed);
	BitBlt(m_pCampagne->m_hDCspeed,0,0,l,h,0,0,0,WHITENESS);
	SetBkMode(m_pCampagne->m_hDCspeed,TRANSPARENT);
	SelectObject(m_pCampagne->m_hDCspeed,theApp.hFont[FNT_SMALL7]);
	::ReleaseDC(this->m_hWnd,hDC);

}

