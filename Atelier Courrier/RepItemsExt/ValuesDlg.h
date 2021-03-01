#pragma once

#include "resource.h"
#include "JFC_MultiHideListBox.h"
#include "IBNumValue.h"
#include "IBClass.h"
#include "BtnST.h"
#include "label.h"

#define WM_USER_VALUE_DLG_DIMSEL   (WM_USER + 110)
#define WM_USER_VALUE_DLG_UPDATED  (WM_USER + 112)
#define WM_USER_VALUE_DLG_QUANTILE (WM_USER + 114)

/**
 * \ingroup RepItems
 *
 *
 * \par requirements
 * win98 or later\n
 * win2k or later\n
 * MFC\n
 *
 * \version 1.0
 * first version
 *
 * \date 07-09-2003
 *
 * \author eddie
 *
 * \par license
 * This code is the property of JFC Informatique & Media\n
 * Unauthorized copying or use is strictly forbiden.
 * 
 * \todo 
 *
 * \bug 
 *
 */
class AFX_EXT_CLASS CValueSel
{
public:

	CValueSel()
	{
		m_Label		= "";
		m_Select	= 0;
		m_Operator	= IBloc::Nop;
		m_Value		= 0;
	}

	~CValueSel(){};
public: 
	void			SetID(JInt32 ID)			{ m_ID = ID; }
	void			SetString(CString &String)	{ m_Label = String; }
	JLabel			GetLabel()					{ return m_Label; }
	void			SetSelected(int Select)		{ m_Select = Select; }
	int				GetSelected()				{ return m_Select; }
	void			SetOperator(IBloc::OPERATOR Op) { m_Operator = Op; }
	IBloc::OPERATOR	GetOperator()				{ return m_Operator; }
	bool			HasOperator()				{ return m_Operator!=IBloc::Nop; }
	void			SetValue(JFlt32 Val)		{ m_Value = Val; }
	JFlt32			GetValue()					{ return m_Value; }
	JInt32			GetID()						{ return m_ID; }

protected:
	
	JLabel			m_Label;
	int				m_Select;
	IBloc::OPERATOR	m_Operator;
	JFlt32			m_Value;
	JInt32			m_ID;
};

/**
 * \ingroup RepItems
 *
 *
 * \par requirements
 * win98 or later\n
 * win2k or later\n
 * MFC\n
 *
 * \version 1.0
 * first version
 *
 * \date 07-10-2003
 *
 * \author eddie
 *
 * \par license
 * This code is the property of JFC Informatique & Media\n
 * Unauthorized copying or use is strictly forbiden.
 * 
 * \todo 
 *
 * \bug 
 *
 */
class AFX_EXT_CLASS CValuesDlg : public CDialog
{
/// Construction
public:
	CValuesDlg(CWnd* pParent = NULL);   ///< standard constructor
	virtual ~CValuesDlg();   ///< standard destructor
	/// Resets the colors
	void SetColors();
	/// Set the quanti values
	void SetQuantiValues(JVector<JFlt32> &Values,JInt32 Divider);
	/// Set the quanti values
	void SetQuantiValues(JVector<JFlt32> &Values,CIBNumValue *pNumValue,JInt32 Divider);
	/// Set the quanti values
	void SetQuantiValues(JVector<JFlt32> &Values,CIBClass *pClass,JInt32 Divider);
	/// Set the quanti values
	void SetSelectedValues(CIBNumValue *pNumValue);
	/// Set the current selected class values
	void SetSelectedValues(CIBClass *pClass);
	/// Get the selected values
	CIBNumValue * GetNumValues();
	/// Get the selected values
	CIBClass * GetClass();
	/// Get the palette values
	JList<JFlt32> & GetPalValues(JList<JFlt32> & NumValues);
	/// Fill the list box with the values
	void FillListBox();
	/// Removes the values in the values map
	void RemoveValues();
	/// Sets up the values map with the current values
	void SetupValuesMap(JVector<JFlt32> &Values);
	/// Sets the string values
	void SetStrings();
	void SetString(CValueSel *pVal);
	/// Creates the values vector
	void CreateValuesVector();
	void UpdateSelections(bool ForceGTE = false);
	void ResetAll();
	void ResetBefore(int Pos);
	void ResetAfter(int Pos);
	void SelectAfter(int Pos, int Select);
	void SelectBefore(int Pos, int Select);
	void SetClassSelection();
	void SetItemMode();
	void SetPaletteMode(JBool bPalValues, const JChar* pLabValues);
	void SetQuantileMode();
	bool HasPalValue() { return (m_bPalValues); }
	bool IsNumValue() { return (m_bNumValues); }
	bool IsClass() { return (!m_bNumValues); }
	void UpdateSelected();
	void UpdateSize();
	void SetBlockFen(bool Bloque);
	void SetFlightWindow(bool Flight=true);

	CATPColors::APPCOLOR		m_CATPColorsApp;

/// Dialog Data
	//{{AFX_DATA(CValuesDlg)
	enum { IDD = IDD_RI_DLG_VALUES };
	//}}AFX_DATA

/// Overrides
	/// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CValuesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    /// DDX/DDV support
	//}}AFX_VIRTUAL

/// Implementation
protected:
	JVoid								UpdateTitle();
	JInt32								m_TypeMode;

/// Implementation
protected:
	JInt32								m_Divider;
	JInt32								m_NbValues;
	JBool								m_bPalValues;
	JBool								m_bNumValues;
	JLabel								m_LabValues;
	JMap<JFlt32,CValueSel *>			m_ValuesMap;
	JVector<CValueSel *>				m_Values;
	JFC_MultiHideListBox<CValueSel *>	m_ListValues;
	CLabel								m_Label;
	CButtonST                           m_Button;
	CButtonST                           m_ButtonQTLE;
	CButtonST                           m_ButtonRAZ;
	CButtonST                           m_ButtonSup1;
	CLabel								m_Box;

	// Flag pour détecter le 1er lancement de la fenêtre value (sinon l'activation de l'entete ne pourra pas
	// être initialisé à vrai au départ
	JBool								m_FenDejaVisu;

	JBool								m_LockFen;

	// Indique si c'est une fenêtre volante (comme avant) ou bien locké sur l'espace dimension 1 (cas mono-item "Valeurs Numériques")
	JBool								m_FlightWindow;

	/// Generated message map functions
	//{{AFX_MSG(CValuesDlg)
	afx_msg void OnButtonValues();
	afx_msg void OnButtonQTLEValues();
	afx_msg void OnButtonRAZValues();
	afx_msg void OnButtonSup1Values();
	afx_msg void OnSelchangeListValues();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos );

};
