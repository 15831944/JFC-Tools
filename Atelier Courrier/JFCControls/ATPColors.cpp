
#include "stdafx.h"
#include ".\atpcolors.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CATPColors CATPColors::m_Colors;

CATPColors::CATPColors(void)
{
	// Nouvelle couleur
	/// Bottle Green
	m_Color_Light[BOTTLE_GREEN]				=	RGB(216, 234, 227);
	m_Color_Medium[BOTTLE_GREEN]			=	RGB(146,186,173);
	m_Color_Dark[BOTTLE_GREEN]				=	RGB( 99,157,137);
	m_Color_SuperDark[BOTTLE_GREEN]			=   RGB( 50, 100, 83);

	// Les couleurs finales du dégradé
	m_Color_Light_EndDegrad[BOTTLE_GREEN]	=	RGB(239, 248, 244);
	m_Color_Medium_EndDegrad[BOTTLE_GREEN]	=	RGB(226, 236, 233);
	m_Color_Dark_EndDegrad[BOTTLE_GREEN]	=	RGB(214, 238, 230);
	
	m_Color_Select[BOTTLE_GREEN]			=	RGB(  0,140,120);
	m_Color_Moyenne[BOTTLE_GREEN]			=	RGB(245,215, 10);

	// Emeraude
	m_Color_Light[EMERAUDE]					=	RGB(221,255,232);
	m_Color_Medium[EMERAUDE]				=	RGB(159,255,191);
	m_Color_Dark[EMERAUDE]					=	RGB(0,180,60);	
	m_Color_SuperDark[EMERAUDE]				=   RGB(0, 117, 39);

	// Les couleurs finales du dégradé
	m_Color_Light_EndDegrad[EMERAUDE]		=	RGB(244, 251, 246);
	m_Color_Medium_EndDegrad[EMERAUDE]		=	RGB(232, 249, 238);
	m_Color_Dark_EndDegrad[EMERAUDE]		=	RGB(212, 248, 224);

	m_Color_Select[EMERAUDE]				=   RGB(0,118,39);
	m_Color_Moyenne[EMERAUDE]				=   RGB(245,215, 10);

	// Bleu Pastel Alain
	m_Color_Light[BLEU_PASTEL]				=	RGB(217,232,255);
	m_Color_Medium[BLEU_PASTEL]				=	RGB(171,196,214);
	m_Color_Dark[BLEU_PASTEL]				=	RGB(137, 174, 229);	
	m_Color_SuperDark[BLEU_PASTEL]			=   RGB(47, 88, 149);

	// Les couleurs finales du dégradé
	m_Color_Light_EndDegrad[BLEU_PASTEL]	=	RGB(239, 242, 248);
	m_Color_Medium_EndDegrad[BLEU_PASTEL]	=	RGB(232, 238, 243);
	m_Color_Dark_EndDegrad[BLEU_PASTEL]		=	RGB(246, 248, 251);

	m_Color_Select[BLEU_PASTEL]				=   RGB(105,105,167);
	m_Color_Moyenne[BLEU_PASTEL]			=   RGB(245,215, 10);

	// Ocean
	m_Color_Light[OCEAN]					=	RGB(220,239,240);
	m_Color_Medium[OCEAN]					=	RGB(176,219,222);
	m_Color_Dark[OCEAN]						=	RGB(130,199,204);
	m_Color_SuperDark[OCEAN]				=   RGB(55, 133, 139);

	// Les couleurs finales du dégradé
	m_Color_Light_EndDegrad[OCEAN]			=	RGB(242, 249, 249);
	m_Color_Medium_EndDegrad[OCEAN]			=	RGB(237, 245, 246);
	m_Color_Dark_EndDegrad[OCEAN]			=	RGB(226, 240, 241);

	m_Color_Select[OCEAN]					=   RGB(65,150,157);
	m_Color_Moyenne[OCEAN]					=   RGB(245,215, 10);

	// Lilas
	m_Color_Light[LILAS]					=	RGB(202,193,221);
	m_Color_Medium[LILAS]					=	RGB(153,148,186);
	m_Color_Dark[LILAS]						=	RGB(118,76,146);	
	m_Color_SuperDark[LILAS]				=   RGB(74, 33, 102);

	// Les couleurs finales du dégradé
	m_Color_Light_EndDegrad[LILAS]			=	RGB(235, 231, 243);
	m_Color_Medium_EndDegrad[LILAS]			=	RGB(222, 221, 229);
	m_Color_Dark_EndDegrad[LILAS]			=	RGB(219, 208, 226);
	
	m_Color_Select[LILAS]					=   RGB(135,161,36);
	m_Color_Moyenne[LILAS]					=   RGB(245,215, 10);

	// Sahara
	m_Color_Light[SAHARA]					=	RGB(255,255,215);
	m_Color_Medium[SAHARA]					=	RGB(255,233,137);
	m_Color_Dark[SAHARA]					=	RGB(253,153,0);	
	m_Color_SuperDark[SAHARA]				=   RGB(163, 100, 3);

	// Les couleurs finales du dégradé
	m_Color_Light_EndDegrad[SAHARA]			=	RGB(249, 249, 244);
	m_Color_Medium_EndDegrad[SAHARA]		=	RGB(244, 239, 214);
	m_Color_Dark_EndDegrad[SAHARA]			=	RGB(248, 234, 213);
	
	m_Color_Select[SAHARA]					=   RGB(102,51,0);
	m_Color_Moyenne[SAHARA]					=   RGB(245,215, 10);
	
	// Terre de sienne
	m_Color_Light[TERRE_SIENNE]				=	RGB(255,205,155);
	m_Color_Medium[TERRE_SIENNE]			=	RGB(255,130,5);
	m_Color_Dark[TERRE_SIENNE]				=	RGB(204,102,0);	
	m_Color_SuperDark[TERRE_SIENNE]			=   RGB(141, 71, 0);

	// Les couleurs finales du dégradé
	m_Color_Light_EndDegrad[TERRE_SIENNE]	=	RGB(248, 236, 225);
	m_Color_Medium_EndDegrad[TERRE_SIENNE]	=	RGB(248, 220, 193);
	m_Color_Dark_EndDegrad[TERRE_SIENNE]	=	RGB(239, 218, 198);

	m_Color_Select[TERRE_SIENNE]			=   RGB(150,75,0);
	m_Color_Moyenne[TERRE_SIENNE]			=   RGB(245,215, 10);
		
	// Rubis
	m_Color_Light[RUBIS]					=	RGB(255,129,129);
	m_Color_Medium[RUBIS]					=	RGB(255,41,41);
	m_Color_Dark[RUBIS]						=	RGB(238,0,0);	
	m_Color_SuperDark[RUBIS]				=   RGB(164, 0, 0);

	// Les couleurs finales du dégradé
	m_Color_Light_EndDegrad[RUBIS]			=	RGB(246, 227, 227);
	m_Color_Medium_EndDegrad[RUBIS]			=	RGB(246, 190, 190);
	m_Color_Dark_EndDegrad[RUBIS]			=	RGB(246, 212, 212);
	
	m_Color_Select[RUBIS]					=   RGB(154,0,0);
	m_Color_Moyenne[RUBIS]					=   RGB(245,215, 10);

	// Gris souris
	m_Color_Light[GREY_MOUSE]				=	RGB(192,192,192);
	m_Color_Medium[GREY_MOUSE]				=	RGB(128,128,128);
	m_Color_Dark[GREY_MOUSE]				=	RGB( 96, 96, 96);
	m_Color_SuperDark[GREY_MOUSE]			=   RGB( 61, 59, 59);

	// Les couleurs finales du dégradé
	m_Color_Light_EndDegrad[GREY_MOUSE]		=	RGB(233, 233, 233);
	m_Color_Medium_EndDegrad[GREY_MOUSE]	=	RGB(222, 222, 222);
	m_Color_Dark_EndDegrad[GREY_MOUSE]		=	RGB(228, 228, 228);
	
	m_Color_Select[GREY_MOUSE]				=	RGB(127,127,127);
	m_Color_Moyenne[GREY_MOUSE]				=	RGB(245,215, 10);
	
	/// Rose (Demande de Gael)
	m_Color_Light[ROSE]						=	RGB(254,216,244);
	m_Color_Medium[ROSE]					=	RGB(249,98, 211);
	m_Color_Dark[ROSE]						=	RGB(245,16, 188);
	m_Color_SuperDark[ROSE]					=   RGB(154, 9, 118);

	// Les couleurs finales du dégradé
	m_Color_Light_EndDegrad[ROSE]			=	RGB(251, 242, 249);
	m_Color_Medium_EndDegrad[ROSE]			=	RGB(249, 227, 244);
	m_Color_Dark_EndDegrad[ROSE]			=	RGB(246, 219, 239);
	
	m_Color_Select[ROSE]					=	RGB(200, 0, 168);
	m_Color_Moyenne[ROSE]					=	RGB(245,215, 10);
		
	// Rose Kaki
	m_Color_Light[ROSE_KAKI]				=	RGB(255,221,221);
	m_Color_Medium[ROSE_KAKI]				=	RGB(255,167,167);
	m_Color_Dark[ROSE_KAKI]					=	RGB(142,140,70);
	m_Color_SuperDark[ROSE_KAKI]			=   RGB( 86, 85,43);

	// Les couleurs finales du dégradé
	m_Color_Light_EndDegrad[ROSE_KAKI]		=	RGB(249, 241, 241);
	m_Color_Medium_EndDegrad[ROSE_KAKI]		=	RGB(248, 224, 224);
	m_Color_Dark_EndDegrad[ROSE_KAKI]		=	RGB(231, 230, 195);
	
	m_Color_Select[ROSE_KAKI]				=	RGB(102,102,51);
	m_Color_Moyenne[ROSE_KAKI]				=	RGB(245,215, 10);

	// Rose gris
	m_Color_Light[ROSE_GRIS]				=	RGB(230,230,230);
	m_Color_Medium[ROSE_GRIS]				=	RGB(144,144,144);
	m_Color_Dark[ROSE_GRIS]					=	RGB(107,107,107);
	m_Color_SuperDark[ROSE_GRIS]			=   RGB( 45, 44, 44);

	// Les couleurs finales du dégradé
	m_Color_Light_EndDegrad[ROSE_GRIS]		=	RGB(249, 242, 242);
	m_Color_Medium_EndDegrad[ROSE_GRIS]		=	RGB(231, 231, 231);
	m_Color_Dark_EndDegrad[ROSE_GRIS]		=	RGB(224, 224, 224);
	
	m_Color_Select[ROSE_GRIS]				=	RGB(255,129,129);
	m_Color_Moyenne[ROSE_GRIS]				=	RGB(245,215, 10);

	// Sable Alain
	m_Color_Light[SABLE]					=	RGB(240,210,169);
	m_Color_Medium[SABLE]					=	RGB(240,206,124);
	m_Color_Dark[SABLE]						=	RGB(216,177,77);
	m_Color_SuperDark[SABLE]				=   RGB(136,111,49);

	// Les couleurs finales du dégradé
	m_Color_Light_EndDegrad[SABLE]			=	RGB(246, 240, 232);
	m_Color_Medium_EndDegrad[SABLE]			=	RGB(241, 233, 214);
	m_Color_Dark_EndDegrad[SABLE]			=	RGB(238, 229, 206);

	m_Color_Select[SABLE]					=   RGB(170,129,9);
	m_Color_Moyenne[SABLE]					=   RGB(197,151, 55);

	// Café/Chocolat Alain
	m_Color_Light[CAFE]						=	RGB(198,191,134);
	m_Color_Medium[CAFE]					=	RGB(179,153,77);
	m_Color_Dark[CAFE]						=	RGB(170,136,51);	
	m_Color_SuperDark[CAFE]					=   RGB(117, 94, 35);

	// Les couleurs finales du dégradé
	m_Color_Light_EndDegrad[CAFE]			=	RGB(229, 227, 213);
	m_Color_Medium_EndDegrad[CAFE]			=	RGB(232, 224, 198);
	m_Color_Dark_EndDegrad[CAFE]			=	RGB(248, 237, 210);

	m_Color_Select[CAFE]					=   RGB(153,102,0);
	m_Color_Moyenne[CAFE]					=   RGB(245,215, 10);


	// New Blue
	m_Color_Light[NEWBLUE]					=	RGB(227, 239, 255);
	m_Color_Medium[NEWBLUE]					=	RGB(186, 216, 255);
	m_Color_Dark[NEWBLUE]					=	RGB(159, 199, 249);	
	m_Color_SuperDark[NEWBLUE]				=   RGB(21, 66, 139);

	// Les couleurs finales du dégradé
	m_Color_Light_EndDegrad[NEWBLUE]		=	RGB(237, 241, 246);
	m_Color_Medium_EndDegrad[NEWBLUE]		=	RGB(239, 244, 251);
	m_Color_Dark_EndDegrad[NEWBLUE]			=	RGB(239, 245, 253);

	m_Color_Select[NEWBLUE]					=   RGB(101, 147, 207);
	m_Color_Moyenne[NEWBLUE]				=   RGB(185,215,255);

	///
	m_Color_White		= RGB(255,255,255);
	m_Color_Black		= RGB(0,0,0);
	m_Color_Red			= RGB(200,0,0);
	m_Color_Beige		= RGB(240,228,189);
	m_Color_GrisClair	= RGB(192,192,192);

	// Couleurs qui serviront à différencier texte dans une grille
	m_Color_Blue    = RGB(0,0,255);
	m_Color_Green   = RGB(0,128,0);
	m_Color_Brown   = RGB(128,128,0);   // 128, 128, 128 gris clair
	m_Color_Violet  = RGB(128,0,128);
    
	m_AppColor[COLORCREUSET]		= OCEAN; 
	m_AppColor[COLORREPITEMS]		= BOTTLE_GREEN; 

	// m_AppColor[COLORREPSUPPORTS]	= GREY_MOUSE; 
	m_AppColor[COLORREPSUPPORTS]	= CAFE; 

	m_AppColor[COLORANALYSEUR]		= LILAS; 
	m_AppColor[COLORCHOIXSUPPORTS]	= BLEU_PASTEL; 
	m_AppColor[COLORLANCEUR]		= NEWBLUE; 
	m_AppColor[COLORZOOM]			= BLEU_PASTEL; 

	m_Color_Graph[0]  = RGB(255,0,0);			//rouge
	m_Color_Graph[1]  = RGB(0,0,255);			//bleu
	m_Color_Graph[2]  = RGB(0,255,0);			//vert
	m_Color_Graph[3]  = RGB(0,255,255);			//cyan
	m_Color_Graph[4]  = RGB(245,215,10);		//jaune paille
	m_Color_Graph[5]  = RGB(0,0,125);			//bleu marine
	m_Color_Graph[6]  = RGB(255,0,255);			//magenta
	m_Color_Graph[7]  = RGB(125,0,0);			//marron
	m_Color_Graph[8]  = RGB(0,125,0);			//vert
    m_Color_Graph[9]  = RGB(125,125,0);			//caca d'oie
	m_Color_Graph[10] = RGB(125,0,125);			//violet
	m_Color_Graph[11] = RGB(0,125,125);			//bleu canard
	m_Color_Graph[12] = RGB(125,125,125);		//gris
}

