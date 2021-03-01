// on inclut les définitions nécessaires
#include "stdafx.h"
#include "resource.h"
#include "ATPEtude.h"
#include "ATPLibelle.h"
#include "ATPPeriodeAction.h"
#include "ATPPeriode.h"
#include "ATPVersion.h"
#include "DialogChoixOffre.h"
#include "DialogFormat.h"
#include "DlgVersionPlan.h"
#include "DlgCiblePondere.h"
#include "ATPClipboard.h"
#include ".\atpbaseframe.h"
#include "ATPExpCsvTestPlan.h"
#include "ATPExpCsvPlanCalend.h"
#include "ATPExportEchelle.h"
#include "JATPDocCreuset.h"
#include "DialogVentilation.h"
#include "afxadv.h"
#include "ExcelPrinter.h"
#include "JATPDocExport.h"
#include "JMFCStream.h"
#include "Offre2MainDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// on définit la taille des splits
static const LONG g_CXSplit = 3;
static const LONG g_CYSplit = 3;

#define IDM_CR_COPIEHYP_LAST IDM_CR_COPIEHYP_FIRST+12
#define IDM_CR_COLLERHYP_LAST IDM_CR_COLLERHYP_FIRST+12
#define IDM_CR_ADDFROMHYP_LAST IDM_CR_ADDFROMHYP_FIRST+12
#define IDM_CR_ADDTOHYP_LAST IDM_CR_ADDTOHYP_FIRST+12

//////////////////
// le constructeur

CATPBaseFrame::CATPBaseFrame(JATPDocCreuset *pDoc)
{
	// on initialise le frame du document
	m_pATPDocCreuset = pDoc;

	// on initialise l'index couleur du frame
	m_CATPColorsApp = CATPColors::COLORCREUSET;	
}

//////////////////
// le destructeur

CATPBaseFrame::~CATPBaseFrame()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPBaseFrame, JFCFrame)
	//{{AFX_MSG_MAP(CATPBaseFrame)
	ON_WM_CREATE()
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
	ON_UPDATE_COMMAND_UI(IDM_CR_ETUDE_NOUVELLE,			OnUpdateEtudeNouvelle)
	ON_UPDATE_COMMAND_UI(IDM_CR_ETUDE_OUVRIR,			OnUpdateEtudeOuvrir)
	ON_UPDATE_COMMAND_UI(IDM_CR_ETUDE_ENREGISTRER,		OnUpdateEtudeEnregistrer)
	ON_UPDATE_COMMAND_UI(IDM_CR_ETUDE_ENREGISTRER_SOUS, OnUpdateEtudeEnregistrerSous)
	ON_UPDATE_COMMAND_UI(IDM_CR_ETUDE_FERMER,			OnUpdateEtudeFermer)
	ON_UPDATE_COMMAND_UI(IDM_CR_PERIMETRE_PERIODE,		OnUpdatePerimetrePeriode)
	ON_UPDATE_COMMAND_UI(IDM_CR_HYPOTH_VALID,			OnUpdateValidHypothese)
	ON_UPDATE_COMMAND_UI(IDM_CR_TRANSFERT_PLANREF,		OnUpdateMasterPlanToHypothese)
	ON_UPDATE_COMMAND_UI(IDM_CR_IMPORT_PLATEAU,			OnUpdateImporterPlateau)

	ON_COMMAND(IDM_CR_ETUDE_NOUVELLE,					OnEtudeNouvelle)
	ON_COMMAND(IDM_CR_ETUDE_OUVRIR,						OnEtudeOuvrir)
	ON_COMMAND(IDM_CR_ETUDE_ENREGISTRER,				OnEtudeEnregistrer)
	ON_COMMAND(IDM_CR_ETUDE_ENREGISTRER_SOUS,			OnEtudeEnregistrerSous)
	ON_COMMAND(IDM_CR_ETUDE_FERMER,						OnEtudeFermer)
	ON_COMMAND(IDM_CR_PERIMETRE_PERIODE,				OnPerimetrePeriode)
	ON_COMMAND(ID_APP_ABOUT,							OnVersion)
	ON_COMMAND(IDM_CR_OFFRE_SOURCES,					OnOffreSources)
	ON_COMMAND(IDM_CR_OFFRE_SUPPORTS,					OnOffreSupports)
	ON_COMMAND(IDM_CR_OFFRE_CIBLES,						OnOffreCibles)
	ON_COMMAND(IDM_CR_OFFRE_FORMATS,					OnOffreFormat)
	ON_COMMAND(IDM_CR_HYPOTH_VALID,						OnValidHypothese)
	ON_COMMAND(IDM_CR_TRANSFERT_PLANREF,				OnMasterPlanToHypothese)
	ON_COMMAND(IDM_CR_IMPORT_PLATEAU,					OnImporterPlateau)

	ON_COMMAND(IDM_CR_PALETTE_1,						OnPalette1)
	ON_COMMAND(IDM_CR_PALETTE_2,						OnPalette2)
	ON_COMMAND(IDM_CR_PALETTE_3,						OnPalette3)
	ON_COMMAND(IDM_CR_PALETTE_4,						OnPalette4)
	ON_COMMAND(IDM_CR_PALETTE_5,						OnPalette5)
	ON_COMMAND(IDM_CR_PALETTE_6,						OnPalette6)
	ON_COMMAND(IDM_CR_PALETTE_7,						OnPalette7)
	ON_COMMAND(IDM_CR_PALETTE_8,						OnPalette8)
	ON_COMMAND(IDM_CR_PALETTE_9,						OnPalette9)
	ON_COMMAND(IDM_CR_PALETTE_10,						OnPalette10)
	ON_COMMAND(IDM_CR_PALETTE_11,						OnPalette11)
	ON_COMMAND(IDM_CR_PALETTE_12,						OnPalette12)
	ON_COMMAND(IDM_CR_PALETTE_13,						OnPalette13)
	ON_COMMAND(IDM_CR_PALETTE_14,						OnPalette14)
	ON_COMMAND(IDM_CR_PALETTE_15,						OnPalette15)
	
	ON_UPDATE_COMMAND_UI(IDM_CR_PALETTE_1,				OnUpdateCrPalette1)
	ON_UPDATE_COMMAND_UI(IDM_CR_PALETTE_2,				OnUpdateCrPalette2)
	ON_UPDATE_COMMAND_UI(IDM_CR_PALETTE_3,				OnUpdateCrPalette3)
	ON_UPDATE_COMMAND_UI(IDM_CR_PALETTE_4,				OnUpdateCrPalette4)
	ON_UPDATE_COMMAND_UI(IDM_CR_PALETTE_5,				OnUpdateCrPalette5)
	ON_UPDATE_COMMAND_UI(IDM_CR_PALETTE_6,				OnUpdateCrPalette6)
	ON_UPDATE_COMMAND_UI(IDM_CR_PALETTE_7,				OnUpdateCrPalette7)
	ON_UPDATE_COMMAND_UI(IDM_CR_PALETTE_8,				OnUpdateCrPalette8)
	ON_UPDATE_COMMAND_UI(IDM_CR_PALETTE_9,				OnUpdateCrPalette9)
	ON_UPDATE_COMMAND_UI(IDM_CR_PALETTE_10,				OnUpdateCrPalette10)
	ON_UPDATE_COMMAND_UI(IDM_CR_PALETTE_11,				OnUpdateCrPalette11)
	ON_UPDATE_COMMAND_UI(IDM_CR_PALETTE_12,				OnUpdateCrPalette12)
	ON_UPDATE_COMMAND_UI(IDM_CR_PALETTE_13,				OnUpdateCrPalette13)
	ON_UPDATE_COMMAND_UI(IDM_CR_PALETTE_14,				OnUpdateCrPalette14)
	ON_UPDATE_COMMAND_UI(IDM_CR_PALETTE_15,				OnUpdateCrPalette15)
		

	ON_COMMAND(IDM_CR_CHROMIE,							OnChromie)
	ON_UPDATE_COMMAND_UI(IDM_CR_CHROMIE,				OnUpdateChromie)
	ON_COMMAND(IDM_CR_INSERTSTATUT,						OnStatutInsert)
	ON_UPDATE_COMMAND_UI(IDM_CR_INSERTSTATUT,			OnUpdateStatutInsert)
	ON_COMMAND(IDM_CR_AFFOPTMOTEUR,						OnStatutMoteur)
	ON_UPDATE_COMMAND_UI(IDM_CR_AFFOPTMOTEUR,			OnUpdateStatutMoteur)

	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_COMMAND(IDM_CR_MODEL,							OnOptionsMoteurAbaque)
	ON_UPDATE_COMMAND_UI(IDM_CR_MODEL,					OnUpdateOptionsMoteurAbaque)
	ON_COMMAND(IDM_CR_MODELETAL,						OnOptionsMoteurAbaqueEtal)
	ON_UPDATE_COMMAND_UI(IDM_CR_MODELETAL,				OnUpdateOptionsMoteurAbaqueEtal)
	ON_COMMAND(IDM_CR_PROBABILIT,						OnOptionsMoteurProba)
	ON_UPDATE_COMMAND_UI(IDM_CR_PROBABILIT,				OnUpdateOptionsMoteurProba)

	ON_COMMAND(IDM_CR_OPT_LNMMAGAZINE,					OnOptionsLNMMagazine)
	ON_UPDATE_COMMAND_UI(IDM_CR_OPT_LNMMAGAZINE,		OnUpdateOptionsLNMMagazine)
	ON_COMMAND(IDM_CR_OPT_LDPQUOTIDIEN,					OnOptionsLDPQuotidien)
	ON_UPDATE_COMMAND_UI(IDM_CR_OPT_LDPQUOTIDIEN,		OnUpdateOptionsLDPQuotidien)

	ON_COMMAND(IDM_CR_REPRISES,							OnOptionsRepriseEnMain)
	ON_UPDATE_COMMAND_UI(IDM_CR_REPRISES,				OnUpdateOptionsRepriseEnMain)
	ON_COMMAND(IDM_CR_NOREPRISES,						OnOptionsNoRepriseEnMain)
	ON_UPDATE_COMMAND_UI(IDM_CR_NOREPRISES,				OnUpdateOptionsNoRepriseEnMain)

	ON_COMMAND(IDM_CR_SUPPINS_HYPACT,					OnSuppinsHypact)
	ON_COMMAND(IDM_CR_SUPPINS_TOUTESHYPS,				OnSuppinsTouteshyps)
	ON_UPDATE_COMMAND_UI(IDM_CR_SUPPINS_TOUTESHYPS,		OnUpdateSuppinsTouteshyps)
	ON_UPDATE_COMMAND_UI(IDM_CR_SUPPINS_HYPACT,			OnUpdateSuppinsHypact)
	
	ON_WM_MENUSELECT()
	
	ON_COMMAND(IDM_CR_CREER_VERSION_PLANREF,			OnCreerVersionPlanref)
	ON_UPDATE_COMMAND_UI(IDM_CR_CREER_VERSION_PLANREF,	OnUpdateCreerVersionPlanref)
	ON_COMMAND(IDM_CR_CHARGER_VERSION_PLANREF,			OnChargerVersionPlanref)
	ON_UPDATE_COMMAND_UI(IDM_CR_CHARGER_VERSION_PLANREF,OnUpdateChargerVersionPlanref)
	ON_COMMAND(IDM_CR_PERIODE_ETUDE,					OnPeriodeEtude)
	ON_UPDATE_COMMAND_UI(IDM_CR_PERIODE_ETUDE,			OnUpdatePeriodeEtude)

	//Export fichier
	ON_COMMAND(IDM_CR_EXPORT_ECHELLE,					OnExportEchelle)
	ON_UPDATE_COMMAND_UI(IDM_CR_EXPORT_ECHELLE,			OnUpdateExportEchelle)
	ON_COMMAND(IDM_CR_EXPORT_RESULTPLAN,				OnExportResultPlan)
	ON_UPDATE_COMMAND_UI(IDM_CR_EXPORT_RESULTPLAN,		OnUpdateExportResult)
	ON_COMMAND(IDM_CR_EXPORT_RESULTCALEND,				OnExportResultCalend)
	ON_UPDATE_COMMAND_UI(IDM_CR_EXPORT_RESULTCALEND,	OnUpdateExportResult)
	ON_COMMAND(IDM_CR_EXPORT_TOUT,						OnExportTout)
	ON_UPDATE_COMMAND_UI(IDM_CR_EXPORT_TOUT,			OnUpdateExportTout)

	// Export Excel (ouvrir)
	ON_COMMAND(IDM_CR_EXCEL_ECHELLE,					OnExcelEchelle)
	ON_UPDATE_COMMAND_UI(IDM_CR_EXCEL_ECHELLE,			OnUpdateExcelEchelle)
	ON_COMMAND(IDM_CR_EXCEL_RESULTPLAN,					OnExcelResultPlan)
	ON_UPDATE_COMMAND_UI(IDM_CR_EXCEL_RESULTPLAN,		OnUpdateExportResult)	
	ON_COMMAND(IDM_CR_EXCEL_RESULTCALEND,				OnExcelResultCalend)
	ON_UPDATE_COMMAND_UI(IDM_CR_EXCEL_RESULTCALEND,		OnUpdateExportResult)
	ON_COMMAND(IDM_CR_EXCEL_TOUT,						OnExcelTout)
	ON_UPDATE_COMMAND_UI(IDM_CR_EXCEL_TOUT,				OnUpdateExcelTout)
	

	//Export Presse papier
	ON_COMMAND(IDM_CR_CLIPBOARD_RESULTPLAN,				OnClipboardResultPlan)
	ON_UPDATE_COMMAND_UI(IDM_CR_CLIPBOARD_RESULTPLAN,	OnUpdateExportResult)
	ON_COMMAND(IDM_CR_CLIPBOARD_RESULTCALEND,			OnClipboardResultCalend)
	ON_UPDATE_COMMAND_UI(IDM_CR_CLIPBOARD_RESULTCALEND, OnUpdateExportResult)
	ON_COMMAND(IDM_CR_CLIPBOARD_ECHELLE,				OnClipboardEchelle)
	ON_UPDATE_COMMAND_UI(IDM_CR_CLIPBOARD_ECHELLE,		OnUpdateExportEchelle)
	ON_COMMAND(IDM_CR_CLIPBOARD_TOUT,					OnClipboardTout)
	ON_UPDATE_COMMAND_UI(IDM_CR_CLIPBOARD_TOUT,			OnUpdateExportTout)

	//Impression 
	ON_COMMAND(IDM_CR_PRINT_RESULTPLAN,					OnPrintResultPlan)
	ON_UPDATE_COMMAND_UI(IDM_CR_PRINT_RESULTPLAN,		OnUpdateExportResult)
	ON_COMMAND(IDM_CR_PRINT_RESULTCALEND,				OnPrintResultCalend)
	ON_UPDATE_COMMAND_UI(IDM_CR_PRINT_RESULTCALEND,		OnUpdateExportResult)
	ON_COMMAND(IDM_CR_PRINT_ECHELLE,					OnPrintEchelle)
	ON_UPDATE_COMMAND_UI(IDM_CR_PRINT_ECHELLE,			OnUpdateExportEchelle)
	ON_COMMAND(IDM_CR_PRINT_TOUT,						OnPrintTout)
	ON_UPDATE_COMMAND_UI(IDM_CR_PRINT_TOUT,				OnUpdateExportTout)

	//
	ON_COMMAND(IDM_CR_NAMEHYPOTH, OnRenameHypo)
	ON_UPDATE_COMMAND_UI(IDM_CR_NAMEHYPOTH,				OnUpdateRenameHypo)
	
	ON_UPDATE_COMMAND_UI(IDM_CR_OFFRE_FORMATS,			OnUpdateOffreFormats)
	ON_UPDATE_COMMAND_UI(IDM_CR_OFFRE_SOURCES,			OnUpdateOffreSources)
	ON_UPDATE_COMMAND_UI(IDM_CR_OFFRE_SUPPORTS,			OnUpdateOffreSupports)
	ON_UPDATE_COMMAND_UI(IDM_CR_OFFRE_CIBLES,			OnUpdateOffreCibles)
	ON_COMMAND(IDM_CR_OPTIONS_LDPBIMESTR,				OnOptionsLDP30j)
	ON_UPDATE_COMMAND_UI(IDM_CR_OPTIONS_LDPBIMESTR,		OnUpdateOptionsLDP30j)
	ON_COMMAND(IDM_CR_OFFRE_ITEMSDEVENTILATION,			OnOffreItemsVentilation)
	ON_UPDATE_COMMAND_UI(IDM_CR_OFFRE_ITEMSDEVENTILATION, OnUpdateOffreItemsVentilation)
	ON_WM_SYSCOLORCHANGE()
	ON_COMMAND(IDM_CR_NBD_AUCUNEDECIMALE,				OnAucunedecimale)
	ON_COMMAND(IDM_CR_NBD_1DECIMALE,					On1decimale)
	ON_COMMAND(IDM_CR_NBD_2DECIMALE,					On2decimale)
	ON_UPDATE_COMMAND_UI(IDM_CR_NBD_AUCUNEDECIMALE,		OnUpdateNbddecimale)
	ON_UPDATE_COMMAND_UI(IDM_CR_NBD_1DECIMALE,			OnUpdateNbddecimale)
	ON_UPDATE_COMMAND_UI(IDM_CR_NBD_2DECIMALE,			OnUpdateNbddecimale)
	ON_COMMAND(IDM_CR_COMPUTEREALTIME,					OnComputeRealTime)
	ON_UPDATE_COMMAND_UI(IDM_CR_COMPUTEREALTIME,		OnUpdateComputeRealTime)
	ON_COMMAND(IDM_CR_COMPUTEDIFF,						OnComputeDiff)
	ON_UPDATE_COMMAND_UI(IDM_CR_COMPUTEDIFF,			OnUpdateComputeDiff)

	ON_COMMAND(IDM_CR_HYPO_COPIER,						OnCopier)
	ON_UPDATE_COMMAND_UI(IDM_CR_HYPO_COPIER,			OnUpdateCopier)

	ON_COMMAND(IDM_CR_HYPO_COLLER,						OnColler)
	ON_UPDATE_COMMAND_UI(IDM_CR_HYPO_COLLER,			OnUpdateColler)
	ON_COMMAND(IDM_CR_HYPO_ADD_1,						OnCollerAdd1)
	ON_UPDATE_COMMAND_UI(IDM_CR_HYPO_ADD_1,				OnUpdateColler)
	ON_COMMAND(IDM_CR_HYPO_ADD_2,						OnCollerAdd2)
	ON_UPDATE_COMMAND_UI(IDM_CR_HYPO_ADD_2,				OnUpdateColler)

	ON_COMMAND(IDM_CR_OPT_FILTRE_AUDIENCE,				OnCrOptFiltreAudience)
	ON_UPDATE_COMMAND_UI(IDM_CR_OPT_FILTRE_AUDIENCE,	OnUpdateCrOptFiltreAudience)
	ON_COMMAND(IDM_CR_OPT_COURBE_REPONSE,				OnCrOptCourbeReponse)
	ON_UPDATE_COMMAND_UI(IDM_CR_OPT_COURBE_REPONSE,		OnUpdateCrOptCourbeReponse)
	
	// Gestion des cibles pondérées
	ON_COMMAND(IDM_CR_CREER_CIBLEPOND,					OnCrCreerCiblepond)
	ON_UPDATE_COMMAND_UI(IDM_CR_CREER_CIBLEPOND,		OnUpdateCrCreerCiblepond)
	ON_COMMAND(IDM_CR_UTIL_CIBLEPOND,					OnCrUtilCiblepond)
	ON_UPDATE_COMMAND_UI(IDM_CR_UTIL_CIBLEPOND,			OnUpdateCrUtilCiblepond)
	END_MESSAGE_MAP()

