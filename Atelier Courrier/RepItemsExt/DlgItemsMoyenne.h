// DlgItemsMoyenne.h : definition file
//
// Dialogue permettant la sélection d'un ou plusieurs items quantitatifs qui
// seront moyennés soit en mode ligne , soit en mode tableau dans les tris croisés
//
#pragma once

#include "afxwin.h"
#include "label.h"
#include "BtnST.h"
#include "resource.h"
#include "ThemeTreeQuanti.h"

#include "DlgItems.h"
#include "ItemWrapper.h"
#include "PaletteMode.h"
#include "JFCXListBox.h"

class CDlgTableau;
class CDlgAnalyseur; 

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Classe clef pour le map des items quantitatifs voués à la sélection
class CKeyItemQuanti
{
public:

	// test d'infériorité clef item
	bool operator<(const CKeyItemQuanti & Rhs) const
	{
		if(m_IdQuestion < Rhs.m_IdQuestion)
			return true;

		if(m_IdQuestion == Rhs.m_IdQuestion && m_NbDimension < Rhs.m_NbDimension)
			return true;

		if (m_IdQuestion == Rhs.m_IdQuestion && m_NbDimension == Rhs.m_NbDimension 
											 && m_IdentDim1 < Rhs.m_IdentDim1)
			return true;

		if (m_IdQuestion == Rhs.m_IdQuestion && m_NbDimension == Rhs.m_NbDimension 
											 && m_IdentDim1 == Rhs.m_IdentDim1
											 && m_IdentDim2 < Rhs.m_IdentDim2)
			return true;

		if (m_IdQuestion == Rhs.m_IdQuestion && m_NbDimension == Rhs.m_NbDimension 
										     && m_IdentDim1 == Rhs.m_IdentDim1
											 && m_IdentDim2 == Rhs.m_IdentDim2
											 && m_IdentDim3 < Rhs.m_IdentDim3)
			return true;

		return false;
	};

	// test de supériorité clef item quanti
	bool operator>(const CKeyItemQuanti & Rhs) const
	{
		if(m_IdQuestion > Rhs.m_IdQuestion)
			return true;

		if(m_IdQuestion == Rhs.m_IdQuestion && m_NbDimension > Rhs.m_NbDimension)
			return true;

		if (m_IdQuestion == Rhs.m_IdQuestion && m_NbDimension == Rhs.m_NbDimension 
											 && m_IdentDim1 > Rhs.m_IdentDim1)
			return true;

		if (m_IdQuestion == Rhs.m_IdQuestion && m_NbDimension == Rhs.m_NbDimension 
											 && m_IdentDim1 == Rhs.m_IdentDim1
											 && m_IdentDim2 > Rhs.m_IdentDim2)
			return true;

		if (m_IdQuestion == Rhs.m_IdQuestion && m_NbDimension == Rhs.m_NbDimension 
											 && m_IdentDim1 == Rhs.m_IdentDim1
											 && m_IdentDim2 == Rhs.m_IdentDim2
											 && m_IdentDim3 > Rhs.m_IdentDim3)
			return true;

		return false;
	};

	// test d'égalité clef item
	bool operator==(const CKeyItemQuanti & Rhs) const
	{
		return (m_IdQuestion	== Rhs.m_IdQuestion		&& 
			    m_NbDimension	== Rhs.m_NbDimension	&&
				m_IdentDim1		== Rhs.m_IdentDim1		&&
				m_IdentDim2		== Rhs.m_IdentDim2		&&
				m_IdentDim3		== Rhs.m_IdentDim3);
	};

	// affectation via clef item 
	CKeyItemQuanti & operator=(const CKeyItemQuanti & Rhs)
	{
		m_IdQuestion	= Rhs.m_IdQuestion;
		m_NbDimension	= Rhs.m_NbDimension;
		m_IdentDim1		= Rhs.m_IdentDim1;
		m_IdentDim2		= Rhs.m_IdentDim2;
		m_IdentDim3		= Rhs.m_IdentDim3;
		
		return *this;
	};

	// sérialisation clef item quanti
	void Send(JStream & Stream) const
	{
		// Récup élmts
		Stream.Send(m_IdQuestion);
		Stream.Send(m_NbDimension);
		Stream.Send(m_IdentDim1);
		Stream.Send(m_IdentDim2);
		Stream.Send(m_IdentDim3);
	};

	// sérialisation clef item quanti
	void Recv(JStream & Stream)
	{
		// Récup élmts
		Stream.Recv(m_IdQuestion);
		Stream.Recv(m_NbDimension);
		Stream.Recv(m_IdentDim1);
		Stream.Recv(m_IdentDim2);
		Stream.Recv(m_IdentDim3);
	};

	// Constructeur 
	CKeyItemQuanti() 
	{ 
		m_IdQuestion	= 0; 
		m_NbDimension	= 0; 
		m_IdentDim1		= 0;
		m_IdentDim2		= 0;
		m_IdentDim3		= 0;
	};

	// attributs clef item quantitatif
	//
	// Ident question
	JInt32 m_IdQuestion; 