CATPColors::~CATPColors(void)
{
	//WRITEPROFILEINT(SECTION_OPTIONS, KEY_COLOR_PERSO_LIGHT, m_Color_Light[PERSO]);
	//WRITEPROFILEINT(SECTION_OPTIONS, KEY_COLOR_PERSO_MEDIUM, m_Color_Medium[PERSO]);
	//WRITEPROFILEINT(SECTION_OPTIONS, KEY_COLOR_PERSO_DARK, m_Color_Dark[PERSO]);
	//WRITEPROFILEINT(SECTION_OPTIONS, KEY_COLOR_PERSO_SELECT, m_Color_Select[PERSO]);
}

const COLORREF & CATPColors::GetColorSuperDark(APPCOLOR app)
{ 
	return m_Colors.m_Color_SuperDark[m_Colors.m_AppColor[app]];
}

const COLORREF & CATPColors::GetColorSuperDark(CLR color)
{ 
	return m_Colors.m_Color_SuperDark[color];
}

const COLORREF & CATPColors::GetColorDark(APPCOLOR app)
{ 
	return m_Colors.m_Color_Dark[m_Colors.m_AppColor[app]];
}

const COLORREF & CATPColors::GetColorDark(CLR color)
{ 
	return m_Colors.m_Color_Dark[color];
}

