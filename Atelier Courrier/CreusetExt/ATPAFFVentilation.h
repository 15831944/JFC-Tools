#pragma once

#include "iitem.h"
#include "JITPDATHypothese.h"

class JATPDocCreuset;

class JATPAFFVentilation
{
public:
	JATPAFFVentilation(void);
	~JATPAFFVentilation(void);

	void SetVisible(JBool bVisible);
	JBool IsVisible();

	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);


	JInt32 GetNbItem() const;

	const JChar* GetLblItem(int rangItem) const;

	JFlt64 GetNbContacts(int idxHypo, int idFormat, int rangItem) const;
	JFlt64 GetAudience000(int idxHypo, int idFormat, int rangItem) const;
	JFlt64 GetGRP(int idxHypo, int idFormat, int rangItem) const;
	JFlt64 GetCouv(int idxHypo, int idFormat, int rangItem) const;
	JFlt64 GetRepet(int idxHypo, int idFormat, int rangItem) const;
	JFlt64 GetIndGRP(int idxHypo, int idFormat, int rangItem) const;
	JFlt64 GetIndCouv(int idxHypo, int idFormat, int rangItem) const;

	CString GetValue(int idxHypo, int idFormat, int ligne) const;
	CString GetLabel(int ligne) const;

	JVoid Update(const JFCUpdate & Message);

	bool ExportVentil(CString & exportStr);

	CString ExportVentil(int nHypo);

	void ShowNbContacts(bool bShow);
	void ShowAudience000(bool bShow);
	void ShowGRP(bool bShow);
	void ShowIndGRP(bool bShow);
	void ShowCouv(bool bShow);
	void ShowIndCouv(bool bShow);
	void ShowRepet(bool bShow);

	bool CanNbContacts();
	bool CanAudience000();
	bool CanGRP();
	bool CanIndGRP();
	bool CanCouv();
	bool CanIndCouv();
	bool CanRepet();

	int NbValue() const;

private:
	void TestItemsVentil();

	JATPDocCreuset* m_pATPDocCreuset; // le document
	bool m_bVisible;

	friend class CDialogVentilation;

	JList<JInt32> m_listIDQST;
	JList<JInt32> m_listIDItem;
	JList<JInt32> m_listIDPal;
	JList<IItem*> m_lstItemVentil;

	int m_lastBtnSel;

	typedef JArray<JITPDATHypoFormat>  TBLhypos;
	//Le tableau de table d'hypothèse
	JArray<TBLhypos>  m_TBLhypos;
	
	bool m_bUpd;
	bool m_bUpdAll;

	bool m_bShowNbContacts;
	bool m_bShowAudience000;
	bool m_bShowGRP;
	bool m_bShowIndGRP;
	bool m_bShowCouv;
	bool m_bShowIndCouv;
	bool m_bShowRepet;

};


