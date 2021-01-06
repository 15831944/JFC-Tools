#pragma once

#include <Resizer.h>
#include <BtnST.h>
#include "afxwin.h"
#include "JATPDocChoixSupport.h"
#include "afxcmn.h"
#include "label.h"
#include "BagMining.h"
#include "BagCurve.h"
#include "BagHierarchy.h"
#include "ListPoint.h"
#include "JFCMapping.h"
#include "MappingGridWnd.h"
#include "TextProgressCtrl.h"
#include "JFCHorzSplit.h"


// Bo�te de dialogue CDlgMapping

class CDlgMapping : public JFCDialog
{
	friend class CLisPoint;
	DECLARE_DYNAMIC(CDlgMapping)

public:
	// constructeur standard
	CDlgMapping(CWnd* pParent = NULL,JATPDocChoixSupport *pDoc = NULL);

	// destructeur
	virtual ~CDlgMapping();

	// Donn�es de bo�te de dialogue
	enum { IDD = IDD_CS_DLG_MAPPING };

	// D�finition des couleurs interface
	void SetColors();

	// R�cup�ration s�lection Nb Insertions
	void SetNbInsertions(JUnt32 NbInsertions);

	// Renvoi s�lection Nb Insertions
	JUnt32  GetNbInsertions();

	// Mise � jour s�lections supports
	bool UpdateSelSupports();

	// Mise � jour des formats supports non d�finis
	JVoid MajFormat();


	// Mise � jour s�lections format supports
	bool UpdateSelFormatSupports();

	// Update r�sultat mapping
	bool UpdateResultMapping(JBool CalculResult);

	// Update des r�sultats au niveau grille cellule (ne touche pas aux headers)
	bool UpdateResultat(JBool CalculResult);

	// Mise � jour du document Mapping
	void MajMapping(JBool InitPos);

	// la fonction pour mettre � jour la s�lection
	void UpdateSelection();

	// Clipboard Resultat Mapping
	bool ClipboardResultat();

	// Print Resultat Mapping
	bool PrintExcel();

	// Open Resultat Mapping
	bool OpenExcel();

	// Clipboard Graphique Mapping
	void ClipboardGraphique();

	// Clipboard du bitmap mapping
	void ClipboardBitmap();

	// R�cup�ration du bitmap mapping
	CBitmap * GetBitmapMapping();

private:
	
	// SaveAsExcel Resultat Mapping
	bool SaveAsExcel(CString filePath);

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

	// Affichage r�sultat mapping
	JBool AfficheResultat();

	// Affichage Moyenne des supports (visibles bien sur)
	JVoid AfficheResultFamille();

	// Mise � jour des formats supports dans mapping
	void UpdateFormatSupports();

public:

	// Affiche barre de calcul
	void AffichBarreCalcul();

	// Export des r�sultats Mapping
	void ExportResultat();

	// Export du graphique Mapping
	void ExportGraphique();

	// R�cup�ration de l'ent�te pour export duplication
	void RecupEnteteExport(CString &Entete);

	// R�cup�ration r�sultat cumulativit� d�taill�e
	void RecupGrilleExport(CString &Entete);

	// R�cup�ration bloc des infos terrains / cibles
	void RecupBlocTerrainCible(CString &Entete);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV
	afx_msg LRESULT OnSplitHorz(WPARAM WParam, LPARAM LPARAM);
	DECLARE_MESSAGE_MAP()

public:
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

	// R�cup�re n� colonne abscisse mapping
	JUnt32 RecupNoColAbscisseMap();

	// R�cup�re n� colonne ordonn�e mapping	
	JUnt32 RecupNoColOrdonneeMap();

	// R�cup�ration des r�sultats mode valeur
	/*
	double RecupResult(JUnt32 IdSupport, JUnt32 NoCol,  JFlt64 Tarif,  JFlt64 VolLecture, JFlt64 AudienceLDP, JFlt64 AudienceProba, JFlt64 Audience, JFlt64 Audience1Ins, 
													    JFlt64 Effectif,   JFlt64 NbCas,
													    JFlt64 VolLectAff, JFlt64 AudAffLDP, JFlt64 AudAffProba, JFlt64 AudAff,     JFlt64 EffAff);
	*/
	double RecupResult(JUnt32 IdSupport, JUnt32 NoCol,  JFlt64 Tarif,  CResultMap &ResultMap);

