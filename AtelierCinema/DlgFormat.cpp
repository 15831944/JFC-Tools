// DlgFormat.cpp : implementation file
//

#include "stdafx.h"
#include "cinema.h"
#include "cinemaDoc.h"
#include "cinemaView.h"

extern CCinemaApp theApp;
#include "DlgFormat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFormat dialog


CDlgFormat::CDlgFormat(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFormat::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgFormat)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgFormat::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFormat)
	DDX_Control(pDX, IDC_CADRE, m_Duree);
	DDX_Control(pDX, IDC_LIST2, m_LB2);
	DDX_Control(pDX, IDC_LIST1, m_LB1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFormat, CDialog)
	//{{AFX_MSG_MAP(CDlgFormat)
	ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeList1)
	ON_LBN_SELCHANGE(IDC_LIST2, OnSelchangeList2)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFormat message handlers

void CDlgFormat::OnOK() 
{
	POSITION pos = theApp.GetFirstDocTemplatePosition();			
	CDocTemplate* DocTemplate = theApp.GetNextDocTemplate(pos);			
	pos = DocTemplate->GetFirstDocPosition(); 			
	CCinemaDoc *  Doc = (CCinemaDoc *)DocTemplate->GetNextDoc(pos);			
	pos = Doc->GetFirstViewPosition(); 			
	CCinemaView * V = (CCinemaView *)Doc->GetNextView(pos); 			
	CCampagne * C;			
	C = &Doc->m_Campagne;			

	m_ListFormat.RemoveAll();

	for(int i=0;i<m_LB1.GetCount();i++)
	{
		CString txt;
		m_LB1.GetText(i,txt);
		m_ListFormat.Add(txt);
	}
	i=0;
	while(i<m_ListFormat.GetSize())
	{
		int nbelement=0;

		// Pour chaque élément de la liste,on comppte le nb d'éléments identique 
		for(int ei=i+1;ei<m_ListFormat.GetSize();ei++)
		{
			if(m_ListFormat[ei]==m_ListFormat[i])
			{
				nbelement++;
				i++;
			}
		}
		// Pour chaque libellé, on cherche le libellé correspondant dans la campagne
		// On boucle sur les libellés de la campagne
		for(int idxf=0;idxf<C->m_LibFormat.GetSize();idxf++)
		{
			if(m_ListFormat[i]==C->m_LibFormat[idxf])
			{
				// On affecte à la position i de la liste l'index (idxf) du format dans la campagne
				for(int nb=0;nb<nbelement+1;nb++)
				{
					// On controle que cet élément est dans la campagne
					if(C->m_LibFormat.GetSize()>(i-nbelement)+nb)
					{
						if(m_ListFormat[(i-nbelement)+nb]==C->m_LibFormat[idxf+nb])
							m_IndexFormatCampagne.Add(idxf+nb);
					}
					else m_IndexFormatCampagne.Add(-1);
				}
				break;
			}
		}
		// le libellé format n'a pas été trouvé
		if(idxf==C->m_LibFormat.GetSize())m_IndexFormatCampagne.Add(-1);
		i++;
	}
	int nb=m_ListFormat.GetSize();
	if(nb==0)
	{
		MessageBox("La sélection est invalide.");
		return;
	}
	if(nb>5)
	{
		CString txt;
		txt.Format("Pas plus de %d formats !",5);
		MessageBox(txt);
		return;
	}

	// affichage d'un message d'avertissement 		
	/*CString message;	
	LPCSTR titre = "Changement de format";	
	message.Format("Les campagnes vont être effacées!\nVoulez vous continuer?");	 	
	if(::MessageBox(this->m_hWnd,message,titre,MB_OKCANCEL)==IDOK)	
	{	
		// effacement de tous les formats de l'hypothèse active			
		POSITION pos = theApp.GetFirstDocTemplatePosition();			
		CDocTemplate* DocTemplate = theApp.GetNextDocTemplate(pos);			
		pos = DocTemplate->GetFirstDocPosition(); 			
		CCinemaDoc *  Doc = (CCinemaDoc *)DocTemplate->GetNextDoc(pos);			
		pos = Doc->GetFirstViewPosition(); 			
		CCinemaView * V = (CCinemaView *)Doc->GetNextView(pos); 			
		CCampagne * C;			
		C = &Doc->m_Campagne;			
		int h=C->m_HypotheseActive;			
		C->Efface(h);
		C->UpdateMap(1); 
		V->m_Grille.Invalidate();			
		V->m_NbPassages.Invalidate();			
		C->LanceCalcul(CCampagne::BUDGETPERF);			
		CDialog::OnOK();
	}*/
	CDialog::OnOK();
}

void CDlgFormat::OnSelchangeList1() 
{
	
	m_LB1.DeleteString(m_LB1.GetCurSel());
}


void CDlgFormat::OnSelchangeList2() 
{
	CString txt;
	m_LB2.GetText(m_LB2.GetCurSel(),txt);
	m_LB1.AddString(txt);
}

void CDlgFormat::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CBrush fond(RGB_BleuJFC); //(RGB(23,114,121));

	CRect rect;
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);
	
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CDlgFormat::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CTableFormat * tabFormat = theApp.m_TableCinema.GetTableFormat();
	for(int i=0;i<tabFormat->GetSize();i++)
	{
		m_LB2.AddString(tabFormat->GetLibelle(i));
	}

	for(i=0;i<m_ListFormat.GetSize();i++)
	{
		m_LB1.AddString(m_ListFormat[i]);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