//////////////////////////////////////////////////
// les fonctions pour charger/sauver les documents

BOOL CATPBaseFrame::OnLoadEtude( JATPDocCreuset & ATPDocCreuset, CString path, bool bDialog)
{
	// on initialise l'indicateur
	BOOL Ret = FALSE;
	JMFCStream* stream = NULL;

	if (bDialog)
	{
		CString ext, szFilters;
		if(ATPDocCreuset.m_AFFCreuset.GetAffQuickPlan())//ouverture dans quick plan
		{
			ext.LoadString(IDS_CR_EXT_QP);
			szFilters.LoadString(IDS_CR_FILTER_QP);
		}
		else
		{
			ext.LoadString(IDS_CR_EXT);
			szFilters.LoadString(IDS_CR_FILTER);
		}

		DWORD flags = OFN_NOCHANGEDIR | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY ;
		CFileDialog fileDlg(TRUE, ext, 0, flags, szFilters, this);

		if( fileDlg.DoModal() == IDOK )
			path = fileDlg.GetPathName();
		else
			return FALSE;
	}
	
	UpdateWindow();
	SetCurrentDirectory(AfxGetApp()->GetProfileString("Data", "SourcesPath", 0));

	try
	{
		CFile file(path, CFile::modeRead);
		CArchive ar(&file, CArchive::load);
		stream = JMFCStream::Create(ar);
		ATPDocCreuset.Recv(*stream);
		ATPDocCreuset.m_KEYEtude.SetPath(path);
	}
	catch(...)
	{
	}

	if (stream)
		delete stream;

	if (ATPDocCreuset.IsValid())
	{
		// Attention si mode cible pondéré actif, et aucune cible pondéré
		// CItemsConstruits * pItemsConstruits = CItemsConstruits::GetInstance();

		// if (ATPDocCreuset.m_CiblesPondsActives && pItemsConstruits->GetNbItemsPonderes() <= 0)
		if (ATPDocCreuset.m_CiblesPondsActives && CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetNbItemsPonderes() <= 0)
		{
			// On se remet sur cible ensemble
			ATPDocCreuset.m_CiblesPondsActives = false;

			// Affichage prévention passage à cible ensemble
			AfxMessageBox("La cible pondérée de cette étude n'existe plus, on prend par défaut la cible ensemble", MB_ICONINFORMATION);
		}

		JDate DateF_QP(30,12,2055);
		if(ATPDocCreuset.m_AFFCreuset.GetAffQuickPlan())//ouverture dans quick plan
		{
			if(ATPDocCreuset.m_KEYEtude.GetDateF() != DateF_QP )
			{
				//Correction de la date de fin
				ATPDocCreuset.m_KEYEtude.SetDateF(DateF_QP);
				ATPDocCreuset.m_KEYPeriodeAction.SetPeriode(ATPDocCreuset.m_KEYEtude.GetDateD(),DateF_QP);
				ATPDocCreuset.m_KEYPeriodeActionHyp.SetPeriode(ATPDocCreuset.m_KEYEtude.GetDateD(),DateF_QP);
				ATPDocCreuset.m_KEYPeriodeActionTablier.SetPeriode(ATPDocCreuset.m_KEYEtude.GetDateD(),DateF_QP);
				ATPDocCreuset.m_MoteurPresse.SetType(CMoteurPresse::PROBA);
			}

			//Test filtre d'audience
			if(ATPDocCreuset.m_IdFiltreAudienceSel != 1)
			{
				::AfxMessageBox("Cette étude ne peut pas être ouverte dans Quick Plan.\nLe fitre d'audience n'est pas celui par défaut.", MB_ICONEXCLAMATION | MB_OK);
				return FALSE;
			}
			
			//Test plan maitre
			ATPDocCreuset.m_TblHypotheses.MoveFirst();
			if(ATPDocCreuset.m_TblHypotheses.GetItem().GetCount() != 0)
			{
				::AfxMessageBox("Cette étude ne peut pas être ouverte dans Quick Plan.\nIl y a un plan maître.", MB_ICONEXCLAMATION | MB_OK);
				return FALSE;
			}
			
			//TODO Contexte de lecture

			//Test des formats
			if(ATPDocCreuset.m_Offre.m_TBLIdFormats.GetCount() > 1)
			{
				if(::AfxMessageBox("\tÉtude Creuset\nVoulez-vous convertir cette étude du Creuset en étude Quick Plan ?\nVous allez perdre les formats des insertions.", MB_ICONQUESTION | MB_YESNO) != IDYES)
					return FALSE;

				//application du format virtuel
				ATPDocCreuset.m_listFormatsQP.Swap(ATPDocCreuset.m_Offre.m_TBLIdFormats);
				ATPDocCreuset.m_Offre.m_TBLIdFormats.Reset();
				ATPDocCreuset.m_Offre.m_TBLIdFormats.AddTail() = FORMAT_VIRTUEL;
				
				// ! on ne peut pas creer le m_mapSubstFormat car un support peut avoir des insertions dans plusieurs formats

				//On applique le format virtuel dans chaque insetion
				for(ATPDocCreuset.m_TblHypotheses.MoveFirst();ATPDocCreuset.m_TblHypotheses.IsValid();ATPDocCreuset.m_TblHypotheses.MoveNext())
				{
					CPlanInsertions plan;
					for(ATPDocCreuset.m_TblHypotheses.GetItem().MoveFirst_SDF();
						ATPDocCreuset.m_TblHypotheses.GetItem().IsValid_SDF();
						ATPDocCreuset.m_TblHypotheses.GetItem().MoveNext_SDF())
					{
						CInsertion * pIns = ATPDocCreuset.m_TblHypotheses.GetItem().GetItem_SDF();
						pIns->m_IdFormat = FORMAT_VIRTUEL;
						plan.AddInsertion(*pIns,ATPDocCreuset.m_KEYPeriodeAction.GetDateD(),ATPDocCreuset.m_KEYPeriodeAction.GetDateF());
					}
					ATPDocCreuset.m_TblHypotheses.GetItem().Swap(plan);
				}					

				//On a modifié l'étude
				ATPDocCreuset.SetModifiedFlag();
				::AfxMessageBox("Étude modifiée", MB_ICONINFORMATION | MB_OK);
				//::AfxMessageBox("Cette étude ne peut pas être ouverte dans Quick Plan\nIl existe plusieurs formats.", MB_ICONEXCLAMATION | MB_OK);
				//return FALSE;
			}
		}
		else//ouverture dans le creuset
		{
			if(ATPDocCreuset.m_KEYEtude.GetDateF() == DateF_QP)
			{
				if(::AfxMessageBox("\tÉtude Quick Plan\nVoulez-vous convertir cette étude Quick Plan en étude du Creuset ?\nAttention: vous ne risquez de ne plus pouvoir l'ouvrir dans Quick Plan.", MB_ICONQUESTION | MB_YESNO) != IDYES)
					return FALSE;
				if(ATPDocCreuset.m_listFormatsQP.GetCount()==0)//on a lu une version 4 ou inférieure de l'etude
				{
					::AfxMessageBox("Conversion impossible.\nEnregistrez à nouveau l'étude dans Quick Plan.", MB_ICONASTERISK | MB_OK);
					return FALSE;
				}


				//recherche de la date extreme
				JDate dateMax(ATPDocCreuset.m_KEYEtude.GetDateD()+1);
				for(ATPDocCreuset.m_TblHypotheses.MoveFirst();ATPDocCreuset.m_TblHypotheses.IsValid();ATPDocCreuset.m_TblHypotheses.MoveNext())
				{
					for(ATPDocCreuset.m_TblHypotheses.GetItem().MoveFirst_SDF();
						ATPDocCreuset.m_TblHypotheses.GetItem().IsValid_SDF();
						ATPDocCreuset.m_TblHypotheses.GetItem().MoveNext_SDF())
					{
						JDate dateFIns = ATPDocCreuset.m_TblHypotheses.GetItem().GetItem_SDF()->m_DateD+ATPDocCreuset.m_TblHypotheses.GetItem().GetItem_SDF()->m_Duree+1;
						if(dateFIns>dateMax)
							dateMax = dateFIns;
					}
				}
				//on applique la nouvelle date
				ATPDocCreuset.m_KEYEtude.SetDateF(dateMax);
				ATPDocCreuset.m_KEYPeriodeAction.SetPeriode(ATPDocCreuset.m_KEYEtude.GetDateD(),dateMax);
				ATPDocCreuset.m_KEYPeriodeActionHyp.SetPeriode(ATPDocCreuset.m_KEYEtude.GetDateD(),dateMax);
				ATPDocCreuset.m_KEYPeriodeActionTablier.SetPeriode(ATPDocCreuset.m_KEYEtude.GetDateD(),dateMax);

				//Traitement des formats
				ATPDocCreuset.m_Offre.m_TBLIdFormats.Swap(ATPDocCreuset.m_listFormatsQP);
				for(ATPDocCreuset.m_TblHypotheses.MoveFirst();ATPDocCreuset.m_TblHypotheses.IsValid();ATPDocCreuset.m_TblHypotheses.MoveNext())
				{
					CPlanInsertions plan;
					for(ATPDocCreuset.m_TblHypotheses.GetItem().MoveFirst_SDF();
						ATPDocCreuset.m_TblHypotheses.GetItem().IsValid_SDF();
						ATPDocCreuset.m_TblHypotheses.GetItem().MoveNext_SDF())
					{
						//on applique le bon format à chaque insertion dans un nouveau plan
						CInsertion * pIns = ATPDocCreuset.m_TblHypotheses.GetItem().GetItem_SDF();
						JInt32 idFormat;
						ATPDocCreuset.SubstitutFormat(pIns->m_IdSupport, idFormat);
						pIns->m_IdFormat = idFormat;
						plan.AddInsertion(*pIns,ATPDocCreuset.m_KEYPeriodeAction.GetDateD(),ATPDocCreuset.m_KEYPeriodeAction.GetDateF());
					}
					//on applique le nouveau plan
					ATPDocCreuset.m_TblHypotheses.GetItem().Swap(plan);

				}

				//On a modifié le document
				ATPDocCreuset.SetModifiedFlag();
				::AfxMessageBox("Étude modifiée", MB_ICONINFORMATION | MB_OK);
			}
		}

		// on prépare le contenu du document
		if (ATPDocCreuset.PrepareContents())
		{
			// on met à jour les vues du document
			try
			{
				ATPDocCreuset.UpdateAllViews(UPDATE_INITIAL);
				// on permute les documents
				m_pATPDocCreuset->Swap(ATPDocCreuset);
				Ret = TRUE;
			}
			catch(...)
			{
				::AfxMessageBox(IDS_CR_ETUDENONVALID ,MB_ICONERROR | MB_OK);
				//Etude vide
				JATPDocCreuset ATPDocCreuset;
				ATPDocCreuset.PrepareContents();
				m_pATPDocCreuset->Swap(ATPDocCreuset);
				// on met à jour les vues du document
				m_pATPDocCreuset->UpdateAllViews(UPDATE_INITIAL);

				Ret = FALSE;
			}

		}
		else
		{
			// on affiche le message
			::AfxMessageBox(IDS_CR_ETUDENONINIT, MB_ICONERROR | MB_OK);
		}
	}
	else
	{
		// on affiche le message
		::AfxMessageBox(IDS_CR_ETUDENONVALID ,MB_ICONERROR | MB_OK);
	}

	return (Ret);
}

BOOL CATPBaseFrame::OnSaveDocument(JATPDocCreuset & ATPDocCreuset, JBool bRename)
{
	// on initialise l'indicateur
	BOOL Ret = FALSE;

	CString path;
	if (bRename || ATPDocCreuset.m_KEYEtude.GetPath() == "")
	{
		CString ext, szFilters;
		if(ATPDocCreuset.m_AFFCreuset.GetAffQuickPlan())//ouverture dans quick plan
		{
			ext.LoadString(IDS_CR_EXT_QP);
			szFilters.LoadString(IDS_CR_FILTER_QP);
		}
		else
		{
			ext.LoadString(IDS_CR_EXT);
			szFilters.LoadString(IDS_CR_FILTER);
		}

		DWORD flags = OFN_NOCHANGEDIR | OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_NOREADONLYRETURN |OFN_OVERWRITEPROMPT ;
		CFileDialog fileDlg(FALSE, ext, 0, flags, szFilters, this);

		if( fileDlg.DoModal() == IDOK )
		{
			path = fileDlg.GetPathName();
			ATPDocCreuset.m_KEYEtude.SetPath(path);
		}
		else
			return FALSE;
	}
	else
		path = ATPDocCreuset.m_KEYEtude.GetPath();


	JMFCStream* stream = NULL;
	try
	{
		CFile file(path, CFile::modeCreate |CFile::modeWrite);
		CArchive ar(&file,CArchive::store);
		stream = JMFCStream::Create(ar);
		
		ATPDocCreuset.Send(*stream);
		
		stream->Flush();
	}
	catch(...)
	{
	}
	if (stream)
		delete stream;
	SetCurrentDirectory(AfxGetApp()->GetProfileString("Data", "SourcesPath", 0));


	ATPDocCreuset.SetModifiedFlag(FALSE);
	//m_pATPDocCreuset->UpdateAllViews(UPDATE_ETUDE); 

	return (Ret);
}

