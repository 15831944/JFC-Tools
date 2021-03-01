//
// Fichier: JITPTBLHypotheses.h
// Auteur:  Alain Chambard & Eddie Gent
// Date:    28/08/2003
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JITP_TABLE_HYPOTHESES_H

// on d�finit la macro pour �viter les inclusions multiples
#define JITP_TABLE_HYPOTHESES_H

// on inclut les d�finitions n�cessaires
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

	// la fonction pour r�cup�rer le nombre d'�l�ments
	JInt32 GetCount() const;

	// la fonction pour ajouter les �l�ments
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

	// les fonctions pour r�cup�rer l'�l�ment du curseur
	const JUnt32 & GetKey() const;
	const JITPDATHypoFormat & GetItem() const;
	JITPDATHypoFormat & GetItem();

	// les fonctions pour d�placer le curseur
	JVoid MoveTo(const JUnt32 & IdxHypothese, JInt32 Move = 0) const;
	JVoid MoveFirst() const;
	JVoid MoveNext() const;
	JVoid MovePrev() const;
	JVoid MoveLast() const;

	// la fonction pour supprimer l'�l�ment courant
	JVoid Remove(JInt32 Move = 0);

	// la fonction pour permuter les �l�ments
	JVoid Swap(JITPTBLHypotheses & Source);

	// la fonction pour lib�rer les �l�ments
	JVoid Reset();

	// le destructeur
	~JITPTBLHypotheses();

	void UpdateTarif(const JATPKEYPeriodeAction & KEYPeriodeAction, const JATPKEYPeriodeAction & KEYPeriodeTablier);

	void CalcBudgInsert(const JATPKEYPeriodeAction & KEYPeriodeAction, const JATPKEYPeriodeAction & KEYPeriodeTablier, const CInsertion * pInsertion , JITPDATHypoFormat & DATTemp);

private:
	// l'op�rateur pour recopier les �l�ments
	JITPTBLHypotheses & operator =(const JITPTBLHypotheses & Source);

	// le constructeur copie
	JITPTBLHypotheses(const JITPTBLHypotheses & Source);

private:
	JATPDocCreuset	*				m_pDocCreuset; // Le doc creuset
	JMap<JUnt32, JITPDATHypoFormat>	m_Hypotheses; // les hypotheses
};

// fin de l'inclusion conditionnelle
#endif
