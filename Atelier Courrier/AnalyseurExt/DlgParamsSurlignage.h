/////////////////////////////////////////////////////////////////////////////////////////////////////
// Fenêtre de paramétrage pour le ou les surlignages directement sur le tableau tris croisés
//
//
// Définition des grandeurs et résultats à prendre en compte
// Défnition des fourchettes de surlignage
// Défénition des paramétres par défaut
// Gestion des colueurs pour le surlignage
/////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <Label.h>
#include <BtnST.h>
#include "JFCEditDecimal.h"
#include "AnalyseurDoc.h"
#include "afxwin.h"

class COpeSurlignage;

// Boîte de dialogue CDlgParamsSurlignage

class CDlgParamsSurlignage : public CDialog
{
	DECLARE_DYNAMIC(CDlgParamsSurlignage)

public:
	// Constructeur
	CDlgParamsSurlignage(CWnd* pParent = NULL);   

	// Destructeur
	virtual ~CDlgParamsSurlignage();

	// Récup du document courant
	void SetParams(CAnalyseurDoc *pDoc, JMap  <JUnt32, CString> *pMapGrandeurs, JMap  <JUnt32, CString> *pMapResultats);

	/////////////////////////////////////// Récupération des différents paramètres positionnés pour le surlignage //////////////////////////////////////
	// Récupère tous les paramètres opération surlignage
	const COpeSurlignage GetParam();

	// Récupération couleur
	COLORREF GetColorSurlign();

	// Récupération Raz (oui/non) 
	JBool GetRazSurlign();

	// Récupère type de paramètrage valeurs
	BYTE  GetTypeParam()			{return m_TypeParamVal; };	

	// Récupération valeur ou bornes
	double GetValeur1()				{return m_Valeur1; };
	double GetValeur2()				{return m_Valeur2; };

	// Récupération Grandeurs à surligner
	RESULTTYPE GetParamGrandeur()	{return m_Grandeur; };
	
	
	// Récupération Résultats à surligner
	RESULTSTYLE GetParamResult()	{return m_Result; };

	// Données de boîte de dialogue
	enum { IDD = IDD_AN_DLG_PARAMS_SURLIGN };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	// Fonctions design controles
	void SetupControls();
	void SetColors();
	void SetControls();

	// Définition des textes tooltip
	void SetTooltips();

	// Récupérartion des élmts combo résultats et grandeurs
	void SetParamsSurlign();

	// Update valeurs de  parametrage
	void  SetTypeParamVal();

	// Test validité paramètres valeurs
	JBool OkParamValue();

	// Positionne les paramètres par défaut
	void SetParamDefaut();

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

protected:

	///////////////////////////////////// Boite sélection résultat et grandeur pour surlignage /////////////////////////
	//
	// Boite contenant les 2 combos pour sélection Résultat et Grandeur de surlignage
	CLabel				m_Box_ResultEtGrandeur;

	// Titre au dessus du combox box sélection résultat à surligner
	CLabel				m_Ttl_Result_Surlign;

	// Titre au dessus du combox box sélection grandeur à surligner
	CLabel				m_Ttl_Grandeur_Surlign;

	// Combobox pour sélectionner résultat à surligner
	CComboBox			m_Cmb_Result_Surlign;

	// Combobox pour sélectionner grandeur à surligner
	CComboBox			m_Cmb_Grandeur_Surlign;

	////////////////////////////////////// Boite Valeurs et Bornes du surlignage //////////////////////////////////////
	//
	// Boite contenant les opérateurs et valeurs pour définir le surlignage
	CLabel				m_Box_ValeursBornes;

	// Titre pour sélectionner valeur ou bornes pour le surlignage
	CLabel				m_Ttl_ValeursBornes;

	// Opérateur surlignage stictement supérieur
	CButtonST			m_BtnRad_Sup;

	// Opérateur surlignage supérieur ou égal
	CButtonST			m_BtnRad_SupEgal;

	// Opérateur surlignage strictement inférieur
	CButtonST			m_BtnRad_Inf;

	// Opérateur surlignage inférieur ou égal
	CButtonST			m_BtnRad_InfEgal;

	// Boite d'édition pour saisie valeur de surlignage
	JFCEditDecimal		m_EditValue;

