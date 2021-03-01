//
// Fichier: JMTFR03RequeteA.h
// Auteur:  Sylvain SAMMURI
// Date:    21/06/2004
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMTFR03_REQUETE_AUDIENCE_H

// on définit la macro pour éviter les inclusions multiples
#define JMTFR03_REQUETE_AUDIENCE_H

// on inclut les définitions nécessaires
#include "JMTFR03TBLTerrains.h"
#include "JMTFR03TBLModulations.h"

// on inclut les définitions nécessaires
#include "JMTFR03Options.h"
#include "JMTFR03Maille.h"

class JMTFR03RequeteA  
{
public:
	// on définit les parutions
	class JParution
	{
	public:
		JInt32 m_Type;            // le type du titre
		JInt32 m_IdTitre;         // l'identifiant du titre
		JInt32 m_IdTuteur;        // l'identifiant du tuteur
		JInt32 m_NbInsertions;    // le nombre d'insertions
		JFlt64 m_GRPRef;          // le GRP de référence du titre
		JInt32 m_PtEtalonnageVol; // le point d'étalonnage du volume de contacts
		JInt32 m_PtEtalonnageRef; // le point d'étalonnage de l'audience de référence

	public:
		JBitArray m_ItemAudience; // l'item de l'audience
		JBitArray m_ItemEffectif; // l'item de l'effectif

	public:
		const JMTFR03KEYPoids*  m_pKEYPoidsVariablesRef;    // les poids des variables de référence
		const JMTFR03DATPoids*  m_pDATPoidsVariablesRef;    // les poids des variables de référence
		const JMTFR03KEYPoids*  m_pKEYPoidsProbabilitesRef; // les poids des probabilités de référence
		const JMTFR03DATPoids*  m_pDATPoidsProbabilitesRef; // les poids des probabilités de référence
		const JMTFR03KEYPoids*  m_pKEYPoidsVariablesDDLRef; // les poids des variables DDL de référence
		const JMTFR03DATPoids*  m_pDATPoidsVariablesDDLRef; // les poids des variables DDL de référence
		const JMTFR03KEYPoids*  m_pKEYPoidsReprises;        // les poids des reprises
		const JMTFR03DATPoids*  m_pDATPoidsReprises;        // les poids des reprises

	public:
		const JMTFR03KEYHomothetie*  m_pKEYHomothetieVariablesRef;    // l'homothétie des variables de référence
		const JMTFR03DATHomothetie*  m_pDATHomothetieVariablesRef;    // l'homothétie des variables de référence
		const JMTFR03KEYHomothetie*  m_pKEYHomothetieProbabilitesRef; // l'homothétie des probabilités de référence
		const JMTFR03DATHomothetie*  m_pDATHomothetieProbabilitesRef; // l'homothétie des probabilités de référence
		const JMTFR03KEYHomothetie*  m_pKEYHomothetieVariablesDDLRef; // l'homothétie des variables DDL de référence
		const JMTFR03DATHomothetie*  m_pDATHomothetieVariablesDDLRef; // l'homothétie des variables DDL de référence
		const JMTFR03KEYHomothetie*  m_pKEYHomothetieReprises;        // l'homothétie des reprises
		const JMTFR03DATHomothetie*  m_pDATHomothetieReprises;        // l'homothétie des reprises

	public:
		const JMTFR03DATVariable* m_pDATVariablesRef; // les variables de référence (LDP ou LNM)
		const JMTFR03DATVariable* m_pDATVariablesLDP; // les variables LDP
		const JMTFR03DATVariable* m_pDATVariablesLNM; // les variables LNM

	public:
		const JMTFR03DATVariable* m_pDATProbabilitesRef; // les probabilités de référence (LDP ou LNM)
		const JMTFR03DATVariable* m_pDATProbabilitesLDP; // les probabilités LDP
		const JMTFR03DATVariable* m_pDATProbabilitesLNM; // les probabilités LNM

	public:
		const JMTFR03DATLectorat* m_pDATVariablesDDL_Vol; // les variables DDL du volume de contacts
		const JMTFR03DATLectorat* m_pDATVariablesDDL_Ref; // les variables DDL de l'audience de référence
		const JMTFR03DATLectorat* m_pDATVariablesDDL_001; // les variables DDL de l'audience à   1 jour
		const JMTFR03DATLectorat* m_pDATVariablesDDL_XX1; // les variables DDL de l'audience à   1 jour bis
		const JMTFR03DATLectorat* m_pDATVariablesDDL_002; // les variables DDL de l'audience à   2 jours
		const JMTFR03DATLectorat* m_pDATVariablesDDL_004; // les variables DDL de l'audience à   4 jours
		const JMTFR03DATLectorat* m_pDATVariablesDDL_007; // les variables DDL de l'audience à   7 jours
		const JMTFR03DATLectorat* m_pDATVariablesDDL_014; // les variables DDL de l'audience à  14 jours
		const JMTFR03DATLectorat* m_pDATVariablesDDL_030; // les variables DDL de l'audience à  30 jours
		const JMTFR03DATLectorat* m_pDATVariablesDDL_060; // les variables DDL de l'audience à  60 jours
		const JMTFR03DATLectorat* m_pDATVariablesDDL_090; // les variables DDL de l'audience à  90 jours
		const JMTFR03DATLectorat* m_pDATVariablesDDL_120; // les variables DDL de l'audience à 120 jours
		const JMTFR03DATLectorat* m_pDATVariablesDDL_180; // les variables DDL de l'audience à 180 jours
		const JMTFR03DATLectorat* m_pDATVariablesDDL_270; // les variables DDL de l'audience à 270 jours
		const JMTFR03DATLectorat* m_pDATVariablesDDL_365; // les variables DDL de l'audience à 365 jours

