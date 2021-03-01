// on inclut les définitions nécessaires
#include "stdafx.h"
#include "JFCApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

JFCApp::JFCApp()
{
	// on ne fait rien
}

/////////////////
// le destructeur

JFCApp::~JFCApp()
{
	// on ne fait rien
}

IMPLEMENT_DYNCREATE(JFCApp, CWinApp)

BEGIN_MESSAGE_MAP(JFCApp, CWinApp)
	//{{AFX_MSG_MAP(JFCApp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////
// les fonctions pour manipuler les frames

BOOL JFCApp::AddFrame(JFCFrame* pFrame)
{
	// on teste le frame
	if (pFrame == NULL) return (FALSE);

	// on se place en début de liste
	POSITION Position = m_FrameList.GetHeadPosition();

	// on boucle sur les éléments de la liste
	while (Position != NULL)
	{
		// on récupère le frame courant et on passe au suivant
		JFCFrame* pCurrentFrame = (JFCFrame*)m_FrameList.GetNext(Position);

		// on teste si le frame existe déjà
		if (pCurrentFrame == pFrame) return (FALSE);
	}

	// on ajoute le frame à la liste
	m_FrameList.AddTail(pFrame);

	// on met à jour la fenêtre principale
	m_pMainWnd = (CWnd*)pFrame;

	// on quitte
	return (TRUE);
}

void JFCApp::RemFrame(JFCFrame* pFrame)
{
	// on teste le frame
	if (pFrame != NULL)
	{
		// on se place en début de liste
		POSITION Position = m_FrameList.GetHeadPosition();

		// on boucle sur les éléments de la liste
		while (Position != NULL)
		{
			// on mémorise la position courante
			POSITION Current = Position;

			// on récupère le frame courant et on passe au suivant
			JFCFrame* pCurrentFrame = (JFCFrame*)m_FrameList.GetNext(Position);

			// on teste si on doit supprimer le frame
			if (pCurrentFrame == pFrame) m_FrameList.RemoveAt(Current);
		}

		// on teste si on doit mettre à jour la fenêtre principale
		if (m_pMainWnd == (CWnd*)pFrame)
		{
			// on teste si la liste contient des frames
			if (FALSE == m_FrameList.IsEmpty())
			{
				// on récupère le premier élément
				JFCFrame* pFirstFrame = (JFCFrame*)m_FrameList.GetHead();

				// on modifie la fenêtre principale
				m_pMainWnd = (CWnd*)pFirstFrame;
			}
		}
	}
}

//////////////////////////
// JFCApp message handlers
