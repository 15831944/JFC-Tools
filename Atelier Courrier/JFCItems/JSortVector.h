//
// Fichier: JSortVector.h
// Auteur:  Sylvain SAMMURI
// Date:    26/10/2005
//

// on inclut les d�finitions n�cessaires
#include "JLib.h"

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JLIB_JSORT_VECTOR_H

// on d�finit la macro pour �viter les inclusions multiples
#define JLIB_JSORT_VECTOR_H

template <class JItem> class JSortVector
{
public:
	// la fonction pour trier les �l�ments
	static JVoid Sort(JVector<JItem> & TBLItems, JInt32 Sort = -1);

private:
	// la fonction pour ordonner les �l�ments
	static JVoid OnOrderItems(JVector<JItem> & TBLItems, JInt32 Index, JInt32 Max, JInt32 Sort);

	// la fonction pour comparer les �l�ments
	static JInt32 OnCompareItems(const JVector<JItem> & TBLItems, JInt32 IndexL, JInt32 IndexR);

	// la fonction pour permuter les �l�ments
	static JVoid OnSwapItems(JVector<JItem> & TBLItems, JInt32 IndexL, JInt32 IndexR);
};

//////////////////////////////////////
// la fonction pour trier les �l�ments

template <class JItem> JVoid JSortVector<JItem>::Sort(JVector<JItem> & TBLItems, JInt32 Sort)
{
	// on v�rifie le tri
	if (Sort != -1 && Sort != +1) throw JInvalidCall::GetInstance();

	// on r�cup�re le nombre d'�l�ments
	JInt32 Count = TBLItems.GetCount();

	// on calcule l'indice du dernier �l�ment et l'indice de l'�l�ment m�dian
	JInt32 Max = Count - 1;
	JInt32 Med = Max   / 2;

	// on ordonne tous les �l�ments pour les transformer en tas
	for (JInt32 Idx = Med; Idx >= 0; Idx -= 1) { JSortVector<JItem>::OnOrderItems(TBLItems, Idx, Max, Sort); }

	// on extrait un par un tous les �l�ments du tas en le r�ordonnant � chaque fois
	for (JInt32 Jdx = Max; Jdx >= 1; Jdx -= 1) { JSortVector<JItem>::OnSwapItems(TBLItems, Jdx, 0); JSortVector<JItem>::OnOrderItems(TBLItems, 0, Jdx - 1, Sort); }
}

/////////////////////////////////////////
// la fonction pour ordonner les �l�ments

template <class JItem> JVoid JSortVector<JItem>::OnOrderItems(JVector<JItem> & TBLItems, JInt32 Index, JInt32 Max, JInt32 Sort)
{
	// on calcule l'indice de d�part et l'indice du fils gauche
	JInt32 Zdx = Index;
	JInt32 Ldx = (Zdx * 2) + 1;

	// on descend l'�l�ment jusqu'au bas de l'arbre
	while (Ldx <= Max)
	{
		// on calcule l'indice du fils droit
		JInt32 Rdx = Ldx + 1;

		// on prend soit le fils gauche, soit le fils droite
		JInt32 Kdx = Ldx; if (Rdx <= Max) { if (Sort == JSortVector<JItem>::OnCompareItems(TBLItems, Ldx, Rdx)) Kdx = Rdx; }

		// on teste si on a fini l'ordonnancement
		if (Sort == JSortVector<JItem>::OnCompareItems(TBLItems, Kdx, Zdx)) return;

		// on permute les �l�ments courants
		JSortVector<JItem>::OnSwapItems(TBLItems, Zdx, Kdx);

		// on met � jour les indices
		Ldx = (Kdx * 2) + 1;
		Zdx = (Kdx);
	}
}

/////////////////////////////////////////
// la fonction pour comparer les �l�ments

template <class JItem> JInt32 JSortVector<JItem>::OnCompareItems(const JVector<JItem> & TBLItems, JInt32 IndexL, JInt32 IndexR)
{
	// on r�cup�re les �l�ments
	TBLItems.MoveTo(IndexL); const JItem & ItemL = TBLItems.GetItem();
	TBLItems.MoveTo(IndexR); const JItem & ItemR = TBLItems.GetItem();

	// on compare les �l�ments
	if (ItemL < ItemR) return (-1);
	if (ItemR < ItemL) return (+1);

	// les �l�ments sont �gaux
	return (0);
}

/////////////////////////////////////////
// la fonction pour permuter les �l�ments

template <class JItem> JVoid JSortVector<JItem>::OnSwapItems(JVector<JItem> & TBLItems, JInt32 IndexL, JInt32 IndexR)
{
	// on r�cup�re les �l�ments
	TBLItems.MoveTo(IndexL); JItem & ItemL = TBLItems.GetItem();
	TBLItems.MoveTo(IndexR); JItem & ItemR = TBLItems.GetItem();

	// on pemute les �l�ments
	JItem ItemT = ItemL; ItemL = ItemR; ItemR = ItemT;
}

// fin de l'inclusion conditionnelle
#endif
