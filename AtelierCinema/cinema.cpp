// cinema.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "cinema.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "ChildFilm.h"
#include "cinemaDoc.h"
#include "cinemaView.h"
#include "FilmDoc.h"
#include "FilmView.h"
#include "Splash.h"
#include "BaseCinema.h"
#include "JFCFileException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCinemaApp

BEGIN_MESSAGE_MAP(CCinemaApp, CWinApp)
	//{{AFX_MSG_MAP(CCinemaApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)

	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCinemaApp construction

CCinemaApp::CCinemaApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	NbIndiv=0;

}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCinemaApp object

CCinemaApp theApp;
CString GCheminAccesDonnee;
/////////////////////////////////////////////////////////////////////////////
// CCinemaApp initialization

#include <direct.h>
#include <locale.h>
BOOL CCinemaApp::InitInstance()
{
	// CG: The following block was added by the Splash Screen component.
\
	{
\
		CCommandLineInfo cmdInfo;
\
		ParseCommandLine(cmdInfo);
\

\
		CSplashWnd::EnableSplashScreen(cmdInfo.m_bShowSplash);
\
	}

	// Pour les virgules, les mois...	
	setlocale(LC_ALL,"French");

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

// MODIF AURELIEN : TEST UGRID
	AfxEnableControlContainer();

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("JFC"));

	/*
	double **m_Test;
	m_Test=new double*[2];
	m_Test[0] = new double[2];
	m_Test[1] = new double[2];

	m_Test[0][0] = 0;
	m_Test[0][1] = 1;
	m_Test[1][0] = 10;
	m_Test[1][1] = 11;
	*/

	
	// dans le cas ou l'on ne trouve rien...
	char tampon[1000];
	CString DefaultPath,Opti;
	_getcwd(tampon,1000);
	DefaultPath=tampon;
	m_Chemin=GetProfileString("Configuration", "Chemin", DefaultPath);
	WriteProfileString("Configuration", "Chemin",m_Chemin);
	GCheminAccesDonnee=m_Chemin;

	m_flagOptimisation=0;
	Opti=GetProfileString("Configuration", "Optimisation", "NON");
	if(Opti=="OUI")
		m_flagOptimisation=1;
	else 
		WriteProfileString("Configuration", "Optimisation","NON");
	m_PostID=GetProfileString("Configuration", "PosteID", "x.x.");
	WriteProfileString("Configuration", "PosteID",m_PostID);

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)


	InitColours();
	CreateBrushes();
	CreatePens();
	CreateFonts();

	hCurMove=LoadCursor(IDC_PASSAGE);
	hCurAjoutSupp=LoadCursor(IDC_AJOUTSUPP);
	hCurGlissement=LoadCursor(IDC_GLISSEMENT);
	hCurBlocTempo=LoadCursor(IDC_BLOCTEMPO);
	hCurBlocEfface=LoadCursor(IDC_BLOCEFFACE);
	hCurBlocChange=LoadCursor(IDC_BLOCCHANGE);
	hCurBlocMove=LoadCursor(IDC_BLOCMOVE);
	hCurInterdit=LoadCursor(IDC_INTERDIT);
	hCurBlocDeplacement=LoadCursor(IDC_BLOCDEPLACEMENT);
	hCurBlocCopierColler=LoadCursor(IDC_BLOCCOPIERCOLLER);
	hCurWait=LoadStandardCursor(IDC_WAIT);
	hIcoFleche=LoadIcon(IDI_FLECHE);
	hIcoFleche2=LoadIcon(IDI_FLECHE2);
	hBmpTom=LoadBitmap(m_hInstance,MAKEINTRESOURCE(IDB_TOM));

	m_Cible.Init();
	if(!LoadSource())
	{
		return(0);
	}

	m_LibMois.Add("Janvier");
	m_LibMois.Add("Février");
	m_LibMois.Add("Mars");
	m_LibMois.Add("Avril");
	m_LibMois.Add("Mai");
	m_LibMois.Add("Juin");
	m_LibMois.Add("Juillet");
	m_LibMois.Add("Août");
	m_LibMois.Add("Septembre");
	m_LibMois.Add("Octobre");
	m_LibMois.Add("Novembre");
	m_LibMois.Add("Décembre");


	// Lecture du nom de la vague en cours
	CStdioFile NomVague;
	CString chemin;
		
	CJFCFileException e;
	CString nom;

	// Par défaut
	m_NomVagueAtelierCinema = "75000 Janvier Décembre 2001";

	nom.Format("%s\\SOURCE\\NomVague.txt",theApp.m_Chemin);

	// Chargement du fichier
	if( !NomVague.Open(nom, CFile::modeRead|CFile::typeText|CFile::shareDenyWrite, &e ) )   
	{
		CString msg;
		msg.Format("Impossible d'ouvrir %s : ",nom);
		e.AfficheErreur(msg);
	}
	else
	{
		// lecture du label
		NomVague.ReadString(m_NomVagueAtelierCinema);
	}

		
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	// Document et view Cinéma
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_CINEMATYPE,
		RUNTIME_CLASS(CCinemaDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CCinemaView));
	AddDocTemplate(pDocTemplate);

	// Alain : Modification selon client (Campagne avec ou pas Campagne Films
	int OptionCampagneFilm;
	OptionCampagneFilm = atoi(GetProfileString("Configuration", "OptionCampagneFilm", "1"));
	
	if (OptionCampagneFilm > 0)
	{
		// Document et view Film
		CMultiDocTemplate* pDocTemplate2;
		pDocTemplate2 = new CMultiDocTemplate(
			IDR_FILMTYPE,
			RUNTIME_CLASS(CFilmDoc),
			RUNTIME_CLASS(CChildFilm), // custom MDI child frame
			RUNTIME_CLASS(CFilmView));
		AddDocTemplate(pDocTemplate2);
	}

	// Initialisation du grid
	GXInit();
	GXSetNewGridLineMode(TRUE);
	
	
	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;



	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	cmdInfo.m_nShellCommand=CCommandLineInfo::FileNothing;

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it.
	pMainFrame->CenterWindow();
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CCinemaApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CCinemaApp message handlers


// fichier de construction des objets GDI

