// DlgRepCible.cpp : implementation file
//

#include "stdafx.h"
#include "cinema.h"
extern CCinemaApp theApp;
#include "DlgRepCible.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define NB_CASAVERTISSEMENT 300
#define NB_CASINTERDIT 80



/////////////////////////////////////////////////////////////////////////////
// CDlgRepCible dialog


CDlgRepCible::CDlgRepCible(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRepCible::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgRepCible)
	m_PctUniCib = _T("");
	m_NbCasCib = 0;
	m_PctUniSeg = _T("");
	m_NbCasSeg = 0;
	m_Titre = _T("Segment");
	m_PopPhyCib = _T("");
	m_PopPonCib = _T("");
	m_PopPhySeg = _T("");
	m_PopPonSeg = _T("");
	//}}AFX_DATA_INIT
	m_NrCritActif=0;
	NrSegmentActif=0;
	SEGMENT S;
	Segment.Add(S);


	fMemorise=1; // 1 si mémorisée
	NumCible=-1;
	Nom="";

	m_hIcon=theApp.LoadIcon(IDI_CIBLE);
}


void CDlgRepCible::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgRepCible)
	DDX_Control(pDX, IDC_EDIT1, m_Edit1);
	DDX_Control(pDX, IDC_CADRE5, m_Cadre3);
	DDX_Control(pDX, IDC_CADRE4, m_Cadre2);
	DDX_Control(pDX, IDC_CADRE1, m_Cadre1);
	DDX_Control(pDX, IDC_LIST3, m_List3);
	DDX_Control(pDX, IDC_LIST2, m_List2);
	DDX_Control(pDX, IDC_LIST1, m_List1);
	DDX_Text(pDX, IDC_EDIT3, m_PctUniCib);
	DDX_Text(pDX, IDC_EDIT4, m_NbCasCib);
	DDX_Text(pDX, IDC_EDIT7, m_PctUniSeg);
	DDX_Text(pDX, IDC_EDIT8, m_NbCasSeg);
	DDX_Text(pDX, IDC_TITRESEGMENT, m_Titre);
	DDX_Text(pDX, IDC_EDIT1, m_PopPhyCib);
	DDX_Text(pDX, IDC_EDIT2, m_PopPonCib);
	DDX_Text(pDX, IDC_EDIT5, m_PopPhySeg);
	DDX_Text(pDX, IDC_EDIT6, m_PopPonSeg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgRepCible, CDialog)
	//{{AFX_MSG_MAP(CDlgRepCible)
	ON_COMMAND(ID_CIBLE_FERMER, OnCibleFermer)
	ON_LBN_SELCHANGE(IDC_LIST2, OnSelchangeList2)
	ON_LBN_SELCHANGE(IDC_LIST3, OnSelchangeList3)
	ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeList1)
	ON_BN_CLICKED(IDC_BUTTON1, OnSegmentMoins)
	ON_BN_CLICKED(IDC_BUTTON2, OnSegmentPlus)
	ON_BN_CLICKED(IDC_BUTTON3, OnButtonEffacer)
	ON_COMMAND(ID_CIBLE_NOUVELLE, OnCibleNouvelle)
	ON_COMMAND(ID_CIBLE_CHARGER, OnCibleCharger)
	ON_COMMAND(ID_CIBLE_MMORISER, OnCibleMmoriser)
	ON_COMMAND(ID_CIBLE_RENOMER, OnCibleRenomer)
	ON_COMMAND(ID_CIBLE_SUPPRIMER, OnCibleSupprimer)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_LBN_DBLCLK(IDC_LIST2, OnDblclkList2)
	ON_LBN_DBLCLK(IDC_LIST3, OnDblclkList3)
	ON_COMMAND(ID_CIBLE_MEMOSOUS, OnCibleMemoriserSous)
	ON_UPDATE_COMMAND_UI(ID_CIBLE_MEMOSOUS, OnUpdateCibleMemoriserSous)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgRepCible message handlers

void CDlgRepCible::OnCibleFermer() 
{
	if(IsMemoOK())EndDialog(IDOK);	
}

