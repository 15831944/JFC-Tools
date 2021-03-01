// on inclut les d�finitions n�cessaires
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

	// on se place en d�but de liste
	POSITION Position = m_FrameList.GetHeadPosition();

	// on boucle sur les �l�ments de la liste
	while (Position != NULL)
	{
		// on r�cup�re le frame courant et on passe au suivant
		JFCFrame* pCurrentFrame = (JFCFrame*)m_FrameList.GetNext(Position);

		// on teste si le frame existe d�j�
		if (pCurrentFrame == pFrame) return (FALSE);
	}

	// on ajoute le frame � la liste
	m_FrameList.AddTail(pFrame);

	// on met � jour la fen�tre principale
	m_pMainWnd = (CWnd*)pFrame;

	// on quitte
	return (TRUE);
}

void JFCApp::RemFrame(JFCFrame* pFrame)
{
	// on teste le frame
	if (pFrame != NULL)
	{
		// on se place en d�but de liste
		POSITION Position = m_FrameList.GetHeadPosition();

		// on boucle sur les �l�ments de la liste
		while (Position != NULL)
		{
			// on m�morise la position courante
			POSITION Current = Position;

			// on r�cup�re le frame courant et on passe au suivant
			JFCFrame* pCurrentFrame = (JFCFrame*)m_FrameList.GetNext(Position);

			// on teste si on doit supprimer le frame
			if (pCurrentFrame == pFrame) m_FrameList.RemoveAt(Current);
		}

		// on teste si on doit mettre � jour la fen�tre principale
		if (m_pMainWnd == (CWnd*)pFrame)
		{
			// on teste si la liste contient des frames
			if (FALSE == m_FrameList.IsEmpty())
			{
				// on r�cup�re le premier �l�ment
				JFCFrame* pFirstFrame = (JFCFrame*)m_FrameList.GetHead();

				// on modifie la fen�tre principale
				m_pMainWnd = (CWnd*)pFirstFrame;
			}
		}
	}
}

//////////////////////////
// JFCApp message handlers