//////////////////////////////////////////
// la fonction pour mettre à jour les vues
VOID CATPBaseFrame::UpdateAllViews(const JFCUpdate & Message)
{
	m_dlgFiltres.Update();
	m_dlgChoixMoteur.Update();
	m_dlgCourbeReponse.Update();

	if(Message == UPDATE_COLOR)
	{
		m_dlgFiltres.SetColors(CATPColors::COLORCREUSET);
		m_dlgChoixMoteur.SetColors(CATPColors::COLORCREUSET);
		m_dlgCourbeReponse.SetColors();
	}
	
	if(Message == UPDATE_AFF_CONTRIB)
	{
		m_dlgContribution.Update();
		return;
	}
	m_dlgContribution.Update();
}

////////////////////////////////
// CATPBaseFrame message handlers

int CATPBaseFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	// on appelle le gestionnaire de base
	if (this->JFCFrame::OnCreate(lpCreateStruct) == -1) return (-1);

	// Create the dialogs
	if (m_dlgFiltres.Create(CDialogFiltres::IDD, this) == FALSE)
		return (-1);
	m_dlgFiltres.SetDocument(m_pATPDocCreuset);
	m_dlgFiltres.SetColors(CATPColors::COLORCREUSET);

	if (m_dlgCourbeReponse.Create(CDlgCourbeReponse::IDD, this) == FALSE)
		return (-1);
	m_dlgCourbeReponse.SetDocument(m_pATPDocCreuset);

	if (m_dlgChoixMoteur.Create(CDlgChoixMoteur::IDD, this) == FALSE)
		return (-1);
	m_dlgChoixMoteur.SetDocument(m_pATPDocCreuset);
	m_dlgChoixMoteur.SetColors(CATPColors::COLORCREUSET);

	
	if (m_dlgContribution.Create(CDialogContrib::IDD, this) == FALSE) return (-1);
		m_dlgContribution.SetDocument(m_pATPDocCreuset);

	// on quitte	
	return (0);
}

void CATPBaseFrame::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
}

void CATPBaseFrame::OnSize(UINT nType, int cx, int cy) 
{
}

void CATPBaseFrame::OnUpdateEtudeNouvelle(CCmdUI* pCmdUI) 
{
	// on modifie l'état du menu
	pCmdUI->Enable(TRUE);
}

void CATPBaseFrame::OnUpdateEtudeOuvrir(CCmdUI* pCmdUI) 
{
	// on modifie l'état du menu
	pCmdUI->Enable(TRUE);
}

void CATPBaseFrame::OnUpdateEtudeEnregistrer(CCmdUI* pCmdUI) 
{
	// on modifie l'état du menu
	pCmdUI->Enable((m_pATPDocCreuset->IsValid()) ? TRUE : FALSE);
}

void CATPBaseFrame::OnUpdateEtudeEnregistrerSous(CCmdUI* pCmdUI) 
{
	// on modifie l'état du menu
	pCmdUI->Enable((m_pATPDocCreuset->IsValid()) ? TRUE : FALSE);
}

void CATPBaseFrame::OnUpdateEtudeFermer(CCmdUI* pCmdUI) 
{
	// on modifie l'état du menu
	pCmdUI->Enable((m_pATPDocCreuset->IsValid()) ? TRUE : FALSE);
}

void CATPBaseFrame::OnUpdatePerimetrePeriode(CCmdUI* pCmdUI) 
{
	// on modifie l'état du menu
	pCmdUI->Enable((m_pATPDocCreuset->IsValid()) ? TRUE : FALSE);
}

void CATPBaseFrame::OnOffreSources()
{
	bool bDateChanged, bOffreModified;

	// on recopie les dates de l'etude
	JDate DebEtude = m_pATPDocCreuset->m_KEYEtude.GetDateD();
	JDate FinEtude = m_pATPDocCreuset->m_KEYEtude.GetDateF();

	m_pATPDocCreuset->m_ListSupportsChange = false;
	JMap<JUnt32, JUnt32> ExMapSupports;
	ExMapSupports.Reset();
	for (m_pATPDocCreuset->m_Offre.m_TBLIdSupports.MoveFirst(); m_pATPDocCreuset->m_Offre.m_TBLIdSupports.IsValid(); m_pATPDocCreuset->m_Offre.m_TBLIdSupports.MoveNext())
	{
		JUnt32 IdSupport = m_pATPDocCreuset->m_Offre.m_TBLIdSupports.GetItem();
		ExMapSupports.Add(IdSupport) = IdSupport;
	}

	// on ouvre le dialogue de modification de l'offre
	if (OpenOffreDialog("Modification de l'offre de l'étude courante", m_CATPColorsApp, 
		m_pATPDocCreuset->m_Offre, COffre2MainDialog::eodoTerrainsSupports,
		DebEtude, FinEtude, bDateChanged, bOffreModified, true, this))
	{
		m_pATPDocCreuset->SetModifiedFlag();

		// si la nouvelle offre a bien été modifiée et validée
		if (bDateChanged)
		{
			// Récupération des nouvelles bornes date étude
			m_pATPDocCreuset->m_KEYEtude.SetDateD(DebEtude);
			m_pATPDocCreuset->m_KEYEtude.SetDateF(FinEtude);

			m_pATPDocCreuset->m_KEYPeriodeAction.SetPeriode(DebEtude, FinEtude);
			m_pATPDocCreuset->m_KEYPeriodeActionHyp.SetPeriode(DebEtude, FinEtude);

			
			// on met à jour les vues du document
			m_pATPDocCreuset->UpdateAllViews(UPDATE_ETUDE);
		}

		// Reconstruit la table des items interdits
		m_pATPDocCreuset->m_Offre.ConstructionItemsInterdits();

		if (ExMapSupports.GetCount() != m_pATPDocCreuset->m_Offre.m_TBLIdSupports.GetCount())
		{
			m_pATPDocCreuset->m_ListSupportsChange = true;
		}
		else
		{
			for (m_pATPDocCreuset->m_Offre.m_TBLIdSupports.MoveFirst(); m_pATPDocCreuset->m_Offre.m_TBLIdSupports.IsValid(); m_pATPDocCreuset->m_Offre.m_TBLIdSupports.MoveNext())
			{
				JUnt32 IdSupport = m_pATPDocCreuset->m_Offre.m_TBLIdSupports.GetItem();
				ExMapSupports.MoveTo(IdSupport);
				if (!ExMapSupports.IsValid())
				{
					m_pATPDocCreuset->m_ListSupportsChange = true;	
				}
			}
		}

		// Mise à jour globale
		m_pATPDocCreuset->UpdateAllViews(UPDATE_OFFRE);

		// On remonte les nouveaux supports dans le plan
		m_pATPDocCreuset->AjouteSupportsPlateau();
	}
}

///////////////////////////////////////////////
// Appel de la boite Offres pour choix Supports
void CATPBaseFrame::OnOffreSupports()
{
	// La boite offre
	CDialogChoixOffre Dlg;

	// Setup de la boite offre supports
	Dlg.SetStartupMode(CDialogChoixOffre::SUPPORT, CATPColors::COLORCREUSET);

	// On passe l'offre et les terrains
	Dlg.SetOffre(m_pATPDocCreuset->m_Offre);

	// Affichage de la fenêtre offre
	if(Dlg.DoModal()== IDOK)
	{
		// Si éléments modifiés, on récupère les nouvelles infos
		//if(Dlg.IsModified())
		{
			// Recup l'offre
			Dlg.GetOffre(m_pATPDocCreuset->m_Offre);

			// Reconstruit la table des items interdits
			m_pATPDocCreuset->m_Offre.ConstructionItemsInterdits();

			m_pATPDocCreuset->SetModifiedFlag();
			// Mise à jour globale
			m_pATPDocCreuset->UpdateAllViews(UPDATE_OFFRE);
		}
	}

}

///////////////////////////////////////////////
// Appel de la boite Offres pour choix Cibles
void CATPBaseFrame::OnOffreCibles()
{
	bool bDateChanged, bOffreModified;

	// on recopie les dates de l'etude
	JDate DebEtude = m_pATPDocCreuset->m_KEYEtude.GetDateD();
	JDate FinEtude = m_pATPDocCreuset->m_KEYEtude.GetDateF();


	m_pATPDocCreuset->m_ListSupportsChange = false;
	JMap<JUnt32, JUnt32> ExMapSupports;
	ExMapSupports.Reset();
	for (m_pATPDocCreuset->m_Offre.m_TBLIdSupports.MoveFirst(); m_pATPDocCreuset->m_Offre.m_TBLIdSupports.IsValid(); m_pATPDocCreuset->m_Offre.m_TBLIdSupports.MoveNext())
	{
		JUnt32 IdSupport = m_pATPDocCreuset->m_Offre.m_TBLIdSupports.GetItem();
		ExMapSupports.Add(IdSupport) = IdSupport;
	}

	// on ouvre le dialogue de modification de l'offre
	if (OpenOffreDialog("Modification de l'offre de l'étude courante", m_CATPColorsApp, 
		m_pATPDocCreuset->m_Offre, COffre2MainDialog::eodoCibles,
		DebEtude, FinEtude, bDateChanged, bOffreModified, true, this))
	{
		m_pATPDocCreuset->SetModifiedFlag();

		// si la nouvelle offre a bien été modifiée et validée
		if (bDateChanged)
		{
			// Récupération des nouvelles bornes date étude
			m_pATPDocCreuset->m_KEYEtude.SetDateD(DebEtude);
			m_pATPDocCreuset->m_KEYEtude.SetDateF(FinEtude);

			m_pATPDocCreuset->m_KEYPeriodeAction.SetPeriode(DebEtude, FinEtude);
			m_pATPDocCreuset->m_KEYPeriodeActionHyp.SetPeriode(DebEtude, FinEtude);

			// on met à jour les vues du document
			m_pATPDocCreuset->UpdateAllViews(UPDATE_ETUDE);
		}

		// Reconstruit la table des items interdits
		m_pATPDocCreuset->m_Offre.ConstructionItemsInterdits();

		if (ExMapSupports.GetCount() != m_pATPDocCreuset->m_Offre.m_TBLIdSupports.GetCount())
		{
			m_pATPDocCreuset->m_ListSupportsChange = true;
		}
		else
		{
			for (m_pATPDocCreuset->m_Offre.m_TBLIdSupports.MoveFirst(); m_pATPDocCreuset->m_Offre.m_TBLIdSupports.IsValid(); m_pATPDocCreuset->m_Offre.m_TBLIdSupports.MoveNext())
			{
				JUnt32 IdSupport = m_pATPDocCreuset->m_Offre.m_TBLIdSupports.GetItem();
				ExMapSupports.MoveTo(IdSupport);
				if (!ExMapSupports.IsValid())
				{
					m_pATPDocCreuset->m_ListSupportsChange = true;	
				}
			}
		}

		// Mise à jour globale
		m_pATPDocCreuset->UpdateAllViews(UPDATE_OFFRE);
	}
}

//////////////////////////////////////////////////
// Appel de la boite Offres pour choix des Formats
void CATPBaseFrame::OnOffreFormat()
{
	bool bDateChanged, bOffreModified;

	// on recopie les dates de l'etude
	JDate DebEtude = m_pATPDocCreuset->m_KEYEtude.GetDateD();
	JDate FinEtude = m_pATPDocCreuset->m_KEYEtude.GetDateF();

	m_pATPDocCreuset->m_ListSupportsChange = false;
	JMap<JUnt32, JUnt32> ExMapSupports;
	ExMapSupports.Reset();
	for (m_pATPDocCreuset->m_Offre.m_TBLIdSupports.MoveFirst(); m_pATPDocCreuset->m_Offre.m_TBLIdSupports.IsValid(); m_pATPDocCreuset->m_Offre.m_TBLIdSupports.MoveNext())
	{
		JUnt32 IdSupport = m_pATPDocCreuset->m_Offre.m_TBLIdSupports.GetItem();
		ExMapSupports.Add(IdSupport) = IdSupport;
	}

	// on ouvre le dialogue de modification de l'offre
	if (OpenOffreDialog("Modification de l'offre de l'étude courante", m_CATPColorsApp, 
		m_pATPDocCreuset->m_Offre, COffre2MainDialog::eodoFormats,
		DebEtude, FinEtude, bDateChanged, bOffreModified, true, this))
	{
		m_pATPDocCreuset->SetModifiedFlag();

		// si la nouvelle offre a bien été modifiée et validée
		if (bDateChanged)
		{
			// Récupération des nouvelles bornes date étude
			m_pATPDocCreuset->m_KEYEtude.SetDateD(DebEtude);
			m_pATPDocCreuset->m_KEYEtude.SetDateF(FinEtude);

			m_pATPDocCreuset->m_KEYPeriodeAction.SetPeriode(DebEtude, FinEtude);
			m_pATPDocCreuset->m_KEYPeriodeActionHyp.SetPeriode(DebEtude, FinEtude);

			// on met à jour les vues du document
			m_pATPDocCreuset->UpdateAllViews(UPDATE_ETUDE);
		}

		// Reconstruit la table des items interdits
		m_pATPDocCreuset->m_Offre.ConstructionItemsInterdits();

		if (ExMapSupports.GetCount() != m_pATPDocCreuset->m_Offre.m_TBLIdSupports.GetCount())
		{
			m_pATPDocCreuset->m_ListSupportsChange = true;
		}
		else
		{
			for (m_pATPDocCreuset->m_Offre.m_TBLIdSupports.MoveFirst(); m_pATPDocCreuset->m_Offre.m_TBLIdSupports.IsValid(); m_pATPDocCreuset->m_Offre.m_TBLIdSupports.MoveNext())
			{
				JUnt32 IdSupport = m_pATPDocCreuset->m_Offre.m_TBLIdSupports.GetItem();
				ExMapSupports.MoveTo(IdSupport);
				if (!ExMapSupports.IsValid())
				{
					m_pATPDocCreuset->m_ListSupportsChange = true;	
				}
			}
		}

		// Mise à jour globale
		m_pATPDocCreuset->UpdateAllViews(UPDATE_OFFRE);
	}
}

void CATPBaseFrame::OnUpdateImporterPlateau(CCmdUI *pCmdUI)
{
	// on modifie l'état du menu
	pCmdUI->Enable(TRUE);	
}

////////////////////////////////////////////////////////////////////////////////////////
// Vérifie que c'est bien un nouveau support à ajouter dans la table global des supports
// C'EST UNE FONCTION DE l'ANCIENNE OFFRE, RECOPIEE PAR MANQUE DE TEMPS
JBool NewSupportTblSupp(JList<JUnt32> &TblSupports, JUnt32 IdSupp)
{
	// Boucle sur tous les ids supports du tableau en cours
	for (TblSupports.MoveFirst(); TblSupports.IsValid(); TblSupports.MoveNext())  
	{
		// Récupère l'ident support courant
		JUnt32 IdSuppCur = TblSupports.GetItem();
		if (IdSupp == IdSuppCur)
			return false;
	}

	// Pas trouvé, c'est un nouveau support
	return true;
}

#include "JATPDocChoixSupport.h"

