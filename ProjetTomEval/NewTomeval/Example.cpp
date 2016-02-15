void Example()
{
	CPopulationCibleFile Fichier;
	
	// Ouverture du fichier
	Fichier.Open("Population.tbl");

	// Recuperation du nombre de dates
	INT NbDates; Fichier.ReadNbDates(&NbDates);

	// boucle sur toutes les dates
	for (INT Idx = 0; Idx < NbDates; Idx += 1)
	{
		INT Annee, Mois, Jour, NbCibles;
		INT* pCibles;
		INT* pPopulations;
		CString* pNoms;

		// récupération de toutes les populations de la date courante
		Fichier.ReadDate(&Annee, &Mois, &Jour, &NbCibles, &pCibles, &pPopulations, &pNoms);

		// boucle sur toutes les cibles
		for (INT Jdx = 0; Jdx < NbCibles; Jdx += 1)
		{
			// METTRE TON TRAITEMENT ICI
			CString Nom    = pNoms[Jdx];
			INT Cible      = pCibles[Jdx];
			INT Population = pPopulations[Jdx];
		}

		// Attention !!! Libération des infos par l'appelant
		delete [] pNoms;
		delete [] pCibles;
		delete [] pPopulations;
	}

	// Fermeture du fichier
	Fichier.Close();	
}
