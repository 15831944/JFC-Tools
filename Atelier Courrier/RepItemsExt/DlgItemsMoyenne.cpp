// DlgItemsMoyenne.cpp : implementation file
//
// Dialogue permettant la sélection d'un ou plusieurs items quantitatifs qui
// seront moyennés soit en mode ligne , soit en mode tableau dans les tris croisés
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
	// Init fenêtres parent
	m_pDlgTableau = pParent;
	m_pDlgAnalyseur = 0;

	// Récup liste sélection à retourner
	m_pLstSelItemAuto = pLstItemMoyAuto;
	m_pLstSelItemUtil = pLstItemMoyUtil;

	// Init liste des sélections
	m_pLstSelItemAuto->Reset();
	m_pLstSelItemUtil->Reset();

	// Init map des questions quantis
	m_MapQuestionsQuantis.Reset();
}

// Constructeur via boite analyseur
CDlgItemsMoyenne::CDlgItemsMoyenne(CDlgAnalyseur * pParent, JList <JUnt32> *pLstItemMoyAuto, JList <JUnt32> *pLstItemMoyUtil): CDialog(CDlgItemsMoyenne::IDD, (CWnd*)pParent)
{
	// Init fenêtres parent
	m_pDlgAnalyseur = pParent;
	m_pDlgTableau = 0;

	// Récup liste sélection à retourner
	m_pLstSelItemAuto = pLstItemMoyAuto;
	m_pLstSelItemUtil = pLstItemMoyUtil;

	// Init liste des sélections
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

	// Titre de la fenêtre
	this->SetWindowText("Sélection de(s) item(s) à moyenner");

	// Fond de la boite
	m_FondBox.SubclassDlgItem(IDC_RI_STATIC_BOX,this);

	// Template system de base
	const CXTemplate & systTemplate = CXTemplateManager::GetInstance()->GetTemplate(CXTemplateManager::GetSystemTemplate());

	// Définition template pour listes elmts quantitatifs
	m_TemplateQuanti = CXTemplateManager::GetInstance()->CreateTemplate(systTemplate.textcolor_, 
																		systTemplate.backcolor_,
																		systTemplate.seltextcolor_,
																		CATPColors::GetColorSelect(CATPColors::COLORANALYSEUR),
																		false,true,false); 

	// Définition des controles
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

	// Parametrage Libellé entetes
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

	// Les boites édition
	m_FondBox.AddTopWindow(m_Lbl_Titre1);
	m_FondBox.AddTopWindow(m_Lst_Items_Moy_Auto);
	m_FondBox.AddTopWindow(m_All_Quanti_Auto);
	m_FondBox.AddTopWindow(m_Tree_Quantis);
	m_FondBox.AddTopWindow(m_Lbl_Titre2);
	m_FondBox.AddTopWindow(m_Lst_Items_Moy_Util);
	m_FondBox.AddTopWindow(m_All_Quanti_Util);

	// Couleur sélection liste
	// m_Lst_Items_Moy_Auto.SetSelectColor(CATPColors::GetColorSelect(CATPColors::COLORREPITEMS));
	// m_Lst_Items_Moy_Util.SetSelectColor(CATPColors::GetColorSelect(CATPColors::COLORREPITEMS));

	// Paramétrage liste tree des quantis
	m_Tree_Quantis.			m_CATPColorsApp = m_CATPColorsApp;
	m_Tree_Quantis.			SetColors();

	// Initialisation de la sélection index thèmatique des elmts quantis
	// m_Tree_Quantis.			ResetMapTree();

	// Set the theme tree data quantis
	SetThemeDataQuanti(true);
	
	// Chargement des listes d'items utilisateurs
	LoadItemMoyUtil();

	// On se repositionne sur la cascade du tree des questions quantis (sans les sélections)
	m_Tree_Quantis.TreeViaSauvegarde(false);

	/* A FINIR
	// Repositionne l'index thématique via denière sélection
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

		// Repositionne état du tree items à moyenner
		m_Tree_Quantis.SetParent(this);
		m_Tree_Quantis.SetData(pTerrain, m_pDlgTableau->GetItems(),QFilter,0, UpdateNow);
		m_Tree_Quantis.SetEtatData(m_pDlgTableau->GetDoc()->GetEtatTreeQuanti());

	}
	else if (m_pDlgAnalyseur)
	{
		CTerrain * pTerrain = m_pDlgAnalyseur->m_pDoc->Get_TerrainTemoin();

		// Repositionne état du tree items à moyenner
		m_Tree_Quantis.SetParent(this);
		m_Tree_Quantis.SetData(pTerrain, CItemsConstruitsManager::GetInstance()->GetItemsInstance(),QFilter,0, UpdateNow);
		m_Tree_Quantis.SetEtatData(m_pDlgAnalyseur->m_pDoc->GetEtatTreeQuanti());
	}
	
	// Repositionne l'index thématique via dernière sélection  (à voir)
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
// Synchronisation de la liste des items quantis à moyenner et du map des questions associées
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

	// Récup liste des question à mettre à jour via la sélection dans le tree questions quantis
	const JList <JInt32> * pLstIdQuestion = m_Tree_Quantis.GetLstQuestions();
	JBool  AjoutQu		= lParam;

	if (pLstIdQuestion)
	{
		// Attente mise à jour
		m_Lst_Items_Moy_Auto.SetRedraw(false);

		// Boucle sur toutes les questions à mettre à jour
		for (pLstIdQuestion->MoveFirst(); pLstIdQuestion->IsValid(); pLstIdQuestion->MoveNext())
		{
			// Question à ajouter
			JInt32 IdQuestion	= pLstIdQuestion->GetItem();

			// Récup la question associé
			IQuestion const * pQuestion = pTerrain->GetQuestionByID(IdQuestion);

			if (pQuestion)
			{
				// Mise à jour de la liste
				m_MapQuestionsQuantis.MoveTo(IdQuestion);
				if (AjoutQu)
				{
					// Ajoute une question quantitative
					if (!m_MapQuestionsQuantis.IsValid())
					{
						// Ajoute la question au map des questions en cours
						m_MapQuestionsQuantis.Add(IdQuestion);

						// Update map de sélections des items quantis
						UpdateMapItemsQuantis(pQuestion, AjoutQu);

						// Créer les strings associés et mise à jour du map des string items numérisés
						UpdateListItems_MoyAuto();
					}
				}
				else
				{
					// Supprime une question quantitative
					if (m_MapQuestionsQuantis.IsValid())
					{
						// Mise à jour du map des questions
						m_MapQuestionsQuantis.Remove();

						// Update map de sélections des items quantis
						UpdateMapItemsQuantis(pQuestion, AjoutQu);

						// Créer les strings associés et mise à jour du map des string items numérisés
						UpdateListItems_MoyAuto();
					}
				}
			}
		}

		// Effectue mise à jour
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

	// Récup liste des question à mettre à jour via la sélection dans le tree questions quantis
	const JList <JInt32> * pLstIdQuestion = m_Tree_Quantis.GetLstQuestions();
	if (pLstIdQuestion)
	{
		// Attente mise à jour
		m_Lst_Items_Moy_Auto.SetRedraw(false);

		// Boucle sur toutes les questions à mettre à jour
		for (pLstIdQuestion->MoveFirst(); pLstIdQuestion->IsValid(); pLstIdQuestion->MoveNext())
		{
			// Question à ajouter
			JInt32 IdQuestion	= pLstIdQuestion->GetItem();

			// Récup la question associé
			IQuestion const * pQuestion = pTerrain->GetQuestionByID(IdQuestion);

			if (pQuestion)
			{
				// Mise à jour de la liste
				m_MapQuestionsQuantis.MoveTo(IdQuestion);
				if (AjoutQu)
				{
					// Ajoute une question quantitative
					if (!m_MapQuestionsQuantis.IsValid())
					{
						// Ajoute la question au map des questions en cours
						int NbQuanti = m_MapQuestionsQuantis.GetCount();

						m_MapQuestionsQuantis.Add(IdQuestion);

						// Update map de sélections des items quantis
						UpdateMapItemsQuantis(pQuestion, AjoutQu);

						// Créer les strings associés et mise à jour du map des string items numérisés
						UpdateListItems_MoyAuto();
					}
				}
				else
				{
					// Supprime une question quantitative
					if (m_MapQuestionsQuantis.IsValid())
					{
						// Mise à jour du map des questions
						m_MapQuestionsQuantis.Remove();

						// Update map de sélections des items quantis
						UpdateMapItemsQuantis(pQuestion, AjoutQu);

						// Créer les strings associés et mise à jour du map des string items numérisés
						UpdateListItems_MoyAuto();
					}
				}
			}
		}

		// Effectue mise à jour
		m_Lst_Items_Moy_Auto.SetRedraw(true);
	}
}

///////////////////////////////////////////////////////////////////////////////////////
// Update map de sélections des items quantis
void CDlgItemsMoyenne::UpdateMapItemsQuantis(const IQuestion *pQuestion, JBool AjoutQu)
{
	// Ident de la question
	JInt32 IdQuestion = pQuestion->GetID();

	if (AjoutQu)
	{
		// Nombre de dimensions de la question
		JInt32 NbDims = pQuestion->HasDimensions();

		// Définition de chaque items quantitaifs possible de la question
		CKeyItemQuanti KeyItemQuanti;
		KeyItemQuanti.m_IdQuestion	= IdQuestion;
		KeyItemQuanti.m_NbDimension = NbDims;

		// Vecteur de modalité
		JList< JVector<const IModalite *> >	DimList;
		JList< JInt32 >						DimType;

		// Boucle sur toutes les dimensions de la question
		JInt32 InxDim = 0;

		// Liste des modalités
		JVector <const IModalite *>  vDim1s;

		if (m_pDlgTableau)
			pQuestion->GetIModalites(vDim1s, 0, InxDim, m_pDlgTableau->GetContingentMask());
		else
			pQuestion->GetIModalites(vDim1s, 0, InxDim, 1);
				
		// récup le libellé de la 1ere modalité
		CString TxtModalite;
		vDim1s.MoveFirst();
		if (vDim1s.IsValid())
		{
			// Récup la modalité associé
			const IModalite* pModalite = vDim1s.GetItem();

			// Libellé de la modalité 
			TxtModalite = pModalite->GetLabel().AsJCharPtr(); 

			// Cadre dimension 1 pour les questions quantitatives sans libellé à 1 seule modalité
			if (vDim1s.GetCount() == 1)
			{
				if (TxtModalite == "Valeurs numériques")
				{
					// Valeur modalité
					JInt32 ValModa = pModalite->GetID();

					// Indique qu'il s'agit d'une question simple numérique
					KeyItemQuanti.m_IdentDim1 = pModalite->GetID();
					KeyItemQuanti.m_IdentDim2 = 0;
					KeyItemQuanti.m_IdentDim3 = 0;
				}
				else
				{
					// Indique qu'il s'agit d'une question simple numérique avec 1 seule modalité
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
						// Récup la modalité associé
						const IModalite* pModalite = vDim1s.GetItem();

						// Précise ident de la modalité dimension 1
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
					// Passe à la dimension 2
					InxDim++;
					
					// Au moins 2 dimensions, on combine les modalités
					JVector <const IModalite *> vDim2s;

					if (m_pDlgTableau)
						pQuestion->GetIModalites(vDim2s, 0, InxDim, m_pDlgTableau->GetContingentMask());
					else
						pQuestion->GetIModalites(vDim2s, 0, InxDim, 1);

					// Question avec plus d'une dimension
					for (vDim1s.MoveFirst(); vDim1s.IsValid(); vDim1s.MoveNext())
					{
						// Récup la modalité associé
						const IModalite* pModalite = vDim1s.GetItem();

						// Précise ident de la modalité dimension 1
						KeyItemQuanti.m_IdentDim1 = pModalite->GetID();
						KeyItemQuanti.m_IdentDim2 = 0;
						KeyItemQuanti.m_IdentDim3 = 0;

						// Si seulement 1 dimension, on stocke les infos ident modalité 
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
								// Récup la modalité associé à la dimension 2
								const IModalite* pModalite = vDim2s.GetItem();

								// Précise ident de la modalité dimension 2
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
			// Récup info clef item quanti
			const CKeyItemQuanti & KeyItemQuanti = m_MapItemsQuantis.GetKey();

			// Enlève du map s'il s'agit de la question
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

// Créer les strings associés et mise à jour du map des string items numérisés
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

	// Sauve d'abord tous les items précedemment sélectionnés
	JMap <CKeyItemQuanti, bool> MapSelIdQuestion;
	for (int InxLst = 0; InxLst < m_Lst_Items_Moy_Auto.GetCount(); InxLst++)
	{
		if (m_Lst_Items_Moy_Auto.GetSel(InxLst))
		{
			// Ajoute la question si déjà sélectionnée
			CKeyItemQuanti * pKeyItemQuanti = reinterpret_cast<CKeyItemQuanti*>(m_Lst_Items_Moy_Auto.GetItemData(InxLst));
			MapSelIdQuestion.MoveTo(*pKeyItemQuanti);
			if (!MapSelIdQuestion.IsValid())
				MapSelIdQuestion.Add(*pKeyItemQuanti) = true;
		}
	}		

	// Reset liste des items à sélectionner
	m_Lst_Items_Moy_Auto.ResetContent();

	// Boucle sur toutes les lignes d'items en cours
	for (m_MapItemsQuantis.MoveFirst(); m_MapItemsQuantis.IsValid(); m_MapItemsQuantis.MoveNext())
	{	
		// Récup info clef item quanti
		const CKeyItemQuanti & KeyItemQuanti = m_MapItemsQuantis.GetKey();

		// Récupère la question principale
		CQuestion const * pQuestion = (CQuestion *)pTerrain->GetQuestionByID(KeyItemQuanti.m_IdQuestion);
	
		if (pQuestion)
		{
			// Affichage ou pas de l'item
			JBool AffichItem = true;

			// Table des dimensions de l'item
			JInt32 Dims = pQuestion->HasDimensions();

			// Libellé global de l'item numérique moyennable
			CString LibItem;

			if (Dims < 3)
			{
				// Boucle sur toutes les dimensions
				for(JInt32 x = 0; x < Dims ; x++)
				{
					// Question quanti à au moins 1 dimension
					if (x == 0)
					{
						// Traitement de la 1ere dimension de la question 
						if (KeyItemQuanti.m_IdentDim1 == 0)
						{
							// Ici il s'agit d'un item numérique mono-dimension avec valeurs directes
							// LibItem.Format("%s (%d-%d-%d)",pQuestion->GetLabel().AsJCharPtr(), KeyItemQuanti.m_IdentDim1, KeyItemQuanti.m_IdentDim2, KeyItemQuanti.m_IdentDim3);  // pour test des idents
							LibItem.Format("%s",pQuestion->GetLabel().AsJCharPtr());
						}
						else
						{
							// Récup le libellé de la modalité dimension 1
							const IModalite * pModa = pQuestion->GetIModalite(KeyItemQuanti.m_IdentDim1, 0);

							// Vérifie si question quantitative simple
							CString TxtModaliteQu1 = pModa->GetLabel().AsJCharPtr(); 
							
							if (TxtModaliteQu1 == "Valeurs numériques")
							{
								LibItem.Format("%s",pQuestion->GetLabel().AsJCharPtr());
							}
							else
							{
								// LibItem.Format("%s - %s (%d-%d-%d)",pQuestion->GetLabel().AsJCharPtr(), pModa->GetLabel().AsJCharPtr(), KeyItemQuanti.m_IdentDim1, KeyItemQuanti.m_IdentDim2, KeyItemQuanti.m_IdentDim3); // pour test des idents
								LibItem.Format("%s - %s", pQuestion->GetLabel().AsJCharPtr(), pModa->GetLabel().AsJCharPtr());
							}
						}	

						// Si seulement 1 dimension on arrête
						if (Dims == 1) break;
					}
					
					// Question quanti à au moins 2 dimensions
					if (x == 1)
					{
						// Récup le libellé de la modalité dimension 1
						const IModalite * pModa = pQuestion->GetIModalite(KeyItemQuanti.m_IdentDim2, 0);
						CString TxtDim2 = pModa->GetLabel().AsJCharPtr();
					}

					// Question quanti à au moins 3 dimensions
					if (x == 2)
					{
						// A FAIRE si nécessaire, pour le moment au delà de 2 dimensions on ne traite pas
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

				// Remet la sélection précédente si déjà sélectionné
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
// Validation de la sélection des items à moyenner 
void CDlgItemsMoyenne::OnBnClickedOk()
{
	// Update sélection en cours des items numérisés automatiques
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

	// Update sélection en cours des items numérisés utilisateurs
	GetItemsMoyUtil();

	// On valide la sélection
	OnOK();
}

///////////////////////////////////////////////////////////////////////////////////////
// Annulation de la sélection
void CDlgItemsMoyenne::OnBnClickedCancel()
{
	// Récup les items construits automatiques pour les virer
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
		// Vecteur des items à afficher
		JList<IItem const *> vItems;
		m_Lst_Items_Moy_Util.ResetContent(); 
		m_pDlgTableau->GetItems()->GetItemsIndependant(vItems,m_pDlgTableau->GetTerrain());

		///////////////////////////////////////////////////////////////////////////////////
		// Get only those valid for this terrain
		for (vItems.MoveFirst(); vItems.IsValid(); vItems.MoveNext())
		{
			// Ident de l'item
			JInt32 IdItem	= vItems.GetItem()->GetID();

			// Libellé de l'item
			const IItem *pItem = vItems.GetItem();
			CString LibItem = pItem->GetLabel().AsJCharPtr();

			// Si sép @ existe , on le remplace par -
			LibItem.Replace("@","-");

			// On ne prend que les items moyennables	
			CItemConstruit *pItemConstruit = (CItemConstruit *)pItem;
			if (m_pDlgTableau->ItemQuantitatif(pItemConstruit, m_pDlgTableau->GetIdSource(), m_pDlgTableau->GetIdTerrain()))
			{
				// Ajoute libellé dans la liste et ident en tant qu'itemdata
				long InxLst = m_Lst_Items_Moy_Util.AddLine(LibItem, m_TemplateQuanti);
				// long InxLst = m_Lst_Items_Moy_Util.AddString(LibItem);
				m_Lst_Items_Moy_Util.SetItemData(InxLst,IdItem);
			}
		}
	}
	else if (m_pDlgAnalyseur)
	{
		// Vecteur des items à afficher
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

			// Libellé de l'item
			const IItem *pItem = vItems.GetItem();
			CString LibItem = pItem->GetLabel().AsJCharPtr();

			// On ne prend que les items moyennables	
			CItemConstruit *pItemConstruit = (CItemConstruit *)pItem;
			if (ItemQuantitatif(pItemConstruit, m_pDlgAnalyseur->m_pDoc->Get_TerrainTemoin()))
			{
				// Ajoute libellé dans la liste et ident en tant qu'itemdata
				long InxLst = m_Lst_Items_Moy_Util.AddLine(LibItem, m_TemplateQuanti);
				m_Lst_Items_Moy_Util.SetItemData(InxLst,IdItem);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////
// // Renvoi liste des items auto sélectionnés
void CDlgItemsMoyenne::GetItemsMoyAuto()
{
	if (m_pLstSelItemAuto)
	{
		// Init liste des elmts sélectionnés
		m_pLstSelItemAuto->Reset();

		// A t-on sélectionné des items quatitatifs automatique
		if (m_Lst_Items_Moy_Auto.GetCount())
		{
			// On revient sur me mode items construits utilisateurs
			CItemsConstruitsManager::GetInstance()->SetMode(CItemsConstruitsManager::ModeAutomatique);

			// Récup les élmts de la liste sélectionnée pour les moyennes
			int NbSel = m_Lst_Items_Moy_Auto.GetSelCount();
			int  *pSel = new int[NbSel];
			m_Lst_Items_Moy_Auto.GetSelItems(NbSel,pSel); 

			// Boucle sur les items sélectionnés
			for(int i = 0; i < NbSel; ++i)
			{
				// Récupère les paramétres de l'item à créer
				CKeyItemQuanti * pKeyItemQuanti = reinterpret_cast<CKeyItemQuanti*>(m_Lst_Items_Moy_Auto.GetItemData(pSel[i]));

				// Créer l'item associé et l'ajouter à la base des items construits automatiques
				JInt32 IdItem = CreateItemAuto(pKeyItemQuanti);

				// Ajoute à la liste des sélections
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
// Creation de l'item automatique via les paramètres questions / modalités
JInt32 CDlgItemsMoyenne::CreateItemAuto(const CKeyItemQuanti * pKeyItemQuanti)
{
	// Ident nouvel item
	JInt32 IdNewItem = 0;

	// Vérifie validité de la clef de l'item
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

		// Créer un nouvel item auto
		CItemConstruit * pItem = m_pItemsConstruitsAuto->GetNewItemConstruit();

		// Les libellés
		CString HeaderTxt = "";
		CString LabelTxt  = "*";

		if (pItem)
		{
			// Init Label item
			pItem->SetLabel((JLabel)LabelTxt, true);

			// Récup la question associé
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
				if (pKeyItemQuanti->m_IdentDim1 && TxtModaliteQu1 == "Valeurs numériques")
				{
					// Question à 1 seule dimension et les valeurs quantitatives directement associées
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

					// Récupération des valeurs quantitatives
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

					// Ajoute la classe à la question en cours
					pIQuestion->SetNumValue(0);
					pIQuestion->SetClass(pClass);

					// Récup libellés pour cette question quanti simple
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

						// Récupération des valeurs quantitatives
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

						// Ajoute la classe à la question en cours
						pIQuestion->SetNumValue(0);
						pIQuestion->SetClass(pClass);

						// Récup libellés pour cette question quanti simple
						HeaderTxt = pQuestion->GetLabel().AsJCharPtr();
						LabelTxt  = pModaDim1->GetLabel().AsJCharPtr();
					}

					else if (pKeyItemQuanti->m_IdentDim2 != 0)
					{
						// Traitement 2ème dimension
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

						// Récupération des valeurs quantitatives
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

						// Ajoute la classe à la question en cours
						pIQuestion->SetNumValue(0);
						pIQuestion->SetClass(pClass);

						// Récup libellés pour cette question quanti simple	
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

				// Récup le libellé final de l'item quantitatif fabriqué
				CString TxtFinal = "";
				if (LabelTxt != "")
					TxtFinal.Format("%s@%s", HeaderTxt, LabelTxt);
				else
				{
					// Libellé afficher après le header (par défaut Tous)
					CString Label2 = "Tous";

					// Intervalle de modalités
					CString IBString = pItem->GetIBString().AsJCharPtr();

					// Recherche de la chaine correspondant à l'intervalle de classe
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

				// Construction du nouvel item (si non déjà existant)
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

			// Après il faut virer cette instance item
			delete pItem;
		}
	}
	return IdNewItem;
}

/////////////////////////////////////////////////////////////////////////////////////////
// Construction du nouvel item (si non déjà existant)
JUnt32 CDlgItemsMoyenne::GetItemAuto(CItemConstruit * pItem)
{
	// Les informations du nouvel item à construire
	JLabel LibItem = pItem->GetLabel();
	JStringEx ABString = pItem->GetABString();
	JStringEx IBString = pItem->GetIBString();

	// Vérifie que cet item existe bien dans la base des items construits automatisés
	const CItemConstruit * pItemAuto = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetItemConstruit(LibItem);
	if (pItemAuto)
	{
		// On vérifie que cet item est construit de la même façon (IBString et ABString)
		JStringEx ABStringAuto = pItemAuto->GetABString();
		JStringEx IBStringAuto = pItemAuto->GetIBString();

		if (ABStringAuto == ABString && IBStringAuto == IBString)
		{
			// Il s'agit bien du même item
			JUnt32 IdItem = pItemAuto->GetID();
			return pItemAuto->GetID();
		}
		else
			// Apparemment item déjà créé mais pas avec la même définition >> on le recrée
			return 0;
	}	
	else
	{
		// Il s'agit là d'un nouvel item à construire
		return 0;
	}
}

// Renvoi liste des items utilisateurs sélectionnés
void CDlgItemsMoyenne::GetItemsMoyUtil()
{
	if (m_pLstSelItemUtil)
	{
		// Init liste des elmts sélectionnés
		m_pLstSelItemUtil->Reset();

		// A t-on sélectionné des items quatitatifs utilisateur
		if (m_Lst_Items_Moy_Util.GetCount())
		{
			// Récup les élmts de la liste sélectionnée pour les moyennes
			int NbSel = m_Lst_Items_Moy_Util.GetSelCount();
			int  *pSel = new int[NbSel];
			m_Lst_Items_Moy_Util.GetSelItems(NbSel,pSel); 

			// Boucle sur les items sélectionnés
			for(int i = 0; i < NbSel ; ++i)
			{
				// Récupère l'ident item
				JInt32 IdItem = m_Lst_Items_Moy_Util.GetItemData(pSel[i]); 

				// Ajoute à la liste des sélections
				m_pLstSelItemUtil->AddTail() = IdItem;
			}	
		}
	}		
}

////////////////////////////////////////////////////////////////////////////////////////
// Récupération des valeurs d'une question numérique
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
// Création de la classe associée aux valeurs quantitatives
JBool CDlgItemsMoyenne::GetClass(CIBClass * pClass, const JList <JFlt32>& Values)
{
	// Vérifie si on a bien un élmt class, et que des valeurs quantis existent
	if (!pClass) return false;
	if (Values.GetCount() <= 0) return false;

	// Init avec début chaine quanti
	std::string StrValue = ">=%";

	// Récupère min et max des valeurs quantitatives
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
// Sélectionne ou déselectionne tous les itmes quantis automatisés à moyenner
void CDlgItemsMoyenne::OnItemsQuantiAuto_Tous()
{
	if (m_Lst_Items_Moy_Auto.GetSelCount())
	{
		// Dévalidation de toute la sélection
		for (JInt32 InxLst = 0; InxLst < m_Lst_Items_Moy_Auto.GetCount(); InxLst++)
		{
			m_Lst_Items_Moy_Auto.SetSel(InxLst, FALSE);
		}
	}
	else
	{
		// Validation de toute la sélection	
		for (JInt32 InxLst = 0; InxLst < m_Lst_Items_Moy_Auto.GetCount(); InxLst++)
		{
			m_Lst_Items_Moy_Auto.SetSel(InxLst, TRUE);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
// Sélectionne ou déselectionne tous les itmes quantis utilisateurs à moyenner
void CDlgItemsMoyenne::OnItemsQuantiUtil_Tous()
{
	if (m_Lst_Items_Moy_Util.GetSelCount())
	{
		// Dévalidation de toute la sélection
		for (JInt32 InxLst = 0; InxLst < m_Lst_Items_Moy_Util.GetCount(); InxLst++)
		{
			m_Lst_Items_Moy_Util.SetSel(InxLst, FALSE);
		}
	}
	else
	{
		// Validation de toute la sélection	
		for (JInt32 InxLst = 0; InxLst < m_Lst_Items_Moy_Util.GetCount(); InxLst++)
		{
			m_Lst_Items_Moy_Util.SetSel(InxLst, TRUE);
		}
	}
}


//////////////////////////////////////////////////////////////////////////////
// Test si item quantitatif (surtout pour définir les éléments moyennables)
JBool CDlgItemsMoyenne::ItemQuantitatif(const CItemConstruit * pItem, CTerrain * pTerrain)
{
	CIBuilder IBuilder;

	// Test le terrain en cours
	if (pTerrain)
	{
		// Récup lé définition de l'item
		pItem->GetItemBuilder(pTerrain,IBuilder);

		// Par défaut ce n'est pas un quantitatif
		JBool Quantitatif = false;

		// Limitation des items moyennables  (sinon c'est le bordel !!!!)
		// -- seulement 1 segment
		// -- pas de combinaison dans 1 segment avec plusieurs items ou questions
		if (IBuilder.GetSegmentCount() > 1) return false;

		// Balaye tous les segments di Builder constrcuteur de l'item
		for (int IdxSeg = IBuilder.GetSegmentCount()-1; IdxSeg >=0; IdxSeg--)
		{
			// Pour chaque segment, on vérie si il y a des questions ou des items quantitatifs
			CIBSegment * pIBSegment = IBuilder.GetSegment(IdxSeg);

			// Balaye les elmts du segment
			if(pIBSegment->HasItems())
			{
				// récupère liste des questions
				JList <JInt32> LstQuestions;
				pIBSegment->GetQuestionsID(LstQuestions); 

				JList <JInt32> LstItems;
				pIBSegment->GetItemsID(LstItems);

				// Balayage des questions du segment
				for (LstQuestions.MoveFirst(); LstQuestions.IsValid(); LstQuestions.MoveNext())
				{
					// Id question segment
					JInt32 IdQuestion = LstQuestions.GetItem();

					// Question associé
					IQuestion const * pQuestion = pTerrain->GetQuestionByID(IdQuestion);

					if(pQuestion)
					{
						// Vérifie si c'est une question
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

				// récupére liste des items
				JList <JInt32> Items;
				pIBSegment->GetItemsID(Items);

				// Limitation à 1 seul item (sinon c'est encore le bordel)
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

									// Repérage si on est bien dans un item avec un ou des intervalles de valeurs
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
						// Vérifie si c'est une question
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

			// Items composés de questions
			else if (pIBSegment->HasQuestions())	
			{
				// récupère liste des questions
				JList <JInt32> LstQuestions;
				pIBSegment->GetQuestionsID(LstQuestions); 

				// Balayage des questions du segment
				for (LstQuestions.MoveFirst(); LstQuestions.IsValid(); LstQuestions.MoveNext())
				{
					// Id question segment
					JInt32 IdQuestion = LstQuestions.GetItem();

					// Question associé
					IQuestion const * pQuestion = pTerrain->GetQuestionByID(IdQuestion);

					if(pQuestion)
					{
						// Vérifie si c'est une question
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