	// Boite d'édition pour saisie borne inférieure plage de surlignage
	JFCEditDecimal		m_EditBorneInf;

	// Boite d'édition pour saisie borne supérieur plage de surlignage
	JFCEditDecimal		m_EditBorneSup;

	// Check pour valider ou pas la prise en compte externe de la plage de surlignage
	CButtonST			m_ChkExtBornes;
	
	// Bouton radion pour sélection surlignage cellules via plages de valeurs
	CButtonST			m_BtnRad_Bornes;

	// Séparateur entre les 2 valeurs de la plage surlignage
	CLabel				m_Label_EntreBornes;

	// Texte % pour la borne inférieure de la plage
	CLabel				m_PrctBorneInf;

	// Texte % pour la borne supérieure de la plage
	CLabel				m_PrctBorneSup;

	// Texte % pour la valeur
	CLabel				m_PrctValeur;

	// Cadre regroupant les opérateurs sur 1 valeur
	CLabel				m_Cadre_OpeValeur;

	// Cadre regroupant les élmts définition des bornes
	CLabel				m_Cadre_Bornes;

	////////////////////////////////////// Boite Définition de la couleur de surlignage //////////////////////////////////////
	//
	// Boite contenant controles pour définition couleur
	CLabel				m_Box_Couleur;

	// Titre de la boite sélection couleur
	CLabel				m_Ttl_Couleur;

	// Cadre affichant la couleur sélectionnée pour le surlignage
	CLabel				m_CadreCouleur;

	// Bouton permettant la modification de la couleur de surlignage
	CButtonST			m_Btn_Modif_Couleur;

	////////////////////////////////////// Boite définition paramétres par défaut /////////////////////////////////////////////
	//
	// Boite contenant controles pour paramètres par défaut
	CLabel				m_Box_ConfigDef;

	// Titre panel configuration par défaut
    CLabel				m_Ttl_ConfigDef;

	// Bouton validant les sélections courantes comme paramètres par défaut du surlignage
	CButtonST			m_Btn_ParamDefaut;

	// Bouton effaçant la sélection par défaut du surlignage
	CButtonST			m_Btn_Effacer_DefDefaut;

	// Bouton affichant les paramètres par défaut
	CButtonST			m_Btn_SetParamDefaut;

	// Réinitialisation de la totalité du surlignage
	// CButtonST			m_Btn_Raz_Surlign;

	////////////////////////////////////// Les autres contrôles //////////////////////////////////
	//
	// Bouton validant la sélection surligange en cours
	CButtonST			m_BtnOk;

	// Bouton annulant le surlignage
	CButtonST			m_BtnCancel;

protected:
	// Le document courant
	CAnalyseurDoc *		m_pDoc;

	// Récupération des élmts grandeurs et résultats
	JMap  <JUnt32, CString> * m_pMapGrandeurs;
	JMap  <JUnt32, CString> * m_pMapResultats;

	// Couleur de surlignage en cours
	COLORREF			m_ColorSurlign;

	// Flag Raz Surlignage
	JBool				m_Raz_Surlignage;

	// Selection courante valuer
	BYTE				m_TypeParamVal;

	// Les valeurs
	double				m_Valeur1;
	double				m_Valeur2;

	// Récupération Grandeurs à surligner
	RESULTTYPE			m_Grandeur;
		
	// Récupération Résultats à surligner
	RESULTSTYLE			m_Result;

public:
	afx_msg void OnBnClickedAnOkSurlign();
	afx_msg void OnBnClickedAnCancelSurlign();
	afx_msg void OnBnClickedAnRadSup();
	afx_msg void OnBnClickedAnRadSupegal();
	afx_msg void OnBnClickedAnRadInf();
	afx_msg void OnBnClickedAnRadInfegal();
	afx_msg void OnBnClickedAnRadBorneinf();
	afx_msg void OnBnClickedAnModifCouleur();
	
	afx_msg void OnBnClickedAnDefdefaut();
	afx_msg void OnBnClickedAnEffacerDefdefaut();
	afx_msg void OnBnClickedAnSetParamDefaut();
	afx_msg void OnBnClickedAnRazSurlignage();
	afx_msg void OnBnClickedAnChkOutbornes();
	afx_msg void OnCbnSelchangeAnComboGrdSurlign();
	afx_msg void OnCbnSelchangeAnComboResSurlign();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
