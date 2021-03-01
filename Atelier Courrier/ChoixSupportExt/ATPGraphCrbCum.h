#pragma once
#include "afxwin.h"
#include "JATPDocChoixSupport.h"
#include "JFCICourbe.h"
class CDlgCumulativite;

// Interface pour traçage courbe cumulativité
class CATPGraphCrbCum : public JFCICourbe
{
public:
	// Constructeur
	CATPGraphCrbCum();

	// Destructeur
	~CATPGraphCrbCum();

	// Récupération du document Choix Support
	virtual JVoid SetDocument(JATPDocChoixSupport* pATPDocChoiSupport);

	// Mise à jour résulats graphe
	JVoid Update();
	
	// Récupération nombre d'éléments horizontaux
	virtual JInt32 GetXElmtCount() const;

	// Récupération nombre total de courbes
	virtual JInt32 GetCourbeCount() const;

	// Récupération index couleur de la courbe
	virtual JInt32 GetCourbeColor(JInt32 CourbeIndex) const;

	// Récupération couleur via index
	virtual COLORREF GetColor(JInt32 IdColor) const;

	// la fonction pour récupérer la validité des éléments
	virtual JBool IsItemValid(JInt32 XIndex, JInt32 CourbeIndex) const;
	
	// la fonction pour récupérer la valeur des éléments
	virtual JFlt64 GetValueItem(JInt32 XIndex, JInt32 CourbeIndex, bool &ItemMarque) const;

	// la fonction pour récupérer le libellé de la courbe
	virtual const JString GetLabelItem(JInt32 CourbeIndex) const;

	// la fonction pour récupérer le type de résultat horizontal
	virtual const JString GetLabelResultHorz() const;

	// la fonction pour récupérer le type de résultat vertical
	virtual const JString GetLabelResultVert() const;

	// la fonction pour récupérer borne inférieure départ courbe
	virtual JUnt32 GetDepCourbe(JUnt32 InxCourbe) const;

	// la fonction pour récupérer le libellé support des courbes
	virtual const JString GetLibCourbeGraph(JUnt32 InxCourbe) const;

private :
	// Le document associé
	JATPDocChoixSupport* m_pATPDocChoixSupport;


protected :
	// Calcul le maximum d'insertions pour le dessin de la courbe (axe horizontal)
	JInt32 MaxJoursSuppGraphe() const;

	// Calcul le nombre de courbes à dessiner
	JInt32 NbCourbeGraph() const;

	// Map résultats par courbe
	JMap <JUnt32, JArray <JFlt64> > m_MapResultCrb;

	// Map étallonage par courbe
	JMap <JUnt32, JArray <bool> > m_MapResultEtalCrb;

	// Consolidation résultats par courbe
	JVoid ConsolidResultCrb();

	// Tableau des couleurs par courbe
	JArray <JInt32> m_TabIndexColor;

	// Récupère couleur de chaque courbe
	JVoid RecupColorCourbe();

	// No ascisse défini pour chaque courbe
	JArray <JUnt32> m_TDepAbscisse;
};
