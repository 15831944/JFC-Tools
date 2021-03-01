// DlgParamsSurlignage.cpp : fichier d'impl�mentation
//

#include "stdafx.h"
#include "DlgParamsSurlignage.h"
#include "AnalyseurDoc.h"
#include "AnalyseurEnums.h"
#include ".\dlgparamssurlignage.h"

class COpeSurlignage;

// Bo�te de dialogue CDlgParamsSurlignage

IMPLEMENT_DYNAMIC(CDlgParamsSurlignage, CDialog)

//////////////////////////////////////////////////////////////////////////////////////
// Constructeur
CDlgParamsSurlignage::CDlgParamsSurlignage(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgParamsSurlignage::IDD, pParent)
{
	// Init document
	m_pDoc = 0;
	
	// Par d�faut pas de Raz Surlignage
	m_Raz_Surlignage = false;
}

//////////////////////////////////////////////////////////////////////////////////////
// Destructeur
CDlgParamsSurlignage::~CDlgParamsSurlignage()
{
	
}

///////////////////////////////////////////////////////////////////////////////
// R�cup�ration param�tres en cours pour surlignage tableau crois�
void CDlgParamsSurlignage::SetParams(CAnalyseurDoc *pDoc, JMap  <JUnt32, CString> *pMapGrandeurs, JMap  <JUnt32, CString> *pMapResultats)
{	
	// R�cup�ration du document g�n�ral analyseur
	m_pDoc			 = pDoc;

	// R�cup�ration des �lmts grandeurs et r�sultats
	m_pMapGrandeurs  = pMapGrandeurs;
	m_pMapResultats  = pMapResultats;

	///////////////////////  R�cup valeur par d�faut
	SetParamDefaut();

	if (m_pDoc->m_Lst_Ope_Surlignages.GetCount() > 0)
	{
		// Au moins 1 op�ration surlignage pr�c�dente, on remet en place la derni�re
		// 
		// R�cup derni�re op�ration
		m_pDoc->m_Lst_Ope_Surlignages.MoveTo(m_pDoc->m_Lst_Ope_Surlignages.GetCount()-1);
		if (m_pDoc->m_Lst_Ope_Surlignages.IsValid())
		{
			const COpeSurlignage & DernOpeSurlignage = m_pDoc->m_Lst_Ope_Surlignages.GetItem();  
		
			// Par d�faut s�lection crit�re sup�rieur
			m_TypeParamVal	= DernOpeSurlignage.TypeParam; 

			// Par d�faut les valeurs
			m_Valeur1		= DernOpeSurlignage.Valeur1;
			m_Valeur2		= DernOpeSurlignage.Valeur2;

			// Par d�faut grandeur = effectif
			m_Grandeur		= DernOpeSurlignage.Param_Grandeur;
				
			// Par d�faut r�sultat = r�sultat normal
			m_Result		= DernOpeSurlignage.Param_Result;

			// Par d�faut couleur de surlignage jaune
			m_ColorSurlign  = DernOpeSurlignage.Param_Color;
		}
	}
}

void CDlgParamsSurlignage::DoDataExchange(CDataExchange* pDX)
{

}

