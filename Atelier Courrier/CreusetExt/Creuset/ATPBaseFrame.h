#if !defined(AFX_ATPBaseFrame_H__69DBE77F_27CB_4150_A428_A4CDBB286F46__INCLUDED_)
#define AFX_ATPBaseFrame_H__69DBE77F_27CB_4150_A428_A4CDBB286F46__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les définitions nécessaires
#include "JATPDocCreuset.h"
#include "JFCFrame.h"
#include "DialogFiltres.h"
#include "DlgChoixMoteur.h"
#include "DlgCourbeReponse.h"

#include "DialogContrib.h"

/////////////////////////////////////////////////////////////////////////////
// CATPBaseFrame

class CATPBaseFrame : public JFCFrame
{
public:
	// le constructeur
	CATPBaseFrame(JATPDocCreuset *pDoc);

	// le destructeur
	virtual ~CATPBaseFrame();

	virtual JBool CanClose();

	// les fonctions pour charger/sauver les documents
	virtual BOOL OnLoadEtude(JATPDocCreuset & ATPDocCreuset, CString path, bool bDialog = false);

protected:
	
	virtual BOOL OnSaveDocument(JATPDocCreuset & ATPDocCreuset, JBool bRename);

	// la fonction pour mettre à jour les vues
	virtual VOID UpdateAllViews(const JFCUpdate & Message);

