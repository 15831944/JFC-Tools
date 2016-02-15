#if !defined(AFX_FORMULAIRETAUXFO_H__16E83CE1_B101_11D5_8A63_0010B5865AAB__INCLUDED_)
#define AFX_FORMULAIRETAUXFO_H__16E83CE1_B101_11D5_8A63_0010B5865AAB__INCLUDED_


#include "Noeud.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FormulaireTauxFO.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormulaireTauxFO dialog

class CFormulaireTauxFO : public CDialog
{
// Construction
public:
	CFormulaireTauxFO(CWnd* pParent = NULL);   // standard constructor
	CString m_CodeNomencl;
	CString m_LibelleNomencl;
	HTREEITEM m_HItemCur;
	CNoeud *m_pNoeudCur;

	// Flag pour indiqué le 1er élément station de base
	long m_InxBaseStation; 

// Dialog Data
	//{{AFX_DATA(CFormulaireTauxFO)
	enum { IDD = IDD_DIALOG_TAUXFO };
	CButton	m_SuppTauxSimul;
	CEdit	m_EditSimulTauxCM;
	CEdit	m_EditSimulTaux5;
	CEdit	m_EditSimulTaux4;
	CEdit	m_EditSimulTaux3;
	CEdit	m_EditSimulTaux2;
	CEdit	m_EditSimulTaux1;
	CStatic	m_TauxSimul;
	CStatic	m_SepTauxSimul;
	CStatic	m_InfoTFO;
	CButton	m_SelRadio;
	int m_SelRadioValue;
	CEdit	m_EditTauxCM;
	CEdit	m_EditTaux5;
	CEdit	m_EditTaux4;
	CEdit	m_EditTaux3;
	CEdit	m_EditTaux2;
	CEdit	m_EditTaux1;
	CButton	m_SuppTaux;
	CObj_Gray	m_Cadre9;
	CButton	m_SauverTaux;
	CListBox	m_ListStation;
	BOOL	m_HistoriqueTaux;
	CString	m_ExNomDate;
	CString	m_NvNomDate;
	CString	m_ExTaux1;
	CString	m_ExTaux2;
	CString	m_ExTaux3;
	CString	m_ExTaux4;
	CString	m_ExTaux5;
	CString	m_ExTauxCM;
	CString	m_ExTauxFO;
	CString	m_Taux1;
	CString	m_Taux2;
	CString	m_Taux3;
	CString	m_Taux4;
	CString	m_Taux5;
	CString	m_TauxCM;
	CString	m_TauxFO;
	CString	m_SimulTaux1;
	CString	m_SimulTaux2;
	CString	m_SimulTaux3;
	CString	m_SimulTaux4;
	CString	m_SimulTaux5;
	CString	m_SimulTauxCM;
	CString	m_SimulTauxFO;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormulaireTauxFO)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Chargement des chaines régionales et locales
	bool ChargeChaine();

	// Affichage des taux de la station (on passe chaine station CodeReg + CodeStation / format rr-sss)
	void AfficheTauxStation(CString StrStation);

	// Initialisation des taux
	void InitTaux();
	void InitSimulTaux();

	// Gestion des taux
	void LireTaux(CString CodeNomencl,int CodeRegion,int CodeStation,CStringArray &TabTaux,int &NbTaux,CString &DateTaux,CString &UtilTaux,CStringArray &TabExTaux,int &NbExTaux,CString &ExDateTaux,CString &ExUtilTaux);
	void EcrireTaux(CString LibNomencl,CString CodeNomencl,int CodeRegion,int CodeStation,CStringArray &TabTaux);
	void DetruireTaux(CString LibNomencl,CString CodeNomencl,int CodeRegion,int CodeStation);
	bool NouvelleValeurTaux();
	void MajTauxFO(bool NvTaux);
	bool ValeurTauxOk(CString &StrTaux);
	bool IsNumeric(CString Txt);

	// Gestion des taux simulés
	void LireSimulTaux(CString CodeNomencl,int CodeRegion,int CodeStation,CStringArray &TabTaux,int &NbTaux,CString &DateTaux,CString &UtilTaux);
	void EcrireSimulTaux(CString LibNomencl,CString CodeNomencl,int CodeRegion,int CodeStation,CStringArray &TabTaux);
	void DetruireSimulTaux(CString LibNomencl,CString CodeNomencl,int CodeRegion,int CodeStation);


// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFormulaireTauxFO)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	virtual void OnCancel();
	afx_msg void OnSauverTaux();
	afx_msg void OnSuppTaux();
	afx_msg void OnSelchangeListStations();
	afx_msg void OnChangeTaux1();
	afx_msg void OnChangeTaux2();
	afx_msg void OnChangeTaux3();
	afx_msg void OnChangeTaux4();
	afx_msg void OnChangeTaux5();
	afx_msg void OnChangeTauxcm();
	afx_msg void OnSeltv();
	afx_msg void OnSelradio();
	afx_msg void OnChangeSimtaux1();
	afx_msg void OnChangeSimtaux2();
	afx_msg void OnChangeSimtaux3();
	afx_msg void OnChangeSimtaux4();
	afx_msg void OnChangeSimtaux5();
	afx_msg void OnChangeSimtauxcm();
	afx_msg void OnSuppTauxSimul();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMULAIRETAUXFO_H__16E83CE1_B101_11D5_8A63_0010B5865AAB__INCLUDED_)