///////////////////////////////////////////////////////////////////////////////////
// Initialisation de la boite de dialogue param�trages surlignage
BOOL CDlgParamsSurlignage::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Centrage de la fen�tre
	this->CenterWindow(); 

	// On d�finit les caract�ristqies des diff�rents controles
	SetupControls();

	// Affiche les elmts liste grandeurs et r�sultats dispo
	SetParamsSurlign();

	// Par d�faut 1ere grandeurs
	if (m_Cmb_Grandeur_Surlign.GetCount())
	{
		m_Cmb_Grandeur_Surlign.SetCurSel(0);
		OnCbnSelchangeAnComboGrdSurlign();
	}

	// On positionne l'�tat des contr�les
	SetControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION�: les pages de propri�t�s OCX devraient retourner FALSE
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Fonctions design controles
void CDlgParamsSurlignage::SetupControls()
{
	// El�ments boite r�sultats et grandeurs
	m_Box_ResultEtGrandeur	.SubclassDlgItem(IDC_AN_STATIC_OBJETS_SURLIGN,		this);
	m_Ttl_Result_Surlign	.SubclassDlgItem(IDC_AN_STATIC_TTL_RES_SURLIGN,		this);
	m_Ttl_Grandeur_Surlign	.SubclassDlgItem(IDC_AN_STATIC_TTL_GRD_SURLIGN,		this);
	m_Cmb_Result_Surlign	.SubclassDlgItem(IDC_AN_COMBO_RES_SURLIGN,			this);
	m_Cmb_Grandeur_Surlign	.SubclassDlgItem(IDC_AN_COMBO_GRD_SURLIGN,			this);

	// El�ments boite s�lection valeurs et bornes
	m_Box_ValeursBornes		.SubclassDlgItem(IDC_AN_STATIC_VAL_SURLIGN,			this);
	m_Ttl_ValeursBornes		.SubclassDlgItem(IDC_AN_STATIC_TTL_VAL_SURLIGN,		this);
	m_BtnRad_Sup			.SubclassDlgItem(IDC_AN_RAD_SUP,					this);
	m_BtnRad_SupEgal		.SubclassDlgItem(IDC_AN_RAD_SUPEGAL,				this);
	m_BtnRad_Inf			.SubclassDlgItem(IDC_AN_RAD_INF,					this);
	m_BtnRad_InfEgal		.SubclassDlgItem(IDC_AN_RAD_INFEGAL,				this);
	m_EditValue				.SubclassDlgItem(IDC_AN_EDIT_VALEUR_SURLIGN,		this);
	m_BtnRad_Bornes			.SubclassDlgItem(IDC_AN_RAD_BORNEINF,				this);
	m_EditBorneInf			.SubclassDlgItem(IDC_AN_EDIT_BORNEINF_SURLIGN,		this);
	m_EditBorneSup			.SubclassDlgItem(IDC_AN_EDIT_BORNESUP_SURLIGN,		this);
	m_Label_EntreBornes		.SubclassDlgItem(IDC_AN_STATIC_ENTRE_BORNES,		this);
	m_ChkExtBornes			.SubclassDlgItem(IDC_AN_CHK_OUTBORNES,				this);
	m_PrctBorneInf			.SubclassDlgItem(IDC_AN_PRCT_BORNEINF,				this);
	m_PrctBorneSup			.SubclassDlgItem(IDC_AN_PRCT_BORNESUP,				this);
	m_PrctValeur			.SubclassDlgItem(IDC_AN_PRCT_VALEUR,				this);
	m_Cadre_OpeValeur		.SubclassDlgItem(IDC_AN_STATIC_CADRE_VAL_SURLIGN,	this);
	m_Cadre_Bornes			.SubclassDlgItem(IDC_AN_STATIC_CADRE_BORNES_SURLIGN,this);
	
	// El�ments boite s�lection couleur
	m_Box_Couleur			.SubclassDlgItem(IDC_AN_STATIC_COULEUR_SURLIGN,	this);
	m_Ttl_Couleur			.SubclassDlgItem(IDC_AN_STATIC_TTL_COULEUR,		this);
	m_CadreCouleur			.SubclassDlgItem(IDC_AN_CADRE_COULEUR,			this);
	m_Btn_Modif_Couleur		.SubclassDlgItem(IDC_AN_MODIF_COULEUR,			this);

	// El�ments boite configuratin par d�faut
	m_Box_ConfigDef			.SubclassDlgItem(IDC_AN_STATIC_CONFIG_DEFAUT,	this);
	m_Ttl_ConfigDef			.SubclassDlgItem(IDC_AN_STATIC_TTL_CONFIGDEF,	this);
	m_Btn_ParamDefaut		.SubclassDlgItem(IDC_AN_DEFDEFAUT,				this);
	m_Btn_Effacer_DefDefaut	.SubclassDlgItem(IDC_AN_EFFACER_DEFDEFAUT,		this);
	m_Btn_SetParamDefaut	.SubclassDlgItem(IDC_AN_SET_PARAM_DEFAUT,		this);	

	// El�ments contr�les divers
	m_BtnOk					.SubclassDlgItem(ID_AN_OK_SURLIGN,				this);
	m_BtnCancel				.SubclassDlgItem(ID_AN_CANCEL_SURLIGN,			this);

	// Style des radio boutons
	m_BtnRad_Sup			.SetIcon(IDI_AN_RADIOON1,	IDI_AN_RADIOOFF1);  
	m_BtnRad_Sup			.SetTextAlignement(DT_LEFT);
	m_BtnRad_Sup			.DrawBorder(false); 

	m_BtnRad_SupEgal		.SetIcon(IDI_AN_RADIOON1,	IDI_AN_RADIOOFF1);  
	m_BtnRad_SupEgal		.SetTextAlignement(DT_LEFT);
	m_BtnRad_SupEgal		.DrawBorder(false); 

	m_BtnRad_Inf			.SetIcon(IDI_AN_RADIOON1,	IDI_AN_RADIOOFF1);  
	m_BtnRad_Inf			.SetTextAlignement(DT_LEFT);
	m_BtnRad_Inf			.DrawBorder(false); 

	m_BtnRad_InfEgal		.SetIcon(IDI_AN_RADIOON1,	IDI_AN_RADIOOFF1);  
	m_BtnRad_InfEgal		.SetTextAlignement(DT_LEFT);
	m_BtnRad_InfEgal		.DrawBorder(false); 

	m_BtnRad_Bornes			.SetIcon(IDI_AN_RADIOON1,	IDI_AN_RADIOOFF1);  
	m_BtnRad_Bornes			.SetTextAlignement(DT_LEFT);
	m_BtnRad_Bornes			.DrawBorder(false);

	m_ChkExtBornes			.SetIcon(IDI_AN_CHECKON,	IDI_AN_CHECKOFF);  
	m_ChkExtBornes			.SetTextAlignement(DT_LEFT);
	m_ChkExtBornes			.DrawBorder(false); 

	// D�finition boite s�lection r�sultat et grandeur
	m_Box_ResultEtGrandeur	.Reset();
	m_Box_ResultEtGrandeur	.SetTransparent(false);
	m_Box_ResultEtGrandeur	.SetSunken(true);
	m_Box_ResultEtGrandeur	.AddTopWindow(m_Ttl_Result_Surlign);
	m_Box_ResultEtGrandeur	.AddTopWindow(m_Ttl_Grandeur_Surlign);
	m_Box_ResultEtGrandeur	.AddTopWindow(m_Cmb_Result_Surlign);
	m_Box_ResultEtGrandeur	.AddTopWindow(m_Cmb_Grandeur_Surlign);

	// D�finition boite valeurs et bornes
	m_Box_ValeursBornes		.Reset();
	m_Box_ValeursBornes		.SetTransparent(false);
	m_Box_ValeursBornes		.SetSunken(true);

	m_Cadre_OpeValeur		.SetTransparent(true);
	m_Cadre_OpeValeur		.SetSunken(true);
	m_Cadre_Bornes			.SetTransparent(true);
	m_Cadre_Bornes			.SetSunken(true);

	m_Box_ValeursBornes		.AddTopWindow(m_Ttl_ValeursBornes);
	m_Box_ValeursBornes		.AddTopWindow(m_BtnRad_Sup);
	m_Box_ValeursBornes		.AddTopWindow(m_BtnRad_SupEgal);
	m_Box_ValeursBornes		.AddTopWindow(m_BtnRad_Inf);
	m_Box_ValeursBornes		.AddTopWindow(m_BtnRad_InfEgal);
	m_Box_ValeursBornes		.AddTopWindow(m_EditValue);
	m_Box_ValeursBornes		.AddTopWindow(m_BtnRad_Bornes);
	m_Box_ValeursBornes		.AddTopWindow(m_EditBorneInf);
	m_Box_ValeursBornes		.AddTopWindow(m_EditBorneSup);
	m_Box_ValeursBornes		.AddTopWindow(m_Label_EntreBornes);
	m_Box_ValeursBornes		.AddTopWindow(m_ChkExtBornes);
	m_Box_ValeursBornes		.AddTopWindow(m_PrctBorneInf);
	m_Box_ValeursBornes		.AddTopWindow(m_PrctBorneSup);
	m_Box_ValeursBornes		.AddTopWindow(m_PrctValeur);
	m_Box_ValeursBornes		.AddTopWindow(m_Cadre_OpeValeur);
	m_Box_ValeursBornes		.AddTopWindow(m_Cadre_Bornes);

	// D�finition boite s�lection couleur
	m_Box_Couleur.Reset();
	m_Box_Couleur.SetTransparent(false);
	m_Box_Couleur.SetSunken(true);
	m_Box_Couleur.AddTopWindow(m_Ttl_Couleur);
	m_Box_Couleur.AddTopWindow(m_CadreCouleur);
	m_Box_Couleur.AddTopWindow(m_Btn_Modif_Couleur);

	// D�finition boite s�elction config par d�faut
	m_Box_ConfigDef.Reset();
	m_Box_ConfigDef.SetTransparent(false);
	m_Box_ConfigDef.SetSunken(true);
	m_Box_ConfigDef.AddTopWindow(m_Ttl_ConfigDef);
	m_Box_ConfigDef.AddTopWindow(m_Btn_ParamDefaut);
	m_Box_ConfigDef.AddTopWindow(m_Btn_Effacer_DefDefaut);
	m_Box_ConfigDef.AddTopWindow(m_Btn_SetParamDefaut);
	
	// D�finition des textes tooltip
	SetTooltips();

	// D�finition des couleurs
	SetColors();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Couleurs des diff�rents contr�les
void CDlgParamsSurlignage::SetColors()
{
	// Couleur fond des boites de s�lection
	m_Box_ResultEtGrandeur		.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORNOAPP, CATPColors::LIGHT_DEGRAD);
	m_Box_ValeursBornes			.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORNOAPP, CATPColors::LIGHT_DEGRAD);	
	m_Box_Couleur				.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORNOAPP, CATPColors::LIGHT_DEGRAD);
	m_Box_ConfigDef				.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORNOAPP, CATPColors::LIGHT_DEGRAD);

	// Couleur fond et de texte des titres
	m_Ttl_Result_Surlign		.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR, CATPColors::DARK_DEGRAD);
	m_Ttl_Result_Surlign		.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORANALYSEUR));
	
	m_Ttl_Grandeur_Surlign		.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR, CATPColors::DARK_DEGRAD);
	m_Ttl_Grandeur_Surlign		.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORANALYSEUR));

	m_Ttl_ValeursBornes			.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR, CATPColors::DARK_DEGRAD);
	m_Ttl_ValeursBornes			.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORANALYSEUR));

	m_Ttl_Couleur				.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR, CATPColors::DARK_DEGRAD);
	m_Ttl_Couleur				.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORANALYSEUR));
	m_Ttl_ConfigDef				.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR, CATPColors::DARK_DEGRAD);
	m_Ttl_ConfigDef				.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORANALYSEUR));

	// Couleur par d�faut de la couleur de surlignage
	m_CadreCouleur				.SetBkColor(m_ColorSurlign,CATPColors::COLORNOAPP, CATPColors::LIGHT_DEGRAD); 

	// Divers label
	m_Label_EntreBornes			.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORNOAPP, CATPColors::LIGHT_DEGRAD);
	m_PrctBorneInf				.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORNOAPP, CATPColors::LIGHT_DEGRAD);
	m_PrctBorneSup				.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORNOAPP, CATPColors::LIGHT_DEGRAD);
	m_PrctValeur				.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORNOAPP, CATPColors::LIGHT_DEGRAD);

	// Couleur bouton check-box
	m_ChkExtBornes				.SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORNOAPP,0);		
	m_ChkExtBornes				.SetColor(CButtonST::BTNST_COLOR_BK_OUT,	CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORNOAPP);		
	m_ChkExtBornes				.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORNOAPP);			

	// Cadre des boutons radios op�rateurs
	m_Cadre_OpeValeur			.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORNOAPP, CATPColors::LIGHT_DEGRAD);		

	// Couleur des boutons radio
	m_BtnRad_Sup				.SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORNOAPP,0);		
	m_BtnRad_Sup				.SetColor(CButtonST::BTNST_COLOR_BK_OUT,	CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORNOAPP);		
	m_BtnRad_Sup				.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORNOAPP);			

	m_BtnRad_SupEgal			.SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORNOAPP,0);		
	m_BtnRad_SupEgal			.SetColor(CButtonST::BTNST_COLOR_BK_OUT,	CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORNOAPP);		
	m_BtnRad_SupEgal			.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORNOAPP);			

	m_BtnRad_Inf				.SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORNOAPP,0);		
	m_BtnRad_Inf				.SetColor(CButtonST::BTNST_COLOR_BK_OUT,	CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORNOAPP);		
	m_BtnRad_Inf				.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORNOAPP);			

	m_BtnRad_InfEgal			.SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORNOAPP,0);		
	m_BtnRad_InfEgal			.SetColor(CButtonST::BTNST_COLOR_BK_OUT,	CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORNOAPP);		
	m_BtnRad_InfEgal			.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORNOAPP);			

	m_BtnRad_Bornes				.SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORNOAPP,0);		
	m_BtnRad_Bornes				.SetColor(CButtonST::BTNST_COLOR_BK_OUT,	CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORNOAPP);		
	m_BtnRad_Bornes				.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORNOAPP);			
}

