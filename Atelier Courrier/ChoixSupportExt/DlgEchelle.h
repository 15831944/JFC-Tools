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


// Bo�te de dialogue CDlgEchelle
class CDlgEchelle : public JFCDialog
{
	DECLARE_DYNAMIC(CDlgEchelle)

public:
	// constructeur standard
	CDlgEchelle(CWnd* pParent = NULL,JATPDocChoixSupport *pDoc = NULL);   

	// destructeur standard
	virtual ~CDlgEchelle();

	virtual BOOL OnInitDialog();

	// Donn�es de bo�te de dialogue
	enum { IDD = IDD_CS_DLG_ECHELLES };

	// El�ments en ent�te
	enum {IDTERRAIN = 1, IDCIBLE = 2, IDGRANDEUR = 3};

	// D�finition des couleurs interface
	void SetColors();

	// R�cup�ration s�lection Nb Insertions
	void SetNbInsertions(JUnt32 NbInsertions);

	// Renvoi s�lection Nb Insertions
	JUnt32  GetNbInsertions();

	// Mise � jour s�lections supports
	bool UpdateSelSupports();

	// Mise � jour s�lections format supports
	bool UpdateSelFormatSupports();

	// Update grille r�sultat �chelle via new s�lection
	bool UpdateResultEchelle(JBool CalculResult);

	// Update des r�sultats au niveau grille cellule (ne touche pas aux headers)
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

	// Set up des diff�rents contr�les
	void SetupControls();

	// Mise en place des contr�les
	void SetWindowRelations();

	// Initialisation des r�f�rences colonnes
	void InitRefGrid();

	// Initialisation du grid
	void InitGrid();

	// Initialisation du r�sultat grid
	JBool InitGridResultat();

	// Affichage r�sultat �chelle selon les grandeurs s�lectionn�es
	JBool AfficheResultat();

	// Affichage rang des r�sultats �chelle selon les grandeurs s�lectionn�es
	JVoid AfficheRang();

	// Affichage Moyenne des supports (visibles bien sur)
	JVoid AfficheResultFamille();

	// Affiche ligne r�sultats moyenne
	JVoid AfficheLigneMoyenne();

	// Affichage Moyenne des tarifs supports
	JVoid AfficheTarifMoyenne();

	// Mise � jour des formats supports dans �chelle
	void UpdateFormatSupports();

	// Mise � jour des libell�s supports dans �chelle
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

	// la fonction pour mettre � jour les vues
	virtual VOID UpdateAllViews(const JFCUpdate & Message);

private:
	// Objet pour le resize des objets
	CResizer m_Resizer;

	// Barre de progression calcul r�sultats �chelle
	CTextProgressCtrl m_ProgressResultEch;

	// Cadre englobant les objets echelles
	CLabel m_CadreEchelle;

	// Titre Echelle (Nom Source + Source Audience)
	CLabel m_TitreEchelle;

	// Nombre de lignes header 
	JUnt32 m_NbHeaderRow;

	// Nombre de lignes supports
	JUnt32 m_NbRowSupport;

	// Nombre de lignes infos suppl�mentaires (Moyenne par exemple)
	JUnt32 m_NbRowInfo;

	// Nombre de colonnes header 
	JUnt32 m_NbHeaderCol;

	// Nombre de colonnes
	JUnt32 m_NbCol;

	// Pointeur document choix supports
	JATPDocChoixSupport *pDocChoixSupport;

	// Mise � jour colonnes grid avec nouvelle s�lection
	bool UpdateCol();

	// Mise � jour lignes grid avec nouvelle s�lection
	bool UpdateRowHeader();

	// R�cup�ration texte header row
	CString RecupTexteHeaderRow(JUnt32 Lig, JUnt32 Col);

	// Init lignes supports
	void InitRowSupports();

	// Mise � jour s�lection supports
	void UpdateEtatSelSupports();

	// Les dif�rentes couleurs pour headers ou cellules sp�cials
	void UpdateColorSpe();

	// Mise � jour affichage lignes avec infos suppl�mentaires
	void UpdateRowInfo();

	// Resize des colonnes
	void ResizeColonneFixe();

	// Mise � jour des formats sur les supports s�lectionn�s
	void MajFormatsSupports();

	// Mise � jour du tri colonne courant (avec derniere grandeur s�lectionn�e)
	JVoid MajTriCourant();

