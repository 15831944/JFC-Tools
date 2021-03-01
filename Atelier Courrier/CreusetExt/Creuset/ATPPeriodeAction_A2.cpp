// on inclut les définitions nécesaires
#include "stdafx.h"
#include "ATPPeriodeAction_A2.h"
#include "ATPPeriodeAction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPPeriodeAction_A2::CATPPeriodeAction_A2()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
	m_bHypo = FALSE;
}

///////////////////////////////////////
// la fonction pour définir le document

JVoid CATPPeriodeAction_A2::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on mémorise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on met à jour le composant
	this->OnUpdate();
}

JVoid CATPPeriodeAction_A2::SetHypo(JBool bHypo)
{
	m_bHypo = bHypo;
}


//////////////////////////////////////////////
// la fonction pour mettre à jour le composant

JVoid CATPPeriodeAction_A2::Update()
{
	// on met à jour le composant
	this->OnUpdate();
}

/////////////////
// le destructeur

CATPPeriodeAction_A2::~CATPPeriodeAction_A2()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPPeriodeAction_A2, JFCWave)
	//{{AFX_MSG_MAP(CATPPeriodeAction_A2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////
// la fonction pour appliquer les mises à jour

JVoid CATPPeriodeAction_A2::OnUpdate()
{
	// on initialise la période
	JDate DateD;
	JDate DateF;

	// on initialise le nombre d'éléments
	LONG cy = 0;

	// on vérifie la validité du document
	if (m_pATPDocCreuset != 0)
	{
		// on récupère la période d'action
		DateD = m_pATPDocCreuset->m_KEYEtude.GetDateD();
		DateF = m_pATPDocCreuset->m_KEYEtude.GetDateF();

		// on récupère le nombre d'éléments
		if (DateD.IsValid() && DateF.IsValid()) cy = 1;
	}

	// on met à jour le composant
	this->SetRange(DateD, DateF, TRUE);
	this->SetCount(cy, TRUE);
}

/////////////////////////////////////
// CATPPeriodeAction_A2 message handlers

void CATPPeriodeAction_A2::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFCWave::PreSubclassWindow();
}

LONG CATPPeriodeAction_A2::OnGetItemState(LONG indice, JDate date)
{

	LONG Etat = 0;

	if (m_pATPDocCreuset != 0)
	{
		// on recherche l'interrogation
		if (m_bHypo)// on récupère l'hypothèse courante
			m_pATPDocCreuset->m_ITRHypotheses.MoveTo(m_pATPDocCreuset->m_IdxHypotheseCourant);
		else
			m_pATPDocCreuset->m_ITRHypotheses.MoveTo(0);

		// On calcul le jour exact
		JInt32	Jour;
		Jour = date - m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD();  

		// on teste si on a trouvé l'hypothèse
		if (Jour >= 0 && m_pATPDocCreuset->m_ITRHypotheses.IsValid())
		{
			// on récupère l'hypothèse
			JITPDATHypoFormat & DATHypothese = m_pATPDocCreuset->m_ITRHypotheses.GetItem();

			if (Jour < DATHypothese.m_JoursInsertExist.GetSize())
			{
				if(DATHypothese.m_JoursInsertExist.GetAt(Jour))
				{

					if((Jour!=0 && DATHypothese.m_JoursInsertExist.GetAt(Jour-1) == false)
					&& (Jour < DATHypothese.m_JoursInsertExist.GetSize()	&& DATHypothese.m_JoursInsertExist.GetAt(Jour+1) == false))
						Etat |= JFCWAVE_ELEM_QTD;
					else	if(Jour!=0 && DATHypothese.m_JoursInsertExist.GetAt(Jour-1) == false)
								Etat |= JFCWAVE_ELEM_START;
							else	if (Jour < DATHypothese.m_JoursInsertExist.GetSize()	&& DATHypothese.m_JoursInsertExist.GetAt(Jour+1) == false)
										Etat |= JFCWAVE_ELEM_STOP;
									else
										Etat |= JFCWAVE_ELEM_ELEM;

					//si un insert existe ce jour
					//if(DATHypothese.m_JoursInsertExist.GetAt(Jour))
					//	Etat |= JFCWAVE_STATE_PLEIN;
					//else
					//	Etat |= JFCWAVE_STATE_BORD;

				
				}
			}
		}
	}


	JDate first, last;
	m_parent->m_A1.GetSelRange(first, last);
	if ((first.IsValid() && last.IsValid())
	&& (first <= date && date <= last))
		Etat |= JFCWAVE_FOND_SEL;

	return Etat;

}

