// MoveManager.h: interface for the CMoveManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOVEMANAGER_H__A65999A1_593E_11D2_AD1C_0080C708A895__INCLUDED_)
#define AFX_MOVEMANAGER_H__A65999A1_593E_11D2_AD1C_0080C708A895__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#define MM_SIZE_X (1)
#define MM_SIZE_Y (2)
#define MM_MOVE_X (4)
#define MM_MOVE_Y (8)
#define MM_HRESIZE_RIGHT	(16)
#define MM_HRESIZE_LEFT		(32)
#define MM_HRESIZE_POSITION		(64)

class CMoveManager  
{
public:
	void SetMasterRect(CRect,CRect);
	void AddWindow(CWnd*,short style);
	void Resize(short cx, short cy);
	void HResize(short dx,int tailleAbsolue);
	void Move(short x, short y);

	CMoveManager();
	virtual ~CMoveManager();
	CRect m_RectOrigine;
	CRect m_RectMouvement;
private:
	bool m_fInit;
	struct SWndPos
	{
		CWnd* pCWnd;
		CRect Rect;
		short Style;
	};
	CArray<SWndPos,SWndPos> m_ChildWnd;
	int m_oldcx;
	int m_oldcy;

};

#endif // !defined(AFX_MOVEMANAGER_H__A65999A1_593E_11D2_AD1C_0080C708A895__INCLUDED_)
