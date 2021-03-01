#pragma once
#include "afxwin.h"
#include "JATPDocChoixSupport.h"
#include "JFCICourbe.h"
class CDlgCumulativite;

// Interface pour tra�age courbe cumulativit�
class CATPGraphCrbCum : public JFCICourbe
{
public:
	// Constructeur
	CATPGraphCrbCum();

	// Destructeur
	~CATPGraphCrbCum();

	// R�cup�ration du document Choix Support
	virtual JVoid SetDocument(JATPDocChoixSupport* pATPDocChoiSupport);

	// Mise � jour r�sulats graphe
	JVoid Update();
	
	// R�cup�ration nombre d'�l�ments horizontaux
	virtual JInt32 GetXElmtCount() const;

	// R�cup�ration nombre total de courbes
	virtual JInt32 GetCourbeCount() const;

	// R�cup�ration index couleur de la courbe
	virtual JInt32 GetCourbeColor(JInt32 CourbeIndex) const;

	// R�cup�ration couleur via index
	virtual COLORREF GetColor(JInt32 IdColor) const;

	// la fonction pour r�cup�rer la validit� des �l�ments
	virtual JBool IsItemValid(JInt32 XIndex, JInt32 CourbeIndex) const;
	
	// la fonction pour r�cup�rer la valeur des �l�ments
	virtual JFlt64 GetValueItem(JInt32 XIndex, JInt32 CourbeIndex, bool &ItemMarque) const;

	// la fonction pour r�cup�rer le libell� de la courbe
	virtual const JString GetLabelItem(JInt32 CourbeIndex) const;

	// la fonction pour r�cup�rer le type de r�sultat horizontal
	virtual const JString GetLabelResultHorz() const;

	// la fonction pour r�cup�rer le type de r�sultat vertical
	virtual const JString GetLabelResultVert() const;

	// la fonction pour r�cup�rer borne inf�rieure d�part courbe
	virtual JUnt32 GetDepCourbe(JUnt32 InxCourbe) const;

	// la fonction pour r�cup�rer le libell� support des courbes
	virtual const JString GetLibCourbeGraph(JUnt32 InxCourbe) const;

private :
	// Le document associ�
	JATPDocChoixSupport* m_pATPDocChoixSupport;


protected :
	// Calcul le maximum d'insertions pour le dessin de la courbe (axe horizontal)
	JInt32 MaxJoursSuppGraphe() const;

	// Calcul le nombre de courbes � dessiner
	JInt32 NbCourbeGraph() const;

	// Map r�sultats par courbe
	JMap <JUnt32, JArray <JFlt64> > m_MapResultCrb;

	// Map �tallonage par courbe
	JMap <JUnt32, JArray <bool> > m_MapResultEtalCrb;

	// Consolidation r�sultats par courbe
	JVoid ConsolidResultCrb();

	// Tableau des couleurs par courbe
	JArray <JInt32> m_TabIndexColor;

	// R�cup�re couleur de chaque courbe
	JVoid RecupColorCourbe();

	// No ascisse d�fini pour chaque courbe
	JArray <JUnt32> m_TDepAbscisse;
};
