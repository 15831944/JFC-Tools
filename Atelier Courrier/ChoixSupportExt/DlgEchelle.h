#pragma once
#include "afxcmn.h"

#include <Resizer.h>
#include <Label.h>
#include <BtnST.h>
#include "afxwin.h"
#include "JLib.h"
#include "EchelleGridWnd.h"
#include "JATPDocChoixSupport.h"
#include "JATPDocument.h"
#include "TextProgressCtrl.h"


// Boîte de dialogue CDlgEchelle
class CDlgEchelle : public JFCDialog
{
	DECLARE_DYNAMIC(CDlgEchelle)

public:
	// constructeur standard
	CDlgEchelle(CWnd* pParent = NULL,JATPDocChoixSupport *pDoc = NULL);   

	// destructeur standard
	virtual ~CDlgEchelle();

	virtual BOOL OnInitDialog();

	// Données de boîte de dialogue
	enum { IDD = IDD_CS_DLG_ECHELLES };

	// Eléments en entête
	enum {IDTERRAIN = 1, IDCIBLE = 2, IDGRANDEUR = 3};

	// Définition des couleurs interface
	void SetColors();

	// Récupération sélection Nb Insertions
	void SetNbInsertions(JUnt32 NbInsertions);

	// Renvoi sélection Nb Insertions
	JUnt32  GetNbInsertions();

	// Mise à jour sélections supports
	bool UpdateSelSupports();

	// Mise à jour sélections format supports
	bool UpdateSelFormatSupports();

	// Update grille résultat échelle via new sélection
	bool UpdateResultEchelle(JBool CalculResult);

	// Update des résultats au niveau grille cellule (ne touche pas aux headers)
	bool UpdateResultat(JBool CalculResult);

	// Clipboard Echelle
	bool Clipboard(JBool ParCible);

	// Print Echelle
	bool PrintExcel(JBool ParCible);

	// Open Excel
	bool OpenExcel(JBool ParCible);
							  
private:
	// SaveAsExcel Echelle
	bool SaveAsExcel(CString filePath, JBool ParCible);

	// Set up des différents contrôles
	void SetupControls();

	// Mise en place des contrôles
	void SetWindowRelations();

	// Initialisation des références colonnes
	void InitRefGrid();

	// Initialisation du grid
	void InitGrid();

	// Initialisation du résultat grid
	JBool InitGridResultat();

	// Affichage résultat échelle selon les grandeurs sélectionnées
	JBool AfficheResultat();

	// Affichage rang des résultats échelle selon les grandeurs sélectionnées
	JVoid AfficheRang();

	// Affichage Moyenne des supports (visibles bien sur)
	JVoid AfficheResultFamille();

	// Affiche ligne résultats moyenne
	JVoid AfficheLigneMoyenne();

	// Affichage Moyenne des tarifs supports
	JVoid AfficheTarifMoyenne();

	// Mise à jour des formats supports dans échelle
	void UpdateFormatSupports();

	// Mise à jour des libellés supports dans échelle
	void UpdateLibelleSupports();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

protected:
	virtual void OnCancel();

	// la fonction pour mettre à jour les vues
	virtual VOID UpdateAllViews(const JFCUpdate & Message);

private:
	// Objet pour le resize des objets
	CResizer m_Resizer;

	// Barre de progression calcul résultats échelle
	CTextProgressCtrl m_ProgressResultEch;

	// Cadre englobant les objets echelles
	CLabel m_CadreEchelle;

	// Titre Echelle (Nom Source + Source Audience)
	CLabel m_TitreEchelle;

	// Nombre de lignes header 
	JUnt32 m_NbHeaderRow;

	// Nombre de lignes supports
	JUnt32 m_NbRowSupport;

	// Nombre de lignes infos supplémentaires (Moyenne par exemple)
	JUnt32 m_NbRowInfo;

	// Nombre de colonnes header 
	JUnt32 m_NbHeaderCol;

	// Nombre de colonnes
	JUnt32 m_NbCol;

	// Pointeur document choix supports
	JATPDocChoixSupport *pDocChoixSupport;

	// Mise à jour colonnes grid avec nouvelle sélection
	bool UpdateCol();

	// Mise à jour lignes grid avec nouvelle sélection
	bool UpdateRowHeader();

	// Récupération texte header row
	CString RecupTexteHeaderRow(JUnt32 Lig, JUnt32 Col);

	// Init lignes supports
	void InitRowSupports();

	// Mise à jour sélection supports
	void UpdateEtatSelSupports();

	// Les diférentes couleurs pour headers ou cellules spécials
	void UpdateColorSpe();

	// Mise à jour affichage lignes avec infos supplémentaires
	void UpdateRowInfo();

	// Resize des colonnes
	void ResizeColonneFixe();

	// Mise à jour des formats sur les supports sélectionnés
	void MajFormatsSupports();

	// Mise à jour du tri colonne courant (avec derniere grandeur sélectionnée)
	JVoid MajTriCourant();

	// Est-ce une grandeur calcul coût
	JBool GrandeurCout(JUnt32 IdGrandeur);

public :
	// Position colonne invisible (Id Support)
	JUnt32 m_NoColIdSupport;

