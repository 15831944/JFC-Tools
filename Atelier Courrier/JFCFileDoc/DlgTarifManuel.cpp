// DlgTarifManuel.cpp : fichier d'impl�mentation
//

#include "stdafx.h"
#include "DlgTarifManuel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Bo�te de dialogue CDlgTarifManuel
IMPLEMENT_DYNAMIC(CDlgTarifManuel, JFCDialog)

CDlgTarifManuel::CDlgTarifManuel(CWnd* pParent, JATPDocument *pDoc)
	: JFCDialog(CDlgTarifManuel::IDD, pParent)
{
	// R�cup�re le document en cours
	m_pDocument = pDoc;

	// Init Map des nouveaux tarifs manuels
	m_MapNewTarifManuel.Reset();

	// Initialisation dates tarifaires
	m_DateDeb.Reset();
	m_DateFin.Reset();

	// Init du format courant pour le support
	m_IdFormatSel = 0;

}

CDlgTarifManuel::~CDlgTarifManuel()
{
	// Sortie boite de dialogue
	bool Sortie = true;
}

void CDlgTarifManuel::DoDataExchange(CDataExchange* pDX)
{
	JFCDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK,					m_BtnOK);
	DDX_Control(pDX, IDCANCEL,				m_BtnCancel);
}

BOOL CDlgTarifManuel::OnInitDialog()
{
	JFCDialog::OnInitDialog();

	// Mise en place des controles
	SetupControls();

	// Affichage du libelle support
	CRegroupements * pReg = CRegroupements::GetInstance();
	IElem * pElem= pReg->GetIElemByID(m_IdSupport);
	if(pElem)
	{
		// on �crit le libell� du Support
		m_TxtSupport.SetWindowText(pElem->GetLabel().AsJCharPtr()); 
	}
	else
		return FALSE;

	// Mise � jour combo box des formats
	const JSRCPresse  & srcpresse = JSRCPresse::GetInstance();

	// On boucle sur tous les formats disponibles
	// JATPDocChoixSupport::GetListIdFormats(JUnt32 IdSupport, JMap <JUnt32,JUnt32> *pMapIdFormats)
	/* @@@@@@
	for (m_pDocument->m_MapIdFormatsSel.MoveFirst();
		 pDocCreuset->m_MapIdFormatsSel.IsValid();
		 pDocCreuset->m_MapIdFormatsSel.MoveNext())
	{
		// R�cup item correspondant � l'indice format
		srcpresse.m_TBLFormats.MoveTo(pDocCreuset->m_MapIdFormatsSel.GetKey()); 
		if(srcpresse.m_TBLFormats.IsValid())
		{
			// Element format
			IElem *pElem = srcpresse.m_TBLFormats.GetItem();

			// Ajoute libelle format et ident format
			JUnt32 IdFormat = pElem->GetID();
			int nNewIndex = m_ComboFormat.AddString(pElem->GetLabel().AsJCharPtr());
			m_ComboFormat.SetItemData(nNewIndex, IdFormat); 
		}
	}
	*/

	for (m_pMapIdFormat->MoveFirst(); m_pMapIdFormat->IsValid(); m_pMapIdFormat->MoveNext())
	{
		// R�cup item correspondant � l'indice format
		srcpresse.m_TBLFormats.MoveTo(m_pMapIdFormat->GetKey()); 
		if(srcpresse.m_TBLFormats.IsValid())
		{
			// Element format
			IElem *pElem = srcpresse.m_TBLFormats.GetItem();

			// Ajoute libelle format et ident format
			JUnt32 IdFormat = pElem->GetID();
			int nNewIndex = m_ComboFormat.AddString(pElem->GetLabel().AsJCharPtr());
			m_ComboFormat.SetItemData(nNewIndex, IdFormat); 
		}
	}

	// R�cup�re le format en cours de s�lection 
	// (si plusieurs on affiche le 1er tarif format de la liste des formats dispo)
	JUnt32 IndexFormat = GetIndexFormatSel();

	// On se positionne sur le format en cours de s�lection
	m_ComboFormat.SetCurSel(IndexFormat); 

	// Affichage du tarif format en cours
	JUnt32 IdFormat = m_ComboFormat.GetItemData(IndexFormat);
	AfficheTarif(IdFormat);

	// Focus sur controle �dition tarif
	m_EditTarif.SetFocus(); 

	// return TRUE;  // retourne TRUE  sauf si vous avez d�fini le focus sur un contr�le
}

