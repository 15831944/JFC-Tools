// DlgClassementItems.cpp : fichier d'impl�mentation
//
#include "stdafx.h"
#include "DlgClassementItems.h"
#include "DlgItems.h"
#include ".\dlgclassementitems.h"

// Bo�te de dialogue CDlgClassementItems
IMPLEMENT_DYNCREATE(CDlgClassementItems, CDialog)

// Constructeur
CDlgClassementItems::CDlgClassementItems(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgClassementItems::IDD, pParent)
{
}

// Destructeur
CDlgClassementItems::~CDlgClassementItems()
{
}

JVoid CDlgClassementItems::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

///////////////////////////////////////////////////////////////////////////////////
// Initialistaion boite de dialogue
BOOL CDlgClassementItems::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Init des contr�les
	SetupControls();

	// Test si nomenclature rubrique en cours valide
	if (m_pTreeRubrique)
	{
		// Init tableau des items en cours
		LoadItems();

		// Remplir liste des rubriques sources
		RecupListRubriqueSrc();

		// Remplir liste rubriques destination
		RecupListRubriqueDst();

		return TRUE;  // retourne TRUE  sauf si vous avez d�fini le focus sur un contr�le
	}
	return FALSE;
}

BEGIN_MESSAGE_MAP(CDlgClassementItems, CDialog)
	ON_WM_SIZE()
	ON_LBN_SELCHANGE(IDC_RI_LIST_RUBRIQUES, OnLbnSelchangeRiListRubriques)
	ON_BN_CLICKED(IDC_RI_BUT_TRANSFERT_ITEMS, OnBnClickedRiButTransfertItems)
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////////
// Initialisation des contr�les
JVoid CDlgClassementItems::SetupControls()
{
	// Cadre et entete de listes
	m_Cadre					.SubclassDlgItem(IDC_RI_STATIC_BOX, this);
	m_Label_Rubr_Src		.SubclassDlgItem(IDC_RI_LABEL_RUBR_SRC, this);
	m_Label_Items			.SubclassDlgItem(IDC_RI_LABEL_ITEMS, this);
	m_Label_Rubr_Dst		.SubclassDlgItem(IDC_RI_LABEL_RUBR_DST, this);
	
	// Les 3 listes pour classement des items
	m_Lst_Rubriques			.SubclassDlgItem(IDC_RI_LIST_RUBRIQUES, this);
	m_Lst_Items				.SubclassDlgItem(IDC_RI_LIST_ITEMS, this);
	m_Lst_Rubrique_Dest		.SubclassDlgItem(IDC_RI_LIST_RUBR_DEST, this);

	// Bouton action transfert items dans rubrique
	m_Btn_Transfert_Items	.SubclassDlgItem(IDC_RI_BUT_TRANSFERT_ITEMS, this);

	// Validation - A Virer
	m_BtnOK					.SubclassDlgItem(IDOK, this);

	// Validation des tooltips sur les listes
	m_Lst_Rubriques			.EnableToolTips(true);
	m_Lst_Items				.EnableToolTips(true);
	m_Lst_Rubrique_Dest		.EnableToolTips(true);

	// Propri�t�s du cadre englobant
	m_Cadre.SetTransparent(false);
	m_Cadre.SetSunken(true);

	// Composants � l'int�rieur du cadre
	m_Cadre.AddTopWindow(m_Lst_Rubriques);
	m_Cadre.AddTopWindow(m_Lst_Items);
	m_Cadre.AddTopWindow(m_Lst_Rubrique_Dest);
	m_Cadre.AddTopWindow(m_Label_Rubr_Src);
	m_Cadre.AddTopWindow(m_Label_Items);
	m_Cadre.AddTopWindow(m_Label_Rubr_Dst);
	m_Cadre.AddTopWindow(m_Btn_Transfert_Items);
	
	// Set the windows colors
	SetColors();

	// Set window relations and positions for resizing
	SetWindowRelations();
}

