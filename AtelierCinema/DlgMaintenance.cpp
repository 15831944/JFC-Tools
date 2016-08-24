// DlgMaintenance.cpp : implementation file
//

#include "stdafx.h"
#include "cinema.h"
extern CCinemaApp theApp;
#include "DlgMaintenance.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMaintenance dialog


CDlgMaintenance::CDlgMaintenance(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMaintenance::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMaintenance)
	m_TarifSaisonDuree = -1;
	//}}AFX_DATA_INIT
}


void CDlgMaintenance::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMaintenance)
	DDX_Control(pDX, IDC_COMBOANNEE, m_CBAnnee);
	DDX_Control(pDX, IDC_CADRE, m_Cadre1);
	DDX_Radio(pDX, IDC_RADIO1, m_TarifSaisonDuree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMaintenance, CDialog)
	//{{AFX_MSG_MAP(CDlgMaintenance)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	ON_CBN_SELCHANGE(IDC_COMBOANNEE, OnSelchangeComboannee)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMaintenance message handlers

BOOL CDlgMaintenance::OnInitDialog() 
{
#ifdef MAINTENANCE
	CDialog::OnInitDialog();
	CString txt;



	// Init des années
	m_CBAnnee.ResetContent();
	for(int i=0;i<theApp.m_Tarifs.GetSize();i++)
	{

		txt.Format("%d",theApp.m_Tarifs[i].m_Annee);
		if(m_CBAnnee.FindString(-1,txt)==CB_ERR)
		{
			m_CBAnnee.AddString(txt);
		}
	}
	m_CBAnnee.SetCurSel(m_CBAnnee.GetCount()-1);


	m_wndGrid.SubclassDlgItem(IDC_GRID,this);   
	m_wndGrid.Initialize();

	m_wndGrid.SetRowCount(100);
	m_wndGrid.SetColCount(100);

	// Init des paramètres
	m_wndGrid.GetParam()->EnableTrackColWidth(0); 
	m_wndGrid.GetParam()->EnableTrackRowHeight(0); 
	m_wndGrid.GetParam()->SetEnterKeyAction(GX_DOWN);
	m_wndGrid.GetParam()->SetActivateCellFlags(FALSE); 
	m_wndGrid.GetParam()->SetGridLineStyle(PS_SOLID);
	m_wndGrid.GetParam()->EnableSelection(GX_SELNONE );
	//m_wndGrid.GetParam()->GetProperties()->SetColor(GX_COLOR_BACKGROUND,RGB(155,90,235));
	m_wndGrid.GetParam()->GetProperties()->SetColor(GX_COLOR_BACKGROUND,RGB(128,128,128));
	

	
	// Init des couleurs des cellules
	m_wndGrid.SetStyleRange( CGXRange().SetCols(1,100),
			CGXStyle()
				.SetInterior(
					CGXBrush()
					.SetColor(RGB(255,255,192)) 
				)
	);
	
	// Les résultats sont cadrés à droite
	m_wndGrid.SetStyleRange( CGXRange().SetCols(1,100),
			CGXStyle()
				.SetHorizontalAlignment(DT_RIGHT)

	);

	m_wndGrid.SetColWidth(0,0,100);


/*	// Init des libelles des colones
	CString txt;
	m_wndGrid.SetValueRange(CGXRange(0,0),"Cible");
	m_wndGrid.SetValueRange(CGXRange(0,1),"GRP");
	m_wndGrid.SetValueRange(CGXRange(0,2),"Répét.");
	m_wndGrid.SetValueRange(CGXRange(0,3),"C%");
	m_wndGrid.SetValueRange(CGXRange(0,4),"E%");

	m_wndGrid.SetValueRange(CGXRange(1,0),"Radio");
	m_wndGrid.SetValueRange(CGXRange(2,0),"Télé");
	m_wndGrid.SetValueRange(CGXRange(3,0),"");
	m_wndGrid.SetValueRange(CGXRange(4,0),"Total");
	
	
	

	// Init des tailles
	m_wndGrid.SetColWidth(0,4,50);
	m_wndGrid.SetRowHeight(3,3,2);

	//Les tooltips
	/*m_wndGrid.EnableGridToolTips();

   m_wndGrid.SetStyleRange(CGXRange(1,0), CGXStyle()
      .SetUserAttribute(GX_IDS_UA_TOOLTIPTEXT, _T("Utilisez le clique gauche pour augmenter,\r\nle clique droit pour diminuer")));
   m_wndGrid.SetStyleRange(CGXRange(2,0), CGXStyle()
      .SetUserAttribute(GX_IDS_UA_TOOLTIPTEXT, _T("Utilisez le clique gauche pour augmenter,\r\nle clique droit pour diminuer")));
   m_wndGrid.SetStyleRange(CGXRange(3,0), CGXStyle()
      .SetUserAttribute(GX_IDS_UA_TOOLTIPTEXT, _T("Utilisez le clique gauche pour augmenter,\r\nle clique droit pour diminuer")));
	  */

/*	// GRP radio
	txt.Format("%.1f",GRPradio);
	m_wndGrid.SetValueRange(CGXRange(1,1),txt);

	// GRP télé
	txt.Format("%.1f",GRPtele);
	m_wndGrid.SetValueRange(CGXRange(2,1),txt);

	// GRP radio+télé
	txt.Format("%.1f",GRPradio+GRPtele);
	m_wndGrid.SetValueRange(CGXRange(4,1),txt);

	// Répétition radio
	txt.Format("%.1f",GRPradio/Couv[0]);
	m_wndGrid.SetValueRange(CGXRange(1,2),txt);

	// Répétition télé
	txt.Format("%.1f",GRPtele/Couv[1]);
	m_wndGrid.SetValueRange(CGXRange(2,2),txt);

	// Répétition radio+télé
	txt.Format("%.1f",(GRPradio+GRPtele)/Couv[2]);
	m_wndGrid.SetValueRange(CGXRange(4,2),txt);


	// C% radio
	txt.Format("%.2f",Couv[0]);
	m_wndGrid.SetValueRange(CGXRange(1,3),txt);

	// C% télé
	txt.Format("%.2f",Couv[1]);
	m_wndGrid.SetValueRange(CGXRange(2,3),txt);

	// C% radio+télé
	txt.Format("%.2f",Couv[2]);
	m_wndGrid.SetValueRange(CGXRange(4,3),txt);


	// E% radio
	txt.Format("%.2f",Couv[3]);
	m_wndGrid.SetValueRange(CGXRange(1,4),txt);

	// E% télé
	txt.Format("%.2f",Couv[4]);
	m_wndGrid.SetValueRange(CGXRange(2,4),txt);

	// E% radio+télé
	txt.Format("%.2f",Couv[5]);
	m_wndGrid.SetValueRange(CGXRange(4,4),txt);*/

#endif	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMaintenance::OnRadio1() 
{
	UpdateData();
	UpdateVisu();
}

void CDlgMaintenance::OnRadio2() 
{
	UpdateData();
	UpdateVisu();
}

void CDlgMaintenance::OnRadio3() 
{
	UpdateData();
	UpdateVisu();
}

void CDlgMaintenance::OnSelchangeComboannee() 
{
	UpdateData();
	UpdateVisu();
}

void CDlgMaintenance::UpdateVisu()
{
#ifdef MAINTENANCE
	CString txt;
	m_CBAnnee.GetWindowText(txt);
	int Annee=atoi(txt);

	switch(m_TarifSaisonDuree)
	{
		case 0: // Tarif
		{
			int nbcol=53; // (semaines)
			int nbline=theApp.NbBinome;
			m_wndGrid.SetRowCount(0);
			m_wndGrid.SetColCount(0);
			m_wndGrid.SetRowCount(nbline);
			m_wndGrid.SetColCount(nbcol);
			// Init des couleurs des cellules
			m_wndGrid.SetStyleRange( CGXRange().SetCols(1,nbcol),
					CGXStyle()
						.SetInterior(
							CGXBrush()
							.SetColor(RGB(255,255,192)) 
						)
			);

			// Les résultats sont cadrés à droite
			m_wndGrid.SetStyleRange( CGXRange().SetCols(1,nbcol),
					CGXStyle()
						.SetHorizontalAlignment(DT_RIGHT)

			);


			m_wndGrid.SetValueRange(CGXRange(0,0),"Tarif (Euros)");
			for(int c=1;c<=nbcol;c++)
			{
				txt.Format("%d",c);
				m_wndGrid.SetValueRange(CGXRange(0,c),txt);
			}

			for(int l=0;l<nbline;l++)
			{
				m_wndGrid.SetValueRange(CGXRange(l+1,0),theApp.TabBinome[l].Nom);
			}
			for(l=0;l<nbline;l++)
			{
				// on recherche le tarif pour ce binôme
				for(int t=0;t<theApp.m_Tarifs.GetSize();t++)
				{
					if(theApp.m_Tarifs[t].m_Annee!=Annee)continue;
					if(theApp.m_Tarifs[t].m_NrBinome==theApp.TabBinome[l].NrUnique)
					{
						for(c=0;c<theApp.m_Tarifs[t].Prix.GetSize();c++)
						{
							if(c>=53)break;
							txt.Format("%d",theApp.m_Tarifs[t].Prix[c]);
							m_wndGrid.SetValueRange(CGXRange(l+1,c+1),txt);
						}
						break;
					}
				}
			}
		}
		break;
		case 1: // Saisonalité
		{
			int nbcol=53; // (semaines)
			int nbline=theApp.NbBinome;
			m_wndGrid.SetRowCount(0);
			m_wndGrid.SetColCount(0);
			m_wndGrid.SetRowCount(nbline);
			m_wndGrid.SetColCount(nbcol);
			// Init des couleurs des cellules
			m_wndGrid.SetStyleRange( CGXRange().SetCols(1,nbcol),
					CGXStyle()
						.SetInterior(
							CGXBrush()
							.SetColor(RGB(255,255,192)) 
						)
			);

			// Les résultats sont cadrés à droite
			m_wndGrid.SetStyleRange( CGXRange().SetCols(1,nbcol),
					CGXStyle()
						.SetHorizontalAlignment(DT_RIGHT)

			);


			m_wndGrid.SetValueRange(CGXRange(0,0),"Saisonalité");
			for(int c=1;c<=nbcol;c++)
			{
				txt.Format("%d",c);
				m_wndGrid.SetValueRange(CGXRange(0,c),txt);
			}

			for(int l=0;l<nbline;l++)
			{
				m_wndGrid.SetValueRange(CGXRange(l+1,0),theApp.TabBinome[l].Nom);
			}
			for(l=0;l<nbline;l++)
			{
				// on recherche le saisins pour ce binôme
				
				for(int t=0;t<theApp.m_Saisons.GetSize();t++)

				{
					if(theApp.m_Saisons[t].m_Annee!=Annee)continue;
					if(theApp.m_Saisons[t].m_NrBinome==theApp.TabBinome[l].NrUnique)
					{
						for(c=0;c<theApp.m_Saisons[t].m_Coeff.GetSize();c++)
						{
							if(c>=53)break;
							double v=theApp.m_Saisons[t].m_Coeff[c]/100.0;
							txt.Format("%.2f",v);
							m_wndGrid.SetValueRange(CGXRange(l+1,c+1),txt);
						}
						break;
					}
				}
			}
		}
		break;
		case 2: // Durée
		{
			int nbline=theApp.m_LibDuree.GetSize();
			m_wndGrid.SetRowCount(nbline);
			m_wndGrid.SetColCount(1);

			m_wndGrid.SetValueRange(CGXRange(0,0),"Durée");
			m_wndGrid.SetValueRange(CGXRange(0,1),"Coeff.");

			for(int i=0;i<nbline;i++)
			{
				m_wndGrid.SetValueRange(CGXRange(i+1,0),theApp.m_LibDuree[i]);
				m_wndGrid.SetValueRange(CGXRange(i+1,1),theApp.m_CoeffDuree[i]);
			}
		}
		break;
		default:
			ASSERT(0);

	}

	
	
/*	// Init des libelles des colones
	CString txt;
	m_wndGrid.SetValueRange(CGXRange(0,0),"Cible");
	m_wndGrid.SetValueRange(CGXRange(0,1),"GRP");
	m_wndGrid.SetValueRange(CGXRange(0,2),"Répét.");
	m_wndGrid.SetValueRange(CGXRange(0,3),"C%");
	m_wndGrid.SetValueRange(CGXRange(0,4),"E%");

	m_wndGrid.SetValueRange(CGXRange(1,0),"Radio");
	m_wndGrid.SetValueRange(CGXRange(2,0),"Télé");
	m_wndGrid.SetValueRange(CGXRange(3,0),"");
	m_wndGrid.SetValueRange(CGXRange(4,0),"Total");
	
	
	// Init des couleurs des cellules
	m_wndGrid.SetStyleRange( CGXRange().SetCols(1,4),
			CGXStyle()
				.SetInterior(
					CGXBrush()
					.SetColor(RGB(255,255,192)) 
				)
	);
	
	// Les résultats sont cadrés à droite !
	m_wndGrid.SetStyleRange( CGXRange(1,1,4,4),
			CGXStyle()
				.SetHorizontalAlignment(DT_RIGHT)

	);
	

	// Init des tailles
	m_wndGrid.SetColWidth(0,4,50);
	m_wndGrid.SetRowHeight(3,3,2);

	//Les tooltips
	/*m_wndGrid.EnableGridToolTips();

   m_wndGrid.SetStyleRange(CGXRange(1,0), CGXStyle()
      .SetUserAttribute(GX_IDS_UA_TOOLTIPTEXT, _T("Utilisez le clique gauche pour augmenter,\r\nle clique droit pour diminuer")));
   m_wndGrid.SetStyleRange(CGXRange(2,0), CGXStyle()
      .SetUserAttribute(GX_IDS_UA_TOOLTIPTEXT, _T("Utilisez le clique gauche pour augmenter,\r\nle clique droit pour diminuer")));
   m_wndGrid.SetStyleRange(CGXRange(3,0), CGXStyle()
      .SetUserAttribute(GX_IDS_UA_TOOLTIPTEXT, _T("Utilisez le clique gauche pour augmenter,\r\nle clique droit pour diminuer")));
	  */

/*	// GRP radio
	txt.Format("%.1f",GRPradio);
	m_wndGrid.SetValueRange(CGXRange(1,1),txt);

	// GRP télé
	txt.Format("%.1f",GRPtele);
	m_wndGrid.SetValueRange(CGXRange(2,1),txt);

	// GRP radio+télé
	txt.Format("%.1f",GRPradio+GRPtele);
	m_wndGrid.SetValueRange(CGXRange(4,1),txt);

	// Répétition radio
	txt.Format("%.1f",GRPradio/Couv[0]);
	m_wndGrid.SetValueRange(CGXRange(1,2),txt);

	// Répétition télé
	txt.Format("%.1f",GRPtele/Couv[1]);
	m_wndGrid.SetValueRange(CGXRange(2,2),txt);

	// Répétition radio+télé
	txt.Format("%.1f",(GRPradio+GRPtele)/Couv[2]);
	m_wndGrid.SetValueRange(CGXRange(4,2),txt);


	// C% radio
	txt.Format("%.2f",Couv[0]);
	m_wndGrid.SetValueRange(CGXRange(1,3),txt);

	// C% télé
	txt.Format("%.2f",Couv[1]);
	m_wndGrid.SetValueRange(CGXRange(2,3),txt);

	// C% radio+télé
	txt.Format("%.2f",Couv[2]);
	m_wndGrid.SetValueRange(CGXRange(4,3),txt);


	// E% radio
	txt.Format("%.2f",Couv[3]);
	m_wndGrid.SetValueRange(CGXRange(1,4),txt);

	// E% télé
	txt.Format("%.2f",Couv[4]);
	m_wndGrid.SetValueRange(CGXRange(2,4),txt);

	// E% radio+télé
	txt.Format("%.2f",Couv[5]);
	m_wndGrid.SetValueRange(CGXRange(4,4),txt);*/

#endif

}

void CDlgMaintenance::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CBrush fond; //(RGB(23,114,121));
	CBitmap Bitmap;
	Bitmap.LoadBitmap(IDB_PATTERN8);
	fond.CreatePatternBrush(&Bitmap);
	CRect rect;
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);
	
	// Do not call CDialog::OnPaint() for painting messages
}