void CCinemaApp::CreateFonts(void)
{
	LOGFONT		Font;

	memset(&Font,0,sizeof(LOGFONT));
	// creation du font courier default system size
	Font.lfWeight 		= FW_NORMAL;
	Font.lfItalic 		= FALSE;
	Font.lfUnderline 	= FALSE;
	Font.lfStrikeOut 	= FALSE;
	Font.lfHeight 		= (long)+15;/*(int)(.5+GetSystemMetrics(SM_CYSIZE)*5.0/6.0)*/;
	Font.lfWidth 		= (BYTE)0;
	Font.lfCharSet 		= ANSI_CHARSET;
	Font.lfPitchAndFamily = (BYTE)(FIXED_PITCH | FF_MODERN);
	strcpy(Font.lfFaceName,"Courier New");
	hFont[FNT_COURIERD]= CreateFontIndirect(&Font);


/*			CurrentFont.lfCharSet = ANSI_CHARSET;
			CurrentFont.lfWeight = FW_BOLD;//NORMAL;
			CurrentFont.lfItalic = FALSE;
			CurrentFont.lfUnderline = FALSE;
			CurrentFont.lfStrikeOut = FALSE;
//			CurrentFont.lfHeight = (int)(.5+GetSystemMetrics(SM_CYSIZE)*5.0/6.0);
			CurrentFont.lfHeight = 8;
			CurrentFont.lfWidth = (BYTE)0;
			CurrentFont.lfPitchAndFamily = (BYTE)(VARIABLE_PITCH | FF_SWISS);
			strcpy(CurrentFont.lfFaceName,"Helv");//etica");
			HelvFont = CreateFontIndirect(&CurrentFont);

		Btn1Font=CreateFont(-8, 0, 0, 0,FW_BOLD, // specify pt size
			0, 0, 0, 0, 0, 0, 0,(BYTE)(VARIABLE_PITCH | FF_SWISS),"Helv");         // and face name only
		if(Btn1Font==NULL)AfficheErreur("Erreur font !");



*/

/*
	// creation du font helv 8
	Font.lfCharSet 		= ANSI_CHARSET;
	Font.lfWeight 		= FW_BOLD;//NORMAL;
	Font.lfItalic 		= FALSE;
	Font.lfUnderline 	= FALSE;
	Font.lfStrikeOut 	= FALSE;
	Font.lfHeight 		= -8;
	Font.lfWidth 		= (BYTE)0;
	Font.lfPitchAndFamily = (BYTE)(VARIABLE_PITCH | FF_SWISS);
	strcpy(Font.lfFaceName,"Helv");
	hFont[FNT_HELV8] = CreateFontIndirect(&Font);
*/
	hFont[FNT_HELV8]=CreateFont(-8, 0, 0, 0,FW_BOLD, // specify pt size
		0, 0, 0, 0, 0, 0, 0,(BYTE)(VARIABLE_PITCH | FF_SWISS),"Helv");         // and face name only


    Font.lfHeight=-9;
    Font.lfWidth=0;
    Font.lfEscapement=0;
    Font.lfOrientation=0;
    Font.lfWeight=400|FW_BOLD;
    Font.lfItalic=0;
    Font.lfUnderline=0;
    Font.lfStrikeOut=0;
    Font.lfCharSet=0;
    Font.lfOutPrecision=1;
    Font.lfClipPrecision=2;
    Font.lfQuality=1;
    Font.lfPitchAndFamily=34;
    strcpy(Font.lfFaceName,"Small Fonts");
	hFont[FNT_SMALL7B] = CreateFontIndirect(&Font);

	// creation du font Small 6
    Font.lfHeight=-9;
    Font.lfWidth=0;
    Font.lfEscapement=0;
    Font.lfOrientation=0;
    Font.lfWeight=400;
    Font.lfItalic=0;
    Font.lfUnderline=0;
    Font.lfStrikeOut=0;
    Font.lfCharSet=0;
    Font.lfOutPrecision=1;
    Font.lfClipPrecision=2;
    Font.lfQuality=1;
    Font.lfPitchAndFamily=34;
    strcpy(Font.lfFaceName,"Small Fonts");
	hFont[FNT_SMALL7] = CreateFontIndirect(&Font);

	// creation du font Small 6
    Font.lfHeight=-8;
    Font.lfWidth=0;
    Font.lfEscapement=0;
    Font.lfOrientation=0;
    Font.lfWeight=400;
    Font.lfItalic=0;
    Font.lfUnderline=0;
    Font.lfStrikeOut=0;
    Font.lfCharSet=0;
    Font.lfOutPrecision=1;
    Font.lfClipPrecision=2;
    Font.lfQuality=1;
    Font.lfPitchAndFamily=34;
    strcpy(Font.lfFaceName,"Small Fonts");
	hFont[FNT_SMALL6] = CreateFontIndirect(&Font);

    Font.lfHeight=-11;
    Font.lfWidth=0;
    Font.lfEscapement=0;
    Font.lfOrientation=0;
    Font.lfWeight=400;
    Font.lfItalic=0;
    Font.lfUnderline=0;
    Font.lfStrikeOut=0;
    Font.lfCharSet=0;
    Font.lfOutPrecision=3;
    Font.lfClipPrecision=2;
    Font.lfQuality=1;
    Font.lfPitchAndFamily=49;
    strcpy(Font.lfFaceName,"Courier New");
	hFont[FNT_COURIERN8]= CreateFontIndirect(&Font);
}

void CCinemaApp::DeleteFonts(void)
{
	int		Font=0;
	for(Font=0;Font<NB_FONTS;Font++) DeleteObject(hFont[Font]);
}

COLORREF CCinemaApp::GetRGB(char * Defcolors,char * NomColor)
{
	int /*j,*/coul[3]/*,n*/;
/*	tamponalloc[0]=0;
	GetPrivateProfileString(_INI_Couleurs,NomColor,Defcolors,tamponalloc,180,_INI_ETUDE);
	for (j=0; j<3; j++) coul[j]=255;
	n=strlen(tamponalloc);
	if(!n)strcpy(tamponalloc,Defcolors);
	for (j=0; j<n; j++) if (tamponalloc[j]==',') tamponalloc[j]=' ';*/
	sscanf(/*tamponalloc*/Defcolors,"%d,%d,%d",coul,coul+1,coul+2);
	return(RGB((BYTE)coul[0],(BYTE)coul[1],(BYTE)coul[2]));
}