//////////////////////////////////////////////////////////////////////////////////////
// D�finition couleurs des contr�les
JVoid CDlgClassementItems::SetColors()
{
	// Fond Cadre
	m_Cadre.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORREPITEMS),CATPColors::COLORREPITEMS, CATPColors::MEDIUM_DEGRAD);

	// Ent�te liste rubriques sources
	m_Label_Rubr_Src.SetFontName(CATPColors::GetFontLabelName());
	m_Label_Rubr_Src.SetFontSize(CATPColors::GetFontLabelSize());
	m_Label_Rubr_Src.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORREPITEMS),CATPColors::COLORREPITEMS, CATPColors::DARK_DEGRAD);
	m_Label_Rubr_Src.SetBorder(false);
	m_Label_Rubr_Src.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORREPITEMS)); 

	// Ent�te liste 
	m_Label_Items.SetFontName(CATPColors::GetFontLabelName());
	m_Label_Items.SetFontSize(CATPColors::GetFontLabelSize());
	m_Label_Items.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORREPITEMS),CATPColors::COLORREPITEMS, CATPColors::DARK_DEGRAD);
	m_Label_Items.SetBorder(false);
	m_Label_Items.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORREPITEMS)); 

	// Ent�te liste rubrique destination
	m_Label_Rubr_Dst.SetFontName(CATPColors::GetFontLabelName());
	m_Label_Rubr_Dst.SetFontSize(CATPColors::GetFontLabelSize());
	m_Label_Rubr_Dst.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORREPITEMS),CATPColors::COLORREPITEMS, CATPColors::DARK_DEGRAD);
	m_Label_Rubr_Dst.SetBorder(false);
	m_Label_Rubr_Dst.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORREPITEMS));
	
	// Bouton transfert
	m_Btn_Transfert_Items.SetColor(CButtonST::BTNST_COLOR_BK_IN,	CATPColors::GetColorLight(CATPColors::COLORREPITEMS), CATPColors::COLORREPITEMS ,0);
	m_Btn_Transfert_Items.SetColor(CButtonST::BTNST_COLOR_BK_OUT,	CATPColors::GetColorMedium(CATPColors::COLORREPITEMS), CATPColors::COLORREPITEMS);
	m_Btn_Transfert_Items.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORREPITEMS), CATPColors::COLORREPITEMS);

	// Les listes
	m_Lst_Rubriques.SetSelectColor(CATPColors::GetColorSelect(CATPColors::COLORREPITEMS));
	m_Lst_Items.SetSelectColor(CATPColors::GetColorSelect(CATPColors::COLORREPITEMS));
	m_Lst_Rubrique_Dest.SetSelectColor(CATPColors::GetColorSelect(CATPColors::COLORREPITEMS));
	
	// Bouton validation
	m_BtnOK.		SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(CATPColors::COLORREPITEMS), CATPColors::COLORREPITEMS,0);
	m_BtnOK.		SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(CATPColors::COLORREPITEMS), CATPColors::COLORREPITEMS);
	m_BtnOK.		SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORREPITEMS), CATPColors::COLORREPITEMS);

}

