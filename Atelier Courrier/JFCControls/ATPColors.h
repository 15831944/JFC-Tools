/*
//	JFC Informatique et Média
//	2004
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CATPColors
//
//	Descriptif:
//	Contient les définitions des couleurs des applications
*/

#pragma once

class AFX_EXT_CLASS CATPColors
{
public:

	// New Colors Sept 2007
	enum CLR
	{
		BOTTLE_GREEN		=	0,
		EMERAUDE			=	1,
		BLEU_PASTEL			=	2,
		OCEAN				=   3,
		LILAS				=   4,
		SAHARA				=	5,
		TERRE_SIENNE		=   6,
		RUBIS				=   7,
		GREY_MOUSE			=	8,
		ROSE				=	9,
		ROSE_KAKI			=  10,
		ROSE_GRIS			=  11,
		SABLE				=  12,
		CAFE				=  13,
		NEWBLUE				=  14,
		MAX_COLORS			=  15,
	};

	// Les applis possibles
	/*
	enum APPCOLOR 
	{
		COLORNOAPP			= 0,
		COLORCREUSET		= 1,
		COLORREPITEMS		= 2,
		COLORREPSUPPORTS	= 3,
		COLORANALYSEUR		= 4,
		COLORCHOIXSUPPORTS	= 5,
		COLORLANCEUR		= 6,
		COLORZOOM			= 7,
		COLORMAX_APPS		= 8
	};
	*/

	enum APPCOLOR 
	{
		COLORCREUSET		= 0,
		COLORREPITEMS		= 1,
		COLORREPSUPPORTS	= 2,
		COLORANALYSEUR		= 3,
		COLORCHOIXSUPPORTS	= 4,
		COLORLANCEUR		= 5,
		COLORZOOM			= 6,
		COLORNOAPP			= 7,
		COLORMAX_APPS		= 8
	};

	// Mode Light / Medium / Dark pour les dégradés
	enum MODE_DEGRAD{LIGHT_DEGRAD = 0, MEDIUM_DEGRAD, DARK_DEGRAD};

	// Style de dégradés (du milieu vers les bords, de haut en bas, de bas en haut)
	enum STYLE_DEGRAD{FROM_MIDDLE_DEGRAD = 0, FROM_TOP_DEGRAD, FROM_BOTTOM_DEGRAD, NO_DEGRAD};

	// Nombre de strates pour les dégradés
	static const int m_NbGrad = 15;

	static const COLORREF & GetColorGrph(int num);

	// Recup Color Dark Application ou Type de couleur
	static const COLORREF & GetColorSuperDark(APPCOLOR app);
	static const COLORREF & GetColorSuperDark(CLR color);

	// Recup Color Dark Application ou Type de couleur
	static const COLORREF & GetColorDark(APPCOLOR app);
	static const COLORREF & GetColorDark(CLR color);

	// Recup couleur light fin graduation
	static const COLORREF & GetColorDark_End(APPCOLOR app);
	static const COLORREF & GetColorDark_End(CLR color);

	// Recup les dégradés Color Dark
	static void GetColorDarkGraduation(APPCOLOR app, COLORREF clr_degrad[], int NbGrad);
	static void GetColorDarkGraduation(CLR color, COLORREF clr_degrad[] , int NbGrad);

	// Recup Color Dark Application ou Type de couleur
	static const COLORREF & GetColorMedium(APPCOLOR app);
	static const COLORREF & GetColorMedium(CLR color);

	// Recup couleur light fin graduation
	static const COLORREF & GetColorMedium_End(APPCOLOR app);
	static const COLORREF & GetColorMedium_End(CLR color);

	// Recup les dégradés Color Médium
	static void GetColorMediumGraduation(APPCOLOR app, COLORREF clr_degrad[], int NbGrad);
	static void GetColorMediumGraduation(CLR color, COLORREF clr_degrad[], int NbGrad);

	// Recup Color Dark Application ou Type de couleur
	static const COLORREF & GetColorLight(APPCOLOR app);
	static const COLORREF & GetColorLight(CLR color);

	// Recup couleur light fin graduation
	static const COLORREF & GetColorLight_End(APPCOLOR app);
	static const COLORREF & GetColorLight_End(CLR color);