void CCinemaApp::InitColours(void)
{
	// les couleurs fixes
	rgb[CLR_NOIR]       = RGB(0,0,0);
	rgb[CLR_BLANC]      = RGB(255,255,255);
	rgb[CLR_GRISF]      = RGB(128,128,128);
	rgb[CLR_GRISC]      = RGB(192,192,192);
	rgb[CLR_GRISCF]     = RGB(0,128,128);
	rgb[CLR_ROUGE]      = RGB(0,102,153);		//RGB(255,0,0);
	rgb[CLR_VERT]       = RGB_BleuJFC;			//RGB(0,128,0);
	rgb[CLR_BLEUE]      = RGB(0,0,255);
	rgb[CLR_JAUNE]      = RGB(217,218,255);		//RGB(255,255,0);
	rgb[CLR_MDI]        = GetSysColor(COLOR_APPWORKSPACE);
	rgb[CLR_PAILLE]     = RGB(217,218,255);		//RGB(255,255,192);
	rgb[CLR_VERTPAILLE] = RGB(192,255,192);

	// les couleurs paramétrables
	/*
	rgb[CLR_SELECTRESU]=GetRGB("255,255,0","Paysage_SelectionDansRESU"); //"192,192,0" pour marron clair
	rgb[CLR_RESUINACTIF]=GetRGB("240,240,240","Paysage_CaseInvalide");
	rgb[CLR_BARRE]=GetRGB("0,128,0","Paysage_BarrePleine");
	rgb[CLR_BARRESELECT]=GetRGB("128,128,0","Paysage_BarrePleineSelect");*/

	/*
	rgb[CLR_FONDPASSAGE]=GetRGB("255,255,0","Creuset_FondPassage");
	rgb[CLR_FONDPRESENT]=GetRGB("255,128,0","Creuset_FondPresent");
	rgb[CLR_BORDPASSAGE]=GetRGB("128,128,0","Creuset_BordPassage");
	rgb[CLR_FONDPASSENDPL]=GetRGB("0,0,128","Creuset_FondPassadeEnDpl");
	*/
	rgb[CLR_FONDPASSAGE]   = RGB_BleuClairJFC;
	rgb[CLR_FONDPRESENT]   = RGB_BleuClairJFC;
	rgb[CLR_BORDPASSAGE]   = RGB_BleuClairJFC;
	rgb[CLR_FONDPASSENDPL] = RGB_BleuClairJFC;

	/*
	rgb[CLR_PRODUITFIGE]=RGB(94,174,255);
	*/

	rgb[CLR_PRODUITFIGE]   = RGB_BleuJFC;

	/*
	rgb[CLR_FONDGRILLEINACTIF]=GetRGB("224,224,224","Creuset_FondGrilleInactif");
	rgb[CLR_FONDDLGGRAPH]=GetRGB("238,213,30","Creuset_FondDlgGraph");
	*/
	
	rgb[CLR_FONDGRILLEINACTIF]	=	GetRGB("224,224,224","Creuset_FondGrilleInactif"); //RGB_BleuClairJFC;
	rgb[CLR_FONDDLGGRAPH]		=	RGB_BleuJFC;

	rgb[CLR_FONDHYP1]=GetRGB("192,255,192","Creuset_FondHypotheseActive1");
	rgb[CLR_FONDHYP2]=GetRGB("255,192,192","Creuset_FondHypotheseActive2");
	rgb[CLR_FONDHYP3]=GetRGB("192,255,255","Creuset_FondHypotheseActive3");
	rgb[CLR_FONDHYP4]=GetRGB("255,255,192","Creuset_FondHypotheseActive4");
	rgb[CLR_FONDHYP5]=GetRGB("255,192,255","Creuset_FondHypotheseActive5");
	rgb[CLR_HYPOTHESE1]=GetRGB("0,128,0","Creuset_TraitHypothese1");
	rgb[CLR_HYPOTHESE2]=GetRGB("255,0,0","Creuset_TraitHypothese2");
	rgb[CLR_HYPOTHESE3]=GetRGB("0,0,255","Creuset_TraitHypothese3");
	rgb[CLR_HYPOTHESE4]=GetRGB("128,128,64","Creuset_TraitHypothese4");
	rgb[CLR_HYPOTHESE5]=GetRGB("255,0,255","Creuset_TraitHypothese5");


	/*
	rgb[CLR_FONDHYP1]			=	RGB_BleuJFC;
	rgb[CLR_FONDHYP2]			=	RGB_BleuJFC;
	rgb[CLR_FONDHYP3]			=	RGB_BleuJFC;
	rgb[CLR_FONDHYP4]			=	RGB_BleuJFC;
	rgb[CLR_FONDHYP5]			=	RGB_BleuJFC;
	rgb[CLR_HYPOTHESE1]			=	RGB_BleuJFC;
	rgb[CLR_HYPOTHESE2]			=	RGB_BleuJFC;
	rgb[CLR_HYPOTHESE3]			=	RGB_BleuJFC;
	rgb[CLR_HYPOTHESE4]			=	RGB_BleuJFC;
	rgb[CLR_HYPOTHESE5]			=	RGB_BleuJFC;
	*/

	rgb[CLR_FONDDLGPERF]=GetRGB("0,159,0","Creuset_FondDlgPerf");
	/*
	rgb[CLR_FONDSELECTBINOME]=GetRGB("128,255,128","Creuset_FondSelectBinome");
	rgb[CLR_FONDSELECTCALEND]=GetRGB("128,255,128","Creuset_FondSelectCalend");
	rgb[CLR_TRAITSELECTCALEND]=GetRGB("0,128,0","Creuset_TraitSelectCalend");
	rgb[CLR_TRAITNRSEMAINEX]=GetRGB("0,0,255","Creuset_TraitNrSemaineX");
	*/

	/*
	rgb[CLR_FONDDLGPERF]		=	RGB_BleuJFC;
	*/

	rgb[CLR_FONDSELECTBINOME]	=	RGB_BleuSpeJFC;
	rgb[CLR_FONDSELECTCALEND]	=	RGB_BleuSpeJFC;
	rgb[CLR_TRAITSELECTCALEND]	=	RGB_BleuJFC;
	rgb[CLR_TRAITNRSEMAINEX]	=	RGB_BleuJFC;

	/*
	rgb[CLR_FONDNIVEAU1]=GetRGB("255,192,130","Fond niveau 1");
	rgb[CLR_FONDBOITEOFFRE]=GetRGB("207,146,41","FondBoitesOffre");

	*/
	rgb[CLR_CELLULEGRISEE]=GetRGB("255,224,224","Creuset_CelluleInactive");

	rgb[CLR_FONDNIVEAU1]		=	RGB_BleuJFC;
	rgb[CLR_FONDBOITEOFFRE]		=	RGB_BleuJFC;

	/* A REMETTRE

	rgb[CLR_CELLULEGRISEE]		=	RGB_BleuJFC;
	*/


/*
	// les pinceaux des fonds de dialogues du moteur
	hBrsh[BRSH_FONDGRILLE]=GetBrsh("130,65,0",_INI_FondGrille);
	hBrsh[BRSH_AVANTGRILLE]=GetBrsh("255,128,51",_INI_AvantGrille);
	hBrsh[BRSH_FONDRESU]=GetBrsh("0,159,0",_INI_FondResu);
	hBrsh[BRSH_FONDRESSTA]=GetBrsh("238,213,30",_INI_FondResSta);
	hBrsh[BRSH_FONDPOIDS]=GetBrsh("130,65,0",_INI_FondPoids);
	hBrsh[BRSH_FONDOFFRE]=GetBrsh("0,159,0",_INI_FondOffre);
	hBrsh[BRSH_FONDECHELLE]=GetBrsh("0,111,0",_INI_FondEchelle);
	hBrsh[BRSH_FONDNIV1]=GetBrsh("",_INI_Boite1); // peut etre null!
	hBrsh[BRSH_FONDNIV2]=GetBrsh("",_INI_Boite2); // peut etre null!

*/
}