BOOL CDlgRepCible::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

	m_List3.Initialise(UNISTATE,ETENDUE_JFC,ETENDUE_JFC,RGB(0,200,0));
	for(int c=0;c<theApp.m_Cible.m_NbCritere;c++)
	{
		m_List2.AddString(theApp.m_Cible.m_LibCritere[c]);
	}
	m_List2.SetCurSel(m_NrCritActif);

	UpdateClasses();

	SetIcon(m_hIcon,0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgRepCible::UpdateClasses()
{
	m_List3.ResetContent();
	if(m_NrCritActif<0)return;
	for(int c=0;c<theApp.m_Cible.m_NbClasse[m_NrCritActif];c++)
	{
		m_List3.AddString(theApp.m_Cible.m_LibClasse[m_NrCritActif][c]);
	}

	// Il faut voir si ce critère existe dans la définition:
	for(c=0;c<Segment[NrSegmentActif].Critere.GetSize();c++)
	{
		if(Segment[NrSegmentActif].Critere[c].NrCritere==m_NrCritActif)
		{
			// Il faut actualiser la sélection avec 
			for(int x=0;x<theApp.m_Cible.m_NbClasse[m_NrCritActif];x++)
			{
				m_List3.SetItemData(x,Segment[NrSegmentActif].Critere[c].Poids[x]==0?0:1);
			}
			break;
		}
	}
}


void CDlgRepCible::OnSelchangeList2() 
{
	m_NrCritActif=m_List2.GetCurSel();
	// Si ce critère est présent dans list1 
	// il faut le sélectionner
	UpdateClasses();
	UpdateList1();
}

void CDlgRepCible::OnSelchangeList3() 
{
	bool flagvalide=0;
	// Ce critère est il valide ?
	for(int c=0;c<theApp.m_Cible.m_NbClasse[m_NrCritActif];c++)
	{
		if(m_List3.GetItemData(c))
		{
			flagvalide=1;
			break;
		}
	}

	if(flagvalide)
	{
		// On boucle sur les critères de ce segment
		// pour retrouver le critère
		for(int c=0;c<Segment[NrSegmentActif].Critere.GetSize();c++)
		{
			if(Segment[NrSegmentActif].Critere[c].NrCritere==m_NrCritActif)break;
		}
		if(c>=Segment[NrSegmentActif].Critere.GetSize())
		{
			CRITERE C;
			C.NrCritere=m_NrCritActif;
			C.Poids.SetSize(theApp.m_Cible.m_NbClasse[m_NrCritActif]);
			c=Segment[NrSegmentActif].Critere.Add(C);
		}

		// On met à jour les poids de ce critère
		for(int p=0;p<theApp.m_Cible.m_NbClasse[m_NrCritActif];p++)
		{
			if(m_List3.GetItemData(p))
			{
				Segment[NrSegmentActif].Critere[c].Poids[p]=100;
			}
			else
			{
				Segment[NrSegmentActif].Critere[c].Poids[p]=0;
			}
		}
	}
	else 
	{
		// il faut éliminer ce critère
		for(int c=0;c<Segment[NrSegmentActif].Critere.GetSize();c++)
		{
			if(Segment[NrSegmentActif].Critere[c].NrCritere==m_NrCritActif)
			{
				Segment[NrSegmentActif].Critere.RemoveAt(c);
			}
		}
	}

	UpdateList1();
	theApp.m_Cible.CalculCible(Segment);
	AfficheResultatCible();
	fMemorise=0;
}

void CDlgRepCible::UpdateList1()
{
	int sel=-1;
	m_List1.ResetContent();
	for(int c=0;c<Segment[NrSegmentActif].Critere.GetSize();c++)
	{
		m_List1.AddString(theApp.m_Cible.m_LibCritere[Segment[NrSegmentActif].Critere[c].NrCritere]);
		if(Segment[NrSegmentActif].Critere[c].NrCritere==m_NrCritActif)
		{
			sel=c;
		}
	}

	m_List1.SetCurSel(sel);
}


void CDlgRepCible::AfficheResultatCible()
{
	m_PopPhyCib.Format("%.0f",theApp.m_Cible.m_PopPhy);
	m_PopPonCib.Format("%.0f",theApp.m_Cible.m_PopPon);
	m_PctUniCib.Format("%.1f",theApp.m_Cible.m_PctUni);
	m_NbCasCib=theApp.m_Cible.m_NbCas;

	m_PopPhySeg.Format("%.0f",Segment[NrSegmentActif].PopPhy);
	m_PopPonSeg.Format("%.0f",Segment[NrSegmentActif].PopPon);
	m_PctUniSeg.Format("%.1f",Segment[NrSegmentActif].PctUni);
	m_NbCasSeg=Segment[NrSegmentActif].NbCas;

	m_Titre.Format("Segment %d",NrSegmentActif+1);

	UpdateData(0);
}

void CDlgRepCible::OnSelchangeList1() 
{
	int sel=m_List1.GetCurSel();
	if(sel<0)return;
	m_NrCritActif=Segment[NrSegmentActif].Critere[sel].NrCritere;
	m_List2.SetCurSel(m_NrCritActif);
	UpdateClasses();
}

void CDlgRepCible::OnSegmentMoins() 
{
	if(NrSegmentActif<=0)return;
	// le segment est il vide ?
	if(Segment[NrSegmentActif].Critere.GetSize()<=0)
	{
		// Alors on le vire
		Segment.RemoveAt(NrSegmentActif);
	}
	NrSegmentActif--;

	UpdateSegment();
}



void CDlgRepCible::OnSegmentPlus() 
{
	// le segment est il vide ?
	if(Segment[NrSegmentActif].Critere.GetSize()<=0)return;
	NrSegmentActif++;
	if(NrSegmentActif>=Segment.GetSize())
	{
		SEGMENT S;
		NrSegmentActif=Segment.Add(S);
	}
	UpdateSegment();
}

void CDlgRepCible::UpdateSegment() 
{
	if(Segment[NrSegmentActif].Critere.GetSize()>0)
	{
		m_NrCritActif=Segment[NrSegmentActif].Critere[0].NrCritere;
	}
	else
	{
		m_NrCritActif=0;
	}
	UpdateList1();
	m_List2.SetCurSel(m_NrCritActif);
	UpdateClasses();
	theApp.m_Cible.CalculCible(Segment);
	AfficheResultatCible();
}


void CDlgRepCible::OnButtonEffacer() 
{
	Segment.RemoveAt(NrSegmentActif);
	if(NrSegmentActif>=Segment.GetSize())
	{
		NrSegmentActif--;
		if(NrSegmentActif<0)
		{
			SEGMENT S;
			NrSegmentActif=Segment.Add(S);
		}
	}
	UpdateSegment();
}

void CDlgRepCible::OnCibleNouvelle() 
{
	if(!IsMemoOK())return;

	NumCible=-1;
	Nom="";
	Segment.RemoveAll();
	SEGMENT S;
	Segment.Add(S);
	NrSegmentActif=0;
	fMemorise=1;
	UpdateSegment();
	SetTitre();
}

void CDlgRepCible::OnCibleCharger() 
{
	if(!IsMemoOK())return;

	CDlgFonctionCible dlg;
	dlg.m_Mode=0;
	if(dlg.DoModal()!=IDOK)return;

	NumCible=dlg.m_NumCible;
	Nom=dlg.m_Nom;
	theApp.m_Cible.ChargerCible(Segment,NumCible);
	NrSegmentActif=0;
	fMemorise=1;
	UpdateSegment();
	SetTitre();
}

void CDlgRepCible::OnCibleMmoriser() 
{
	MemoriseCible();
	SetTitre();
}


void CDlgRepCible::OnCibleRenomer() 
{
	if(!IsMemoOK())return;

	CDlgFonctionCible dlg;
	dlg.m_Mode=2;
	dlg.m_Nom=Nom;
	if(dlg.DoModal()!=IDOK)return;
	theApp.m_Cible.RenommerCible(dlg.m_NumCible,dlg.m_Nom);
	if(dlg.m_NumCible==NumCible)
	{
		Nom=dlg.m_Nom;
		SetTitre();
	}
}

void CDlgRepCible::OnCibleSupprimer() 
{
	if(!IsMemoOK())return;

	CDlgFonctionCible dlg;
	dlg.m_Mode=3;
	dlg.m_Nom=Nom;
	if(dlg.DoModal()!=IDOK)return;
	theApp.m_Cible.DeleteCible(dlg.m_NumCible);
	if(dlg.m_NumCible==NumCible)
	{
		NumCible=-1;
		Nom="";
		Segment.RemoveAll();
		SEGMENT S;
		Segment.Add(S);
		NrSegmentActif=0;
		fMemorise=1;
		UpdateSegment();
		SetTitre();
	}
}

void CDlgRepCible::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CBrush fond(RGB_BleuJFC); //(RGB(23,114,121));

	/*
	CBitmap Bitmap;
	Bitmap.LoadBitmap(IDB_PATTERN1);
	fond.CreatePatternBrush(&Bitmap);
	*/

	CRect rect;
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);

}

