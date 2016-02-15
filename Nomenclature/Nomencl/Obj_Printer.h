// Obj_Printer.h: interface for the CObj_Printer class.
//
//////////////////////////////////////////////////////////////////////

#include <afxtempl.h>
#if !defined(AFX_OBJ_PRINTER_H__F68AFF76_82BD_11D2_BCC3_006052017E36__INCLUDED_)
#define AFX_OBJ_PRINTER_H__F68AFF76_82BD_11D2_BCC3_006052017E36__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "JFCBitmap.h"

class CObj_Printer  
{
/////////////////////////////////

//Orientation
#define	PORTRAIT		1
#define	PAYSAGE			2

//Nb lignes du corps de la page
#define	NBLIGNES_PORTRAIT		77
//#define	NBLIGNES_PAYSAGE		52
#define	NBLIGNES_PAYSAGE		53
#define	NBCOLONNES_PORTRAIT		80
#define	NBCOLONNES_PAYSAGE		130
//marge Feuille
#define MARGE_COL			1	// x% de la page  ds colonne
#ifndef MARGE_PAGE_G
#define MARGE_PAGE_G		5	// x% de la page  gauche
#endif
#ifndef MARGE_PAGE_D
#define MARGE_PAGE_D		5	// x% de la page  droit
#endif
#define MARGE_PAGE_H		2	// x% de la page  en haut
#define MARGE_PAGE_B		2	// x% de la page  en bas
//section d'un tableau
#define PRINT_INTERLIGNE	0
#define PRINT_TITRE			1
#define PRINT_TETE			2
#define PRINT_CORPS			3
#define PRINT_PIEDS			4
#define PRINT_TOTAL			5
//Font
#define MAXFONT				30
#define FONT_TIME6			25

#define FONT_TIME7			26
#define FONT_TIME7I			27

#define FONT_TIME8			10
#define FONT_TIME8B			11
#define FONT_TIME8I			12
#define FONT_TIME8U			13
#define FONT_TIME8BI		14
#define FONT_TIME8BU		15
#define FONT_TIME8IU		16
#define FONT_TIME8BIU		17

#define FONT_TIME10			1
#define FONT_TIME10B		2
#define FONT_TIME12			20
#define FONT_TIME12B		22
#define FONT_TIME12BI		23
#define FONT_TIME12BU		21
#define FONT_TIME14			3
#define FONT_TIME14BI		6
#define FONT_SERIF8			4
#define FONT_SERIF10		5

//Pen
#define MAXPEN				5
#define	PEN_1NOIR			0
#define	PEN_2NOIR			1
#define	PEN_4NOIR			2
#define	PEN_1GRISC			3
#define PEN_8NOIR			4

//Bitmap
#define MAXBITMAP           4
#define BITMAP_1            0
#define BITMAP_2            1
#define BITMAP_3            2
#define BITMAP_4            3

/////////////////////////////////
public:
	CObj_Printer();
	virtual ~CObj_Printer();

	virtual void SetDocument(CDocument * Doc);
	virtual bool OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);