void CCinemaApp::CreateBrushes(void)
{
	int		Color=0;
	for(Color=0;Color<NB_COULEURS;Color++) hBrush[Color]=CreateSolidBrush(rgb[Color]);
}

void CCinemaApp::DeleteBrushes(void)
{
	int		Color=0;
	for(Color=0;Color<NB_COULEURS;Color++) DeleteObject(hBrush[Color]);
}

void CCinemaApp::CreatePens(void)
{
	int		Color=0;
	for(Color=0;Color<NB_COULEURS;Color++) hPen[Color]=CreatePen(PS_SOLID,1,rgb[Color]);
	hPen2Noir=CreatePen(PS_SOLID,2,RGB(0,0,0));
}

void CCinemaApp::DeletePens(void)
{
	int		Color=0;
	for(Color=0;Color<NB_COULEURS;Color++) DeleteObject(hPen[Color]);
	DeleteObject(hPen2Noir);
}


int CCinemaApp::ExitInstance() 
{
	UnloadSource();

	DeletePens();
	DeleteBrushes();
	DeleteFonts();

	DeleteObject(hCurMove);
	DeleteObject(hCurAjoutSupp);
	DeleteObject(hCurGlissement);
	DeleteObject(hCurBlocTempo);
	DeleteObject(hCurBlocEfface);
	DeleteObject(hCurBlocChange);
	DeleteObject(hCurBlocMove);
	DeleteObject(hCurInterdit);
	DeleteObject(hCurBlocDeplacement);
	DeleteObject(hCurBlocCopierColler);
	DestroyIcon(hIcoFleche);
	DestroyIcon(hIcoFleche2);
	DeleteObject(hBmpTom);

	return CWinApp::ExitInstance();
}





bool CCinemaApp::LoadSource()
{
	ASSERT(AfxCheckMemory());

	// chargement des tables
	if(!m_TableCinema.LoadTable(0))
	{
		return(0);
	}

	ASSERT(AfxCheckMemory());

	
	CStdioFile f;
	CString txt;
	txt=theApp.m_Chemin+"\\SOURCE\\vague.sou";
	if(!f.Open(txt,CFile::modeRead | CFile::typeText))
	{
		CString err;
		err.Format("Impossible d'ouvrir le fichier %s",txt);
		AfxMessageBox(err);
		//return 0;
	}

	f.ReadString(txt);
	NbIndiv=atoi(txt);
	f.Close();

	ChargerPoids();
	LoadProba();
	CalculCoeffCorrectionCNC();

	ASSERT(AfxCheckMemory());

	return(1);
}


// Test à ne faire qu'une fois
#define TESTMATRICE

