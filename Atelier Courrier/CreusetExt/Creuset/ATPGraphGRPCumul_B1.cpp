// on inclut les définitions nécessaires
#include "stdafx.h"
#include "ATPGraphGRPCumul_B1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPGraphGRPCumul_B1::CATPGraphGRPCumul_B1()
{
}

/////////////////
// le destructeur

CATPGraphGRPCumul_B1::~CATPGraphGRPCumul_B1()
{
	// on ne fait rien
}

void CATPGraphGRPCumul_B1::CalcValues(const JITPDATHypoFormat & DATHypoFormat, int idxHypo, int i, JFlt64 &A2, JFlt64 &A3, JFlt64 &A4)
{
	// on récupère les couvertures à 1+ de la veille
	JFlt64 N3 = 0.0; 
	if (i > 0)
		N3 = DATHypoFormat.GetDATHypoFormatActif().m_NbContactsJour.Item(i - 1);
	JFlt64 N4 = DATHypoFormat.GetDATHypoFormatActif().m_NbContactsJour.Item(i);

	m_MapArrayGRP.MoveTo(idxHypo);
	if(m_MapArrayGRP.IsValid())
	{
		if (i  > 0)   A2 = m_MapArrayGRP.GetItem().Item(i-1);
		if (N3  > 0) A3 = m_MapArrayGRP.GetItem().Item(i-1);
		if (N4  > 0) A4 = m_MapArrayGRP.GetItem().Item(i);
	}
}

JVoid CATPGraphGRPCumul_B1::Update()
{
	// on initialise la dimension
	long cx = 0;

	// on vérifie la validité du document
	if (m_pATPDocCreuset != 0)
	{
		// on récupère la période d'action
		JDate DateD = m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD();
		JDate DateF = m_pATPDocCreuset->m_KEYPeriodeAction.GetDateF();

		// on vérifie la validité de la période d'action
		if (DateD.IsValid() && DateF.IsValid())
		{
			// on initialise le maximum
			JFlt64 Max = 0.0;

			// on calcule le nombre d'éléments
			cx = (DateF - DateD) + 1;

			// on boucle sur toutes les hypothèses
			for (m_pATPDocCreuset->m_ITRHypotheses.MoveFirst(); m_pATPDocCreuset->m_ITRHypotheses.IsValid(); m_pATPDocCreuset->m_ITRHypotheses.MoveNext())
			{
				// on récupère l'hypothèse courante
				const JITPDATHypoFormat & DATHypoFormat = m_pATPDocCreuset->m_ITRHypotheses.GetItem();
				const JITPDATHypothese & DATHypothese = DATHypoFormat.GetDATHypoFormatActif();

				// on récupère le nombre de jours
				JInt32 NbJours = DATHypothese.m_NbContactsJour.GetCount();
				JArrayJFlt64 ArrayGRP;
				ArrayGRP.SetCount(NbJours);
				// on récupère l'effectif
				JFlt64 Effectif = DATHypoFormat.m_Effectif;
				// on teste l'effectif
				if (Effectif > 0.0)
				{
					// on boucle sur tous les jours
					for (JInt32 IdxJour = 0; IdxJour < NbJours; IdxJour += 1)
					{
						// on récupère la couverture cumulée à 1+ du jour courant
						JFlt64 XGRPMax = 100.0 * DATHypothese.m_NbContactsJour.Item(IdxJour) / Effectif;
						if(IdxJour>0)
							ArrayGRP.Item(IdxJour) = ArrayGRP.Item(IdxJour-1) + XGRPMax;
						else
							ArrayGRP.Item(IdxJour) = XGRPMax;
					}
				}
				m_MapArrayGRP.MoveTo(m_pATPDocCreuset->m_ITRHypotheses.GetKey());
				if(m_MapArrayGRP.IsValid())
					m_MapArrayGRP.GetItem().Swap(ArrayGRP);
				else
					m_MapArrayGRP.Add(m_pATPDocCreuset->m_ITRHypotheses.GetKey()).Swap(ArrayGRP);
				
			}

		}

	}
	// on met à jour le nombre d'histogrammes
	this->SetHorzItemRange(0, cx);
}

