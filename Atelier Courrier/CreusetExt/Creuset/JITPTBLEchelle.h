//
// Fichier: JITPTBLEchelle.h
// Auteur:  Alain Chambard & Eddie Gent
// Date:    28/08/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JITP_TABLE_ECHELLES_H

// on définit la macro pour éviter les inclusions multiples
#define JITP_TABLE_ECHELLES_H

// on inclut les définitions nécessaires
#include "JLib.h"
class JATPDocCreuset;

//#include "JATPDocCreuset.h"
#include "JITPDATEchelle.h"
#include "JATPKEYEtude.h"
#include "JATPKEYPeriodeAction.h"

class JITPTBLHypotheses;

class JITPTBLEchelle
{
public:
	// le constructeur
	JITPTBLEchelle(JATPDocCreuset *pDoc);

	// la fonction pour récupérer le nombre d'éléments
	JInt32 GetCount() const;

	// la fonction pour ajouter les éléments
	JVoid Add(const JUnt32 & IdSupport, const JATPKEYEtude & KEYEtude, const JATPKEYPeriodeAction & KEYPeriodeAction, JITPTBLHypotheses & ITRHypotheses, JBool Move = false, JBool AvecApport = false);

	// la fonction pour tester le curseur
	JBool IsValid() const;

	// les fonctions pour récupérer l'élément du curseur
	const JUnt32 & GetKey() const;
	const JITPDATEchelle & GetItem() const;
	JVoid SetApport(JUnt32 IdSupport, JFlt64 Apport);

	// les fonctions pour déplacer le curseur
	JVoid MoveTo(const JUnt32 & IdSupport, JInt32 Move = 0) const;
	JVoid MoveFirst() const;
	JVoid MoveNext() const;
	JVoid MovePrev() const;
	JVoid MoveLast() const;

	// la fonction pour supprimer l'élément courant
	JVoid Remove(JInt32 Move = 0);

	// la fonction pour libérer les éléments
	JVoid Reset();

	// le destructeur
	~JITPTBLEchelle();
	// la fonction pour permuter les éléments
	JVoid Swap(JITPTBLEchelle & Source);

	void UpdateTarifs(const JATPKEYEtude & KEYEtude);

private:
	bool GetTarif(JUnt32 IdSupport, JUnt32 & Tarif, JBool &TarifManuel, JInt32 & Periodicite, 
		          JDate DateDebut,  JDate DateFin);
	
	// l'opérateur pour recopier les éléments
	JITPTBLEchelle & operator =(const JITPTBLEchelle & Source);

	// le constructeur copie
	JITPTBLEchelle(const JITPTBLEchelle & Source);

private:
	JMap<JUnt32, JITPDATEchelle>	m_Echelles;		// les échelles
	JATPDocCreuset	*				m_pDocCreuset;	// le document Creuset
};

// fin de l'inclusion conditionnelle
#endif