BOOL CCinemaApp::LoadProba()
{
	FILE			*fp_aud;
	BOOLEAN			B00000Existe = FALSE;

	// proba d'être touché 0 et une fois
	m_PT0=new double [PROBABILISATION+1];
	m_PT1=new double [PROBABILISATION+1];
	for(int i=0;i<=PROBABILISATION;i++){
		m_PT1[i]=(double)i/(double)PROBABILISATION;
		m_PT0[i]=(double)(PROBABILISATION-i)/(double)PROBABILISATION;
	}

	// Construction de la matrice binomiale
	m_MPP=new double**[PROBABILISATION+1];
	m_NbMPP=new BYTE*[PROBABILISATION+1];
	for(int p=0;p<PROBABILISATION+1;p++){
		m_MPP[p]=new double*[PROBABILISATION+1];
		for(int z=0;z<PROBABILISATION+1;z++)m_MPP[p][z]=NULL;
			
		m_NbMPP[p]=new BYTE[PROBABILISATION+1];
	}

	CString txt;
	txt=theApp.m_Chemin+"\\SOURCE\\VAGUE.AUD";


	//////////////////////////////////////////////////// GROSSE VERRUE /////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Lecture Bidouiile pour détecter B00000
	if((fp_aud=fopen(txt,"rb"))==NULL) return(FALSE);
	DWORD nb;
	fread(&nb,sizeof(DWORD),1,fp_aud);
	fread(&m_NbBacProbabilise,sizeof(DWORD),1,fp_aud);
	for(int ii=0;ii<m_NbBacProbabilise;ii++)
	{
		CString txt;
		WORD w;
		DWORD dw;
		const int taillecode=5;
		char code[taillecode+1];
		
		// ERREUR CODE BAC ALAIN !!!!!!
		fread(code,sizeof(char),taillecode,fp_aud);
		
		
		code[taillecode]=0; // null terminated string
		txt.Format("B%s",code);

		// ???????
		if (txt == "B00000") 
		{
			
			B00000Existe = TRUE;
			break;
		}
		else
		{

			fread(&nb,sizeof(DWORD),1,fp_aud);
			for(int y=0;y<nb;y++)
			{
				// lecture des nr d'individus
				fread(&dw,sizeof(DWORD),1,fp_aud);

				fread(&w,sizeof(WORD),1,fp_aud);
				
				fread(&w,sizeof(WORD),1,fp_aud);

				fread(&w,sizeof(WORD),1,fp_aud);
			}
		}
	}
	fclose(fp_aud);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	if((fp_aud=fopen(txt,"rb"))==NULL) return(FALSE);

	DWORD nbindivinfile;
	fread(&nbindivinfile,sizeof(DWORD),1,fp_aud);

	if(NbIndiv!=nbindivinfile)
	{
		AfxMessageBox("Erreur dans les fichiers: le fichier .AUD ne dispose pas d'un nombre d'individus correcte !");
		return(0);
	}

	fread(&m_NbBacProbabilise,sizeof(DWORD),1,fp_aud);
	m_CodeBacProbabilise.SetSize(m_NbBacProbabilise);

	// TODO FRED
	m_NbBacProbabilise--;


	// ??????? Bidouille voir si Code B00000 existe auquel cas ne pas tenir compte en refaisant --
	if (B00000Existe == TRUE)
		m_NbBacProbabilise--;


#ifdef _DEBUG
	int NrBacTest=-1;
//	char BACTEST[]="B01004";
	char BACTEST[]="B75999";
//	char BACTEST[]="B75060";
#endif
	// MODIF FRED: 16/05/2002 BAC REGIE
	m_NbIndivSupp	= new DWORD [m_NbBacProbabilise];
	m_NrIndivSupp	= new DWORD*[m_NbBacProbabilise];
	m_Proba1		= new BYTE* [m_NbBacProbabilise];
	m_Proba2		= new BYTE* [m_NbBacProbabilise];
	m_Proba30		= new BYTE* [m_NbBacProbabilise];
	m_NrIndivDirect	= new DWORD[NbIndiv];
	m_NrBacParIndiv	= new DWORD[NbIndiv];

	m_MapIndexBac.RemoveAll();
	for(int iBac=0;iBac<m_NbBacProbabilise;iBac++)
	{
		CString txt;
		WORD w;
		DWORD dw;
		const int taillecode=5;
		
		char code[taillecode+1];
		
		// ERREUR CODE BAC ALAIN !!!!!!
		fread(code,sizeof(char),taillecode,fp_aud);
		
		//CString code;
		//code = theApp.m_TableCinema.GetTableBac()->GetCode(iBac);
		//int Nb = theApp.m_TableCinema.GetTableBac()->GetSize();
		//CString code = theApp.m_TableCinema.GetTableBac()->GetCode(iBac);
		
		code[taillecode]=0; // null terminated string
		txt.Format("B%s",code);

		// ???????
		if (txt == "B00000") 
		{
			

			fread(&dw,sizeof(DWORD),1,fp_aud);
			m_NbIndivSupp[iBac]=dw;
			for(int y=0;y<m_NbIndivSupp[iBac];y++)
			{
				// lecture des nr d'individus
				fread(&dw,sizeof(DWORD),1,fp_aud);

				fread(&w,sizeof(WORD),1,fp_aud);
				
				fread(&w,sizeof(WORD),1,fp_aud);

				fread(&w,sizeof(WORD),1,fp_aud);

			}

			iBac--;

			continue;
		}

		// ERREUR CODE BAC ALAIN !!!!!!
		// MODIF FRED : 16/05/2002
		int IndexBacReel = iBac;
		int idxregieA = m_NbBacProbabilise+IndexBacReel;
		int idxregieM = m_NbBacProbabilise*2+IndexBacReel;
		int idxregieN = m_NbBacProbabilise*3+IndexBacReel;


		m_CodeBacProbabilise.SetAt(iBac,txt);
		m_MapIndexBac.SetAt(txt,IndexBacReel);

		// Ajout des codes de bacs régie au map
		m_MapIndexBac.SetAt(txt+"A",idxregieA);
		m_MapIndexBac.SetAt(txt+"M",idxregieM);
		m_MapIndexBac.SetAt(txt+"N",idxregieN);



//		ASSERT(txt!=BACTEST);
#ifdef _DEBUG
		if(txt==BACTEST)NrBacTest=iBac;
#endif
		// MODIF FRED : 16/05/2002
		// lecture du nb d'individu
		fread(&dw,sizeof(DWORD),1,fp_aud);

		m_NbIndivSupp[iBac]=dw;
		/*m_NbIndivSupp[idxregieA]=dw;
		m_NbIndivSupp[idxregieM]=dw;*/

		m_NrIndivSupp[iBac]= new DWORD[m_NbIndivSupp[iBac]];
		/*m_NrIndivSupp[idxregieA]= new DWORD[m_NbIndivSupp[idxregieA]];
		m_NrIndivSupp[idxregieM]= new DWORD[m_NbIndivSupp[idxregieM]];*/

		m_Proba1[iBac]=new BYTE[m_NbIndivSupp[iBac]];
		m_Proba2[iBac]=new BYTE[m_NbIndivSupp[iBac]];
		m_Proba30[iBac]=new BYTE[m_NbIndivSupp[iBac]];

		for(int y=0;y<m_NbIndivSupp[iBac];y++)
		{
			// lecture des nr d'individus
			fread(&dw,sizeof(DWORD),1,fp_aud);
			m_NrIndivSupp[iBac][y]=dw;
			/*m_NrIndivSupp[idxregieA][y]=dw;
			m_NrIndivSupp[idxregieM][y]=dw;*/


			fread(&w,sizeof(WORD),1,fp_aud);
			m_Proba1[iBac][y]=w; 

			fread(&w,sizeof(WORD),1,fp_aud);

			m_Proba2[iBac][y]=w;

			fread(&w,sizeof(WORD),1,fp_aud);

			m_Proba30[iBac][y]=w;


			// Test Nb Indiv
			ASSERT(m_NrIndivSupp[iBac][y]<NbIndiv);

			ASSERT(m_Proba1[iBac][y]<=PROBABILISATION);
			ASSERT(m_Proba2[iBac][y]<=PROBABILISATION*9);
			ASSERT(m_Proba2[iBac][y]>=m_Proba1[iBac][y]);


		}

/*	
	
	  // transformation des probas par support en proba par parc

  
	// Ce bloc utilisait des variables qui étaient déclarées plus haut.
	WORD NbIndivSupp;
	DWORD *NrIndivSupp;// par support, par individus(>64K)
	WORD *Proba1;// par support, par individus(>64K)
	WORD *Proba2;// par support, par individus(>64K)
	// ------------------------------------------------------------------

	
	  
		
		  for(int p=0;p<TabBinome[xx].NbParc;p++){
			int Parc=NrDirectParc[xx][p];
			m_NbIndivSupp[Parc]=NbIndivSupp/2;
			if(p==0)m_NbIndivSupp[Parc]+=NbIndivSupp%2;
			m_NrIndivSupp[Parc]=new DWORD[m_NbIndivSupp[Parc]];
			m_Proba1[Parc]=new WORD[m_NbIndivSupp[Parc]];
			m_Proba2[Parc]=new WORD[m_NbIndivSupp[Parc]];
		}

		for(int ind=0;ind<NbIndivSupp;ind++){
			p=ind%2;
			int indp=ind/2;
			int Parc=NrDirectParc[xx][p];

			m_NrIndivSupp[Parc][indp]=NrIndivSupp[ind];
			m_Proba1[Parc][indp]=Proba1[ind];
			m_Proba2[Parc][indp]=Proba2[ind];
		}

		delete [] Proba2;
		delete [] Proba1;
		delete [] NrIndivSupp;*/
	}

	fclose(fp_aud);
	ASSERT(AfxCheckMemory());

/*
#ifdef _DEBUG
	if(NrBacTest>=0)
	{
		double NbIndividusTouche=0;
		double NbContacts=0;
		double Asymptote=0;
		for(i=0;i<m_NbIndivSupp[NrBacTest];i++)
		{
			int NrI=m_NrIndivSupp[NrBacTest][i];
			NbIndividusTouche+=m_Proba1[NrBacTest][i]*CoeffEx[NrI];
			NbContacts+=m_Proba2[NrBacTest][i]*CoeffEx[NrI];
			Asymptote+=CoeffEx[NrI];
			TRACE("Individu %d Poids %.0f Proba %d Cumul %.0f\n",NrI,CoeffEx[NrI],m_Proba2[NrBacTest][i],NbContacts);
		}
		NbIndividusTouche/=PROBABILISATION;
		NbContacts/=PROBABILISATION;

		TRACE("Résultat sur le BAC %s:\n",BACTEST);
		TRACE("Index du bac = %d\n",NrBacTest);
		TRACE("Nb interview = %d\n",m_NbIndivSupp[NrBacTest]);
		TRACE("Nb individus touchés = %.0f\n",NbIndividusTouche);
		TRACE("Nb contacts = %.0f\n",NbContacts);
		TRACE("Asymptote = %.0f\n",Asymptote);
	}
	else
		TRACE("Le BAC de test %s n'est pas trouvé\n",BACTEST);

#endif
*/
	

	
// Accès aux bac depuis l'individu
	long dlong=-1L;
	for(int y=0;y<NbIndiv;y++)
	{
		m_NrIndivDirect[y]=dlong;
		m_NrBacParIndiv[y]=dlong;
	}

	bool flagerreurfichier=0;

	// MODIF FRED : 16/05/2002
	for(iBac=0;iBac<m_NbBacProbabilise;iBac++)
	{
/*		DWORD * pDirect=m_NrIndivDirect[iBac];
		DWORD * pIndiv=m_NrIndivSupp[iBac];
		WORD * pProba1=m_Proba1[iBac];
		WORD * pProba2=m_Proba2[iBac];
*/		
		// acces au individus direct depuis BAC
		for(y=0;y<m_NbIndivSupp[iBac];y++)
		{
			int NrI=m_NrIndivSupp[iBac][y];
			if((m_NrIndivDirect[NrI]!=-1) || (m_NrBacParIndiv[NrI]!=-1))
				flagerreurfichier=1;
			else
			{
				m_NrIndivDirect[NrI]=y;
				m_NrBacParIndiv[NrI]=iBac;
			}
		}

/*		// construction de la matrice binomiale des probas
		for(y=0;y<m_NbIndivSupp[x];y++){
			WORD p1=m_Proba1[x][y];
			WORD p2=m_Proba2[x][y];

		// C'était avant la version 3.1.
		// Maintemant, à cause de la saisonalité, il faut une matrice pleine!
*/
		WORD p1,p2;
		for(p1=1;p1<=PROBABILISATION;p1++)for(p2=p1;p2<=PROBABILISATION;p2++)
		{
			// la matrice binomiale
			if(m_MPP[p1][p2]==NULL){
				// un test de cohérence des probas
				// à faire qu'une fois TEST
				#ifdef TESTMATRICE
				if(p1>p2)
				{
					ASSERT(0);
					AfxMessageBox("BP: proba1> proba2");
				}
				#endif

				m_MPP[p1][p2]=new double [1+NBUBINOMEMAX];
				if(p1==p2){
					m_MPP[p1][p2][0]=m_PT0[p1];
					m_MPP[p1][p2][1]=m_PT1[p1];
					m_NbMPP[p1][p2]=1;
				}
				else {

					// La bonne méthode doit venir d'un nouveau fichier:
					// Maintenant que nous probabilisons le fichier il faut calculer lors
					// de la probabilisation la probabilité d'être touché 1 fois, 2 fois, 3 fois.
					// De plus il est toujours necessaire de donner p' afin de calculer correctement
					// les contacts.

					// En attendant on procède avec un nouvea modèle accpetant une plurifréquentation
					//  limitée à 2.


					// La probabilité de ne pas être touché
					m_MPP[p1][p2][0]=m_PT0[p1];

					// La probabilité d'être touché une fois
					m_MPP[p1][p2][1]=m_PT1[p1];

					// Les deux fois touchés:
					double deuxfoistouche=m_PT1[p2]-m_PT1[p1];

					// Qui est limité à ceux que l'on peu toucher
					if (deuxfoistouche>m_PT1[p1])deuxfoistouche=m_PT1[p1];

					// Qui sont donc touché 2 fois:
					m_MPP[p1][p2][2]=deuxfoistouche;

					// Qui ne sont donc plus dans les touchés une fois:
					m_MPP[p1][p2][1]-=deuxfoistouche;

					// Et c'est une vecteur avec une chance d'être touché 2 fois
					m_NbMPP[p1][p2]=2;

						
						
					/*
					if(p1==36 && p2==80)ASSERT(0); BUG
					// les proba expo
					m_MPP[p1][p2][0]=m_PT0[p1];
					double prob=+1.0;
					double cumul=+0.0;
					for(int u=1;u<=NBUBINOMEMAX;u++){
						prob*=m_PT1[p1];
						m_MPP[p1][p2][u]=prob;
						cumul+=prob;
					}

					// le rabotage
					u=NBUBINOMEMAX;
					m_NbMPP[p1][p2]=u;
					cumul-=m_PT1[p2];
					while(cumul>0.0){
						if(cumul>=m_MPP[p1][p2][u]){
							cumul-=m_MPP[p1][p2][u];
							m_MPP[p1][p2][u]=+0.0;
							m_NbMPP[p1][p2]=u-1;
						}
						else {
							m_MPP[p1][p2][u]-=cumul;
							cumul=+0.0;
							m_NbMPP[p1][p2]=u;
							break;
						}
						u--;
					}

					// transformation en individus ???? pas sur ????
					// à priori ça dépend de p2>1 ou non...
					//for(u=1;u<=NBUBINOMEMAX;u++)MPP[p1][p2][u]/=(double)u;

					// transformation en exactement touché
					m_MPP[p1][p2][2]=(m_PT1[p2]-m_PT1[p1]);
					for(u=1;u<NBUBINOMEMAX;u++)m_MPP[p1][p2][u]-=m_MPP[p1][p2][u+1];
					*/
				}



				// vérification de NBUBINOMEMAX
				// à faire qu'une fois
				#ifdef TESTMATRICE
				double cumul=+0.0;
				for(int u=0;u<=m_NbMPP[p1][p2];u++)cumul+=m_MPP[p1][p2][u];
				if(cumul<0.99999999 || cumul>1.0000001){
					// à faire sous débuger
					CString txt;
					txt.Format("Cumul de p1=%d p2=%d !=1",p1,p2);
					AfxMessageBox(txt);
				}
				#endif
			}
		}
	}

	if(flagerreurfichier)
	{
		AfxMessageBox("Une erreur a été détectée au moment de la lecture du fichier des audiences:\nAu moins un individu est utilisé dans plusieurs BACs");
	}

	return(0); //pas d'erreur
}

