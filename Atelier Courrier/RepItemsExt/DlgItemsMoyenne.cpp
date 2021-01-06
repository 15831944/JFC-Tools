// DlgItemsMoyenne.cpp : implementation file
//
// Dialogue permettant la s�lection d'un ou plusieurs items quantitatifs qui
// seront moyenn�s soit en mode ligne , soit en mode tableau dans les tris crois�s
//
#include "stdafx.h"
#include "DlgTableau.h"
#include "DlgAnalyseur.h"
#include "DlgItemsMoyenne.h"
#include "TerrainManager.h"
#include "AnalyseurDoc.h"
#include "IBClass.h"
#include "IBClass.h"
using namespace std;

IMPLEMENT_DYNAMIC(CDlgItemsMoyenne, CDialog)

// Constructeur via boite tableau
CDlgItemsMoyenne::CDlgItemsMoyenne(CDlgTableau * pParent, JList <JUnt32> *pLstItemMoyAuto, JList <JUnt32> *pLstItemMoyUtil): CDialog(CDlgItemsMoyenne::IDD, (CWnd*)pParent)
{
	// Init fen�tres parent
	m_pDlgTableau = pParent;
	m_pDlgAnalyseur = 0;

	// R�cup liste s�lection � retourner
	m_pLstSelItemAuto = pLstItemMoyAuto;
	m_pLstSelItemUtil = pLstItemMoyUtil;

	// Init liste des s�lections
	m_pLstSelItemAuto->Reset();
	m_pLstSelItemUtil->Reset();

	// Init map des questions quantis
	m_MapQuestionsQuantis.Reset();
}

// Constructeur via boite analyseur
CDlgItemsMoyenne::CDlgItemsMoyenne(CDlgAnalyseur * pParent, JList <JUnt32> *pLstItemMoyAuto, JList <JUnt32> *pLstItemMoyUtil): CDialog(CDlgItemsMoyenne::IDD, (CWnd*)pParent)
{
	// Init fen�tres parent
	m_pDlgAnalyseur = pParent;
	m_pDlgTableau = 0;

	// R�cup liste s�lection � retourner
	m_pLstSelItemAuto = pLstItemMoyAuto;
	m_pLstSelItemUtil = pLstItemMoyUtil;

	// Init liste des s�lections
	m_pLstSelItemAuto->Reset();
	m_pLstSelItemUtil->Reset();

	// Init map des questions quantis
	m_MapQuestionsQuantis.Reset();
}	

// Destructeur
CDlgItemsMoyenne::~CDlgItemsMoyenne()
{
}

// Initialisation boite de dialogue
BOOL CDlgItemsMoyenne::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Titre de la fen�tre
	this->SetWindowText("S�lection de(s) item(s) � moyenner");

	// Fond de la boite
	m_FondBox.SubclassDlgItem(IDC_RI_STATIC_BOX,this);

	// Template system de base
	const CXTemplate & systTemplate = CXTemplateManager::GetInstance()->GetTemplate(CXTemplateManager::GetSystemTemplate());

	// D�finition template pour listes elmts quantitatifs
	m_TemplateQuanti = CXTemplateManager::GetInstance()->CreateTemplate(systTemplate.textcolor_, 
																		systTemplate.backcolor_,
																		systTemplate.seltextcolor_,
																		CATPColors::GetColorSelect(CATPColors::COLORANALYSEUR),
																		false,true,false); 

	// D�finition des controles
	m_Lbl_Titre1.			SubclassDlgItem(IDC_RI_LABEL1, this);
	m_Lst_Items_Moy_Auto.	SubclassDlgItem(IDC_RI_LIST_ITEMSMOY_AUTO,this);
	m_Tree_Quantis.			SubclassDlgItem(IDC_RI_TREE_QUANTI,this);
	m_All_Quanti_Auto.		SubclassDlgItem(IDC_RI_ALL_QUANTI_AUTO,this);
	m_Lbl_Titre2.			SubclassDlgItem(IDC_RI_LABEL2, this);
	m_Lst_Items_Moy_Util.	SubclassDlgItem(IDC_RI_LIST_ITEMSMOY_UTIL,this);
	m_All_Quanti_Util.		SubclassDlgItem(IDC_RI_ALL_QUANTI_UTIL,this);
	m_Btn_OK.				SubclassDlgItem(IDOK, this);
	m_Btn_Cancel.			SubclassDlgItem(IDCANCEL, this);

	// Titre liste des items
	m_Lbl_Titre1.SetWindowText("Liste des questions moyennables");
	m_Lbl_Titre2.SetWindowText("Liste des items utilisateurs moyennables");

	// Parametrage Libell� entetes
	m_Lbl_Titre1.SetFontName(CATPColors::GetFontLabelName());
	m_Lbl_Titre1.SetFontSize(CATPColors::GetFontLabelSize());
	m_Lbl_Titre1.SetBkColor(CATPColors::GetColorDark(m_CATPColorsApp),m_CATPColorsApp, CATPColors::DARK_DEGRAD);
	m_Lbl_Titre1.SetBorder(false);
	m_Lbl_Titre1.SetTextColor(CATPColors::GetColorSuperDark(m_CATPColorsApp)); 

	m_Lbl_Titre2.SetFontName(CATPColors::GetFontLabelName());
	m_Lbl_Titre2.SetFontSize(CATPColors::GetFontLabelSize());
	m_Lbl_Titre2.SetBkColor(CATPColors::GetColorDark(m_CATPColorsApp),m_CATPColorsApp, CATPColors::DARK_DEGRAD);
	m_Lbl_Titre2.SetBorder(false);
	m_Lbl_Titre2.SetTextColor(CATPColors::GetColorSuperDark(m_CATPColorsApp)); 

	// Coloration et style fond de boite
	m_FondBox.SetBkColor(CATPColors::GetColorMedium(m_CATPColorsApp),m_CATPColorsApp, CATPColors::MEDIUM_DEGRAD);
	m_FondBox.SetTransparent(false);
	m_FondBox.SetSunken(true);

	m_All_Quanti_Auto.SetColor(CButtonST::BTNST_COLOR_BK_IN,CATPColors::GetColorLight(m_CATPColorsApp),m_CATPColorsApp,0);
	m_All_Quanti_Auto.SetColor(CButtonST::BTNST_COLOR_BK_OUT,CATPColors::GetColorMedium(m_CATPColorsApp),m_CATPColorsApp,0);
	m_All_Quanti_Auto.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,CATPColors::GetColorDark(m_CATPColorsApp),m_CATPColorsApp);

	m_All_Quanti_Util.SetColor(CButtonST::BTNST_COLOR_BK_IN,CATPColors::GetColorLight(m_CATPColorsApp),m_CATPColorsApp,0);
	m_All_Quanti_Util.SetColor(CButtonST::BTNST_COLOR_BK_OUT,CATPColors::GetColorMedium(m_CATPColorsApp),m_CATPColorsApp,0);
	m_All_Quanti_Util.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,CATPColors::GetColorDark(m_CATPColorsApp),m_CATPColorsApp);

	// Les boites �dition
	m_FondBox.AddTopWindow(m_Lbl_Titre1);
	m_FondBox.AddTopWindow(m_Lst_Items_Moy_Auto);
	m_FondBox.AddTopWindow(m_All_Quanti_Auto);
	m_FondBox.AddTopWindow(m_Tree_Quantis);
	m_FondBox.AddTopWindow(m_Lbl_Titre2);
	m_FondBox.AddTopWindow(m_Lst_Items_Moy_Util);
	m_FondBox.AddTopWindow(m_All_Quanti_Util);

	// Couleur s�lection liste
	// m_Lst_Items_Moy_Auto.SetSelectColor(CATPColors::GetColorSelect(CATPColors::COLORREPITEMS));
	// m_Lst_Items_Moy_Util.SetSelectColor(CATPColors::GetColorSelect(CATPColors::COLORREPITEMS));

	// Param�trage liste tree des quantis
	m_Tree_Quantis.			m_CATPColorsApp = m_CATPColorsApp;
	m_Tree_Quantis.			SetColors();

	// Initialisation de la s�lection index th�matique des elmts quantis
	// m_Tree_Quantis.			ResetMapTree();

	// Set the theme tree data quantis
	SetThemeDataQuanti(true);
	
	// Chargement des listes d'items utilisateurs
	LoadItemMoyUtil();

	// On se repositionne sur la cascade du tree des questions quantis (sans les s�lections)
	m_Tree_Quantis.TreeViaSauvegarde(false);

	/* A FINIR
	// Repositionne l'index th�matique via deni�re s�lection
	m_Tree_Quantis.TreeViaSauvegarde();
	m_Tree_Quantis.UpdateQuestionList(); 
	m_Tree_Quantis.AffichPreselectQu();

	// UpdateListItems_MoyAuto();
	Updated_ItemQuantiList(true);
	*/

	return TRUE;
}