const COLORREF & CATPColors::GetColorDark_End(APPCOLOR app)
{ 
	return m_Colors.m_Color_Dark_EndDegrad[m_Colors.m_AppColor[app]];
}

const COLORREF & CATPColors::GetColorDark_End(CLR color)
{ 
	return m_Colors.m_Color_Dark_EndDegrad[color];
}

const COLORREF & CATPColors::GetColorMedium(APPCOLOR app)		
{
	return m_Colors.m_Color_Medium[m_Colors.m_AppColor[app]];
}

const COLORREF & CATPColors::GetColorMedium(CLR color)
{ 
	return m_Colors.m_Color_Medium[color];
}

const COLORREF & CATPColors::GetColorMedium_End(APPCOLOR app)
{ 
	return m_Colors.m_Color_Medium_EndDegrad[m_Colors.m_AppColor[app]];
}

const COLORREF & CATPColors::GetColorMedium_End(CLR color)
{ 
	return m_Colors.m_Color_Medium_EndDegrad[color];
}

const COLORREF & CATPColors::GetColorLight(APPCOLOR app)		
{
	return m_Colors.m_Color_Light[m_Colors.m_AppColor[app]];
}

const COLORREF & CATPColors::GetColorLight(CLR color)
{ 
	return m_Colors.m_Color_Light[color];
}

