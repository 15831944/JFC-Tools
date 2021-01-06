//
// Fichier: JMTFR03TBLTerrains.cpp
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on inclut les définitions nécessaires
#include "JFR03Sources.h"

// on inclut les définitions nécessaires
#include "JMTFR03TBLTerrains.h"

// on définit le répertoire des sources
static JChar g_Sources[] = "";

// on définit les suffixes des archives des sources
static JChar g_Suffixe_04[]       = ".04";
static JChar g_Suffixe_0A[]       = ".0A";
static JChar g_Suffixe_09[]       = ".09";
static JChar g_Suffixe_0B[]       = ".0B";
static JChar g_Suffixe_2A[]       = ".2A";
static JChar g_Suffixe_08_5[]     = "_5.08";
static JChar g_Suffixe_08_6[]     = "_6.08";
static JChar g_Suffixe_08_7[]     = "_7.08";
static JChar g_Suffixe_08_8[]     = "_8.08";
static JChar g_Suffixe_08_12001[] = "_12001.08";
static JChar g_Suffixe_08_12XX1[] = "_12991.08";
static JChar g_Suffixe_08_12002[] = "_12002.08";
static JChar g_Suffixe_08_12004[] = "_12004.08";
static JChar g_Suffixe_08_12007[] = "_12007.08";
static JChar g_Suffixe_08_12014[] = "_12014.08";
static JChar g_Suffixe_08_12030[] = "_12030.08";
static JChar g_Suffixe_08_12060[] = "_12060.08";
static JChar g_Suffixe_08_12090[] = "_12090.08";
static JChar g_Suffixe_08_12120[] = "_12120.08";
static JChar g_Suffixe_08_12180[] = "_12180.08";
static JChar g_Suffixe_08_12270[] = "_12270.08";
static JChar g_Suffixe_08_12365[] = "_12365.08";

////////////////////
// les constructeurs

JMTFR03TBLTerrains::JMTFR03TBLTerrains()
{
	// on ne fait rien
}

