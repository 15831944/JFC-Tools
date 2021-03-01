// on inclut les définitions nécesaires
#include "stdafx.h"
#include "ATPCreusetPlan_C3.h"
#include "ATPCreusetPlan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPCreusetPlan_C3::CATPCreusetPlan_C3()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

///////////////////////////////////////
// la fonction pour définir le document

JVoid CATPCreusetPlan_C3::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on mémorise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on met à jour le composant
	this->OnUpdate();
}

JVoid CATPCreusetPlan_C3::SetParent(CATPCreusetPlan * pATPCreusetPlan)
{
	m_pATPCreusetPlan = pATPCreusetPlan; 
}

//////////////////////////////////////////////
// la fonction pour mettre à jour le composant

JVoid CATPCreusetPlan_C3::Update()
{
	// on met à jour le composant
	this->OnUpdate();
}

/////////////////
// le destructeur

CATPCreusetPlan_C3::~CATPCreusetPlan_C3()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPCreusetPlan_C3, JFCWave)
	//{{AFX_MSG_MAP(CATPCreusetPlan_C3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////
// la fonction pour appliquer les mises à jour

JVoid CATPCreusetPlan_C3::OnUpdate()
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
		// DateDebut = m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD();
		// DateFin = m_pATPDocCreuset->m_KEYPeriodeAction.GetDateF();

		// MODIF ALAIN
		if(m_pATPDocCreuset->m_IdxHypotheseCourant == 0)
		{
			DateD = m_pATPDocCreuset->m_KEYEtude.GetDateD();
			DateF = m_pATPDocCreuset->m_KEYEtude.GetDateF();
		}
		else
		{				
			DateD = m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD();
			DateF = m_pATPDocCreuset->m_KEYPeriodeAction.GetDateF();
		}

		// on récupère le nombre d'éléments
		if (DateD.IsValid() && DateF.IsValid()) cy = 1;
	}

	// on met à jour le composant
	this->SetRange(DateD, DateF, TRUE);
	this->SetCount(cy, TRUE);
	Invalidate();
}

/////////////////////////////////////
// CATPCreusetPlan_C3 message handlers

void CATPCreusetPlan_C3::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFCWave::PreSubclassWindow();
}

LONG CATPCreusetPlan_C3::OnGetItemState(LONG indice, JDate date)
{
	LONG Etat = 0;

	if (m_pATPDocCreuset != 0 && m_pATPDocCreuset->IsValid())
	{
		// on recherche l'interrogation
		m_pATPDocCreuset->m_ITRHypotheses.MoveTo(m_pATPDocCreuset->m_IdxHypotheseCourant);

		// On calcul le jour exact
		JInt32	Jour;
		Jour = date - m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD();  

		// on teste si on a trouvé l'hypothèse
		if (Jour >= 0 && m_pATPDocCreuset->m_ITRHypotheses.IsValid())
		{
			// on récupère l'hypothèse
			const JITPDATHypoFormat & DATHypoFormat = m_pATPDocCreuset->m_ITRHypotheses.GetItem();
			//const JITPDATHypothese & DATHypothese = DATHypoFormat.GetDATHypoFormatActif();

			if (Jour < DATHypoFormat.m_JoursActifs.GetSize())
			{
				if(DATHypoFormat.m_JoursActifs.GetAt(Jour))
				{

					if((Jour!=0 && DATHypoFormat.m_JoursActifs.GetAt(Jour-1) == false)
					&& (Jour < DATHypoFormat.m_JoursActifs.GetSize()-1	&& DATHypoFormat.m_JoursActifs.GetAt(Jour+1) == false))
						Etat |= JFCWAVE_ELEM_QTD;
					else	if(Jour!=0 && DATHypoFormat.m_JoursActifs.GetAt(Jour-1) == false)
								Etat |= JFCWAVE_ELEM_START;
							else	if (Jour < DATHypoFormat.m_JoursActifs.GetSize()-1	&& DATHypoFormat.m_JoursActifs.GetAt(Jour+1) == false)
										Etat |= JFCWAVE_ELEM_STOP;
									else
										Etat |= JFCWAVE_ELEM_ELEM;

					//si un insert existe ce jour
					if(!DATHypoFormat.m_JoursInsertExist.GetAt(Jour))
						Etat += 1;
					//if(DATHypoFormat.m_JoursInsertExist.GetAt(Jour))
					//	Etat |= JFCWAVE_STATE_PLEIN;
					//else
					//	Etat |= JFCWAVE_STATE_BORD;

				
				}
			}
		}
	}


	JDate first, last;
	m_pATPCreusetPlan->m_C1.GetSelRange(first, last);
	if ((first.IsValid() && last.IsValid())
	&& (first <= date && date <= last))
		Etat |= JFCWAVE_FOND_SEL;

	return Etat;
}

