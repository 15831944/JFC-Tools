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
	// Init R�sultats Courbe
	m_MapResultCrb.Reset();

	// Init R�sultats Etalonnage Courbe
	m_MapResultEtalCrb.Reset();

	// Init Tableau des couleurs courbes
	m_TabIndexColor.Reset();

	// Init tableau d�but de chaque courbe (par d�faut on d�marre au point 0)
	m_TDepAbscisse.Reset();
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Destructeur
CATPGraphCrbCum::~CATPGraphCrbCum()
{
	// A FAIRE
}

///////////////////////////////////////
// la fonction pour d�finir le document

JVoid CATPGraphCrbCum::SetDocument(JATPDocChoixSupport* pDocChoixSupport)
{
	// on m�morise le document
	m_pATPDocChoixSupport = pDocChoixSupport;

}

//////////////////////////////////////////////////////////////////////////////////////////////
// Mise � jour r�sultats graphe
JVoid CATPGraphCrbCum::Update()
{
	// Les r�sultats � tracer
	m_pATPDocChoixSupport->PrepareMatriceTraceCum();

	// R�cup�re couleur de chaque courbe
	RecupColorCourbe();

	// R�cup�re libell� de chaque courbe (A FAIRE)
	// m_TabLibCourbe.SetCount(NbCourbeGraph); 

	// Consolidation r�sultats par couleur de courbe
	ConsolidResultCrb();

	// Mise � jour du point de d�part
	m_TDepAbscisse.Reset();
	m_TDepAbscisse.SetCount(m_MapResultEtalCrb.GetCount());
	for (JUnt32 InxCrb = 0; InxCrb < m_MapResultEtalCrb.GetCount(); InxCrb++)
		m_TDepAbscisse.Item(InxCrb) = 0;


	m_pATPDocChoixSupport->m_MapIdGrandCumSel.MoveFirst();
	if (m_pATPDocChoixSupport->m_MapIdGrandCumSel.IsValid())
	{
		// A FAIRE / d�finir les constantes grandeurs
		if (m_pATPDocChoixSupport->m_MapIdGrandCumSel.GetItem() == 7)
		{
			// Boucle sur toutes les courbes
			JUnt32 InxCourbe = 0;
			for (m_MapResultEtalCrb.MoveFirst(); m_MapResultEtalCrb.IsValid(); m_MapResultEtalCrb.MoveNext())
			{
				// R�cup tableau des �talonnages correspondant
				JArray <bool> &TResultEtal = m_MapResultEtalCrb.GetItem();

				m_TDepAbscisse.Item(InxCourbe) = 0;
				for (JUnt32 k = 0; k < TResultEtal.GetCount(); k++)
				{
					if (TResultEtal.Item(k))
					{
						// Init d�part courbe
						m_TDepAbscisse.Item(InxCourbe) = k;
						break;
					}
				}

				// Passe � la courbe suivante
				InxCourbe++;
			}
		}	
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�ration nombre d'�l�ments axe horizontal
JInt32 CATPGraphCrbCum::GetXElmtCount() const
{
	// Calcul maximum graduation
	return MaxJoursSuppGraphe();
}

//////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�ration nombre total de courbes
JInt32 CATPGraphCrbCum::GetCourbeCount() const
{
	return NbCourbeGraph();
}

//////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�ration couleur de la courbe
JInt32 CATPGraphCrbCum::GetCourbeColor(JInt32 CourbeIndex) const
{
	return m_TabIndexColor.Item(CourbeIndex);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�ration couleur via index
COLORREF CATPGraphCrbCum::GetColor(JInt32 IdColor) const
{
	return CATPColors::GetColorGrph(IdColor-1);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// la fonction pour r�cup�rer la validit� des �l�ments
JBool CATPGraphCrbCum::IsItemValid(JInt32 XIndex, JInt32 CourbeIndex) const
{
	// Attention aux bornes horizontales et verticales
	if (XIndex <= MaxJoursSuppGraphe() && CourbeIndex <= m_TabIndexColor.GetCount()) 
		return true;
	else
		return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// la fonction pour r�cup�rer la valeur des �l�ments
JFlt64 CATPGraphCrbCum::GetValueItem(JInt32 XIndex, JInt32 CourbeIndex, bool &ItemMarque) const
{
	// On se positionne sur les r�sultats courbe
	m_MapResultCrb.MoveTo(CourbeIndex);

	// Voir si ce point fait partie des p�riodicit�s
	m_MapResultEtalCrb.MoveTo(CourbeIndex);
	if (m_MapResultEtalCrb.IsValid())
	{
		JUnt32 NbItem = m_MapResultEtalCrb.GetItem().GetCount(); 

		if (XIndex < m_MapResultEtalCrb.GetItem().GetCount())
			ItemMarque = m_MapResultEtalCrb.GetItem().Item(XIndex);
	}

	// Si r�sultats existent
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
		// Ici r�sultats non existants
		return -1.0;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// la fonction pour r�cup�rer le libell� de la courbe
const JString CATPGraphCrbCum::GetLabelItem(JInt32 CourbeIndex) const
{
	// Info n� de la courbe ou libell� support
	CString LibCourbe;

	LibCourbe.Format("%s %d","Courbe",m_TabIndexColor.Item(CourbeIndex)); 
	JString Libelle = LibCourbe;
	return Libelle;
}


/////////////////////////////////////////////////////////////////////////////////////////////
// la fonction pour r�cup�rer le type de r�sultat horizontal
const JString CATPGraphCrbCum::GetLabelResultHorz() const
{
	// Forc�ment en abscisse nb jours
	return "Jours";
}

/////////////////////////////////////////////////////////////////////////////////////////////
// la fonction pour r�cup�rer le type de r�sultat vertical
const JString CATPGraphCrbCum::GetLabelResultVert() const
{
	// On se positionne sur l'�l�ment type r�sultat s�lectionn�
	m_pATPDocChoixSupport->m_MapIdGrandCumSel.MoveFirst();
	if (m_pATPDocChoixSupport->m_MapIdGrandCumSel.IsValid()) 
	{
		// R�cup�re l'indice de la s�lection
		JInt32 IndexGrandeur = m_pATPDocChoixSupport->m_MapIdGrandCumSel.GetItem();

		// Renvoi le libell� trouv�
		const JString ret(m_pATPDocChoixSupport->GetGrandeur(m_pATPDocChoixSupport->ModuleCum, IndexGrandeur));
		return ret;
	}		
	else
		// pas de grandeur d�finie
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
				// au moins 1 terrain s�lectionn�
				IdTerrain = m_pATPDocChoixSupport->GetMapIdTerrainsAudienceSel()->GetKey(); 
			}
			else
				return 0;

			// Recup la cible en cours
			JUnt32 IdCible;
			m_pATPDocChoixSupport->GetMapIdCiblesAudienceSel()->MoveFirst();
			if (m_pATPDocChoixSupport->GetMapIdCiblesAudienceSel()->IsValid())
			{
				// au moins 1 cible s�lectionn�e
				IdCible = m_pATPDocChoixSupport->GetMapIdCiblesAudienceSel()->GetKey();  
			}
			else
				return 0;

			// Renvoi maximum jours � prendre ne compte
			return m_pATPDocChoixSupport->MaxJourCourbeCum(IdTerrain,IdCible);
		}
	}

	// Aucun support s�lectionn�
	return 0;
}	

/////////////////////////////////////////////////////////////////////////////////////////////
// Calcul le nombre de courbes � dessiner
JInt32 CATPGraphCrbCum::NbCourbeGraph() const
{
	// Init nombre de courbes
	JInt32 NbCourbe = 0;

	// Boucle sur les supports s�lectionn�s
	if (m_pATPDocChoixSupport->m_ModeColorGraphe == 0)
	{
		//////////////////////////////  Mode supports couleurs ind�pendantes //////////////////////////
		//
		// Boucle sur les supports s�lectionn�s
		for (m_pATPDocChoixSupport->m_MapIdColorSupports.MoveFirst(); 
			 m_pATPDocChoixSupport->m_MapIdColorSupports.IsValid(); 
			 m_pATPDocChoixSupport->m_MapIdColorSupports.MoveNext())
		{
			// R�cup�re Id Support
			JUnt32 IdSupport = m_pATPDocChoixSupport->m_MapIdColorSupports.GetKey(); 

			// Attention aux supports non visibles
			JBool EtatVisible = m_pATPDocChoixSupport->GetSupportVisible(IdSupport);

			// R�cup�re Id Couleur Associ�e
			JUnt32 IdColor = m_pATPDocChoixSupport->m_MapIdColorSupports.GetItem();

			// 1 courbe par support, incr�mente le nb courbe
			if (IdColor > 0 && EtatVisible)
				NbCourbe++;
		}
	}

	else
	{
		//////////////////////////////  Mode supports couleurs regroup�es //////////////////////////
		//
		// Boucle sur les couleurs existantes
		for (JUnt32 IdColor = 1; IdColor <= 12; IdColor++)
		{
			// Test d�j� si cette couleur a �t� s�lectionn�e au moins pour 1 support
			bool ColorSelect = false;
			for (m_pATPDocChoixSupport->m_MapIdColorSupports.MoveFirst();
				 m_pATPDocChoixSupport->m_MapIdColorSupports.IsValid();
				 m_pATPDocChoixSupport->m_MapIdColorSupports.MoveNext())
			{
				// R�cup�re Id Couleur Associ�e
				JUnt32 IdColorSupp = m_pATPDocChoixSupport->m_MapIdColorSupports.GetItem();
				if (IdColorSupp == IdColor)
				{
					ColorSelect = true;
					break;
				}
			}

			// Si couleur s�lectionn�e sur au moins 1 support, incr�mente nb couleurs valides
			if (ColorSelect)
				NbCourbe++;
		}
	}

	// Retourne le nombre de courbes � dessiner
	return NbCourbe;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// la fonction pour r�cup�rer le libell� support des courbes
const JString CATPGraphCrbCum::GetLibCourbeGraph(JUnt32 InxCourbe) const
{
	// Boucle sur les supports s�lectionn�s
	if (m_pATPDocChoixSupport->m_ModeColorGraphe == 0)
	{
		// Init Index Courbe
		JUnt32 InxCrbCur = -1;

		//////////////////////////////  Mode supports couleurs ind�pendantes //////////////////////////
		//
		// Boucle sur les supports s�lectionn�s
		for (m_pATPDocChoixSupport->m_MapIdColorSupports.MoveFirst(); 
			 m_pATPDocChoixSupport->m_MapIdColorSupports.IsValid(); 
			 m_pATPDocChoixSupport->m_MapIdColorSupports.MoveNext())
		{
			// R�cup�re Id Support
			JUnt32 IdSupport = m_pATPDocChoixSupport->m_MapIdColorSupports.GetKey(); 

			// R�cup�re Id Couleur Associ�e
			JUnt32 IdColor = m_pATPDocChoixSupport->m_MapIdColorSupports.GetItem();

			// Attention aux supports non visibles
			JBool EtatVisible = m_pATPDocChoixSupport->GetSupportVisible(IdSupport);

			// 1 courbe par support, incr�mente le nb courbe
			if (IdColor > 0 && EtatVisible)
			{
				InxCrbCur++;
			}

			if (InxCrbCur == InxCourbe)
			{
				// R�cup�re objet regroupement
				CRegroupements * pRegroupements = CRegroupements::GetInstance();

				// Element support
				IElem* pSupport = pRegroupements->GetIElemByID(IdSupport);

				// on r�cup�re le libell� support
				CString LibSupport = "";
				if (pSupport)
					// Affichage libell� support
					LibSupport = pSupport->GetLabel().AsJCharPtr();

				const JString ret (LibSupport);
				return ret;
			}
		}
	}
	else
	{
		// On indique simplement n� de la couleur courbe
		CString LibCourbe;

		LibCourbe.Format("%s %d","Courbe",m_TabIndexColor.Item(InxCourbe)); 
		JString Libelle = LibCourbe;
		return Libelle;
	}
	return "erreur";
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Consolidation r�sultats par courbe
JVoid CATPGraphCrbCum::ConsolidResultCrb()
{
	// Init R�sultats Courbe
	m_MapResultCrb.Reset();

	// Init R�sultats Etalonnage Courbe
	m_MapResultEtalCrb.Reset();

	// Maximum jour pour la courbe
	JUnt32 MaxJourCourbe = MaxJoursSuppGraphe();						

	// Index courbe
	JUnt32 InxCourbe = 0;

	// Boucle sur les supports s�lectionn�s
	if (m_pATPDocChoixSupport->m_ModeColorGraphe == 0)
	{

		//////////////////////////////  Mode supports couleurs ind�pendantes //////////////////////////
		//
		// Boucle sur les supports s�lectionn�s
		for (m_pATPDocChoixSupport->m_MapIdColorSupports.MoveFirst(); 
			 m_pATPDocChoixSupport->m_MapIdColorSupports.IsValid(); 
			 m_pATPDocChoixSupport->m_MapIdColorSupports.MoveNext())
		{
			// R�cup�re Id Support
			JUnt32 IdSupport = m_pATPDocChoixSupport->m_MapIdColorSupports.GetKey(); 

			// Attention aux supports non visibles
			JBool EtatVisible = m_pATPDocChoixSupport->GetSupportVisible(IdSupport);

			// R�cup�re Id Couleur Associ�e
			JUnt32 IdColor = m_pATPDocChoixSupport->m_MapIdColorSupports.GetItem();

			// 1 courbe par support, incr�mente le nb courbe
			if (IdColor > 0 && EtatVisible)
			{
				// Tableau r�sultat pour cette couleur
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

				// R�cup tableau r�sultat
				m_pATPDocChoixSupport->m_MapResultCrbCum.MoveTo(IdSupport); 
				m_pATPDocChoixSupport->m_MapResultEtalCrbCum.MoveTo(IdSupport); 
				if (m_pATPDocChoixSupport->m_MapResultCrbCum.IsValid() && m_pATPDocChoixSupport->m_MapResultEtalCrbCum.IsValid())
				{
					JArray <JFlt64> &TResultSupp		=  m_pATPDocChoixSupport->m_MapResultCrbCum.GetItem(); 
					JArray <bool>   &TResultEtalSupp	=  m_pATPDocChoixSupport->m_MapResultEtalCrbCum.GetItem(); 

					// R�cup pour tous les indices horizontaux
					for (JInt32 iRow = 0; iRow < TResultSupp.GetCount(); iRow++)
					{
						// on r�cup�re les r�sultats
						JFlt64 ValBase = TResult.Item(iRow);

						JFlt64 Valeur		= TResultSupp.Item(iRow);
						TResult.Item(iRow) += TResultSupp.Item(iRow); 

						JFlt64 ValRes = TResult.Item(iRow);
						JFlt64 ValRes2 = ValRes; 

						// R�cup�re si �talonnage ou pas
						TResultEtal.Item(iRow) = TResultEtalSupp.Item(iRow);
					}

					// Sauve r�sultat pour la courbe
					m_MapResultCrb.Add(InxCourbe).Swap(TResult); 

					// Sauve r�sultat �talonnage pour la courbe
					m_MapResultEtalCrb.Add(InxCourbe).Swap(TResultEtal); 

					// Passe � l'index courbe suivant
					InxCourbe++;
				}					
			}
		}
	}

	else
	{
		//////////////////////////////  Mode supports couleurs regroup�es //////////////////////////
		//
		// Boucle sur les couleurs existantes
		// Ici on cumule les r�sultats pour regrouper les supports d'une m�me couleur
		for (JUnt32 IdColor = 1; IdColor <= 12; IdColor++)
		{
			// Test d�j� si cette couleur a �t� s�lectionn�e au moins pour 1 support
			bool ColorSelect = false;
			for (m_pATPDocChoixSupport->m_MapIdColorSupports.MoveFirst();
				 m_pATPDocChoixSupport->m_MapIdColorSupports.IsValid();
				 m_pATPDocChoixSupport->m_MapIdColorSupports.MoveNext())
			{
				// R�cup�re Id Couleur Associ�e
				JUnt32 IdColorSupp = m_pATPDocChoixSupport->m_MapIdColorSupports.GetItem();
				if (IdColorSupp == IdColor)
				{
					ColorSelect = true;
					break;
				}
			}

			// Si couleur s�lectionn�e on effectue les calculs cumul�s
			if (ColorSelect)
			{
				// Tableau r�sultat pour cette couleur
				JArray <JFlt64> TResult;
				TResult.Reset();
				TResult.SetCount(MaxJourCourbe);

				for (JInt32 jj=0; jj < TResult.GetCount(); jj++)
				{
					TResult.Item(jj) = 0.0; 
				}

				// Tableau �talonnage r�sultat pour cette couleur
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
					// R�cup�re Id Support
					JUnt32 IdSupport = m_pATPDocChoixSupport->m_MapIdColorSupports.GetKey(); 

					// Attention aux supports non visibles
					JBool EtatVisible = m_pATPDocChoixSupport->GetSupportVisible(IdSupport);

					if (EtatVisible)
					{
						// Test si support avec couleur courante	
						if (m_pATPDocChoixSupport->m_MapIdColorSupports.GetItem() == IdColor)
						{
							// R�cup tableau r�sultat
							m_pATPDocChoixSupport->m_MapResultCrbCum.MoveTo(IdSupport); 
							m_pATPDocChoixSupport->m_MapResultEtalCrbCum.MoveTo(IdSupport); 

							if (m_pATPDocChoixSupport->m_MapResultCrbCum.IsValid() && m_pATPDocChoixSupport->m_MapResultEtalCrbCum.IsValid())
							{
								JArray <JFlt64> &TResultSupp		= m_pATPDocChoixSupport->m_MapResultCrbCum.GetItem(); 
								JArray <bool>   &TResultEtalSupp	= m_pATPDocChoixSupport->m_MapResultEtalCrbCum.GetItem(); 

								// R�cup pour tous les indices horizontaux
								bool DepResult = false;
								
								// Cas sp�cif pour les c�ut, prendre en compte de la zone ind�fini du d�but
								m_pATPDocChoixSupport->m_MapIdGrandCumSel.MoveFirst();
								if (m_pATPDocChoixSupport->m_MapIdGrandCumSel.GetItem() != 7)
									DepResult = true;

								// Boucle pour cumul des valeurs
								for (JUnt32 iRow = 0; iRow < TResultSupp.GetCount(); iRow++)
								{
									// on r�cup�re les �talonnages couplages
									if (TResultEtalSupp.Item(iRow))
									{
										TResultEtal.Item(iRow) = true;
										DepResult = true;
									}

									// on r�cup�re et cumule les r�sultats
									// A VOIR !!!!
									//if (DepResult)
									TResult.Item(iRow) += TResultSupp.Item(iRow); 

								}
							}					
						}
					}
				}

				// Sauve r�sultat pour la courbe
				m_MapResultCrb.Add(InxCourbe).Swap(TResult); 

				// Sauve r�sultat �talonnage pour la courbe
				m_MapResultEtalCrb.Add(InxCourbe).Swap(TResultEtal); 

				// Passe � la courbe suivante
				InxCourbe++;
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�re couleur de chaque courbe
JVoid CATPGraphCrbCum::RecupColorCourbe()
{
	// Dimensionne le tableau des couleurs de courbe
	m_TabIndexColor.Reset();
	m_TabIndexColor.SetCount(NbCourbeGraph()); 

	// Init nombre de courbes
	JUnt32 InxCourbe = 0;

	// Boucle sur les supports s�lectionn�s
	if (m_pATPDocChoixSupport->m_ModeColorGraphe == 0)
	{
		//////////////////////////////  Mode supports couleurs ind�pendantes //////////////////////////
		//
		// Boucle sur les supports s�lectionn�s
		for (m_pATPDocChoixSupport->m_MapIdColorSupports.MoveFirst(); 
			 m_pATPDocChoixSupport->m_MapIdColorSupports.IsValid(); 
			 m_pATPDocChoixSupport->m_MapIdColorSupports.MoveNext())
		{
			// R�cup�re Id Support
			JUnt32 IdSupport = m_pATPDocChoixSupport->m_MapIdColorSupports.GetKey(); 

			// Attention aux supports non visibles
			JBool EtatVisible = m_pATPDocChoixSupport->GetSupportVisible(IdSupport);

			// R�cup�re Id Couleur Associ�e
			JUnt32 IdColor = m_pATPDocChoixSupport->m_MapIdColorSupports.GetItem();

			// 1 courbe par support, incr�mente le nb courbe
			if (IdColor > 0 && EtatVisible)
			{
				// Stocke la couleur courbe
				m_TabIndexColor.Item(InxCourbe) = IdColor;

				// Passe � l'index courbe suivant
				InxCourbe++;
			}
		}
	}

	else
	{
		//////////////////////////////  Mode supports couleurs regroup�es //////////////////////////
		//
		// Boucle sur les couleurs existantes
		for (JUnt32 IdColor = 1; IdColor <= 12; IdColor++)
		{
			// Test d�j� si cette couleur a �t� s�lectionn�e au moins pour 1 support
			bool ColorSelect = false;
			for (m_pATPDocChoixSupport->m_MapIdColorSupports.MoveFirst();
				 m_pATPDocChoixSupport->m_MapIdColorSupports.IsValid();
				 m_pATPDocChoixSupport->m_MapIdColorSupports.MoveNext())
			{
				// R�cup�re Id Couleur Associ�e
				JUnt32 IdColorSupp = m_pATPDocChoixSupport->m_MapIdColorSupports.GetItem();
				if (IdColorSupp == IdColor)
				{
					ColorSelect = true;
					break;
				}
			}

			// Si couleur s�lectionn�e sur au moins 1 support, incr�mente nb couleurs valides
			if (ColorSelect)
			{
				// Stocke la couleur courbe
				m_TabIndexColor.Item(InxCourbe) = IdColor;

				// Passe � l'index courbe suivant
				InxCourbe++;
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
// la fonction pour r�cup�rer borne inf�rieure d�part courbe
JUnt32 CATPGraphCrbCum::GetDepCourbe(JUnt32 InxCourbe) const
{
	return m_TDepAbscisse.Item(InxCourbe) ;
}