public:
	//////////////////////////////////////////////////////////////////////////////////////////
	//Variables pour chaque tableau
	struct SInfoTab
	{
	public:
		SInfoTab(){
			m_NbLigneTitre=0;
			m_NbLigneTete=0;
			m_NbLigneCorps=0;
			m_NbLignePieds=0;
			m_NbLigneTotal=0;
			m_NbLigneInterLigne=0;
			m_CarColonne.RemoveAll();	//nb de caracteres pour chacune des colonnes
			m_NbCarGauche=0;
			m_NbCarDroit=0;
			m_fColonne=0;
			m_NbLigneGarantie=1;
		}
		SInfoTab(const SInfoTab & Data){*this=Data;}
		SInfoTab & operator=(const SInfoTab & Data){
			m_NbLigneTitre=Data.m_NbLigneTitre;
			m_NbLigneTete=Data.m_NbLigneTete;
			m_NbLigneCorps=Data.m_NbLigneCorps;
			m_NbLignePieds=Data.m_NbLignePieds;
			m_NbLigneTotal=Data.m_NbLigneTotal;
			m_NbLigneInterLigne=Data.m_NbLigneInterLigne;
			m_CarColonne.Copy(Data.m_CarColonne);
			m_NbCarGauche=Data.m_NbCarGauche;
			m_NbCarDroit=Data.m_NbCarDroit;
			m_fColonne=Data.m_fColonne;
			m_NbLigneGarantie=Data.m_NbLigneGarantie;
			return *this;
		}
		
		short m_NbLigneTitre;		// nb lignes pour titre tableau
		short m_NbLigneTete;		// nb lignes pour entete tableau
		short m_NbLigneCorps;		// nb lignes pour corps tableau
		short m_NbLignePieds;		// nb lignes pour pied tableau
		short m_NbLigneTotal;		// nb lignes pour un total tableau
		short m_NbLigneInterLigne;	// nb lignes pour interligne entre tableau
		CWordArray m_CarColonne;	// nb de caracteres pour chacune des colonnes
		short m_NbCarGauche;		// nb caracteres pour colonnes de gauche
		short m_NbCarDroit;			// nb caracteres pour colonnes de droite
		char  m_fColonne;			// 1:1appel par ligne  2:1appel par colonne
		char m_NbLigneGarantie;		// nb lignes successives garanties

	};
	typedef CArray<SInfoTab, SInfoTab> CInfoTabArray;
	//////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////
	//Variables renseignées lors de l'initialisation
	struct SIniPrint
	{
		CInfoTabArray m_InfoTab;	// stucture de tableau(x)
		short m_NbLigneTete;		// nombre de ligne d'entete de page
		short m_NbLignePieds;		// nombre de ligne de pieds de page
		short m_Marge[4];			// Marge haut(0) bas(1) gauche(2) droite(3) en % de la page
//		CFont * m_PoliceDef;		// police par defaut
		short m_fOrientation;		// 0:libre 1:Paysage 2:Portrait
	}m_Init;
	//////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////
	//description de chacune des lignes du tableau
	struct SMappTab
	{
		unsigned short m_Reel;				// index 1 à n ligne
		unsigned short m_Tab;				// numero du tableau
		unsigned short m_Type;				// 1:Titre 2:tete 3:corps 4:pieds 5:total
		unsigned short m_Lig;				// numero de la ligne
		unsigned short m_NbLig;				// nb ligne du type
		unsigned short m_NrCol;				// numero de la colonne
		unsigned short m_NbCol;				// nombre de colonne

		unsigned short m_Rappel;				// numero de ligne pour tracer les traits verticaux
	};
	typedef CArray<SMappTab, SMappTab> CMappTabArray;
	CMappTabArray m_Mapping;
	//////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////
	//description de chacune des bitmaps
	typedef CArray<JFCBitmap, JFCBitmap&> CBitmapArray;
	CBitmapArray m_Bitmaps;

	//Variables calculées AVANT l'initialisation
	short m_Orientation;

	//Variables calculées APRES l'initialisation
	short m_NbLignePage;			// Nombre de ligne par page
	short m_NbLigneCorps;			// Nombre de ligne dans le corps de la page
	short m_HLig;					// hauteur de ligne
	short m_LCol;					// largeur d'une colonne (car)
	short m_NbColPage;				// nombre de colonne par page
	short m_NbPage;					// nombre de page

	CFont    m_Font[MAXFONT];		// tableau de fonts
	CPen     m_Pen[MAXPEN];         // tableau de pen

	CPrintInfo * m_pInfo;

	//Fonstions à redéfinir ds objet fils
public:
	virtual void Initialise(CDC * pDC,SIniPrint * pInit);
	//Il y a 2 fonctions DrawTete pour permettre aux versions precedentes d'être compilé
	virtual bool DrawTete(CDC * pDC, CRect * pR, short NrPage, short NrLig, short PremierNrTab, short DernierNrTab);
	virtual bool DrawTete(CDC * pDC, CRect * pR, short NrPage, short NrLig);
	virtual bool DrawPieds(CDC * pDC, CRect * pR, short NrPage, short NrLig);
	virtual bool DrawTab(CDC * pDC, CRect * pR, short NrTab, short NrType, short NrLig);
	virtual bool DrawCol(CDC * pDC, CRect * pR, short NrTab, short NrType, short NrLig,short NrCol,short NbCol);
	virtual bool DrawVTab(CDC * pDC, CRect * pR_haut, CRect * pR_bas,short NrTab);
	virtual bool DrawBk(CDC * pDC, CRect * pR);

