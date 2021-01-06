/////////////////////////////////////////////////////////////////////////////////////////////////////
// Fen�tre de param�trage pour le ou les surlignages directement sur le tableau tris crois�s
//
//
// D�finition des grandeurs et r�sultats � prendre en compte
// D�fnition des fourchettes de surlignage
// D�f�nition des param�tres par d�faut
// Gestion des colueurs pour le surlignage
/////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <Label.h>
#include <BtnST.h>
#include "JFCEditDecimal.h"
#include "AnalyseurDoc.h"
#include "afxwin.h"

class COpeSurlignage;

// Bo�te de dialogue CDlgParamsSurlignage

class CDlgParamsSurlignage : public CDialog
{
	DECLARE_DYNAMIC(CDlgParamsSurlignage)

public:
	// Constructeur
	CDlgParamsSurlignage(CWnd* pParent = NULL);   

	// Destructeur
	virtual ~CDlgParamsSurlignage();

	// R�cup du document courant
	void SetParams(CAnalyseurDoc *pDoc, JMap  <JUnt32, CString> *pMapGrandeurs, JMap  <JUnt32, CString> *pMapResultats);

	/////////////////////////////////////// R�cup�ration des diff�rents param�tres positionn�s pour le surlignage //////////////////////////////////////
	// R�cup�re tous les param�tres op�ration surlignage
	const COpeSurlignage GetParam();

	// R�cup�ration couleur
	COLORREF GetColorSurlign();

	// R�cup�ration Raz (oui/non) 
	JBool GetRazSurlign();

	// R�cup�re type de param�trage valeurs
	BYTE  GetTypeParam()			{return m_TypeParamVal; };	

	// R�cup�ration valeur ou bornes
	double GetValeur1()				{return m_Valeur1; };
	double GetValeur2()				{return m_Valeur2; };

	// R�cup�ration Grandeurs � surligner
	RESULTTYPE GetParamGrandeur()	{return m_Grandeur; };
	
	
	// R�cup�ration R�sultats � surligner
	RESULTSTYLE GetParamResult()	{return m_Result; };

	// Donn�es de bo�te de dialogue
	enum { IDD = IDD_AN_DLG_PARAMS_SURLIGN };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	// Fonctions design controles
	void SetupControls();
	void SetColors();
	void SetControls();

	// D�finition des textes tooltip
	void SetTooltips();

	// R�cup�rartion des �lmts combo r�sultats et grandeurs
	void SetParamsSurlign();

	// Update valeurs de  parametrage
	void  SetTypeParamVal();

	// Test validit� param�tres valeurs
	JBool OkParamValue();

	// Positionne les param�tres par d�faut
	void SetParamDefaut();

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

protected:

	///////////////////////////////////// Boite s�lection r�sultat et grandeur pour surlignage /////////////////////////
	//
	// Boite contenant les 2 combos pour s�lection R�sultat et Grandeur de surlignage
	CLabel				m_Box_ResultEtGrandeur;

	// Titre au dessus du combox box s�lection r�sultat � surligner
	CLabel				m_Ttl_Result_Surlign;

	// Titre au dessus du combox box s�lection grandeur � surligner
	CLabel				m_Ttl_Grandeur_Surlign;

	// Combobox pour s�lectionner r�sultat � surligner
	CComboBox			m_Cmb_Result_Surlign;

	// Combobox pour s�lectionner grandeur � surligner
	CComboBox			m_Cmb_Grandeur_Surlign;

	////////////////////////////////////// Boite Valeurs et Bornes du surlignage //////////////////////////////////////
	//
	// Boite contenant les op�rateurs et valeurs pour d�finir le surlignage
	CLabel				m_Box_ValeursBornes;

	// Titre pour s�lectionner valeur ou bornes pour le surlignage
	CLabel				m_Ttl_ValeursBornes;

	// Op�rateur surlignage stictement sup�rieur
	CButtonST			m_BtnRad_Sup;

	// Op�rateur surlignage sup�rieur ou �gal
	CButtonST			m_BtnRad_SupEgal;

	// Op�rateur surlignage strictement inf�rieur
	CButtonST			m_BtnRad_Inf;

	// Op�rateur surlignage inf�rieur ou �gal
	CButtonST			m_BtnRad_InfEgal;

	// Boite d'�dition pour saisie valeur de surlignage
	JFCEditDecimal		m_EditValue;

	// Boite d'�dition pour saisie borne inf�rieure plage de surlignage
	JFCEditDecimal		m_EditBorneInf;

	// Boite d'�dition pour saisie borne sup�rieur plage de surlignage
	JFCEditDecimal		m_EditBorneSup;

	// Check pour valider ou pas la prise en compte externe de la plage de surlignage
	CButtonST			m_ChkExtBornes;
	
	// Bouton radion pour s�lection surlignage cellules via plages de valeurs
	CButtonST			m_BtnRad_Bornes;

	// S�parateur entre les 2 valeurs de la plage surlignage
	CLabel				m_Label_EntreBornes;

	// Texte % pour la borne inf�rieure de la plage
	CLabel				m_PrctBorneInf;

	// Texte % pour la borne sup�rieure de la plage
	CLabel				m_PrctBorneSup;

	// Texte % pour la valeur
	CLabel				m_PrctValeur;

	// Cadre regroupant les op�rateurs sur 1 valeur
	CLabel				m_Cadre_OpeValeur;

	// Cadre regroupant les �lmts d�finition des bornes
	CLabel				m_Cadre_Bornes;

	////////////////////////////////////// Boite D�finition de la couleur de surlignage //////////////////////////////////////
	//
	// Boite contenant controles pour d�finition couleur
	CLabel				m_Box_Couleur;

	// Titre de la boite s�lection couleur
	CLabel				m_Ttl_Couleur;

	// Cadre affichant la couleur s�lectionn�e pour le surlignage
	CLabel				m_CadreCouleur;

	// Bouton permettant la modification de la couleur de surlignage
	CButtonST			m_Btn_Modif_Couleur;

	////////////////////////////////////// Boite d�finition param�tres par d�faut /////////////////////////////////////////////
	//
	// Boite contenant controles pour param�tres par d�faut
	CLabel				m_Box_ConfigDef;

	// Titre panel configuration par d�faut
    CLabel				m_Ttl_ConfigDef;

	// Bouton validant les s�lections courantes comme param�tres par d�faut du surlignage
	CButtonST			m_Btn_ParamDefaut;

	// Bouton effa�ant la s�lection par d�faut du surlignage
	CButtonST			m_Btn_Effacer_DefDefaut;

	// Bouton affichant les param�tres par d�faut
	CButtonST			m_Btn_SetParamDefaut;

	// R�initialisation de la totalit� du surlignage
	// CButtonST			m_Btn_Raz_Surlign;

	////////////////////////////////////// Les autres contr�les //////////////////////////////////
	//
	// Bouton validant la s�lection surligange en cours
	CButtonST			m_BtnOk;

	// Bouton annulant le surlignage
	CButtonST			m_BtnCancel;

protected:
	// Le document courant
	CAnalyseurDoc *		m_pDoc;

	// R�cup�ration des �lmts grandeurs et r�sultats
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

	// R�cup�ration Grandeurs � surligner
	RESULTTYPE			m_Grandeur;
		
	// R�cup�ration R�sultats � surligner
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
