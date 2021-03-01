//
// Fichier: JMTFR03BetaBinomial.cpp
// Auteur:  Sylvain SAMMURI
// Date:    01/12/2003
//

// on inclut les d�finitions n�cessaires
#include "JMTFR03BetaBinomial.h"

// on inclut les d�finitions n�cessaires
#include "JMTFR03Exception.h"

// on inclut les d�finitions n�cessaires
#include "JBetaBinomial.h"

// on inclut les d�finitions n�cessaires
#include "JMTResultats.h"

// on initialise la classe maximum des effectifs
static const JInt32 g_KMax = JDISTRIBUTION_MAX;

////////////////////
// les constructeurs

JMTFR03BetaBinomial::JMTFR03BetaBinomial()
{
	// on ne fait rien
}

JMTFR03BetaBinomial::JMTFR03BetaBinomial(const JMTFR03BetaBinomial & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'op�rateur pour recopier les �l�ments

JMTFR03BetaBinomial & JMTFR03BetaBinomial::operator =(const JMTFR03BetaBinomial & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////
// la fonction pour calculer le Lambda

JInt32 JMTFR03BetaBinomial::OnComputeLambda(const JMTTBLVolContacts & TBLVolContacts) const
{
	// on initialise le Fpu
	JFpu Fpu;

	// on initialise le Lambda
	JInt32 Lambda = 0;

	// on r�cup�re le nombre d'individus
	JInt32 NbIndividus = TBLVolContacts.GetCount();

	// on configure le Fpu
	Fpu.SetRoundingModeUp();

	// on boucle sur tous les individus
	for (JInt32 IdxIndividu = 0; IdxIndividu < NbIndividus; IdxIndividu += 1)
	{
		// on d�termine le volume de contacts
		JInt32 VolContacts = (JInt32)JMath::Round(TBLVolContacts.Item(IdxIndividu));

		// on met � jour le Lambda
		if (VolContacts > Lambda) Lambda = VolContacts;
	}

	// on alloue la table des classes de contacts
	JArray<JInt32> TBLClasses; TBLClasses.SetCount(Lambda);

	for (JInt32 IdxClasse = 0; IdxClasse < Lambda; IdxClasse += 1) TBLClasses.Item(IdxClasse) = 0;

	// on boucle sur tous les individus
	for (JInt32 JdxIndividu = 0; JdxIndividu < NbIndividus; JdxIndividu += 1)
	{
		// on d�termine le volume de contacts
		JInt32 VolContacts = (JInt32)JMath::Round(TBLVolContacts.Item(JdxIndividu));

		// on initialise la table des classes
		if (VolContacts > 0 && VolContacts <= Lambda) TBLClasses.Item(VolContacts - 1) += VolContacts;
	}

	// on configure le Fpu
	Fpu.SetRoundingModeNear();

	// on initialise le nombre de classes utiles et le nombre de classes � z�ro
	JInt32 NbZeros = 0;
	JInt32 NbUtils = 0;

	// on boucle sur toutes les classes
	for (JInt32 Kdx = Lambda; Kdx > 0; Kdx -= 1)
	{
		// on r�cup�re le nombre de cas de la classe
		JInt32 NbCas = TBLClasses.Item(Kdx - 1);

		// on teste si on a des individus
		if (NbCas > 0)
		{
			// on teste si on doit tronquer la distribution sur la classe
			if (NbUtils <= NbZeros) Lambda = Kdx;

			// on met � jour les nombres de classes
			NbUtils += 1; 
			NbZeros  = 0;
		}
		else
		{
			// on met � jour le nombre de classes
			NbZeros += 1;
		}
	}

	// on renvoie le lambda
	return (Lambda);
}

//////////////////////////////////////////
// la fonction pour calculer les r�sultats

JBool JMTFR03BetaBinomial::OnComputeResultats(const JMTFR03DATCible & DATCible, const JMTFR03TBLPonderations & TblPonderations, const JMTFR03DATModulation & DATModulation, const JMTFR03RequeteA & RequeteA, const JMTFR03Options & Options, JBool fgEtalonnage, JResultats & Resultats) const
{
	// on initialise les r�sultats
	Resultats.m_Effectif       = 0.0;
	Resultats.m_NbVariables    = 11;
	Resultats.m_VolLectures1Jr = 0.0;
	Resultats.m_VolLecturesPrb = 0.0;
	Resultats.m_Audience_Prb   = 0.0;
	Resultats.m_Audience_001   = 0.0; Resultats.m_Validite_001 = true;
	Resultats.m_Audience_002   = 0.0; Resultats.m_Validite_002 = true;
	Resultats.m_Audience_004   = 0.0; Resultats.m_Validite_004 = true;
	Resultats.m_Audience_007   = 0.0; Resultats.m_Validite_007 = true;
	Resultats.m_Audience_014   = 0.0; Resultats.m_Validite_014 = true;
	Resultats.m_Audience_030   = 0.0; Resultats.m_Validite_030 = true;
	Resultats.m_Audience_060   = 0.0; Resultats.m_Validite_060 = true;
	Resultats.m_Audience_090   = 0.0; Resultats.m_Validite_090 = true;
	Resultats.m_Audience_120   = 0.0; Resultats.m_Validite_120 = false; // on ne tient pas compte volontairement du point � 120 jours
	Resultats.m_Audience_180   = 0.0; Resultats.m_Validite_180 = false; // on ne tient pas compte volontairement du point � 180 jours
	Resultats.m_Audience_270   = 0.0; Resultats.m_Validite_270 = false; // on ne tient pas compte volontairement du point � 270 jours
	Resultats.m_Audience_365   = 0.0;
	Resultats.m_Lambda         = 0;

	// on r�cup�re la requ�te
	const JMTFR03RequeteA::JRequete & Requete = RequeteA.GetItems();

	// on r�cup�re le nombre d'individus
	JInt32 NbIndividus = Requete.m_ItemEffectif.GetCount();

	// on calcule l'effectif de r�f�rence
	JMTResultats EffectifRef; EffectifRef.SetItem(Requete.m_ItemEffectif, *Requete.m_pKEYPoids, *Requete.m_pDATPoids, *Requete.m_pTBLEchantillons, *Requete.m_pTBLHomotheties, DATCible, TblPonderations);

	// on teste si on a des parutions
	if (Requete.m_TBLParutions.GetCount() > 0)
	{
		// on initialise les volumes de lectures sans contexte
		JFlt64 VolLectures1Jr = 0.0;
		JFlt64 VolLecturesPrb = 0.0;

		// on cr�e la table des contacts de la maille
		JMTTBLVolContacts TBLVolContactsMaille; TBLVolContactsMaille.SetCount(NbIndividus);

		// on cr�e les audiences de la maille
		JMTTBLAudiences TBLAudiences_Prb; TBLAudiences_Prb.SetCount(NbIndividus);
		JMTTBLAudiences TBLAudiences_001; TBLAudiences_001.SetCount(NbIndividus);
		JMTTBLAudiences TBLAudiences_002; TBLAudiences_002.SetCount(NbIndividus);
		JMTTBLAudiences TBLAudiences_004; TBLAudiences_004.SetCount(NbIndividus);
		JMTTBLAudiences TBLAudiences_007; TBLAudiences_007.SetCount(NbIndividus);
		JMTTBLAudiences TBLAudiences_014; TBLAudiences_014.SetCount(NbIndividus);
		JMTTBLAudiences TBLAudiences_030; TBLAudiences_030.SetCount(NbIndividus);
		JMTTBLAudiences TBLAudiences_060; TBLAudiences_060.SetCount(NbIndividus);
		JMTTBLAudiences TBLAudiences_090; TBLAudiences_090.SetCount(NbIndividus);
		JMTTBLAudiences TBLAudiences_120; TBLAudiences_120.SetCount(NbIndividus);
		JMTTBLAudiences TBLAudiences_180; TBLAudiences_180.SetCount(NbIndividus);
		JMTTBLAudiences TBLAudiences_270; TBLAudiences_270.SetCount(NbIndividus);
		JMTTBLAudiences TBLAudiences_365; TBLAudiences_365.SetCount(NbIndividus);

		// on boucle sur toutes les parutions de la maille
		for (Requete.m_TBLParutions.MoveFirst(); Requete.m_TBLParutions.IsValid(); Requete.m_TBLParutions.MoveNext())
		{
			// on r�cup�re la parution de la requ�te
			const JMTFR03RequeteA::JParution & Parution = Requete.m_TBLParutions.GetItem();

			// on v�rifie la validit� de la probabilit� de r�f�rence
			if (Parution.m_pDATProbabilitesRef != 0)
			{
				// on incr�mente l'audience probabiliste
				TBLAudiences_Prb.AddItems(*Parution.m_pDATProbabilitesRef, 1);
			}
			else
			{
				JInt32 LOG_A_FAIRE;
			}

			if (Parution.m_pDATProbabilitesRef != 0 && Parution.m_pDATProbabilitesRef == Parution.m_pDATProbabilitesLNM)
			{
				if (Parution.m_PtEtalonnageRef > 0)
				{
					if (Resultats.m_Validite_001) TBLAudiences_001.AddItems(*Parution.m_pDATProbabilitesRef, Parution.m_PtEtalonnageRef,   1);
					if (Resultats.m_Validite_002) TBLAudiences_002.AddItems(*Parution.m_pDATProbabilitesRef, Parution.m_PtEtalonnageRef,   2);
					if (Resultats.m_Validite_004) TBLAudiences_004.AddItems(*Parution.m_pDATProbabilitesRef, Parution.m_PtEtalonnageRef,   4);
					if (Resultats.m_Validite_007) TBLAudiences_007.AddItems(*Parution.m_pDATProbabilitesRef, Parution.m_PtEtalonnageRef,   7);
					if (Resultats.m_Validite_014) TBLAudiences_014.AddItems(*Parution.m_pDATProbabilitesRef, Parution.m_PtEtalonnageRef,  14);
					if (Resultats.m_Validite_030) TBLAudiences_030.AddItems(*Parution.m_pDATProbabilitesRef, Parution.m_PtEtalonnageRef,  30);
					if (Resultats.m_Validite_060) TBLAudiences_060.AddItems(*Parution.m_pDATProbabilitesRef, Parution.m_PtEtalonnageRef,  60);
					if (Resultats.m_Validite_090) TBLAudiences_090.AddItems(*Parution.m_pDATProbabilitesRef, Parution.m_PtEtalonnageRef,  90);
					if (Resultats.m_Validite_120) TBLAudiences_120.AddItems(*Parution.m_pDATProbabilitesRef, Parution.m_PtEtalonnageRef, 120);
					if (Resultats.m_Validite_180) TBLAudiences_180.AddItems(*Parution.m_pDATProbabilitesRef, Parution.m_PtEtalonnageRef, 180);
					if (Resultats.m_Validite_270) TBLAudiences_270.AddItems(*Parution.m_pDATProbabilitesRef, Parution.m_PtEtalonnageRef, 270);

					TBLAudiences_365.AddItems(*Parution.m_pDATProbabilitesRef, Parution.m_PtEtalonnageRef, 365);
				}
				else
				{
					JInt32 LOG_A_FAIRE;
				}
			}
			else
			{
				// on v�rifie la validit� de la variable DDL � 365 jours
				if (Parution.m_pDATVariablesDDL_365 != 0)
				{
					// on incr�mente les audiences DDL interm�diaires de la maille
					if (Parution.m_pDATVariablesDDL_001 != 0 && Resultats.m_Validite_001) TBLAudiences_001.AddItems(*Parution.m_pDATVariablesDDL_001); else Resultats.m_Validite_001 = false;
					if (Parution.m_pDATVariablesDDL_002 != 0 && Resultats.m_Validite_002) TBLAudiences_002.AddItems(*Parution.m_pDATVariablesDDL_002); else Resultats.m_Validite_002 = false;
					if (Parution.m_pDATVariablesDDL_004 != 0 && Resultats.m_Validite_004) TBLAudiences_004.AddItems(*Parution.m_pDATVariablesDDL_004); else Resultats.m_Validite_004 = false;
					if (Parution.m_pDATVariablesDDL_007 != 0 && Resultats.m_Validite_007) TBLAudiences_007.AddItems(*Parution.m_pDATVariablesDDL_007); else Resultats.m_Validite_007 = false;
					if (Parution.m_pDATVariablesDDL_014 != 0 && Resultats.m_Validite_014) TBLAudiences_014.AddItems(*Parution.m_pDATVariablesDDL_014); else Resultats.m_Validite_014 = false;
					if (Parution.m_pDATVariablesDDL_030 != 0 && Resultats.m_Validite_030) TBLAudiences_030.AddItems(*Parution.m_pDATVariablesDDL_030); else Resultats.m_Validite_030 = false;
					if (Parution.m_pDATVariablesDDL_060 != 0 && Resultats.m_Validite_060) TBLAudiences_060.AddItems(*Parution.m_pDATVariablesDDL_060); else Resultats.m_Validite_060 = false;
					if (Parution.m_pDATVariablesDDL_090 != 0 && Resultats.m_Validite_090) TBLAudiences_090.AddItems(*Parution.m_pDATVariablesDDL_090); else Resultats.m_Validite_090 = false;
					if (Parution.m_pDATVariablesDDL_120 != 0 && Resultats.m_Validite_120) TBLAudiences_120.AddItems(*Parution.m_pDATVariablesDDL_120); else Resultats.m_Validite_120 = false;
					if (Parution.m_pDATVariablesDDL_180 != 0 && Resultats.m_Validite_180) TBLAudiences_180.AddItems(*Parution.m_pDATVariablesDDL_180); else Resultats.m_Validite_180 = false;
					if (Parution.m_pDATVariablesDDL_270 != 0 && Resultats.m_Validite_270) TBLAudiences_270.AddItems(*Parution.m_pDATVariablesDDL_270); else Resultats.m_Validite_270 = false;

					// on incr�mente l'audience DDL � 365 jours de la maille
					TBLAudiences_365.AddItems(*Parution.m_pDATVariablesDDL_365);
				}
				else
				{
					JInt32 LOG_A_FAIRE;
				}
			}

			// on teste si on doit prendre en compte les reprises en mains
			if (Options.GetReprises() && Parution.m_pDATReprises != 0 && Parution.m_pDATPoidsReprises != 0)
			{
				// on calcule l'effectif
				JMTResultats EffectifSup; EffectifSup.SetItem(Parution.m_ItemEffectif, *Parution.m_pDATPoidsReprises, *Requete.m_pDATEchantillon, *Parution.m_pDATHomothetieReprises, DATCible, TblPonderations);

				// on cr�e et on initialise la table des volumes de contacts
				JMTTBLVolContacts TBLVolContacts; TBLVolContacts.SetCount(NbIndividus); TBLVolContacts.AddItems(*Parution.m_pDATReprises, 1);

				// on calcule le volume de contacts
				JMTResultats VolContacts; VolContacts.SetItem(Parution.m_ItemAudience, *Parution.m_pDATPoidsReprises, *Requete.m_pDATEchantillon, *Parution.m_pDATHomothetieReprises, DATCible, TblPonderations, TBLVolContacts);
				
				// on cr�� l'audience de r�f�rence
				JMTResultats Audience;

				// on calcule l'audience de r�f�rence
				if (Parution.m_pDATVariablesRef != 0 && Parution.m_pDATPoidsVariablesRef != 0)
				{
					// on cr�e et on initialise l'audience de r�f�rence
					JMTTBLAudiences TBLAudiences; TBLAudiences.SetCount(NbIndividus); TBLAudiences.AddItems(*Parution.m_pDATVariablesRef, 1);

					// on calcule l'audience de r�f�rence
					Audience.SetItem(Parution.m_ItemAudience, *Parution.m_pDATPoidsVariablesRef, *Requete.m_pDATEchantillon, *Parution.m_pDATHomothetieVariablesRef, DATCible, TblPonderations, TBLAudiences);
				}

				// on v�rifie la validit� des effectifs
				if (EffectifRef.AsJFlt64() > 0.0 && EffectifSup.AsJFlt64() > 0.0)
				{
					// on calcule l'audience et le volume de lectures
					JFlt64 SupAudienceRef    = (Audience   .AsJFlt64() * EffectifRef.AsJFlt64()) / EffectifSup.AsJFlt64();
					JFlt64 SupVolLectures1Jr = (VolContacts.AsJFlt64() * EffectifRef.AsJFlt64()) / EffectifSup.AsJFlt64();

					// on incr�mente les volumes de contacts
					Resultats.m_VolLectures1Jr += SupVolLectures1Jr;
					Resultats.m_VolLecturesPrb += SupVolLectures1Jr * (JFlt64)Parution.m_PtEtalonnageRef * (JFlt64)Parution.m_NbInsertions;

					// on incr�mente les volumes de contacts
					VolLectures1Jr += SupVolLectures1Jr;
					VolLecturesPrb += SupVolLectures1Jr * (JFlt64)Parution.m_PtEtalonnageRef * (JFlt64)Parution.m_NbInsertions;
				}

				// on incr�mente le volume de contacts de la maille
				TBLVolContactsMaille.AddItems(*Parution.m_pDATReprises, 1);
			}
			else if (Parution.m_PtEtalonnageVol > 0 && Parution.m_pDATVariablesDDL_Vol != 0 && Parution.m_pDATPoidsVariablesDDLRef != 0)
			{
				// on calcule l'effectif
				JMTResultats EffectifSup; EffectifSup.SetItem(Parution.m_ItemEffectif, *Parution.m_pDATPoidsVariablesDDLRef, *Requete.m_pDATEchantillon, *Parution.m_pDATHomothetieVariablesDDLRef, DATCible, TblPonderations);

				// on cr�e et on initialise la table des volumes de contacts
				JMTTBLVolContacts TBLVolContacts; TBLVolContacts.SetCount(NbIndividus); TBLVolContacts.AddItems(*Parution.m_pDATVariablesDDL_Vol, Parution.m_PtEtalonnageVol);

				// on calcule les volumes de contacts
				JMTResultats VolContacts; VolContacts.SetItem(Parution.m_ItemAudience, *Parution.m_pDATPoidsVariablesDDLRef, *Requete.m_pDATEchantillon, *Parution.m_pDATHomothetieVariablesDDLRef, DATCible, TblPonderations, TBLVolContacts);

				// on cr�� l'audience de r�f�rence
				JMTResultats Audience;

				// on calcule l'audience de r�f�rence
				if (Parution.m_pDATVariablesRef != 0 && Parution.m_pDATPoidsVariablesRef != 0)
				{
					// on cr�e et on initialise l'audience de r�f�rence
					JMTTBLAudiences TBLAudiences; TBLAudiences.SetCount(NbIndividus); TBLAudiences.AddItems(*Parution.m_pDATVariablesRef, 1);

					// on calcule l'audience de r�f�rence
					Audience.SetItem(Parution.m_ItemAudience, *Parution.m_pDATPoidsVariablesRef, *Requete.m_pDATEchantillon, *Parution.m_pDATHomothetieVariablesRef, DATCible, TblPonderations, TBLAudiences);
				}

				// on v�rifie la validit� des effectifs
				if (EffectifRef.AsJFlt64() > 0.0 && EffectifSup.AsJFlt64() > 0.0)
				{
					// on calcule l'audience et le volume de lectures
					JFlt64 SupAudienceRef    = (Audience   .AsJFlt64() * EffectifRef.AsJFlt64()) / EffectifSup.AsJFlt64();
					JFlt64 SupVolLectures1Jr = (VolContacts.AsJFlt64() * EffectifRef.AsJFlt64()) / EffectifSup.AsJFlt64();

// Correctif des reprises en main < 1.0 d�sactiv� pour faire les tests de la nouvelle m�thode (cumul 3 vagues)
//
//					// on teste si on peut calculer le taux de reprises
//					if (Parution.m_PtEtalonnageRef > 1 && SupAudienceRef > 0.0 && SupVolLectures1Jr > 0.0 && Parution.m_PtEtalonnageVol > 0)
//					{
//						// on calcule le taux de reprise
//						JFlt64 TxReprise = (SupVolLectures1Jr * (JFlt64)Parution.m_PtEtalonnageRef) / (SupAudienceRef * (JFlt64)Parution.m_PtEtalonnageVol);
//
//						// on teste si le taux de reprise est en dessous du seuil de correction
//						if (TxReprise < 1.5)
//						{
//							// on corrige le taux de reprise
//							TxReprise = ((4.0 * TxReprise * TxReprise * TxReprise) / 27.0) + 1.0;
//
//							// on met � jour le volume de lecture du r�sultats
//							SupVolLectures1Jr = (SupAudienceRef * TxReprise) / (JFlt64)Parution.m_PtEtalonnageRef;
//						}
//					}

					// on incr�mente les volumes de contacts
					Resultats.m_VolLectures1Jr += SupVolLectures1Jr;
					Resultats.m_VolLecturesPrb += SupVolLectures1Jr * (JFlt64)Parution.m_PtEtalonnageRef * (JFlt64)Parution.m_NbInsertions;

					// on incr�mente les volumes de contacts
					VolLectures1Jr += SupVolLectures1Jr;
					VolLecturesPrb += SupVolLectures1Jr * (JFlt64)Parution.m_PtEtalonnageRef * (JFlt64)Parution.m_NbInsertions;
				}

				// on incr�mente le volume de contacts de la maille
				TBLVolContactsMaille.AddItems(*Parution.m_pDATVariablesDDL_Vol, Parution.m_PtEtalonnageVol);
			}
			else
			{
				JInt32 LOG_A_FAIRE;
			}
		}

		// on corrige le nombre de variables
		if (!Resultats.m_Validite_001) Resultats.m_NbVariables -= 1;
		if (!Resultats.m_Validite_002) Resultats.m_NbVariables -= 1;
		if (!Resultats.m_Validite_004) Resultats.m_NbVariables -= 1;
		if (!Resultats.m_Validite_007) Resultats.m_NbVariables -= 1;
		if (!Resultats.m_Validite_014) Resultats.m_NbVariables -= 1;
		if (!Resultats.m_Validite_030) Resultats.m_NbVariables -= 1;
		if (!Resultats.m_Validite_060) Resultats.m_NbVariables -= 1;
		if (!Resultats.m_Validite_090) Resultats.m_NbVariables -= 1;
		if (!Resultats.m_Validite_120) Resultats.m_NbVariables -= 1;
		if (!Resultats.m_Validite_180) Resultats.m_NbVariables -= 1;
		if (!Resultats.m_Validite_270) Resultats.m_NbVariables -= 1;

		// on v�rifie le nombre de variables
		if (Resultats.m_NbVariables < 2) { JInt32 LOG_A_FAIRE; return (false); }

		// on v�rifie la validit� de l'effectif de r�f�rence et du volume de lectures
		if (EffectifRef.AsJFlt64() > 0.0 && VolLectures1Jr > 0.0)
		{
			// on teste la validit� des poids des probabilit�s
			if (Requete.m_pDATPoidsProbabilitesRef != 0)
			{
				// on cr�e l'effectif
				JMTResultats Effectif; Effectif.SetItem(Requete.m_ItemEffectif, *Requete.m_pDATPoidsProbabilitesRef, *Requete.m_pDATEchantillon, *Requete.m_pDATHomothetieProbabilitesRef, DATCible, TblPonderations);

				// on teste l'effectif
				if (Effectif.AsJFlt64() > 0.0)
				{
					// on calcule l'audience probabiliste
					JMTResultats Resultats_Prb; Resultats_Prb.SetItem(Requete.m_ItemAudience, *Requete.m_pDATPoidsProbabilitesRef, *Requete.m_pDATEchantillon, *Requete.m_pDATHomothetieProbabilitesRef, DATCible, TblPonderations, TBLAudiences_Prb);

					// on met � jour le r�sultat
					Resultats.m_Audience_Prb = (Resultats_Prb.AsJFlt64() * EffectifRef.AsJFlt64() * Resultats.m_VolLectures1Jr) / (Effectif.AsJFlt64() * VolLectures1Jr);
				}
			}

			// on teste la validit� des poids de variables DDL
			if (Requete.m_pDATPoidsVariablesDDLRef != 0)
			{
				// on cr�e l'effectif
				JMTResultats Effectif; Effectif.SetItem(Requete.m_ItemEffectif, *Requete.m_pDATPoidsVariablesDDLRef, *Requete.m_pDATEchantillon, *Requete.m_pDATHomothetieVariablesDDLRef, DATCible, TblPonderations);

				// on teste l'effectif
				if (Effectif.AsJFlt64() > 0.0)
				{
					// on calcule les audiences DDL et on met � jour les r�sultats
					if (Resultats.m_Validite_001) { JMTResultats Resultats_001; Resultats_001.SetItem(Requete.m_ItemAudience, *Requete.m_pDATPoidsVariablesDDLRef, *Requete.m_pDATEchantillon, *Requete.m_pDATHomothetieVariablesDDLRef, DATCible, TblPonderations, TBLAudiences_001); Resultats.m_Audience_001 = (Resultats_001.AsJFlt64() * EffectifRef.AsJFlt64() * Resultats.m_VolLectures1Jr) / (Effectif.AsJFlt64() * VolLectures1Jr); }
					if (Resultats.m_Validite_002) { JMTResultats Resultats_002; Resultats_002.SetItem(Requete.m_ItemAudience, *Requete.m_pDATPoidsVariablesDDLRef, *Requete.m_pDATEchantillon, *Requete.m_pDATHomothetieVariablesDDLRef, DATCible, TblPonderations, TBLAudiences_002); Resultats.m_Audience_002 = (Resultats_002.AsJFlt64() * EffectifRef.AsJFlt64() * Resultats.m_VolLectures1Jr) / (Effectif.AsJFlt64() * VolLectures1Jr); }
					if (Resultats.m_Validite_004) { JMTResultats Resultats_004; Resultats_004.SetItem(Requete.m_ItemAudience, *Requete.m_pDATPoidsVariablesDDLRef, *Requete.m_pDATEchantillon, *Requete.m_pDATHomothetieVariablesDDLRef, DATCible, TblPonderations, TBLAudiences_004); Resultats.m_Audience_004 = (Resultats_004.AsJFlt64() * EffectifRef.AsJFlt64() * Resultats.m_VolLectures1Jr) / (Effectif.AsJFlt64() * VolLectures1Jr); }
					if (Resultats.m_Validite_007) { JMTResultats Resultats_007; Resultats_007.SetItem(Requete.m_ItemAudience, *Requete.m_pDATPoidsVariablesDDLRef, *Requete.m_pDATEchantillon, *Requete.m_pDATHomothetieVariablesDDLRef, DATCible, TblPonderations, TBLAudiences_007); Resultats.m_Audience_007 = (Resultats_007.AsJFlt64() * EffectifRef.AsJFlt64() * Resultats.m_VolLectures1Jr) / (Effectif.AsJFlt64() * VolLectures1Jr); }
					if (Resultats.m_Validite_014) { JMTResultats Resultats_014; Resultats_014.SetItem(Requete.m_ItemAudience, *Requete.m_pDATPoidsVariablesDDLRef, *Requete.m_pDATEchantillon, *Requete.m_pDATHomothetieVariablesDDLRef, DATCible, TblPonderations, TBLAudiences_014); Resultats.m_Audience_014 = (Resultats_014.AsJFlt64() * EffectifRef.AsJFlt64() * Resultats.m_VolLectures1Jr) / (Effectif.AsJFlt64() * VolLectures1Jr); }
					if (Resultats.m_Validite_030) { JMTResultats Resultats_030; Resultats_030.SetItem(Requete.m_ItemAudience, *Requete.m_pDATPoidsVariablesDDLRef, *Requete.m_pDATEchantillon, *Requete.m_pDATHomothetieVariablesDDLRef, DATCible, TblPonderations, TBLAudiences_030); Resultats.m_Audience_030 = (Resultats_030.AsJFlt64() * EffectifRef.AsJFlt64() * Resultats.m_VolLectures1Jr) / (Effectif.AsJFlt64() * VolLectures1Jr); }
					if (Resultats.m_Validite_060) { JMTResultats Resultats_060; Resultats_060.SetItem(Requete.m_ItemAudience, *Requete.m_pDATPoidsVariablesDDLRef, *Requete.m_pDATEchantillon, *Requete.m_pDATHomothetieVariablesDDLRef, DATCible, TblPonderations, TBLAudiences_060); Resultats.m_Audience_060 = (Resultats_060.AsJFlt64() * EffectifRef.AsJFlt64() * Resultats.m_VolLectures1Jr) / (Effectif.AsJFlt64() * VolLectures1Jr); }
					if (Resultats.m_Validite_090) { JMTResultats Resultats_090; Resultats_090.SetItem(Requete.m_ItemAudience, *Requete.m_pDATPoidsVariablesDDLRef, *Requete.m_pDATEchantillon, *Requete.m_pDATHomothetieVariablesDDLRef, DATCible, TblPonderations, TBLAudiences_090); Resultats.m_Audience_090 = (Resultats_090.AsJFlt64() * EffectifRef.AsJFlt64() * Resultats.m_VolLectures1Jr) / (Effectif.AsJFlt64() * VolLectures1Jr); }
					if (Resultats.m_Validite_120) { JMTResultats Resultats_120; Resultats_120.SetItem(Requete.m_ItemAudience, *Requete.m_pDATPoidsVariablesDDLRef, *Requete.m_pDATEchantillon, *Requete.m_pDATHomothetieVariablesDDLRef, DATCible, TblPonderations, TBLAudiences_120); Resultats.m_Audience_120 = (Resultats_120.AsJFlt64() * EffectifRef.AsJFlt64() * Resultats.m_VolLectures1Jr) / (Effectif.AsJFlt64() * VolLectures1Jr); }
					if (Resultats.m_Validite_180) { JMTResultats Resultats_180; Resultats_180.SetItem(Requete.m_ItemAudience, *Requete.m_pDATPoidsVariablesDDLRef, *Requete.m_pDATEchantillon, *Requete.m_pDATHomothetieVariablesDDLRef, DATCible, TblPonderations, TBLAudiences_180); Resultats.m_Audience_180 = (Resultats_180.AsJFlt64() * EffectifRef.AsJFlt64() * Resultats.m_VolLectures1Jr) / (Effectif.AsJFlt64() * VolLectures1Jr); }
					if (Resultats.m_Validite_270) { JMTResultats Resultats_270; Resultats_270.SetItem(Requete.m_ItemAudience, *Requete.m_pDATPoidsVariablesDDLRef, *Requete.m_pDATEchantillon, *Requete.m_pDATHomothetieVariablesDDLRef, DATCible, TblPonderations, TBLAudiences_270); Resultats.m_Audience_270 = (Resultats_270.AsJFlt64() * EffectifRef.AsJFlt64() * Resultats.m_VolLectures1Jr) / (Effectif.AsJFlt64() * VolLectures1Jr); }

					// on calcule l'audience DDL et on met � jour le r�sultat
					JMTResultats Resultats_365; Resultats_365.SetItem(Requete.m_ItemAudience, *Requete.m_pDATPoidsVariablesDDLRef, *Requete.m_pDATEchantillon, *Requete.m_pDATHomothetieVariablesDDLRef, DATCible, TblPonderations, TBLAudiences_365); Resultats.m_Audience_365 = (Resultats_365.AsJFlt64() * EffectifRef.AsJFlt64() * Resultats.m_VolLectures1Jr) / (Effectif.AsJFlt64() * VolLectures1Jr);
				}
			}
		}

		// on calcule le Lambda
		Resultats.m_Lambda = this->OnComputeLambda(TBLVolContactsMaille);
	}

	// on m�morise l'effectif
	Resultats.m_Effectif = EffectifRef.AsJFlt64();

	// on quitte
	return (true);
}

///////////////////////////////////////////
// la fonction pour calculer les param�tres

JBool JMTFR03BetaBinomial::OnComputeParametres(JResultats & Resultats, JBool fgEtalonnage) const
{
	// on initialise les param�tres Delta, Alpha, Beta, Prob et Disp
	Resultats.m_Delta = 0.0;
	Resultats.m_Alpha = 0.0;
	Resultats.m_Beta  = 0.0;
	Resultats.m_Prob  = 0.0;
	Resultats.m_Disp  = 0.0;

	// on teste le volume de lectures, l'audience � 365 jours et le Lambda
	if (Resultats.m_VolLectures1Jr > 0.0 && Resultats.m_VolLecturesPrb > 0.0 && Resultats.m_Audience_365 > 0.0 && Resultats.m_Lambda > 0)
	{
		// on initialise les mod�les th�oriques
		JBetaBinomial BetaBinomial_Prb;
		JBetaBinomial BetaBinomial_001;
		JBetaBinomial BetaBinomial_002;
		JBetaBinomial BetaBinomial_004;
		JBetaBinomial BetaBinomial_007;
		JBetaBinomial BetaBinomial_014;
		JBetaBinomial BetaBinomial_030;
		JBetaBinomial BetaBinomial_060;
		JBetaBinomial BetaBinomial_090;
		JBetaBinomial BetaBinomial_120;
		JBetaBinomial BetaBinomial_180;
		JBetaBinomial BetaBinomial_270;

		// on initialise l'indicateur
		JBool Terminate = false;

		// on initialise le nombre de tours
		JInt32 NbTours = 1;

		// on boucle tant qu'on n'a pas termin�
		while (!Terminate)
		{
			// on initialise les bornes
			JInt32 Frst = 1;
			JInt32 Last = 9;

			// on initialise le diviseur
			JInt32 Diviseur = 1;

			// on initialise la correction du lambda
			JInt32 DLambda = 0;

			// on calcule le GRP unitaire
			JFlt64 GRPu = Resultats.m_VolLectures1Jr / Resultats.m_Audience_365;

			// on calcule la probabilit�
			JFlt64 Prob = GRPu / (JFlt64)Resultats.m_Lambda; if (Prob >= 1.0) Prob = 0.999999;

			// on boucle jusqu'� obtenir une pr�cision suffisante
			for (JInt32 ItrDiviseur = 0; ItrDiviseur < 6; ItrDiviseur += 1)
			{
				// on calcule le diviseur
				Diviseur *= 10;

				// on initialise la dispersion
				JInt32 Disp = -1;

				// on initialise la distance quadratique
				JFlt64 Quadra = 0.0;

				// on boucle sur toutes les dispersions
				for (JInt32 ItrDisp = Frst; ItrDisp <= Last; ItrDisp += 1)
				{
					// on initialise le delta
					JFlt64 Delta = 0.0;

					// on calcule les param�tres Alpha et Beta
					JFlt64 Alpha = ((JFlt64)Diviseur * Prob) / (JFlt64)ItrDisp;
					JFlt64 Beta  = ((JFlt64)Diviseur * (1.0 - Prob)) / (JFlt64)ItrDisp;

					// on teste si on doit utiliser le point d'�talonnage probabiliste
					if (fgEtalonnage)
					{
						// on calcule la distribution th�orique
						JFlt64 Lambda = (Resultats.m_VolLecturesPrb / Resultats.m_Audience_365) / (1.0 - (Beta / (Alpha + Beta))); BetaBinomial_Prb.SetItems(Alpha, Beta, Lambda);

						// on calcule la diff�rence entre le mod�le et les valeurs empirique
						JFlt64 Dif = (Resultats.m_Audience_Prb / Resultats.m_Audience_365) - BetaBinomial_Prb.Item(1);

						// on met � jour le delta
						Delta += (Dif * Dif);
					}

					// on teste si on doit utiliser le point d'�talonnage au jour 001
					if (Resultats.m_Validite_001 && !fgEtalonnage)
					{
						// on calcule la distribution th�orique
						JFlt64 Lambda = (GRPu * 1.0) / (1.0 - (Beta / (Alpha + Beta))); BetaBinomial_001.SetItems(Alpha, Beta, Lambda);

						// on calcule la diff�rence entre le mod�le et les valeurs empirique
						JFlt64 Dif = (Resultats.m_Audience_001 / Resultats.m_Audience_365) - BetaBinomial_001.Item(1);

						// on met � jour le delta
						Delta += (Dif * Dif);
					}

					// on teste si on doit utiliser le point d'�talonnage au jour 002
					if (Resultats.m_Validite_002 && !fgEtalonnage)
					{
						// on calcule la distribution th�orique
						JFlt64 Lambda = (GRPu * 2.0) / (1.0 - (Beta / (Alpha + Beta))); BetaBinomial_002.SetItems(Alpha, Beta, Lambda);

						// on calcule la diff�rence entre le mod�le et les valeurs empirique
						JFlt64 Dif = (Resultats.m_Audience_002 / Resultats.m_Audience_365) - BetaBinomial_002.Item(1);

						// on met � jour le delta
						Delta += (Dif * Dif);
					}

					// on teste si on doit utiliser le point d'�talonnage au jour 004
					if (Resultats.m_Validite_004 && !fgEtalonnage)
					{
						// on calcule la distribution th�orique
						JFlt64 Lambda = (GRPu * 4.0) / (1.0 - (Beta / (Alpha + Beta))); BetaBinomial_004.SetItems(Alpha, Beta, Lambda);

						// on calcule la diff�rence entre le mod�le et les valeurs empirique
						JFlt64 Dif = (Resultats.m_Audience_004 / Resultats.m_Audience_365) - BetaBinomial_004.Item(1);

						// on met � jour le delta
						Delta += (Dif * Dif);
					}

					// on teste si on doit utiliser le point d'�talonnage au jour 007
					if (Resultats.m_Validite_007 && !fgEtalonnage)
					{
						// on calcule la distribution th�orique
						JFlt64 Lambda = (GRPu * 7.0) / (1.0 - (Beta / (Alpha + Beta))); BetaBinomial_007.SetItems(Alpha, Beta, Lambda);

						// on calcule la diff�rence entre le mod�le et les valeurs empirique
						JFlt64 Dif = (Resultats.m_Audience_007 / Resultats.m_Audience_365) - BetaBinomial_007.Item(1);

						// on met � jour le delta
						Delta += (Dif * Dif);
					}

					// on teste si on doit utiliser le point d'�talonnage au jour 015
					if (Resultats.m_Validite_014 && !fgEtalonnage)
					{
						// on calcule la distribution th�orique
						JFlt64 Lambda = (GRPu * 14.0) / (1.0 - (Beta / (Alpha + Beta))); BetaBinomial_014.SetItems(Alpha, Beta, Lambda);

						// on calcule la diff�rence entre le mod�le et les valeurs empirique
						JFlt64 Dif = (Resultats.m_Audience_014 / Resultats.m_Audience_365) - BetaBinomial_014.Item(1);

						// on met � jour le delta
						Delta += (Dif * Dif);
					}

					// on teste si on doit utiliser le point d'�talonnage au jour 030
					if (Resultats.m_Validite_030 && !fgEtalonnage)
					{
						// on calcule la distribution th�orique
						JFlt64 Lambda = (GRPu * 30.0) / (1.0 - (Beta / (Alpha + Beta))); BetaBinomial_030.SetItems(Alpha, Beta, Lambda);

						// on calcule la diff�rence entre le mod�le et les valeurs empirique
						JFlt64 Dif = (Resultats.m_Audience_030 / Resultats.m_Audience_365) - BetaBinomial_030.Item(1);

						// on met � jour le delta
						Delta += (Dif * Dif);
					}

					// on teste si on doit utiliser le point d'�talonnage au jour 060
					if (Resultats.m_Validite_060 && !fgEtalonnage)
					{
						// on calcule la distribution th�orique
						JFlt64 Lambda = (GRPu * 60.0) / (1.0 - (Beta / (Alpha + Beta))); BetaBinomial_060.SetItems(Alpha, Beta, Lambda);

						// on calcule la diff�rence entre le mod�le et les valeurs empirique
						JFlt64 Dif = (Resultats.m_Audience_060 / Resultats.m_Audience_365) - BetaBinomial_060.Item(1);

						// on met � jour le delta
						Delta += (Dif * Dif);
					}

					// on teste si on doit utiliser le point d'�talonnage au jour 090
					if (Resultats.m_Validite_090 && !fgEtalonnage)
					{
						// on calcule la distribution th�orique
						JFlt64 Lambda = (GRPu * 90.0) / (1.0 - (Beta / (Alpha + Beta))); BetaBinomial_090.SetItems(Alpha, Beta, Lambda);

						// on calcule la diff�rence entre le mod�le et les valeurs empirique
						JFlt64 Dif = (Resultats.m_Audience_090 / Resultats.m_Audience_365) - BetaBinomial_090.Item(1);

						// on met � jour le delta
						Delta += (Dif * Dif);
					}

					// on teste si on doit utiliser le point d'�talonnage au jour 120
					if (Resultats.m_Validite_120 && !fgEtalonnage)
					{
						// on calcule la distribution th�orique
						JFlt64 Lambda = (GRPu * 120.0) / (1.0 - (Beta / (Alpha + Beta))); BetaBinomial_120.SetItems(Alpha, Beta, Lambda);

						// on calcule la diff�rence entre le mod�le et les valeurs empirique
						JFlt64 Dif = (Resultats.m_Audience_120 / Resultats.m_Audience_365) - BetaBinomial_120.Item(1);

						// on met � jour le delta
						Delta += (Dif * Dif);
					}

					// on teste si on doit utiliser le point d'�talonnage au jour 180
					if (Resultats.m_Validite_180 && !fgEtalonnage)
					{
						// on calcule la distribution th�orique
						JFlt64 Lambda = (GRPu * 180.0) / (1.0 - (Beta / (Alpha + Beta))); BetaBinomial_180.SetItems(Alpha, Beta, Lambda);

						// on calcule la diff�rence entre le mod�le et les valeurs empirique
						JFlt64 Dif = (Resultats.m_Audience_180 / Resultats.m_Audience_365) - BetaBinomial_180.Item(1);

						// on met � jour le delta
						Delta += (Dif * Dif);
					}

					// on teste si on doit utiliser le point d'�talonnage au jour 270
					if (Resultats.m_Validite_270 && !fgEtalonnage)
					{
						// on calcule la distribution th�orique
						JFlt64 Lambda = (GRPu * 270.0) / (1.0 - (Beta / (Alpha + Beta))); BetaBinomial_270.SetItems(Alpha, Beta, Lambda);

						// on calcule la diff�rence entre le mod�le et les valeurs empirique
						JFlt64 Dif = (Resultats.m_Audience_270 / Resultats.m_Audience_365) - BetaBinomial_270.Item(1);

						// on met � jour le delta
						Delta += (Dif * Dif);
					}

					// on teste si on doit garder les param�tres courants
					if (Disp < 0 || Delta < Quadra)
					{
						// on m�morise la dispersion et la distance quadratique
						Disp = ItrDisp; Quadra = Delta;

						// on m�morise les param�tres
						Resultats.m_Delta = Delta;
						Resultats.m_Alpha = Alpha;
						Resultats.m_Beta  = Beta;
						Resultats.m_Prob  = Prob;
						Resultats.m_Disp  = (JFlt64)Disp / (JFlt64)Diviseur;
					}
				}

				// on calcule les nouvelles bornes
				if (Disp == Frst) { Frst = (Disp * 10) - 9; DLambda = -1; } else { Frst = (Disp * 10) - 5; DLambda = 0; }
				if (Disp == Last) { Last = (Disp * 10) + 9; DLambda = +1; } else { Last = (Disp * 10) + 5; DLambda = 0; }
			}

			// on teste si on doit s'arr�ter ou corriger le Lambda
			if      (DLambda > 0) { if (NbTours < 3) { JInt32 Delta = (Resultats.m_Lambda > 1) ? (Resultats.m_Lambda + 2) / 4 : 1; Resultats.m_Lambda += Delta; NbTours += 1; } else { Terminate = true; } }
			else if (DLambda < 0) { Terminate = true; }
			else                  { Terminate = true; }
		}
	}

	// on quitte
	return (true);
}

////////////////////////////////////////
// la fonction pour d�finir les �l�ments

JVoid JMTFR03BetaBinomial::SetItems(const JMTFR03KEYCible & KEYCible, const JMTFR03TBLTerrains & TBLTerrains, const JMTFR03TBLModulations & TBLModulations, const JMTFR03TBLCibles & TBLCibles, const JMTFR03Options & Options, const JMTFR03Maille & Maille, JBool fgEtalonnage, JMTFR03Logs* pLogs)
{
	// on v�rifie la validit� de la cl� et des options
	if (!KEYCible.IsValid() || !Options.IsValid()) throw JInvalidCall::GetInstance();

	// on recherche la cible
	TBLCibles.MoveTo(KEYCible); if (!TBLCibles.IsValid()) throw JInvalidCall::GetInstance();

	// on r�cup�re la cible
	const JMTFR03DATCible & DATCible = TBLCibles.GetItem();

	// on cr�e la cl� des modulations
	JMTFR03KEYModulation KEYModulation;

	// on initialise la cl� des modulations
	KEYModulation.m_IdSource  = KEYCible.m_IdSource;
	KEYModulation.m_IdTerrain = KEYCible.m_IdTerrain;

	// on r�cup�re les pond�rations
	JMTFR03KEYTerrain KEYTerrain;
	KEYTerrain.m_IdSource  = KEYCible.m_IdSource;
	KEYTerrain.m_IdTerrain = KEYCible.m_IdTerrain;
	const JMTFR03TBLPonderations & TblPonderations = TBLTerrains.GetTblPonderations(KEYTerrain);

	// on recherche les modulations
	TBLModulations.MoveTo(KEYModulation); if (!TBLModulations.IsValid()) throw JInvalidCall::GetInstance();

	// on r�cup�re les modulations
	const JMTFR03DATModulation & DATModulation = TBLModulations.GetItem();

	// on cr�e et on construit la requ�te
	JMTFR03RequeteA RequeteA; RequeteA.SetItems(TBLTerrains, TBLModulations, KEYCible.m_IdSource, KEYCible.m_IdTerrain, Options, Maille);

	// on cr�e les nouveaux �l�ments
	JArray<JResultats> TBLItems; TBLItems.SetCount(1);

	// on calcule les r�sultats
	if (!this->OnComputeResultats(DATCible, TblPonderations, DATModulation, RequeteA, Options, fgEtalonnage, TBLItems.Item(0))) throw JMTFR03Exception::GetInstance();

	// on calcule les param�tres
	if (!this->OnComputeParametres(TBLItems.Item(0), fgEtalonnage)) throw JMTFR03Exception::GetInstance();

	// on permute les �l�ments
	m_TBLItems.Swap(TBLItems);
}

////////////////////////////////////////////
// la fonction pour calculer la distribution

JVoid JMTFR03BetaBinomial::MakeDistribution(JFlt64 VolLectures, JDistribution & Distribution) const
{
	// on v�rifie le volume de lectures
	if (VolLectures < 0.0) throw JInvalidCall::GetInstance();

	// on v�rifie l'existence des r�sultats
	if (m_TBLItems.GetCount() > 0)
	{
		// on r�cup�re les r�sultats
		const JResultats & Resultats = m_TBLItems.Item(0);

		// on v�rifie les param�tres des r�sultats
		if (Resultats.m_Alpha > 0.0 && Resultats.m_Beta > 0.0)
		{
			// on calcule le param�tre Lambda du GRP
			JFlt64 Lambda = VolLectures / (Resultats.m_Audience_365 * (1.0 - (Resultats.m_Beta / (Resultats.m_Alpha + Resultats.m_Beta))));

			// on calcule le mod�le th�orique pour Lambda
			JBetaBinomial BetaBinomial; BetaBinomial.SetItems(Resultats.m_Alpha, Resultats.m_Beta, Lambda);

			// on initialise la distribution
			for (JInt32 Idx = 0; Idx < g_KMax; Idx += 1) Distribution.m_Items[Idx] = BetaBinomial.Item(Idx + 1) * Resultats.m_Audience_365;
		}
		else
		{
			// on r�initialise la distribution
			for (JInt32 Idx = 0; Idx < g_KMax; Idx += 1) Distribution.m_Items[Idx] = 0.0;
		}
	}
	else
	{
		// on r�initialise la distribution
		for (JInt32 Idx = 0; Idx < g_KMax; Idx += 1) Distribution.m_Items[Idx] = 0.0;
	}
}

/////////////////////////////////////////
// la fonction pour permuter les �l�ments

JVoid JMTFR03BetaBinomial::Swap(JMTFR03BetaBinomial & Source)
{
	// on permute les �l�ments
	m_TBLItems.Swap(Source.m_TBLItems);
}

////////////////////////////////////////
// la fonction pour lib�rer les �l�ments

JVoid JMTFR03BetaBinomial::Reset()
{
	// on lib�re les �l�ments
	m_TBLItems.Reset();
}

/////////////////
// le destructeur

JMTFR03BetaBinomial::~JMTFR03BetaBinomial()
{
	// on ne fait rien
}