///////////////////////////////////////////////////////////////////////////////////////
// D�finition des controles
void CDlgTarifManuel::SetupControls()
{
	// D�clare les controles
	m_CadreTarif.SubclassDlgItem(IDC_FD_BK_SAISIETARIF, this);
	m_LblFormat.SubclassDlgItem(IDC_FD_LBL_FORMAT, this);
	m_LblTarif.SubclassDlgItem(IDC_FD_LBL_TARIF, this);
	m_ComboFormat.SubclassDlgItem(IDC_FD_CBX_FORMAT, this);
	m_EditTarif.SubclassDlgItem(IDC_FD_EDIT_TARIF, this);
	m_BtnTarifDeBase.SubclassDlgItem(IDC_FD_TARIFBASE, this);
	m_Separ.SubclassDlgItem(IDC_FD_SEPAR, this);
	m_TxtSupport.SubclassDlgItem(IDC_FD_TXTSUPPORT,this);

	// Ajout des controles dans la boite cadre
	m_CadreTarif.AddTopWindow(m_LblFormat);
	m_CadreTarif.AddTopWindow(m_LblTarif);
	m_CadreTarif.AddTopWindow(m_ComboFormat);
	m_CadreTarif.AddTopWindow(m_EditTarif);
	m_CadreTarif.AddTopWindow(m_BtnTarifDeBase);
	m_CadreTarif.AddTopWindow(m_Separ);
	m_CadreTarif.AddTopWindow(m_TxtSupport);

	// D�finit les couleurs
	SetColors();
}

////////////////////////////////////////////////////////////////////////////////////////
// Mise � jour des couleurs
void CDlgTarifManuel::SetColors(CATPColors::APPCOLOR app)
{
	m_appColor = app;
}

///////////////////////////////////////////////////////////////////////////////////////
// Couleur des controles
void CDlgTarifManuel::SetColors()
{
	// Cadre
	m_CadreTarif.SetBkColor(CATPColors::GetColorMedium(m_appColor),CATPColors::COLORNOAPP, CATPColors::MEDIUM_DEGRAD);
	m_CadreTarif.SetSunken(true);

	// Couleur des labels
	m_LblFormat.SetFontSize(CATPColors::GetFontLabelSize());
	m_LblFormat.SetBkColor(CATPColors::GetColorDark(m_appColor),m_appColor, CATPColors::DARK_DEGRAD);
	m_LblFormat.SetTextColor(CATPColors::GetColorSuperDark(m_appColor));
	
	m_LblTarif.SetFontSize(CATPColors::GetFontLabelSize());
	m_LblTarif.SetBkColor(CATPColors::GetColorDark(m_appColor),m_appColor, CATPColors::DARK_DEGRAD);
	m_LblTarif.SetTextColor(CATPColors::GetColorSuperDark(m_appColor));
	
	// Nom du support
	m_TxtSupport.SetFontSize(CATPColors::GetFontBigLabelSize());
	m_TxtSupport.SetFontName(CATPColors::GetFontLabelName());
	m_TxtSupport.SetBkColor(CATPColors::GetColorMedium(m_appColor),m_appColor, CATPColors::MEDIUM_DEGRAD);
	m_TxtSupport.SetTextColor(CATPColors::GetColorWhite());
}

//////////////////////////////////////////////////////////////////////////////
// Positionne les infos tarifaires
void CDlgTarifManuel::SetDateTarif(JDate DateDeb, JDate DateFin)
{
	m_DateDeb = DateDeb;
	m_DateFin = DateFin;
}

/////////////////////////////////////////////////////////////////////////////
// Positionne les formats disponibles
void CDlgTarifManuel::SetFormatSupport(JMap <JUnt32,JUnt32> *pMapIdFormat, JUnt32 IdFormatSel)
{
	// R�cup la liste des formats s�lectionn�s pour ce support
	m_pMapIdFormat = pMapIdFormat;

	// R�cup le format en cours pour ce support
	m_IdFormatSel = IdFormatSel;
}