	// Est-ce une grandeur calcul co�t
	JBool GrandeurCout(JUnt32 IdGrandeur);

public :
	// Position colonne invisible (Id Support)
	JUnt32 m_NoColIdSupport;

	// Position des colonnes visibles pr�d�finis (S�l support, Lib Support, Tarif, Lib P�riodicit�)
	JUnt32 m_NoColSelSupport;
	JUnt32 m_NoColSupport;
	JUnt32 m_NoColFormat;
	JUnt32 m_NoColTarif;
	JUnt32 m_NoColPeriodicite;

	// R�cup�re nombre de lignes header
	JUnt32 GetNbHeaderRow();

	// R�cup�re nombre de colonnes header
	JUnt32 GetNbHeaderCol();

	// Trie des colonnes
	void TrierColonne();

	// Mode de tri � appliquer sur une colonne (0 / croissant, 1 /d�croissant)
	JUnt32 ModeRangColonne(JUnt32 NoCol);
	
	// Affiche barre de calcul
	void AffichBarreCalcul();

	// Export des r�sultats Echelle (Global ou par cibles)
	void ExportResultat(JBool ParCible);

	// R�cup�ration de l'ent�te � exporter
	void RecupEnteteExport(CString &Entete);

	// R�cup�ration r�sultat �chelle d�taill�e
	void RecupGrilleExport(CString &Entete);

	// R�cup�ration des r�sultats visibles du grille �chelle
	void RecupResultEcran(CString &Entete);

	// R�cup�ration des r�sultats visibles du grille �chelle par cible
	void RecupResultEcranParCible(CString &Entete);

	// R�cup�ration bloc des infos terrains / cibles
	void RecupBlocTerrainCible(CString &Entete);

	// R�cup�ration Id Grandeur
	JUnt32 RecupIdGrandeur(JUnt32 &NoCol, JUnt32 NivDim);

	// Update visualisation supports
	void UpdateVisuSupports();

protected:

	// Grille r�sultats pour les �chelles
	CEchelleGridWnd m_GridEchelle;

	// D�finit si la grille r�sultat est valide
	bool GrilleResultValid();

	// Tableau nb dimensions par niveau
	CArray <JUnt32,JUnt32> m_NbDim;

	// R�cup�ration Id Terrain
	JUnt32 RecupIdTerrain(JUnt32 &NoCol, JUnt32 NivDim);

	// R�cup�ration Id Cible
	JUnt32 RecupIdCible(JUnt32 &NoCol, JUnt32 NivDim);

	// R�sultats selon grandeur formatage texte
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

	// R�sultats selon grandeur en valeur num�rique
	/*
	JFlt64 ResultValCellule(JUnt32 IdSupport, JUnt32 NoLig,  	 JUnt32 NoCol,      JFlt64 Tarif,     JFlt64 VolLecture, JFlt64 AudienceLDP, JFlt64 AudienceProba, JFlt64 Audience, JFlt64 Audience1Ins, 
										      JFlt64 Effectif,   JFlt64 NbCas,
		                                      JFlt64 VolLectAff, JFlt64 AudAffLDP, JFlt64 AudAffProba, JFlt64 AudAff,     JFlt64 EffAff);
	*/
	JFlt64 ResultValCellule(JUnt32 IdSupport, JUnt32 NoLig,  	 JUnt32 NoCol,      JFlt64 Tarif,
							CResultEch &ResultEch);

	// R�cup�re r�sultat format� selon type et nb dec data exo
	CString GetResultDataExo(JUnt32 IdGrandeur, JFlt64 ResultCell);

	// Calcul des r�sultats moyenn�s
	JFlt64 MoyenneResultCellule(JUnt32 NoCol, JUnt32 IdTerrain, JUnt32 IdCible);

	/////////////////////// UPDATE SPECIFIQUE FENETRE ECHELLE ///////////////////
	// Update Entete Analyse (Nom de la source / terrain et date tarification)
	JVoid UpdateTitreAnalyse();

	// Update R�sultat des Co�ts (suite � modif Format ou Tarif)
	JVoid UpdateResultCout();

	// Mise � jour affichage des rangs r�sultats
	JVoid UpdateRangResultat();

	// Affichage libell� moyenne et ligne moyenne apparente ou non
	JVoid UpdateAfficheMoyenne();

	// Affichage ou pas certains supports
	JVoid UpdateAfficheSupports();
};
