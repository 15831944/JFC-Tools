//
// Fichier: JMTFR03DATTerrain.h
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMTFR03_DAT_TERRAIN_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMTFR03_DAT_TERRAIN_H

// on inclut les d�finitions n�cessaires
#include "JMTFR03TBLFiltres.h"
#include "JMTFR03TBLSegments.h"
#include "JMTFR03TBLUnivers.h"
#include "JMTFR03TBLEchantillons.h"
#include "JMTFR03TBLHomotheties.h"
#include "JMTFR03TBLPoids.h"
#include "JMTFR03TBLPonderations.h"
#include "JMTFR03TBLVariables.h"
#include "JMTFR03TBLLectorats.h"
#include "JMTFR03TBLReprises.h"
#include "JMTFR03TBLAttributs.h"

class JMTFR03DATTerrain
{
public:
	// le constructeur
	JMTFR03DATTerrain();

	// le destructeur
	~JMTFR03DATTerrain();



private:
	// l'op�rateur pour recopier l'�l�ment
	JMTFR03DATTerrain & operator =(const JMTFR03DATTerrain & Source);

	// le constructeur copie
	JMTFR03DATTerrain(const JMTFR03DATTerrain & Source);

public:
	JMTFR03TBLFiltres       m_TBLFiltres;          // les filtres d'audience
	JMTFR03TBLSegments      m_TBLSegments;         // les segments de population
	JMTFR03TBLUnivers       m_TBLUnivers;          // les univers de redressement
	JMTFR03TBLEchantillons  m_TBLEchantillons;     // les �chantillons
	JMTFR03TBLHomotheties   m_TBLHomotheties;      // les facteurs d'homoth�tie
	JMTFR03TBLPoids         m_TBLPoids;            // les poids des individus
	JMTFR03TBLPonderations  m_TBLPonderations;     // les ponderations des individus
	JMTFR03TBLReprises      m_TBLReprises;         // les reprises en mains
	JMTFR03TBLVariables     m_TBLVariablesLDP;     // les variables LDP
	JMTFR03TBLVariables     m_TBLVariablesLNM;     // les variables LNM
	JMTFR03TBLVariables     m_TBLProbabilitesLDP;  // les probabilit�s LDP
	JMTFR03TBLVariables     m_TBLProbabilitesLNM;  // les probabilit�s LNM
	JMTFR03TBLLectorats     m_TBLVariablesDDL_001; // les variables DDL �   1 jour
	JMTFR03TBLLectorats     m_TBLVariablesDDL_XX1; // les variables DDL �   1 jour bis
	JMTFR03TBLLectorats     m_TBLVariablesDDL_002; // les variables DDL �   2 jours
	JMTFR03TBLLectorats     m_TBLVariablesDDL_004; // les variables DDL �   4 jours
	JMTFR03TBLLectorats     m_TBLVariablesDDL_007; // les variables DDL �   7 jours
	JMTFR03TBLLectorats     m_TBLVariablesDDL_014; // les variables DDL �  14 jours
	JMTFR03TBLLectorats     m_TBLVariablesDDL_030; // les variables DDL �  30 jours
	JMTFR03TBLLectorats     m_TBLVariablesDDL_060; // les variables DDL �  60 jours
	JMTFR03TBLLectorats     m_TBLVariablesDDL_090; // les variables DDL �  90 jours
	JMTFR03TBLLectorats     m_TBLVariablesDDL_120; // les variables DDL � 120 jours
	JMTFR03TBLLectorats     m_TBLVariablesDDL_180; // les variables DDL � 180 jours
	JMTFR03TBLLectorats     m_TBLVariablesDDL_270; // les variables DDL � 270 jours
	JMTFR03TBLLectorats     m_TBLVariablesDDL_365; // les variables DDL � 365 jours
	JMTFR03TBLAttributs     m_TBLAttributs;        // les attributs
};

// fin de l'inclusion conditionnelle
#endif