	// R�cup�ration des r�sultats mode chaine formatt�e
	/*
	CString RecupResultTxt(JUnt32 IdSupport, JUnt32 NoCol,  JFlt64 Tarif,  JFlt64 VolLecture, JFlt64 AudienceLDP, JFlt64 AudienceProba, JFlt64 Audience, JFlt64 Audience1Ins, 
														    JFlt64 Effectif,   JFlt64 NbCas,
														    JFlt64 VolLectAff, JFlt64 AudAffLDP, JFlt64 AudAffProba, JFlt64 AudAff,     JFlt64 EffAff);
	*/
	CString RecupResultTxt(JUnt32 IdSupport, JUnt32 NoCol,  JFlt64 Tarif, CResultMap &ResultMap);

	// R�cup�re r�sultat format� selon type et nb dec data exo
	CString GetResultDataExo(JUnt32 IdGrandeur, JFlt64 ResultCell);

	// R�cup�ration texte header row
	CString RecupTexteHeaderRow(JUnt32 Lig, JUnt32 Col);

	// Update visualisation supports
	void UpdateVisuSupports();

	// R�cup�ration Id Grandeur (Abscisse ou Ordonn�e)
	JUnt32 RecupIdGrandeur(JUnt32 NoCol);

private:
	// Pointeur document choix supports
	JATPDocChoixSupport *pDocChoixSupport;

	// Objet pour le resize des objets
	CResizer m_Resizer;

	// Titre pour informations analyse Mapping
	CLabel m_TitreMapping;

	// Cadre englobant le mapping
	CLabel m_CadreMapping;

	// Objet Mapping
	JFCMapping m_Mapping;

	// Barre de progression r�sultat Mapping
	CTextProgressCtrl m_ProgressResultMap;

	// Cadre du grid relatif au mapping
	CLabel m_CadreGridMapping;

	// Nombre de lignes header 
	JUnt32 m_NbHeaderRow;

	// Nombre de lignes supports
	JUnt32 m_NbRowSupport;

	// Nombre de colonnes header 
	JUnt32 m_NbHeaderCol;

	// Nombre de colonnes
	JUnt32 m_NbCol;

	// Mise � jour colonnes grid avec nouvelle s�lection
	bool UpdateCol();

	// Mise � jour lignes grid avec nouvelle s�lection
	bool UpdateRow();

	// Init lignes supports
	void InitRowSupports();

	// Init size colonnes
	void InitSizeColonne();

	// Mise � jour s�lection supports
	void UpdateEtatSelSupports();

	// Les dif�rentes couleurs pour headers ou cellules sp�cials
	void UpdateColorSpe();

	
protected:
	// Grid relatif au mapping
	CMappingGridWnd m_GridMapping;

	// D�finit si la grille r�sultat est valide
	bool GrilleResultValid();

	// Les supports du mapping ont-ils �t� modifi�s
	JBool SupportModif();

public:
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

// Implementation
protected:
	CBagMining	  m_BagMining;			// l'ensemble des �l�ments
	CBagCurve	  m_BagCurve;			// l'ensemble des courbes
	CBagHierarchy m_BagHierarchy;		// l'ensemble des hierarchie
	// CListPoint m_Liste;				// l'ensemble des �l�ments

	// Membre split horizontal
	JFCHorzSplit  m_SplitHorz;

protected:
	virtual void OnCancel();
	virtual BOOL OnInitDialog();

	// la fonction pour mettre � jour les vues
	virtual VOID UpdateAllViews(const JFCUpdate & Message);

	// Calcul des r�sultats selon grandeur
	CString ResultCellule(JUnt32 IdSupport, JUnt32 NoLig, JUnt32 NoCol, JFlt64 Tarif,
						  CResultMap &ResultMapping);
	
	// Update Entete Analyse (Nom de la source / terrain et date tarification)
	void UpdateTitreAnalyse();

	// Update R�sultat des Co�ts (suite � modif Format ou Tarif)
	JVoid UpdateResultCout();

	// Affichage ou pas certains supports
	JVoid UpdateAfficheSupports();

	// Visibilit� des �lmts support ou famille
	JBool VisibleSupportMode(JUnt32 IdSupport);

	/////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////
	void PrintBmp(CBitmap *pBmp);
	CBitmap * CopyWindowToBitmap(JFControl* pWnd ,bool bWindowClient);
	CBitmap *CopyScreenToBitmap(LPRECT lpRect);
	bool CopyBitmapToClipBoard(CBitmap *pBitmap); 

};
