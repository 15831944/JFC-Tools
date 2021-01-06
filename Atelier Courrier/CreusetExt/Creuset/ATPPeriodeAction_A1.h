#if !defined(AFX_ATPPERIODEACTION_A1_H__DB58FE05_099E_469D_973A_2A81B4C4B848__INCLUDED_)
#define AFX_ATPPERIODEACTION_A1_H__DB58FE05_099E_469D_973A_2A81B4C4B848__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CATPPeriodeAction_A1

class CATPPeriodeAction_A1 : public JFCCalendar
{
public:
	// le constructeur
	CATPPeriodeAction_A1();

	// le destructeur
	virtual ~CATPPeriodeAction_A1();

protected:
	//{{AFX_VIRTUAL(CATPPeriodeAction_A1)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CATPPeriodeAction_A1)
	afx_msg LRESULT OnSelChanging(WPARAM, LPARAM);
	afx_msg LRESULT OnSelChanged(WPARAM, LPARAM);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	// les classes amies
	friend class CATPPeriodeAction;
//	friend class CATPPeriodeAction_A2;

private:
	CATPPeriodeAction * m_parent; //la fenetre parente
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPPERIODEACTION_A1_H__DB58FE05_099E_469D_973A_2A81B4C4B848__INCLUDED_)
