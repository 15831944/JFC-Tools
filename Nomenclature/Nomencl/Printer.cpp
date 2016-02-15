// Printer.cpp: implementation of the CPrinter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Printer.h"
#include "Nomenclature.h"
#include "Nomenclaturedoc.h"
#include "Nomenclatureview.h"
#include "TreeCtrlX.h"

extern CNomenclatureApp theApp;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPrinter::CPrinter()
{
	m_pView=NULL;
}

CPrinter::~CPrinter()
{

}

void CPrinter::Initialise(CDC * pDC,SIniPrint * pInit)
{
	ASSERT(m_pView!=NULL);

	// Affichage particularité client
	LoadBitmap(pDC,theApp.m_PrintLogo,0);

    // Définition de la page
	pInit->m_NbLigneTete=12;
	pInit->m_NbLignePieds=10;
	pInit->m_Marge[0]=2;	//haut
	pInit->m_Marge[1]=2;	//bas
	pInit->m_Marge[2]=5;	//gauche
	pInit->m_Marge[3]=5;	//droite

	LoadFont(pDC, FONT_TIME8);
	LoadFont(pDC, FONT_TIME8B);
	LoadFont(pDC, FONT_TIME8I);
	LoadFont(pDC, FONT_TIME10);
	LoadFont(pDC, FONT_TIME10B);
	LoadFont(pDC, FONT_TIME12);
	LoadFont(pDC, FONT_TIME12B);
	LoadFont(pDC, FONT_TIME14);
		
	LoadPen(pDC, PEN_1NOIR);
	LoadPen(pDC, PEN_2NOIR);
	LoadPen(pDC, PEN_4NOIR);
	pInit->m_fOrientation=0;		//0:libre 1:Paysage 2:Portrait


	// Définition des tableaux
	SInfoTab InfoTab;

	if (m_pView->m_TabTxtNomencl.GetSize() > 0)
	{
		InfoTab.m_NbLigneTitre=0;
		InfoTab.m_NbLigneTete=0;

		// Taille arbo nomenclure A FAIRE
		InfoTab.m_NbLigneCorps=m_pView->m_TabTxtNomencl.GetSize();
		//InfoTab.m_NbLigneCorps = 127;
		InfoTab.m_NbLignePieds=0;
		InfoTab.m_NbLigneTotal=0;
		InfoTab.m_NbLigneInterLigne=0;
		InfoTab.m_fColonne=0;				// mode multicolonne
		InfoTab.m_NbCarGauche=0;
		InfoTab.m_CarColonne.SetSize(0);	 	
		InfoTab.m_NbLigneGarantie=1;		 
		pInit->m_InfoTab.Add(InfoTab);
	}

	// remplissage du Tableau Texte des lignes nomenclatures
	//m_pView->CreerTabTxtNomencl();


}

bool CPrinter::DrawTete(CDC * pDC, CRect * pR, short NrPage, short NrLig)
{
	return(0);
}
	
bool CPrinter::DrawPieds(CDC * pDC, CRect * pR, short NrPage, short NrLig)
{

	switch (NrLig)
	{
	case 3:

		{
			// on calcule la position du logo
			CRect R  = *pR;
			R.bottom = R.top + 4 * m_HLig;
			// on dessine le logo
			DessineBitmap(pDC,0,&R,4,100 );
		}
	break;

	case 8:
		{
			// on selectionne un stylo fin noir
			CPen *pPen = pDC->SelectObject( &m_Pen[PEN_1NOIR] );
			// on ecrit la raison sociale
			EcritC( pDC,theApp.m_PrintRaison, pR, 0, 100, &m_Font[FONT_TIME8] );
			// on restaure l'ancien stylo
			pDC->SelectObject(pPen);
		}
	break;
	}

	// on quitte
	return TRUE;

}

// Et hop on écrit la ligne nomenclature
void CPrinter::DessineLigne(CDC * pDC, CRect * pR,CFont * pFont,CString Txt)
{
	int PrctX;
	int CodeDessin;
	CString Tmp;
	CString TiretEsp  ="--------";
	CString Espace = "           ";
	CString Tiret = "--------";
	int PosDeb = 33;

	// Récupère le niveau du thème nomenclature
	PrctX = atoi(Txt.Left(1));

	// Récupère le code dessin
	CodeDessin = atoi(Txt.Mid(1,1));

	Tmp = "";

	if (PrctX > 2) Tmp = Tmp + TiretEsp;

	// si niveau campagne on décale encore plus
	if (CodeDessin == CTreeCtrlX::ImageCampagne)
	{
		if (PrctX > 3)
			Tmp = "               ---------" + Tmp + ">";
		else
			Tmp = "------------------------" + Tmp + ">";
	}

	// des blancs si niveau <=2
	if (PrctX <= 2)
		PosDeb = PosDeb + (PrctX-1)*5;
	else
		PosDeb = PosDeb + 6;

	// on ajoute le texte item (vire les 2 ers caractéres = niveau + code dessin)
	Tmp = Tmp + " " + Txt.Mid(2);
	EcritG(pDC,Tmp,pR,PosDeb ,100 ,0,pFont);

}