	// Position des colonnes visibles prédéfinis (Sél support, Lib Support, Tarif, Lib Périodicité)
	JUnt32 m_NoColSelSupport;
	JUnt32 m_NoColSupport;
	JUnt32 m_NoColFormat;
	JUnt32 m_NoColTarif;
	JUnt32 m_NoColPeriodicite;

	// Récupére nombre de lignes header
	JUnt32 GetNbHeaderRow();

	// Récupére nombre de colonnes header
	JUnt32 GetNbHeaderCol();

	// Trie des colonnes
	void TrierColonne();

	// Mode de tri à appliquer sur une colonne (0 / croissant, 1 /décroissant)
	JUnt32 ModeRangColonne(JUnt32 NoCol);
	
	// Affiche barre de calcul
	void AffichBarreCalcul();

	// Export des résultats Echelle (Global ou par cibles)
	void ExportResultat(JBool ParCible);

	// Récupération de l'entête à exporter
	void RecupEnteteExport(CString &Entete);

	// Récupération résultat échelle détaillée
	void RecupGrilleExport(CString &Entete);

	// Récupération des résultats visibles du grille échelle
	void RecupResultEcran(CString &Entete);

	// Récupération des résultats visibles du grille échelle par cible
	void RecupResultEcranParCible(CString &Entete);

	// Récupération bloc des infos terrains / cibles
	void RecupBlocTerrainCible(CString &Entete);

	// Récupération Id Grandeur
	JUnt32 RecupIdGrandeur(JUnt32 &NoCol, JUnt32 NivDim);

	// Update visualisation supports
	void UpdateVisuSupports();

protected:

	// Grille résultats pour les échelles
	CEchelleGridWnd m_GridEchelle;

	// Définit si la grille résultat est valide
	bool GrilleResultValid();

	// Tableau nb dimensions par niveau
	CArray <JUnt32,JUnt32> m_NbDim;

	// Récupération Id Terrain
	JUnt32 RecupIdTerrain(JUnt32 &NoCol, JUnt32 NivDim);

	// Récupération Id Cible
	JUnt32 RecupIdCible(JUnt32 &NoCol, JUnt32 NivDim);

	// Résultats selon grandeur formatage texte
	/*
	CString ResultCellule(JUnt32 IdSupport, JUnt32 NoLig, JUnt32 NoCol, JBool ResultMoyenne, JUnt32 IdTerrain, JUnt32 IdCible,
											JFlt64 Tarif		= 0.0,
											JFlt64 VolLecture	= 0.0,
											JFlt64 AudienceLDP	= 0.0,
											JFlt64 AudienceProba= 0.0,
											JFlt64 Audience		= 0.0,
											JFlt64 Audience1Ins = 0.0, 
											JFlt64 Effectif		= 0.0,
											JFlt64 NbCas		= 0.0,
											JFlt64 VolLectAff	= 0.0,
											JFlt64 AudAffLDP	= 0.0,
											JFlt64 AudAffProba	= 0.0,
											JFlt64 AudAff		= 0.0,
											JFlt64 EffAff		= 0.0);
	*/
	CString CDlgEchelle::ResultCellule(JUnt32 IdSupport, JUnt32 NoLig, JUnt32 NoCol, JBool ResultMoyenne, JUnt32 IdTerrain, JUnt32 IdCible, 
								       CResultEch	&ResultEch, JFlt64 Tarif = 0.0);

	// Résultats selon grandeur en valeur numérique
	/*
	JFlt64 ResultValCellule(JUnt32 IdSupport, JUnt32 NoLig,  	 JUnt32 NoCol,      JFlt64 Tarif,     JFlt64 VolLecture, JFlt64 AudienceLDP, JFlt64 AudienceProba, JFlt64 Audience, JFlt64 Audience1Ins, 
										      JFlt64 Effectif,   JFlt64 NbCas,
		                                      JFlt64 VolLectAff, JFlt64 AudAffLDP, JFlt64 AudAffProba, JFlt64 AudAff,     JFlt64 EffAff);
	*/
	JFlt64 ResultValCellule(JUnt32 IdSupport, JUnt32 NoLig,  	 JUnt32 NoCol,      JFlt64 Tarif,
							CResultEch &ResultEch);

	// Récupère résultat formaté selon type et nb dec data exo
	CString GetResultDataExo(JUnt32 IdGrandeur, JFlt64 ResultCell);

	// Calcul des résultats moyennés
	JFlt64 MoyenneResultCellule(JUnt32 NoCol, JUnt32 IdTerrain, JUnt32 IdCible);

	/////////////////////// UPDATE SPECIFIQUE FENETRE ECHELLE ///////////////////
	// Update Entete Analyse (Nom de la source / terrain et date tarification)
	JVoid UpdateTitreAnalyse();

	// Update Résultat des Coûts (suite à modif Format ou Tarif)
	JVoid UpdateResultCout();

	// Mise à jour affichage des rangs résultats
	JVoid UpdateRangResultat();

	// Affichage libellé moyenne et ligne moyenne apparente ou non
	JVoid UpdateAfficheMoyenne();

	// Affichage ou pas certains supports
	JVoid UpdateAfficheSupports();
};
