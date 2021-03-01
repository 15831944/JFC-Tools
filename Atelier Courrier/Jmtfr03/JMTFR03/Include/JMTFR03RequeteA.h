//
// Fichier: JMTFR03RequeteA.h
// Auteur:  Sylvain SAMMURI
// Date:    21/06/2004
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMTFR03_REQUETE_AUDIENCE_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMTFR03_REQUETE_AUDIENCE_H

// on inclut les d�finitions n�cessaires
#include "JMTFR03TBLTerrains.h"
#include "JMTFR03TBLModulations.h"

// on inclut les d�finitions n�cessaires
#include "JMTFR03Options.h"
#include "JMTFR03Maille.h"

class JMTFR03RequeteA  
{
public:
	// on d�finit les parutions
	class JParution
	{
	public:
		JInt32 m_Type;            // le type du titre
		JInt32 m_IdTitre;         // l'identifiant du titre
		JInt32 m_IdTuteur;        // l'identifiant du tuteur
		JInt32 m_NbInsertions;    // le nombre d'insertions
		JFlt64 m_GRPRef;          // le GRP de r�f�rence du titre
		JInt32 m_PtEtalonnageVol; // le point d'�talonnage du volume de contacts
		JInt32 m_PtEtalonnageRef; // le point d'�talonnage de l'audience de r�f�rence

	public:
		JBitArray m_ItemAudience; // l'item de l'audience
		JBitArray m_ItemEffectif; // l'item de l'effectif

	public:
		const JMTFR03KEYPoids*  m_pKEYPoidsVariablesRef;    // les poids des variables de r�f�rence
		const JMTFR03DATPoids*  m_pDATPoidsVariablesRef;    // les poids des variables de r�f�rence
		const JMTFR03KEYPoids*  m_pKEYPoidsProbabilitesRef; // les poids des probabilit�s de r�f�rence
		const JMTFR03DATPoids*  m_pDATPoidsProbabilitesRef; // les poids des probabilit�s de r�f�rence
		const JMTFR03KEYPoids*  m_pKEYPoidsVariablesDDLRef; // les poids des variables DDL de r�f�rence
		const JMTFR03DATPoids*  m_pDATPoidsVariablesDDLRef; // les poids des variables DDL de r�f�rence
		const JMTFR03KEYPoids*  m_pKEYPoidsReprises;        // les poids des reprises
		const JMTFR03DATPoids*  m_pDATPoidsReprises;        // les poids des reprises

	public:
		const JMTFR03KEYHomothetie*  m_pKEYHomothetieVariablesRef;    // l'homoth�tie des variables de r�f�rence
		const JMTFR03DATHomothetie*  m_pDATHomothetieVariablesRef;    // l'homoth�tie des variables de r�f�rence
		const JMTFR03KEYHomothetie*  m_pKEYHomothetieProbabilitesRef; // l'homoth�tie des probabilit�s de r�f�rence
		const JMTFR03DATHomothetie*  m_pDATHomothetieProbabilitesRef; // l'homoth�tie des probabilit�s de r�f�rence
		const JMTFR03KEYHomothetie*  m_pKEYHomothetieVariablesDDLRef; // l'homoth�tie des variables DDL de r�f�rence
		const JMTFR03DATHomothetie*  m_pDATHomothetieVariablesDDLRef; // l'homoth�tie des variables DDL de r�f�rence
		const JMTFR03KEYHomothetie*  m_pKEYHomothetieReprises;        // l'homoth�tie des reprises
		const JMTFR03DATHomothetie*  m_pDATHomothetieReprises;        // l'homoth�tie des reprises

	public:
		const JMTFR03DATVariable* m_pDATVariablesRef; // les variables de r�f�rence (LDP ou LNM)
		const JMTFR03DATVariable* m_pDATVariablesLDP; // les variables LDP
		const JMTFR03DATVariable* m_pDATVariablesLNM; // les variables LNM

	public:
		const JMTFR03DATVariable* m_pDATProbabilitesRef; // les probabilit�s de r�f�rence (LDP ou LNM)
		const JMTFR03DATVariable* m_pDATProbabilitesLDP; // les probabilit�s LDP
		const JMTFR03DATVariable* m_pDATProbabilitesLNM; // les probabilit�s LNM

	public:
		const JMTFR03DATLectorat* m_pDATVariablesDDL_Vol; // les variables DDL du volume de contacts
		const JMTFR03DATLectorat* m_pDATVariablesDDL_Ref; // les variables DDL de l'audience de r�f�rence
		const JMTFR03DATLectorat* m_pDATVariablesDDL_001; // les variables DDL de l'audience �   1 jour
		const JMTFR03DATLectorat* m_pDATVariablesDDL_XX1; // les variables DDL de l'audience �   1 jour bis
		const JMTFR03DATLectorat* m_pDATVariablesDDL_002; // les variables DDL de l'audience �   2 jours
		const JMTFR03DATLectorat* m_pDATVariablesDDL_004; // les variables DDL de l'audience �   4 jours
		const JMTFR03DATLectorat* m_pDATVariablesDDL_007; // les variables DDL de l'audience �   7 jours
		const JMTFR03DATLectorat* m_pDATVariablesDDL_014; // les variables DDL de l'audience �  14 jours
		const JMTFR03DATLectorat* m_pDATVariablesDDL_030; // les variables DDL de l'audience �  30 jours
		const JMTFR03DATLectorat* m_pDATVariablesDDL_060; // les variables DDL de l'audience �  60 jours
		const JMTFR03DATLectorat* m_pDATVariablesDDL_090; // les variables DDL de l'audience �  90 jours
		const JMTFR03DATLectorat* m_pDATVariablesDDL_120; // les variables DDL de l'audience � 120 jours
		const JMTFR03DATLectorat* m_pDATVariablesDDL_180; // les variables DDL de l'audience � 180 jours
		const JMTFR03DATLectorat* m_pDATVariablesDDL_270; // les variables DDL de l'audience � 270 jours
		const JMTFR03DATLectorat* m_pDATVariablesDDL_365; // les variables DDL de l'audience � 365 jours