//////////////////////////////////////////////////////////////////////////////////////////////
// D�finition des textes tooltip
void CDlgParamsSurlignage::SetTooltips()
{
	// Tooltip des boutons d fonction
	m_Btn_ParamDefaut.			SetTooltipText("D�fini les param�tres en cours comme les param�tres par d�faut");
	m_Btn_Effacer_DefDefaut.	SetTooltipText("Efface les param�tres par d�faut");
	m_Btn_SetParamDefaut.		SetTooltipText("Affiche les param�tres par d�faut");
	m_BtnOk.					SetTooltipText("Valide op�ration surlignage");
	m_BtnCancel.				SetTooltipText("Annule op�ration surlignage");

	// Les combo-box
	// m_Cmb_Result_Surlign.		SetTooltipText("S�lection du mode r�sultat pour le surlignage");
	// m_Box_ResultEtGrandeur.	    SetTooltipText("S�lection du mode grandeur pour le surlignage");
	
	// Les autres controles
	m_ChkExtBornes.				SetTooltipText("Prise en compte des bornes externes ou internes");

	// Panneau s�lection couleur
	m_Btn_Modif_Couleur.		SetTooltipText("S�lection de la couleur de surlignage");
}

/////////////////////////////////////////////////////////(/////////////////////////////////////
// Positionnement des contr�les
void CDlgParamsSurlignage::SetControls()
{
	CRect RectVal;;
	switch (m_TypeParamVal)
	{
		case m_pDoc->TYPE_SUP :
		{
			// R�cup position bouton radio "Sup"
			m_BtnRad_Sup.GetWindowRect(&RectVal);

			// Active bouton radio "Sup"
			CheckRadioButton(IDC_AN_RAD_SUP,IDC_AN_RAD_BORNEINF ,IDC_AN_RAD_SUP);
			break;
		}

		case m_pDoc->TYPE_SUP_EGAL :
		{
			// R�cup position bouton radio "Sup ou �gal"
			m_BtnRad_SupEgal.GetWindowRect(&RectVal);

			// Active bouton radio "Sup ou �gal"
			CheckRadioButton(IDC_AN_RAD_SUP,IDC_AN_RAD_BORNEINF ,IDC_AN_RAD_SUPEGAL);
			break;
		}

		case m_pDoc->TYPE_INF :
		{
			// R�cup position bouton radio "Inf"
			m_BtnRad_Inf.GetWindowRect(&RectVal);

			// Active bouton radio "Inf"
			CheckRadioButton(IDC_AN_RAD_SUP,IDC_AN_RAD_BORNEINF ,IDC_AN_RAD_INF);
			break;
		}

		case m_pDoc->TYPE_INF_EGAL :
		{
			// R�cup position bouton radio "Inf ou �gal"
			m_BtnRad_InfEgal.GetWindowRect(&RectVal);

			// Active bouton radio "Inf ou �gal"
			CheckRadioButton(IDC_AN_RAD_SUP,IDC_AN_RAD_BORNEINF ,IDC_AN_RAD_INFEGAL);
			break;
		}

		case m_pDoc->TYPE_IN_BORNES  :
		case m_pDoc->TYPE_OUT_BORNES :
			// Active bouton radio "D�f Bornes"
			CheckRadioButton(IDC_AN_RAD_SUP,IDC_AN_RAD_BORNEINF ,IDC_AN_RAD_BORNEINF);
			break;
	}

	// Repositionne les �lmts boutons radio de la partie haute
	if (m_TypeParamVal != m_pDoc->TYPE_IN_BORNES && m_TypeParamVal != m_pDoc->TYPE_OUT_BORNES)
	{
		// Hauteur boite �dition valeur unique
		CRect RectEdit, RectPrctVal;
		m_EditValue.GetWindowRect(&RectEdit);
		m_PrctValeur.GetWindowRect(&RectPrctVal);

		// Recalage de la boite �dition valeur
		RectEdit.top	= RectVal.top - 2;
		RectPrctVal.top = RectVal.top;
		this->ScreenToClient(&RectEdit);
		this->ScreenToClient(&RectPrctVal);

		m_EditValue.SetWindowPos(NULL, RectEdit.left, RectEdit.top, RectEdit.Width(), RectEdit.Height() ,SWP_NOZORDER|SWP_NOSIZE);
		m_PrctValeur.SetWindowPos(NULL, RectPrctVal.left, RectPrctVal.top, RectPrctVal.Width(), RectPrctVal.Height() ,SWP_NOZORDER|SWP_NOSIZE);
	}

	// Param�trage des boites �dition valeurs
	m_EditValue.SetNumberOfDecimals(2);
	m_EditBorneInf.SetNumberOfDecimals(2);
	m_EditBorneSup.SetNumberOfDecimals(2);
}

