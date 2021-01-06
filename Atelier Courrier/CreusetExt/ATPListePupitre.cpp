#include "StdAfx.h"
#include "ATPListePupitre.h"

/////////////////////////////////////////////////////
// Constructeur
CATPListePupitre::CATPListePupitre(void)
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

/////////////////////////////////////////////////////
// la fonction pour définir le document
JVoid CATPListePupitre::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on mémorise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on met à jour le composant
	// this->OnUpdate();
}

/////////////////////////////////////////////////////
// la fonction pour mettre à jour le composant

JVoid CATPListePupitre::Update()
{
	// on met à jour le composant
	// this->OnUpdate();
}

/////////////////////////////////////////////////////
// le destructeur
CATPListePupitre::~CATPListePupitre()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPListePupitre, JFCSelListBox)
	//{{AFX_MSG_MAP(CATPListePupitre)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