void CATPBaseFrame::OnImporterPlateau()
{
	// on crée un nouveau document
	JATPDocCreuset ATPDocCreuset;
	ATPDocCreuset.m_AFFCreuset = m_pATPDocCreuset->m_AFFCreuset;

	// on teste la validité du document courant
	if (m_pATPDocCreuset->IsValid())
	{
		// on teste l'indicateur de modification du document courant
		if (FALSE != m_pATPDocCreuset->IsModified())
		{
			// on affiche le message
			INT Ret = ::AfxMessageBox(IDS_CR_CONFIRMSAV, MB_ICONQUESTION | MB_YESNOCANCEL);

			// on teste la réponse de l'utilisateur
			if (Ret == IDYES)
			{
				// on sauvegarde le document courant
				this->OnSaveDocument(*m_pATPDocCreuset, false);
			}
			if(Ret == IDCANCEL)
				return;
		}
	}

	JMFCStream* stream = NULL;

	CString szFilters;
	szFilters.LoadString(IDS_FD_FILTER);
	DWORD flags = OFN_NOCHANGEDIR | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY ;
	CFileDialog fileDlg(TRUE, "ofr", 0, flags, szFilters, this);

	// Affichage de la liste des plateaux existants
	JATPDocChoixSupport ATPDocChoixSupport;
	CString path;
	if( fileDlg.DoModal() == IDOK )
		path = fileDlg.GetPathName();
	else
		return;

	// On se positionne sur le répertoire User
	SetCurrentDirectory(AfxGetApp()->GetProfileString("Data", "SourcesPath", 0));

	try
	{
		// Récupération n° de version
		JUnt32 NoVersion = 0;
		CFile filetmp(path, CFile::modeRead);

		// Ouvre le fichier en mode sérialize
		CArchive ar(&filetmp, CArchive::load);
		stream = JMFCStream::Create(ar);

		// Récup n° de version
		ATPDocChoixSupport.RecvNoVersion(*stream, NoVersion);

		// Ferme l'archive
		ar.Close();

		// Ferme le fichier
		filetmp.Close(); 

		// Selon le N° de version on ouvre en CFile ou StdioFile (pour récupérer les anciens fichiers)
		JMFCStream* stream = NULL;

		if (NoVersion <= 7)
		{
			CStdioFile file(path, CFile::modeRead);
			CArchive ar(&file, CArchive::load);
			stream = JMFCStream::Create(ar);
			ATPDocChoixSupport.Recv(*stream);
		}
		else
		{
			// Attention sinon problème sur certains codes (type idterrain > = 282)
			CFile file(path, CFile::modeRead);

			// Le reste reste identique
			CArchive ar(&file, CArchive::load);
			stream = JMFCStream::Create(ar);
			ATPDocChoixSupport.Recv(*stream);
		}

	}
	catch(...)
	{
	}


	if (stream)
		delete stream;

	if (ATPDocChoixSupport.IsValid())
	{
		ATPDocCreuset.m_Offre = ATPDocChoixSupport.m_Offre;

		ATPDocCreuset.m_Offre.m_TBLIdSupports.Reset();
		ATPDocCreuset.m_Offre.m_MapSelFamilles.Reset();

		for(ATPDocChoixSupport.m_MapIdSupportsSel.MoveFirst();
			ATPDocChoixSupport.m_MapIdSupportsSel.IsValid();
			ATPDocChoixSupport.m_MapIdSupportsSel.MoveNext())
		{
			if(ATPDocChoixSupport.m_MapIdSupportsSel.GetItem() == true)
			{
				// Attention aux doublons
				JUnt32 IdSupp = ATPDocChoixSupport.m_MapIdSupportsSel.GetKey();

				// Attention si c'est une famille
				if (IdSupp < 100000)
				{
					if (NewSupportTblSupp(ATPDocCreuset.m_Offre.m_TBLIdSupports, IdSupp))
						ATPDocCreuset.m_Offre.m_TBLIdSupports.AddTail() = ATPDocChoixSupport.m_MapIdSupportsSel.GetKey();
				}
				else
				{
					ATPDocChoixSupport.m_Offre.m_MapSelFamilles.MoveTo(IdSupp);   
					// if (!ATPDocChoixSupport.m_Offre.m_MapSelFamilles.IsValid())
					if (ATPDocChoixSupport.m_Offre.m_MapSelFamilles.IsValid())
					{
						// Récupère les élmts regroupement
						CRegroupements * pRegroupements = CRegroupements::GetInstance();
						IElem* pelem = pRegroupements->GetIElemByID(IdSupp);

						if (pelem)
						{
							// Nom famille
							CInfoFamille InfoFamille;
							InfoFamille.m_NomFamille = pelem->GetLabel().AsJCharPtr(); 

							// Récupère les composants supports
							InfoFamille.m_LstIdSupport.Reset();
							JList <IElem*> ListSup;
							ILot * pLot = pelem->AsLot();
							if (pLot)
							{
								pLot->GetFils(ListSup);

								// On ajoute les supports valides uniquement
								for(ListSup.MoveFirst(); ListSup.IsValid(); ListSup.MoveNext())
								{
									// Ident composant famille
									JUnt32 IdSuppFam = ListSup.GetItem()->GetID();

									// De préférence, on ajoute tous les composants, même si non valide sur Source, Terrain
									InfoFamille.m_LstIdSupport.AddTail()= IdSuppFam;
								}
							}

							// Ajoute la famille
							ATPDocCreuset.m_Offre.m_MapSelFamilles.Add(IdSupp) = InfoFamille;
						}
					}
				}
			}
		}

		//m_pdlgSource->Init();
		//m_pdlgSupport->Init();
		//m_pdlgCible->Init();
		//m_preselcible = false;
		//EnableOK();

		// on définit les dates de l'etude
		JDate dateDeb, dateFin;
		dateDeb = ATPDocCreuset.m_KEYEtude.GetDateD();
		dateFin = ATPDocCreuset.m_KEYEtude.GetDateF();

		COffre2MainDialog Dlg(CATPColors::COLORCREUSET, this, true);
		Dlg.SetTitle("Ouverture d'une Offre provenant d'un plateau");

		// Setup de la boite offre source
		Dlg.SetStartupMode(COffre2MainDialog::eodoTerrainsSupports);

		// On passe l'offre et les terrains
		Dlg.SetOffre(ATPDocCreuset.m_Offre);

		// on passe les bornes de la periode
		Dlg.SetPeriode(dateDeb, dateFin);

		// on affiche la boîte de dialogue
		if (IDOK == Dlg.DoModal())
		{

			//if(Dlg.IsModified()) TODO : aurelien : gestion modification ou pas de l'offre

			// on récupère la nouvelle offre sélectionnée par l'utilisateur
			Dlg.GetOffre(ATPDocCreuset.m_Offre);

			// on modifie la période de l'étude
			ATPDocCreuset.m_KEYEtude.SetDateD(Dlg.GetPeriodeDebut());
			ATPDocCreuset.m_KEYEtude.SetDateF(Dlg.GetPeriodeFin());

			// on vérifie la validité du nouveau document
			if (ATPDocCreuset.IsValid())
			{
				// on prépare le contenu du document
				if (FALSE != ATPDocCreuset.PrepareContents())
				{
					// on permute les documents
					m_pATPDocCreuset->Swap(ATPDocCreuset);

					// on positionne l'indicateur de modification
					m_pATPDocCreuset->SetModifiedFlag(TRUE);

					// on met à jour les vues du document
					m_pATPDocCreuset->UpdateAllViews(UPDATE_INITIAL);

					// Via module offre, on charge les paramétres courants "Source"
					JString NomAppli = "FrameHypo";
					m_pATPDocCreuset->m_Offre.SaveParamSource(NomAppli); 

					return;
				}
			}
			::AfxMessageBox(IDS_CR_ERR_INITETU, MB_ICONERROR | MB_OK);
		}
	}
}

void CATPBaseFrame::OnEtudeNouvelle() 
{
	// on crée un nouveau document
	JATPDocCreuset ATPDocCreuset;
	ATPDocCreuset.m_AFFCreuset = m_pATPDocCreuset->m_AFFCreuset;

	// on teste la validité du document courant
	if (m_pATPDocCreuset->IsValid())
	{
		// on teste l'indicateur de modification du document courant
		if (FALSE != m_pATPDocCreuset->IsModified())
		{
			// on affiche le message
			INT Ret = ::AfxMessageBox(IDS_CR_CONFIRMSAV, MB_ICONQUESTION | MB_YESNOCANCEL);

			// on teste la réponse de l'utilisateur
			if (Ret == IDYES)
			{
				// on sauvegarde le document courant
				this->OnSaveDocument(*m_pATPDocCreuset, false);
			}
			if(Ret == IDCANCEL)
				return;
		}
	}

	// Via module offre, on charge les paramétres courants "Source"
	JString NomAppli = "FrameHypo";
	ATPDocCreuset.m_Offre.LoadParamSource(NomAppli); 

	// on définit les dates de l'etude
	JDate dateDeb, dateFin;
	dateDeb = ATPDocCreuset.m_KEYEtude.GetDateD();
	dateFin = ATPDocCreuset.m_KEYEtude.GetDateF();

	COffre2MainDialog Dlg(CATPColors::COLORCREUSET, this, true);
	Dlg.SetTitle("Création de l'offre pour une nouvelle étude");

	// Setup de la boite offre source
	Dlg.SetStartupMode(COffre2MainDialog::eodoTerrainsSupports);

	// On passe l'offre et les terrains
	Dlg.SetOffre(ATPDocCreuset.m_Offre);

	// on passe les bornes de la periode
	Dlg.SetPeriode(dateDeb, dateFin);

	// on affiche la boîte de dialogue
	if (IDOK == Dlg.DoModal())
	{

		//if(Dlg.IsModified()) TODO : aurelien : gestion modification ou pas de l'offre

		// on récupère la nouvelle offre sélectionnée par l'utilisateur
		Dlg.GetOffre(ATPDocCreuset.m_Offre);

		// on modifie la période de l'étude
		ATPDocCreuset.m_KEYEtude.SetDateD(Dlg.GetPeriodeDebut());
		ATPDocCreuset.m_KEYEtude.SetDateF(Dlg.GetPeriodeFin());

		// on vérifie la validité du nouveau document
		if (ATPDocCreuset.IsValid())
		{
			// on prépare le contenu du document
			if (FALSE != ATPDocCreuset.PrepareContents())
			{
				// on permute les documents
				m_pATPDocCreuset->Swap(ATPDocCreuset);

				// on positionne l'indicateur de modification
				m_pATPDocCreuset->SetModifiedFlag(TRUE);

				// on met à jour les vues du document
				m_pATPDocCreuset->UpdateAllViews(UPDATE_INITIAL);

				// On remonte les nouveaux supports dans le plan
				m_pATPDocCreuset->AjouteSupportsPlateau();

				// Via module offre, on charge les paramétres courants "Source"
				JString NomAppli = "FrameHypo";
				m_pATPDocCreuset->m_Offre.SaveParamSource(NomAppli); 

				return;
			}
		}
		::AfxMessageBox(IDS_CR_ERR_INITETU, MB_ICONERROR | MB_OK);
	}
}



void CATPBaseFrame::OnEtudeOuvrir() 
{
	// on teste la validité du document courant
	if (m_pATPDocCreuset->IsValid())
	{
		// on teste l'indicateur de modification du document courant
		if (FALSE != m_pATPDocCreuset->IsModified())
		{
			// on affiche le message
			INT Ret = ::AfxMessageBox(IDS_CR_CONFIRMSAV, MB_ICONQUESTION | MB_YESNOCANCEL);

			// on teste la réponse de l'utilisateur
			if (Ret == IDYES)
			{
				// on sauvegarde le document courant
				this->OnSaveDocument(*m_pATPDocCreuset, false);
			}
			if(Ret == IDCANCEL)
				return;
		}
	}

	OnLoadEtude(*m_pATPDocCreuset, "", true);
}

void CATPBaseFrame::OnEtudeEnregistrer() 
{
	// on teste la validité du document courant
	if (m_pATPDocCreuset->IsValid())
	{
		// on sauvegarde le document courant
		if (FALSE != this->OnSaveDocument(*m_pATPDocCreuset, false))
		{
			// on annule l'indicateur de modification
			m_pATPDocCreuset->SetModifiedFlag(FALSE);
		}
	}
}

void CATPBaseFrame::OnEtudeEnregistrerSous() 
{
	// on teste la validité du document courant
	if (m_pATPDocCreuset->IsValid())
	{
		// on sauvegarde le document courant
		if (FALSE != this->OnSaveDocument(*m_pATPDocCreuset, true))
		{
			// on annule l'indicateur de modification
			m_pATPDocCreuset->SetModifiedFlag(FALSE);
		}
	}
}

void CATPBaseFrame::OnEtudeFermer() 
{
	// on teste la validité du document courant
	if (m_pATPDocCreuset->IsValid())
	{
		// on crée un nouveau document
		JATPDocCreuset ATPDocCreuset;
		ATPDocCreuset.PrepareContents();

		// on teste l'indicateur de modification du document courant
		if (FALSE != m_pATPDocCreuset->IsModified())
		{
			// on affiche le message
			INT Ret = AfxMessageBox(IDS_CR_CONFIRMSAV, MB_ICONQUESTION | MB_YESNOCANCEL);

			// on teste la réponse de l'utilisateur
			if (Ret == IDYES)
			{
				// on sauvegarde le document courant
				if (FALSE == this->OnSaveDocument(*m_pATPDocCreuset, false)) return;

				// on annule l'indicateur de modification
				m_pATPDocCreuset->SetModifiedFlag(FALSE);
			}
			if (Ret == IDCANCEL)
			{
				// on quitte
				return;
			}
		}

		// on permute les documents
		m_pATPDocCreuset->Swap(ATPDocCreuset);

		// on annule l'indicateur de modification
		m_pATPDocCreuset->SetModifiedFlag(FALSE);

		// on met à jour les vues du document
		m_pATPDocCreuset->UpdateAllViews(UPDATE_INITIAL);
	}
}

void CATPBaseFrame::OnClose() 
{
	// on appelle le gestionnaire de base	
	if(CanClose())
        this->JFCFrame::OnClose();
}

// Modification de la période d'action
void CATPBaseFrame::OnPerimetrePeriode() 
{
	// on teste la validité du document courant
	if (m_pATPDocCreuset->IsValid())
	{
		// on crée la boîte de dialogue
		//CATPPeriodeAction Dlg(IDS_CR_PAWNDVAGUE, IDS_CR_PATTLVAGUE, this);
		CATPPeriodeAction Dlg(IDS_CR_PAWNDTRAVAIL, IDS_CR_PATTLVAGUE, this);
		Dlg.SetHypo(false);
		Dlg.SetDocument(m_pATPDocCreuset);

		Dlg.m_DateD_Etude = m_pATPDocCreuset->m_KEYEtude.GetDateD();
		Dlg.m_DateF_Etude = m_pATPDocCreuset->m_KEYEtude.GetDateF();

		if(m_pATPDocCreuset->m_IdxHypotheseCourant == 0)
		{
			// cas de l'hypothèse 0 (plan étendu)
			Dlg.m_DateD_Action = m_pATPDocCreuset->m_KEYPeriodeActionHyp.GetDateD();  
			Dlg.m_DateF_Action = m_pATPDocCreuset->m_KEYPeriodeActionHyp.GetDateF();
		}
		else
		{
			// cas des autres hypothèses
			Dlg.m_DateD_Action = m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD();  
			Dlg.m_DateF_Action = m_pATPDocCreuset->m_KEYPeriodeAction.GetDateF();
		}

		// on affiche la boîte
		if (IDOK == Dlg.DoModal())
		{
			//// Récupèration des dates période action
			m_pATPDocCreuset->m_KEYPeriodeAction.SetPeriode(Dlg.m_DateD_Action,Dlg.m_DateF_Action);  

			// Si nouvelle période action hypothèse
			if (m_pATPDocCreuset->m_KEYPeriodeActionHyp.GetDateD() != Dlg.m_DateD_Action ||
				m_pATPDocCreuset->m_KEYPeriodeActionHyp.GetDateF() != Dlg.m_DateF_Action)
			{
				// Réaffecte nouvelle période
				m_pATPDocCreuset->m_KEYPeriodeActionHyp.SetPeriode(Dlg.m_DateD_Action,Dlg.m_DateF_Action);  
				m_pATPDocCreuset->m_KEYPeriodeActionTablier.SetPeriode(Dlg.m_DateD_Action,Dlg.m_DateF_Action);  

				// on enlève tous les supports du plateau période action et on ne garde que ceux ayant au moins
				// 1 insertion sur cette nouvelle vague
				m_pATPDocCreuset->MajPlateauHypotheseNewVague();
			}
			
			// on positionne l'indicateur de modification
			m_pATPDocCreuset->SetModifiedFlag(TRUE);

			// on met à jour les vues du document
			m_pATPDocCreuset->UpdateAllViews(UPDATE_KEY_PERIODE_ACTION);
		}
	}
}

void CATPBaseFrame::OnVersion() 
{
	// on affiche la boîte
	CATPVersion Dlg; 
	Dlg.DoModal();
}

JBool CATPBaseFrame::CanClose() 
{
	// on teste la validité du document courant
	if (m_pATPDocCreuset->IsValid())
	{
		// on teste l'indicateur de modification du document courant
		if (FALSE != m_pATPDocCreuset->IsModified())
		{
			// on affiche le message
			
			CString ttl, qst;
			ttl.LoadString(IDR_CR_ATPFRAME);
			qst.LoadString(IDS_CR_CONFIRMSAV);
			int Ret = MessageBox(qst, ttl, MB_ICONQUESTION|MB_YESNOCANCEL);
			//INT Ret = AfxMessageBox(IDS_CR_CONFIRMSAV, MB_ICONQUESTION | MB_YESNO);

			// on teste la réponse de l'utilisateur
			if (Ret == IDYES)
			{
				// on sauvegarde le document courant
				if (FALSE == this->OnSaveDocument(*m_pATPDocCreuset, false))
					return false;

				// on annule l'indicateur de modification
				m_pATPDocCreuset->SetModifiedFlag(FALSE);
			}
			if (Ret == IDCANCEL) // on quitte
				return false;
		}
	}
	// on crée un nouveau document
	JATPDocCreuset ATPDocCreuset;
	ATPDocCreuset.PrepareContents();

	// on permute les documents
	m_pATPDocCreuset->Swap(ATPDocCreuset);

	// on annule l'indicateur de modification
	m_pATPDocCreuset->SetModifiedFlag(FALSE);
	return true;
}