	// Recup les dégradés Color Light
	static void GetColorLightGraduation(APPCOLOR app, COLORREF clr_degrad[], int NbGrad);
	static void GetColorLightGraduation(CLR color, COLORREF clr_degrad[], int NbGrad);

	// Fill rectangle with graduate colors
	static void FillRectGraduate(CDC & dc, RECT & rect, COLORREF clr_degrad[], int NbGrad, STYLE_DEGRAD StyleDegrad = FROM_MIDDLE_DEGRAD);
	static void FillRgnPolygoneGraduate(CDC & dc, CRgn & Rng, COLORREF clr_degrad[], int NbGrad, STYLE_DEGRAD StyleDegrad = FROM_MIDDLE_DEGRAD);
	static void FillRectGraduateWithSurround(CDC & dc, RECT & rect, COLORREF clr_degrad[], int NbGrad, STYLE_DEGRAD StyleDegrad, APPCOLOR AppColor);

	static void FillRectModeGraduate(CDC & dc, RECT & rect, APPCOLOR app, MODE_DEGRAD modedegrad);

	static const COLORREF & GetColorSelect(APPCOLOR app);
	static const COLORREF & GetColorSelect(CLR color);
	static const COLORREF & GetColorMoyenne(APPCOLOR app);
	static const COLORREF & GetColorMoyenne(CLR color);
	static const COLORREF & GetColorWhite();
	static const COLORREF & GetColorBlack();
	static const COLORREF & GetColorRed();
	static const COLORREF & GetColorBeige();
	static const COLORREF & GetColorGrisClair();
	static int				GetAppColor(APPCOLOR app);
	static bool				SetAppColor(APPCOLOR app, CLR color);
	static bool				SetAppColor(APPCOLOR app, int color);

	// Définition de couleur texte pour type de résultat différent
	static const COLORREF & GetColorResult1();
	static const COLORREF & GetColorResult2();
	static const COLORREF & GetColorResult3();
	static const COLORREF & GetColorResult4();

	// Font Name
	static const TCHAR	*	GetFontName();

	// les fonctions pour récupérer les tailles de la fonte
	static LONG				GetFontSizeS();
	static LONG				GetFontSizeM();
	static LONG				GetFontSizeL();
	static LONG				GetFontSizeX();

	static const TCHAR*		GetFontStaticName();
	static const TCHAR*		GetFontLabelName();
	static int				GetFontStaticSize();
	static int				GetFontLabelSize();
	static int				GetFontBigLabelSize();

private:
	COLORREF	m_Color_Select[MAX_COLORS];

	// Tab couleur de base pour chaque niveau dark, medium, light
	COLORREF	m_Color_SuperDark[MAX_COLORS];
	COLORREF	m_Color_Dark[MAX_COLORS];
	COLORREF	m_Color_Medium[MAX_COLORS];
	COLORREF	m_Color_Light[MAX_COLORS];

	// Tab couleur de base fin dégradé pour chaque niveau dark, medium, light
	COLORREF	m_Color_Dark_EndDegrad[MAX_COLORS];
	COLORREF	m_Color_Medium_EndDegrad[MAX_COLORS];
	COLORREF	m_Color_Light_EndDegrad[MAX_COLORS];

	// Calcul des couleurs dégradées
	static void CalcCouleursDegrades(const COLORREF ColorDep, const COLORREF ColorFin,  COLORREF clr_degrad[], int NbGrad);

	COLORREF	m_Color_Moyenne[MAX_COLORS];
	COLORREF	m_Color_White;
	COLORREF	m_Color_Black;
	COLORREF	m_Color_Red;
	COLORREF	m_Color_Beige;
	COLORREF	m_Color_GrisClair;

	COLORREF    m_Color_Blue;  
	COLORREF    m_Color_Green; 
	COLORREF    m_Color_Brown; 
	COLORREF    m_Color_Violet;
    
	
	int			m_AppColor[COLORMAX_APPS];

	COLORREF	m_Color_Graph[12];
	
private:
	CATPColors(void);
	virtual ~CATPColors(void);
	static CATPColors	m_Colors;
};