protected:
	void LoadFont(CDC* pDC, short Nr);
	void LoadPen(CDC* pDC, short Nr);
	void LoadBitmap(CDC* pDC, CString filename, short Nr);

	void CalculNbPage(CDC * pDC, CPrintInfo* pInfo);
	CRect CalcRect(CRect * pR, short Pour100_Debut=0, short Pour100_Fin=100);
	void  CalcBitmapRect(CRect* pR, DWORD largeur, DWORD hauteur, DWORD resoX, DWORD resoY);
	CPoint CalcPointMilieuG(CRect * pR);
	CPoint CalcPointMilieuD(CRect * pR);
	void TraitH(CDC * pDC, CRect * pR, short Pour100_Debut=0, short Pour100_Fin=100, CPen * pPen=NULL);
	void TraitHtop(CDC * pDC, CRect * pR, short Pour100_Debut=0, short Pour100_Fin=100, CPen * pPen=NULL);
	void TraitHbottom(CDC * pDC, CRect * pR, short Pour100_Debut=0, short Pour100_Fin=100, CPen * pPen=NULL);
	void TraitDoubleH(CDC * pDC, CRect * pR, short Pour100_Debut=0, short Pour100_Fin=100, CPen * pPen=NULL);
	void TraitV(CDC * pDC, CRect * pR_haut, CRect * pR_bas, short Pour100_Debut=0, short Pour100_Fin=100, CPen * pPen=NULL);
	void TraitVfull(CDC * pDC, CRect * pR_haut, CRect * pR_bas, short Pour100_Debut=0, short Pour100_Fin=100, CPen * pPen=NULL);
	void TraitDoubleV(CDC * pDC, CRect * pR_haut, CRect * pR_bas, short Position, CPen * pPen=NULL);
	void TraitDoubleVfull(CDC * pDC, CRect * pR_haut, CRect * pR_bas, short Position, CPen * pPen=NULL);

	void TraitVG(CDC * pDC, CRect * pR_haut, CRect * pR_bas, short Pour100_Debut=0, short Pour100_Fin=100, CPen * pPen=NULL);
	void TraitVGfull(CDC * pDC, CRect * pR_haut, CRect * pR_bas, short Pour100_Debut=0, short Pour100_Fin=100, CPen * pPen=NULL);
	void TraitVD(CDC * pDC, CRect * pR_haut, CRect * pR_bas, short Pour100_Debut=0, short Pour100_Fin=100, CPen * pPen=NULL);
	void TraitVDfull(CDC * pDC, CRect * pR_haut, CRect * pR_bas, short Pour100_Debut=0, short Pour100_Fin=100, CPen * pPen=NULL);
	void EcritG(CDC * pDC, CString txt, CRect * pR, short Pour100_Debut=0, short Pour100_Fin=100, short Offset=0, CFont * pFont=NULL, COLORREF TxtRGB=RGB(0,0,0));
	void EcritD(CDC * pDC, CString txt, CRect * pR, short Pour100_Debut=0, short Pour100_Fin=100, short Offset=0, CFont * pFont=NULL, COLORREF TxtRGB=RGB(0,0,0));
	void EcritC(CDC * pDC, CString txt, CRect * pR, short Pour100_Debut=0, short Pour100_Fin=100, CFont * pFont=NULL, COLORREF TxtRGB=RGB(0,0,0));
	//La première fonction de Bitmap dessine le bitmap à l'écran mais ne l'imprime pas correctement
	//Le seconde corrige ce soucis
	void DessineBitmap(CDC * pDC,CRect * pR,CBitmap * pBmp, short PDebut=0, short PFin=100);
	void DessineBitmap(CDC* pDC, short Nr, CRect* pR, short Pour100_Debut=0, short Pour100_Fin=100);
	void RemplisRect(CDC* pDC, CRect * pR, COLORREF rgb, short Pour100_Debut=0, short Pour100_Fin=100);
	void ShadowRect(CDC* pDC, CRect * pR_haut, CRect * pR_bas, COLORREF rgb=RGB(0,0,0), short Pour100_Debut=0, short Pour100_Fin=100);
	void DessineCadre(CDC* pDC, CRect * pR_haut, CRect * pR_bas, short Pour100_Debut=0, short Pour100_Fin=100, CPen * pPen=NULL);
	void DessineCadre2(CDC* pDC, CRect * pR_haut, CRect * pR_bas, short Pour100_Debut=0, short Pour100_Fin=100, CPen * pPen=NULL);

private:
	bool YAPasPLace(short reel,short place);
	bool NouvellePage(short &reel);
};

#endif // !defined(AFX_OBJ_PRINTER_H__F68AFF76_82BD_11D2_BCC3_006052017E36__INCLUDED_)