	// Nombre de dimensions
	JInt32 m_NbDimension;	

	// Ident Modalite pour chaque dimension
	JInt32 m_IdentDim1;
	JInt32 m_IdentDim2;
	JInt32 m_IdentDim3;
};


class AFX_EXT_CLASS CDlgItemsMoyenne : public CDialog
{
	DECLARE_DYNAMIC(CDlgItemsMoyenne)

public:
	// Constructeur
	CDlgItemsMoyenne(CDlgTableau * pParent, JList <JUnt32> *pLstItemMoyAuto, JList <JUnt32> *pLstItemMoyUtil);

	// Constructeur
	CDlgItemsMoyenne(CDlgAnalyseur * pParent, JList <JUnt32> *pLstItemMoyAuto, JList <JUnt32> *pLstItemMoyUtil);

	// Destructeur
	virtual ~CDlgItemsMoyenne();

	// Initialisation boite de dialogue
	virtual BOOL OnInitDialog();

	CATPColors::APPCOLOR		m_CATPColorsApp;

	// Dialog Data
	enum { IDD = IDD_RI_DLG_ITEMS_MOYENNE };

public:
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg LRESULT OnUpdated_ItemQuantiList(WPARAM wParam, LPARAM lParam);

	// Sélection total ou raz sélection des items quantitatifs à moyenner
	afx_msg virtual void OnItemsQuantiAuto_Tous();
	afx_msg virtual void OnItemsQuantiUtil_Tous();

	DECLARE_MESSAGE_MAP()
public:
	// Titres au dessus des listes
	CLabel		m_Lbl_Titre1;
	CLabel		m_Lbl_Titre2;

	// Boite Background
	CLabel		m_FondBox;

	// Listes des items à moyenner
	CJFCXListBox	m_Lst_Items_Moy_Auto;
	CJFCXListBox	m_Lst_Items_Moy_Util;

	// Boutons all select
	CButtonST	m_All_Quanti_Auto;
	CButtonST	m_All_Quanti_Util;

	// Boutons de validation ou de dévalidation sélection
	CButtonST	m_Btn_OK;
	CButtonST	m_Btn_Cancel;

	// Récupère fenêtre items
	CDlgTableau	* m_pDlgTableau;

	// Récupère fenêtre anlyseur
	CDlgAnalyseur * m_pDlgAnalyseur;

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();


protected:
	// Chargement des items moyennables utilisateurs
	void LoadItemMoyUtil();

private:
	// Liste des items auto et util sélectionnés
	JList <JUnt32>  * m_pLstSelItemAuto;
	JList <JUnt32>  * m_pLstSelItemUtil;

	// Les items construits automatisés
	CItemsConstruitsAuto * m_pItemsConstruitsAuto;

protected:

	// Recup liste des items auto sélectionnés
	void GetItemsMoyAuto(); 	

	// Recup liste des items utilisateurs sélectionnés
	void GetItemsMoyUtil(); 

	// Template paramétrage liste des items quanntis
	HXTemplateHandle m_TemplateQuanti; 

public:
	CThemeTreeQuanti	m_Tree_Quantis;

protected:
	// Liste sélection en cours sur tree des questions quantitatives
	// JMap<DWORD,JUnt32>	m_MapSelTreeQuanti;

	// Liste des questions quantitatives en cours
	JMap <JInt32, JInt32> m_MapQuestionsQuantis;

	// Map des items numérises sélectionnés ou pas
	JMap <CKeyItemQuanti, JBool> m_MapItemsQuantis;

	/// Constructed Target visualisation window
	// CThemeTreeQuanti	m_Tree_Quantis;

	// Mise en place de l'arborescence des questions quantitatives (envoi message update now ou pas)
	void SetThemeDataQuanti(JBool UpdateNow = false);

	// Update map de sélections des items quantis
	void UpdateMapItemsQuantis(const IQuestion *pQuestion, JBool AjoutQu);

	// Créer les strings associés et mise à jour du map des string items numérisés
	void UpdateListItems_MoyAuto();

	// Creation de l'item automatique via les paramètres questions / modalités
	JInt32 CreateItemAuto(const CKeyItemQuanti * pKeyItemQuanti);

	// Récupération des valeurs d'une question numérique
	JBool GetQuestionNumericValues(CTerrain *pTerrain, CIBQuestion * pIQuestion, JVector<JFlt32> & Values, JInt32 & Divider);

	// Création de la classe associée aux valeurs quantitatives
	JBool GetClass(CIBClass * pClass, const JList <JFlt32>& Values);

	// Test si item quantitatif (surtout pour définir les éléments moyennables)
	JBool ItemQuantitatif(const CItemConstruit * pItem, CTerrain * pTerrain);

	/////////////////////////////////////////////////////////////////////////////////////////
	// Construction du nouvel item (si non déjà existant)
	JUnt32 GetItemAuto(CItemConstruit * pItem);
	
	// Update synchro
	void Updated_ItemQuantiList(JBool AjoutQu);
};