/* En attente (Chargement uniquement des grandeurs et r�sultats s�lectionn�s)
//////////////////////////////////////////////////////////////////////////////////////////
// Mise en place des �lmts s�lectionnables
void CDlgParamsSurlignage::SetParamsSurlign()
{
	if (m_pDoc)
	{
		//////////////////////////////////////////// Les grandeurs (Effectif, Nb cas etc ...) dispos pour le surlignage
		// Les s�lections dispos grandeur(et chacune des s�lection courante)
		m_Cmb_Grandeur_Surlign.ResetContent(); 
		for (m_pDoc->m_MapGrandeursDispos.MoveFirst(); m_pDoc->m_MapGrandeursDispos.IsValid(); m_pDoc->m_MapGrandeursDispos.MoveNext())
		{
			// R�cup�ration de l'item data (attention x 10000 pour avoir m�me ordre que dans analyseur)
			JUnt32 ItemData = m_pDoc->m_MapGrandeursDispos.GetKey() % 10000;

			// R�cup�ration du libell� grandeur
			CString LibGrandeur = m_pDoc->m_MapGrandeursDispos.GetItem();

			// Ajoute � la liste param�tres surlignage dispo
			JInt32 InxLst = m_Cmb_Grandeur_Surlign.AddString(LibGrandeur); 

			// Info ident result
			m_Cmb_Grandeur_Surlign.SetItemData(InxLst, ItemData); 
		}

		// puis on positionne la grandeur surlignage par d�faut
		if (m_Cmb_Grandeur_Surlign.GetCount() > 0)
		{
			for (JInt32 InxLst = 0; InxLst < m_Cmb_Grandeur_Surlign.GetCount(); InxLst++)	
			{
				// R�cup item data de l'�lmt
				JInt32 ItemData = m_Cmb_Grandeur_Surlign.GetItemData(InxLst);

				// Si c'est celui courant, on le s�lectionne
				if (ItemData == m_Grandeur)
				{
					m_Cmb_Grandeur_Surlign.SetCurSel(InxLst); 
					break;
				}
			}

			// Si aucune s�lection, on se positionne sur le 1er elmt de la liste des grandeurs dispos
			if (m_Cmb_Grandeur_Surlign.GetCurSel() < 0)
			{
				m_Cmb_Grandeur_Surlign.SetCurSel(0);
				JInt32 ItemData = m_Cmb_Grandeur_Surlign.GetItemData(0);
				
				RESULTTYPE ResultType = (RESULTTYPE) ItemData;
				m_pDoc->Set_GrandeurTri(ResultType); 
			}
		}

		//////////////////////////////////////////// Les r�sultats (Result, H%,V% etc ...) dispos pour le surlignage
		// Les s�lections dispos grandeur(et chacune des s�lection courante)
		m_Cmb_Result_Surlign.ResetContent();
		for (m_pDoc->m_MapResultCellDispo.MoveFirst(); m_pDoc->m_MapResultCellDispo.IsValid(); m_pDoc->m_MapResultCellDispo.MoveNext())
		{
			// R�cup�ration de l'item data 
			JUnt32 ItemData = m_pDoc->m_MapResultCellDispo.GetKey();

			// R�cup�ration du libell� r�sultat
			CString LibResult = m_pDoc->m_MapResultCellDispo.GetItem();

			// Attention si c'est la grandeur global
			if (LibResult == "Global")
			{
				// Mode calcul global standard
				if(m_pDoc->Get_ResultStyle() & RSTYLE_GBL_CORRELATION)
				{
					// LibResult = "IC";
					LibResult.LoadString(IDS_AN_ABR_CONCOMITENCE);
				}
				else if(m_pDoc->Get_ResultStyle() & RSTYLE_GBL_PROXIMITY)
				{
					// LibResult = "IP";
					LibResult.LoadString(IDS_AN_ABR_COEFFYULE); 
				}
				else if(m_pDoc->Get_ResultStyle() & RSTYLE_GBL_ZSCORE)
				{
					// LibResult = "ZSCORE";
					LibResult.LoadString(IDS_AN_ABR_ZSCORE);
				}
				else
				{
					// LibResult = "G%";
					LibResult.LoadString(IDS_AN_ABR_GLOBAL);
				}
			}

			// Ajoute � la liste param�tres tris dispo
			JInt32 InxLst = m_Cmb_Result_Surlign.AddString(LibResult); 

			// Info ident result
			m_Cmb_Result_Surlign.SetItemData(InxLst, ItemData); 
		}

		// puis on positionne le resultat surlignage en cours
		if (m_Cmb_Result_Surlign.GetCount() > 0)
		{
			for (JInt32 InxLst = 0; InxLst < m_Cmb_Result_Surlign.GetCount(); InxLst++)	
			{
				// R�cup item data de l'�lmt
				JInt32 ItemData = m_Cmb_Result_Surlign.GetItemData(InxLst);

				// Si c'est celui courant, on le s�lectionne
				if (ItemData == m_Result)
				{
					m_Cmb_Result_Surlign.SetCurSel(InxLst); 
					break;
				}
			}

			// Si aucune s�lection, on se positionne sur le 1er elmt de la liste des r�sultats dispos
			if (m_Cmb_Result_Surlign.GetCurSel() < 0)
			{
				// On s�lectionne le 1er elmt
				m_Cmb_Result_Surlign.SetCurSel(0);
				JInt32 ItemData = m_Cmb_Result_Surlign.GetItemData(0);
				
				// On met � jour le doc
				RESULTSTYLE ResultStyle = (RESULTSTYLE) ItemData; 
				m_pDoc->Set_ResultTri(ResultStyle);
			}
		}
	}

	/////////////////////////////// Les param�tres valeurs ////////////////////////////
	//
	CString Txt;
	if (m_TypeParamVal != m_pDoc->TYPE_IN_BORNES && m_TypeParamVal != m_pDoc->TYPE_OUT_BORNES)
	{
		if (m_Result == RSTYLE_HORIZONTAL || m_Result == RSTYLE_VERTICAL)
		{
			// 1 seul valeur
			Txt.Format("%.0f",m_Valeur1);
		}
		else
		{
			// 1 seul valeur
			Txt.Format("%0.2f",m_Valeur1);
		}
		m_EditValue.SetWindowText(Txt);
	}
	else
	{
		// Les 2 valeurs de bornes
		if (m_Result == RSTYLE_HORIZONTAL || m_Result == RSTYLE_VERTICAL)
		{
			Txt.Format("%.0f",m_Valeur1);
			m_EditBorneInf.SetWindowText(Txt);
			Txt.Format("%.0f",m_Valeur2);
			m_EditBorneSup.SetWindowText(Txt);
		}
		else
		{
			Txt.Format("%0.2f",m_Valeur1);
			m_EditBorneInf.SetWindowText(Txt);
			Txt.Format("%0.2f",m_Valeur2);
			m_EditBorneSup.SetWindowText(Txt);
		}

		// Positionnement check box in/out bornes
		if (m_TypeParamVal == m_pDoc->TYPE_OUT_BORNES)
			m_ChkExtBornes.SetCheck(1);
		else
			m_ChkExtBornes.SetCheck(0);

	}

	// Positionnement des checks value
	switch (m_TypeParamVal)
	{
		case m_pDoc->TYPE_SUP :
			CheckRadioButton(IDC_AN_RAD_SUP,IDC_AN_RAD_BORNEINF ,IDC_AN_RAD_SUP);
			break;

		case m_pDoc->TYPE_SUP_EGAL :
			CheckRadioButton(IDC_AN_RAD_SUP,IDC_AN_RAD_BORNEINF ,IDC_AN_RAD_SUPEGAL);
			break;

		case m_pDoc->TYPE_INF :
			CheckRadioButton(IDC_AN_RAD_SUP,IDC_AN_RAD_BORNEINF ,IDC_AN_RAD_INF);
			break;

		case m_pDoc->TYPE_INF_EGAL :
			CheckRadioButton(IDC_AN_RAD_SUP,IDC_AN_RAD_BORNEINF ,IDC_AN_RAD_INFEGAL);
			break;

		case m_pDoc->TYPE_IN_BORNES :
			CheckRadioButton(IDC_AN_RAD_SUP,IDC_AN_RAD_BORNEINF ,IDC_AN_RAD_BORNEINF);
			break;

		case m_pDoc->TYPE_OUT_BORNES :
			CheckRadioButton(IDC_AN_RAD_SUP,IDC_AN_RAD_BORNEINF ,IDC_AN_RAD_BORNEINF);
			break;
	}

	// % Visible ou invisible selon r�sultat demand�
	if (m_Result == RSTYLE_HORIZONTAL || m_Result == RSTYLE_VERTICAL)
	{
		// Les labels pourcentages visibles
		m_PrctBorneInf.ShowWindow(SW_SHOW);
		m_PrctBorneSup.ShowWindow(SW_SHOW);
		m_PrctValeur.ShowWindow(SW_SHOW);
	}
	else
	{
		// Les labels pourcentages non visibles
		m_PrctBorneInf.ShowWindow(SW_HIDE);
		m_PrctBorneSup.ShowWindow(SW_HIDE);
		m_PrctValeur.ShowWindow(SW_HIDE);
	}

	// Couleur de surlignage jaune en cours
	m_CadreCouleur.SetBkColor(m_ColorSurlign); 
}
*/