void CCinemaApp::LibereProba()
{
	int x,p1,p2;


	if(m_NbIndivSupp==NULL)return;
	for(x=0;x<m_NbBacProbabilise;x++)
	{
		delete [] m_Proba30[x];
		delete [] m_Proba2[x];
		delete [] m_Proba1[x];
		delete [] m_NrIndivSupp[x];
	}
	delete [] m_Proba30;
	delete [] m_Proba2;
	delete [] m_Proba1;
	delete [] m_NrIndivSupp;
	delete [] m_NrIndivDirect;
	delete [] m_NrBacParIndiv;
	delete [] m_NbIndivSupp;

	delete [] m_PT0;
	delete [] m_PT1;

	for(p1=0;p1<=PROBABILISATION;p1++){
		for(p2=0;p2<=PROBABILISATION;p2++){
			if(m_MPP[p1][p2]!=NULL)delete [] m_MPP[p1][p2];
		}
		delete [] m_MPP[p1];
		delete [] m_NbMPP[p1];
	}
	delete [] m_MPP;
	delete [] m_NbMPP;

	delete [] m_CoeffCorrectionCNC;

	m_NbIndivSupp=NULL;
}


BOOL CCinemaApp::ChargerPoids()
{
	CStdioFile f;
	CString txt;
	txt=theApp.m_Chemin+"\\SOURCE\\vague.pds";
	if(!f.Open(txt,CFile::modeRead | CFile::typeBinary))
	{
		AfxMessageBox("Erreur: Fichier poids (.PDS) introuvable");
		return(FALSE);
	}

	CoeffEx = new double[NbIndiv];
	f.Read(CoeffEx,NbIndiv*sizeof(double));
	f.Close();
	return(TRUE);
}