// Validation de l'hypothese actif dans le master plan
void CATPBaseFrame::OnValidHypothese()
{
	// Pas de document valide en cours
	if (!m_pATPDocCreuset->m_KEYEtude.GetDateD().IsValid()) return;

	// Test the Hypothese Actif
	if(!m_pATPDocCreuset->m_IdxHypotheseActif)
		return;

	//if( AfxMessageBox(IDS_CD_STR_VALIDHYPO, MB_YESNO|MB_ICONQUESTION) == IDNO )
	//	return;

	JDate DateD = m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD();  
	JDate DateF = m_pATPDocCreuset->m_KEYPeriodeAction.GetDateF();

	// On met à jour le plan master
	if(!m_pATPDocCreuset->ReplacePlanWithHypothese(&m_pATPDocCreuset->m_MasterPlan,
												   m_pATPDocCreuset->m_IdxHypotheseActif,
												   DateD,DateF))
		return;

	m_pATPDocCreuset->CopyMasterPlanToHypothese(0);

	// Save a version of the Plan Master (sauvegarde automatique sans nom)
	CPlanMasters Version;
	Version.m_Plan			= m_pATPDocCreuset->m_MasterPlan;

	// Stocke la version automatiquement dans tab archive plans
	m_pATPDocCreuset->m_TblPlanArchive.AddTail() = Version; 

	// Mise à jour Creuset
	m_pATPDocCreuset->UpdateAllViews(UPDATE_ETUDE);
}

// Copy master plan to the hypothese 
void CATPBaseFrame::OnMasterPlanToHypothese()
{
	// Pas de document valide en cours
	if (!m_pATPDocCreuset->m_KEYEtude.GetDateD().IsValid()) return;

	// Do nothing if Plan Master of invalid index
	if(m_pATPDocCreuset->CopyMasterPlanToHypothese(m_pATPDocCreuset->m_IdxHypotheseActif))
		// Mise à jour Creuset
		m_pATPDocCreuset->UpdateAllViews(UPDATE_CURRENT_HYPO);
}

// Chargement du version master antérieure
void CATPBaseFrame::OnVersionCharger()
{
	// Test si au moins une ancienne version à proposer
	if (m_pATPDocCreuset->m_TblPlanArchive.GetCount())
	{
		// Ouvrir boite sélection des versions précédentes
		CDlgVersionPlan dlg(true);

		// On passe le document
		dlg.m_pATPDocCreuset = m_pATPDocCreuset;

		// Ouverture
		if( dlg.DoModal()== IDOK)
		{
			// Sauvegarde du master courant si version différente
			if (!(m_pATPDocCreuset->m_MasterPlan == dlg.m_PlanRefArchive))
			{
				// On teste également si version non sauvegardé précédemment, pour éviter les doublons
				CPlanMasters Version;
				Version.m_Plan			= m_pATPDocCreuset->m_MasterPlan;
				JBool NewVersion = true;
				for (m_pATPDocCreuset->m_TblPlanArchive.MoveFirst(); 
					 m_pATPDocCreuset->m_TblPlanArchive.IsValid();
					 m_pATPDocCreuset->m_TblPlanArchive.MoveNext())
				{
					if (Version.m_Plan == m_pATPDocCreuset->m_TblPlanArchive.GetItem().m_Plan)
					{
						// Version déjà archivé
						NewVersion = false;
						break;
					}

				}
				// Au moment du changement on sauve la version si non déjà existante
				if (NewVersion)
					m_pATPDocCreuset->m_TblPlanArchive.AddTail() = Version; 
			}

			// Récupère nouveau master
			m_pATPDocCreuset->m_MasterPlan = dlg.m_PlanRefArchive;

			// Do nothing if Plan Master of invalid index
			if(m_pATPDocCreuset->CopyMasterPlanToHypothese(m_pATPDocCreuset->m_IdxHypotheseActif))
				// Mise à jour Creuset
				m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_CREUSET);
		}
		
	}
}

void CATPBaseFrame::OnUpdateValidHypothese(CCmdUI* pCmdUI)
{
	if(m_pATPDocCreuset->m_IdxHypotheseCourant==0 || !m_pATPDocCreuset->m_KEYEtude.GetDateD().IsValid())
		pCmdUI->Enable(false);
	else
		pCmdUI->Enable(true);
}

void CATPBaseFrame::OnUpdateMasterPlanToHypothese(CCmdUI* pCmdUI)
{
	pCmdUI->Enable((m_pATPDocCreuset->IsValid() && m_pATPDocCreuset->m_IdxHypotheseActif!=0 && m_pATPDocCreuset->m_TblPlanArchive.GetCount()) ? TRUE : FALSE);
}

void CATPBaseFrame::OnUpdateOptionsMoteurAbaque(CCmdUI *pCmdUI)
{
	pCmdUI->Enable((m_pATPDocCreuset->IsValid()) ? TRUE : FALSE);
	(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::ABAQUE) ? pCmdUI->SetRadio(TRUE) : pCmdUI->SetRadio(FALSE);
}
void CATPBaseFrame::OnUpdateOptionsMoteurAbaqueEtal(CCmdUI *pCmdUI)
{
	pCmdUI->Enable((m_pATPDocCreuset->IsValid()) ? TRUE : FALSE);
	if(m_pATPDocCreuset->IsValid())
	{
		CKeyTerrain keyTerrain;
		m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.MoveFirst();
		keyTerrain.Set(m_pATPDocCreuset->m_IdSrcAudienceSel, m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.GetKey());

		(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::ABAQUE_ETAL) ? pCmdUI->SetRadio(TRUE) : pCmdUI->SetRadio(FALSE);
		//
		if(! AfxGetApp()->GetProfileInt("Moteur", "VersionJFC", 0))
			pCmdUI->Enable(m_pATPDocCreuset->m_MoteurPresse.CanAbaque(keyTerrain));

	}
}
void CATPBaseFrame::OnUpdateOptionsMoteurProba(CCmdUI *pCmdUI)
{
	pCmdUI->Enable((m_pATPDocCreuset->IsValid()) ? TRUE : FALSE);
	(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA) ? pCmdUI->SetRadio(TRUE) : pCmdUI->SetRadio(FALSE);
}

void CATPBaseFrame::OnUpdateOptionsRepriseEnMain(CCmdUI *pCmdUI)
{
	pCmdUI->Enable((m_pATPDocCreuset->IsValid()) ? TRUE : FALSE);
	m_pATPDocCreuset->m_bRepriseEnMain ? pCmdUI->SetRadio(TRUE) : pCmdUI->SetRadio(FALSE);
}
void CATPBaseFrame::OnUpdateOptionsNoRepriseEnMain(CCmdUI *pCmdUI)
{
	pCmdUI->Enable((m_pATPDocCreuset->IsValid()) ? TRUE : FALSE);
	m_pATPDocCreuset->m_bRepriseEnMain ? pCmdUI->SetRadio(FALSE) : pCmdUI->SetRadio(TRUE);
}

void CATPBaseFrame::OnUpdateOptionsLNMMagazine(CCmdUI *pCmdUI)
{
	//pCmdUI->Enable((m_pATPDocCreuset->IsValid()) ? TRUE : FALSE);
	m_pATPDocCreuset->m_bLNMMag ? pCmdUI->SetCheck(TRUE) : pCmdUI->SetCheck(FALSE);
	pCmdUI->Enable(FALSE);
}

void CATPBaseFrame::OnUpdateOptionsLDPQuotidien(CCmdUI *pCmdUI)
{
	pCmdUI->Enable((m_pATPDocCreuset->IsValid()) ? TRUE : FALSE);
	m_pATPDocCreuset->m_bLDPQtd ? pCmdUI->SetCheck(TRUE) : pCmdUI->SetCheck(FALSE);
}

void CATPBaseFrame::OnOptionsMoteurAbaque()
{
	if (m_pATPDocCreuset->IsValid())
	{
		m_pATPDocCreuset->m_MoteurPresse.SetType(CMoteurPresse::ABAQUE);
		m_pATPDocCreuset->UpdateAllViews(UPDATE_OPTIONSMOTEUR);
	}
}
void CATPBaseFrame::OnOptionsMoteurAbaqueEtal()
{
	if (m_pATPDocCreuset->IsValid())
	{
		m_pATPDocCreuset->m_MoteurPresse.SetType(CMoteurPresse::ABAQUE_ETAL);
		m_pATPDocCreuset->UpdateAllViews(UPDATE_OPTIONSMOTEUR);

		// Mise à jour des graphes (par defaut GRP Jour)
		m_pATPDocCreuset->m_AFFResultPlan.SetGraphGRPJour();
		m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_RESULT_PLAN);
	}
}
void CATPBaseFrame::OnOptionsMoteurProba()
{
	if (m_pATPDocCreuset->IsValid())
	{
		m_pATPDocCreuset->m_MoteurPresse.SetType(CMoteurPresse::PROBA);
		m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_PANEL);
		m_pATPDocCreuset->UpdateAllViews(UPDATE_OPTIONSMOTEUR);

		// Mise à jour des graphes (par defaut GRP Jour)
		m_pATPDocCreuset->m_AFFResultPlan.SetGraphGRPJour();
		m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_RESULT_PLAN);
	}
}

void CATPBaseFrame::OnOptionsRepriseEnMain()
{
	if (m_pATPDocCreuset->IsValid())
	{
		m_pATPDocCreuset->m_bRepriseEnMain = !m_pATPDocCreuset->m_bRepriseEnMain;
		m_pATPDocCreuset->UpdateAllViews(UPDATE_OPTIONSMOTEUR);
	}
}
void CATPBaseFrame::OnOptionsNoRepriseEnMain()
{
	if (m_pATPDocCreuset->IsValid())
	{
		m_pATPDocCreuset->m_bRepriseEnMain = !m_pATPDocCreuset->m_bRepriseEnMain;
		m_pATPDocCreuset->UpdateAllViews(UPDATE_OPTIONSMOTEUR);
	}
}
void CATPBaseFrame::OnOptionsLNMMagazine()
{
	if (m_pATPDocCreuset->IsValid())
	{
		m_pATPDocCreuset->m_bLNMMag = !m_pATPDocCreuset->m_bLNMMag;
		m_pATPDocCreuset->UpdateAllViews(UPDATE_OPTIONSMOTEUR);
	}
}
void CATPBaseFrame::OnOptionsLDPQuotidien()
{
	if (m_pATPDocCreuset->IsValid())
	{
		m_pATPDocCreuset->m_bLDPQtd = !m_pATPDocCreuset->m_bLDPQtd;
		m_pATPDocCreuset->UpdateAllViews(UPDATE_OPTIONSMOTEUR);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
// Changement Filtre d'Audience
void CATPBaseFrame::OnCrOptFiltreAudience()
{
	if(m_pATPDocCreuset->IsValid())
		m_pATPDocCreuset->m_AFFFiltreAudience = !m_pATPDocCreuset->m_AFFFiltreAudience;
	m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_PANEL);	

}

void CATPBaseFrame::OnUpdateCrOptFiltreAudience(CCmdUI *pCmdUI)
{
	// Vérifie étude valide
	pCmdUI->Enable((m_pATPDocCreuset->IsValid()) ? TRUE : FALSE);
}

////////////////////////////////////////////////////////////////////////////////////////////
// Gestion des courbes de réponse
void CATPBaseFrame::OnCrOptCourbeReponse()
{
	if(m_pATPDocCreuset->IsValid())
		m_pATPDocCreuset->m_AFFCourbeReponse = !m_pATPDocCreuset->m_AFFCourbeReponse;
	m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_PANEL);
}

void CATPBaseFrame::OnUpdateCrOptCourbeReponse(CCmdUI *pCmdUI)
{
	// Vérifie étude valide
	pCmdUI->Enable((m_pATPDocCreuset->IsValid()) ? TRUE : FALSE);
}


void CATPBaseFrame::OnPalette1()
{
	CATPColors::SetAppColor(CATPColors::COLORCREUSET, 0);
	m_pATPDocCreuset->UpdateAllViews(UPDATE_COLOR);
}
void CATPBaseFrame::OnPalette2()
{
	CATPColors::SetAppColor(CATPColors::COLORCREUSET, 1);
	m_pATPDocCreuset->UpdateAllViews(UPDATE_COLOR);
}
void CATPBaseFrame::OnPalette3()
{
	CATPColors::SetAppColor(CATPColors::COLORCREUSET, 2);
	m_pATPDocCreuset->UpdateAllViews(UPDATE_COLOR);
}
void CATPBaseFrame::OnPalette4()
{
	CATPColors::SetAppColor(CATPColors::COLORCREUSET, 3);
	m_pATPDocCreuset->UpdateAllViews(UPDATE_COLOR);
}
void CATPBaseFrame::OnPalette5()
{
	CATPColors::SetAppColor(CATPColors::COLORCREUSET, 4);
	m_pATPDocCreuset->UpdateAllViews(UPDATE_COLOR);
}
void CATPBaseFrame::OnPalette6()
{
	CATPColors::SetAppColor(CATPColors::COLORCREUSET, 5);
	m_pATPDocCreuset->UpdateAllViews(UPDATE_COLOR);
}

void CATPBaseFrame::OnPalette7()
{
	CATPColors::SetAppColor(CATPColors::COLORCREUSET, 6);
	m_pATPDocCreuset->UpdateAllViews(UPDATE_COLOR);
}
void CATPBaseFrame::OnPalette8()
{
	CATPColors::SetAppColor(CATPColors::COLORCREUSET, 7);
	m_pATPDocCreuset->UpdateAllViews(UPDATE_COLOR);
}
void CATPBaseFrame::OnPalette9()
{
	CATPColors::SetAppColor(CATPColors::COLORCREUSET, 8);
	m_pATPDocCreuset->UpdateAllViews(UPDATE_COLOR);
}

void CATPBaseFrame::OnPalette10()
{
	CATPColors::SetAppColor(CATPColors::COLORCREUSET, 9);
	m_pATPDocCreuset->UpdateAllViews(UPDATE_COLOR);
}

void CATPBaseFrame::OnPalette11()
{
	CATPColors::SetAppColor(CATPColors::COLORCREUSET, 10);
	m_pATPDocCreuset->UpdateAllViews(UPDATE_COLOR);
}

void CATPBaseFrame::OnPalette12()
{
	CATPColors::SetAppColor(CATPColors::COLORCREUSET, 11);
	m_pATPDocCreuset->UpdateAllViews(UPDATE_COLOR);
}

void CATPBaseFrame::OnPalette13()
{
	CATPColors::SetAppColor(CATPColors::COLORCREUSET, 12);
	m_pATPDocCreuset->UpdateAllViews(UPDATE_COLOR);
}

void CATPBaseFrame::OnPalette14()
{
	CATPColors::SetAppColor(CATPColors::COLORCREUSET, 13);
	m_pATPDocCreuset->UpdateAllViews(UPDATE_COLOR);
}

void CATPBaseFrame::OnPalette15()
{
	CATPColors::SetAppColor(CATPColors::COLORCREUSET, 14);
	m_pATPDocCreuset->UpdateAllViews(UPDATE_COLOR);
}

void CATPBaseFrame::OnChromie()
{
	m_pATPDocCreuset->m_AFFChromie = ! m_pATPDocCreuset->m_AFFChromie;
	m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_CREUSET);
}

void CATPBaseFrame::OnUpdateChromie(CCmdUI *pCmdUI)
{
	pCmdUI->Enable((m_pATPDocCreuset->IsValid()) ? TRUE : FALSE);
	m_pATPDocCreuset->m_AFFChromie ? pCmdUI->SetCheck() : pCmdUI->SetCheck(FALSE);
//	pCmdUI->Enable(0);//à retirer
}

void CATPBaseFrame::OnStatutInsert()
{
	m_pATPDocCreuset->m_AFFStatutInsert = ! m_pATPDocCreuset->m_AFFStatutInsert;
	m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_CREUSET);
}

