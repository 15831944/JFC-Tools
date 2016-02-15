// FormulaireNom.cpp : implementation file
//

#include "stdafx.h"
#include "Nomenclature.h"
#include "FormulaireNom.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFormulaireNom dialog


CFormulaireNom::CFormulaireNom(CWnd* pParent /*=NULL*/)
	: CDialog(CFormulaireNom::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFormulaireNom)
	//}}AFX_DATA_INIT
}


void CFormulaireNom::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormulaireNom)
	DDX_Control(pDX, IDC_CADRE3, m_Cadre3);
	DDX_Control(pDX, IDC_EDIT3_2, m_ChampsAliasFN);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormulaireNom, CDialog)
	//{{AFX_MSG_MAP(CFormulaireNom)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormulaireNom message handlers

BOOL CFormulaireNom::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetWindowText(m_TitreFenN);

	// Recup les nouveaux libellés
	m_ChampsAliasFN.SetWindowText(m_StrAliasFN);
	m_ChampsAliasFN.SetFocus();

	return false;  // return TRUE unless you set the focus to a control
	               // EXCEPTION: OCX Property Pages should return FALSE
	
}


// Affiche les valeurs de champs sur les controles dialogues
void CFormulaireNom ::MajValChamps()
{
	m_ChampsAliasFN.SetWindowText(m_StrAliasFN);
}

// Recupére les valeurs controles >> variables m_Str
void CFormulaireNom::RenvoiValChamps()
{
	m_ChampsAliasFN.GetWindowText(m_StrAliasFN);
}

void CFormulaireNom::OnOK() 
{

	CString MessErr;
	bool NomExist = false;

	// Recup les nouveaux libellés marque
	m_ChampsAliasFN.GetWindowText(m_StrAliasFN);

	if (m_TabMarque.GetSize() !=0 || m_TabFamille.GetSize() !=0 || m_TabSecteur.GetSize() !=0)
	{
		if (m_TabMarque.GetSize() !=0)
		{
			// Saisie Marque >> Vérifie que le nom de la marque n'est pas déjà utilisée
			for (int i= 0;i<m_TabMarque.GetSize();i++)
			{
				if (m_StrAliasFN == m_TabMarque[i])
				{
					NomExist = true;
					break;
				}
			}
		}

		else if (m_TabFamille.GetSize() !=0)
		{
			// Saisie Famille >> Vérifie que le nom de la famille n'est pas déjà utilisée
			for (int i= 0;i<m_TabFamille.GetSize();i++)
			{
				if (m_StrAliasFN == m_TabFamille[i])
				{
					NomExist = true;
					break;
				}
			}
		}
		
		else if (m_TabSecteur.GetSize() !=0)
		{
			// Saisie Secteur >> Vérifie que le nom de la famille n'est pas déjà utilisée
			for (int i= 0;i<m_TabSecteur.GetSize();i++)
			{
				if (m_StrAliasFN == m_TabSecteur[i])
				{
					NomExist = true;
					break;
				}
			}
		}
		
		if (m_StrAliasFN != "")
			if (NomExist == false)
				CDialog::OnOK();
			else
			{   // Nom déjà utilisée
				AfxMessageBox("Nom déjà utilisé !!!");
				m_ChampsAliasFN.SetWindowText("");
				m_ChampsAliasFN.SetFocus();
			}

		else 
		{
			MessErr = "Champs nom vide\n";
			MessErr = MessErr + "Vous devez saisir tous ces champs !!!!";
			AfxMessageBox(MessErr);
		}
	}

	else
	{
		// Saisie standard
		if (m_StrAliasFN != "")
			CDialog::OnOK();
		else 
		{
			MessErr = "Champs nom vide\n";
			MessErr = MessErr + "Vous devez saisir tous ces champs !!!!";
			AfxMessageBox(MessErr);
		}
	}	
		
}

void CFormulaireNom::OnPaint() 
{
	CRect rect;

	CPaintDC dc(this); // device context for painting
	
	// Coloriage du fond de ce formulaire
	/*
	CBrush fond(RGB_OrangeFormulaire);
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);
	*/

	CBrush fond(RGB_BleuJFC);
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);
}