/////////////////////////////////////////////////////////////////////////////////////
// Relations et positionnements des contr�les
JVoid CDlgClassementItems::SetWindowRelations()
{
	// Set all widow relation positions
	static CResizer::CBorderInfo s_bi[] = {

	{IDC_RI_STATIC_BOX,			{CResizer::eFixed,			IDC_MAIN, CResizer::eLeft},					//Left side
								{CResizer::eFixed,			IDC_MAIN, CResizer::eTop},					//Top side
								{CResizer::eFixed,			IDC_MAIN, CResizer::eRight},				//Right side
								{CResizer::eFixed,			IDC_MAIN, CResizer::eBottom}},				//Bottom side

	{IDC_RI_LABEL_RUBR_SRC,		{CResizer::eFixed,			IDC_RI_STATIC_BOX, CResizer::eLeft},		//Left side
								{CResizer::eFixed,			IDC_RI_STATIC_BOX, CResizer::eTop},			//Top side
								{CResizer::eFixed,			IDC_RI_STATIC_BOX, CResizer::eLeft},		//Right side
								{CResizer::eFixed,			IDC_RI_STATIC_BOX, CResizer::eTop}},		//Bottom side

	{IDC_RI_LIST_RUBRIQUES,		{CResizer::eFixed,			IDC_RI_STATIC_BOX, CResizer::eLeft},		//Left side
								{CResizer::eFixed,			IDC_RI_STATIC_BOX, CResizer::eTop},			//Top side
								{CResizer::eFixed,			IDC_RI_STATIC_BOX, CResizer::eLeft},		//Right side
								{CResizer::eFixed,			IDC_RI_STATIC_BOX, CResizer::eBottom}},		//Bottom side

	{IDC_RI_LABEL_ITEMS,		{CResizer::eFixed,			IDC_RI_LIST_RUBRIQUES, CResizer::eRight},	//Left side
								{CResizer::eFixed,			IDC_RI_STATIC_BOX, CResizer::eTop},			//Top side
								{CResizer::eFixed,			IDC_RI_STATIC_BOX, CResizer::eRight},		//Right side
								{CResizer::eFixed,			IDC_RI_STATIC_BOX, CResizer::eTop}},		//Bottom side

	{IDC_RI_LIST_ITEMS,			{CResizer::eFixed,			IDC_RI_LIST_RUBRIQUES, CResizer::eRight},	//Left side
								{CResizer::eFixed,			IDC_RI_STATIC_BOX, CResizer::eTop},			//Top side
								{CResizer::eFixed,			IDC_RI_STATIC_BOX, CResizer::eRight},		//Right side
								{CResizer::eFixed,			IDC_RI_STATIC_BOX, CResizer::eBottom}},		//Bottom side
	
	{IDC_RI_BUT_TRANSFERT_ITEMS,{CResizer::eFixed,			IDC_RI_LIST_ITEMS, CResizer::eRight},		//Left side
								{CResizer::eFixed,			IDC_RI_STATIC_BOX, CResizer::eTop},			//Top side
								{CResizer::eFixed,			IDC_RI_LIST_ITEMS, CResizer::eRight},		//Right side
								{CResizer::eFixed,			IDC_RI_STATIC_BOX, CResizer::eTop}},		//Bottom side

	{IDC_RI_LABEL_RUBR_DST,		{CResizer::eFixed,			IDC_RI_STATIC_BOX, CResizer::eRight},		//Left side
								{CResizer::eFixed,			IDC_RI_STATIC_BOX, CResizer::eTop},			//Top side
								{CResizer::eFixed,			IDC_RI_STATIC_BOX, CResizer::eRight},		//Right side
								{CResizer::eFixed,			IDC_RI_STATIC_BOX, CResizer::eTop}},		//Bottom side

	{IDC_RI_LIST_RUBR_DEST,		{CResizer::eFixed,			IDC_RI_STATIC_BOX, CResizer::eRight},		//Left side
								{CResizer::eFixed,			IDC_RI_STATIC_BOX, CResizer::eTop},			//Top side
								{CResizer::eFixed,			IDC_RI_STATIC_BOX, CResizer::eRight},		//Right side
								{CResizer::eFixed,			IDC_RI_STATIC_BOX, CResizer::eBottom}},		//Bottom side

	{IDOK,						{CResizer::eFixed,			IDC_RI_STATIC_BOX, CResizer::eRight},		//Left side
								{CResizer::eFixed,			IDC_RI_STATIC_BOX, CResizer::eBottom},		//Top side
								{CResizer::eFixed,			IDC_RI_STATIC_BOX, CResizer::eRight},		//Right side
								{CResizer::eFixed,			IDC_RI_STATIC_BOX, CResizer::eBottom}},		//Bottom side
	};

	const int nSize = sizeof(s_bi)/sizeof(s_bi[0]);
	m_Resizer.Init(m_hWnd, NULL, s_bi, nSize);
}

/////////////////////////////////////////////////////////////////////////////////////
// Retaille la fen�tre de dialogue
JVoid CDlgClassementItems::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// Gestion du resizer
	m_Resizer.Move();

	// Mise � jour 
	Invalidate();
	UpdateWindow();
}