void CATPBaseFrame::OnUpdateStatutInsert(CCmdUI *pCmdUI)
{
	m_pATPDocCreuset->m_AFFStatutInsert ? pCmdUI->SetCheck() : pCmdUI->SetCheck(FALSE);
	pCmdUI->Enable(0);//à retirer
}

void CATPBaseFrame::OnStatutMoteur()
{
	m_pATPDocCreuset->m_AFFOptionsMoteur = ! m_pATPDocCreuset->m_AFFOptionsMoteur;
	m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_PANEL);
}

void CATPBaseFrame::OnUpdateStatutMoteur(CCmdUI *pCmdUI)
{
	pCmdUI->Enable((m_pATPDocCreuset->IsValid()) ? TRUE : FALSE);
	//pCmdUI->Enable(0);//à retirer
	m_pATPDocCreuset->m_AFFOptionsMoteur ? pCmdUI->SetCheck() : pCmdUI->SetCheck(FALSE);
}


// Suppression des insertions format(s) actif(s) sur l'hypothèse active et sur la période d'action
void CATPBaseFrame::OnSuppinsHypact()
{

	// Test si au moins 1 support
	if (!m_pATPDocCreuset->m_KEYEtude.GetDateD().IsValid()) return;

	CString ttl, qst;
	ttl.LoadString(IDR_CR_ATPFRAME);
	if(m_pATPDocCreuset->m_AFFCreuset.GetAffQuickPlan())
		qst = "Voulez-vous supprimer les insertions de l'hypothèse active?";
	else
		qst.LoadString(IDS_CR_SUPPINSERT);
	if ( MessageBox(qst, ttl, MB_ICONQUESTION|MB_OKCANCEL) == IDOK)
	{
		// Traitement suppression
		m_pATPDocCreuset->OnSuppInsertion(m_pATPDocCreuset->m_IdxHypotheseCourant,false);

		// on met à jour les vues du document
		m_pATPDocCreuset->UpdateAllViews(UPDATE_CURRENT_HYPO);
	}
}

// Suppression des insertions format(s) actif(s) sur toutes les hypothèses et sur la période d'action
void CATPBaseFrame::OnSuppinsTouteshyps()
{
	// Test si au moins 1 support
	if (!m_pATPDocCreuset->m_KEYEtude.GetDateD().IsValid()) return;

	CString ttl, qst;
	ttl.LoadString(IDR_CR_ATPFRAME);
	if(m_pATPDocCreuset->m_AFFCreuset.GetAffQuickPlan())
		qst = "Voulez-vous supprimer les insertions dans toutes les hypothèses?";
	else
		qst.LoadString(IDS_CR_SUPPINSERT);
	if ( MessageBox(qst, ttl, MB_ICONQUESTION|MB_OKCANCEL) == IDOK)
	{
		// Activation état effacement hypothèses
		m_pATPDocCreuset->OnSuppInsertionAllHypo();

		// on met à jour les vues du document
		m_pATPDocCreuset->UpdateAllViews(UPDATE_ETUDE);
	}

}

void CATPBaseFrame::OnUpdateSuppinsTouteshyps(CCmdUI *pCmdUI)
{
	// Vérifie étude valide
	pCmdUI->Enable((m_pATPDocCreuset->IsValid() && m_pATPDocCreuset->m_IdxHypotheseActif!=0) ? TRUE : FALSE);
}

void CATPBaseFrame::OnUpdateSuppinsHypact(CCmdUI *pCmdUI)
{
	// Vérifie étude valide
	pCmdUI->Enable((m_pATPDocCreuset->IsValid() && m_pATPDocCreuset->m_IdxHypotheseActif!=0) ? TRUE : FALSE);
}

///////////////////////////////////////////////////////////////
// Mise à jour du menu hypothèses pour la copie des hypothèses
void CATPBaseFrame::OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu)
{
	JFCFrame::OnMenuSelect(nItemID, nFlags, hSysMenu);

	//Menu choix moteur et reprise en main
	if(m_pATPDocCreuset->m_AFFCreuset.GetAffQuickPlan())
		return;

	// if(nItemID == 4)
	if(nItemID == 3)
	{
		CMenu *pMenuCreuset;
		pMenuCreuset = this->GetMenu();
		
		if (pMenuCreuset == NULL)
			return;

		// Récup le menu hypothèses
		CMenu *pMenu;
		pMenu = pMenuCreuset->GetSubMenu(nItemID);
		ASSERT(pMenu);
		
		// Récup ss menu choix moteur
		CMenu *pMenuChoixMoteur;
		pMenuChoixMoteur = pMenu->GetSubMenu(4);

		if (pMenuChoixMoteur == NULL)
			return;
		
		//Il faut creer la clé registre Moteur/VersionJFC == 1 pour acceder totalement à ces menu
		if(!AfxGetApp()->GetProfileInt("Moteur", "VersionJFC", 0) )
		{
	 		pMenuChoixMoteur->DeleteMenu(IDM_CR_MODEL, MF_BYCOMMAND);
			CMenu *pMenuReprises = NULL;

			pMenuReprises = pMenu->GetSubMenu(5);
			if(pMenuReprises && pMenuReprises->DeleteMenu(IDM_CR_REPRISES, MF_BYCOMMAND))
				pMenu->DeleteMenu(5, MF_BYPOSITION);
		}
	}

}

///////////////////////////////////////////////////
// Création d'une version plan de référence
void CATPBaseFrame::OnCreerVersionPlanref()
{
	// Test si document en cours
	if (m_pATPDocCreuset)
	{
		CPlanMasters Version;
		Version.m_DateArchive.GetCurrentDateTime();      

		// On demande le nom de ce plan
		CATPLibelle Dlg(IDS_CR_LBLPLANMAITRE, this);
		
		JDate DateArchive = Version.m_DateArchive.GetDate();
		JTime TimeArchive = Version.m_DateArchive.GetTime();
		CString TxtDateArchive = CFormatStr::FormatDateLong(DateArchive, TimeArchive);

		// Formate info date - time
		Dlg.SetLibelle(JLabel(TxtDateArchive));

		// on affiche la boîte de dialogue
		if (IDOK == Dlg.DoModal())
		{
			// on récupère le libellé du plan
			OnValidHypothese();
			// Save a version of the Plan Master (version nominative)
			CPlanMasters Version;
			Version.m_Plan			= m_pATPDocCreuset->m_MasterPlan;
	
			Version.m_Libelle = Dlg.GetLibelle();
			Version.m_DateArchive.GetCurrentDateTime();      

			// On ajoute le plan à la table des plans archivés
			m_pATPDocCreuset->m_TblPlanArchive.AddTail() = Version; 
		}			
	}
}

void CATPBaseFrame::OnUpdateCreerVersionPlanref(CCmdUI *pCmdUI)
{
	// on modifie l'état du menu
	pCmdUI->Enable((m_pATPDocCreuset->IsValid() && m_pATPDocCreuset->m_IdxHypotheseActif!=0) ? TRUE : FALSE);
}

///////////////////////////////////////////////////
// Charger une version plan de référence
void CATPBaseFrame::OnChargerVersionPlanref()
{
	// Test si document en cours
	if (m_pATPDocCreuset)
	{
		// Test si au moins une ancienne version à proposer
		if (m_pATPDocCreuset->m_TblPlanArchive.GetCount())
		{
			// Ouvrir boite sélection des versions précédémment sauvegardées et nommées
			CDlgVersionPlan dlg(false);

			// On passe le document
			dlg.m_pATPDocCreuset = m_pATPDocCreuset;

			// Ouverture
			if( dlg.DoModal()== IDOK)
			{
				// Sauvegarde du master courant si version différente
				if (!(m_pATPDocCreuset->m_MasterPlan == dlg.m_PlanRefArchive))
				{
					// On teste également si version non sauvegardé précédemment, pour éviter les doublons
					CPlanMasters Version;
					Version.m_Plan			= m_pATPDocCreuset->m_MasterPlan;
					JBool NewVersion = true;
					for (m_pATPDocCreuset->m_TblPlanArchive.MoveFirst(); 
						m_pATPDocCreuset->m_TblPlanArchive.IsValid();
						m_pATPDocCreuset->m_TblPlanArchive.MoveNext())
					{
						if (Version.m_Plan == m_pATPDocCreuset->m_TblPlanArchive.GetItem().m_Plan)
						{
							// Version déjà archivé
							NewVersion = false;
							break;
						}

					}
					// Au moment du changement on sauve la version si non déjà existante
					if (NewVersion)
						m_pATPDocCreuset->m_TblPlanArchive.AddTail() = Version; 
				}

				// Récupère nouveau master
				m_pATPDocCreuset->m_MasterPlan = dlg.m_PlanRefArchive;

				// Do nothing if Plan Master of invalid index
				if(m_pATPDocCreuset->CopyMasterPlanToHypothese(m_pATPDocCreuset->m_IdxHypotheseActif))
					// Mise à jour Creuset
					m_pATPDocCreuset->UpdateAllViews(UPDATE_CURRENT_HYPO);
			}
			
		}

	}
}

void CATPBaseFrame::OnUpdateChargerVersionPlanref(CCmdUI *pCmdUI)
{
	// Vérifie étude valide
	pCmdUI->Enable((m_pATPDocCreuset->IsValid() && m_pATPDocCreuset->m_IdxHypotheseActif!=0 && m_pATPDocCreuset->m_TblPlanArchive.GetCount()) ? TRUE : FALSE);
}


// Redéfinition de la période étude
void CATPBaseFrame::OnPeriodeEtude()
{
	bool bDateChanged, bOffreModified;

	// on recopie les dates de l'etude
	JDate DebEtude = m_pATPDocCreuset->m_KEYEtude.GetDateD();
	JDate FinEtude = m_pATPDocCreuset->m_KEYEtude.GetDateF();

	m_pATPDocCreuset->m_ListSupportsChange = false;
	JMap<JUnt32, JUnt32> ExMapSupports;
	ExMapSupports.Reset();
	for (m_pATPDocCreuset->m_Offre.m_TBLIdSupports.MoveFirst(); m_pATPDocCreuset->m_Offre.m_TBLIdSupports.IsValid(); m_pATPDocCreuset->m_Offre.m_TBLIdSupports.MoveNext())
	{
		JUnt32 IdSupport = m_pATPDocCreuset->m_Offre.m_TBLIdSupports.GetItem();
		ExMapSupports.Add(IdSupport) = IdSupport;
	}

	// on ouvre le dialogue de modification de l'offre
	if (OpenOffreDialog("Modification de la période de l'étude courante", m_CATPColorsApp, 
		m_pATPDocCreuset->m_Offre, COffre2MainDialog::eodoPeriode,
		DebEtude, FinEtude, bDateChanged, bOffreModified, true, this))
	{
		m_pATPDocCreuset->SetModifiedFlag();

		// si la nouvelle offre a bien été modifiée et validée
		if (bDateChanged)
		{
			// Récupération des nouvelles bornes date étude
			m_pATPDocCreuset->m_KEYEtude.SetDateD(DebEtude);
			m_pATPDocCreuset->m_KEYEtude.SetDateF(FinEtude);

			m_pATPDocCreuset->m_KEYPeriodeAction.SetPeriode(DebEtude, FinEtude);
			m_pATPDocCreuset->m_KEYPeriodeActionHyp.SetPeriode(DebEtude, FinEtude);

			// on met à jour les vues du document
			m_pATPDocCreuset->UpdateAllViews(UPDATE_ETUDE);
		}

		// Reconstruit la table des items interdits
		m_pATPDocCreuset->m_Offre.ConstructionItemsInterdits();

		if (ExMapSupports.GetCount() != m_pATPDocCreuset->m_Offre.m_TBLIdSupports.GetCount())
		{
			m_pATPDocCreuset->m_ListSupportsChange = true;
		}
		else
		{
			for (m_pATPDocCreuset->m_Offre.m_TBLIdSupports.MoveFirst(); m_pATPDocCreuset->m_Offre.m_TBLIdSupports.IsValid(); m_pATPDocCreuset->m_Offre.m_TBLIdSupports.MoveNext())
			{
				JUnt32 IdSupport = m_pATPDocCreuset->m_Offre.m_TBLIdSupports.GetItem();
				ExMapSupports.MoveTo(IdSupport);
				if (!ExMapSupports.IsValid())
				{
					m_pATPDocCreuset->m_ListSupportsChange = true;	
				}
			}
		}

		// Mise à jour globale
		m_pATPDocCreuset->UpdateAllViews(UPDATE_OFFRE);
	}
}

// Validation option prolongation période étude
void CATPBaseFrame::OnUpdatePeriodeEtude(CCmdUI *pCmdUI)
{
	if (m_pATPDocCreuset->IsValid())
	{
		// Document valide / test si dates définis
		pCmdUI->Enable((m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD().IsValid() &&
			            m_pATPDocCreuset->m_KEYPeriodeAction.GetDateF().IsValid()) ? TRUE : FALSE);
	}
	else
		// Option non valide
		pCmdUI->Enable(FALSE);
}


void CATPBaseFrame::OnRenameHypo()
{
	if (m_pATPDocCreuset)
	{
		m_pATPDocCreuset->m_TblHypotheses.MoveTo(m_pATPDocCreuset->m_IdxHypotheseActif);
		if (m_pATPDocCreuset->m_TblHypotheses.IsValid())
		{
			CPlanInsertions & plan = m_pATPDocCreuset->m_TblHypotheses.GetItem();
			CATPLibelle dlg(IDS_CR_LBLRENHYPO, this);
			
			CString str;
			if (plan.GetLabel().IsEmpty())
				str.FormatMessage(IDS_CR_HYPOTHESE, m_pATPDocCreuset->m_IdxHypotheseActif);
			else
				str = plan.GetLabel().AsJCharPtr();
			dlg.SetLibelle(JLabel(str));
			if (dlg.DoModal() == IDOK)
			{
				plan.SetLabel(dlg.GetLibelle());
				UpdateAllViews(UPDATE_AFF_CREUSET);
			}

		}
	}
}

void CATPBaseFrame::OnUpdateRenameHypo(CCmdUI *pCmdUI)
{
	// Vérifie étude valide
	pCmdUI->Enable((m_pATPDocCreuset->IsValid() && m_pATPDocCreuset->m_IdxHypotheseActif!=0) ? TRUE : FALSE);

}


void CATPBaseFrame::OnUpdateOffreFormats(CCmdUI *pCmdUI)
{
	pCmdUI->Enable((m_pATPDocCreuset->IsValid()) ? TRUE : FALSE);
}

void CATPBaseFrame::OnUpdateOffreSources(CCmdUI *pCmdUI)
{
	pCmdUI->Enable((m_pATPDocCreuset->IsValid()) ? TRUE : FALSE);
}

void CATPBaseFrame::OnUpdateOffreSupports(CCmdUI *pCmdUI)
{
	pCmdUI->Enable((m_pATPDocCreuset->IsValid()) ? TRUE : FALSE);
}

void CATPBaseFrame::OnUpdateOffreCibles(CCmdUI *pCmdUI)
{
	pCmdUI->Enable((m_pATPDocCreuset->IsValid()) ? TRUE : FALSE);
}



void CATPBaseFrame::OnOptionsLDP30j()
{
	if (m_pATPDocCreuset->IsValid())
	{
		m_pATPDocCreuset->m_bLDP30J = !m_pATPDocCreuset->m_bLDP30J;
		m_pATPDocCreuset->UpdateAllViews(UPDATE_OPTIONSMOTEUR);
	}
}

void CATPBaseFrame::OnUpdateOptionsLDP30j(CCmdUI *pCmdUI)
{
	//pCmdUI->Enable((m_pATPDocCreuset->IsValid()) ? TRUE : FALSE);
	pCmdUI->Enable(FALSE);
	m_pATPDocCreuset->m_bLDP30J ? pCmdUI->SetCheck(TRUE) : pCmdUI->SetCheck(FALSE);
}

void CATPBaseFrame::OnOffreItemsVentilation()
{
	CDialogVentilation dlg(this);
	dlg.SetDocument(m_pATPDocCreuset);
	CKeyTerrain keyterrain;
	m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.MoveFirst();
	keyterrain.Set(m_pATPDocCreuset->m_IdSrcAudienceSel, m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.GetKey());
	dlg.SetTerrain(keyterrain);
	dlg.DoModal();
}

void CATPBaseFrame::OnUpdateOffreItemsVentilation(CCmdUI *pCmdUI)
{
	pCmdUI->Enable((m_pATPDocCreuset->IsValid()) ? TRUE : FALSE);
//	pCmdUI->Enable(FALSE);
}



