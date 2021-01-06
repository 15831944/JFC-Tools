#pragma once

// on inclut les définitions nécessaires
#include "JFCFrame.h"

/////////////////////////////////////////////////////////////////////////////
// JFCApp

class AFX_EXT_CLASS JFCApp : public CWinApp
{
protected:
	// le constructeur
	JFCApp();           

	// le destructeur
	virtual ~JFCApp();

public:
	// les fonctions pour manipuler les frames
	virtual BOOL AddFrame(JFCFrame* pFrame);
	virtual void RemFrame(JFCFrame* pFrame);

protected:
	// AFX_DYNAMIC_CREATION
	DECLARE_DYNCREATE(JFCApp)

protected:
	//{{AFX_VIRTUAL(JFCApp)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(JFCApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CPtrList m_FrameList; // la liste des frames
};