void CCinemaApp::UnloadSource()
{

	delete [] CoeffEx;
	LibereProba();
}





int CCinemaApp::GetSaisons(int NrBinome, int Annee, int NrSemaine)
{
	int retour=-1;

	// récupération de la table des saisons
	POSITION pos;
	CDocTemplate * docTemplate = theApp.GetNextDocTemplate(pos); 
	CCinemaDoc  * doc = (CCinemaDoc *)docTemplate->GetNextDoc(pos);
	CCampagne * C =&doc->m_Campagne;
	retour = C->m_Saison.GetSaisonnalite(NrSemaine-1); 
	return(retour);
}


BOOL CCinemaApp::PreTranslateMessage(MSG* pMsg)
{
	// CG: The following lines were added by the Splash Screen component.
	if (CSplashWnd::PreTranslateAppMessage(pMsg))
		return TRUE;

	return CWinApp::PreTranslateMessage(pMsg);
}


/*void CCinemaApp::ImportSaisons(LPCSTR nomfic)
{
	CStdioFile f;
	CString txt,txt2;

	if(!f.Open(nomfic,CFile::modeRead | CFile::typeText))
	{
		CString err;
		err.Format("Erreur à l'ouverture de : %s",nomfic);
		AfxMessageBox(err);
		return;
	}


	// La première ligne n'est pas utilisée
	f.ReadString(txt);


	CArray<CSaisons,CSaisons&>newSaisons;

	while(f.ReadString(txt))
	{
		CSaisons S;
		// L'année
		int pos=txt.Find('\t');
		txt2=txt.Left(pos);
		txt=txt.Mid(pos+1);
		S.m_Annee=atoi(txt2);

		// Le numéro de binôme
		pos=txt.Find('\t');
		txt2=txt.Left(pos);
		txt=txt.Mid(pos+1);
		S.m_NrBinome=atoi(txt2);

		// Le libellé (notused)
		pos=txt.Find('\t');
		txt2=txt.Left(pos);
		txt=txt.Mid(pos+1);

		// Les 53 semaines
		for(int s=0;s<53;s++)
		{
			pos=txt.Find('\t');
			if(pos>0)
			{
				txt2=txt.Left(pos);
				txt=txt.Mid(pos+1);
			}
			else txt2=txt;
			double d=atof(txt2)*100.0;
			DWORD v=(DWORD)(d+0.1);
			S.m_Coeff.Add(v);
		}

		newSaisons.Add(S);
	}
	
	f.Close();


	// Maintenant que le fichier est terminé, on insère
	// les nouveaux tarifs dans ceux existants
	int nb1=0;
	int nb2=0;
	for(int t=0;t<newSaisons.GetSize();t++)
	{
		for(int tb=0;tb<m_Saisons.GetSize();tb++)
		{
			if(newSaisons[t].m_Annee==m_Saisons[tb].m_Annee 
			&& newSaisons[t].m_NrBinome==m_Saisons[tb].m_NrBinome)
			{
				m_Saisons[tb].m_Coeff.Copy(newSaisons[t].m_Coeff);
				nb2++;
				break;
			}
		}
		if(tb>=m_Saisons.GetSize())
		{
			m_Saisons.Add(newSaisons[t]);
			nb1++;
		}
	}

	// Ecriture du fichier
	txt=theApp.m_Chemin+"\\DATA\\saisons.tbl";
	if(!f.Open(txt,CFile::modeWrite | CFile::modeCreate | CFile::typeText))
	{
		CString err;
		err="Erreur à l'ouverture de : "+txt;
		AfxMessageBox(err);
		return;
	}

	
	txt.Format("*%d\r\n",m_Saisons.GetSize());
	f.WriteString(txt);
	f.WriteString("#\r\n");
	for(t=0;t<m_Saisons.GetSize();t++)
	{
		txt.Format("%10d%10d\r\n",m_Saisons[t].m_Annee,m_Saisons[t].m_NrBinome);
		f.WriteString(txt);
		txt="";
		for(int w=0;w<55;w++)
		{
			int p=0;
			if(w<m_Saisons[t].m_Coeff.GetSize())p=m_Saisons[t].m_Coeff[w];
			txt2.Format("%10d ",p);
			txt+=txt2;
		}
		txt+="\r\n";
		f.WriteString(txt);
	}
	f.Close();

	txt.Format("Nb saisonalités lues : %d\nNb saisonalités nouvelles : %d\nNb saisonalités actualisées : %d",newSaisons.GetSize(),nb1,nb2);
	AfxMessageBox(txt);
}
*/