//////////////////////////////////////////////////////////////////////////////////////////
// Mise en place des �lmts s�lectionnables
void CDlgParamsSurlignage::SetParamsSurlign()
{
	if (m_pDoc)
	{
		//////////////////////////////////////////// Les grandeurs (Effectif, Nb cas etc ...) dispos pour le surlignage
		// Les s�lections dispos grandeur(et chacune des s�lection courante)
		m_Cmb_Grandeur_Surlign.ResetContent(); 
		for (m_pMapGrandeurs->MoveFirst() ;m_pMapGrandeurs->IsValid(); m_pMapGrandeurs->MoveNext())
		{
			// R�cup�ration de l'item data (attention x 10000 pour avoir m�me ordre que dans analyseur)
			JUnt32 ItemData = m_pMapGrandeurs->GetKey() % 10000;

			// R�cup�ration du libell� grandeur
			CString LibGrandeur = m_pMapGrandeurs->GetItem();

			// Ajoute � la liste param�tres surlignage dispo
			JInt32 InxLst = m_Cmb_Grandeur_Surlign.AddString(LibGrandeur); 

			// Info ident result
			m_Cmb_Grandeur_Surlign.SetItemData(InxLst, ItemData); 
		}

		/*
		m_Cmb_Grandeur_Surlign.ResetContent(); 
		for (m_pDoc->m_MapGrandeursDispos.MoveFirst(); m_pDoc->m_MapGrandeursDispos.IsValid(); m_pDoc->m_MapGrandeursDispos.MoveNext())
		{
			// R�cup�ration de l'item data (attention x 10000 pour avoir m�me ordre que dans analyseur)
			JUnt32 ItemData = m_pDoc->m_MapGrandeursDispos.GetKey() % 10000;

			// R�cup�ration du libell� grandeur
			CString LibGrandeur = m_pDoc->m_MapGrandeursDispos.GetItem();

			// Ajoute � la liste param�tres surlignage dispo
			JInt32 InxLst = m_Cmb_Grandeur_Surlign.AddString(LibGrandeur); 

			// Info ident result
			m_Cmb_Grandeur_Surlign.SetItemData(InxLst, ItemData); 
		}
		*/

		// puis on positionne la grandeur surlignage par d�faut
		if (m_Cmb_Grandeur_Surlign.GetCount() > 0)
		{
			for (JInt32 InxLst = 0; InxLst < m_Cmb_Grandeur_Surlign.GetCount(); InxLst++)	
			{
				// R�cup item data de l'�lmt
				JInt32 ItemData = m_Cmb_Grandeur_Surlign.GetItemData(InxLst);

				// Si c'est celui courant, on le s�lectionne
				if (ItemData == m_Grandeur)
				{
					m_Cmb_Grandeur_Surlign.SetCurSel(InxLst); 
					break;
				}
			}

			// Si aucune s�lection, on se positionne sur le 1er elmt de la liste des grandeurs dispos
			if (m_Cmb_Grandeur_Surlign.GetCurSel() < 0)
			{
				m_Cmb_Grandeur_Surlign.SetCurSel(0);
				JInt32 ItemData = m_Cmb_Grandeur_Surlign.GetItemData(0);
				
				RESULTTYPE ResultType = (RESULTTYPE) ItemData;
				m_pDoc->Set_GrandeurTri(ResultType); 
			}
		}


		//////////////////////////////////////////// Les r�sultats (Result, H%,V% etc ...)
		// Les s�lections dispos grandeur(et chacune des s�lection courante)
		m_Cmb_Result_Surlign.ResetContent();
		for (m_pMapResultats->MoveFirst(); m_pMapResultats->IsValid(); m_pMapResultats->MoveNext())
		{
			// R�cup�ration de l'item data 
			JUnt32 ItemData = m_pMapResultats->GetKey();

			// R�cup�ration du libell� r�sultat
			CString LibResult = m_pMapResultats->GetItem();

			// Score puls utilis�
			if (LibResult == "Score") continue;

			/*
			// Attention si c'est la grandeur global
			if (LibResult == "Global")
			{
				// Mode calcul global standard
				if(m_pDoc->Get_ResultStyle() & RSTYLE_GBL_CORRELATION)
				{
					// LibResult = "IC";
					LibResult.LoadString(IDS_AN_ABR_CONCOMITENCE);
				}
				else if(m_pDoc->Get_ResultStyle() & RSTYLE_GBL_PROXIMITY)
				{
					// LibResult = "IP";
					LibResult.LoadString(IDS_AN_ABR_COEFFYULE); 
				}
				
				else if(m_pDoc->Get_ResultStyle() & RSTYLE_GBL_ZSCORE)
				{
					// LibResult = "ZSCORE";
					LibResult.LoadString(IDS_AN_ABR_ZSCORE);

					// Plus de Zscore
					continue;
				}
				else
				{
					// LibResult = "G%";
					LibResult.LoadString(IDS_AN_ABR_GLOBAL);
				}
			}
			*/

			// Ajoute � la liste param�tres tris dispo
			JInt32 InxLst = m_Cmb_Result_Surlign.AddString(LibResult); 

			// Info ident result
			m_Cmb_Result_Surlign.SetItemData(InxLst, ItemData); 
		}

		// puis on positionne le resultat surlignage en cours
		if (m_Cmb_Result_Surlign.GetCount() > 0)
		{
			for (JInt32 InxLst = 0; InxLst < m_Cmb_Result_Surlign.GetCount(); InxLst++)	
			{
				// R�cup item data de l'�lmt
				JInt32 ItemData = m_Cmb_Result_Surlign.GetItemData(InxLst);

				// Si c'est celui courant, on le s�lectionne
				if (ItemData == m_Result)
				{
					m_Cmb_Result_Surlign.SetCurSel(InxLst); 
					break;
				}
			}

			// Si aucune s�lection, on se positionne sur le 1er elmt de la liste des r�sultats dispos
			if (m_Cmb_Result_Surlign.GetCurSel() < 0)
			{
				// On s�lectionne le 1er elmt
				m_Cmb_Result_Surlign.SetCurSel(0);
				JInt32 ItemData = m_Cmb_Result_Surlign.GetItemData(0);
				
				// On met � jour le doc
				RESULTSTYLE ResultStyle = (RESULTSTYLE) ItemData; 
				m_pDoc->Set_ResultTri(ResultStyle);
			}
		}
	}

	/////////////////////////////// Les param�tres valeurs ////////////////////////////
	//
	CString Txt;
	if (m_TypeParamVal != m_pDoc->TYPE_IN_BORNES && m_TypeParamVal != m_pDoc->TYPE_OUT_BORNES)
	{
		if (m_Result == RSTYLE_HORIZONTAL || m_Result == RSTYLE_VERTICAL)
		{
			// 1 seul valeur
			Txt.Format("%.0f",m_Valeur1);
		}
		else
		{
			// 1 seul valeur
			Txt.Format("%0.2f",m_Valeur1);
		}
		m_EditValue.SetWindowText(Txt);
	}
	else
	{
		// Les 2 valeurs de bornes
		if (m_Result == RSTYLE_HORIZONTAL || m_Result == RSTYLE_VERTICAL)
		{
			Txt.Format("%.0f",m_Valeur1);
			m_EditBorneInf.SetWindowText(Txt);
			Txt.Format("%.0f",m_Valeur2);
			m_EditBorneSup.SetWindowText(Txt);
		}
		else
		{
			Txt.Format("%0.2f",m_Valeur1);
			m_EditBorneInf.SetWindowText(Txt);
			Txt.Format("%0.2f",m_Valeur2);
			m_EditBorneSup.SetWindowText(Txt);
		}

		// Positionnement check box in/out bornes
		if (m_TypeParamVal == m_pDoc->TYPE_OUT_BORNES)
			m_ChkExtBornes.SetCheck(1);
		else
			m_ChkExtBornes.SetCheck(0);

	}

	// Positionnement des checks value
	switch (m_TypeParamVal)
	{
		case m_pDoc->TYPE_SUP :
			CheckRadioButton(IDC_AN_RAD_SUP,IDC_AN_RAD_BORNEINF ,IDC_AN_RAD_SUP);
			break;

		case m_pDoc->TYPE_SUP_EGAL :
			CheckRadioButton(IDC_AN_RAD_SUP,IDC_AN_RAD_BORNEINF ,IDC_AN_RAD_SUPEGAL);
			break;

		case m_pDoc->TYPE_INF :
			CheckRadioButton(IDC_AN_RAD_SUP,IDC_AN_RAD_BORNEINF ,IDC_AN_RAD_INF);
			break;

		case m_pDoc->TYPE_INF_EGAL :
			CheckRadioButton(IDC_AN_RAD_SUP,IDC_AN_RAD_BORNEINF ,IDC_AN_RAD_INFEGAL);
			break;

		case m_pDoc->TYPE_IN_BORNES :
			CheckRadioButton(IDC_AN_RAD_SUP,IDC_AN_RAD_BORNEINF ,IDC_AN_RAD_BORNEINF);
			break;

		case m_pDoc->TYPE_OUT_BORNES :
			CheckRadioButton(IDC_AN_RAD_SUP,IDC_AN_RAD_BORNEINF ,IDC_AN_RAD_BORNEINF);
			break;
	}

	// % Visible ou invisible selon r�sultat demand�
	if (m_Result == RSTYLE_HORIZONTAL || m_Result == RSTYLE_VERTICAL)
	{
		// Les labels pourcentages visibles
		m_PrctBorneInf.ShowWindow(SW_SHOW);
		m_PrctBorneSup.ShowWindow(SW_SHOW);
		m_PrctValeur.ShowWindow(SW_SHOW);
	}
	else
	{
		// Les labels pourcentages non visibles
		m_PrctBorneInf.ShowWindow(SW_HIDE);
		m_PrctBorneSup.ShowWindow(SW_HIDE);
		m_PrctValeur.ShowWindow(SW_HIDE);
	}

	// Couleur de surlignage jaune en cours
	m_CadreCouleur.SetBkColor(m_ColorSurlign,CATPColors::COLORNOAPP, CATPColors::LIGHT_DEGRAD); 
}