JMTFR03TBLTerrains::JMTFR03TBLTerrains(const JMTFR03TBLTerrains & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'opérateur pour recopier les éléments

JMTFR03TBLTerrains & JMTFR03TBLTerrains::operator =(const JMTFR03TBLTerrains & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// la fonction pour fabriquer les chemins

JVoid JMTFR03TBLTerrains::OnMakePath(const JPath & Root, const JChar* pSuffixe, JPath & Path) const
{
	// on initialise l'indice
	JInt32 Idx = 0;

	// on initialise le buffer
	JChar Buf[JPATH_MAX + 24];

	// on ajoute les sources au chemin
	const JChar* pSources = g_Sources; while (*pSources != 0) { Buf[Idx] = *pSources; pSources += 1; Idx += 1; }

	// on ajoute la racine du terrain au chemin
	const JChar* pRoot = Root.AsJCharPtr(); while (*pRoot != 0) { Buf[Idx] = *pRoot; pRoot += 1; Idx += 1; }

	// on ajoute le suffixe au chemin
	while (*pSuffixe != 0) { Buf[Idx] = *pSuffixe; pSuffixe += 1; Idx += 1; }

	// on ajoute le code final
	Buf[Idx] = 0; Idx += 1;

	// on renvoie le chemin
	Path = Buf;
}

////////////////////////////////////////////
// les fonctions pour sérialiser les entêtes

JBool JMTFR03TBLTerrains::OnRecvHeader(JStream & Stream, JInt32 Magic) const
{
	// on sérialise l'entête
	JInt32 H1; Stream.Recv(H1); if (H1 != 0x0043464A) return (false);
	JInt32 H2; Stream.Recv(H2); if (H2 != Magic)      return (false);
	JInt32 H3; Stream.Recv(H3); if (H3 != 0x00000000) return (false);
	JInt32 H4; Stream.Recv(H4); if (H4 != 0x00000000) return (false);
	JInt32 H5; Stream.Recv(H5); if (H5 != 0x00000001) return (false);

	// on quitte
	return (true);
}

JBool JMTFR03TBLTerrains::OnRecvSource(JStream & Stream, JRef32 IdSource, JRef32 IdTerrain) const
{
	// on sérialise les identifiants de la source et du terrain
	JRef32 IdSrc; IdSrc.Recv(Stream); if (IdSrc != IdSource)  return (false);
	JRef32 IdTer; IdTer.Recv(Stream); if (IdTer != IdTerrain) return (false);

	// on quitte
	return (true);
}

////////////////////////////////////////////////////////
// les fonctions pour sérialiser les éléments du terrain

JBool JMTFR03TBLTerrains::OnRecvFiltresAudience(JStream & Stream, JInt32 NbIndividus, JMTFR03TBLFiltres & TBLFiltres) const
{
	// on sérialise le nombre de dimensions
	JInt32 NbDimensions; Stream.Recv(NbDimensions); if (NbDimensions != 1) return (false);

	// on sérialise l'identifiant de l'unique dimension
	JRef32 IdDimension; IdDimension.Recv(Stream); if (IdDimension.AsJInt32() != 1) return (false);

	// on sérialise le nombre de filtres
	JInt32 NbFiltres; Stream.Recv(NbFiltres); if (NbFiltres < 0) return (false);

	// on boucle sur tous les filtres
	for (JInt32 IdxFiltre = 0; IdxFiltre < NbFiltres; IdxFiltre += 1)
	{
		// on crée la clé
		JMTFR03KEYFiltre KEYFiltre;

		// on sérialise les éléments du filtre
		JRef32 IdTitre;  IdTitre .Recv(Stream); if (!IdTitre .IsValid()) return (false);
		JRef32 IdFiltre; IdFiltre.Recv(Stream); if (!IdFiltre.IsValid()) return (false);

		// on alloue le vecteur des individus
		JBitArray ItemIndividus; ItemIndividus.SetCount(NbIndividus);

		// on boucle sur tous les masques des individus
		for (JInt32 IdxMasque = 0; IdxMasque < NbIndividus; IdxMasque += 32)
		{
			// on sérialise le masque et on met à jour le vecteur des individus
			JMsk32 Mask; Mask.Recv(Stream); ItemIndividus.SetMaskAt(IdxMasque, Mask);
		}

		// on initialise la clé du filtre d'audience
		KEYFiltre.m_IdFiltre = IdFiltre;
		KEYFiltre.m_IdTitre  = IdTitre;

		// on teste si le filtre d'audience existe déjà
		TBLFiltres.MoveTo(KEYFiltre); if (TBLFiltres.IsValid()) return (false);

		// on ajoute le nouveau filtre d'audience
		JMTFR03DATFiltre & DATFiltre = TBLFiltres.Add(KEYFiltre);

		// on initialise le fitre d'audience
		DATFiltre.m_ItemIndividus.Swap(ItemIndividus);
	}

	// on quitte
	return (true);
}

JBool JMTFR03TBLTerrains::OnRecvSegmentsPopulation(JStream & Stream, JInt32 NbIndividus, JMTFR03TBLSegments & TBLSegments) const
{
	// on sérialise le nombre de segments
	JInt32 NbSegments; Stream.Recv(NbSegments); if (NbSegments < 0) return (false);

	// on boucle sur tous les segments
	for (JInt32 IdxSegment = 0; IdxSegment < NbSegments; IdxSegment += 1)
	{
		// on crée la clé
		JMTFR03KEYSegment KEYSegment;

		// on sérialise les éléments du segment
		JRef32 IdSegment; IdSegment.Recv(Stream); if (!IdSegment.IsValid()) return (false);
		JMsk32 Masque;    Masque   .Recv(Stream);

		// on alloue le vecteur des individus
		JBitArray ItemIndividus; ItemIndividus.SetCount(NbIndividus);

		// on boucle sur tous les masques des individus
		for (JInt32 IdxMasque = 0; IdxMasque < NbIndividus; IdxMasque += 32)
		{
			// on sérialise le masque et on met à jour le vecteur des individus
			JMsk32 Mask; Mask.Recv(Stream); ItemIndividus.SetMaskAt(IdxMasque, Mask);
		}

		// on initialise la clé du segment de population
		KEYSegment.m_IdSegment = IdSegment;

		// on teste si le segment de population existe déjà
		TBLSegments.MoveTo(KEYSegment); if (TBLSegments.IsValid()) return (false);

		// on ajoute le nouveau segment de population
		JMTFR03DATSegment & DATSegment = TBLSegments.Add(KEYSegment);

		// on initialise le segment de population
		DATSegment.m_ItemIndividus.Swap(ItemIndividus);
		DATSegment.m_Masque = Masque;
	}

	// on quitte
	return (true);
}

JBool JMTFR03TBLTerrains::OnRecvPoidsIndividus(JStream & Stream, JInt32 NbIndividus, JMTFR03TBLUnivers & TBLUnivers, JMTFR03TBLEchantillons & TBLEchantillons, JMTFR03TBLHomotheties & TBLHomotheties, JMTFR03TBLPoids & TBLPoids, JMTFR03TBLPonderations & TBLPonderations) const
{
	// on récupère le nombre d'univers
	JInt32 NbUnivers; Stream.Recv(NbUnivers); if (NbUnivers < 0) return (false);

	// on alloue les identifiants des univers
	JArray<JRef32> TBLIdUnivers; TBLIdUnivers.SetCount(NbUnivers);

	// on boucle sur tous les univers
	for (JInt32 IdxUnivers = 0; IdxUnivers < NbUnivers; IdxUnivers += 1)
	{
		// on crée la clé
		JMTFR03KEYUnivers KEYUnivers;

		// on sérialise l'identifiant de l'univers
		JRef32 IdUnivers; IdUnivers.Recv(Stream); if (!IdUnivers.IsValid()) return (false);

		// on mémorise l'identifiant de l'univers
		TBLIdUnivers.Item(IdxUnivers) = IdUnivers;

		// on alloue le vecteur des individus
		JBitArray ItemIndividus; ItemIndividus.SetCount(NbIndividus);

		// on boucle sur tous les masques des individus
		for (JInt32 IdxMasque = 0; IdxMasque < NbIndividus; IdxMasque += 32)
		{
			// on sérialise le masque et on met à jour le vecteur des individus
			JMsk32 Mask; Mask.Recv(Stream); ItemIndividus.SetMaskAt(IdxMasque, Mask);
		}

		// on initialise la clé de l'univers de redressement
		KEYUnivers.m_IdUnivers = IdUnivers;

		// on teste si l'univers de redressement existe déjà
		TBLUnivers.MoveTo(KEYUnivers); if (TBLUnivers.IsValid()) return (false);

		// on ajoute le nouvel univers de redressement
		JMTFR03DATUnivers & DATUnivers = TBLUnivers.Add(KEYUnivers);

		// on initialise l'univers de redressement
		DATUnivers.m_ItemIndividus.Swap(ItemIndividus);
		DATUnivers.m_Etat.SetAt(0, IdxUnivers == 0);
	}

	// on récupère le nombre de types de poids
	JInt32 NbPoids; Stream.Recv(NbPoids); if (NbPoids < 0) return (false);

	// on alloue les identifiants des types de poids et les diviseurs
	JArray<JRef32> TBLIdPoids;   TBLIdPoids  .SetCount(NbPoids);
	JArray<JInt32> TBLDiviseurs; TBLDiviseurs.SetCount(NbPoids);

	// on boucle sur tous les types de poids
	for (JInt32 IdxPoids = 0; IdxPoids < NbPoids; IdxPoids += 1)
	{
		// on sérialise l'identifiant du type de poids
		JRef32 IdPoids; IdPoids.Recv(Stream); if (!IdPoids.IsValid()) return (false);

		// on sérialise le diviseur
		JInt32 Diviseur; Stream.Recv(Diviseur); if (Diviseur < 1) return (false);

		// on mémorise l'identifiant du type de poids et le diviseur
		TBLIdPoids  .Item(IdxPoids) = IdPoids;
		TBLDiviseurs.Item(IdxPoids) = Diviseur;
	}

	// on boucle sur tous les univers
	for (IdxUnivers = 0; IdxUnivers < NbUnivers; IdxUnivers += 1)
	{
		// on boucle sur tous les types de poids
		for (IdxPoids = 0; IdxPoids < NbPoids; IdxPoids += 1)
		{
			// on crée la clé
			JMTFR03KEYPoids KEYPoids;

			// on alloue le vecteur des poids
			JArray<JInt32x> ItemPoids; ItemPoids.SetCount(NbIndividus);

			// on boucle sur tous les individus
			for (JInt32 IdxIndividu = 0; IdxIndividu < NbIndividus; IdxIndividu += 1)
			{
				// on sérialise le poids de l'individu
				JInt32 Poids; Stream.Recv(Poids); if (Poids < 0) return (false);

				// on initialise le poids
				ItemPoids.Item(IdxIndividu) = Poids;
			}

			// on initialise la clé des poids
			KEYPoids.m_IdPoids   = TBLIdPoids  .Item(IdxPoids);
			KEYPoids.m_IdUnivers = TBLIdUnivers.Item(IdxUnivers);

			// on teste si les poids existent déjà
			TBLPoids.MoveTo(KEYPoids); if (TBLPoids.IsValid()) return (false);

			// on ajoute les nouveaux poids
			JMTFR03DATPoids & DATPoids = TBLPoids.Add(KEYPoids);

			// on initialise les poids
			DATPoids.m_ItemPoids.Swap(ItemPoids);
			DATPoids.m_Diviseur = TBLDiviseurs.Item(IdxPoids);
		}
	}

	// Initialisation du vecteur de pondérations par individu
	TBLPonderations.InitPonderation(NbIndividus); 
	
	// on récupère le nombre d'échantillons
	JInt32 NbEchantillons; Stream.Recv(NbEchantillons); if (NbEchantillons < 0) return (false);

	// on boucle sur tous les échantillons
	for (JInt32 IdxEchantillon = 0; IdxEchantillon < NbEchantillons; IdxEchantillon += 1)
	{
		// on crée la clé
		JMTFR03KEYEchantillon KEYEchantillon;

		// on sérialise l'identifiant de l'échantillon
		JRef32 IdEchantillon; IdEchantillon.Recv(Stream); if (!IdEchantillon.IsValid()) return (false);
		JMsk32 Masque;        Masque       .Recv(Stream);

		// on alloue le vecteur des individus
		JBitArray ItemIndividus; ItemIndividus.SetCount(NbIndividus);
		
		// on boucle sur tous les masques des individus
		for (JInt32 IdxMasque = 0; IdxMasque < NbIndividus; IdxMasque += 32)
		{
			// on sérialise le masque et on met à jour le vecteur des individus
			JMsk32 Mask; Mask.Recv(Stream); ItemIndividus.SetMaskAt(IdxMasque, Mask);
		}

		// on initialise la clé de l'échantillon
		KEYEchantillon.m_IdEchantillon = IdEchantillon;

		// on recherche l'échantillon
		TBLEchantillons.MoveTo(KEYEchantillon); if (TBLEchantillons.IsValid()) return (false);

		// on ajoute le nouvel échantillon
		JMTFR03DATEchantillon & DATEchantillon = TBLEchantillons.Add(KEYEchantillon);

		// on initialise l'échantillon
		DATEchantillon.m_ItemIndividus.Swap(ItemIndividus);
		DATEchantillon.m_Masque = Masque;

		// on sérialise le diviseur
		JInt32 Diviseur; Stream.Recv(Diviseur); if (Diviseur < 1) return (false);

		// on sérialise le nombre de facteurs d'homothétie
		JInt32 NbHomotheties; Stream.Recv(NbHomotheties); if (NbHomotheties < 0) return (false);

		// on boucle sur tous les facteurs d'homothétie
		for (JInt32 IdxHomothetie = 0; IdxHomothetie < NbHomotheties; IdxHomothetie += 1)
		{
			// on crée la clé
			JMTFR03KEYHomothetie KEYHomothetie;

			// on sérialise l'identifiant de l'univers
			JRef32 IdUnivers; IdUnivers.Recv(Stream); if (!IdUnivers.IsValid()) return (false);

			// on sérialise le facteur d'homothétie
			JInt32 Homothetie; Stream.Recv(Homothetie); if (Homothetie < Diviseur) return (false);

			// on initialise la clé
			KEYHomothetie.m_IdEchantillon = IdEchantillon;
			KEYHomothetie.m_IdUnivers     = IdUnivers;

			// on teste si le facteur d'homothétie existe déjà
			TBLHomotheties.MoveTo(KEYHomothetie); if (TBLHomotheties.IsValid()) return (false);

			// on ajoute le nouveau facteur d'homothétie
			JMTFR03DATHomothetie & DATHomothetie = TBLHomotheties.Add(KEYHomothetie);

			// on initialise le facteur d'homothétie
			DATHomothetie.m_Homothetie = Homothetie;
			DATHomothetie.m_Diviseur   = Diviseur;
		}
	}

	// on quitte
	return (true);
}

JBool JMTFR03TBLTerrains::OnRecvReprisesMains(JStream & Stream, JInt32 NbIndividus, JMTFR03TBLReprises & TBLReprises) const
{
	// on sérialise le type de la variable
	JInt32 Type; Stream.Recv(Type); if (Type < 1 || Type > 3) return (false);

	// on sérialise le nombre de dimensions
	JInt32 NbDimensions; Stream.Recv(NbDimensions); if (NbDimensions != 1) return (false);

	// on sérialise l'identifiant de l'unique dimension
	JRef32 IdDimension; IdDimension.Recv(Stream); if (IdDimension.AsJInt32() != 1) return (false);

	// on sérialise le diviseur
	JInt32 Diviseur; Stream.Recv(Diviseur); if (Diviseur < 1) return (false);

	// on sérialise le nombre de titres
	JInt32 NbTitres; Stream.Recv(NbTitres); if (NbTitres < 0) return (false);

	// on boucle sur tous les supports
	for (JInt32 IdxTitre = 0; IdxTitre < NbTitres; IdxTitre += 1)
	{
		// on crée la clé
		JMTFR03KEYReprise KEYReprise;

		// on sérialise l'identifiant du titre
		JRef32 IdTitre; IdTitre.Recv(Stream); if (!IdTitre.IsValid()) return (false);

		// on sérialise l'identifiant de l'univers
		JRef32 IdUnivers; IdUnivers.Recv(Stream); if (!IdUnivers.IsValid()) return (false);

		// on sérialise le nombre de reprises différentes de zéro
		JInt32 NbReprisesR; Stream.Recv(NbReprisesR); if (NbReprisesR < 0 || NbReprisesR > NbIndividus) return (false);

		// on alloue le vecteur des reprises
		JArray<JInt32x> ItemReprises; ItemReprises.SetCount(NbReprisesR);

		// on alloue le vecteur des individus
		JBitArray ItemIndividus; ItemIndividus.SetCount(NbIndividus);

		// on teste le type de la reprise
		if (Type == 1)
		{
			// on boucle sur tous les masques des individus
			for (JInt32 IdxMasque = 0; IdxMasque < NbIndividus; IdxMasque += 32)
			{
				// on sérialise le masque et on met à jour le vecteur des individus
				JMsk32 Mask; Mask.Recv(Stream); ItemIndividus.SetMaskAt(IdxMasque, Mask);
			}

			// on initialise l'indice de la reprise et le nombre total
			JInt32 IdxReprise = 0;
			JInt32 NbReprises = 0;

			// on boucle sur tous les individus
			for (JInt32 IdxIndividu = 0; IdxIndividu < NbIndividus; IdxIndividu += 1)
			{
				// on teste si l'individu est actif
				if (ItemIndividus.GetAt(IdxIndividu))
				{
					// on sérialise la reprise de l'individu
					JInt32 Reprise; Stream.Recv(Reprise); if (Reprise < 1) return (false);

					// on mémorise la reprise
					if (IdxReprise < NbReprisesR) { ItemReprises.Item(IdxReprise) = Reprise; IdxReprise += 1; }

					// on incrémente le nombre de reprises
					NbReprises += 1;
				}
			}

			// on vérifie le nombre de reprises
			if (NbReprises != NbReprisesR) return (false);
		}
		else if (Type == 2)
		{
			// on initialise le tri
			JInt32 SrtIndividu = -1;

			// on boucle sur toutes les reprises
			for (JInt32 IdxReprise = 0; IdxReprise < NbReprisesR; IdxReprise += 1)
			{
				// on sérialise l'indice de l'individu
				JInt32 IdxIndividu; Stream.Recv(IdxIndividu); if (IdxIndividu <= SrtIndividu || IdxIndividu >= NbIndividus) return (false);

				// on positionne le masque de l'individu
				ItemIndividus.SetAt(IdxIndividu, true);

				// on met à jour le tri
				SrtIndividu = IdxIndividu;
			}

			// on initialise l'indice de la variable et le nombre total
			JInt32 JdxReprise = 0;
			JInt32 NbReprises = 0;

			// on boucle sur tous les individus
			for (JInt32 JdxIndividu = 0; JdxIndividu < NbIndividus; JdxIndividu += 1)
			{
				// on teste si l'individu est actif
				if (ItemIndividus.GetAt(JdxIndividu))
				{
					// on sérialise la reprise de l'individu
					JInt32 Reprise; Stream.Recv(Reprise); if (Reprise < 1) return (false);

					// on mémorise la reprise
					if (JdxReprise < NbReprisesR) { ItemReprises.Item(JdxReprise) = Reprise; JdxReprise += 1; }

					// on incrémente le nombre de reprises
					NbReprises += 1;
				}
			}

			// on vérifie le nombre de reprises
			if (NbReprises != NbReprisesR) return (false);
		}
		else if (Type == 3)
		{
			// on boucle sur tous les masques des individus
			for (JInt32 IdxMasque = 0; IdxMasque < NbIndividus; IdxMasque += 32)
			{
				// on sérialise le masque et on met à jour le vecteur des individus
				JMsk32 Mask; Mask.Recv(Stream); ItemIndividus.SetMaskAt(IdxMasque, Mask);
			}

			// on initialise l'indice de la reprise et le nombre total
			JInt32 IdxReprise = 0;
			JInt32 NbReprises = 0;

			// on boucle sur tous les individus
			for (JInt32 IdxIndividu = 0; IdxIndividu < NbIndividus; IdxIndividu += 1)
			{
				// on teste si l'individu est actif
				if (ItemIndividus.GetAt(IdxIndividu))
				{
					// on mémorise la reprise
					if (IdxReprise < NbReprisesR) { ItemReprises.Item(IdxReprise) = Diviseur; IdxReprise += 1; }

					// on incrémente le nombre de reprises
					NbReprises += 1;
				}
			}

			// on vérifie le nombre de reprises
			if (NbReprises != NbReprisesR) return (false);
		}
		else
		{
			// on génère une exception
			throw JInternalError::GetInstance();
		}

		// on initialise la clé des reprises
		KEYReprise.m_IdTitre = IdTitre;

		// on teste si les reprises existent déjà
		TBLReprises.MoveTo(KEYReprise); if (TBLReprises.IsValid()) return (false);

		// on ajoute les nouvelles reprises
		JMTFR03DATReprise & DATReprise = TBLReprises.Add(KEYReprise);

		// on initialise les variables
		DATReprise.m_IdUnivers = IdUnivers;
		DATReprise.m_ItemIndividus.Swap(ItemIndividus);
		DATReprise.m_ItemReprises .Swap(ItemReprises);
		DATReprise.m_Diviseur = Diviseur;
	}

	// on quitte
	return (true);
}

JBool JMTFR03TBLTerrains::OnRecvVariablesAgregation(JStream & Stream, JInt32 NbIndividus, JMTFR03TBLVariables & TBLVariables) const
{
	// on sérialise le type de la variable
	JInt32 Type; Stream.Recv(Type); if (Type < 1 || Type > 3) return (false);

	// on sérialise le nombre de dimensions
	JInt32 NbDimensions; Stream.Recv(NbDimensions); if (NbDimensions != 1) return (false);

	// on sérialise l'identifiant de l'unique dimension
	JRef32 IdDimension; IdDimension.Recv(Stream); if (IdDimension.AsJInt32() != 1) return (false);

	// on sérialise le diviseur
	JInt32 Diviseur; Stream.Recv(Diviseur); if (Diviseur < 1) return (false);

	// on sérialise le nombre de titres
	JInt32 NbTitres; Stream.Recv(NbTitres); if (NbTitres < 0) return (false);

	// on boucle sur tous les supports
	for (JInt32 IdxTitre = 0; IdxTitre < NbTitres; IdxTitre += 1)
	{
		// on crée la clé
		JMTFR03KEYVariable KEYVariable;

		// on sérialise l'identifiant du titre
		JRef32 IdTitre; IdTitre.Recv(Stream); if (!IdTitre.IsValid()) return (false);

		// on sérialise l'identifiant de l'univers
		JRef32 IdUnivers; IdUnivers.Recv(Stream); if (!IdUnivers.IsValid()) return (false);

		// on sérialise le nombre de variables différentes de zéro
		JInt32 NbVariablesR; Stream.Recv(NbVariablesR); if (NbVariablesR < 0 || NbVariablesR > NbIndividus) return (false);

		// on alloue le vecteur des variables
		JArray<JInt32x> ItemVariables; ItemVariables.SetCount(NbVariablesR);

		// on alloue le vecteur des individus
		JBitArray ItemIndividus; ItemIndividus.SetCount(NbIndividus);

		// on teste le type de la variable
		if (Type == 1)
		{
			// on boucle sur tous les masques des individus
			for (JInt32 IdxMasque = 0; IdxMasque < NbIndividus; IdxMasque += 32)
			{
				// on sérialise le masque et on met à jour le vecteur des individus
				JMsk32 Mask; Mask.Recv(Stream); ItemIndividus.SetMaskAt(IdxMasque, Mask);
			}

			// on initialise l'indice de la variable et le nombre total
			JInt32 IdxVariable = 0;
			JInt32 NbVariables = 0;

			// on boucle sur tous les individus
			for (JInt32 IdxIndividu = 0; IdxIndividu < NbIndividus; IdxIndividu += 1)
			{
				// on teste si l'individu est actif
				if (ItemIndividus.GetAt(IdxIndividu))
				{
					// on sérialise la variable de l'individu
					JInt32 Variable; Stream.Recv(Variable); if (Variable < 0 || Variable > Diviseur) return (false);

					// on mémorise la variable
					if (IdxVariable < NbVariablesR) { ItemVariables.Item(IdxVariable) = Variable; IdxVariable += 1; }

					// on incrémente le nombre de variables
					NbVariables += 1;
				}
			}

			// on vérifie le nombre de variables
			if (NbVariables != NbVariablesR) return (false);
		}
		else if (Type == 2)
		{
			// on initialise le tri
			JInt32 SrtIndividu = -1;

			// on boucle sur toutes les variables
			for (JInt32 IdxVariable = 0; IdxVariable < NbVariablesR; IdxVariable += 1)
			{
				// on sérialise l'indice de l'individu
				JInt32 IdxIndividu; Stream.Recv(IdxIndividu); if (IdxIndividu <= SrtIndividu || IdxIndividu >= NbIndividus) return (false);

				// on positionne le masque de l'individu
				ItemIndividus.SetAt(IdxIndividu, true);

				// on met à jour le tri
				SrtIndividu = IdxIndividu;
			}

			// on initialise l'indice de la variable et le nombre total
			JInt32 JdxVariable = 0;
			JInt32 NbVariables = 0;

			// on boucle sur tous les individus
			for (JInt32 JdxIndividu = 0; JdxIndividu < NbIndividus; JdxIndividu += 1)
			{
				// on teste si l'individu est actif
				if (ItemIndividus.GetAt(JdxIndividu))
				{
					// on sérialise la variable de l'individu
					JInt32 Variable; Stream.Recv(Variable); if (Variable < 0 || Variable > Diviseur) return (false);

					// on mémorise la variable
					if (JdxVariable < NbVariablesR) { ItemVariables.Item(JdxVariable) = Variable; JdxVariable += 1; }

					// on incrémente le nombre de variables
					NbVariables += 1;
				}
			}

			// on vérifie le nombre de variables
			if (NbVariables != NbVariablesR) return (false);
		}
		else if (Type == 3)
		{
			// on boucle sur tous les masques des individus
			for (JInt32 IdxMasque = 0; IdxMasque < NbIndividus; IdxMasque += 32)
			{
				// on sérialise le masque et on met à jour le vecteur des individus
				JMsk32 Mask; Mask.Recv(Stream); ItemIndividus.SetMaskAt(IdxMasque, Mask);
			}

			// on initialise l'indice de la variable et le nombre total
			JInt32 IdxVariable = 0;
			JInt32 NbVariables = 0;

			// on boucle sur tous les individus
			for (JInt32 IdxIndividu = 0; IdxIndividu < NbIndividus; IdxIndividu += 1)
			{
				// on teste si l'individu est actif
				if (ItemIndividus.GetAt(IdxIndividu))
				{
					// on mémorise la variable
					if (IdxVariable < NbVariablesR) { ItemVariables.Item(IdxVariable) = Diviseur; IdxVariable += 1; }

					// on incrémente le nombre de variables
					NbVariables += 1;
				}
			}

			// on vérifie le nombre de variables
			if (NbVariables != NbVariablesR) return (false);
		}
		else
		{
			// on génère une exception
			throw JInternalError::GetInstance();
		}

		// on initialise la clé des variables
		KEYVariable.m_IdTitre = IdTitre;

		// on teste si les variables existent déjà
		TBLVariables.MoveTo(KEYVariable); if (TBLVariables.IsValid()) return (false);

		// on ajoute les nouvelles variables
		JMTFR03DATVariable & DATVariable = TBLVariables.Add(KEYVariable);

		// on initialise les variables
		DATVariable.m_IdUnivers = IdUnivers;
		DATVariable.m_ItemIndividus.Swap(ItemIndividus);
		DATVariable.m_ItemVariables.Swap(ItemVariables);
		DATVariable.m_Diviseur = Diviseur;
	}

	// on quitte
	return (true);
}

JBool JMTFR03TBLTerrains::OnRecvVariablesEtalonnage(JStream & Stream, JInt32 NbIndividus, JMTFR03TBLLectorats & TBLLectorats) const
{
	// on sérialise le type de la variable
	JInt32 Type; Stream.Recv(Type); if (Type < 1 || Type > 3) return (false);

	// on sérialise le nombre de dimensions
	JInt32 NbDimensions; Stream.Recv(NbDimensions); if (NbDimensions != 1) return (false);

	// on sérialise l'identifiant de l'unique dimension
	JRef32 IdDimension; IdDimension.Recv(Stream); if (IdDimension.AsJInt32() != 1) return (false);

	// on sérialise le diviseur
	JInt32 Diviseur; Stream.Recv(Diviseur); if (Diviseur < 1) return (false);

	// on sérialise le nombre de titres
	JInt32 NbTitres; Stream.Recv(NbTitres); if (NbTitres < 0) return (false);

	// on boucle sur tous les supports
	for (JInt32 IdxTitre = 0; IdxTitre < NbTitres; IdxTitre += 1)
	{
		// on crée la clé
		JMTFR03KEYLectorat KEYLectorat;

		// on sérialise l'identifiant du titre
		JRef32 IdTitre; IdTitre.Recv(Stream); if (!IdTitre.IsValid()) return (false);

		// on sérialise l'identifiant de l'univers
		JRef32 IdUnivers; IdUnivers.Recv(Stream); if (!IdUnivers.IsValid()) return (false);

		// on sérialise le nombre de variables différentes de zéro
		JInt32 NbVariablesR; Stream.Recv(NbVariablesR); if (NbVariablesR < 0 || NbVariablesR > NbIndividus) return (false);

		// on alloue le vecteur des individus
		JBitArray ItemIndividus; ItemIndividus.SetCount(NbIndividus);

		// on teste le type de la variable
		if (Type == 1)
		{
			// on boucle sur tous les masques des individus
			for (JInt32 IdxMasque = 0; IdxMasque < NbIndividus; IdxMasque += 32)
			{
				// on sérialise le masque et on met à jour le vecteur des individus
				JMsk32 Mask; Mask.Recv(Stream); ItemIndividus.SetMaskAt(IdxMasque, Mask);
			}

			// on initialise le nombre de variables
			JInt32 NbVariables = 0;

			// on boucle sur tous les individus
			for (JInt32 IdxIndividu = 0; IdxIndividu < NbIndividus; IdxIndividu += 1)
			{
				// on teste si l'individu est actif
				if (ItemIndividus.GetAt(IdxIndividu))
				{
					// on sérialise la variable de l'individu
					JInt32 Variable; Stream.Recv(Variable); if (Variable != Diviseur) return (false);

					// on incrémente le nombre de variables
					NbVariables += 1;
				}
			}

			// on vérifie le nombre de variables
			if (NbVariables != NbVariablesR) return (false);
		}
		else if (Type == 2)
		{
			// on initialise le tri
			JInt32 SrtIndividu = -1;

			// on boucle sur toutes les variables
			for (JInt32 IdxVariable = 0; IdxVariable < NbVariablesR; IdxVariable += 1)
			{
				// on sérialise l'indice de l'individu
				JInt32 IdxIndividu; Stream.Recv(IdxIndividu); if (IdxIndividu <= SrtIndividu || IdxIndividu >= NbIndividus) return (false);

				// on positionne le masque de l'individu
				ItemIndividus.SetAt(IdxIndividu, true);

				// on met à jour le tri
				SrtIndividu = IdxIndividu;
			}

			// on initialise le nombre de variables
			JInt32 NbVariables = 0;

			// on boucle sur tous les individus
			for (JInt32 JdxIndividu = 0; JdxIndividu < NbIndividus; JdxIndividu += 1)
			{
				// on teste si l'individu est actif
				if (ItemIndividus.GetAt(JdxIndividu))
				{
					// on sérialise la variable de l'individu
					JInt32 Variable; Stream.Recv(Variable); if (Variable != Diviseur) return (false);

					// on incrémente le nombre de variables
					NbVariables += 1;
				}
			}

			// on vérifie le nombre de variables
			if (NbVariables != NbVariablesR) return (false);
		}
		else if (Type == 3)
		{
			// on boucle sur tous les masques des individus
			for (JInt32 IdxMasque = 0; IdxMasque < NbIndividus; IdxMasque += 32)
			{
				// on sérialise le masque et on met à jour le vecteur des individus
				JMsk32 Mask; Mask.Recv(Stream); ItemIndividus.SetMaskAt(IdxMasque, Mask);
			}

			// on initialise le nombre de variables
			JInt32 NbVariables = 0;

			// on boucle sur tous les individus
			for (JInt32 IdxIndividu = 0; IdxIndividu < NbIndividus; IdxIndividu += 1)
			{
				// on teste si l'individu est actif
				if (ItemIndividus.GetAt(IdxIndividu))
				{
					// on incrémente le nombre de variables
					NbVariables += 1;
				}
			}

			// on vérifie le nombre de variables
			if (NbVariables != NbVariablesR) return (false);
		}
		else
		{
			// on génère une exception
			throw JInternalError::GetInstance();
		}

		// on initialise la clé des lectorats
		KEYLectorat.m_IdTitre = IdTitre;

		// on teste si les lectorats existent déjà
		TBLLectorats.MoveTo(KEYLectorat); if (TBLLectorats.IsValid()) return (false);

		// on ajoute les nouveaux lectorats
		JMTFR03DATLectorat & DATLectorat = TBLLectorats.Add(KEYLectorat);

		// on initialise le lectorat
		DATLectorat.m_ItemIndividus.Swap(ItemIndividus);
		DATLectorat.m_IdUnivers = IdUnivers;
	}

	// on quitte
	return (true);
}

JBool JMTFR03TBLTerrains::OnRecvAttributsTitres(JStream & Stream, JMTFR03TBLAttributs & TBLAttributs) const
{
	// on sérialise le nombre de dimensions
	JInt32 NbDimensions; Stream.Recv(NbDimensions); if (NbDimensions != 1) return (false);

	// on sérialise l'identifiant de l'unique dimension
	JRef32 IdDimension; IdDimension.Recv(Stream); if (IdDimension != (JRef32)1) return (false);

	// on sérialise le nombre de titres
	JInt32 NbTitres; Stream.Recv(NbTitres); if (NbTitres < 0) return (false);

	// on boucle sur tous les titres
	for (JInt32 IdxTitre = 0; IdxTitre < NbTitres; IdxTitre += 1)
	{
		// on crée la clé
		JMTFR03KEYAttribut KEYAttribut;

		// on sérialise l'identifiant du titre
		JRef32 IdTitre; IdTitre.Recv(Stream); if (!IdTitre.IsValid()) return (false);

		// on sérialise le nombre d'attributs
		JInt32 NbAttributs; Stream.Recv(NbAttributs); if (NbAttributs != 3) return (false);

		// on sérialise les éléments des attributs
		JRef32  IdEchantillon; IdEchantillon.Recv(Stream); if (!IdEchantillon.IsValid())         return (false);
		JRef32  IdType;        IdType       .Recv(Stream); if (!IdType       .IsValid())         return (false);
		JInt32x PtEtalonnage;  PtEtalonnage .Recv(Stream); if (!PtEtalonnage .IsBetween(0, 364)) return (false);

		// on initialise la clé des attributs
		KEYAttribut.m_IdTitre = IdTitre;

		// on teste si les attributs existent déjà
		TBLAttributs.MoveTo(KEYAttribut); if (TBLAttributs.IsValid()) return (false);

		// on ajoute les nouveaux attributs
		JMTFR03DATAttribut & DATAttribut = TBLAttributs.Add(KEYAttribut);

		// on initialise les attributs
		DATAttribut.m_IdEchantillon = IdEchantillon;
		DATAttribut.m_IdType        = IdType;
		DATAttribut.m_PtEtalonnage  = PtEtalonnage;
	}

	// on quitte
	return (true);
}

//////////////////////////////////////////////////
// la fonction pour récupérer le nombre d'éléments

JInt32 JMTFR03TBLTerrains::GetCount() const
{
	// on renvoie le nombre d'éléments
	return (m_TBLItems.GetCount());
}

////////////////////////////////////////
// la fonction pour ajouter les éléments

const JMTFR03DATTerrain & JMTFR03TBLTerrains::Add(const JMTFR03KEYTerrain & KEYTerrain, JBool Move)
{
	// on vérifie la validité de la clé
	if (!KEYTerrain.IsValid()) throw JInvalidCall::GetInstance();

	// on récupère les sources
	const JFR03Sources & FR03Sources = JFR03Sources::GetInstance();

	// on recherche l'indice du terrain
	JInt32 IdxTerrain = FR03Sources.m_TBLTerrains.FindItem(KEYTerrain.m_IdTerrain); if (IdxTerrain < 0) throw JInvalidCall::GetInstance();

	// on récupère le terrain
	const JFR03Terrain & FR03Terrain = FR03Sources.m_TBLTerrains.Item(IdxTerrain);

	// on vérifie la correspondance de la source
	if (KEYTerrain.m_IdSource != FR03Terrain.m_IdSource) throw JInvalidCall::GetInstance();

	// on recherche l'indice de la source
	JInt32 IdxSource = FR03Sources.m_TBLSources.FindItem(KEYTerrain.m_IdSource); if (IdxSource < 0) throw JInvalidCall::GetInstance();

	// on récupère la source
	const JFR03Source & FR03Source = FR03Sources.m_TBLSources.Item(IdxSource);

	// on crée les archives
	JArchive* pArchive_04       = 0;
	JArchive* pArchive_0A       = 0;
	JArchive* pArchive_09       = 0;
	JArchive* pArchive_0B       = 0;
	JArchive* pArchive_2A       = 0;
	JArchive* pArchive_08_5     = 0;
	JArchive* pArchive_08_6     = 0;
	JArchive* pArchive_08_7     = 0;
	JArchive* pArchive_08_8     = 0;
	JArchive* pArchive_08_12001 = 0;
	JArchive* pArchive_08_12XX1 = 0;
	JArchive* pArchive_08_12002 = 0;
	JArchive* pArchive_08_12004 = 0;
	JArchive* pArchive_08_12007 = 0;
	JArchive* pArchive_08_12014 = 0;
	JArchive* pArchive_08_12030 = 0;
	JArchive* pArchive_08_12060 = 0;
	JArchive* pArchive_08_12090 = 0;
	JArchive* pArchive_08_12120 = 0;
	JArchive* pArchive_08_12180 = 0;
	JArchive* pArchive_08_12270 = 0;
	JArchive* pArchive_08_12365 = 0;

	// on crée les éléments du terrain
	JMTFR03TBLFiltres       TBLFiltres;
	JMTFR03TBLSegments      TBLSegments;
	JMTFR03TBLUnivers       TBLUnivers;
	JMTFR03TBLEchantillons  TBLEchantillons;
	JMTFR03TBLHomotheties   TBLHomotheties;
	JMTFR03TBLPoids         TBLPoids;
	JMTFR03TBLPonderations  TBLPonderations;
	JMTFR03TBLReprises      TBLReprises;
	JMTFR03TBLVariables     TBLVariablesLDP;
	JMTFR03TBLVariables     TBLVariablesLNM;
	JMTFR03TBLVariables     TBLProbabilitesLDP;
	JMTFR03TBLVariables     TBLProbabilitesLNM;
	JMTFR03TBLLectorats     TBLVariablesDDL_001;
	JMTFR03TBLLectorats     TBLVariablesDDL_XX1;
	JMTFR03TBLLectorats     TBLVariablesDDL_002;
	JMTFR03TBLLectorats     TBLVariablesDDL_004;
	JMTFR03TBLLectorats     TBLVariablesDDL_007;
	JMTFR03TBLLectorats     TBLVariablesDDL_014;
	JMTFR03TBLLectorats     TBLVariablesDDL_030;
	JMTFR03TBLLectorats     TBLVariablesDDL_060;
	JMTFR03TBLLectorats     TBLVariablesDDL_090;
	JMTFR03TBLLectorats     TBLVariablesDDL_120;
	JMTFR03TBLLectorats     TBLVariablesDDL_180;
	JMTFR03TBLLectorats     TBLVariablesDDL_270;
	JMTFR03TBLLectorats     TBLVariablesDDL_365;
	JMTFR03TBLAttributs     TBLAttributs;

	// on crée les chemins du terrain
	JPath Path_04;       this->OnMakePath(FR03Terrain.m_Racine, g_Suffixe_04,       Path_04);
	JPath Path_0A;       this->OnMakePath(FR03Terrain.m_Racine, g_Suffixe_0A,       Path_0A);
	JPath Path_09;       this->OnMakePath(FR03Terrain.m_Racine, g_Suffixe_09,       Path_09);
	JPath Path_0B;       this->OnMakePath(FR03Terrain.m_Racine, g_Suffixe_0B,       Path_0B);
	JPath Path_2A;       this->OnMakePath(FR03Terrain.m_Racine, g_Suffixe_2A,       Path_2A);
	JPath Path_08_5;     this->OnMakePath(FR03Terrain.m_Racine, g_Suffixe_08_5,     Path_08_5);
	JPath Path_08_6;     this->OnMakePath(FR03Terrain.m_Racine, g_Suffixe_08_6,     Path_08_6);
	JPath Path_08_7;     this->OnMakePath(FR03Terrain.m_Racine, g_Suffixe_08_7,     Path_08_7);
	JPath Path_08_8;     this->OnMakePath(FR03Terrain.m_Racine, g_Suffixe_08_8,     Path_08_8);
	JPath Path_08_12001; this->OnMakePath(FR03Terrain.m_Racine, g_Suffixe_08_12001, Path_08_12001);
	JPath Path_08_12XX1; this->OnMakePath(FR03Terrain.m_Racine, g_Suffixe_08_12XX1, Path_08_12XX1);
	JPath Path_08_12002; this->OnMakePath(FR03Terrain.m_Racine, g_Suffixe_08_12002, Path_08_12002);
	JPath Path_08_12004; this->OnMakePath(FR03Terrain.m_Racine, g_Suffixe_08_12004, Path_08_12004);
	JPath Path_08_12007; this->OnMakePath(FR03Terrain.m_Racine, g_Suffixe_08_12007, Path_08_12007);
	JPath Path_08_12014; this->OnMakePath(FR03Terrain.m_Racine, g_Suffixe_08_12014, Path_08_12014);
	JPath Path_08_12030; this->OnMakePath(FR03Terrain.m_Racine, g_Suffixe_08_12030, Path_08_12030);
	JPath Path_08_12060; this->OnMakePath(FR03Terrain.m_Racine, g_Suffixe_08_12060, Path_08_12060);
	JPath Path_08_12090; this->OnMakePath(FR03Terrain.m_Racine, g_Suffixe_08_12090, Path_08_12090);
	JPath Path_08_12120; this->OnMakePath(FR03Terrain.m_Racine, g_Suffixe_08_12120, Path_08_12120);
	JPath Path_08_12180; this->OnMakePath(FR03Terrain.m_Racine, g_Suffixe_08_12180, Path_08_12180);
	JPath Path_08_12270; this->OnMakePath(FR03Terrain.m_Racine, g_Suffixe_08_12270, Path_08_12270);
	JPath Path_08_12365; this->OnMakePath(FR03Terrain.m_Racine, g_Suffixe_08_12365, Path_08_12365);

	// on teste les exceptions
	try
	{
		// on ouvre les archives
		pArchive_04 = JArchive::Create(Path_04);
		pArchive_0A = JArchive::Create(Path_0A);
		pArchive_09 = JArchive::Create(Path_09);
		pArchive_0B = JArchive::Create(Path_0B);

		// on ouvre les archives
		if (JPathManager::IsFileExists(Path_2A))       pArchive_2A       = JArchive::Create(Path_2A);
		if (JPathManager::IsFileExists(Path_08_5))     pArchive_08_5     = JArchive::Create(Path_08_5);
		if (JPathManager::IsFileExists(Path_08_6))     pArchive_08_6     = JArchive::Create(Path_08_6);
		if (JPathManager::IsFileExists(Path_08_7))     pArchive_08_7     = JArchive::Create(Path_08_7);
		if (JPathManager::IsFileExists(Path_08_8))     pArchive_08_8     = JArchive::Create(Path_08_8);
		if (JPathManager::IsFileExists(Path_08_12001)) pArchive_08_12001 = JArchive::Create(Path_08_12001);
		if (JPathManager::IsFileExists(Path_08_12XX1)) pArchive_08_12XX1 = JArchive::Create(Path_08_12XX1);
		if (JPathManager::IsFileExists(Path_08_12002)) pArchive_08_12002 = JArchive::Create(Path_08_12002);
		if (JPathManager::IsFileExists(Path_08_12004)) pArchive_08_12004 = JArchive::Create(Path_08_12004);
		if (JPathManager::IsFileExists(Path_08_12007)) pArchive_08_12007 = JArchive::Create(Path_08_12007);
		if (JPathManager::IsFileExists(Path_08_12014)) pArchive_08_12014 = JArchive::Create(Path_08_12014);
		if (JPathManager::IsFileExists(Path_08_12030)) pArchive_08_12030 = JArchive::Create(Path_08_12030);
		if (JPathManager::IsFileExists(Path_08_12060)) pArchive_08_12060 = JArchive::Create(Path_08_12060);
		if (JPathManager::IsFileExists(Path_08_12090)) pArchive_08_12090 = JArchive::Create(Path_08_12090);
		if (JPathManager::IsFileExists(Path_08_12120)) pArchive_08_12120 = JArchive::Create(Path_08_12120);
		if (JPathManager::IsFileExists(Path_08_12180)) pArchive_08_12180 = JArchive::Create(Path_08_12180);
		if (JPathManager::IsFileExists(Path_08_12270)) pArchive_08_12270 = JArchive::Create(Path_08_12270);
		if (JPathManager::IsFileExists(Path_08_12365)) pArchive_08_12365 = JArchive::Create(Path_08_12365);

		// on sérialise les entêtes des archives
		if (!this->OnRecvHeader(*pArchive_04, 0x6FD35904)) throw JBadSchema::GetInstance();
		if (!this->OnRecvHeader(*pArchive_0A, 0x6FD3590A)) throw JBadSchema::GetInstance();
		if (!this->OnRecvHeader(*pArchive_09, 0x6FD35909)) throw JBadSchema::GetInstance();
		if (!this->OnRecvHeader(*pArchive_0B, 0x6FD3590B)) throw JBadSchema::GetInstance();

		// on sérialise les entêtes des archives
		if (pArchive_2A       != 0) if (!this->OnRecvHeader(*pArchive_2A,       0x6FD3592A)) throw JBadSchema::GetInstance();
		if (pArchive_08_5     != 0) if (!this->OnRecvHeader(*pArchive_08_5,     0x6FD35908)) throw JBadSchema::GetInstance();
		if (pArchive_08_6     != 0) if (!this->OnRecvHeader(*pArchive_08_6,     0x6FD35908)) throw JBadSchema::GetInstance();
		if (pArchive_08_7     != 0) if (!this->OnRecvHeader(*pArchive_08_7,     0x6FD35908)) throw JBadSchema::GetInstance();
		if (pArchive_08_8     != 0) if (!this->OnRecvHeader(*pArchive_08_8,     0x6FD35908)) throw JBadSchema::GetInstance();
		if (pArchive_08_12001 != 0) if (!this->OnRecvHeader(*pArchive_08_12001, 0x6FD35908)) throw JBadSchema::GetInstance();
		if (pArchive_08_12XX1 != 0) if (!this->OnRecvHeader(*pArchive_08_12XX1, 0x6FD35908)) throw JBadSchema::GetInstance();
		if (pArchive_08_12002 != 0) if (!this->OnRecvHeader(*pArchive_08_12002, 0x6FD35908)) throw JBadSchema::GetInstance();
		if (pArchive_08_12004 != 0) if (!this->OnRecvHeader(*pArchive_08_12004, 0x6FD35908)) throw JBadSchema::GetInstance();
		if (pArchive_08_12007 != 0) if (!this->OnRecvHeader(*pArchive_08_12007, 0x6FD35908)) throw JBadSchema::GetInstance();
		if (pArchive_08_12014 != 0) if (!this->OnRecvHeader(*pArchive_08_12014, 0x6FD35908)) throw JBadSchema::GetInstance();
		if (pArchive_08_12030 != 0) if (!this->OnRecvHeader(*pArchive_08_12030, 0x6FD35908)) throw JBadSchema::GetInstance();
		if (pArchive_08_12060 != 0) if (!this->OnRecvHeader(*pArchive_08_12060, 0x6FD35908)) throw JBadSchema::GetInstance();
		if (pArchive_08_12090 != 0) if (!this->OnRecvHeader(*pArchive_08_12090, 0x6FD35908)) throw JBadSchema::GetInstance();
		if (pArchive_08_12120 != 0) if (!this->OnRecvHeader(*pArchive_08_12120, 0x6FD35908)) throw JBadSchema::GetInstance();
		if (pArchive_08_12180 != 0) if (!this->OnRecvHeader(*pArchive_08_12180, 0x6FD35908)) throw JBadSchema::GetInstance();
		if (pArchive_08_12270 != 0) if (!this->OnRecvHeader(*pArchive_08_12270, 0x6FD35908)) throw JBadSchema::GetInstance();
		if (pArchive_08_12365 != 0) if (!this->OnRecvHeader(*pArchive_08_12365, 0x6FD35908)) throw JBadSchema::GetInstance();

		// on sérialise les entêtes des sources
		if (!this->OnRecvSource(*pArchive_04, KEYTerrain.m_IdSource, KEYTerrain.m_IdTerrain)) throw JBadSchema::GetInstance();
		if (!this->OnRecvSource(*pArchive_0A, KEYTerrain.m_IdSource, KEYTerrain.m_IdTerrain)) throw JBadSchema::GetInstance();
		if (!this->OnRecvSource(*pArchive_09, KEYTerrain.m_IdSource, KEYTerrain.m_IdTerrain)) throw JBadSchema::GetInstance();
		if (!this->OnRecvSource(*pArchive_0B, KEYTerrain.m_IdSource, KEYTerrain.m_IdTerrain)) throw JBadSchema::GetInstance();

		// on sérialise les entêtes des sources
		if (pArchive_2A       != 0) if (!this->OnRecvSource(*pArchive_2A,       KEYTerrain.m_IdSource, KEYTerrain.m_IdTerrain)) throw JBadSchema::GetInstance();
		if (pArchive_08_5     != 0) if (!this->OnRecvSource(*pArchive_08_5,     KEYTerrain.m_IdSource, KEYTerrain.m_IdTerrain)) throw JBadSchema::GetInstance();
		if (pArchive_08_6     != 0) if (!this->OnRecvSource(*pArchive_08_6,     KEYTerrain.m_IdSource, KEYTerrain.m_IdTerrain)) throw JBadSchema::GetInstance();
		if (pArchive_08_7     != 0) if (!this->OnRecvSource(*pArchive_08_7,     KEYTerrain.m_IdSource, KEYTerrain.m_IdTerrain)) throw JBadSchema::GetInstance();
		if (pArchive_08_8     != 0) if (!this->OnRecvSource(*pArchive_08_8,     KEYTerrain.m_IdSource, KEYTerrain.m_IdTerrain)) throw JBadSchema::GetInstance();
		if (pArchive_08_12001 != 0) if (!this->OnRecvSource(*pArchive_08_12001, KEYTerrain.m_IdSource, KEYTerrain.m_IdTerrain)) throw JBadSchema::GetInstance();
		if (pArchive_08_12XX1 != 0) if (!this->OnRecvSource(*pArchive_08_12XX1, KEYTerrain.m_IdSource, KEYTerrain.m_IdTerrain)) throw JBadSchema::GetInstance();
		if (pArchive_08_12002 != 0) if (!this->OnRecvSource(*pArchive_08_12002, KEYTerrain.m_IdSource, KEYTerrain.m_IdTerrain)) throw JBadSchema::GetInstance();
		if (pArchive_08_12004 != 0) if (!this->OnRecvSource(*pArchive_08_12004, KEYTerrain.m_IdSource, KEYTerrain.m_IdTerrain)) throw JBadSchema::GetInstance();
		if (pArchive_08_12007 != 0) if (!this->OnRecvSource(*pArchive_08_12007, KEYTerrain.m_IdSource, KEYTerrain.m_IdTerrain)) throw JBadSchema::GetInstance();
		if (pArchive_08_12014 != 0) if (!this->OnRecvSource(*pArchive_08_12014, KEYTerrain.m_IdSource, KEYTerrain.m_IdTerrain)) throw JBadSchema::GetInstance();
		if (pArchive_08_12030 != 0) if (!this->OnRecvSource(*pArchive_08_12030, KEYTerrain.m_IdSource, KEYTerrain.m_IdTerrain)) throw JBadSchema::GetInstance();
		if (pArchive_08_12060 != 0) if (!this->OnRecvSource(*pArchive_08_12060, KEYTerrain.m_IdSource, KEYTerrain.m_IdTerrain)) throw JBadSchema::GetInstance();
		if (pArchive_08_12090 != 0) if (!this->OnRecvSource(*pArchive_08_12090, KEYTerrain.m_IdSource, KEYTerrain.m_IdTerrain)) throw JBadSchema::GetInstance();
		if (pArchive_08_12120 != 0) if (!this->OnRecvSource(*pArchive_08_12120, KEYTerrain.m_IdSource, KEYTerrain.m_IdTerrain)) throw JBadSchema::GetInstance();
		if (pArchive_08_12180 != 0) if (!this->OnRecvSource(*pArchive_08_12180, KEYTerrain.m_IdSource, KEYTerrain.m_IdTerrain)) throw JBadSchema::GetInstance();
		if (pArchive_08_12270 != 0) if (!this->OnRecvSource(*pArchive_08_12270, KEYTerrain.m_IdSource, KEYTerrain.m_IdTerrain)) throw JBadSchema::GetInstance();
		if (pArchive_08_12365 != 0) if (!this->OnRecvSource(*pArchive_08_12365, KEYTerrain.m_IdSource, KEYTerrain.m_IdTerrain)) throw JBadSchema::GetInstance();

		// on sérialise les nombres d'individus
		JInt32 NbIndividus_04; pArchive_04->Recv(NbIndividus_04); if (NbIndividus_04 <  0)              throw JBadSchema::GetInstance();
		JInt32 NbIndividus_0A; pArchive_0A->Recv(NbIndividus_0A); if (NbIndividus_0A != NbIndividus_04) throw JBadSchema::GetInstance();
		JInt32 NbIndividus_09; pArchive_09->Recv(NbIndividus_09); if (NbIndividus_09 != NbIndividus_04) throw JBadSchema::GetInstance();

		// on sérialise les nombres d'individus
		JInt32 NbIndividus_2A       = 0; if (pArchive_2A       != 0) { pArchive_2A      ->Recv(NbIndividus_2A);       if (NbIndividus_2A       != NbIndividus_04) throw JBadSchema::GetInstance(); }
		JInt32 NbIndividus_08_5     = 0; if (pArchive_08_5     != 0) { pArchive_08_5    ->Recv(NbIndividus_08_5);     if (NbIndividus_08_5     != NbIndividus_04) throw JBadSchema::GetInstance(); }
		JInt32 NbIndividus_08_6     = 0; if (pArchive_08_6     != 0) { pArchive_08_6    ->Recv(NbIndividus_08_6);     if (NbIndividus_08_6     != NbIndividus_04) throw JBadSchema::GetInstance(); }
		JInt32 NbIndividus_08_7     = 0; if (pArchive_08_7     != 0) { pArchive_08_7    ->Recv(NbIndividus_08_7);     if (NbIndividus_08_7     != NbIndividus_04) throw JBadSchema::GetInstance(); }
		JInt32 NbIndividus_08_8     = 0; if (pArchive_08_8     != 0) { pArchive_08_8    ->Recv(NbIndividus_08_8);     if (NbIndividus_08_8     != NbIndividus_04) throw JBadSchema::GetInstance(); }
		JInt32 NbIndividus_08_12001 = 0; if (pArchive_08_12001 != 0) { pArchive_08_12001->Recv(NbIndividus_08_12001); if (NbIndividus_08_12001 != NbIndividus_04) throw JBadSchema::GetInstance(); }
		JInt32 NbIndividus_08_12XX1 = 0; if (pArchive_08_12XX1 != 0) { pArchive_08_12XX1->Recv(NbIndividus_08_12XX1); if (NbIndividus_08_12XX1 != NbIndividus_04) throw JBadSchema::GetInstance(); }
		JInt32 NbIndividus_08_12002 = 0; if (pArchive_08_12002 != 0) { pArchive_08_12002->Recv(NbIndividus_08_12002); if (NbIndividus_08_12002 != NbIndividus_04) throw JBadSchema::GetInstance(); }
		JInt32 NbIndividus_08_12004 = 0; if (pArchive_08_12004 != 0) { pArchive_08_12004->Recv(NbIndividus_08_12004); if (NbIndividus_08_12004 != NbIndividus_04) throw JBadSchema::GetInstance(); }
		JInt32 NbIndividus_08_12007 = 0; if (pArchive_08_12007 != 0) { pArchive_08_12007->Recv(NbIndividus_08_12007); if (NbIndividus_08_12007 != NbIndividus_04) throw JBadSchema::GetInstance(); }
		JInt32 NbIndividus_08_12014 = 0; if (pArchive_08_12014 != 0) { pArchive_08_12014->Recv(NbIndividus_08_12014); if (NbIndividus_08_12014 != NbIndividus_04) throw JBadSchema::GetInstance(); }
		JInt32 NbIndividus_08_12030 = 0; if (pArchive_08_12030 != 0) { pArchive_08_12030->Recv(NbIndividus_08_12030); if (NbIndividus_08_12030 != NbIndividus_04) throw JBadSchema::GetInstance(); }
		JInt32 NbIndividus_08_12060 = 0; if (pArchive_08_12060 != 0) { pArchive_08_12060->Recv(NbIndividus_08_12060); if (NbIndividus_08_12060 != NbIndividus_04) throw JBadSchema::GetInstance(); }
		JInt32 NbIndividus_08_12090 = 0; if (pArchive_08_12090 != 0) { pArchive_08_12090->Recv(NbIndividus_08_12090); if (NbIndividus_08_12090 != NbIndividus_04) throw JBadSchema::GetInstance(); }
		JInt32 NbIndividus_08_12120 = 0; if (pArchive_08_12120 != 0) { pArchive_08_12120->Recv(NbIndividus_08_12120); if (NbIndividus_08_12120 != NbIndividus_04) throw JBadSchema::GetInstance(); }
		JInt32 NbIndividus_08_12180 = 0; if (pArchive_08_12180 != 0) { pArchive_08_12180->Recv(NbIndividus_08_12180); if (NbIndividus_08_12180 != NbIndividus_04) throw JBadSchema::GetInstance(); }
		JInt32 NbIndividus_08_12270 = 0; if (pArchive_08_12270 != 0) { pArchive_08_12270->Recv(NbIndividus_08_12270); if (NbIndividus_08_12270 != NbIndividus_04) throw JBadSchema::GetInstance(); }
		JInt32 NbIndividus_08_12365 = 0; if (pArchive_08_12365 != 0) { pArchive_08_12365->Recv(NbIndividus_08_12365); if (NbIndividus_08_12365 != NbIndividus_04) throw JBadSchema::GetInstance(); }
		
		// on sérialise les éléments du terrain
		if (!this->OnRecvFiltresAudience   (*pArchive_04, NbIndividus_04, TBLFiltres))                                            throw JBadSchema::GetInstance();
		if (!this->OnRecvSegmentsPopulation(*pArchive_0A, NbIndividus_0A, TBLSegments))                                           throw JBadSchema::GetInstance();
		if (!this->OnRecvPoidsIndividus    (*pArchive_09, NbIndividus_09, TBLUnivers, TBLEchantillons, TBLHomotheties, TBLPoids, TBLPonderations)) throw JBadSchema::GetInstance();
		if (!this->OnRecvAttributsTitres   (*pArchive_0B,                 TBLAttributs))                                          throw JBadSchema::GetInstance();

		// on sérialise les éléments du terrain
		if (pArchive_2A       != 0) if (!this->OnRecvReprisesMains      (*pArchive_2A,       NbIndividus_2A,       TBLReprises))         throw JBadSchema::GetInstance();
		if (pArchive_08_5     != 0) if (!this->OnRecvVariablesAgregation(*pArchive_08_5,     NbIndividus_08_5,     TBLVariablesLDP))     throw JBadSchema::GetInstance();
		if (pArchive_08_6     != 0) if (!this->OnRecvVariablesAgregation(*pArchive_08_6,     NbIndividus_08_6,     TBLVariablesLNM))     throw JBadSchema::GetInstance();
		if (pArchive_08_7     != 0) if (!this->OnRecvVariablesAgregation(*pArchive_08_7,     NbIndividus_08_7,     TBLProbabilitesLDP))  throw JBadSchema::GetInstance();
		if (pArchive_08_8     != 0) if (!this->OnRecvVariablesAgregation(*pArchive_08_8,     NbIndividus_08_8,     TBLProbabilitesLNM))  throw JBadSchema::GetInstance();
		if (pArchive_08_12001 != 0) if (!this->OnRecvVariablesEtalonnage(*pArchive_08_12001, NbIndividus_08_12001, TBLVariablesDDL_001)) throw JBadSchema::GetInstance();
		if (pArchive_08_12XX1 != 0) if (!this->OnRecvVariablesEtalonnage(*pArchive_08_12XX1, NbIndividus_08_12XX1, TBLVariablesDDL_XX1)) throw JBadSchema::GetInstance();
		if (pArchive_08_12002 != 0) if (!this->OnRecvVariablesEtalonnage(*pArchive_08_12002, NbIndividus_08_12002, TBLVariablesDDL_002)) throw JBadSchema::GetInstance();
		if (pArchive_08_12004 != 0) if (!this->OnRecvVariablesEtalonnage(*pArchive_08_12004, NbIndividus_08_12004, TBLVariablesDDL_004)) throw JBadSchema::GetInstance();
		if (pArchive_08_12007 != 0) if (!this->OnRecvVariablesEtalonnage(*pArchive_08_12007, NbIndividus_08_12007, TBLVariablesDDL_007)) throw JBadSchema::GetInstance();
		if (pArchive_08_12014 != 0) if (!this->OnRecvVariablesEtalonnage(*pArchive_08_12014, NbIndividus_08_12014, TBLVariablesDDL_014)) throw JBadSchema::GetInstance();
		if (pArchive_08_12030 != 0) if (!this->OnRecvVariablesEtalonnage(*pArchive_08_12030, NbIndividus_08_12030, TBLVariablesDDL_030)) throw JBadSchema::GetInstance();
		if (pArchive_08_12060 != 0) if (!this->OnRecvVariablesEtalonnage(*pArchive_08_12060, NbIndividus_08_12060, TBLVariablesDDL_060)) throw JBadSchema::GetInstance();
		if (pArchive_08_12090 != 0) if (!this->OnRecvVariablesEtalonnage(*pArchive_08_12090, NbIndividus_08_12090, TBLVariablesDDL_090)) throw JBadSchema::GetInstance();
		if (pArchive_08_12120 != 0) if (!this->OnRecvVariablesEtalonnage(*pArchive_08_12120, NbIndividus_08_12120, TBLVariablesDDL_120)) throw JBadSchema::GetInstance();
		if (pArchive_08_12180 != 0) if (!this->OnRecvVariablesEtalonnage(*pArchive_08_12180, NbIndividus_08_12180, TBLVariablesDDL_180)) throw JBadSchema::GetInstance();
		if (pArchive_08_12270 != 0) if (!this->OnRecvVariablesEtalonnage(*pArchive_08_12270, NbIndividus_08_12270, TBLVariablesDDL_270)) throw JBadSchema::GetInstance();
		if (pArchive_08_12365 != 0) if (!this->OnRecvVariablesEtalonnage(*pArchive_08_12365, NbIndividus_08_12365, TBLVariablesDDL_365)) throw JBadSchema::GetInstance();
	}
	catch (...)
	{
		// on libère les archives
		delete pArchive_08_12365;
		delete pArchive_08_12270;
		delete pArchive_08_12180;
		delete pArchive_08_12120;
		delete pArchive_08_12090;
		delete pArchive_08_12060;
		delete pArchive_08_12030;
		delete pArchive_08_12014;
		delete pArchive_08_12007;
		delete pArchive_08_12004;
		delete pArchive_08_12002;
		delete pArchive_08_12XX1;
		delete pArchive_08_12001;
		delete pArchive_08_8;
		delete pArchive_08_7;
		delete pArchive_08_6;
		delete pArchive_08_5;
		delete pArchive_2A;
		delete pArchive_0B;
		delete pArchive_09;
		delete pArchive_0A;
		delete pArchive_04;

		// on propage l'exception
		throw;
	}

	// on libère les archives
	delete pArchive_08_12365;
	delete pArchive_08_12270;
	delete pArchive_08_12180;
	delete pArchive_08_12120;
	delete pArchive_08_12090;
	delete pArchive_08_12060;
	delete pArchive_08_12030;
	delete pArchive_08_12014;
	delete pArchive_08_12007;
	delete pArchive_08_12004;
	delete pArchive_08_12002;
	delete pArchive_08_12XX1;
	delete pArchive_08_12001;
	delete pArchive_08_8;
	delete pArchive_08_7;
	delete pArchive_08_6;
	delete pArchive_08_5;
	delete pArchive_2A;
	delete pArchive_0B;
	delete pArchive_09;
	delete pArchive_0A;
	delete pArchive_04;

	// on ajoute le nouveau élément
	JMTFR03DATTerrain & DATTerrain = m_TBLItems.Add(KEYTerrain, Move);

	// on initialise l'élément
	DATTerrain.m_TBLFiltres         .Swap(TBLFiltres);
	DATTerrain.m_TBLSegments        .Swap(TBLSegments);
	DATTerrain.m_TBLUnivers         .Swap(TBLUnivers);
	DATTerrain.m_TBLEchantillons    .Swap(TBLEchantillons);
	DATTerrain.m_TBLHomotheties     .Swap(TBLHomotheties);
	DATTerrain.m_TBLPoids           .Swap(TBLPoids);
	DATTerrain.m_TBLPonderations    .Swap(TBLPonderations);
	DATTerrain.m_TBLReprises        .Swap(TBLReprises);
	DATTerrain.m_TBLVariablesLDP    .Swap(TBLVariablesLDP);
	DATTerrain.m_TBLVariablesLNM    .Swap(TBLVariablesLNM);
	DATTerrain.m_TBLProbabilitesLDP .Swap(TBLProbabilitesLDP);
	DATTerrain.m_TBLProbabilitesLNM .Swap(TBLProbabilitesLNM);
	DATTerrain.m_TBLVariablesDDL_001.Swap(TBLVariablesDDL_001);
	DATTerrain.m_TBLVariablesDDL_XX1.Swap(TBLVariablesDDL_XX1);
	DATTerrain.m_TBLVariablesDDL_002.Swap(TBLVariablesDDL_002);
	DATTerrain.m_TBLVariablesDDL_004.Swap(TBLVariablesDDL_004);
	DATTerrain.m_TBLVariablesDDL_007.Swap(TBLVariablesDDL_007);
	DATTerrain.m_TBLVariablesDDL_014.Swap(TBLVariablesDDL_014);
	DATTerrain.m_TBLVariablesDDL_030.Swap(TBLVariablesDDL_030);
	DATTerrain.m_TBLVariablesDDL_060.Swap(TBLVariablesDDL_060);
	DATTerrain.m_TBLVariablesDDL_090.Swap(TBLVariablesDDL_090);
	DATTerrain.m_TBLVariablesDDL_120.Swap(TBLVariablesDDL_120);
	DATTerrain.m_TBLVariablesDDL_180.Swap(TBLVariablesDDL_180);
	DATTerrain.m_TBLVariablesDDL_270.Swap(TBLVariablesDDL_270);
	DATTerrain.m_TBLVariablesDDL_365.Swap(TBLVariablesDDL_365);
	DATTerrain.m_TBLAttributs       .Swap(TBLAttributs);

	// on renvoie l'élément
	return (DATTerrain);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// les fonctions pour récupérer le tableau des pondérations du terrain
JMTFR03TBLPonderations & JMTFR03TBLTerrains::GetTblPonderations(JMTFR03KEYTerrain KEYTerrain)
{
	// on déplace le curseur
	m_TBLItems.MoveTo(KEYTerrain);

	// On récupère les datas
	if (m_TBLItems.IsValid())
	{
		JMTFR03DATTerrain & DATTerrain = m_TBLItems.GetItem();

		// Récupère la table des pondérations
		return (DATTerrain.m_TBLPonderations); 
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
// les fonctions pour récupérer le tableau des pondérations du terrain
const JMTFR03TBLPonderations & JMTFR03TBLTerrains::GetTblPonderations(const JMTFR03KEYTerrain KEYTerrain) const
{
	// on déplace le curseur
	m_TBLItems.MoveTo(KEYTerrain);

	// On récupère les datas
	if (m_TBLItems.IsValid())
	{
		const JMTFR03DATTerrain & DATTerrain = m_TBLItems.GetItem();

		// Récupère la table des pondérations
		return (DATTerrain.m_TBLPonderations); 
	}
}

/////////////////////////////////////
// la fonction pour tester le curseur

JBool JMTFR03TBLTerrains::IsValid() const
{
	// on renvoie l'état du curseur
	return (m_TBLItems.IsValid());
}

////////////////////////////////////////////////////
// les fonctions pour récupérer l'élément du curseur

const JMTFR03KEYTerrain & JMTFR03TBLTerrains::GetKey() const
{
	// on renvoie l'élément du curseur
	return (m_TBLItems.GetKey());
}

const JMTFR03DATTerrain & JMTFR03TBLTerrains::GetItem() const
{
	// on renvoie l'élément du curseur
	return (m_TBLItems.GetItem());
}

JMTFR03DATTerrain & JMTFR03TBLTerrains::GetItem()
{
	// on renvoie l'élément du curseur
	return (m_TBLItems.GetItem());
}

/////////////////////////////////////////
// les fonctions pour déplacer le curseur

JVoid JMTFR03TBLTerrains::MoveTo(const JMTFR03KEYTerrain & KEYTerrain, JInt32 Move) const
{
	// on vérifie la validité de l'élément
	if (!KEYTerrain.IsValid()) throw JInvalidCall::GetInstance();

	// on déplace le curseur
	m_TBLItems.MoveTo(KEYTerrain, Move);
}

JVoid JMTFR03TBLTerrains::MoveFirst() const
{
	// on déplace le curseur
	m_TBLItems.MoveFirst();
}

JVoid JMTFR03TBLTerrains::MoveNext() const
{
	// on déplace le curseur
	m_TBLItems.MoveNext();
}

JVoid JMTFR03TBLTerrains::MovePrev() const
{
	// on déplace le curseur
	m_TBLItems.MovePrev();
}

JVoid JMTFR03TBLTerrains::MoveLast() const
{
	// on déplace le curseur
	m_TBLItems.MoveLast();
}

//////////////////////////////////////////////////
// la fonction pour supprimer l'élément du curseur

JVoid JMTFR03TBLTerrains::Remove(JInt32 Move)
{
	// on supprime l'élément courant
	m_TBLItems.Remove(Move);
}

/////////////////////////////////////////
// la fonction pour permuter les éléments

JVoid JMTFR03TBLTerrains::Swap(JMTFR03TBLTerrains & Source)
{
	// on permute les éléments
	m_TBLItems.Swap(Source.m_TBLItems);
}

////////////////////////////////////////
// la fonction pour libérer les éléments

JVoid JMTFR03TBLTerrains::Reset()
{
	// on libère les éléments
	m_TBLItems.Reset();
}

/////////////////
// le destructeur

JMTFR03TBLTerrains::~JMTFR03TBLTerrains()
{
	// on ne fait rien
}