const COLORREF & CATPColors::GetColorLight_End(APPCOLOR app)
{ 
	return m_Colors.m_Color_Light_EndDegrad[m_Colors.m_AppColor[app]];
}

const COLORREF & CATPColors::GetColorLight_End(CLR color)
{ 
	return m_Colors.m_Color_Light_EndDegrad[color];
}

////////////////////////////////////////////////////////////////////////////
// Les couleurs dégradées

// Recup les dégradés Color Dark
void CATPColors::GetColorDarkGraduation(APPCOLOR app, COLORREF clr_degrad[], int NbGrad)
{
	// Recup couleur début et fin d'origine
	COLORREF  ColorDep = m_Colors.m_Color_Dark[m_Colors.m_AppColor[app]];
	COLORREF  ColorFin = m_Colors.m_Color_Dark_EndDegrad[m_Colors.m_AppColor[app]];
		
	// Calcul des couleurs dégradées
	CalcCouleursDegrades(ColorDep, ColorFin,  clr_degrad, NbGrad);
}

void CATPColors::GetColorDarkGraduation(CLR color, COLORREF clr_degrad[], int NbGrad)
{
	// Recup couleur début et fin d'origine
	COLORREF  ColorDep = m_Colors.m_Color_Dark[color];
	COLORREF  ColorFin = m_Colors.m_Color_Dark_EndDegrad[color];

	// Calcul des couleurs dégradées
	CalcCouleursDegrades(ColorDep, ColorFin,  clr_degrad, NbGrad);
}