//////////////////////////////////////////////////////////////////////////////
// R�cup�ration index format du format en cours de s�lection sur �chelle
JUnt32 CDlgTarifManuel::GetIndexFormatSel()
{
	// Nombre de formats s�lectionn�s dans �chelle
	JUnt32 NbFormatSel = 0;

	// Ident format s�lectionn�
	JUnt32 IdFormatSel = 0;

	// Balaye tous les formats disponibles 
	for(m_pMapIdFormat->MoveFirst(); m_pMapIdFormat->IsValid(); m_pMapIdFormat->MoveNext())
	{
		if(m_pMapIdFormat->GetItem() >=2)
		{
			IdFormatSel = m_pMapIdFormat->GetKey();
			NbFormatSel ++;
		}
	}

	// Si plusieurs formats s�lectonn�s on s�lectionnera dans combox box le 1er de la liste
	if (NbFormatSel > 1)
	{
		// Index 1er elmt de la liste des formats
		return 0;
	}
	else
	{
		// R�cup�re index liste du seul format s�lectionn� dans �chelle
		for (JUnt32 Index = 0; Index < m_ComboFormat.GetCount(); Index++)
		{
			if (m_ComboFormat.GetItemData(Index) == m_IdFormatSel)
			{
				// Ident format trouv�, on renvoie son index
				return Index;
			}
		}
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�ration tarif de base
JFlt32 CDlgTarifManuel::GetTarifSource(JUnt32 IdSupport, JUnt32 IdFormat)
{
	// R�cup�re instance tarifs
	CRegroupements * pReg = CRegroupements::GetInstance();
	CTarifs * pTarifs = CTarifs::GetInstance();

	// R�cup id support
	IElem *pElem = pReg->GetIElemByID(IdSupport);

	// Recherche dui tarif
	JUnt32 Duree;
	JDate  DateExact;
	JUnt32 Tarif = 0;

	// On v�rifie tout d'abord si les dates tarifaires sont valides
	if (m_DateDeb.IsValid() && m_DateFin.IsValid())
	{
		if(pTarifs->GetTarifByDate(IdSupport, IdFormat, m_DateDeb, m_DateFin, DateExact,Duree,Tarif))
			return (JFlt32)Tarif;
		else
			return 0.0;
	}

	return 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////
// Affichage du tarif 
void CDlgTarifManuel::AfficheTarif(JUnt32 IdFormat)
{
	// Init Tarif � afficher
	JFlt32 TarifCur = 0.0;

	// Affichage du tarif en cours (avec format courant)
	CKeyTarifManuel KeyTarifManuel;
	KeyTarifManuel.m_IdSupport  = m_IdSupport;
	KeyTarifManuel.m_IdFormat   = IdFormat;

	// On regarde d'abord dans les nouveaux tarifs manuels
	m_MapNewTarifManuel.MoveTo(KeyTarifManuel);
	if (m_MapNewTarifManuel.IsValid())
	{
		// Lecture tarif saisie depuis l'ouverture de la boite
		TarifCur = m_MapNewTarifManuel.GetItem(); 		
	}
	else
	{
		// 
		m_pDocument->m_MapTarifManuel.MoveTo(KeyTarifManuel);
		if (m_pDocument->m_MapTarifManuel.IsValid())
		{
			// Affiche tarif manuel
			TarifCur = m_pDocument->m_MapTarifManuel.GetItem(); 		
		}
		else 
		{
			// Affiche tarif existant dans la base
			TarifCur = GetTarifSource(m_IdSupport, IdFormat);
		}
	}

	// Update controle affichage tarif
	CString TxtTarif;
	TxtTarif.Format("%0.f",TarifCur); 
	m_EditTarif.SetWindowText(TxtTarif);
}

BEGIN_MESSAGE_MAP(CDlgTarifManuel, JFCDialog)
	ON_CBN_SELCHANGE(IDC_FD_CBX_FORMAT, OnCbnSelchangeCrCbxFormat)
	ON_EN_CHANGE(IDC_FD_EDIT_TARIF, OnEnChangeCrEditTarif)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_WM_KEYDOWN()
	ON_BN_CLICKED(IDC_FD_TARIFBASE, OnBnClickedCrTarifbase)
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////
// Changement de format
void CDlgTarifManuel::OnCbnSelchangeCrCbxFormat()
{
	// R�cup�re nouvelle s�lection format
	JUnt32 IndexCur = m_ComboFormat.GetCurSel(); 
	JUnt32 IdFormat = m_ComboFormat.GetItemData(IndexCur); 

	// Affichage du tarif format en cours
	AfficheTarif(IdFormat);
}

///////////////////////////////////////////////////////////////////////////
// Changement d e tarif
void CDlgTarifManuel::OnEnChangeCrEditTarif()
{
	// R�cup�ration et sauvegarde nouveau tarif
	RecupEtSauveTarif();
}

//////////////////////////////////////////////////////////////////////////////////////
// Validation des nouveaux tarifs manuels
void CDlgTarifManuel::OnBnClickedOk()
{
	// Sauve le dernier tarif entr� si valide
	 if (!RecupEtSauveTarif())
		 return;

	// Recopie les nouveaux tarifs manuels dans map tarif manuel
	if (m_MapNewTarifManuel.GetCount() > 0)
	{
		// Balaye tous les nouveaux tarifs manuels
		for (m_MapNewTarifManuel.MoveFirst(); m_MapNewTarifManuel.IsValid(); m_MapNewTarifManuel.MoveNext())
		{
			// Cl� tarif manuel
			CKeyTarifManuel NewKeyTarifManuel = m_MapNewTarifManuel.GetKey();

			// Tarif de base
			JFlt32 TarifSource	= GetTarifSource(NewKeyTarifManuel.m_IdSupport , 
												 NewKeyTarifManuel.m_IdFormat);

			// Nouveau tarif manuel
			JFlt32 NewTarifManuel = m_MapNewTarifManuel.GetItem(); 
			
			// On l'ajoute si inexistant au map courant des tarifs manuels
			m_pDocument->m_MapTarifManuel.MoveTo(NewKeyTarifManuel);
			if (!m_pDocument->m_MapTarifManuel.IsValid())
			{
				// On l'ajoute uniquement si non identique au tarif de base
				if (NewTarifManuel != TarifSource)
					m_pDocument->m_MapTarifManuel.Add(NewKeyTarifManuel) = NewTarifManuel; 
			}
			else
			{
				// Tarif manuel d�j� existant dans map des tarifs manuels
				if (NewTarifManuel != TarifSource)
				{
					// On remplace l'ancien tarif manuel si diff�rent tarif de base
					JFlt32 &ExTarifManuel = m_pDocument->m_MapTarifManuel.GetItem();
					ExTarifManuel =  NewTarifManuel;
				}
				else
				{
					// On vire le tarif manuel inutile
					m_pDocument->m_MapTarifManuel.Remove(); 
				}
			}
		}	
	}

	// On sort et on valide
	OnOK();
}

///////////////////////////////////////////////////////////////////////
// Annulation des nouveaus tarifs manuels
void CDlgTarifManuel::OnBnClickedCancel()
{
	// Rien � faire
	OnCancel();
}

///////////////////////////////////////////////////////////////////////
// R�cup�ration et sauvegarde nouveau tarif
JBool CDlgTarifManuel::RecupEtSauveTarif()
{
	// Recup format en cours
	JUnt32 IndexCur		= m_ComboFormat.GetCurSel(); 
	JUnt32 IdFormatCur	= m_ComboFormat.GetItemData(IndexCur); 

	// Recup Tarif manuel
	JFlt32 TarifSource	= GetTarifSource(m_IdSupport, IdFormatCur);

	// Recup tarif
	CString TxtTarif;
	JFlt32 NewTarif;
	m_EditTarif.GetWindowText(TxtTarif);
	NewTarif = atof(TxtTarif);

	if (NewTarif > 9999999)
	{
		// D�passement maximum float
		CString TxtMess;
		TxtMess.LoadString(IDS_FD_MAX_TARIF); 
		AfxMessageBox(TxtMess,MB_ICONEXCLAMATION); 
		return false;
	}

	// Cl� nouveau tarif manuel
	CKeyTarifManuel KeyTarifManuel;
	KeyTarifManuel.m_IdSupport  = m_IdSupport;
	KeyTarifManuel.m_IdFormat   = IdFormatCur;

	// On compare tarif source et nouveau tarif
	if (NewTarif != TarifSource)
	{
		// On v�rifie que ce tarif n'est pas d�j� un tarif manuel
		m_pDocument->m_MapTarifManuel.MoveTo(KeyTarifManuel);
		if (m_pDocument->m_MapTarifManuel.IsValid())
		{
			// R�cup�re tarif manuel d�j� entr�
			JFlt32 TarifManuelCur = m_pDocument->m_MapTarifManuel.GetItem(); 
			
			// On le compare avec le nouveau tarif
			if (NewTarif != TarifManuelCur)
			{
				// Nouveau tarif diff�rent du tarif entr� pr�c�demment
				// On stocke ce nouveau tarif
				m_MapNewTarifManuel.MoveTo(KeyTarifManuel);
				if (!m_MapNewTarifManuel.IsValid())
				{
					m_MapNewTarifManuel.Add(KeyTarifManuel) = NewTarif; 
				}
				else
				{
					// Existait d�j�
					JFlt32 &ExTarif = m_MapNewTarifManuel.GetItem(); 
					ExTarif = NewTarif;
				}
			}
		}
		else
		{
			// Nouveau tarif, on v�rifie qu'il n'a pas d�j� un nouveau tarif manuel
			m_MapNewTarifManuel.MoveTo(KeyTarifManuel);
			if (!m_MapNewTarifManuel.IsValid())
			{
				// Ajout nouveau tarif manuel
				m_MapNewTarifManuel.Add(KeyTarifManuel) = NewTarif; 
			}
			else
			{
				// Remplace nouveau tarif manuel
				JFlt32 &ExTarif = m_MapNewTarifManuel.GetItem(); 
				ExTarif = NewTarif;
			}
		}
	}
	else
	{
		// M�me tarif que tarif source
		// On v�rifie que ce tarif n'est pas d�j� un tarif manuel
		m_pDocument->m_MapTarifManuel.MoveTo(KeyTarifManuel);
		if (m_pDocument->m_MapTarifManuel.IsValid())
		{
			// R�cup�re tarif manuel d�j� entr�
			JFlt32 TarifManuelCur = m_pDocument->m_MapTarifManuel.GetItem();

			// Si tarif manuel courant, diff�rent du tarif entr� (lui m�me �gal au tarif source)
			if (TarifManuelCur != TarifSource)
			{
				// Nouveau tarif, on v�rifie qu'il n'a pas d�j� un nouveau tarif manuel
				m_MapNewTarifManuel.MoveTo(KeyTarifManuel);
				if (!m_MapNewTarifManuel.IsValid())
				{
					// Ajout nouveau tarif manuel
					m_MapNewTarifManuel.Add(KeyTarifManuel) = NewTarif; 
				}
				else
				{
					// Remplace nouveau tarif manuel
					JFlt32 &ExTarif = m_MapNewTarifManuel.GetItem(); 
					ExTarif = NewTarif;
				}				
			}
		}
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////
// Gestion des messages 
BOOL CDlgTarifManuel::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		// On valide dernier tarif
		OnBnClickedOk();
		return JFCDialog::PreTranslateMessage(pMsg);
	}

	return JFCDialog::PreTranslateMessage(pMsg);
}

/////////////////////////////////////////////////////////////////////////////
// Gestion �venement clavier
void CDlgTarifManuel::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
		// Retour chariot >> on valide dernier tarif entr�
		case VK_RETURN:	
			OnBnClickedOk();
			break;
	}
	JFCDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

////////////////////////////////////////////////////////
// On remet les tarifs source pour ce support
void CDlgTarifManuel::OnBnClickedCrTarifbase()
{
	// Il suffit de r�initialiser les nouveaux tarifs manuels
	m_MapNewTarifManuel.Reset();

	// Cl� nouveau tarif manuel
	CKeyTarifManuel KeyTarifManuel;
	KeyTarifManuel.m_IdSupport  = m_IdSupport;

	// R�cup�re index liste du seul format s�lectionn� dans �chelle
	for (JUnt32 Index = 0; Index < m_ComboFormat.GetCount(); Index++)
	{
		// R�cup�re le format
		JUnt32 IdFormat = m_ComboFormat.GetItemData(Index);

		// R�cup le tarif source correspondant
        JFlt32 TarifSource = GetTarifSource(m_IdSupport, IdFormat);

		// Ajoute au tarif manuel temporaire 
		KeyTarifManuel.m_IdFormat = IdFormat;
		m_MapNewTarifManuel.Add(KeyTarifManuel) = TarifSource; 
	}

	// Puis on r�affiche le tarif source courant
	JUnt32 IndexCurF	= m_ComboFormat.GetCurSel();
	JUnt32 IdFormatCur	= m_ComboFormat.GetItemData(IndexCurF);
	AfficheTarif(IdFormatCur);

	// Repositionne focus sur tarif � modifier
	m_EditTarif.SetFocus(); 

}