	public:
		const JMTFR03DATReprise* m_pDATReprises; // les reprises en mains
	};

public:
	// on définit la requête
	class JRequete
	{
	public:
		JList<JParution> m_TBLParutions; // la table des parutions

	public:
		const JMTFR03KEYPoids* m_pKEYPoids; // les poids de l'univers par défaut
		const JMTFR03DATPoids* m_pDATPoids; // les poids de l'univers par défaut

	public:
		const JMTFR03TBLEchantillons* m_pTBLEchantillons; // la table des échantillons
		const JMTFR03TBLHomotheties*  m_pTBLHomotheties;  // la table des homothéties

	public:
		const JMTFR03KEYPoids*  m_pKEYPoidsVariablesRef;    // les poids des variables de référence
		const JMTFR03DATPoids*  m_pDATPoidsVariablesRef;    // les poids des variables de référence
		const JMTFR03KEYPoids*  m_pKEYPoidsProbabilitesRef; // les poids des probabilités de référence
		const JMTFR03DATPoids*  m_pDATPoidsProbabilitesRef; // les poids des probabilités de référence
		const JMTFR03KEYPoids*  m_pKEYPoidsVariablesDDLRef; // les poids des variables DDL de référence
		const JMTFR03DATPoids*  m_pDATPoidsVariablesDDLRef; // les poids des variables DDL de référence
		const JMTFR03KEYPoids*  m_pKEYPoidsReprises;        // les poids des reprises
		const JMTFR03DATPoids*  m_pDATPoidsReprises;        // les poids des reprises

	public:
		const JMTFR03KEYHomothetie*  m_pKEYHomothetieVariablesRef;    // l'homothétie des variables de référence
		const JMTFR03DATHomothetie*  m_pDATHomothetieVariablesRef;    // l'homothétie des variables de référence
		const JMTFR03KEYHomothetie*  m_pKEYHomothetieProbabilitesRef; // l'homothétie des probabilités de référence
		const JMTFR03DATHomothetie*  m_pDATHomothetieProbabilitesRef; // l'homothétie des probabilités de référence
		const JMTFR03KEYHomothetie*  m_pKEYHomothetieVariablesDDLRef; // l'homothétie des variables DDL de référence
		const JMTFR03DATHomothetie*  m_pDATHomothetieVariablesDDLRef; // l'homothétie des variables DDL de référence
		const JMTFR03KEYHomothetie*  m_pKEYHomothetieReprises;        // l'homothétie des reprises
		const JMTFR03DATHomothetie*  m_pDATHomothetieReprises;        // l'homothétie des reprises

	public:
		const JMTFR03KEYEchantillon* m_pKEYEchantillon; // l'échantillon commun de la maille
		const JMTFR03DATEchantillon* m_pDATEchantillon; // l'échantillon commun de la maille

	public:
		JBitArray m_ItemAudience; // l'item de l'audience
		JBitArray m_ItemEffectif; // l'item de l'effectif
	};

public:
	// le constructeur
	JMTFR03RequeteA();

	// la fonction pour tester les éléments
	JBool IsValid() const;

	// la fonction pour définir les éléments
	JVoid SetItems(const JMTFR03TBLTerrains & TBLTerrains, const JMTFR03TBLModulations & TBLModulations, const JRef32 & IdSource, const JRef32 & IdTerrain, const JMTFR03Options & Options, const JMTFR03Maille & Maille);

	// la fonction pour récupérer les éléments
	const JRequete & GetItems() const;

	// la fonction pour permuter les éléments
	JVoid Swap(JMTFR03RequeteA & Source);

	// la fonction pour libérer les éléments
	JVoid Reset();

	// le destructeur
	~JMTFR03RequeteA();

private:
	// la fonction pour tester le filtre des modulations
	JBool IsFilterActive(const JMTFR03FLTSegments & FLTSegments) const;

private:
	// la fonction pour rechercher les univers de redressements
	JVoid OnFindUnivers(const JMTFR03DATTerrain & DATTerrain, const JBitArray & ItemUnivers, const JRef32 & IdEchantillon, const JRef32 & IdPoids, const JMTFR03KEYPoids* & pKEYPoids, const JMTFR03DATPoids* & pDATPoids, const JMTFR03KEYHomothetie* & pKEYHomothetie, const JMTFR03DATHomothetie* & pDATHomothetie) const;

private:
	// la fonction pour construire la requête
	JBool OnMakeRequete(const JMTFR03DATTerrain & DATTerrain, const JMTFR03DATModulation & DATModulation, const JMTFR03Options & Options, const JMTFR03Maille & Maille, JRequete & Requete);

private:
	// l'opérateur pour recopier les éléments
	JMTFR03RequeteA & operator =(const JMTFR03RequeteA & Source);

	// le constructeur copie
	JMTFR03RequeteA(const JMTFR03RequeteA & Source);

private:
	JArray<JRequete> m_TBLItems; // la table des éléments
};

// fin de l'inclusion conditionnelle
#endif
