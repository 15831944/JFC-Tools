// DlgItemsMoyenne.h : definition file
//
// Dialogue permettant la s�lection d'un ou plusieurs items quantitatifs qui
// seront moyenn�s soit en mode ligne , soit en mode tableau dans les tris crois�s
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
// Classe clef pour le map des items quantitatifs vou�s � la s�lection
class CKeyItemQuanti
{
public:

	// test d'inf�riorit� clef item
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

	// test de sup�riorit� clef item quanti
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

	// test d'�galit� clef item
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

	// s�rialisation clef item quanti
	void Send(JStream & Stream) const
	{
		// R�cup �lmts
		Stream.Send(m_IdQuestion);
		Stream.Send(m_NbDimension);
		Stream.Send(m_IdentDim1);
		Stream.Send(m_IdentDim2);
		Stream.Send(m_IdentDim3);
	};

	// s�rialisation clef item quanti
	void Recv(JStream & Stream)
	{
		// R�cup �lmts
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

	// S�lection total ou raz s�lection des items quantitatifs � moyenner
	afx_msg virtual void OnItemsQuantiAuto_Tous();
	afx_msg virtual void OnItemsQuantiUtil_Tous();

	DECLARE_MESSAGE_MAP()
public:
	// Titres au dessus des listes
	CLabel		m_Lbl_Titre1;
	CLabel		m_Lbl_Titre2;

	// Boite Background
	CLabel		m_FondBox;

	// Listes des items � moyenner
	CJFCXListBox	m_Lst_Items_Moy_Auto;
	CJFCXListBox	m_Lst_Items_Moy_Util;

	// Boutons all select
	CButtonST	m_All_Quanti_Auto;
	CButtonST	m_All_Quanti_Util;

	// Boutons de validation ou de d�validation s�lection
	CButtonST	m_Btn_OK;
	CButtonST	m_Btn_Cancel;

	// R�cup�re fen�tre items
	CDlgTableau	* m_pDlgTableau;

	// R�cup�re fen�tre anlyseur
	CDlgAnalyseur * m_pDlgAnalyseur;

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();


protected:
	// Chargement des items moyennables utilisateurs
	void LoadItemMoyUtil();

private:
	// Liste des items auto et util s�lectionn�s
	JList <JUnt32>  * m_pLstSelItemAuto;
	JList <JUnt32>  * m_pLstSelItemUtil;

	// Les items construits automatis�s
	CItemsConstruitsAuto * m_pItemsConstruitsAuto;

protected:

	// Recup liste des items auto s�lectionn�s
	void GetItemsMoyAuto(); 	

	// Recup liste des items utilisateurs s�lectionn�s
	void GetItemsMoyUtil(); 

	// Template param�trage liste des items quanntis
	HXTemplateHandle m_TemplateQuanti; 

public:
	CThemeTreeQuanti	m_Tree_Quantis;

protected:
	// Liste s�lection en cours sur tree des questions quantitatives
	// JMap<DWORD,JUnt32>	m_MapSelTreeQuanti;

	// Liste des questions quantitatives en cours
	JMap <JInt32, JInt32> m_MapQuestionsQuantis;

	// Map des items num�rises s�lectionn�s ou pas
	JMap <CKeyItemQuanti, JBool> m_MapItemsQuantis;

	/// Constructed Target visualisation window
	// CThemeTreeQuanti	m_Tree_Quantis;

	// Mise en place de l'arborescence des questions quantitatives (envoi message update now ou pas)
	void SetThemeDataQuanti(JBool UpdateNow = false);

	// Update map de s�lections des items quantis
	void UpdateMapItemsQuantis(const IQuestion *pQuestion, JBool AjoutQu);

	// Cr�er les strings associ�s et mise � jour du map des string items num�ris�s
	void UpdateListItems_MoyAuto();

	// Creation de l'item automatique via les param�tres questions / modalit�s
	JInt32 CreateItemAuto(const CKeyItemQuanti * pKeyItemQuanti);

	// R�cup�ration des valeurs d'une question num�rique
	JBool GetQuestionNumericValues(CTerrain *pTerrain, CIBQuestion * pIQuestion, JVector<JFlt32> & Values, JInt32 & Divider);

	// Cr�ation de la classe associ�e aux valeurs quantitatives
	JBool GetClass(CIBClass * pClass, const JList <JFlt32>& Values);

	// Test si item quantitatif (surtout pour d�finir les �l�ments moyennables)
	JBool ItemQuantitatif(const CItemConstruit * pItem, CTerrain * pTerrain);

	/////////////////////////////////////////////////////////////////////////////////////////
	// Construction du nouvel item (si non d�j� existant)
	JUnt32 GetItemAuto(CItemConstruit * pItem);
	
	// Update synchro
	void Updated_ItemQuantiList(JBool AjoutQu);
};