// Mise en place de l'arborescence des questions quantitatives
void CDlgItemsMoyenne::SetThemeDataQuanti(JBool UpdateNow)
{
	// Positionne les datas
	CString QFilter = "";

	// Le terrain en cours
	if (m_pDlgTableau)
	{
		CTerrain *pTerrain = CTerrainManager::GetInstance()->GetTerrain(m_pDlgTableau->GetIdSource(),m_pDlgTableau->GetIdTerrain());

		// Repositionne �tat du tree items � moyenner
		m_Tree_Quantis.SetParent(this);
		m_Tree_Quantis.SetData(pTerrain, m_pDlgTableau->GetItems(),QFilter,0, UpdateNow);
		m_Tree_Quantis.SetEtatData(m_pDlgTableau->GetDoc()->GetEtatTreeQuanti());

	}
	else if (m_pDlgAnalyseur)
	{
		CTerrain * pTerrain = m_pDlgAnalyseur->m_pDoc->Get_TerrainTemoin();

		// Repositionne �tat du tree items � moyenner
		m_Tree_Quantis.SetParent(this);
		m_Tree_Quantis.SetData(pTerrain, CItemsConstruitsManager::GetInstance()->GetItemsInstance(),QFilter,0, UpdateNow);
		m_Tree_Quantis.SetEtatData(m_pDlgAnalyseur->m_pDoc->GetEtatTreeQuanti());
	}
	
	// Repositionne l'index th�matique via derni�re s�lection  (� voir)
	// m_Tree_Quantis.TreeViaSauvegarde();
}

void CDlgItemsMoyenne::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgItemsMoyenne, CDialog)
	ON_BN_CLICKED(IDOK,							&CDlgItemsMoyenne::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL,						&CDlgItemsMoyenne::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_RI_ALL_QUANTI_AUTO,		&CDlgItemsMoyenne::OnItemsQuantiAuto_Tous)
	ON_BN_CLICKED(IDC_RI_ALL_QUANTI_UTIL,		&CDlgItemsMoyenne::OnItemsQuantiUtil_Tous)

	ON_MESSAGE(WM_USER_ITEMQUANTILISTUPDATED,	OnUpdated_ItemQuantiList)
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////////////////////
// Synchronisation de la liste des items quantis � moyenner et du map des questions associ�es
LRESULT CDlgItemsMoyenne::OnUpdated_ItemQuantiList(WPARAM wParam, LPARAM lParam)
{
	// Update synchronisation
	JBool  AjoutQu		= lParam;
	Updated_ItemQuantiList(AjoutQu);

	/* A VIRER
	// Le terrain en cours
	CTerrain * pTerrain = NULL;
	if (m_pDlgTableau)
	{
		pTerrain = CTerrainManager::GetInstance()->GetTerrain(m_pDlgTableau->GetIdSource(),m_pDlgTableau->GetIdTerrain());
	}
	else if (m_pDlgAnalyseur)
	{
		pTerrain = m_pDlgAnalyseur->m_pDoc->Get_TerrainTemoin();
	}

	// R�cup liste des question � mettre � jour via la s�lection dans le tree questions quantis
	const JList <JInt32> * pLstIdQuestion = m_Tree_Quantis.GetLstQuestions();
	JBool  AjoutQu		= lParam;

	if (pLstIdQuestion)
	{
		// Attente mise � jour
		m_Lst_Items_Moy_Auto.SetRedraw(false);

		// Boucle sur toutes les questions � mettre � jour
		for (pLstIdQuestion->MoveFirst(); pLstIdQuestion->IsValid(); pLstIdQuestion->MoveNext())
		{
			// Question � ajouter
			JInt32 IdQuestion	= pLstIdQuestion->GetItem();

			// R�cup la question associ�
			IQuestion const * pQuestion = pTerrain->GetQuestionByID(IdQuestion);

			if (pQuestion)
			{
				// Mise � jour de la liste
				m_MapQuestionsQuantis.MoveTo(IdQuestion);
				if (AjoutQu)
				{
					// Ajoute une question quantitative
					if (!m_MapQuestionsQuantis.IsValid())
					{
						// Ajoute la question au map des questions en cours
						m_MapQuestionsQuantis.Add(IdQuestion);

						// Update map de s�lections des items quantis
						UpdateMapItemsQuantis(pQuestion, AjoutQu);

						// Cr�er les strings associ�s et mise � jour du map des string items num�ris�s
						UpdateListItems_MoyAuto();
					}
				}
				else
				{
					// Supprime une question quantitative
					if (m_MapQuestionsQuantis.IsValid())
					{
						// Mise � jour du map des questions
						m_MapQuestionsQuantis.Remove();

						// Update map de s�lections des items quantis
						UpdateMapItemsQuantis(pQuestion, AjoutQu);

						// Cr�er les strings associ�s et mise � jour du map des string items num�ris�s
						UpdateListItems_MoyAuto();
					}
				}
			}
		}

		// Effectue mise � jour
		m_Lst_Items_Moy_Auto.SetRedraw(true);
	}
	*/
	
	return 0;
}