// Recup les dégradés Color Médium
void CATPColors::GetColorMediumGraduation(APPCOLOR app, COLORREF clr_degrad[], int NbGrad)
{
	// Recup couleur début et fin d'origine
	COLORREF  ColorDep = m_Colors.m_Color_Medium[m_Colors.m_AppColor[app]];
	COLORREF  ColorFin = m_Colors.m_Color_Medium_EndDegrad[m_Colors.m_AppColor[app]];

	// Calcul des couleurs dégradées
	CalcCouleursDegrades(ColorDep, ColorFin,  clr_degrad, NbGrad);
}

void CATPColors::GetColorMediumGraduation(CLR color, COLORREF clr_degrad[], int NbGrad)
{
	// Recup couleur début et fin d'origine
	COLORREF  ColorDep = m_Colors.m_Color_Medium[color];
	COLORREF  ColorFin = m_Colors.m_Color_Medium_EndDegrad[color];

	// Calcul des couleurs dégradées
	CalcCouleursDegrades(ColorDep, ColorFin,  clr_degrad, NbGrad);
}

// Recup les dégradés Color Light
void CATPColors::GetColorLightGraduation(APPCOLOR app, COLORREF clr_degrad[], int NbGrad)
{
	// Recup couleur début et fin d'origine
	COLORREF  ColorDep = m_Colors.m_Color_Light[m_Colors.m_AppColor[app]];
	COLORREF  ColorFin = m_Colors.m_Color_Light_EndDegrad[m_Colors.m_AppColor[app]];

	// Calcul des couleurs dégradées
	CalcCouleursDegrades(ColorDep, ColorFin,  clr_degrad, NbGrad);
}

