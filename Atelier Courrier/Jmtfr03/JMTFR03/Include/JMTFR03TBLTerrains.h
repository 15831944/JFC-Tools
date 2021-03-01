//
// Fichier: JMTFR03TBLTerrains.h
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMTFR03_TABLE_TERRAINS_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMTFR03_TABLE_TERRAINS_H

// on inclut les d�finitions n�cessaires
#include "JMTFR03KEYTerrain.h"
#include "JMTFR03DATTerrain.h"

class JMTFR03TBLTerrains
{
public:
	// le constructeur
	JMTFR03TBLTerrains();

	// la fonction pour r�cup�rer le nombre d'�l�ments
	JInt32 GetCount() const;

	// la fonction pour ajouter les �l�ments
	const JMTFR03DATTerrain & Add(const JMTFR03KEYTerrain & KEYTerrain, JBool Move = false);

	// la fonction pour tester le curseur
	JBool IsValid() const;

	// les fonctions pour r�cup�rer l'�l�ment du curseur
	const JMTFR03KEYTerrain & GetKey() const;
	const JMTFR03DATTerrain & GetItem() const;

	JMTFR03DATTerrain & GetItem();

	// les fonctions pour r�cup�rer le tableau des pond�rations du terrain
	JMTFR03TBLPonderations		  & GetTblPonderations(JMTFR03KEYTerrain KeyTerrain);
	const JMTFR03TBLPonderations  & GetTblPonderations(const JMTFR03KEYTerrain KeyTerrain) const;
	
	// les fonctions pour d�placer le curseur
	JVoid MoveTo(const JMTFR03KEYTerrain & KEYTerrain, JInt32 Move = 0) const;
	JVoid MoveFirst() const;
	JVoid MoveNext() const;
	JVoid MovePrev() const;
	JVoid MoveLast() const;

	// la fonction pour supprimer l'�l�ment du curseur
	JVoid Remove(JInt32 Move = 0);

	// la fonction pour permuter les �l�ments
	JVoid Swap(JMTFR03TBLTerrains & Source);

	// la fonction pour lib�rer les �l�ments
	JVoid Reset();

	// le destructeur
	~JMTFR03TBLTerrains();

private:
	// la fonction pour fabriquer les chemins
	JVoid OnMakePath(const JPath & Root, const JChar* pSuffixe, JPath & Path) const;

private:
	// les fonctions pour s�rialiser les ent�tes
	JBool OnRecvHeader(JStream & Stream, JInt32 Magic) const;
	JBool OnRecvSource(JStream & Stream, JRef32 IdSource, JRef32 IdTerrain) const;

private:
	// les fonctions pour s�rialiser les �l�ments du terrain
	JBool OnRecvFiltresAudience     (JStream & Stream, JInt32 NbIndividus, JMTFR03TBLFiltres   & TBLFiltres) const;
	JBool OnRecvSegmentsPopulation  (JStream & Stream, JInt32 NbIndividus, JMTFR03TBLSegments  & TBLSegments) const;
	JBool OnRecvPoidsIndividus      (JStream & Stream, JInt32 NbIndividus, JMTFR03TBLUnivers   & TBLUnivers, JMTFR03TBLEchantillons & TBLEchantillons, JMTFR03TBLHomotheties & TBLHomotheties, JMTFR03TBLPoids & TBLPoids, JMTFR03TBLPonderations & TBLPonderations) const;
	JBool OnRecvReprisesMains       (JStream & Stream, JInt32 NbIndividus, JMTFR03TBLReprises  & TBLReprises) const;
	JBool OnRecvVariablesAgregation (JStream & Stream, JInt32 NbIndividus, JMTFR03TBLVariables & TBLVariables) const;
	JBool OnRecvVariablesEtalonnage (JStream & Stream, JInt32 NbIndividus, JMTFR03TBLLectorats & TBLLectorats) const;
	JBool OnRecvAttributsTitres     (JStream & Stream,                     JMTFR03TBLAttributs & TBLAttributs) const;

private:
	// l'op�rateur pour recopier les �l�ments
	JMTFR03TBLTerrains & operator =(const JMTFR03TBLTerrains & Source);

	// le constructeur copie
	JMTFR03TBLTerrains(const JMTFR03TBLTerrains & Source);

private:
	JMap<JMTFR03KEYTerrain, JMTFR03DATTerrain> m_TBLItems; // la table des �l�ments
};

// fin de l'inclusion conditionnelle
#endif