void CDlgItemsMoyenne::Updated_ItemQuantiList(JBool AjoutQu)
{
	// Le terrain en cours
	CTerrain * pTerrain = NULL;
	if (m_pDlgTableau)
	{
		pTerrain = CTerrainManager::GetInstance()->GetTerrain(m_pDlgTableau->GetIdSource(),m_pDlgTableau->GetIdTerrain());
	}
	else if (m_pDlgAnalyseur)
	{
		pTerrain = m_pDlgAnalyseur->m_pDoc->Get_TerrainTemoin();
	}

	// R�cup liste des question � mettre � jour via la s�lection dans le tree questions quantis
	const JList <JInt32> * pLstIdQuestion = m_Tree_Quantis.GetLstQuestions();
	if (pLstIdQuestion)
	{
		// Attente mise � jour
		m_Lst_Items_Moy_Auto.SetRedraw(false);

		// Boucle sur toutes les questions � mettre � jour
		for (pLstIdQuestion->MoveFirst(); pLstIdQuestion->IsValid(); pLstIdQuestion->MoveNext())
		{
			// Question � ajouter
			JInt32 IdQuestion	= pLstIdQuestion->GetItem();

			// R�cup la question associ�
			IQuestion const * pQuestion = pTerrain->GetQuestionByID(IdQuestion);

			if (pQuestion)
			{
				// Mise � jour de la liste
				m_MapQuestionsQuantis.MoveTo(IdQuestion);
				if (AjoutQu)
				{
					// Ajoute une question quantitative
					if (!m_MapQuestionsQuantis.IsValid())
					{
						// Ajoute la question au map des questions en cours
						int NbQuanti = m_MapQuestionsQuantis.GetCount();

						m_MapQuestionsQuantis.Add(IdQuestion);

						// Update map de s�lections des items quantis
						UpdateMapItemsQuantis(pQuestion, AjoutQu);

						// Cr�er les strings associ�s et mise � jour du map des string items num�ris�s
						UpdateListItems_MoyAuto();
					}
				}
				else
				{
					// Supprime une question quantitative
					if (m_MapQuestionsQuantis.IsValid())
					{
						// Mise � jour du map des questions
						m_MapQuestionsQuantis.Remove();

						// Update map de s�lections des items quantis
						UpdateMapItemsQuantis(pQuestion, AjoutQu);

						// Cr�er les strings associ�s et mise � jour du map des string items num�ris�s
						UpdateListItems_MoyAuto();
					}
				}
			}
		}

		// Effectue mise � jour
		m_Lst_Items_Moy_Auto.SetRedraw(true);
	}
}