void CATPColors::GetColorLightGraduation(CLR color, COLORREF clr_degrad[], int NbGrad)
{
	// Recup couleur début et fin d'origine
	COLORREF  ColorDep = m_Colors.m_Color_Light[color];
	COLORREF  ColorFin = m_Colors.m_Color_Light[color];

	// Calcul des couleurs dégradées
	CalcCouleursDegrades(ColorDep, ColorFin,  clr_degrad, NbGrad);
}

// Fill rectangle with graduate colors
void CATPColors::FillRectGraduate(CDC & dc, RECT & rect, COLORREF clr_degrad[], int NbGrad, STYLE_DEGRAD StyleDegrad)
{
	// Dimension du rectangle à redessinner
	int nWidth = rect.right - rect.left;
	int nHeight = rect.bottom - rect.top;
	CRect rectangle;

	if (StyleDegrad == STYLE_DEGRAD::FROM_MIDDLE_DEGRAD)
	{
		// Calcul milieu vertical de l'élément à dessiner
		int Milieu = nHeight / 2;

		// Dessin dégradé vers le haut
		int index_grad;
		for(int i = Milieu; i >= 0; i--)
		{
			rectangle.SetRect(rect.left, rect.top + i, rect.left + nWidth, rect.top + i - 1);
			CBrush brush;

			index_grad = (int)(((float)NbGrad / (float)Milieu) * (Milieu - i));

			if (index_grad >= NbGrad) index_grad = NbGrad-1;
			if (index_grad < 0) index_grad = 0;

			COLORREF CurColor = clr_degrad[index_grad];
			brush.CreateSolidBrush(CurColor);

			dc.FillRect(&rectangle, &brush);
		}

		// Dessin dégradé vers le bas
		for(int i = Milieu; i <= nHeight; i++)
		{
			rectangle.SetRect(rect.left, rect.top + i, rect.left + nWidth, rect.top + i + 1);
			CBrush brush;

			index_grad = (int)(((float)NbGrad / (float)Milieu) * (i-Milieu));

			if (index_grad >= NbGrad) index_grad = NbGrad-1;
			if (index_grad < 0) index_grad = 0;

			COLORREF CurColor = clr_degrad[index_grad];
			brush.CreateSolidBrush(CurColor);

			dc.FillRect(&rectangle, &brush);
		}
	}

	// Remplissage dégradé du haut vers le bas
	else if (StyleDegrad == STYLE_DEGRAD::FROM_TOP_DEGRAD)
	{
		int index_grad;
		for(int i = nHeight; i >= 0; i--)
		{
			rectangle.SetRect(rect.left, rect.top + i, rect.left + nWidth, rect.top + i - 1);	

			CBrush brush;

			index_grad = (int)(((float)NbGrad / (float)nHeight) * (nHeight-i));

			if (index_grad >= NbGrad) index_grad = NbGrad-1;
			if (index_grad < 0) index_grad = 0;

			COLORREF CurColor = clr_degrad[index_grad];
			brush.CreateSolidBrush(CurColor);

			dc.FillRect(&rectangle, &brush);
		}
	}
	// Remplissage dégradé du bas vers le haut
	else if (StyleDegrad == STYLE_DEGRAD::FROM_BOTTOM_DEGRAD)
	{
		int index_grad;
		for(int i = 0; i <= nHeight; i++)
		{
			rectangle.SetRect(rect.left, rect.top + i, rect.left + nWidth, rect.top + i + 1);
			CBrush brush;

			index_grad = (int)(((float)NbGrad / (float)nHeight) * i);

			if (index_grad >= NbGrad) index_grad = NbGrad-1;
			if (index_grad < 0) index_grad = 0;

			COLORREF CurColor = clr_degrad[index_grad];
			brush.CreateSolidBrush(CurColor);

			dc.FillRect(&rectangle, &brush);
		}

	}
}

