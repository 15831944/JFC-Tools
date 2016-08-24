// MyCug97.h : interface of the MyCug class
//
/////////////////////////////////////////////////////////////////////////////
#include "ugctrl.h"

class MyCug:public CUGCtrl
{
public:
	MyCug();
	~MyCug();

protected:
	CFont m_defFont;

	virtual void OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed);
	virtual COLORREF OnGetDefBackColor(int section);

private:
	BOOL m_bPrintInProdress;
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	//}}AFX_VIRTUAL

	
	//{{AFX_MSG(MyCug)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	int PrintHeader ( CDC *pDC, int style = 0, int alignment = 0, int page = 0, CString string = "" );
	int PrintRangeEx ( int stCol, long stRow, int edCol, long edRow );
	//***** Over-ridable Notify Functions *****
	virtual void OnSetup();

	virtual void OnGetCell(int col,long row,CUGCell *cell);
};
