#include "StdAfx.h"
#include "ATPGraphCrbCum.h"
#include "DlgCumulativite.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////////////////////////////
// Constructeur
CATPGraphCrbCum::CATPGraphCrbCum()
{
	// Init Résultats Courbe
	m_MapResultCrb.Reset();

	// Init Résultats Etalonnage Courbe
	m_MapResultEtalCrb.Reset();

	// Init Tableau des couleurs courbes
	m_TabIndexColor.Reset();

	// Init tableau début de chaque courbe (par défaut on démarre au point 0)
	m_TDepAbscisse.Reset();
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Destructeur
CATPGraphCrbCum::~CATPGraphCrbCum()
{
	// A FAIRE
}

///////////////////////////////////////
// la fonction pour définir le document

JVoid CATPGraphCrbCum::SetDocument(JATPDocChoixSupport* pDocChoixSupport)
{
	// on mémorise le document
	m_pATPDocChoixSupport = pDocChoixSupport;

}

//////////////////////////////////////////////////////////////////////////////////////////////
// Mise à jour résultats graphe
JVoid CATPGraphCrbCum::Update()
{
	// Les résultats à tracer
	m_pATPDocChoixSupport->PrepareMatriceTraceCum();

	// Récupère couleur de chaque courbe
	RecupColorCourbe();

	// Récupére libellé de chaque courbe (A FAIRE)
	// m_TabLibCourbe.SetCount(NbCourbeGraph); 

	// Consolidation résultats par couleur de courbe
	ConsolidResultCrb();

	// Mise à jour du point de départ
	m_TDepAbscisse.Reset();
	m_TDepAbscisse.SetCount(m_MapResultEtalCrb.GetCount());
	for (JUnt32 InxCrb = 0; InxCrb < m_MapResultEtalCrb.GetCount(); InxCrb++)
		m_TDepAbscisse.Item(InxCrb) = 0;


	m_pATPDocChoixSupport->m_MapIdGrandCumSel.MoveFirst();
	if (m_pATPDocChoixSupport->m_MapIdGrandCumSel.IsValid())
	{
		// A FAIRE / définir les constantes grandeurs
		if (m_pATPDocChoixSupport->m_MapIdGrandCumSel.GetItem() == 7)
		{
			// Boucle sur toutes les courbes
			JUnt32 InxCourbe = 0;
			for (m_MapResultEtalCrb.MoveFirst(); m_MapResultEtalCrb.IsValid(); m_MapResultEtalCrb.MoveNext())
			{
				// Récup tableau des étalonnages correspondant
				JArray <bool> &TResultEtal = m_MapResultEtalCrb.GetItem();

				m_TDepAbscisse.Item(InxCourbe) = 0;
				for (JUnt32 k = 0; k < TResultEtal.GetCount(); k++)
				{
					if (TResultEtal.Item(k))
					{
						// Init départ courbe
						m_TDepAbscisse.Item(InxCourbe) = k;
						break;
					}
				}

				// Passe à la courbe suivante
				InxCourbe++;
			}
		}	
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////
// Récupération nombre d'éléments axe horizontal
JInt32 CATPGraphCrbCum::GetXElmtCount() const
{
	// Calcul maximum graduation
	return MaxJoursSuppGraphe();
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Récupération nombre total de courbes
JInt32 CATPGraphCrbCum::GetCourbeCount() const
{
	return NbCourbeGraph();
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Récupération couleur de la courbe
JInt32 CATPGraphCrbCum::GetCourbeColor(JInt32 CourbeIndex) const
{
	return m_TabIndexColor.Item(CourbeIndex);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Récupération couleur via index
COLORREF CATPGraphCrbCum::GetColor(JInt32 IdColor) const
{
	return CATPColors::GetColorGrph(IdColor-1);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// la fonction pour récupérer la validité des éléments
JBool CATPGraphCrbCum::IsItemValid(JInt32 XIndex, JInt32 CourbeIndex) const
{
	// Attention aux bornes horizontales et verticales
	if (XIndex <= MaxJoursSuppGraphe() && CourbeIndex <= m_TabIndexColor.GetCount()) 
		return true;
	else
		return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// la fonction pour récupérer la valeur des éléments
JFlt64 CATPGraphCrbCum::GetValueItem(JInt32 XIndex, JInt32 CourbeIndex, bool &ItemMarque) const
{
	// On se positionne sur les résultats courbe
	m_MapResultCrb.MoveTo(CourbeIndex);

	// Voir si ce point fait partie des périodicités
	m_MapResultEtalCrb.MoveTo(CourbeIndex);
	if (m_MapResultEtalCrb.IsValid())
	{
		JUnt32 NbItem = m_MapResultEtalCrb.GetItem().GetCount(); 

		if (XIndex < m_MapResultEtalCrb.GetItem().GetCount())
			ItemMarque = m_MapResultEtalCrb.GetItem().Item(XIndex);
	}

	// Si résultats existent
	if (m_MapResultCrb.IsValid())
	{
		if (XIndex < m_MapResultCrb.GetItem().GetCount()) 
		{
			const JFlt64 ValueItem = m_MapResultCrb.GetItem().Item(XIndex);
			return ValueItem; 
		}
		else
			return -1,0;
	}	
	else
		// Ici résultats non existants
		return -1.0;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// la fonction pour récupérer le libellé de la courbe
const JString CATPGraphCrbCum::GetLabelItem(JInt32 CourbeIndex) const
{
	// Info n° de la courbe ou libellé support
	CString LibCourbe;

	LibCourbe.Format("%s %d","Courbe",m_TabIndexColor.Item(CourbeIndex)); 
	JString Libelle = LibCourbe;
	return Libelle;
}


/////////////////////////////////////////////////////////////////////////////////////////////
// la fonction pour récupérer le type de résultat horizontal
const JString CATPGraphCrbCum::GetLabelResultHorz() const
{
	// Forcément en abscisse nb jours
	return "Jours";
}

/////////////////////////////////////////////////////////////////////////////////////////////
// la fonction pour récupérer le type de résultat vertical
const JString CATPGraphCrbCum::GetLabelResultVert() const
{
	// On se positionne sur l'élément type résultat sélectionné
	m_pATPDocChoixSupport->m_MapIdGrandCumSel.MoveFirst();
	if (m_pATPDocChoixSupport->m_MapIdGrandCumSel.IsValid()) 
	{
		// Récupére l'indice de la sélection
		JInt32 IndexGrandeur = m_pATPDocChoixSupport->m_MapIdGrandCumSel.GetItem();

		// Renvoi le libellé trouvé
		const JString ret(m_pATPDocChoixSupport->GetGrandeur(m_pATPDocChoixSupport->ModuleCum, IndexGrandeur));
		return ret;
	}		
	else
		// pas de grandeur définie
		return "";
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Calcul le maximum de jours pour le dessin de la courbe (axe horizontal)
JInt32 CATPGraphCrbCum::MaxJoursSuppGraphe() const
{
	if (m_pATPDocChoixSupport != NULL)
	{
		// Voir si au moins 1 support pour la courbe
		m_pATPDocChoixSupport->m_MapIdColorSupports.MoveFirst();
		if (m_pATPDocChoixSupport->m_MapIdColorSupports.IsValid())
		{
			// Recup le terrain en cours
			JUnt32 IdTerrain;
			m_pATPDocChoixSupport->GetMapIdTerrainsAudienceSel()->MoveFirst();
			if (m_pATPDocChoixSupport->GetMapIdTerrainsAudienceSel()->IsValid())
			{
				// au moins 1 terrain sélectionné
				IdTerrain = m_pATPDocChoixSupport->GetMapIdTerrainsAudienceSel()->GetKey(); 
			}
			else
				return 0;

			// Recup la cible en cours
			JUnt32 IdCible;
			m_pATPDocChoixSupport->GetMapIdCiblesAudienceSel()->MoveFirst();
			if (m_pATPDocChoixSupport->GetMapIdCiblesAudienceSel()->IsValid())
			{
				// au moins 1 cible sélectionnée
				IdCible = m_pATPDocChoixSupport->GetMapIdCiblesAudienceSel()->GetKey();  
			}
			else
				return 0;

			// Renvoi maximum jours à prendre ne compte
			return m_pATPDocChoixSupport->MaxJourCourbeCum(IdTerrain,IdCible);
		}
	}

	// Aucun support sélectionné
	return 0;
}	

/////////////////////////////////////////////////////////////////////////////////////////////
// Calcul le nombre de courbes à dessiner
JInt32 CATPGraphCrbCum::NbCourbeGraph() const
{
	// Init nombre de courbes
	JInt32 NbCourbe = 0;

	// Boucle sur les supports sélectionnés
	if (m_pATPDocChoixSupport->m_ModeColorGraphe == 0)
	{
		//////////////////////////////  Mode supports couleurs indépendantes //////////////////////////
		//
		// Boucle sur les supports sélectionnés
		for (m_pATPDocChoixSupport->m_MapIdColorSupports.MoveFirst(); 
			 m_pATPDocChoixSupport->m_MapIdColorSupports.IsValid(); 
			 m_pATPDocChoixSupport->m_MapIdColorSupports.MoveNext())
		{
			// Récupère Id Support
			JUnt32 IdSupport = m_pATPDocChoixSupport->m_MapIdColorSupports.GetKey(); 

			// Attention aux supports non visibles
			JBool EtatVisible = m_pATPDocChoixSupport->GetSupportVisible(IdSupport);

			// Récupère Id Couleur Associée
			JUnt32 IdColor = m_pATPDocChoixSupport->m_MapIdColorSupports.GetItem();

			// 1 courbe par support, incrémente le nb courbe
			if (IdColor > 0 && EtatVisible)
				NbCourbe++;
		}
	}

	else
	{
		//////////////////////////////  Mode supports couleurs regroupées //////////////////////////
		//
		// Boucle sur les couleurs existantes
		for (JUnt32 IdColor = 1; IdColor <= 12; IdColor++)
		{
			// Test déjà si cette couleur a été sélectionnée au moins pour 1 support
			bool ColorSelect = false;
			for (m_pATPDocChoixSupport->m_MapIdColorSupports.MoveFirst();
				 m_pATPDocChoixSupport->m_MapIdColorSupports.IsValid();
				 m_pATPDocChoixSupport->m_MapIdColorSupports.MoveNext())
			{
				// Récupère Id Couleur Associée
				JUnt32 IdColorSupp = m_pATPDocChoixSupport->m_MapIdColorSupports.GetItem();
				if (IdColorSupp == IdColor)
				{
					ColorSelect = true;
					break;
				}
			}

			// Si couleur sélectionnée sur au moins 1 support, incrémente nb couleurs valides
			if (ColorSelect)
				NbCourbe++;
		}
	}

	// Retourne le nombre de courbes à dessiner
	return NbCourbe;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// la fonction pour récupérer le libellé support des courbes
const JString CATPGraphCrbCum::GetLibCourbeGraph(JUnt32 InxCourbe) const
{
	// Boucle sur les supports sélectionnés
	if (m_pATPDocChoixSupport->m_ModeColorGraphe == 0)
	{
		// Init Index Courbe
		JUnt32 InxCrbCur = -1;

		//////////////////////////////  Mode supports couleurs indépendantes //////////////////////////
		//
		// Boucle sur les supports sélectionnés
		for (m_pATPDocChoixSupport->m_MapIdColorSupports.MoveFirst(); 
			 m_pATPDocChoixSupport->m_MapIdColorSupports.IsValid(); 
			 m_pATPDocChoixSupport->m_MapIdColorSupports.MoveNext())
		{
			// Récupère Id Support
			JUnt32 IdSupport = m_pATPDocChoixSupport->m_MapIdColorSupports.GetKey(); 

			// Récupère Id Couleur Associée
			JUnt32 IdColor = m_pATPDocChoixSupport->m_MapIdColorSupports.GetItem();

			// Attention aux supports non visibles
			JBool EtatVisible = m_pATPDocChoixSupport->GetSupportVisible(IdSupport);

			// 1 courbe par support, incrémente le nb courbe
			if (IdColor > 0 && EtatVisible)
			{
				InxCrbCur++;
			}

			if (InxCrbCur == InxCourbe)
			{
				// Récupère objet regroupement
				CRegroupements * pRegroupements = CRegroupements::GetInstance();

				// Element support
				IElem* pSupport = pRegroupements->GetIElemByID(IdSupport);

				// on récupére le libellé support
				CString LibSupport = "";
				if (pSupport)
					// Affichage libellé support
					LibSupport = pSupport->GetLabel().AsJCharPtr();

				const JString ret (LibSupport);
				return ret;
			}
		}
	}
	else
	{
		// On indique simplement n° de la couleur courbe
		CString LibCourbe;

		LibCourbe.Format("%s %d","Courbe",m_TabIndexColor.Item(InxCourbe)); 
		JString Libelle = LibCourbe;
		return Libelle;
	}
	return "erreur";
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Consolidation résultats par courbe
JVoid CATPGraphCrbCum::ConsolidResultCrb()
{
	// Init Résultats Courbe
	m_MapResultCrb.Reset();

	// Init Résultats Etalonnage Courbe
	m_MapResultEtalCrb.Reset();

	// Maximum jour pour la courbe
	JUnt32 MaxJourCourbe = MaxJoursSuppGraphe();						

	// Index courbe
	JUnt32 InxCourbe = 0;

	// Boucle sur les supports sélectionnés
	if (m_pATPDocChoixSupport->m_ModeColorGraphe == 0)
	{

		//////////////////////////////  Mode supports couleurs indépendantes //////////////////////////
		//
		// Boucle sur les supports sélectionnés
		for (m_pATPDocChoixSupport->m_MapIdColorSupports.MoveFirst(); 
			 m_pATPDocChoixSupport->m_MapIdColorSupports.IsValid(); 
			 m_pATPDocChoixSupport->m_MapIdColorSupports.MoveNext())
		{
			// Récupère Id Support
			JUnt32 IdSupport = m_pATPDocChoixSupport->m_MapIdColorSupports.GetKey(); 

			// Attention aux supports non visibles
			JBool EtatVisible = m_pATPDocChoixSupport->GetSupportVisible(IdSupport);

			// Récupère Id Couleur Associée
			JUnt32 IdColor = m_pATPDocChoixSupport->m_MapIdColorSupports.GetItem();

			// 1 courbe par support, incrémente le nb courbe
			if (IdColor > 0 && EtatVisible)
			{
				// Tableau résultat pour cette couleur
				JArray <JFlt64> TResult;
				JArray <bool>   TResultEtal;

				TResult.SetCount(MaxJourCourbe);
				for (JInt32 jj=0; jj < TResult.GetCount(); jj++)
				{
					TResult.Item(jj) = 0.0; 
				}

				TResultEtal.SetCount(MaxJourCourbe);
				for (JInt32 jj=0; jj < TResultEtal.GetCount(); jj++)
				{
					TResultEtal.Item(jj) = false; 
				}

				// Récup tableau résultat
				m_pATPDocChoixSupport->m_MapResultCrbCum.MoveTo(IdSupport); 
				m_pATPDocChoixSupport->m_MapResultEtalCrbCum.MoveTo(IdSupport); 
				if (m_pATPDocChoixSupport->m_MapResultCrbCum.IsValid() && m_pATPDocChoixSupport->m_MapResultEtalCrbCum.IsValid())
				{
					JArray <JFlt64> &TResultSupp		=  m_pATPDocChoixSupport->m_MapResultCrbCum.GetItem(); 
					JArray <bool>   &TResultEtalSupp	=  m_pATPDocChoixSupport->m_MapResultEtalCrbCum.GetItem(); 

					// Récup pour tous les indices horizontaux
					for (JInt32 iRow = 0; iRow < TResultSupp.GetCount(); iRow++)
					{
						// on récupère les résultats
						JFlt64 ValBase = TResult.Item(iRow);

						JFlt64 Valeur		= TResultSupp.Item(iRow);
						TResult.Item(iRow) += TResultSupp.Item(iRow); 

						JFlt64 ValRes = TResult.Item(iRow);
						JFlt64 ValRes2 = ValRes; 

						// Récupére si étalonnage ou pas
						TResultEtal.Item(iRow) = TResultEtalSupp.Item(iRow);
					}

					// Sauve résultat pour la courbe
					m_MapResultCrb.Add(InxCourbe).Swap(TResult); 

					// Sauve résultat étalonnage pour la courbe
					m_MapResultEtalCrb.Add(InxCourbe).Swap(TResultEtal); 

					// Passe à l'index courbe suivant
					InxCourbe++;
				}					
			}
		}
	}

	else
	{
		//////////////////////////////  Mode supports couleurs regroupées //////////////////////////
		//
		// Boucle sur les couleurs existantes
		// Ici on cumule les résultats pour regrouper les supports d'une même couleur
		for (JUnt32 IdColor = 1; IdColor <= 12; IdColor++)
		{
			// Test déjà si cette couleur a été sélectionnée au moins pour 1 support
			bool ColorSelect = false;
			for (m_pATPDocChoixSupport->m_MapIdColorSupports.MoveFirst();
				 m_pATPDocChoixSupport->m_MapIdColorSupports.IsValid();
				 m_pATPDocChoixSupport->m_MapIdColorSupports.MoveNext())
			{
				// Récupère Id Couleur Associée
				JUnt32 IdColorSupp = m_pATPDocChoixSupport->m_MapIdColorSupports.GetItem();
				if (IdColorSupp == IdColor)
				{
					ColorSelect = true;
					break;
				}
			}

			// Si couleur sélectionnée on effectue les calculs cumulés
			if (ColorSelect)
			{
				// Tableau résultat pour cette couleur
				JArray <JFlt64> TResult;
				TResult.Reset();
				TResult.SetCount(MaxJourCourbe);

				for (JInt32 jj=0; jj < TResult.GetCount(); jj++)
				{
					TResult.Item(jj) = 0.0; 
				}

				// Tableau étalonnage résultat pour cette couleur
				JArray <bool> TResultEtal;
				TResultEtal.Reset();
				TResultEtal.SetCount(MaxJourCourbe);

				for (JInt32 jj=0; jj < TResultEtal.GetCount(); jj++)
				{
					TResultEtal.Item(jj) = false; 
				}

				for (m_pATPDocChoixSupport->m_MapIdColorSupports.MoveFirst();
					 m_pATPDocChoixSupport->m_MapIdColorSupports.IsValid();
				     m_pATPDocChoixSupport->m_MapIdColorSupports.MoveNext())
				{
					// Récupère Id Support
					JUnt32 IdSupport = m_pATPDocChoixSupport->m_MapIdColorSupports.GetKey(); 

					// Attention aux supports non visibles
					JBool EtatVisible = m_pATPDocChoixSupport->GetSupportVisible(IdSupport);

					if (EtatVisible)
					{
						// Test si support avec couleur courante	
						if (m_pATPDocChoixSupport->m_MapIdColorSupports.GetItem() == IdColor)
						{
							// Récup tableau résultat
							m_pATPDocChoixSupport->m_MapResultCrbCum.MoveTo(IdSupport); 
							m_pATPDocChoixSupport->m_MapResultEtalCrbCum.MoveTo(IdSupport); 

							if (m_pATPDocChoixSupport->m_MapResultCrbCum.IsValid() && m_pATPDocChoixSupport->m_MapResultEtalCrbCum.IsValid())
							{
								JArray <JFlt64> &TResultSupp		= m_pATPDocChoixSupport->m_MapResultCrbCum.GetItem(); 
								JArray <bool>   &TResultEtalSupp	= m_pATPDocChoixSupport->m_MapResultEtalCrbCum.GetItem(); 

								// Récup pour tous les indices horizontaux
								bool DepResult = false;
								
								// Cas spécif pour les côut, prendre en compte de la zone indéfini du début
								m_pATPDocChoixSupport->m_MapIdGrandCumSel.MoveFirst();
								if (m_pATPDocChoixSupport->m_MapIdGrandCumSel.GetItem() != 7)
									DepResult = true;

								// Boucle pour cumul des valeurs
								for (JUnt32 iRow = 0; iRow < TResultSupp.GetCount(); iRow++)
								{
									// on récupére les étalonnages couplages
									if (TResultEtalSupp.Item(iRow))
									{
										TResultEtal.Item(iRow) = true;
										DepResult = true;
									}

									// on récupère et cumule les résultats
									// A VOIR !!!!
									//if (DepResult)
									TResult.Item(iRow) += TResultSupp.Item(iRow); 

								}
							}					
						}
					}
				}

				// Sauve résultat pour la courbe
				m_MapResultCrb.Add(InxCourbe).Swap(TResult); 

				// Sauve résultat étalonnage pour la courbe
				m_MapResultEtalCrb.Add(InxCourbe).Swap(TResultEtal); 

				// Passe à la courbe suivante
				InxCourbe++;
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Récupère couleur de chaque courbe
JVoid CATPGraphCrbCum::RecupColorCourbe()
{
	// Dimensionne le tableau des couleurs de courbe
	m_TabIndexColor.Reset();
	m_TabIndexColor.SetCount(NbCourbeGraph()); 

	// Init nombre de courbes
	JUnt32 InxCourbe = 0;

	// Boucle sur les supports sélectionnés
	if (m_pATPDocChoixSupport->m_ModeColorGraphe == 0)
	{
		//////////////////////////////  Mode supports couleurs indépendantes //////////////////////////
		//
		// Boucle sur les supports sélectionnés
		for (m_pATPDocChoixSupport->m_MapIdColorSupports.MoveFirst(); 
			 m_pATPDocChoixSupport->m_MapIdColorSupports.IsValid(); 
			 m_pATPDocChoixSupport->m_MapIdColorSupports.MoveNext())
		{
			// Récupère Id Support
			JUnt32 IdSupport = m_pATPDocChoixSupport->m_MapIdColorSupports.GetKey(); 

			// Attention aux supports non visibles
			JBool EtatVisible = m_pATPDocChoixSupport->GetSupportVisible(IdSupport);

			// Récupère Id Couleur Associée
			JUnt32 IdColor = m_pATPDocChoixSupport->m_MapIdColorSupports.GetItem();

			// 1 courbe par support, incrémente le nb courbe
			if (IdColor > 0 && EtatVisible)
			{
				// Stocke la couleur courbe
				m_TabIndexColor.Item(InxCourbe) = IdColor;

				// Passe à l'index courbe suivant
				InxCourbe++;
			}
		}
	}

	else
	{
		//////////////////////////////  Mode supports couleurs regroupées //////////////////////////
		//
		// Boucle sur les couleurs existantes
		for (JUnt32 IdColor = 1; IdColor <= 12; IdColor++)
		{
			// Test déjà si cette couleur a été sélectionnée au moins pour 1 support
			bool ColorSelect = false;
			for (m_pATPDocChoixSupport->m_MapIdColorSupports.MoveFirst();
				 m_pATPDocChoixSupport->m_MapIdColorSupports.IsValid();
				 m_pATPDocChoixSupport->m_MapIdColorSupports.MoveNext())
			{
				// Récupère Id Couleur Associée
				JUnt32 IdColorSupp = m_pATPDocChoixSupport->m_MapIdColorSupports.GetItem();
				if (IdColorSupp == IdColor)
				{
					ColorSelect = true;
					break;
				}
			}

			// Si couleur sélectionnée sur au moins 1 support, incrémente nb couleurs valides
			if (ColorSelect)
			{
				// Stocke la couleur courbe
				m_TabIndexColor.Item(InxCourbe) = IdColor;

				// Passe à l'index courbe suivant
				InxCourbe++;
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
// la fonction pour récupérer borne inférieure départ courbe
JUnt32 CATPGraphCrbCum::GetDepCourbe(JUnt32 InxCourbe) const
{
	return m_TDepAbscisse.Item(InxCourbe) ;
}