void CATPColors::FillRectModeGraduate(CDC & dc, RECT & rect, APPCOLOR app, MODE_DEGRAD modedegrad)
{
	// A REMETTRE
	static COLORREF clr_degrad[m_NbGrad];
	// COLORREF clr_degrad[m_NbGrad];

	if (modedegrad == MODE_DEGRAD::LIGHT_DEGRAD)
		GetColorLightGraduation(app, clr_degrad, CATPColors::m_NbGrad);
	else if (modedegrad == MODE_DEGRAD::MEDIUM_DEGRAD)
		GetColorMediumGraduation(app, clr_degrad, CATPColors::m_NbGrad);
	else if (modedegrad == MODE_DEGRAD::DARK_DEGRAD)
		GetColorDarkGraduation(app, clr_degrad, CATPColors::m_NbGrad);
	else return;


	// Dimension du rectangle à redessinner
	int nWidth = rect.right - rect.left;
	int nHeight = rect.bottom - rect.top;
	CRect rectangle;

	// Remplissage dégradé du haut vers le bas
	int index_grad;
	for(int i = nHeight; i >= 0; i--)
	{
		rectangle.SetRect(rect.left, rect.top + i, rect.left + nWidth, rect.top + i - 1);	

		CBrush brush;

		index_grad = (int)(((float)m_NbGrad / (float)nHeight) * (nHeight-i));

		if (index_grad >= m_NbGrad) index_grad = m_NbGrad-1;
		if (index_grad < 0) index_grad = 0;

		COLORREF CurColor = clr_degrad[index_grad];
		brush.CreateSolidBrush(CurColor);

		dc.FillRect(&rectangle, &brush);
	}

	/*
	FillRectGraduate(dc, rect, clr_degrad, m_NbGrad, FROM_TOP_DEGRAD);
	*/
}


// Fill rectangle with graduate colors
void CATPColors::FillRectGraduateWithSurround(CDC & dc, RECT & rect, COLORREF clr_degrad[], int NbGrad, STYLE_DEGRAD StyleDegrad, APPCOLOR AppColor)
{
	// Remplissage du rectangle
	FillRectGraduate(dc, rect, clr_degrad, NbGrad, StyleDegrad);

	// Ajout Liseret autour
	CPen pen3Border		(PS_SOLID, 0, CATPColors::GetColorDark(AppColor));			// Border color

	// on dessine les bords haut et gauche en foncé
	dc.SelectObject(&pen3Border);
	dc.MoveTo(rect.left ,		rect.bottom-1);
	dc.LineTo(rect.left ,		rect.top);
	dc.LineTo(rect.right-1 ,	rect.top);

	// on dessine le bord bas en plus foncé
	CPen pen3DarkBorder		(PS_SOLID, 0, CATPColors::GetColorSuperDark(AppColor));	// Dark Border color
	dc.SelectObject(&pen3DarkBorder);
	dc.MoveTo(rect.right-1 , rect.top);
	dc.LineTo(rect.right-1 , rect.bottom-1);
	dc.LineTo(rect.left	,	 rect.bottom-1);
}	

// Fill rectangle with graduate colors
void CATPColors::FillRgnPolygoneGraduate(CDC & dc, CRgn & Rng, COLORREF clr_degrad[], int NbGrad, STYLE_DEGRAD StyleDegrad)
{
	// Dimension du rectangle à redessinner
	CRect RectOrg;
    Rng.GetRgnBox(&RectOrg);
	int nWidth = RectOrg.right - RectOrg.left;
	int nHeight = RectOrg.bottom - RectOrg.top;

	CRect rectangle;

	// Remplissage dégradé du haut vers le bas
	int index_grad;
	for(int i = nHeight; i >= 0; i--)
	{
		rectangle.SetRect(0, i, nWidth, i - 1);
		CBrush brush;

		index_grad = (int)(((float)NbGrad / (float)nHeight) * (nHeight-i));

		if (index_grad >= NbGrad) index_grad = NbGrad-1;
		if (index_grad < 0) index_grad = 0;

		COLORREF CurColor = clr_degrad[index_grad];
		brush.CreateSolidBrush(CurColor);
  	    dc.FillRect(&rectangle, &brush);
	}
}