//////////////////////////////////////////////////////////////////////////////////////////
// R�cup�re tous les param�tres op�ration surlignage
const COpeSurlignage CDlgParamsSurlignage::GetParam()
{
	// Infos surlignage
	COpeSurlignage OpeSurlignage;

	// R�cup toutes les infos
	OpeSurlignage.Param_Grandeur  = m_Grandeur;
	OpeSurlignage.Param_Result    = m_Result;
	OpeSurlignage.TypeParam       = m_TypeParamVal;
	OpeSurlignage.Valeur1         = m_Valeur1;
	OpeSurlignage.Valeur2         = m_Valeur2;
	OpeSurlignage.Param_Color     = m_ColorSurlign; 

	return OpeSurlignage;
}

BEGIN_MESSAGE_MAP(CDlgParamsSurlignage, CDialog)
	ON_BN_CLICKED(ID_AN_OK_SURLIGN,				OnBnClickedAnOkSurlign)
	ON_BN_CLICKED(ID_AN_CANCEL_SURLIGN,			OnBnClickedAnCancelSurlign)
	ON_BN_CLICKED(IDC_AN_RAD_SUP,				OnBnClickedAnRadSup)
	ON_BN_CLICKED(IDC_AN_RAD_SUPEGAL,			OnBnClickedAnRadSupegal)
	ON_BN_CLICKED(IDC_AN_RAD_INF,				OnBnClickedAnRadInf)
	ON_BN_CLICKED(IDC_AN_RAD_INFEGAL,			OnBnClickedAnRadInfegal)
	ON_BN_CLICKED(IDC_AN_RAD_BORNEINF,			OnBnClickedAnRadBorneinf)
	ON_BN_CLICKED(IDC_AN_MODIF_COULEUR,			OnBnClickedAnModifCouleur)
	ON_BN_CLICKED(IDC_AN_DEFDEFAUT,				OnBnClickedAnDefdefaut)
	ON_BN_CLICKED(IDC_AN_EFFACER_DEFDEFAUT,		OnBnClickedAnEffacerDefdefaut)
	ON_BN_CLICKED(IDC_AN_SET_PARAM_DEFAUT,		OnBnClickedAnSetParamDefaut)
	ON_BN_CLICKED(IDC_AN_CHK_OUTBORNES,			OnBnClickedAnChkOutbornes)
	ON_CBN_SELCHANGE(IDC_AN_COMBO_GRD_SURLIGN,	OnCbnSelchangeAnComboGrdSurlign)
	ON_CBN_SELCHANGE(IDC_AN_COMBO_RES_SURLIGN,	OnCbnSelchangeAnComboResSurlign)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////////////
