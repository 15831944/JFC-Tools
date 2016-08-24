// MyCug97.h : interface of the MyCug class
//
/////////////////////////////////////////////////////////////////////////////
#include "ugctrl.h"
#include ".\GridAdditions\UGEdit2.h"
#include ".\GridAdditions\UGDLType2.h"

class MyCug:public CUGCtrl
{
public:
	MyCug();
	~MyCug();

protected:
	CUGEdit2 m_newEditControl;
	CUGDropListType2 m_newDLType;
	int m_newDLIndex;

private:
	BOOL m_bDroplistStarted;
	int m_lostFocusFlag;

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	//}}AFX_VIRTUAL

	
	//{{AFX_MSG(MyCug)
	// NOTE - the ClassWizard will add and remove member functions here.
	//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	//***** Over-ridable Notify Functions *****
	virtual void OnSetup();
	//GetCellIndirect notification
	virtual void OnGetCell(int col,long row,CUGCell *cell);
	virtual void OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed);
	//cell type notifications
	virtual int OnCellTypeNotify(long ID,int col,long row,long msg,long param);
	virtual COLORREF OnGetDefBackColor(int section);
	virtual void OnSetFocus(int section);
	virtual void OnKillFocus(int section);
};