	//{{AFX_VIRTUAL(CATPBaseFrame)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CATPBaseFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnUpdateEtudeNouvelle(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEtudeOuvrir(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEtudeEnregistrer(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEtudeEnregistrerSous(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEtudeFermer(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePerimetrePeriode(CCmdUI* pCmdUI);
	afx_msg void OnUpdateValidHypothese(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMasterPlanToHypothese(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOptionsMoteurAbaque(CCmdUI *pCmdUI);
	afx_msg void OnUpdateOptionsMoteurAbaqueEtal(CCmdUI *pCmdUI);
	afx_msg void OnUpdateOptionsMoteurProba(CCmdUI *pCmdUI);
	afx_msg void OnUpdateOptionsRepriseEnMain(CCmdUI *pCmdUI);
	afx_msg void OnUpdateOptionsNoRepriseEnMain(CCmdUI *pCmdUI);
	afx_msg void OnUpdateOptionsLNMMagazine(CCmdUI *pCmdUI);
	afx_msg void OnUpdateOptionsLDPQuotidien(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCopieHypothese(CCmdUI *pCmdUI);
	afx_msg void OnUpdateImporterPlateau(CCmdUI *pCmdUI);

	afx_msg void OnEtudeNouvelle();
	afx_msg void OnEtudeOuvrir();
	afx_msg void OnEtudeEnregistrer();
	afx_msg void OnEtudeEnregistrerSous();
	afx_msg void OnEtudeFermer();
	afx_msg void OnPerimetrePeriode();
	afx_msg void OnVersion();
	afx_msg void OnClose();
	afx_msg void OnOffreSources();
	afx_msg void OnOffreSupports();
	afx_msg void OnOffreCibles();
	afx_msg void OnOffreFormat();
	afx_msg void OnValidHypothese();
	afx_msg void OnMasterPlanToHypothese();
	afx_msg void OnOptionsMoteurAbaque();
	afx_msg void OnOptionsMoteurAbaqueEtal();
	afx_msg void OnOptionsMoteurProba();
	afx_msg void OnOptionsRepriseEnMain();
	afx_msg void OnOptionsNoRepriseEnMain();
	afx_msg void OnOptionsLNMMagazine();
	afx_msg void OnOptionsLDPQuotidien();
	afx_msg void OnVersionCharger();
	afx_msg void OnImporterPlateau();

	afx_msg void OnPalette1();
	afx_msg void OnPalette2();
	afx_msg void OnPalette3();
	afx_msg void OnPalette4();
	afx_msg void OnPalette5();
	afx_msg void OnPalette6();
	afx_msg void OnPalette7();
	afx_msg void OnPalette8();
	afx_msg void OnPalette9();
	afx_msg void OnPalette10();
	afx_msg void OnPalette11();
	afx_msg void OnPalette12();
	afx_msg void OnPalette13();
	afx_msg void OnPalette14();
	afx_msg void OnPalette15();

	afx_msg void OnUpdateCrPalette1(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCrPalette2(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCrPalette3(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCrPalette4(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCrPalette5(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCrPalette6(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCrPalette7(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCrPalette8(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCrPalette9(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCrPalette10(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCrPalette11(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCrPalette12(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCrPalette13(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCrPalette14(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCrPalette15(CCmdUI *pCmdUI);

	afx_msg void OnChromie();
	afx_msg void OnUpdateChromie(CCmdUI *pCmdUI);
	afx_msg void OnStatutInsert();
	afx_msg void OnUpdateStatutInsert(CCmdUI *pCmdUI);
	afx_msg void OnStatutMoteur();
	afx_msg void OnUpdateStatutMoteur(CCmdUI *pCmdUI);

	afx_msg void OnSuppinsHypact();
	afx_msg void OnSuppinsTouteshyps();
	afx_msg void OnUpdateSuppinsTouteshyps(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSuppinsHypact(CCmdUI *pCmdUI);
	afx_msg void OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu);
	afx_msg void OnCreerVersionPlanref();
	afx_msg void OnUpdateCreerVersionPlanref(CCmdUI *pCmdUI);
	afx_msg void OnChargerVersionPlanref();
	afx_msg void OnUpdateChargerVersionPlanref(CCmdUI *pCmdUI);
	afx_msg void OnPeriodeEtude();
	afx_msg void OnUpdatePeriodeEtude(CCmdUI *pCmdUI);
	afx_msg void OnRenameHypo();
	afx_msg void OnUpdateRenameHypo(CCmdUI *pCmdUI);
	afx_msg void OnUpdateOffreFormats(CCmdUI *pCmdUI);
	afx_msg void OnUpdateOffreSources(CCmdUI *pCmdUI);
	afx_msg void OnUpdateOffreSupports(CCmdUI *pCmdUI);
	afx_msg void OnUpdateOffreCibles(CCmdUI *pCmdUI);
	afx_msg void OnOptionsLDP30j();
	afx_msg void OnUpdateOptionsLDP30j(CCmdUI *pCmdUI);
	afx_msg void OnOffreItemsVentilation();
	afx_msg void OnUpdateOffreItemsVentilation(CCmdUI *pCmdUI);
	afx_msg void OnSysColorChange();
	afx_msg void OnAucunedecimale();
	afx_msg void On1decimale();
	afx_msg void On2decimale();
	afx_msg void OnUpdateNbddecimale(CCmdUI *pCmdUI);

	afx_msg void OnExportResultPlan();
	afx_msg void OnClipboardResultPlan();
	afx_msg void OnPrintResultPlan();
	afx_msg void OnExportResultCalend();
	afx_msg void OnClipboardResultCalend();
	afx_msg void OnPrintResultCalend();
	afx_msg void OnUpdateExportResult(CCmdUI *pCmdUI);
	afx_msg void OnExportEchelle();
	afx_msg void OnClipboardEchelle();
	afx_msg void OnPrintEchelle();
	afx_msg void OnUpdateExportEchelle(CCmdUI *pCmdUI);
	afx_msg void OnExportTout();
	afx_msg void OnClipboardTout();
	afx_msg void OnPrintTout();
	afx_msg void OnUpdateExportTout(CCmdUI *pCmdUI);

	afx_msg void OnUpdateExcelEchelle(CCmdUI *pCmdUI);
	afx_msg void OnExcelEchelle();
	afx_msg void OnExcelResultPlan();
	afx_msg void OnExcelResultCalend();
	afx_msg void OnUpdateExcelTout(CCmdUI *pCmdUI);
	afx_msg void OnExcelTout();

	afx_msg void OnComputeRealTime();
	afx_msg void OnUpdateComputeRealTime(CCmdUI *pCmdUI);
	afx_msg void OnComputeDiff();
	afx_msg void OnUpdateComputeDiff(CCmdUI *pCmdUI);

	afx_msg void OnCopier();
	afx_msg void OnUpdateCopier(CCmdUI *pCmdUI);

	afx_msg void OnColler();
	afx_msg void OnUpdateColler(CCmdUI *pCmdUI);
	afx_msg void OnCollerAdd1();
	afx_msg void OnCollerAdd2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CDialogFiltres m_dlgFiltres;
	CDlgCourbeReponse m_dlgCourbeReponse;
	CDlgChoixMoteur m_dlgChoixMoteur;

	JATPDocCreuset * m_pATPDocCreuset; // le document

	CDialogContrib     m_dlgContribution;
	
	virtual bool ExportResultPlan(CFile & file);

	virtual bool ExportEchelle(CFile & file);
	bool ExportResultCalend(CFile & file);

	// Color Scheme fenêtre dialogue ChoixSupport
	CATPColors::APPCOLOR			m_CATPColorsApp;

public:
	afx_msg void OnCrOptFiltreAudience();
	afx_msg void OnUpdateCrOptFiltreAudience(CCmdUI *pCmdUI);
	afx_msg void OnCrOptCourbeReponse();
	afx_msg void OnUpdateCrOptCourbeReponse(CCmdUI *pCmdUI);
	
	afx_msg void OnCrCreerCiblepond();
	afx_msg void OnUpdateCrCreerCiblepond(CCmdUI *pCmdUI);
	afx_msg void OnCrUtilCiblepond();
	afx_msg void OnUpdateCrUtilCiblepond(CCmdUI *pCmdUI);


	//bool OpenOffreDialog(	LPCTSTR lpszDlgTitle, CATPColors::APPCOLOR appColor,
	//						JATPOffre & atpOffre, int eOngletCourant,
	//						JDate & dateDeb, JDate & dateFin,
	//						bool & bDateChanged, bool & bOffreModified,
	//						const bool bDateDebReadOnly = true);

};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPBaseFrame_H__69DBE77F_27CB_4150_A428_A4CDBB286F46__INCLUDED_)