// Validation de la s�lection pour surlignage
void CDlgParamsSurlignage::OnBnClickedAnOkSurlign()
{

	// int ValApp = m_Cadre_OpeValeur.GetApp();

	// V�rifie validit� des valeurs param�trages
	if (OkParamValue())
		OnOK();
	else
		AfxMessageBox("Valeurs saisies invalides", MB_ICONEXCLAMATION);
}

///////////////////////////////////////////////////////////////////////////////////////
//
void CDlgParamsSurlignage::OnBnClickedAnCancelSurlign()
{
	OnCancel();
}

// TypeParamVal {TYPE_SUP = 0, TYPE_SUP_EGAL = 1, TYPE_INF = 2, TYPE_INF_EGAL = 3, TYPE_IN_BORNES = 4, TYPE_OUT_BORNES = 5};
////////////////////////////////////////////////////////////////////////////////////////////////
// Changement s�lection param�tre sup�rieur �
void CDlgParamsSurlignage::OnBnClickedAnRadSup()
{
	m_TypeParamVal = m_pDoc->TYPE_SUP;
	SetControls();
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Changement s�lection param�tre sup�rieur ou �gal  �
void CDlgParamsSurlignage::OnBnClickedAnRadSupegal()
{
	m_TypeParamVal = m_pDoc->TYPE_SUP_EGAL;
	SetControls();
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Changement s�lection param�tre inf�rieur �
void CDlgParamsSurlignage::OnBnClickedAnRadInf()
{
	m_TypeParamVal = m_pDoc->TYPE_INF;
	SetControls();
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Changement s�lection param�tre inf�rieur ou �gal �
void CDlgParamsSurlignage::OnBnClickedAnRadInfegal()
{
	m_TypeParamVal = m_pDoc->TYPE_INF_EGAL;
	SetControls();
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Changement s�lection param�tre s�lection bornes de valeur
void CDlgParamsSurlignage::OnBnClickedAnRadBorneinf()
{
	// Choix valeur entre les 2 bornes
	m_TypeParamVal = m_pDoc->TYPE_IN_BORNES;

	if (m_ChkExtBornes.GetCheck() > 0)
	{
		m_TypeParamVal = m_pDoc->TYPE_OUT_BORNES;	
	}

	SetControls();
}

////////////////////////////////////////////////////////////////////////////////////////////////
// S�lection entre les bornes ou en dehors
void CDlgParamsSurlignage::OnBnClickedAnChkOutbornes()
{
	m_TypeParamVal = m_pDoc->TYPE_IN_BORNES;	
	if (m_ChkExtBornes.GetCheck() > 0)
	{
		m_TypeParamVal = m_pDoc->TYPE_OUT_BORNES;	
	}

	SetControls();
}

///////////////////////////////////////////////////////////////////////////////////////////////
// S�lection d'une nouvelle couleur
void CDlgParamsSurlignage::OnBnClickedAnModifCouleur()
{
	// Instance boite de dialogue couleur
	CColorDialog ColorDialog(m_ColorSurlign);

	// Calcul position fen�tre parametrage
	/*
	CRect RectFen;
	CPoint pt;
	this->GetWindowRect(RectFen);
	pt.x = RectFen.right; 
	pt.y += RectFen.top; 

	// Repositionne la fenetre s�lection couleur
	ColorDialog.ShowWindow(SW_SHOWNORMAL); 
	ColorDialog.SetWindowPos(&CWnd::wndTop, pt.x , pt.y, 0, 0, SWP_NOSIZE);
	*/

	// Affichage de la boite s�lection couleur
	if (ColorDialog.DoModal())
	{
		// R�cup�re la couleur
		m_ColorSurlign = ColorDialog.GetColor(); 
		m_CadreCouleur.SetBkColor(m_ColorSurlign,CATPColors::COLORNOAPP, CATPColors::LIGHT_DEGRAD); 
	}
}

//////////////////////////////////////////////////////////////////////////////////////
// D�finition des param�tres en cours par d�faut
void CDlgParamsSurlignage::OnBnClickedAnDefdefaut()
{
	// R�cup�re les infos
	SetTypeParamVal();

	// Stockage des informations param�tres par d�faut
	m_pDoc->m_pDefault_OpeSurlignage->Param_Color		= m_ColorSurlign;
	m_pDoc->m_pDefault_OpeSurlignage->Param_Grandeur	= m_Grandeur;
	m_pDoc->m_pDefault_OpeSurlignage->Param_Result		= m_Result;
	m_pDoc->m_pDefault_OpeSurlignage->TypeParam			= m_TypeParamVal;
	m_pDoc->m_pDefault_OpeSurlignage->Valeur1			= m_Valeur1;
	m_pDoc->m_pDefault_OpeSurlignage->Valeur2			= m_Valeur2;
}

//////////////////////////////////////////////////////////////////////////////////////
// D�truire les param�tres par d�faut
void CDlgParamsSurlignage::OnBnClickedAnEffacerDefdefaut()
{
	// Stockage des informations param�tres par d�faut
	m_pDoc->m_pDefault_OpeSurlignage->Param_Color		= 0;
	m_pDoc->m_pDefault_OpeSurlignage->Param_Grandeur	= RTYPE_NOTYPE;
	m_pDoc->m_pDefault_OpeSurlignage->Param_Result		= RSTYLE_NOSTYLE;
	m_pDoc->m_pDefault_OpeSurlignage->TypeParam			= 0;
	m_pDoc->m_pDefault_OpeSurlignage->Valeur1			= 0.0;
	m_pDoc->m_pDefault_OpeSurlignage->Valeur2			= 0.0;
}

//////////////////////////////////////////////////////////////////////////////////////
// Affiche les param�tres par d�faut dans la boite
void CDlgParamsSurlignage::OnBnClickedAnSetParamDefaut()
{
	// Positionne les param�tres par d�faut
	SetParamDefaut();

	// Affiche les params
	SetParamsSurlign();
}

//////////////////////////////////////////////////////////////////////////////////////
// Affichage par d�faut
void CDlgParamsSurlignage::SetParamDefaut()
{
	if (m_pDoc)
	{
		// Par d�faut s�lection crit�re sup�rieur
		m_TypeParamVal	= m_pDoc->m_pDefault_OpeSurlignage->TypeParam;

		// Par d�faut les valeurs
		m_Valeur1		= m_pDoc->m_pDefault_OpeSurlignage->Valeur1;
		m_Valeur2		= m_pDoc->m_pDefault_OpeSurlignage->Valeur2;

		// Par d�faut grandeur = effectif
		m_Grandeur		= m_pDoc->m_pDefault_OpeSurlignage->Param_Grandeur;
			
		// Par d�faut r�sultat = r�sultat normal
		m_Result		= m_pDoc->m_pDefault_OpeSurlignage->Param_Result;

		// Par d�faut couleur de surlignage jaune
		m_ColorSurlign = m_pDoc->m_pDefault_OpeSurlignage->Param_Color;

	}
}

/////////////////////////////////////////////////////////////////////////////////////
// Supprimer tous les �lmts de surlignage, et retour au grid
void CDlgParamsSurlignage::OnBnClickedAnRazSurlignage()
{
	// Flag Raz
	m_Raz_Surlignage = true;

	// Retour aux tris crois�s
	OnOK();
}

/////////////////////////////////////////////////////////////////////////////////////
// R�cup�ration Raz Surlignage
JBool CDlgParamsSurlignage::GetRazSurlign()
{
	return m_Raz_Surlignage;
}

/////////////////////////////////////////////////////////////////////////////////////
// R�cup�ration Couleur
COLORREF CDlgParamsSurlignage::GetColorSurlign()
{
	return m_ColorSurlign;
}

/////////////////////////////////////////////////////////////////////////////////////
// R�cup�ration type de valeur parametrage, et valeurs
void  CDlgParamsSurlignage::SetTypeParamVal()
{
	// Tempo Text
	CString Txt;

	// Init les valeurs de retour
	m_Valeur1 = 0.0;
	m_Valeur2 = 0.0;

	if (m_TypeParamVal != m_pDoc->TYPE_IN_BORNES && m_TypeParamVal != m_pDoc->TYPE_OUT_BORNES)
	{
		// 1 seul valeur
		m_EditValue.GetWindowText(Txt);
		m_Valeur1 = atof(Txt);
	}
	else
	{
		// Les 2 valeurs de bornes
		// Valeur Borne inf�rieure
		m_EditBorneInf.GetWindowText(Txt);
		m_Valeur1 = atof(Txt);
		
		// Valeur borne sup�rieure
		m_EditBorneSup.GetWindowText(Txt);
		m_Valeur2 = atof(Txt);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
// V�rification des valeurs saisies
JBool CDlgParamsSurlignage::OkParamValue()
{
	// R�cup valeur et type de saisie
	SetTypeParamVal();

	// Saisie avec 1 seule valeur
	if (m_TypeParamVal != m_pDoc->TYPE_IN_BORNES && m_TypeParamVal != m_pDoc->TYPE_OUT_BORNES)	
	{
		if (m_Valeur1 < 0)
			return false;

		return true;
	}
	else
	// Saisie par bornes
	{
		if (m_Valeur2 <= m_Valeur1)
		{
			return false;
		}	
		return true;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Changement de grandeur de surlignage
void CDlgParamsSurlignage::OnCbnSelchangeAnComboGrdSurlign()
{
	JInt32 InxSel = m_Cmb_Grandeur_Surlign.GetCurSel();
	JInt32 ItemData = m_Cmb_Grandeur_Surlign.GetItemData(InxSel);

	m_Grandeur = (RESULTTYPE) (m_Cmb_Grandeur_Surlign.GetItemData(InxSel) % 10000);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Changement de r�sultat de surlignage
void CDlgParamsSurlignage::OnCbnSelchangeAnComboResSurlign()
{
	// Change la s�lection mode r�sultat (Result, H%, V% etx....)
	JInt32 InxSel = m_Cmb_Result_Surlign.GetCurSel();
	m_Result = (RESULTSTYLE) m_Cmb_Result_Surlign.GetItemData(InxSel);

	// Mise � jour affichage des contr�les (pour les % vibles ou non visibles
	SetParamsSurlign();
}

//void CDlgParamsSurlignage::OnSize(UINT nType, int cx, int cy)
//{
//	CDialog::OnSize(nType, cx, cy);
//
//	// TODO : ajoutez ici le code de votre gestionnaire de messages
//
//	/*
//	// Resize de la boite SAS
//	CRect RectBoxSAS;
//	m_Box_SAS.GetWindowRect(&RectBoxSAS);
//	RectBoxSAS.bottom = RectBoxSeg.top - BordureJfc;
//	this->ScreenToClient(&RectBoxSAS);
//	m_Box_SAS.SetWindowPos(NULL, RectBoxSAS.left,	RectBoxSAS.top,
//									RectBoxSAS.Width(),RectBoxSAS.Height() ,SWP_NOZORDER|SWP_NOMOVE);
//	*/
//}

BOOL CDlgParamsSurlignage::PreTranslateMessage(MSG* pMsg)
{
	// TODO : ajoutez ici votre code sp�cialis� et/ou l'appel de la classe de base

	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgParamsSurlignage::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO : ajoutez ici le code de votre gestionnaire de messages
}