	public:
		const JMTFR03DATReprise* m_pDATReprises; // les reprises en mains
	};

public:
	// on d�finit la requ�te
	class JRequete
	{
	public:
		JList<JParution> m_TBLParutions; // la table des parutions

	public:
		const JMTFR03KEYPoids* m_pKEYPoids; // les poids de l'univers par d�faut
		const JMTFR03DATPoids* m_pDATPoids; // les poids de l'univers par d�faut

	public:
		const JMTFR03TBLEchantillons* m_pTBLEchantillons; // la table des �chantillons
		const JMTFR03TBLHomotheties*  m_pTBLHomotheties;  // la table des homoth�ties

	public:
		const JMTFR03KEYPoids*  m_pKEYPoidsVariablesRef;    // les poids des variables de r�f�rence
		const JMTFR03DATPoids*  m_pDATPoidsVariablesRef;    // les poids des variables de r�f�rence
		const JMTFR03KEYPoids*  m_pKEYPoidsProbabilitesRef; // les poids des probabilit�s de r�f�rence
		const JMTFR03DATPoids*  m_pDATPoidsProbabilitesRef; // les poids des probabilit�s de r�f�rence
		const JMTFR03KEYPoids*  m_pKEYPoidsVariablesDDLRef; // les poids des variables DDL de r�f�rence
		const JMTFR03DATPoids*  m_pDATPoidsVariablesDDLRef; // les poids des variables DDL de r�f�rence
		const JMTFR03KEYPoids*  m_pKEYPoidsReprises;        // les poids des reprises
		const JMTFR03DATPoids*  m_pDATPoidsReprises;        // les poids des reprises

	public:
		const JMTFR03KEYHomothetie*  m_pKEYHomothetieVariablesRef;    // l'homoth�tie des variables de r�f�rence
		const JMTFR03DATHomothetie*  m_pDATHomothetieVariablesRef;    // l'homoth�tie des variables de r�f�rence
		const JMTFR03KEYHomothetie*  m_pKEYHomothetieProbabilitesRef; // l'homoth�tie des probabilit�s de r�f�rence
		const JMTFR03DATHomothetie*  m_pDATHomothetieProbabilitesRef; // l'homoth�tie des probabilit�s de r�f�rence
		const JMTFR03KEYHomothetie*  m_pKEYHomothetieVariablesDDLRef; // l'homoth�tie des variables DDL de r�f�rence
		const JMTFR03DATHomothetie*  m_pDATHomothetieVariablesDDLRef; // l'homoth�tie des variables DDL de r�f�rence
		const JMTFR03KEYHomothetie*  m_pKEYHomothetieReprises;        // l'homoth�tie des reprises
		const JMTFR03DATHomothetie*  m_pDATHomothetieReprises;        // l'homoth�tie des reprises

	public:
		const JMTFR03KEYEchantillon* m_pKEYEchantillon; // l'�chantillon commun de la maille
		const JMTFR03DATEchantillon* m_pDATEchantillon; // l'�chantillon commun de la maille

	public:
		JBitArray m_ItemAudience; // l'item de l'audience
		JBitArray m_ItemEffectif; // l'item de l'effectif
	};

public:
	// le constructeur
	JMTFR03RequeteA();

	// la fonction pour tester les �l�ments
	JBool IsValid() const;

	// la fonction pour d�finir les �l�ments
	JVoid SetItems(const JMTFR03TBLTerrains & TBLTerrains, const JMTFR03TBLModulations & TBLModulations, const JRef32 & IdSource, const JRef32 & IdTerrain, const JMTFR03Options & Options, const JMTFR03Maille & Maille);

	// la fonction pour r�cup�rer les �l�ments
	const JRequete & GetItems() const;

	// la fonction pour permuter les �l�ments
	JVoid Swap(JMTFR03RequeteA & Source);

	// la fonction pour lib�rer les �l�ments
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
	// la fonction pour construire la requ�te
	JBool OnMakeRequete(const JMTFR03DATTerrain & DATTerrain, const JMTFR03DATModulation & DATModulation, const JMTFR03Options & Options, const JMTFR03Maille & Maille, JRequete & Requete);

private:
	// l'op�rateur pour recopier les �l�ments
	JMTFR03RequeteA & operator =(const JMTFR03RequeteA & Source);

	// le constructeur copie
	JMTFR03RequeteA(const JMTFR03RequeteA & Source);

private:
	JArray<JRequete> m_TBLItems; // la table des �l�ments
};

// fin de l'inclusion conditionnelle
#endif