///////////////////////////////////////////////////////////////////////////////////////
// Calcul des couleurs dégradées
void CATPColors::CalcCouleursDegrades(const COLORREF ColorDep, const COLORREF ColorFin,  COLORREF clr_degrad[], int NbGrad)
{
	// Nombre de dégradé
	BYTE RGB_Start[3];
	BYTE RGB_End[3];

	// Init Start Color
	RGB_Start[0] = GetRValue(ColorDep);
	RGB_Start[1] = GetGValue(ColorDep);
	RGB_Start[2] = GetBValue(ColorDep);

	// Init End Color
	RGB_End[0] = GetRValue(ColorFin);
	RGB_End[1] = GetGValue(ColorFin);
	RGB_End[2] = GetBValue(ColorFin);

	if (NbGrad > 0)
	{
		BYTE RGB_Result[3];

		for (int iGrad = 0; iGrad<= NbGrad; iGrad++)
		{
			for (int iRgb = 0; iRgb < 3; iRgb++)
			{
				RGB_Result[iRgb] = (BYTE)((int)RGB_Start[iRgb] + int((float(iGrad)/float(NbGrad)) * ((int)RGB_End[iRgb] - (int)RGB_Start[iRgb])));
			}

			// Stocke dans tab colors degradés
			COLORREF ColorGrad;
			ColorGrad = RGB(RGB_Result[0], RGB_Result[1], RGB_Result[2]);
			clr_degrad[iGrad] = ColorGrad;
		}
	}
}

const COLORREF & CATPColors::GetColorSelect(APPCOLOR app)		
{
	return m_Colors.m_Color_Select[m_Colors.m_AppColor[app]];
}

const COLORREF & CATPColors::GetColorSelect(CLR color)
{ 
	return m_Colors.m_Color_Select[color];
}

const COLORREF & CATPColors::GetColorMoyenne(APPCOLOR app)		
{
	return m_Colors.m_Color_Moyenne[m_Colors.m_AppColor[app]];
}

const COLORREF & CATPColors::GetColorMoyenne(CLR color)		
{
	return m_Colors.m_Color_Moyenne[color];
}




const COLORREF & CATPColors::GetColorWhite()				
{
	return m_Colors.m_Color_White;
}

const COLORREF & CATPColors::GetColorBlack()				
{
	return m_Colors.m_Color_Black;
}

const COLORREF & CATPColors::GetColorRed()				
{
	return m_Colors.m_Color_Red;
}

const COLORREF & CATPColors::GetColorBeige()				
{
	return m_Colors.m_Color_Beige;
}

const COLORREF & CATPColors::GetColorGrisClair()				
{
	return m_Colors.m_Color_GrisClair;
}

////////////////////////////////////////////////////////////////////////////
// Définition couleur texte pour différencier les résultats
const COLORREF & CATPColors::GetColorResult1()				
{
	return m_Colors.m_Color_Black; 
}

const COLORREF & CATPColors::GetColorResult2()				
{
	return m_Colors.m_Color_Blue;
}

const COLORREF & CATPColors::GetColorResult3()				
{
	return m_Colors.m_Color_Green;
}

const COLORREF & CATPColors::GetColorResult4()				
{
	return m_Colors.m_Color_Brown;
}

int	CATPColors::GetAppColor(APPCOLOR app)		
{
	return  m_Colors.m_AppColor[app];
}

bool CATPColors::SetAppColor(APPCOLOR app, CLR color)
{ 
	m_Colors.m_AppColor[app] = color;
	return true;
}

bool CATPColors::SetAppColor(APPCOLOR app, int color)
{
	if(color < 0 || color >= MAX_COLORS)
		return false;
	m_Colors.m_AppColor[app] = color;
	return true;
}

LONG	CATPColors::GetFontSizeS()
{
	return ( 80);
}

LONG	CATPColors::GetFontSizeM()	
{
	return ( 90);
}

LONG	CATPColors::GetFontSizeL()
{
	return (100);
}

LONG	CATPColors::GetFontSizeX()
{
	return (110);
}

const TCHAR* CATPColors::GetFontName()
{
	return (_T("MS Shell Dlg"));
}

const TCHAR* CATPColors::GetFontStaticName()
{
	return (_T("MS Shell Dlg"));
}

const TCHAR* CATPColors::GetFontLabelName()
{
//	return (_T("Comic sans MS"));
	return (_T("MS Shell Dlg"));
}

int	CATPColors::GetFontStaticSize()
{
	return 8;
}

int	CATPColors::GetFontLabelSize()
{
	return 10;
}

int	CATPColors::GetFontBigLabelSize()
{
	return 12;
}

const COLORREF & CATPColors::GetColorGrph(int num)
{
	return m_Colors.m_Color_Graph[num];
}