HBRUSH CDlgRepCible::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if(nCtlColor==CTLCOLOR_EDIT)
	{
		pDC->SetBkMode(TRANSPARENT);
		return(theApp.hBrush[CLR_PAILLE]);
	}
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}


// retourne 1 si la mémorisation est ok
bool CDlgRepCible::IsMemoOK()
{
	if(fMemorise)return(true);

	int ret=MessageBox("Cette cible n'est pas mémorisée.\nVoulez-vous la mémoriser ?","Confirmation",MB_YESNOCANCEL|MB_ICONQUESTION);
	if(ret==IDCANCEL)return(false);
	fMemorise=1;
	if(ret==IDNO)return(true);
	return(MemoriseCible());
}

bool CDlgRepCible::MemoriseCible()
{
	CDlgFonctionCible dlg;
	// controle du nombre de cas
	if(m_NbCasSeg<NB_CASINTERDIT)
	{
		MessageBox("Le nombre d'interviews de la cible est insuffisant.\nLa cible ne peut pas être mémorisée.","Avertissement",MB_OK |MB_ICONWARNING); 
		return(true);
	}
	if(m_NbCasSeg<NB_CASAVERTISSEMENT)
	{
		if(MessageBox("Le nombre d'interviews de la cible semble insuffisant.\nEtes vous sûr de vouloir continuer ?","Avertissement",MB_OKCANCEL |MB_ICONWARNING)==IDCANCEL)
		{
			return(true);
		}

	}
	// cette cible a-t-elle un nom et un numéro ?
	if(NumCible<0)
	{
		dlg.m_Mode=1;
		if(dlg.DoModal()!=IDOK)return(false);

		NumCible=theApp.m_Cible.m_NextNumCible;
		theApp.m_Cible.m_NextNumCible++;
		Nom=dlg.m_Nom;
		
		theApp.m_Cible.m_NumCible.Add(NumCible);
		theApp.m_Cible.m_LibCible.Add(Nom);	
	}

	theApp.m_Cible.MemoriserCible(Segment,NumCible);

	fMemorise=1;
	return(true);
}

void CDlgRepCible::OnClose() 
{
	if(!IsMemoOK())return;
	
	CDialog::OnClose();
}

void CDlgRepCible::SetTitre()
{
	CString txt;
	txt="Répertoire de cibles";

	if(Nom!="")
	{
		txt+=" : "+Nom;
	}
	SetWindowText(txt);
}

void CDlgRepCible::OnDestroy() 
{
	CDialog::OnDestroy();
	

}



void CDlgRepCible::OnDblclkList2() 
{
	OnSelchangeList2();	
}

void CDlgRepCible::OnDblclkList3() 
{
	OnSelchangeList3();	
}

void CDlgRepCible::OnCibleMemoriserSous() 
{
	int oldnum=NumCible;
	NumCible=-1;
	CString oldnom;
	oldnom=Nom;
	Nom="";
	if(!MemoriseCible())
	{
		NumCible=oldnum;
		Nom=oldnom;
	}
	SetTitre();
	
}

void CDlgRepCible::OnUpdateCibleMemoriserSous(CCmdUI* pCmdUI) 
{
//	pCmdUI->Enable(NumCible>=0);
	
}