void CATPBaseFrame::OnSysColorChange()
{
	JFCFrame::OnSysColorChange();
	m_pATPDocCreuset->UpdateAllViews(UPDATE_COLOR);
}



///////////////////////////////////////////////////////////////////////////////////
// Mode 0 décimale pour l'affichage des coûts
void CATPBaseFrame::OnAucunedecimale()
{
	// Sauve nouve nb décimale pour coût
	m_pATPDocCreuset->m_NbDecCout = 0;

	// Update résultat suite à la modif paramètre nb insertions
	m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_CREUSET);
}

///////////////////////////////////////////////////////////////////////////////////
// Mode 1 décimale pour l'affichage des coûts
void CATPBaseFrame::On1decimale()
{
	// Sauve nouve nb décimale pour coût
	m_pATPDocCreuset->m_NbDecCout = 1;

	// Update résultat suite à la modif paramètre nb insertions
	m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_CREUSET);
}

///////////////////////////////////////////////////////////////////////////////////
// Mode 2 décimale pour l'affichage des coûts
void CATPBaseFrame::On2decimale()
{
	// Sauve nouve nb décimale pour coût
	m_pATPDocCreuset->m_NbDecCout = 2;

	// Update résultat suite à la modif paramètre nb insertions
	m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_CREUSET);
}

///////////////////////////////////////////////////////////////////////////////////
// Update affichage mode décimale pour les coûts coûts
void CATPBaseFrame::OnUpdateNbddecimale(CCmdUI *pCmdUI)
{
	// Dévalide toutes les sélections
	pCmdUI->SetRadio(0);

	// Mise à jour du ss-menu sélectionné
	if(pCmdUI->m_nID == IDM_CR_NBD_AUCUNEDECIMALE && m_pATPDocCreuset->m_NbDecCout == 0)
		pCmdUI->SetRadio(1);
	if(pCmdUI->m_nID == IDM_CR_NBD_1DECIMALE && m_pATPDocCreuset->m_NbDecCout == 1)
		pCmdUI->SetRadio(1);
	if(pCmdUI->m_nID == IDM_CR_NBD_2DECIMALE && m_pATPDocCreuset->m_NbDecCout == 2)
		pCmdUI->SetRadio(1);


}


//////////////////////////////////////////////////////////////////////////////////////////
// Export des résultats Creuset plan
void CATPBaseFrame::OnExportResultPlan()
{
	// pop-up file-open dlg to ask for location
	CFileDialog dlgFile(
		FALSE,
		_T(".xls"),
		NULL,
		OFN_NOCHANGEDIR|OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("Fichier Excel (*.xls)|*.xls|Fichier Texte(*.txt)|*.txt|"),this);


	if (dlgFile.DoModal( ) == IDCANCEL)
		return;

	if(dlgFile.GetFileExt().CompareNoCase("xls") == 0)
	{
		CSharedFile clipb (GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);

		if(ExportResultPlan(clipb))
		{

			VERIFY(OpenClipboard());
			BOOL bEmpty = EmptyClipboard();
			VERIFY(CloseClipboard());

			HGLOBAL hData = 0;
			HANDLE hResult = 0;
			hData = clipb.Detach();

			// Now, hand over memory block to clipboard
			if (bEmpty && hData)
			{
				VERIFY(OpenClipboard());
				hResult = SetClipboardData(CF_TEXT, hData);
				VERIFY(CloseClipboard());

				CExcelPrinter excelPrinter(CATPColors::COLORCREUSET, this);

				CString path = AfxGetApp()->GetProfileString("", "InstallPath", 0);
				path += "\\MacroPresse.xla";
				excelPrinter.SaveAsFile(dlgFile.GetPathName(), path, "DefinirTypeExportPresse");
			}
		}
	}

	else
	{

		CStdioFile txtFile;
		CFileException ex;
		if(txtFile.Open(dlgFile.GetPathName( ), CFile::modeCreate | CFile::modeWrite | CFile::typeText, &ex))
		{
			ExportResultPlan(txtFile);
			txtFile.Close();
		}
		else
			ex.ReportError();

	}
}

void CATPBaseFrame::OnClipboardResultPlan()
{
	CSharedFile clipb (GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);

	if(ExportResultPlan(clipb))
	{

		VERIFY(OpenClipboard());
		BOOL bEmpty = EmptyClipboard();
		VERIFY(CloseClipboard());

		HGLOBAL hData = 0;
		HANDLE hResult = 0;
		hData = clipb.Detach();

		// Now, hand over memory block to clipboard
		if (bEmpty && hData)
		{
			VERIFY(OpenClipboard());
			hResult = SetClipboardData(CF_TEXT, hData);
			VERIFY(CloseClipboard());
		}
	}
}

