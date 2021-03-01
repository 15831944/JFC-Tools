//
// Fichier: JITPTBLEchelle.h
// Auteur:  Alain Chambard & Eddie Gent
// Date:    28/08/2003
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JITP_TABLE_ECHELLES_H

// on d�finit la macro pour �viter les inclusions multiples
#define JITP_TABLE_ECHELLES_H

// on inclut les d�finitions n�cessaires
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

	// la fonction pour r�cup�rer le nombre d'�l�ments
	JInt32 GetCount() const;

	// la fonction pour ajouter les �l�ments
	JVoid Add(const JUnt32 & IdSupport, const JATPKEYEtude & KEYEtude, const JATPKEYPeriodeAction & KEYPeriodeAction, JITPTBLHypotheses & ITRHypotheses, JBool Move = false, JBool AvecApport = false);

	// la fonction pour tester le curseur
	JBool IsValid() const;

	// les fonctions pour r�cup�rer l'�l�ment du curseur
	const JUnt32 & GetKey() const;
	const JITPDATEchelle & GetItem() const;
	JVoid SetApport(JUnt32 IdSupport, JFlt64 Apport);

	// les fonctions pour d�placer le curseur
	JVoid MoveTo(const JUnt32 & IdSupport, JInt32 Move = 0) const;
	JVoid MoveFirst() const;
	JVoid MoveNext() const;
	JVoid MovePrev() const;
	JVoid MoveLast() const;

	// la fonction pour supprimer l'�l�ment courant
	JVoid Remove(JInt32 Move = 0);

	// la fonction pour lib�rer les �l�ments
	JVoid Reset();

	// le destructeur
	~JITPTBLEchelle();
	// la fonction pour permuter les �l�ments
	JVoid Swap(JITPTBLEchelle & Source);

	void UpdateTarifs(const JATPKEYEtude & KEYEtude);

private:
	bool GetTarif(JUnt32 IdSupport, JUnt32 & Tarif, JBool &TarifManuel, JInt32 & Periodicite, 
		          JDate DateDebut,  JDate DateFin);
	
	// l'op�rateur pour recopier les �l�ments
	JITPTBLEchelle & operator =(const JITPTBLEchelle & Source);

	// le constructeur copie
	JITPTBLEchelle(const JITPTBLEchelle & Source);

private:
	JMap<JUnt32, JITPDATEchelle>	m_Echelles;		// les �chelles
	JATPDocCreuset	*				m_pDocCreuset;	// le document Creuset
};

// fin de l'inclusion conditionnelle
#endif
