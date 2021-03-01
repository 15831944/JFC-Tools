//
// Fichier: JITPTBLHypotheses.h
// Auteur:  Alain Chambard & Eddie Gent
// Date:    28/08/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JITP_TABLE_HYPOTHESES_H

// on définit la macro pour éviter les inclusions multiples
#define JITP_TABLE_HYPOTHESES_H

// on inclut les définitions nécessaires
#include "JLib.h"
#include "JITPDATHypothese.h"
#include "JATPKEYPeriodeAction.h"
#include "PlanInsertions.h"

class  JATPDocCreuset;


class JITPTBLHypotheses
{
public:
	// le constructeur
	JITPTBLHypotheses(JATPDocCreuset *pDoc);

	// la fonction pour récupérer le nombre d'éléments
	JInt32 GetCount() const;

	// la fonction pour ajouter les éléments
	JVoid Add(const JUnt32 & IdxHypothese, const JATPKEYPeriodeAction & KEYPeriodeAction, const JATPKEYPeriodeAction & KEYPeriodeTablier, LIST_HYPOTHESES & PLNHypotheses, JBool Move = false);

	void CalcDatHypo(const JATPKEYPeriodeAction & KEYPeriodeAction, const JATPKEYPeriodeAction & KEYPeriodeTablier, CPlanInsertions & PLNInsertions, JITPDATHypoFormat & DATTemp, bool bNoDistrib/*=false*/);
	void CalcDatHypo(const JATPKEYPeriodeAction & KEYPeriodeAction, const JATPKEYPeriodeAction & KEYPeriodeTablier, CPlanInsertions & PLNInsertions, JITPDATHypoFormat & DATTemp, JUnt32 idItem, bool bNoDistrib = false);
	void CalcDatHypoFActif(const JATPKEYPeriodeAction & KEYPeriodeAction, const JATPKEYPeriodeAction & KEYPeriodeTablier, CPlanInsertions & PLNInsertions, JUnt32 idItem);
	
private:
	void CalcDatInsert(const JATPKEYPeriodeAction & KEYPeriodeAction, const JATPKEYPeriodeAction & KEYPeriodeTablier, const CInsertion * pInsertion , JITPDATHypoFormat & DATTemp, CKeyMoteur & keyMoteur, bool noBudget = false);
	void CalcDatInsertFActif(const JATPKEYPeriodeAction & KEYPeriodeAction, const JATPKEYPeriodeAction & KEYPeriodeTablier, const CInsertion * pInsertion , CKeyMoteur & keyMoteur, bool noBudget = false);

public:
	// la fonction pour tester le curseur
	JBool IsValid() const;

	// les fonctions pour récupérer l'élément du curseur
	const JUnt32 & GetKey() const;
	const JITPDATHypoFormat & GetItem() const;
	JITPDATHypoFormat & GetItem();

	// les fonctions pour déplacer le curseur
	JVoid MoveTo(const JUnt32 & IdxHypothese, JInt32 Move = 0) const;
	JVoid MoveFirst() const;
	JVoid MoveNext() const;
	JVoid MovePrev() const;
	JVoid MoveLast() const;

	// la fonction pour supprimer l'élément courant
	JVoid Remove(JInt32 Move = 0);

	// la fonction pour permuter les éléments
	JVoid Swap(JITPTBLHypotheses & Source);

	// la fonction pour libérer les éléments
	JVoid Reset();

	// le destructeur
	~JITPTBLHypotheses();

	void UpdateTarif(const JATPKEYPeriodeAction & KEYPeriodeAction, const JATPKEYPeriodeAction & KEYPeriodeTablier);

	void CalcBudgInsert(const JATPKEYPeriodeAction & KEYPeriodeAction, const JATPKEYPeriodeAction & KEYPeriodeTablier, const CInsertion * pInsertion , JITPDATHypoFormat & DATTemp);

private:
	// l'opérateur pour recopier les éléments
	JITPTBLHypotheses & operator =(const JITPTBLHypotheses & Source);

	// le constructeur copie
	JITPTBLHypotheses(const JITPTBLHypotheses & Source);

private:
	JATPDocCreuset	*				m_pDocCreuset; // Le doc creuset
	JMap<JUnt32, JITPDATHypoFormat>	m_Hypotheses; // les hypotheses
};

// fin de l'inclusion conditionnelle
#endif