bool CPrinter::DrawTab(CDC * pDC, CRect * pR, short NrTab, short NrType, short NrLig)
{

	switch(NrType)
	{
		case PRINT_INTERLIGNE:break;
		case PRINT_TITRE:break;
		case PRINT_TETE:
			TraitH(pDC, pR,0,100,&m_Pen[PEN_2NOIR]);
		break;
		case PRINT_CORPS:
			{
				// Impression Arborescence Nomenclature
				CString TxtItem;
				int CodeDessin;

				TxtItem =  m_pView->m_TabTxtNomencl[NrLig];
				CodeDessin = atoi(TxtItem.Mid(1,1));

				if (NrLig > 0)
				{
					if (CodeDessin == CTreeCtrlX::ImageGroupe )

						// En gras les niveau 1 arborescence (Groupe)
						DessineLigne(pDC,pR, &m_Font[FONT_TIME10],TxtItem);
					else if (atoi(TxtItem.Mid(1,1)) == CTreeCtrlX::ImageCampagne)
						// Niveau campagne
						DessineLigne(pDC,pR, &m_Font[FONT_TIME8I],TxtItem);

						// Niveau Annonceur et Marque
					else if (atoi(TxtItem.Mid(1,1)) == CTreeCtrlX::ImageAnnonceur ||
							 atoi(TxtItem.Mid(1,1)) == CTreeCtrlX::ImageMarque ||
							 atoi(TxtItem.Mid(1,1)) == CTreeCtrlX::ImageSecteur)
						//DessineLigne(pDC,pR, &m_Font[FONT_TIME8B],TxtItem);
						DessineLigne(pDC,pR, &m_Font[FONT_TIME10B],TxtItem);

					else
						//DessineLigne(pDC,pR, &m_Font[FONT_TIME8],TxtItem);
						DessineLigne(pDC,pR, &m_Font[FONT_TIME10],TxtItem);

				}
			}
		break;

		///
		case PRINT_PIEDS:
			switch(NrLig)
			{
			case 0:
			case 2:
				TraitH(pDC, pR,0,100,&m_Pen[PEN_2NOIR]);
			break;
			}

		break;
		//
		
		case PRINT_TOTAL:break;
		}

	return 0;
}
bool CPrinter::DrawVTab(CDC * pDC, CRect * pR_haut, CRect * pR_bas,short NrTab)
{
	return 1;
}

bool CPrinter::DrawTete(CDC * pDC, CRect * pR, short NrPage, short NrLig,short PremierNrTab,short DernierNrTab)
{

	CString txt,libcible;
	switch(NrLig)
	{
	case 0:
		//JFC Titre
		EcritG(pDC, "JFC", pR, 0, 100, 0, &m_Font[FONT_TIME10]);
		EcritD(pDC,theApp.m_PrintUser, pR, 0, 100, 0, &m_Font[FONT_TIME8]);
		txt="REPERTOIRE CLIENT";
		pR->top=pR->top-pR->Height()/2;
		pR->bottom=pR->bottom+pR->Height()/2;
		EcritC(pDC, txt, pR, 20, 80, &m_Font[FONT_TIME12]);
		break;
	case 3:{
		//date
		COleDateTime date=COleDateTime::GetCurrentTime();
		txt.Format("Le %02d/%02d/%4d - %02dH%02d", date.GetDay(), date.GetMonth(), date.GetYear(), date.GetHour(), date.GetMinute());
		EcritG(pDC, txt, pR, 0, 100, 0, &m_Font[FONT_TIME8]);
		txt.Format("Page %2d", NrPage+1);
		EcritD(pDC, txt, pR, 0, 100, 0, &m_Font[FONT_TIME8]);
		break;
		}
	case 4:
		TraitH(pDC, pR,0,100,&m_Pen[PEN_4NOIR]);
		break;

	case 6:{
		CRect R=*pR;
		R.top+=m_HLig/2;
		R.bottom+=5*m_HLig/2;
		RemplisRect(pDC, &R, RGB(192,255,192));
		TraitH(pDC, pR,0,100,&m_Pen[PEN_4NOIR]);
		}
		break;

	case 9:{
		// Affichage Titre = Repertoire Client
		CRect R=*pR;
		R.top-=3*m_HLig/2;
		R.bottom=R.top+m_HLig;

		txt="Nomenclature " + theApp.m_TypeNomencl + "  (Année " + theApp.m_AnneeNomencl + ")";
		
		EcritC(pDC,txt, &R, 0, 100, &m_Font[FONT_TIME12B]);
		TraitH(pDC, pR,0,100,&m_Pen[PEN_4NOIR]);
		break;
		}

	case 11:
		{
			TraitH(pDC, pR,0,100,&m_Pen[PEN_4NOIR]);
			CRect R=*pR;
			R.top=pR->top-(7*m_HLig);
			R.bottom=R.top+m_HLig;
			TraitV(pDC, &R, pR,0,100,&m_Pen[PEN_4NOIR]);
		}
		break;
	}
	return 1;
}