///////////////////////////////////////////////////////////////////////////////////////
// Update map de s�lections des items quantis
void CDlgItemsMoyenne::UpdateMapItemsQuantis(const IQuestion *pQuestion, JBool AjoutQu)
{
	// Ident de la question
	JInt32 IdQuestion = pQuestion->GetID();

	if (AjoutQu)
	{
		// Nombre de dimensions de la question
		JInt32 NbDims = pQuestion->HasDimensions();

		// D�finition de chaque items quantitaifs possible de la question
		CKeyItemQuanti KeyItemQuanti;
		KeyItemQuanti.m_IdQuestion	= IdQuestion;
		KeyItemQuanti.m_NbDimension = NbDims;

		// Vecteur de modalit�
		JList< JVector<const IModalite *> >	DimList;
		JList< JInt32 >						DimType;

		// Boucle sur toutes les dimensions de la question
		JInt32 InxDim = 0;

		// Liste des modalit�s
		JVector <const IModalite *>  vDim1s;

		if (m_pDlgTableau)
			pQuestion->GetIModalites(vDim1s, 0, InxDim, m_pDlgTableau->GetContingentMask());
		else
			pQuestion->GetIModalites(vDim1s, 0, InxDim, 1);
				
		// r�cup le libell� de la 1ere modalit�
		CString TxtModalite;
		vDim1s.MoveFirst();
		if (vDim1s.IsValid())
		{
			// R�cup la modalit� associ�
			const IModalite* pModalite = vDim1s.GetItem();

			// Libell� de la modalit� 
			TxtModalite = pModalite->GetLabel().AsJCharPtr(); 

			// Cadre dimension 1 pour les questions quantitatives sans libell� � 1 seule modalit�
			if (vDim1s.GetCount() == 1)
			{
				if (TxtModalite == "Valeurs num�riques")
				{
					// Valeur modalit�
					JInt32 ValModa = pModalite->GetID();

					// Indique qu'il s'agit d'une question simple num�rique
					KeyItemQuanti.m_IdentDim1 = pModalite->GetID();
					KeyItemQuanti.m_IdentDim2 = 0;
					KeyItemQuanti.m_IdentDim3 = 0;
				}
				else
				{
					// Indique qu'il s'agit d'une question simple num�rique avec 1 seule modalit�
					KeyItemQuanti.m_IdentDim1 = pModalite->GetID();
				}

				if (NbDims == 1)
				{
					// Ajoute au map des items en cours
					m_MapItemsQuantis.MoveTo(KeyItemQuanti);
					if (!m_MapItemsQuantis.IsValid())
					{
						m_MapItemsQuantis.Add(KeyItemQuanti) = false;
					}
				}
			}
			else
			{
				// Si seulement 1 seul dimension
				if (NbDims == 1)
				{
					// Question avec plus d'une dimension
					for (vDim1s.MoveFirst(); vDim1s.IsValid(); vDim1s.MoveNext())
					{
						// R�cup la modalit� associ�
						const IModalite* pModalite = vDim1s.GetItem();

						// Pr�cise ident de la modalit� dimension 1
						KeyItemQuanti.m_IdentDim1 = pModalite->GetID();
						KeyItemQuanti.m_IdentDim2 = 0;
						KeyItemQuanti.m_IdentDim3 = 0;

						// Ajoute au map des items en cours
						m_MapItemsQuantis.MoveTo(KeyItemQuanti);
						if (!m_MapItemsQuantis.IsValid())
						{
							m_MapItemsQuantis.Add(KeyItemQuanti) = false;
						}
					}
				}
				else
				{
					// Passe � la dimension 2
					InxDim++;
					
					// Au moins 2 dimensions, on combine les modalit�s
					JVector <const IModalite *> vDim2s;

					if (m_pDlgTableau)
						pQuestion->GetIModalites(vDim2s, 0, InxDim, m_pDlgTableau->GetContingentMask());
					else
						pQuestion->GetIModalites(vDim2s, 0, InxDim, 1);

					// Question avec plus d'une dimension
					for (vDim1s.MoveFirst(); vDim1s.IsValid(); vDim1s.MoveNext())
					{
						// R�cup la modalit� associ�
						const IModalite* pModalite = vDim1s.GetItem();

						// Pr�cise ident de la modalit� dimension 1
						KeyItemQuanti.m_IdentDim1 = pModalite->GetID();
						KeyItemQuanti.m_IdentDim2 = 0;
						KeyItemQuanti.m_IdentDim3 = 0;

						// Si seulement 1 dimension, on stocke les infos ident modalit� 
						if (NbDims == 1)
						{
							// Ajoute au map des items en cours
							m_MapItemsQuantis.MoveTo(KeyItemQuanti);
							if (!m_MapItemsQuantis.IsValid())
							{
								m_MapItemsQuantis.Add(KeyItemQuanti) = false;
							}
						}
						else
						{
							

							// Question avec plus d'une dimension
							for (vDim2s.MoveFirst(); vDim2s.IsValid(); vDim2s.MoveNext())
							{
								// R�cup la modalit� associ� � la dimension 2
								const IModalite* pModalite = vDim2s.GetItem();

								// Pr�cise ident de la modalit� dimension 2
								KeyItemQuanti.m_IdentDim2 = pModalite->GetID();

								// Ajoute au map des items en cours
								m_MapItemsQuantis.MoveTo(KeyItemQuanti);
								if (!m_MapItemsQuantis.IsValid())
								{
									m_MapItemsQuantis.Add(KeyItemQuanti) = false;
								}
							}							
						}
					}
				}
			}
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Suppression de toutes les occurences avec l'ident question
	else
	{
		//// Boucle sur toutes les lignes d'items en cours
		m_MapItemsQuantis.MoveFirst();
		while (m_MapItemsQuantis.IsValid())
		{
			// R�cup info clef item quanti
			const CKeyItemQuanti & KeyItemQuanti = m_MapItemsQuantis.GetKey();

			// Enl�ve du map s'il s'agit de la question
			if (KeyItemQuanti.m_IdQuestion == IdQuestion)
			{
				// On le supprime
				m_MapItemsQuantis.Remove(1);

				// Si c'est le dernier on sort
				if (!m_MapItemsQuantis.IsValid())
					break;
			}
			else
			{
				// Passe au suivant
				m_MapItemsQuantis.MoveNext();
			}
		}
	}
}

// Cr�er les strings associ�s et mise � jour du map des string items num�ris�s
void CDlgItemsMoyenne::UpdateListItems_MoyAuto()
{
	// Le terrain en cours
	// CTerrain * pTerrain = CTerrainManager::GetInstance()->GetTerrain(m_pDlgTableau->GetIdSource(),m_pDlgTableau->GetIdTerrain());

	// Le terrain en cours
	CTerrain * pTerrain = NULL;
	if (m_pDlgTableau)
	{
		pTerrain = CTerrainManager::GetInstance()->GetTerrain(m_pDlgTableau->GetIdSource(),m_pDlgTableau->GetIdTerrain());
	}
	else if (m_pDlgAnalyseur)
	{
		pTerrain = m_pDlgAnalyseur->m_pDoc->Get_TerrainTemoin();
	}

	// Sauve d'abord tous les items pr�cedemment s�lectionn�s
	JMap <CKeyItemQuanti, bool> MapSelIdQuestion;
	for (int InxLst = 0; InxLst < m_Lst_Items_Moy_Auto.GetCount(); InxLst++)
	{
		if (m_Lst_Items_Moy_Auto.GetSel(InxLst))
		{
			// Ajoute la question si d�j� s�lectionn�e
			CKeyItemQuanti * pKeyItemQuanti = reinterpret_cast<CKeyItemQuanti*>(m_Lst_Items_Moy_Auto.GetItemData(InxLst));
			MapSelIdQuestion.MoveTo(*pKeyItemQuanti);
			if (!MapSelIdQuestion.IsValid())
				MapSelIdQuestion.Add(*pKeyItemQuanti) = true;
		}
	}		

	// Reset liste des items � s�lectionner
	m_Lst_Items_Moy_Auto.ResetContent();

	// Boucle sur toutes les lignes d'items en cours
	for (m_MapItemsQuantis.MoveFirst(); m_MapItemsQuantis.IsValid(); m_MapItemsQuantis.MoveNext())
	{	
		// R�cup info clef item quanti
		const CKeyItemQuanti & KeyItemQuanti = m_MapItemsQuantis.GetKey();

		// R�cup�re la question principale
		CQuestion const * pQuestion = (CQuestion *)pTerrain->GetQuestionByID(KeyItemQuanti.m_IdQuestion);
	
		if (pQuestion)
		{
			// Affichage ou pas de l'item
			JBool AffichItem = true;

			// Table des dimensions de l'item
			JInt32 Dims = pQuestion->HasDimensions();

			// Libell� global de l'item num�rique moyennable
			CString LibItem;

			if (Dims < 3)
			{
				// Boucle sur toutes les dimensions
				for(JInt32 x = 0; x < Dims ; x++)
				{
					// Question quanti � au moins 1 dimension
					if (x == 0)
					{
						// Traitement de la 1ere dimension de la question 
						if (KeyItemQuanti.m_IdentDim1 == 0)
						{
							// Ici il s'agit d'un item num�rique mono-dimension avec valeurs directes
							// LibItem.Format("%s (%d-%d-%d)",pQuestion->GetLabel().AsJCharPtr(), KeyItemQuanti.m_IdentDim1, KeyItemQuanti.m_IdentDim2, KeyItemQuanti.m_IdentDim3);  // pour test des idents
							LibItem.Format("%s",pQuestion->GetLabel().AsJCharPtr());
						}
						else
						{
							// R�cup le libell� de la modalit� dimension 1
							const IModalite * pModa = pQuestion->GetIModalite(KeyItemQuanti.m_IdentDim1, 0);

							// V�rifie si question quantitative simple
							CString TxtModaliteQu1 = pModa->GetLabel().AsJCharPtr(); 
							
							if (TxtModaliteQu1 == "Valeurs num�riques")
							{
								LibItem.Format("%s",pQuestion->GetLabel().AsJCharPtr());
							}
							else
							{
								// LibItem.Format("%s - %s (%d-%d-%d)",pQuestion->GetLabel().AsJCharPtr(), pModa->GetLabel().AsJCharPtr(), KeyItemQuanti.m_IdentDim1, KeyItemQuanti.m_IdentDim2, KeyItemQuanti.m_IdentDim3); // pour test des idents
								LibItem.Format("%s - %s", pQuestion->GetLabel().AsJCharPtr(), pModa->GetLabel().AsJCharPtr());
							}
						}	

						// Si seulement 1 dimension on arr�te
						if (Dims == 1) break;
					}
					
					// Question quanti � au moins 2 dimensions
					if (x == 1)
					{
						// R�cup le libell� de la modalit� dimension 1
						const IModalite * pModa = pQuestion->GetIModalite(KeyItemQuanti.m_IdentDim2, 0);
						CString TxtDim2 = pModa->GetLabel().AsJCharPtr();
					}

					// Question quanti � au moins 3 dimensions
					if (x == 2)
					{
						// A FAIRE si n�cessaire, pour le moment au del� de 2 dimensions on ne traite pas
						AffichItem = false;
					}
				}
			}
			else
			{
				// Pas d'affichage pour les items avec + de 2 dimensions
				AffichItem = false;
			}

			// Ajout dans liste
			if (AffichItem)
			{
				JInt32 Idx = m_Lst_Items_Moy_Auto.AddLine(LibItem, m_TemplateQuanti);
				// JInt32 Idx = m_Lst_Items_Moy_Auto.AddString(LibItem);
				m_Lst_Items_Moy_Auto.SetItemData(Idx, reinterpret_cast<DWORD_PTR>(&KeyItemQuanti));

				// Remet la s�lection pr�c�dente si d�j� s�lectionn�
				MapSelIdQuestion.MoveTo(KeyItemQuanti);
				if (MapSelIdQuestion.IsValid())
				{
					m_Lst_Items_Moy_Auto.SetSel(Idx,true);
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////
// Validation de la s�lection des items � moyenner 
void CDlgItemsMoyenne::OnBnClickedOk()
{
	// Update s�lection en cours des items num�ris�s automatiques
	GetItemsMoyAuto();

	// Sauve les items construits automatiques
	CItemsConstruitsManager::GetInstance()->SetMode(CItemsConstruitsManager::ModeAutomatique);
	CItemsConstruitsBase * pItemsConstruitsBase = CItemsConstruitsManager::GetInstance()->GetItemsInstance();

	if (pItemsConstruitsBase->IsDirty())
	{
		pItemsConstruitsBase->Save();
	}

	// Remet en mode items utilisateurs
	CItemsConstruitsManager::GetInstance()->SetMode(CItemsConstruitsManager::ModeUtilisateur);

	// Update s�lection en cours des items num�ris�s utilisateurs
	GetItemsMoyUtil();

	// On valide la s�lection
	OnOK();
}

///////////////////////////////////////////////////////////////////////////////////////
// Annulation de la s�lection
void CDlgItemsMoyenne::OnBnClickedCancel()
{
	// R�cup les items construits automatiques pour les virer
	CItemsConstruitsManager::GetInstance()->SetMode(CItemsConstruitsManager::ModeAutomatique);
	CItemsConstruitsBase * pItemsConstruitsAuto		= CItemsConstruitsManager::GetInstance()->GetItemsInstance();

	// On vide le fichier des items autos temporaires
	// A  FAIRE !!!!!

	// Puis on remet le mode utilisateur courant
	CItemsConstruitsManager::GetInstance()->SetMode(CItemsConstruitsManager::ModeUtilisateur);

	OnCancel();
}

///////////////////////////////////////////////////////////////////////////////////////
// Chargement des items moyennables utilisateurs
void CDlgItemsMoyenne::LoadItemMoyUtil()
{
	if (m_pDlgTableau)
	{
		// Vecteur des items � afficher
		JList<IItem const *> vItems;
		m_Lst_Items_Moy_Util.ResetContent(); 
		m_pDlgTableau->GetItems()->GetItemsIndependant(vItems,m_pDlgTableau->GetTerrain());

		///////////////////////////////////////////////////////////////////////////////////
		// Get only those valid for this terrain
		for (vItems.MoveFirst(); vItems.IsValid(); vItems.MoveNext())
		{
			// Ident de l'item
			JInt32 IdItem	= vItems.GetItem()->GetID();

			// Libell� de l'item
			const IItem *pItem = vItems.GetItem();
			CString LibItem = pItem->GetLabel().AsJCharPtr();

			// Si s�p @ existe , on le remplace par -
			LibItem.Replace("@","-");

			// On ne prend que les items moyennables	
			CItemConstruit *pItemConstruit = (CItemConstruit *)pItem;
			if (m_pDlgTableau->ItemQuantitatif(pItemConstruit, m_pDlgTableau->GetIdSource(), m_pDlgTableau->GetIdTerrain()))
			{
				// Ajoute libell� dans la liste et ident en tant qu'itemdata
				long InxLst = m_Lst_Items_Moy_Util.AddLine(LibItem, m_TemplateQuanti);
				// long InxLst = m_Lst_Items_Moy_Util.AddString(LibItem);
				m_Lst_Items_Moy_Util.SetItemData(InxLst,IdItem);
			}
		}
	}
	else if (m_pDlgAnalyseur)
	{
		// Vecteur des items � afficher
		JList<IItem const *> vItems;
		m_Lst_Items_Moy_Util.ResetContent(); 

		// IdSource et IdTerrain
		CTerrain * pTerrain = m_pDlgAnalyseur->m_pDoc->Get_TerrainTemoin();
		ASSERT(pTerrain);
			
		CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetItemsIndependant(vItems, pTerrain);

		// Get only those valid for this terrain
		for (vItems.MoveFirst(); vItems.IsValid(); vItems.MoveNext())
		{
			// Ident de l'item
			JInt32 IdItem	= vItems.GetItem()->GetID();

			// Libell� de l'item
			const IItem *pItem = vItems.GetItem();
			CString LibItem = pItem->GetLabel().AsJCharPtr();

			// On ne prend que les items moyennables	
			CItemConstruit *pItemConstruit = (CItemConstruit *)pItem;
			if (ItemQuantitatif(pItemConstruit, m_pDlgAnalyseur->m_pDoc->Get_TerrainTemoin()))
			{
				// Ajoute libell� dans la liste et ident en tant qu'itemdata
				long InxLst = m_Lst_Items_Moy_Util.AddLine(LibItem, m_TemplateQuanti);
				m_Lst_Items_Moy_Util.SetItemData(InxLst,IdItem);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////
// // Renvoi liste des items auto s�lectionn�s
void CDlgItemsMoyenne::GetItemsMoyAuto()
{
	if (m_pLstSelItemAuto)
	{
		// Init liste des elmts s�lectionn�s
		m_pLstSelItemAuto->Reset();

		// A t-on s�lectionn� des items quatitatifs automatique
		if (m_Lst_Items_Moy_Auto.GetCount())
		{
			// On revient sur me mode items construits utilisateurs
			CItemsConstruitsManager::GetInstance()->SetMode(CItemsConstruitsManager::ModeAutomatique);

			// R�cup les �lmts de la liste s�lectionn�e pour les moyennes
			int NbSel = m_Lst_Items_Moy_Auto.GetSelCount();
			int  *pSel = new int[NbSel];
			m_Lst_Items_Moy_Auto.GetSelItems(NbSel,pSel); 

			// Boucle sur les items s�lectionn�s
			for(int i = 0; i < NbSel; ++i)
			{
				// R�cup�re les param�tres de l'item � cr�er
				CKeyItemQuanti * pKeyItemQuanti = reinterpret_cast<CKeyItemQuanti*>(m_Lst_Items_Moy_Auto.GetItemData(pSel[i]));

				// Cr�er l'item associ� et l'ajouter � la base des items construits automatiques
				JInt32 IdItem = CreateItemAuto(pKeyItemQuanti);

				// Ajoute � la liste des s�lections
				m_pLstSelItemAuto->AddTail() = IdItem;
			}

			// Puis on sauvegarde les items quantitatifs automatiques et temporaires (ne pas oublier en fin d'utilsation de les virer)
			CItemsConstruitsManager::GetInstance()->GetItemsInstance()->Save();

			// On revient sur me mode items construits utilisateurs
			CItemsConstruitsManager::GetInstance()->SetMode(CItemsConstruitsManager::ModeUtilisateur);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////
// Creation de l'item automatique via les param�tres questions / modalit�s
JInt32 CDlgItemsMoyenne::CreateItemAuto(const CKeyItemQuanti * pKeyItemQuanti)
{
	// Ident nouvel item
	JInt32 IdNewItem = 0;

	// V�rifie validit� de la clef de l'item
	if (pKeyItemQuanti)
	{
		// On se positionne en mode items automatiques
		CItemsConstruitsManager::GetInstance()->SetMode(CItemsConstruitsManager::ModeAutomatique);

		// Le terrain en cours
		CTerrain * pTerrain = NULL;
		if (m_pDlgTableau)
			pTerrain = CTerrainManager::GetInstance()->GetTerrain(m_pDlgTableau->GetIdSource(),m_pDlgTableau->GetIdTerrain());
		else
			pTerrain = m_pDlgAnalyseur->m_pDoc->Get_TerrainTemoin();

		// L'instance items construits auto
		CItemsConstruitsBase * m_pItemsConstruitsAuto = CItemsConstruitsManager::GetInstance()->GetItemsInstance();

		// Cr�er un nouvel item auto
		CItemConstruit * pItem = m_pItemsConstruitsAuto->GetNewItemConstruit();

		// Les libell�s
		CString HeaderTxt = "";
		CString LabelTxt  = "*";

		if (pItem)
		{
			// Init Label item
			pItem->SetLabel((JLabel)LabelTxt, true);

			// R�cup la question associ�
			CQuestion const * pQuestion = (CQuestion *)pTerrain->GetQuestionByID(pKeyItemQuanti->m_IdQuestion);

			if (pQuestion)
			{
				// Construction de l'item
				CIBuilder Builder;

				// Set segment operator (1 seul segment)
				Builder.SetOperator(IBloc::Or);

				// Create a new segment
				CIBSegment *pISegment = new CIBSegment();

				// Set the question operator
				pISegment->SetOperator(IBloc::And);

				// Create a new question
				CIBQuestion * pIQuestion = new CIBQuestion;

				// Set the question id
				pIQuestion->SetID(pQuestion->GetID());

				CString TxtModaliteQu1 = pQuestion->GetIModalite(pKeyItemQuanti->m_IdentDim1, 0)->GetLabel().AsJCharPtr(); 

				// Traitement des questions simples quantitatives
				if (pKeyItemQuanti->m_IdentDim1 && TxtModaliteQu1 == "Valeurs num�riques")
				{
					// Question � 1 seule dimension et les valeurs quantitatives directement associ�es
					// Traitement 1ere dimension
					const IModalite * pModaDim1 = pQuestion->GetIModalite(pKeyItemQuanti->m_IdentDim1, 0);

					// Builder modality
					CIBModalite * pIModalite = new CIBModalite;
					pIModalite->AddValue(pModaDim1->GetID()); 
					pIModalite->SetOperator(IBloc::Or);
					pIQuestion->AddModalite(pIModalite);

					JVector<JFlt32> VectValues;
					JInt32 Divider = 0;
					GetQuestionNumericValues(pTerrain, pIQuestion, VectValues, Divider);

					// R�cup�ration des valeurs quantitatives
					CIBNumValue * pValues = 0;
					JList<JFlt32> Values;

					if(VectValues.GetCount())
					{
						for(VectValues.MoveFirst(); VectValues.IsValid(); VectValues.MoveNext())
						{
							Values.AddTail() = VectValues.GetItem();
						}
						if(Values.GetCount())
						{
							pValues = new CIBNumValue;
							pValues->SetValues(Values);
						}
					}
					
					// Construction de la classe quantitative
					CIBClass * pClass = new CIBClass();
					GetClass(pClass, Values);

					// Ajoute la classe � la question en cours
					pIQuestion->SetNumValue(0);
					pIQuestion->SetClass(pClass);

					// R�cup libell�s pour cette question quanti simple
					HeaderTxt = pQuestion->GetLabel().AsJCharPtr();
					LabelTxt = "";
				}
				else
				{
					// Traitement 1ere dimension
					const IModalite * pModaDim1 = pQuestion->GetIModalite(pKeyItemQuanti->m_IdentDim1, 0);
					JInt32 IdModa1 = pModaDim1->GetID();

					// Builder modality
					CIBModalite * pIModalite = new CIBModalite;
					pIModalite->AddValue(pModaDim1->GetID()); 
					pIModalite->SetOperator(IBloc::Or);
					pIQuestion->AddModalite(pIModalite);


					if (pKeyItemQuanti->m_IdentDim2 == 0)
					{
						JVector<JFlt32> VectValues;
						JInt32 Divider = 0;
						GetQuestionNumericValues(pTerrain, pIQuestion, VectValues, Divider);

						// R�cup�ration des valeurs quantitatives
						CIBNumValue * pValues = 0;
						JList<JFlt32> Values;

						if(VectValues.GetCount())
						{
							for(VectValues.MoveFirst(); VectValues.IsValid(); VectValues.MoveNext())
							{
								Values.AddTail() = VectValues.GetItem();
							}
							if(Values.GetCount())
							{
								pValues = new CIBNumValue;
								pValues->SetValues(Values);
							}
						}

						// Construction de la classe quantitative
						CIBClass * pClass = new CIBClass();
						GetClass(pClass, Values);

						// Ajoute la classe � la question en cours
						pIQuestion->SetNumValue(0);
						pIQuestion->SetClass(pClass);

						// R�cup libell�s pour cette question quanti simple
						HeaderTxt = pQuestion->GetLabel().AsJCharPtr();
						LabelTxt  = pModaDim1->GetLabel().AsJCharPtr();
					}

					else if (pKeyItemQuanti->m_IdentDim2 != 0)
					{
						// Traitement 2�me dimension
						const IModalite * pModaDim2 = pQuestion->GetIModalite(pKeyItemQuanti->m_IdentDim2, 1);
						JInt32 IdModa2 = pModaDim2->GetID();

						// Builder modality
						CIBModalite * pIModalite = new CIBModalite;
						pIModalite->AddValue(pModaDim2->GetID()); 
						pIModalite->SetOperator(IBloc::Or);

						pIQuestion->AddModalite(pIModalite);

						JVector<JFlt32> VectValues;
						JInt32 Divider = 0;
						GetQuestionNumericValues(pTerrain, pIQuestion, VectValues, Divider);

						// R�cup�ration des valeurs quantitatives
						CIBNumValue * pValues = 0;
						JList<JFlt32> Values;

						if(VectValues.GetCount())
						{
							for(VectValues.MoveFirst(); VectValues.IsValid(); VectValues.MoveNext())
							{
								Values.AddTail() = VectValues.GetItem();
							}
							if(Values.GetCount())
							{
								pValues = new CIBNumValue;
								pValues->SetValues(Values);
							}
						}

						// Construction de la classe quantitative
						CIBClass * pClass = new CIBClass();
						GetClass(pClass, Values);

						// Ajoute la classe � la question en cours
						pIQuestion->SetNumValue(0);
						pIQuestion->SetClass(pClass);

						// R�cup libell�s pour cette question quanti simple	
						HeaderTxt = pQuestion->GetLabel().AsJCharPtr();
						LabelTxt= "";
						LabelTxt.Format("%s" - "%s", pModaDim1->GetLabel().AsJCharPtr(), pModaDim2->GetLabel().AsJCharPtr());
					}	
				}

				// Ajoute la question au segment
				pISegment->Add(pIQuestion);
				Builder.Add(pISegment);

				// Set the builder object
				pItem->SetItemBuilder(pTerrain, Builder);

				// R�cup le libell� final de l'item quantitatif fabriqu�
				CString TxtFinal = "";
				if (LabelTxt != "")
					TxtFinal.Format("%s@%s", HeaderTxt, LabelTxt);
				else
				{
					// Libell� afficher apr�s le header (par d�faut Tous)
					CString Label2 = "Tous";

					// Intervalle de modalit�s
					CString IBString = pItem->GetIBString().AsJCharPtr();

					// Recherche de la chaine correspondant � l'intervalle de classe
					JUnt32 PosSup		 = IBString.Find(">=%");
					JUnt32 PosFinClasse1 = 0;
					if (PosSup)
						PosFinClasse1 = IBString.Find(",",PosSup);
					JUnt32 PosInf = IBString.Find("<=%");
					JUnt32 PosFinClasse2 = 0;
					if (PosInf)
						PosFinClasse2 = IBString.Find(",",PosInf);

					if (PosSup && PosFinClasse1 && PosInf && PosFinClasse2)
					{
						// Valeur intervalle borne inf
						CString StrInf = IBString.Mid(PosSup+3,PosFinClasse1-PosSup - 3);
						JInt32 Valeur1 = atoi(StrInf);
						if (Valeur1 < 0) Valeur1 = 0;

						// Valeur intervalle borne sup
						CString StrSup = IBString.Mid(PosInf+3,PosFinClasse2-PosInf - 3);
						JInt32 Valeur2 = atoi(StrSup );

						// Compose l'intervalle correspondant (exemple si age 15..99)
						Label2.Format("%d..%d",Valeur1, Valeur2);
					}

					// Afficher les valeurs limites
					TxtFinal.Format("%s@%s", HeaderTxt, Label2);
				}

				JLabel LabelItem = TxtFinal;
				pItem->SetLabel(LabelItem, true);

				// Construction du nouvel item (si non d�j� existant)
				IdNewItem = GetItemAuto(pItem);
				if (IdNewItem == 0)
				{
					// L'item n'existe pas, on le construit
					const CItemConstruit * pItemConst = m_pItemsConstruitsAuto->RegisterItemConstruit(pItem);

					// Recup ident new item
					IdNewItem = pItemConst->GetID();
				}

				// Indique modification base items construits auto
				m_pItemsConstruitsAuto->SetDirty();
			}

			// Apr�s il faut virer cette instance item
			delete pItem;
		}
	}
	return IdNewItem;
}

/////////////////////////////////////////////////////////////////////////////////////////
// Construction du nouvel item (si non d�j� existant)
JUnt32 CDlgItemsMoyenne::GetItemAuto(CItemConstruit * pItem)
{
	// Les informations du nouvel item � construire
	JLabel LibItem = pItem->GetLabel();
	JStringEx ABString = pItem->GetABString();
	JStringEx IBString = pItem->GetIBString();

	// V�rifie que cet item existe bien dans la base des items construits automatis�s
	const CItemConstruit * pItemAuto = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetItemConstruit(LibItem);
	if (pItemAuto)
	{
		// On v�rifie que cet item est construit de la m�me fa�on (IBString et ABString)
		JStringEx ABStringAuto = pItemAuto->GetABString();
		JStringEx IBStringAuto = pItemAuto->GetIBString();

		if (ABStringAuto == ABString && IBStringAuto == IBString)
		{
			// Il s'agit bien du m�me item
			JUnt32 IdItem = pItemAuto->GetID();
			return pItemAuto->GetID();
		}
		else
			// Apparemment item d�j� cr�� mais pas avec la m�me d�finition >> on le recr�e
			return 0;
	}	
	else
	{
		// Il s'agit l� d'un nouvel item � construire
		return 0;
	}
}

// Renvoi liste des items utilisateurs s�lectionn�s
void CDlgItemsMoyenne::GetItemsMoyUtil()
{
	if (m_pLstSelItemUtil)
	{
		// Init liste des elmts s�lectionn�s
		m_pLstSelItemUtil->Reset();

		// A t-on s�lectionn� des items quatitatifs utilisateur
		if (m_Lst_Items_Moy_Util.GetCount())
		{
			// R�cup les �lmts de la liste s�lectionn�e pour les moyennes
			int NbSel = m_Lst_Items_Moy_Util.GetSelCount();
			int  *pSel = new int[NbSel];
			m_Lst_Items_Moy_Util.GetSelItems(NbSel,pSel); 

			// Boucle sur les items s�lectionn�s
			for(int i = 0; i < NbSel ; ++i)
			{
				// R�cup�re l'ident item
				JInt32 IdItem = m_Lst_Items_Moy_Util.GetItemData(pSel[i]); 

				// Ajoute � la liste des s�lections
				m_pLstSelItemUtil->AddTail() = IdItem;
			}	
		}
	}		
}

////////////////////////////////////////////////////////////////////////////////////////
// R�cup�ration des valeurs d'une question num�rique
JBool CDlgItemsMoyenne::GetQuestionNumericValues(CTerrain *pTerrain, CIBQuestion * pIQuestion, JVector<JFlt32> & Values, JInt32 & Divider)
{
	bool Ret = true;

	// Create builder object
	CIBuilder tBuilder;
	tBuilder.SetOperator(IBloc::And);

	// Create segment
	CIBSegment *pSegment = new CIBSegment;
	pSegment->SetOperator(IBloc::And);

	// Add the question
	pSegment->Add(pIQuestion);
	tBuilder.Add(pSegment);

	// Get a new item construit
	// CItemConstruit *pItem = m_pDlgTableau->GetItems()->GetNewItemConstruit();
	CItemConstruit *pItem = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetNewItemConstruit();

	// Set the builder object
	pItem->SetItemBuilder(pTerrain,tBuilder);

	// Test if the current target is valid
	if(pItem->IsValid(pTerrain))
	{
		// Calculate the quanti values
		JInt32 Divider = 0;
		pItem->CalcQuantiValues(pTerrain, Values, Divider);
	}
	else
		Ret = false;

	pSegment->RemoveQuestion(pIQuestion);
	delete pItem;

	return Ret;
}

///////////////////////////////////////(//////////////////////////////////////////////
// Cr�ation de la classe associ�e aux valeurs quantitatives
JBool CDlgItemsMoyenne::GetClass(CIBClass * pClass, const JList <JFlt32>& Values)
{
	// V�rifie si on a bien un �lmt class, et que des valeurs quantis existent
	if (!pClass) return false;
	if (Values.GetCount() <= 0) return false;

	// Init avec d�but chaine quanti
	std::string StrValue = ">=%";

	// R�cup�re min et max des valeurs quantitatives
	Values.MoveFirst();
	JFlt32 ValMin = Values.GetItem();

	Values.MoveLast();
	JFlt32 ValMax = Values.GetItem();

	char buffer[50];
	sprintf(buffer, "%.6f<=%%%.6f", ValMin, ValMax);
	StrValue.append(buffer);

	StrValue.append(")));");

	const char * pStr = StrValue.c_str();
	if (pClass->FromString(&pStr))
		return true;
	else
		return false;
}

////////////////////////////////////////////////////////////////////////////////////////////
// S�lectionne ou d�selectionne tous les itmes quantis automatis�s � moyenner
void CDlgItemsMoyenne::OnItemsQuantiAuto_Tous()
{
	if (m_Lst_Items_Moy_Auto.GetSelCount())
	{
		// D�validation de toute la s�lection
		for (JInt32 InxLst = 0; InxLst < m_Lst_Items_Moy_Auto.GetCount(); InxLst++)
		{
			m_Lst_Items_Moy_Auto.SetSel(InxLst, FALSE);
		}
	}
	else
	{
		// Validation de toute la s�lection	
		for (JInt32 InxLst = 0; InxLst < m_Lst_Items_Moy_Auto.GetCount(); InxLst++)
		{
			m_Lst_Items_Moy_Auto.SetSel(InxLst, TRUE);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
// S�lectionne ou d�selectionne tous les itmes quantis utilisateurs � moyenner
void CDlgItemsMoyenne::OnItemsQuantiUtil_Tous()
{
	if (m_Lst_Items_Moy_Util.GetSelCount())
	{
		// D�validation de toute la s�lection
		for (JInt32 InxLst = 0; InxLst < m_Lst_Items_Moy_Util.GetCount(); InxLst++)
		{
			m_Lst_Items_Moy_Util.SetSel(InxLst, FALSE);
		}
	}
	else
	{
		// Validation de toute la s�lection	
		for (JInt32 InxLst = 0; InxLst < m_Lst_Items_Moy_Util.GetCount(); InxLst++)
		{
			m_Lst_Items_Moy_Util.SetSel(InxLst, TRUE);
		}
	}
}


//////////////////////////////////////////////////////////////////////////////
// Test si item quantitatif (surtout pour d�finir les �l�ments moyennables)
JBool CDlgItemsMoyenne::ItemQuantitatif(const CItemConstruit * pItem, CTerrain * pTerrain)
{
	CIBuilder IBuilder;

	// Test le terrain en cours
	if (pTerrain)
	{
		// R�cup l� d�finition de l'item
		pItem->GetItemBuilder(pTerrain,IBuilder);

		// Par d�faut ce n'est pas un quantitatif
		JBool Quantitatif = false;

		// Limitation des items moyennables  (sinon c'est le bordel !!!!)
		// -- seulement 1 segment
		// -- pas de combinaison dans 1 segment avec plusieurs items ou questions
		if (IBuilder.GetSegmentCount() > 1) return false;

		// Balaye tous les segments di Builder constrcuteur de l'item
		for (int IdxSeg = IBuilder.GetSegmentCount()-1; IdxSeg >=0; IdxSeg--)
		{
			// Pour chaque segment, on v�rie si il y a des questions ou des items quantitatifs
			CIBSegment * pIBSegment = IBuilder.GetSegment(IdxSeg);

			// Balaye les elmts du segment
			if(pIBSegment->HasItems())
			{
				// r�cup�re liste des questions
				JList <JInt32> LstQuestions;
				pIBSegment->GetQuestionsID(LstQuestions); 

				JList <JInt32> LstItems;
				pIBSegment->GetItemsID(LstItems);

				// Balayage des questions du segment
				for (LstQuestions.MoveFirst(); LstQuestions.IsValid(); LstQuestions.MoveNext())
				{
					// Id question segment
					JInt32 IdQuestion = LstQuestions.GetItem();

					// Question associ�
					IQuestion const * pQuestion = pTerrain->GetQuestionByID(IdQuestion);

					if(pQuestion)
					{
						// V�rifie si c'est une question
						const CQuestion *pQu = pQuestion->AsQuestion();

						// C'est une question, est-elle quantitative
						if (pQu)
						{
							// C'est une question, est-elle quantitative
							if (pQuestion->AsQuestion()->IsQuantiAssim() || pQuestion->AsQuestion()->IsQuantiPure())
							{
								return true;
							}
						}
					}
				}

				// r�cup�re liste des items
				JList <JInt32> Items;
				pIBSegment->GetItemsID(Items);

				// Limitation � 1 seul item (sinon c'est encore le bordel)
				if (Items.GetCount() > 1) return false;

				for(Items.MoveFirst(); Items.IsValid(); Items.MoveNext())
				{
					IQuestion const * pQuestion = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetQuestionByID(Items.GetItem());

					IItem const * pItem = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetIItemByID(Items.GetItem());

					CIBItem * pTargetItem = pIBSegment->FindItem(Items.GetItem());

					if (pTargetItem)
					{
						JList<JInt32> Moda;
						pTargetItem->GetValues(Moda);

						// Get the modalities
						for(Moda.MoveFirst(); Moda.IsValid(); Moda.MoveNext())
						{
							JInt32 ModaItemId = Moda.GetItem();
							const IItem * pModaItem = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetIItemByID(ModaItemId);
							if(pModaItem)
							{
								if (pModaItem->IsItemConstruit())
								{
									const CItemConstruit * pItemConst = pModaItem->AsItemConstruit();

									// Rep�rage si on est bien dans un item avec un ou des intervalles de valeurs
									CString ValString = pItemConst->GetIBString().AsJCharPtr();
									if (ValString.Find("<")!= -1 || ValString.Find(">") != -1)
									{
										// Test si c'est un item qauntitatif
										if (ItemQuantitatif(pItemConst,pTerrain))
										{
											return true;
										}
										else
										{
											return false;
										}
									}
								}
							}
						}
					}


					if(pQuestion)
					{
						// V�rifie si c'est une question
						const CQuestion *pQu = pQuestion->AsQuestion();

						// C'est une question, est-elle quantitative
						if (pQu)
						{
							if (pQuestion->AsQuestion()->IsQuantiAssim() || pQuestion->AsQuestion()->IsQuantiPure())
							{
								return true;
							}
						}
					}
					else
					{
						CPseudo * m_pPseudoItem = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetNewPseudo();

						if(m_pPseudoItem && Items.GetItem()==0)
						{
							JList <JInt32> ItemsP;
							m_pPseudoItem->GetItems(ItemsP); 
							for (ItemsP.MoveFirst(); ItemsP.IsValid(); ItemsP.MoveNext())
							{
								JInt32 IdItem = ItemsP.GetItem();

								IItem * pItem = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetIItemByID(IdItem);
								// Est ce un autre item, ou une question
								CPseudo			*  pPseudo			= pItem->AsPseudo(); 
								if (pItem)
								{

									JLabel LabelItem = pItem->GetLabel(); 
									JBool Ok = true;
								}
								else if (pPseudo)
								{
									JList <JInt32> ItemsP2;
									pPseudo->GetItems(ItemsP2); 
								}

								JBool Val = true;
							}
						}
					}
				}
			}

			// Items compos�s de questions
			else if (pIBSegment->HasQuestions())	
			{
				// r�cup�re liste des questions
				JList <JInt32> LstQuestions;
				pIBSegment->GetQuestionsID(LstQuestions); 

				// Balayage des questions du segment
				for (LstQuestions.MoveFirst(); LstQuestions.IsValid(); LstQuestions.MoveNext())
				{
					// Id question segment
					JInt32 IdQuestion = LstQuestions.GetItem();

					// Question associ�
					IQuestion const * pQuestion = pTerrain->GetQuestionByID(IdQuestion);

					if(pQuestion)
					{
						// V�rifie si c'est une question
						const CQuestion *pQu = pQuestion->AsQuestion();

						// C'est une question, est-elle quantitative
						if (pQu)
						{
							// C'est une question, est-elle quantitative
							if (pQuestion->AsQuestion()->IsQuantiAssim() || pQuestion->AsQuestion()->IsQuantiPure())
							{
								// Attention si la question n'est pas en mode regroupement de valeurs quantitatives
								CIBQuestion *pTargetQuestion = pIBSegment->FindQuestion(IdQuestion);
								CString ValuesStr = "";
								CIBClass * pClass = pTargetQuestion->GetClass();
								CIBNumValue * pNumValue = pTargetQuestion->GetNumValue();

								if (pClass)
									return true;
								else
									return false;
							}
						}
					}
				}
			}
		}
	}

	return false;
}