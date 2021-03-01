#pragma once

// on inclut les définitions nécessaires
#include "JFCUpdate.h"

/////////////////////////////////////////////////////////////////////////////
// JFCFrame

class AFX_EXT_CLASS JFCFrame : public CFrameWnd
{
public:
	// le constructeur
	JFCFrame();

	// la fonction pour créer le frame
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, const RECT & rect = rectDefault, CWnd* pParentWnd = NULL, LPCTSTR lpszMenuName = NULL, DWORD dwExStyle = 0);

	// la fonction pour charger le frame
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL);

	// le destructeur
	virtual ~JFCFrame();

protected:
	// la fonction pour mettre à jour les vues
	virtual VOID UpdateAllViews(const JFCUpdate & Message);

protected:
	// la fonction d'enregistrement de la classe
	static BOOL RegisterClass();

protected:
	//{{AFX_VIRTUAL(JFCFrame)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(JFCFrame)
	afx_msg void OnPaint();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	friend class JFCDocument; // pour appeler la fonction UpdateAllViews

public:
	BOOL m_bAutoDelete; // l'indicateur de libération
};