/////////////////////////////////////////////////////////////////////////////////////
// Init tableau des items en cours
JVoid CDlgClassementItems::LoadItems()
{
	// Init map des items
	m_MapItems.Reset();

	// Ici c'est le traitement des rubriques items
	if (m_pTreeRubrique->m_Type_Rubrique == CRubriqueItemTree::TYPE_RUBRIQUE_ITEM)
	{
		// Chargement des items existants
		JList<IItem const *> vItems;
		// m_pDlgItems->GetItems()->GetItemsIndependant(vItems,m_pDlgItems->GetTerrain());
		m_pItemsConstruits->GetItemsIndependant(vItems,m_pTerrain);

		// Get only those valid for this terrain
		for (vItems.MoveFirst(); vItems.IsValid(); vItems.MoveNext())
		{
			// Ident de l'item
			JInt32 IdItem		= vItems.GetItem()->GetID();

			// Label de l'item
			JLabel LabelItem	= vItems.GetItem()->GetLabel(); 

			// Ajoute infos items
			m_MapItems.Add(IdItem) = LabelItem.AsJCharPtr();   
		}
	}

	// Ici c'est le traitement des rubriques palettes
	else if (m_pTreeRubrique->m_Type_Rubrique == CRubriqueItemTree::TYPE_RUBRIQUE_PALETTE)
	{
		// Chargement des palettes existantes
		JList<IItem const *> vPalettes;
		// m_pDlgItems->GetItems()->GetPalettes(vPalettes, m_pDlgItems->GetTerrain());
		m_pItemsConstruits->GetPalettes(vPalettes, m_pTerrain);

		// Get only those valid for this terrain
		for (vPalettes.MoveFirst(); vPalettes.IsValid(); vPalettes.MoveNext())
		{
			// Ident de la palette
			JInt32 IdPalette	= vPalettes.GetItem()->GetID();

			// Label de la palette
			JLabel LabelPalette	= vPalettes.GetItem()->GetLabel(); 

			// Ajoute infos palettes
			m_MapItems.Add(IdPalette) = LabelPalette.AsJCharPtr();   
		}
	}
	/* Voir plus tard si les autres objets seront class�s par rubriques
	/  soit les classes, quantiles, questions induites 
	else if ()
	*/ 
}

/////////////////////////////////////////////////////////////////////////////////////
// Remplir liste des rubriques sources
JVoid CDlgClassementItems::RecupListRubriqueSrc()
{
	// Init Liste rubriques Sources
	m_Lst_Rubriques.ResetContent(); 

	if (m_pTreeRubrique)
	{
		// Balaye map des rubriques
		for (m_pTreeRubrique->m_Map_RubriquesItems.MoveFirst();
			 m_pTreeRubrique->m_Map_RubriquesItems.IsValid();
			 m_pTreeRubrique->m_Map_RubriquesItems.MoveNext())
		{
			// Recup Texte
			CString TxtRubrique;
			TxtRubrique = m_pTreeRubrique->m_Map_RubriquesItems.GetKey();

			// Ajout Texte Rubrique dans liste des rubrique sources
			m_Lst_Rubriques.AddString(TxtRubrique); 
		}
	}

	/*
	// Balaye map 
	for (m_pDlgItems->m_Tree_Rubrique_Items.m_Map_RubriquesItems.MoveFirst();
		 m_pDlgItems->m_Tree_Rubrique_Items.m_Map_RubriquesItems.IsValid();
		 m_pDlgItems->m_Tree_Rubrique_Items.m_Map_RubriquesItems.MoveNext())
	{
		// Recup Texte
		CString TxtRubrique;
		TxtRubrique = m_pDlgItems->m_Tree_Rubrique_Items.m_Map_RubriquesItems.GetKey();

		// Ajout Texte Rubrique dans liste des rubrique sources
		m_Lst_Rubriques.AddString(TxtRubrique); 
	}
	*/
}