void CATPBaseFrame::OnPrintResultPlan()
{
	CSharedFile clipb (GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);

	if(ExportResultPlan(clipb))
	{

		VERIFY(OpenClipboard());
		BOOL bEmpty = EmptyClipboard();
		VERIFY(CloseClipboard());

		HGLOBAL hData = 0;
		HANDLE hResult = 0;
		hData = clipb.Detach();

		// Now, hand over memory block to clipboard
		if (bEmpty && hData)
		{
			VERIFY(OpenClipboard());
			hResult = SetClipboardData(CF_TEXT, hData);
			VERIFY(CloseClipboard());

			CExcelPrinter excelPrinter(CATPColors::COLORCREUSET, this);
			CString path = AfxGetApp()->GetProfileString("", "InstallPath", 0);
			path += "\\MacroPresse.xla";
			#ifdef _DEBUG
				excelPrinter.PrintPreview( path, "DefinirTypeExportPresse");
			#else
				excelPrinter.PrintPreview( path, "DefinirTypeExportPresse");
			#endif
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
// Export des résultats Creuset Calendrier
void CATPBaseFrame::OnExportResultCalend()
{
	// pop-up file-open dlg to ask for location
	CFileDialog dlgFile(
		FALSE,
		_T(".xls"),
		NULL,
		OFN_NOCHANGEDIR|OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("Fichier Excel (*.xls)|*.xls|Fichier Texte(*.txt)|*.txt|"),this);

	if (dlgFile.DoModal( ) == IDCANCEL)
		return;

	if(dlgFile.GetFileExt().CompareNoCase("xls") == 0)
	{
		CSharedFile clipb (GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);

		if(ExportResultCalend(clipb))
		{

			VERIFY(OpenClipboard());
			BOOL bEmpty = EmptyClipboard();
			VERIFY(CloseClipboard());

			HGLOBAL hData = 0;
			HANDLE hResult = 0;
			hData = clipb.Detach();

			// Now, hand over memory block to clipboard
			if (bEmpty && hData)
			{
				VERIFY(OpenClipboard());
				hResult = SetClipboardData(CF_TEXT, hData);
				VERIFY(CloseClipboard());

				CExcelPrinter excelPrinter(CATPColors::COLORCREUSET, this);
				CString path = AfxGetApp()->GetProfileString("", "InstallPath", 0);
				path += "\\MacroPresse.xla";
				excelPrinter.SaveAsFile(dlgFile.GetPathName( ), path, "DefinirTypeExportPresse");
			}
		}
	}
	else
	{
		CStdioFile txtFile;
		CFileException ex;
		if(txtFile.Open(dlgFile.GetPathName( ), CFile::modeCreate | CFile::modeWrite | CFile::typeText, &ex))
		{
			ExportResultCalend(txtFile);
			txtFile.Close();
		}
		else
			ex.ReportError();


	}
}
void CATPBaseFrame::OnClipboardResultCalend()
{
	CSharedFile clipb (GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);

	if(ExportResultCalend(clipb))
	{

		VERIFY(OpenClipboard());
		BOOL bEmpty = EmptyClipboard();
		VERIFY(CloseClipboard());

		HGLOBAL hData = 0;
		HANDLE hResult = 0;
		hData = clipb.Detach();

		// Now, hand over memory block to clipboard
		if (bEmpty && hData)
		{
			VERIFY(OpenClipboard());
			hResult = SetClipboardData(CF_TEXT, hData);
			VERIFY(CloseClipboard());
		}
	}
}

void CATPBaseFrame::OnPrintResultCalend()
{
	CSharedFile clipb (GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);

	if(ExportResultCalend(clipb))
	{

		VERIFY(OpenClipboard());
		BOOL bEmpty = EmptyClipboard();
		VERIFY(CloseClipboard());

		HGLOBAL hData = 0;
		HANDLE hResult = 0;
		hData = clipb.Detach();

		// Now, hand over memory block to clipboard
		if (bEmpty && hData)
		{
			VERIFY(OpenClipboard());
			hResult = SetClipboardData(CF_TEXT, hData);
			VERIFY(CloseClipboard());

			CExcelPrinter excelPrinter(CATPColors::COLORCREUSET, this);
			CString path = AfxGetApp()->GetProfileString("", "InstallPath", 0);
			path += "\\MacroPresse.xla";
			#ifdef _DEBUG
				excelPrinter.PrintPreview( path, "DefinirTypeExportPresse");
			#else
				excelPrinter.PrintPreview( path, "DefinirTypeExportPresse");
			#endif
		}
	}
}

void CATPBaseFrame::OnUpdateExportResult(CCmdUI *pCmdUI)
{
	// A FAIRE -- si au moins 1 résultat à exporter
	if (m_pATPDocCreuset->IsValid())
	{
		// ici on testera au moins 1 insertions 
		pCmdUI->Enable(TRUE);
	}
	else
		// Option non valide
		pCmdUI->Enable(FALSE);
}

void CATPBaseFrame::OnUpdateExcelEchelle(CCmdUI *pCmdUI)
{
	// Export echelle valide si au moins 1 support
	pCmdUI->Enable(m_pATPDocCreuset->m_EchelleSupports.GetCount() > 0);
}

void CATPBaseFrame::OnExcelEchelle()
{
	CSharedFile clipb (GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);

	if(ExportEchelle(clipb))
	{
		VERIFY(OpenClipboard());
		BOOL bEmpty = EmptyClipboard();
		VERIFY(CloseClipboard());

		HGLOBAL hData = 0;
		HANDLE hResult = 0;
		hData = clipb.Detach();

		// Now, hand over memory block to clipboard
		if (bEmpty && hData)
		{
			VERIFY(OpenClipboard());
			hResult = SetClipboardData(CF_TEXT, hData);
			VERIFY(CloseClipboard());

			CExcelPrinter excelPrinter(CATPColors::COLORCREUSET, this);
			CString path = AfxGetApp()->GetProfileString("", "InstallPath", 0);
			path += "\\MacroPresse.xla";
			excelPrinter.Open( path, "DefinirTypeExportPresse");
		}
	}
}

void CATPBaseFrame::OnUpdateExcelTout(CCmdUI *pCmdUI)
{
	pCmdUI->Enable((m_pATPDocCreuset->IsValid()) ? TRUE : FALSE);
}

void CATPBaseFrame::OnExcelTout()
{
	CSharedFile clipb (GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);

	if(ExportEchelle(clipb) && ExportResultPlan(clipb) && ExportResultCalend(clipb))
	{
		VERIFY(OpenClipboard());
		BOOL bEmpty = EmptyClipboard();
		VERIFY(CloseClipboard());

		HGLOBAL hData = 0;
		HANDLE hResult = 0;
		hData = clipb.Detach();

		// Now, hand over memory block to clipboard
		if (bEmpty && hData)
		{
			VERIFY(OpenClipboard());
			hResult = SetClipboardData(CF_TEXT, hData);
			VERIFY(CloseClipboard());

			CExcelPrinter excelPrinter(CATPColors::COLORCREUSET, this);
			CString path = AfxGetApp()->GetProfileString("", "InstallPath", 0);
			path += "\\MacroPresse.xla";
			excelPrinter.Open(path, "DefinirTypeExportPresse");

		}
	}
}

void CATPBaseFrame::OnExcelResultPlan()
{
	CSharedFile clipb (GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);

	if(ExportResultPlan(clipb))
	{
		VERIFY(OpenClipboard());
		BOOL bEmpty = EmptyClipboard();
		VERIFY(CloseClipboard());

		HGLOBAL hData = 0;
		HANDLE hResult = 0;
		hData = clipb.Detach();

		// Now, hand over memory block to clipboard
		if (bEmpty && hData)
		{
			VERIFY(OpenClipboard());
			hResult = SetClipboardData(CF_TEXT, hData);
			VERIFY(CloseClipboard());

			CExcelPrinter excelPrinter(CATPColors::COLORCREUSET, this);

			CString path = AfxGetApp()->GetProfileString("", "InstallPath", 0);
			path += "\\MacroPresse.xla";
			excelPrinter.Open(path, "DefinirTypeExportPresse");
		}
	}	
}

void CATPBaseFrame::OnExcelResultCalend()
{
	CSharedFile clipb (GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);

	if(ExportResultCalend(clipb))
	{
		VERIFY(OpenClipboard());
		BOOL bEmpty = EmptyClipboard();
		VERIFY(CloseClipboard());

		HGLOBAL hData = 0;
		HANDLE hResult = 0;
		hData = clipb.Detach();

		// Now, hand over memory block to clipboard
		if (bEmpty && hData)
		{
			VERIFY(OpenClipboard());
			hResult = SetClipboardData(CF_TEXT, hData);
			VERIFY(CloseClipboard());

			CExcelPrinter excelPrinter(CATPColors::COLORCREUSET, this);
			CString path = AfxGetApp()->GetProfileString("", "InstallPath", 0);
			path += "\\MacroPresse.xla";
			excelPrinter.Open(path, "DefinirTypeExportPresse");
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
// Export Echelle Creuset
void CATPBaseFrame::OnExportEchelle()
{
	// pop-up file-open dlg to ask for location
	CFileDialog dlgFile(
		FALSE,
		_T(".xls"),
		NULL,
		OFN_NOCHANGEDIR|OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("Fichier Excel (*.xls)|*.xls|Fichier Texte(*.txt)|*.txt|"),this);

	if (dlgFile.DoModal( ) == IDCANCEL)
		return;

	if(dlgFile.GetFileExt().CompareNoCase("xls") == 0)
	{
		CSharedFile clipb (GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);

		if(ExportEchelle(clipb))
		{

			VERIFY(OpenClipboard());
			BOOL bEmpty = EmptyClipboard();
			VERIFY(CloseClipboard());

			HGLOBAL hData = 0;
			HANDLE hResult = 0;
			hData = clipb.Detach();

			// Now, hand over memory block to clipboard
			if (bEmpty && hData)
			{
				VERIFY(OpenClipboard());
				hResult = SetClipboardData(CF_TEXT, hData);
				VERIFY(CloseClipboard());
				
				CExcelPrinter excelPrinter(CATPColors::COLORCREUSET, this);
				CString path = AfxGetApp()->GetProfileString("", "InstallPath", 0);
				path += "\\MacroPresse.xla";
				excelPrinter.SaveAsFile(dlgFile.GetPathName( ), path, "DefinirTypeExportPresse");

			}
		}
	}
	else
	{
		CStdioFile txtFile;
		CFileException ex;
		if(txtFile.Open(dlgFile.GetPathName( ), CFile::modeCreate | CFile::modeWrite | CFile::typeText, &ex))
		{
			ExportEchelle(txtFile);
			txtFile.Close();
		}
		else
			ex.ReportError();

	}
}


void CATPBaseFrame::OnClipboardEchelle()
{
	CSharedFile clipb (GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);

	if(ExportEchelle(clipb))
	{

		VERIFY(OpenClipboard());
		BOOL bEmpty = EmptyClipboard();
		VERIFY(CloseClipboard());

		HGLOBAL hData = 0;
		HANDLE hResult = 0;
		hData = clipb.Detach();

		// Now, hand over memory block to clipboard
		if (bEmpty && hData)
		{
			VERIFY(OpenClipboard());
			hResult = SetClipboardData(CF_TEXT, hData);
			VERIFY(CloseClipboard());
		}
	}
}
void CATPBaseFrame::OnPrintEchelle()
{
	CSharedFile clipb (GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);

	if(ExportEchelle(clipb))
	{

		VERIFY(OpenClipboard());
		BOOL bEmpty = EmptyClipboard();
		VERIFY(CloseClipboard());

		HGLOBAL hData = 0;
		HANDLE hResult = 0;
		hData = clipb.Detach();

		// Now, hand over memory block to clipboard
		if (bEmpty && hData)
		{
			VERIFY(OpenClipboard());
			hResult = SetClipboardData(CF_TEXT, hData);
			VERIFY(CloseClipboard());
			
			CExcelPrinter excelPrinter(CATPColors::COLORCREUSET, this);
			CString path = AfxGetApp()->GetProfileString("", "InstallPath", 0);
			path += "\\MacroPresse.xla";
			#ifdef _DEBUG
				excelPrinter.PrintPreview( path, "DefinirTypeExportPresse");
			#else
				excelPrinter.PrintPreview( path, "DefinirTypeExportPresse");
			#endif

		}
	}
}

void CATPBaseFrame::OnUpdateExportEchelle(CCmdUI *pCmdUI)
{
	// Export echelle valide si au moins 1 support
	pCmdUI->Enable(m_pATPDocCreuset->m_EchelleSupports.GetCount() > 0);
}



//////////////////////////////////////////////////////////////////////////////////////////
// Export Tout Creuset
void CATPBaseFrame::OnExportTout()
{
	// pop-up file-open dlg to ask for location
	CFileDialog dlgFile(
		FALSE,
		_T(".xls"),
		NULL,
		OFN_NOCHANGEDIR|OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("Fichier Excel (*.xls)|*.xls|Fichier Texte(*.txt)|*.txt|"),this);

	if (dlgFile.DoModal( ) == IDCANCEL)
		return;

	if(dlgFile.GetFileExt().CompareNoCase("xls") == 0)
	{
		CSharedFile clipb (GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);

		if(ExportEchelle(clipb) && ExportResultPlan(clipb) && ExportResultCalend(clipb))
		{

			VERIFY(OpenClipboard());
			BOOL bEmpty = EmptyClipboard();
			VERIFY(CloseClipboard());

			HGLOBAL hData = 0;
			HANDLE hResult = 0;
			hData = clipb.Detach();

			// Now, hand over memory block to clipboard
			if (bEmpty && hData)
			{
				VERIFY(OpenClipboard());
				hResult = SetClipboardData(CF_TEXT, hData);
				VERIFY(CloseClipboard());

				CExcelPrinter excelPrinter(CATPColors::COLORCREUSET, this);
				CString path = AfxGetApp()->GetProfileString("", "InstallPath", 0);
				path += "\\MacroPresse.xla";
				excelPrinter.SaveAsFile(dlgFile.GetPathName( ), path, "DefinirTypeExportPresse");

			}
		}
	}
	else
	{

		CStdioFile txtFile;
		CFileException ex;
		if(txtFile.Open(dlgFile.GetPathName( ), CFile::modeCreate | CFile::modeWrite | CFile::typeText, &ex))
		{
			ExportEchelle(txtFile);
			ExportResultPlan(txtFile);
			ExportResultCalend(txtFile);
			txtFile.Close();
		}
		else
			ex.ReportError();



	}
}

void CATPBaseFrame::OnClipboardTout()
{
	CSharedFile clipb (GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);

	if(ExportEchelle(clipb) && ExportResultPlan(clipb) && ExportResultCalend(clipb))
	{

		VERIFY(OpenClipboard());
		BOOL bEmpty = EmptyClipboard();
		VERIFY(CloseClipboard());

		HGLOBAL hData = 0;
		HANDLE hResult = 0;
		hData = clipb.Detach();

		// Now, hand over memory block to clipboard
		if (bEmpty && hData)
		{
			VERIFY(OpenClipboard());
			hResult = SetClipboardData(CF_TEXT, hData);
			VERIFY(CloseClipboard());
		}
	}
}

void CATPBaseFrame::OnPrintTout()
{
	CSharedFile clipb (GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);

	if(ExportEchelle(clipb) && ExportResultPlan(clipb) && ExportResultCalend(clipb))
	{

		VERIFY(OpenClipboard());
		BOOL bEmpty = EmptyClipboard();
		VERIFY(CloseClipboard());

		HGLOBAL hData = 0;
		HANDLE hResult = 0;
		hData = clipb.Detach();

		// Now, hand over memory block to clipboard
		if (bEmpty && hData)
		{
			VERIFY(OpenClipboard());
			hResult = SetClipboardData(CF_TEXT, hData);
			VERIFY(CloseClipboard());

			CExcelPrinter excelPrinter(CATPColors::COLORCREUSET, this);
			CString path = AfxGetApp()->GetProfileString("", "InstallPath", 0);
			path += "\\MacroPresse.xla";
			#ifdef _DEBUG
				excelPrinter.PrintPreview( path, "DefinirTypeExportPresse");
			#else
				excelPrinter.PrintPreview( path, "DefinirTypeExportPresse");
			#endif

		}
	}
}

void CATPBaseFrame::OnUpdateExportTout(CCmdUI *pCmdUI)
{
	pCmdUI->Enable((m_pATPDocCreuset->IsValid()) ? TRUE : FALSE);
}//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
bool CATPBaseFrame::ExportEchelle(CFile & file)
{
	// Flag validité de l'export
	bool ExportOk = false;
	// on teste la validité du document courant
	if (m_pATPDocCreuset->IsValid())
	{
		// on modifie le curseur
		CWaitCursor Wait;

		// Test regroupement data export
		JATPDocExport DocExport(*m_pATPDocCreuset);

		// on teste les exceptions
		try
		{
			// Création de l'objet export échelle
			CATPExportEchelle *pExportEchelle = new CATPExportEchelle();

			// on exporte le document échelle
			ExportOk = DocExport.AcceptEch(pExportEchelle);
			delete pExportEchelle;
		}

		catch (...)
		{
			// on affiche le message
			::AfxMessageBox(_T("La création de l'export échelle a échoué."), MB_ICONERROR | MB_OK);
			ExportOk = false;
		}

		if (ExportOk == true)
		{
			file.Write(DocExport.m_StrExport.GetBuffer(), DocExport.m_StrExport.GetLength() );
			//::AfxMessageBox(_T("L'export de l'echelle s'est effectué avec succès"), MB_ICONINFORMATION | MB_OK);
		}

	}	
	return ExportOk;
}

bool CATPBaseFrame::ExportResultPlan(CFile & file)
{
	// Flag validité de l'export
	bool ExportOk = false;
	// on teste la validité du document courant
	if (m_pATPDocCreuset->IsValid())
	{
		// on modifie le curseur
		CWaitCursor Wait;


		m_pATPDocCreuset->UpdateAllViews(UPDATE_FORCEVENTIL);
		m_pATPDocCreuset->UpdateAllViews(UPDATE_FORCECONTRIB);

		// Test regroupement data export
		JATPDocExport DocExport(*m_pATPDocCreuset);

		// on teste les exceptions
		try
		{
			// Création de l'objet export test plans
			CATPExpCsvTestPlan *pExportCsvTestPlan = new CATPExpCsvTestPlan();

			// on exporte le document test plans
			ExportOk = DocExport.Accept(pExportCsvTestPlan); 
			delete pExportCsvTestPlan;

		}

		catch (...)
		{
			// on affiche le message
			ExportOk = false;
		}

		m_pATPDocCreuset->UpdateAllViews(UPDATE_VENTIL);
		if (ExportOk == true)
		{
			file.Write(DocExport.m_StrExport.GetBuffer(), DocExport.m_StrExport.GetLength());
			//::AfxMessageBox(_T("L'export des hypothèses s'est effectué avec succès"), MB_ICONINFORMATION | MB_OK);
		}
		else
		{
			::AfxMessageBox(_T("La création du fichier d'export a échoué."), MB_ICONERROR | MB_OK);
		}

		
	}
	return ExportOk;
}

bool CATPBaseFrame::ExportResultCalend(CFile & file)
{
	// Flag validité de l'export
	bool ExportOk = false;
	// on teste la validité du document courant
	if (m_pATPDocCreuset->IsValid())
	{
		// on modifie le curseur
		CWaitCursor Wait;

		// Test regroupement data export
		JATPDocExport DocExport(*m_pATPDocCreuset);


		// on teste les exceptions
		try
		{
			// Crétion de l'objet export plans calendaires
			CATPExpCsvPlanCalend *pExportCsvPlanCalend = new CATPExpCsvPlanCalend();


			// on exporte le document plans calendaires
			ExportOk = DocExport.Accept(pExportCsvPlanCalend);
			delete pExportCsvPlanCalend;
		}

		catch (...)
		{
			// on affiche le message
			::AfxMessageBox(_T("La création du fichier d'export a échoué."), MB_ICONERROR | MB_OK);
			ExportOk = false;
		}

		if (ExportOk == true)
		{
			file.Write(DocExport.m_StrExport.GetBuffer(), DocExport.m_StrExport.GetLength());
			//::AfxMessageBox(_T("L'export du plan s'est effectué avec succès"), MB_ICONINFORMATION | MB_OK);
		}
		
	}
	return ExportOk;
}

void CATPBaseFrame::OnComputeRealTime()
{
	if (m_pATPDocCreuset->IsValid())
	{
		m_pATPDocCreuset->m_bCalculsDiff = false;
		m_pATPDocCreuset->UpdateAllViews(UPDATE_ETUDE);
	}
}
void CATPBaseFrame::OnUpdateComputeRealTime(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_pATPDocCreuset->IsValid());
	if (m_pATPDocCreuset->IsValid())
		pCmdUI->SetRadio(!m_pATPDocCreuset->m_bCalculsDiff);

}
void CATPBaseFrame::OnComputeDiff()
{
	if (m_pATPDocCreuset->IsValid())
	{
		m_pATPDocCreuset->m_bCalculsDiff = true;
		m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_RESULTBAR);

	}
}
void CATPBaseFrame::OnUpdateComputeDiff(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_pATPDocCreuset->IsValid());
	if (m_pATPDocCreuset->IsValid())
		pCmdUI->SetRadio(m_pATPDocCreuset->m_bCalculsDiff);
}
///////////////////////////////////////////////////////////////////////////////
void CATPBaseFrame::OnCopier()
{
	if (m_pATPDocCreuset->IsValid())
		m_pATPDocCreuset->CopierPlanClipBoard();
}
void CATPBaseFrame::OnUpdateCopier(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_pATPDocCreuset->IsValid());
}

void CATPBaseFrame::OnColler()
{
	if (m_pATPDocCreuset->IsValid())
		m_pATPDocCreuset->CollerPlanClipBoard();
}
void CATPBaseFrame::OnCollerAdd1()
{
	if (m_pATPDocCreuset->IsValid())
		m_pATPDocCreuset->AjouterPlanClipBoard();
}

void CATPBaseFrame::OnCollerAdd2()
{
	if (m_pATPDocCreuset->IsValid())
		m_pATPDocCreuset->AjouterSuccessifPlanClipBoard();
}

void CATPBaseFrame::OnUpdateColler(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_pATPDocCreuset->m_bSomethingToPaste);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Identication de la couleur en cours
/*  Rappel des couleurs dispos
		BOTTLE_GREEN		=	0,
		EMERAUDE			=	1,
		BLEU_PASTEL			=	2,
		OCEAN				=   3,
		LILAS				=   4,
		SAHARA				=	5,
		TERRE_SIENNE		=   6,
		RUBIS				=   7,
		GREY_MOUSE			=	8,
		ROSE				=	9,
		ROSE_KAKI			=  10,
		ROSE_GRIS			=  11,
*/
void CATPBaseFrame::OnUpdateCrPalette1(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(m_CATPColorsApp) == CATPColors::BOTTLE_GREEN) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CATPBaseFrame::OnUpdateCrPalette2(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(m_CATPColorsApp) == CATPColors::EMERAUDE) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CATPBaseFrame::OnUpdateCrPalette3(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(m_CATPColorsApp) == CATPColors::BLEU_PASTEL) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CATPBaseFrame::OnUpdateCrPalette4(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(m_CATPColorsApp) == CATPColors::OCEAN) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CATPBaseFrame::OnUpdateCrPalette5(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(m_CATPColorsApp) == CATPColors::LILAS) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CATPBaseFrame::OnUpdateCrPalette6(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(m_CATPColorsApp) == CATPColors::SAHARA) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CATPBaseFrame::OnUpdateCrPalette7(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(m_CATPColorsApp) == CATPColors::TERRE_SIENNE) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CATPBaseFrame::OnUpdateCrPalette8(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(m_CATPColorsApp) == CATPColors::RUBIS) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CATPBaseFrame::OnUpdateCrPalette9(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(m_CATPColorsApp) == CATPColors::GREY_MOUSE) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CATPBaseFrame::OnUpdateCrPalette10(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(m_CATPColorsApp) == CATPColors::ROSE_KAKI) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CATPBaseFrame::OnUpdateCrPalette11(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(m_CATPColorsApp) == CATPColors::ROSE_KAKI) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CATPBaseFrame::OnUpdateCrPalette12(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(m_CATPColorsApp) == CATPColors::ROSE_GRIS) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CATPBaseFrame::OnUpdateCrPalette13(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(m_CATPColorsApp) == CATPColors::SABLE) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CATPBaseFrame::OnUpdateCrPalette14(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(m_CATPColorsApp) == CATPColors::CAFE) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CATPBaseFrame::OnUpdateCrPalette15(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(m_CATPColorsApp) == CATPColors::NEWBLUE) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Création de cibles pondérées
void CATPBaseFrame::OnCrCreerCiblepond()
{
	// La boite cibles pondérées
	CDlgCiblePondere DlgCiblePond;

	// Passe le document
	DlgCiblePond.SetData(m_pATPDocCreuset);

	// On se met en mode création
	DlgCiblePond.SetMode(CDlgCiblePondere::MODE_CREATION);

	// Affichage de la fenêtre offre
	if(DlgCiblePond.DoModal()== IDOK)
	{
		// Attention si mode cible pondérée actif et plus de cible pondéré
		// CItemsConstruits * pItemsConstruits = CItemsConstruits::GetInstance();

		// if (pItemsConstruits->GetNbItemsPonderes() <= 0 && m_pATPDocCreuset->m_CiblesPondsActives)
		if (CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetNbItemsPonderes() <= 0 && m_pATPDocCreuset->m_CiblesPondsActives)
		{
			// Force la non utilisation des cibles pondérés
			m_pATPDocCreuset->m_CiblesPondsActives = false;

			// Indique modification cible
			m_pATPDocCreuset->SetModifiedFlag();

			// Mise à jour de l'offre
			m_pATPDocCreuset->UpdateAllViews(UPDATE_OFFRE);
		}

		// Mise à jour de l'offre
		// m_pATPDocCreuset->UpdateAllViews(UPDATE_OFFRE);
	}
}

void CATPBaseFrame::OnUpdateCrCreerCiblepond(CCmdUI *pCmdUI)
{
	// Seulement si un plan creuset en cours
	pCmdUI->Enable((m_pATPDocCreuset->IsValid()) ? TRUE : FALSE);
}

/////////////////////////////////////////////////////////////////////////////////////
// Activation ou pas des cibles pondérées existantes
void CATPBaseFrame::OnCrUtilCiblepond()
{
	// Positionne le flag pour indiquer que l'on utilise ou pas les cibles pondérées
	m_pATPDocCreuset->m_CiblesPondsActives =  !m_pATPDocCreuset->m_CiblesPondsActives;

	// Indique modification cible
	m_pATPDocCreuset->SetModifiedFlag();

	// Mise à jour liste des cibles utilisables et des résultats
	m_pATPDocCreuset->UpdateAllViews(UPDATE_OFFRE);
}

void CATPBaseFrame::OnUpdateCrUtilCiblepond(CCmdUI *pCmdUI)
{
	// Seulement si un plan creuset en cours, et qu'il existe au mpois 1 cible pondérée
	// CItemsConstruits * pItemsConstruits = CItemsConstruits::GetInstance();

	// Acif si au moins 1 cible pondérée existante
	// pCmdUI->Enable((m_pATPDocCreuset->IsValid() && pItemsConstruits->GetNbItemsPonderes() > 0) ? TRUE : FALSE);
	pCmdUI->Enable((m_pATPDocCreuset->IsValid() && CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetNbItemsPonderes() > 0) ? TRUE : FALSE);

	// Check ou uncheck l'option
	// if (pItemsConstruits->GetNbItemsPonderes() <= 0 && m_pATPDocCreuset->m_CiblesPondsActives)
	if (CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetNbItemsPonderes() <= 0 && m_pATPDocCreuset->m_CiblesPondsActives)
	{
		m_pATPDocCreuset->m_CiblesPondsActives = false;

		// Indique modification cible
		m_pATPDocCreuset->SetModifiedFlag();

		// Mise à jour liste des cibles utilisables et des résultats
		m_pATPDocCreuset->UpdateAllViews(UPDATE_OFFRE);
		
	}

	m_pATPDocCreuset->m_CiblesPondsActives ? pCmdUI->SetCheck(TRUE) : pCmdUI->SetCheck(FALSE);
}
