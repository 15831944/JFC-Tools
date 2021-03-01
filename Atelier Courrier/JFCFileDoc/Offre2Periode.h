#pragma once

#include "Offre2OngletBase.h"
#include "resource.h"
#include "afxdtctl.h"
#include "afxwin.h"

#include "Label.h"

class COffre2MainDialog;

// Boîte de dialogue COffre2Periode

class COffre2Periode : public COffre2OngletBase
{
		DECLARE_DYNAMIC(COffre2Periode)

	public:

		COffre2Periode(COffre2MainDialog * pDialogOffre);   // constructeur standard
		virtual ~COffre2Periode();

		// les fonctions pour manipuler la date de début
		JVoid SetDateD(JDate DateD);
		JDate GetNewDateD() const;

		// les fonctions pour manipuler la date de fin
		JVoid SetDateF(JDate DateF);
		JDate GetNewDateF() const;

		void SetDateDReadOnly(bool bReadOnly = true);		
		
	// Données de boîte de dialogue
		enum { IDD = IDD_FD_DLG_OFFRE2_PERIODE};

		// retourne true si l'état de la sélection est correct
		virtual bool IsSelectionOk() const;

	protected:

		virtual BOOL OnInitDialog();
		afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		afx_msg void OnChangeSpinS(NMHDR* pNMHDR, LRESULT* pResult);
		afx_msg void OnChangeSpinJ(NMHDR* pNMHDR, LRESULT* pResult);
		afx_msg void OnStartCalendarSelChange(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnEndCalendarSelChange	 (NMHDR *pNMHDR, LRESULT *pResult);

		virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

		DECLARE_MESSAGE_MAP()

		// la fonction pour calculer la durée maximum
		JInt32 OnGetMaxDuree(JDate DateD) const;

		// la fonction pour mettre à jour la boîte
		VOID OnUpdateSpin();
		VOID OnUpdateDateFin();

	private:

		CSpinButtonCtrl m_SpinS;
		CSpinButtonCtrl m_SpinJ;

		CMonthCalCtrl m_StartCalendar;
		CMonthCalCtrl m_EndCalendar;

		JInt32			m_Duree;		// la durée en jours

		JDate			m_DateND;		// la nouvelle date de début
		JDate			m_DateNF;		// la nouvelle date de fin
		JDate			m_DateD;		// la date de début
		JDate			m_DateF;		// la date de fin

		bool			m_bDateDReadOnly;

		CLabel			m_ttlTitre1;
		CLabel			m_ttlTitre2;
		CLabel			m_ttlTitre3;
		CLabel			m_ttlTitre4;
};