/////////////////////////////////////////////////////////////////////////////////////
// Remplir liste des rubriques sources
JVoid CDlgClassementItems::RecupListRubriqueDst()
{
	// Init Liste rubriques destinations
	m_Lst_Rubrique_Dest.ResetContent(); 

	// M�me liste que la liste source
	for (int InxLst = 0; InxLst < m_Lst_Rubriques.GetCount(); InxLst++) 
	{
		// Recup Texte
		CString TxtRubrique;
		m_Lst_Rubriques.GetText(InxLst,TxtRubrique);

		// Ajout Texte Rubrique dans liste des rubrique sources
		m_Lst_Rubrique_Dest.AddString(TxtRubrique); 
	}
}

/////////////////////////////////////////////////////////////////////////////////////
// Affiche items des rubriques sources s�lectionn�es
JVoid CDlgClassementItems::AffichItemRubriqueSrc()
{

	// Init liste des items
	m_Lst_Items.ResetContent();

	if (m_pTreeRubrique)
	{
		// Balaye liste des rubriques
		for (int InxLst = 0; InxLst < m_Lst_Rubriques.GetCount(); InxLst++)
		{
			// Voir si rubrique s�lectionn�e
			if (m_Lst_Rubriques.GetSel(InxLst))
			{
				// R�cup texte rubrique
				CString TxtRubrique;
				m_Lst_Rubriques.GetText(InxLst,TxtRubrique); 

				// On se positionne sur map <rubrique, lst items>
				m_pTreeRubrique->m_Map_RubriquesItems.MoveTo(TxtRubrique);
				if (m_pTreeRubrique->m_Map_RubriquesItems.IsValid())
				{
					// Liste des items de cette rubrqiue
					JList <JInt32> & LstItems = m_pTreeRubrique->m_Map_RubriquesItems.GetItem(); 

					for (LstItems.MoveFirst(); LstItems.IsValid(); LstItems.MoveNext())
					{
						// Ident item 
						JInt32 IdItem = LstItems.GetItem(); 

						// On se positionne sur le map infos item
						m_MapItems.MoveTo(IdItem);
						if (m_MapItems.IsValid())
						{
							// Puis ajoute �lmt list dans liste des items � transf�rer
							JUnt32 IndexCur = m_Lst_Items.AddString(m_MapItems.GetItem()); 
							m_Lst_Items.SetItemData(IndexCur,IdItem); 
						}
					}
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////
// Modification s�lection des rubriques sources
JVoid CDlgClassementItems::OnLbnSelchangeRiListRubriques()
{
	// R�affichage des items en cours de transfert
	AffichItemRubriqueSrc();
}

//////////////////////////////////////////////////////////////////////////////////
// Transfert des items
JVoid CDlgClassementItems::OnBnClickedRiButTransfertItems()
{
	if (m_pTreeRubrique)
	{
		// On v�rifie que des items ont �t� s�lectionn�s et qu'une rubrique dest � �t� choisi
		if (m_Lst_Items.GetSelCount() > 0 && m_Lst_Rubrique_Dest.GetCurSel() >= 0)
		{
			// R�cup�re la rubrique pour le transfert
			JUnt32 IdRubriqueDst = m_Lst_Rubrique_Dest.GetCurSel(); 
			CString TxtRubriqueDst;
			m_Lst_Rubrique_Dest.GetText(IdRubriqueDst, TxtRubriqueDst);

			// Balaye liste des rubriques sources
			for (int InxLst = 0; InxLst < m_Lst_Rubriques.GetCount(); InxLst++)
			{
				if (m_Lst_Items.GetSelCount())
				{
					// Voir si rubrique s�lectionn�e
					if (m_Lst_Rubriques.GetSel(InxLst))
					{
						// R�cup texte rubrique
						CString TxtRubriqueSrc;
						m_Lst_Rubriques.GetText(InxLst,TxtRubriqueSrc); 

						// On se positionne sur map <rubrique, lst items>
						/*
						m_pDlgItems->m_Tree_Rubrique_Items.m_Map_RubriquesItems.MoveTo(TxtRubriqueSrc);
						if (m_pDlgItems->m_Tree_Rubrique_Items.m_Map_RubriquesItems.IsValid())
						{
							// Liste des items de cette rubrqiue
							JList <JInt32> & LstItems = m_pDlgItems->m_Tree_Rubrique_Items.m_Map_RubriquesItems.GetItem(); 
						*/
						m_pTreeRubrique->m_Map_RubriquesItems.MoveTo(TxtRubriqueSrc);
						if (m_pTreeRubrique->m_Map_RubriquesItems.IsValid())
						{
							// Liste des items de cette rubrqiue
							JList <JInt32> & LstItems = m_pTreeRubrique->m_Map_RubriquesItems.GetItem(); 

							// Voir s'il est dans les �lmts � transf�rer
							for (int InxItem = 0; InxItem < m_Lst_Items.GetCount(); InxItem++)
							{
								// V�rifie uniquement les �lmts items � transf�rer
								if (m_Lst_Items.GetSel(InxItem))
								{
									// Ident item � transf�rer
									JUnt32 IdItemTransfert = static_cast<JUnt32>(m_Lst_Items.GetItemData(InxItem));

									for (LstItems.MoveFirst(); LstItems.IsValid(); LstItems.MoveNext())
									{
										// Ident item source
										JInt32 IdItem = LstItems.GetItem(); 

										if (IdItem == IdItemTransfert)
										{
											// Permuter l'item de la rubrique source vers la rubrique destination
											if (PermutItemRubrique(TxtRubriqueSrc, TxtRubriqueDst, IdItem))
											{	
												// Puis on d�selectionne l'item des �lmts � transf�rer
												m_Lst_Items.SetSel(InxItem,false); 
											}

											// Attention
											JBool OkTransf = true;
											break;
										}
									}
								}
							}
						}
					}
				}
			}

			// Mise � jour liste items de transfert en cours
			UpdateItemTransfert();
		}
		else
		{
			// Transfert impossible, pas d'items s�lectionn�s et/ou ni de rubrique destination
			CString Message;
			Message.LoadString(IDS_RI_MESS_RUBRIQUE1); 
			AfxMessageBox(Message, MB_ICONINFORMATION);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////
// Permuter item d'une rubrique � l'autre
JBool CDlgClassementItems::PermutItemRubrique(CString TxtRubriqueSrc, CString TxtRubriqueDst, JUnt32 IdItem)
{
	if (m_pTreeRubrique)
	{
		// Pas la peine si c'est les 2 m�mes rubriques
		if (TxtRubriqueSrc != TxtRubriqueDst)
		{
			// R�cup�re list des items de la rubrique source
			m_pTreeRubrique->m_Map_RubriquesItems.MoveTo(TxtRubriqueSrc);
			if (m_pTreeRubrique->m_Map_RubriquesItems.IsValid())
			{
				// Liste des items en cours sur la rubrique source
				JList <JInt32> & LstItemSrc = m_pTreeRubrique->m_Map_RubriquesItems.GetItem(); 

				m_pTreeRubrique->m_Map_RubriquesItems.MoveTo(TxtRubriqueDst);
				if (m_pTreeRubrique->m_Map_RubriquesItems.IsValid())
				{
					// Liste des items en cours sur la rubrique destination
					JList <JInt32> & LstItemDst = m_pTreeRubrique->m_Map_RubriquesItems.GetItem(); 

					// Vire l'item de la rubrique source
					for (LstItemSrc.MoveFirst(); LstItemSrc.IsValid(); LstItemSrc.MoveNext())
					{
						if (LstItemSrc.GetItem() == IdItem)
						{
							LstItemSrc.Remove();
							break;
						}
					}

					// L'ajoute � la rubrique destination
					LstItemDst.AddTail() = IdItem;

					return true;
				}
				return false;
			}
			else 
				return false;
		}
		
		// Source et destination identiques, on consid�re la permutation comme effectu�e
		return true;
	}
	else
		// Probl�me, identifiant nomenclature rubriques inexistant
		return false;

}

//////////////////////////////////////////////////////////////////////////////////////
// Mise � jour liste items de transfert en cours
JVoid CDlgClassementItems::UpdateItemTransfert()
{
	// R�actualise la liste des items � transf�rer
	AffichItemRubriqueSrc();
	
	// D�s�lectionne la rubrique destination
	RecupListRubriqueDst();
}
