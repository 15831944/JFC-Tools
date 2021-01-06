#pragma once

#include <Resizer.h>
#include <Label.h>
#include <BtnST.h>
#include "afxwin.h"
#include "JATPDocChoixSupport.h"
#include "JLib.h"
#include "DupGridWnd.h"
#include "afxcmn.h"
#include "TextProgressCtrl.h"

// Bo�te de dialogue CDlgDuplication
class CDlgDuplication : public JFCDialog
{
	DECLARE_DYNAMIC(CDlgDuplication)

public:
    // Constructeur standard
	CDlgDuplication(CWnd* pParent = NULL,JATPDocChoixSupport *pDoc = NULL);

	// Constructeur
	virtual ~CDlgDuplication();

	// Donn�es de bo�te de dialogue
	enum { IDD = IDD_CS_DLG_DUPLICATION };

	// D�finition des couleurs interface
	void SetColors();

	// Mise � jour s�lections supports
	bool UpdateSelSupports();

	// Mise � jour s�lections format supports
	bool UpdateSelFormatSupports();

	// Update r�sultat duplication
	bool UpdateResultDup(JBool CalculResult);

	// Update des r�sultats au niveau grille cellule (ne touche pas aux headers)
	bool UpdateResultat(JBool CalculResult);

	// Clipboard Duplication
	bool Clipboard();

	// Print Duplication
	bool PrintExcel();

	// Open (Excel) Duplication
	bool OpenExcel();

private:

	// SaveAsExcel Duplication
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

	// Affichage ou pas certains supports
	JVoid UpdateAfficheSupports();

	// Affichage r�sultat duplication
	JBool AfficheResultat();

	// Mise � jour des formats supports dans duplication
	void UpdateFormatSupports();

	// Mise � jour du tri courant
	JVoid MajTriCourant();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSize(UINT nType, int cx, int cy);

protected:
	virtual void OnCancel();
	virtual BOOL OnInitDialog();

	// la fonction pour mettre � jour les vues
	virtual VOID UpdateAllViews(const JFCUpdate & Message);

private:
	// Pointeur document choix supports
	JATPDocChoixSupport *pDocChoixSupport;

	// Objet pour le resize des objets
	CResizer m_Resizer;

	// Barre de progression calcul duplication
	CTextProgressCtrl m_ProgressResultDup;

	// Cadre englobant les objets duplication
	CLabel m_CadreDuplication;

	// Titre informations analyse duplication
	CLabel m_TitreDuplication;

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

	// D�finit si la grille r�sultat est valide
	bool GrilleResultValid();

	// Mise � jour s�lection supports
	void UpdateEtatSelSupports();

	// Couleurs sp�ciales de certaines cellules
	void UpdateColorSpe();

public :
	// R�cup�re nombre de lignes header
	JUnt32 GetNbHeaderRow();

	// R�cup�re nombre de colonnes header
	JUnt32 GetNbHeaderCol();

	// Affiche barre de calcul
	void AffichBarreCalcul();

	// Export des r�sultats Duplication
	void ExportResultat();

	// R�cup�ration de l'ent�te pour export mapping
	void RecupEnteteExport(CString &Entete);

	// R�cup�ration r�sultats duplication d�taill�e
	void RecupGrilleExport(CString &Entete);

	// R�cup�ration bloc des infos terrains / cibles
	void RecupBlocTerrainCible(CString &Entete);

	// R�cup�ration Id Grandeur
	JUnt32 RecupIdGrandeur(JUnt32 &NoCol, JUnt32 NivDim);

	// Update visualisation supports
	void UpdateVisuSupports();

	// Position colonne invisible (Id Support)
	JUnt32 m_NoColIdSupport;

	// Position des colonnes pr�d�finis (S�l support, Libell� support, Format Support, Libell� p�riodicit�)
	JUnt32 m_NoColSelSupport;
	JUnt32 m_NoColSupport;
	JUnt32 m_NoColFormat;
	JUnt32 m_NoColPeriodicite;

	// Calcul Centrage entre 2 titres en horizontal ou en vertical
	JFlt64 CalculCentrage(JFlt64 &AudienceDupliquee, JFlt64 &AudienceCible);

	// Calcul Coefficient de Yule
	JFlt64 CalculCoeffYule(JFlt64 &AudienceDupliquee, JFlt64 &AudienceCibleRow, JFlt64 &AudienceCibleCol, JFlt64 &EffectifGlobal);

protected:
	// Grille r�sultat duplication
	CDupGridWnd m_GridDuplication;

	// Update Entete Analyse (Nom de la source / terrain et date tarification)
	void UpdateTitreAnalyse();

	// Update R�sultat des Co�ts (suite � modif Format ou Tarif)
	JVoid UpdateResultCout();

	// R�sultat cellule pour toutes les grandeurs duplication
	CString ResultCellule(JUnt32 NoLig, JUnt32 NoCol, CKeyResultDup KeyResultDup, CResultDup ResultDup);
};