void CCinemaApp::CalculCoeffCorrectionCNC()
{
	// Calcul des coefficients de correction pour aligner la donnée des BAC sur la donnée CNC
	bool BacNontrouve=0;

	m_CoeffCorrectionCNC = new double [m_NbBacProbabilise];

	for(int b=0;b<m_NbBacProbabilise;b++)
	{
		if(m_CodeBacProbabilise[b].Left(3)=="B99")
		{
			m_CoeffCorrectionCNC[b]=1.0;
			continue;
		}
//	ASSERT(b!=173);

		// calcul des contacts distribués par ce BAC depuis les probabilités
		double NbCtsProba=+0.0;
		for(int i=0;i<m_NbIndivSupp[b];i++)
		{
			int NrI=m_NrIndivSupp[b][i];
			NbCtsProba+=(double)m_Proba2[b][i]*CoeffEx[NrI]/PROBABILISATION;
		}

		// Calcul des contacts distribués par ce BAC depuis les entrées CNC
		int idxBacFichier=m_TableCinema.GetTableBac()->FindIndexFromCode(m_CodeBacProbabilise[b]);
		if(idxBacFichier<0)
		{
			BacNontrouve=1;
			m_CoeffCorrectionCNC[b]=1.0;
			continue;
		}

		double NbEntreeCNC=m_TableCinema.GetTableBac()->GetAt(idxBacFichier).m_NbEntree;
//		NbEntreeCNC*=12.0/52.0;
//		NbCtsProba/=1000;

		if(NbCtsProba>0)m_CoeffCorrectionCNC[b]=NbEntreeCNC/NbCtsProba;
		else m_CoeffCorrectionCNC[b]=1; // 0 ?

		// TRACE("Code BAC: %s\t\tEntrée CNC: %.0f\t\tCorrection=%.2f\n",m_CodeBacProbabilise[b],NbEntreeCNC,m_CoeffCorrectionCNC[b]);
	}

	if(BacNontrouve)
	{
//		ASSERT(0);
//		AfxMessageBox("Parce-que tous les BAC dans le fichier des probabilités n'ont pas été retrouvés dans la donnée CNC tous ne seront pas corrigés par cette donnée.");
	}
	

}

CCampagne * CCinemaApp::GetCampagne()
{
	POSITION pos = theApp.GetFirstDocTemplatePosition();			
	CDocTemplate* DocTemplate = theApp.GetNextDocTemplate(pos);				
	pos = DocTemplate->GetFirstDocPosition(); 				
	CCinemaDoc *  Doc = (CCinemaDoc *)DocTemplate->GetNextDoc(pos);				
	return(&Doc->m_Campagne);				

}
