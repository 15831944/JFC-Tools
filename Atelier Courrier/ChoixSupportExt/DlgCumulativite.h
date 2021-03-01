#pragma once

#include <Resizer.h>
#include <Label.h>
#include <BtnST.h>
#include "afxwin.h"
#include "JATPDocChoixSupport.h"
#include "JLib.h"
#include "CumGridWnd.h"
#include "afxcmn.h"
#include "ATPGraphCrbCum.h"
#include "TextProgressCtrl.h"
#include "JFCCourbe.h"
#include "JFCHorzSplit.h"

// Bo�te de dialogue CDlgCumulativite
class CDlgCumulativite : public JFCDialog
{
	DECLARE_DYNAMIC(CDlgCumulativite)

public:
    // constructeur standard
	CDlgCumulativite(CWnd* pParent = NULL,JATPDocChoixSupport *pDoc = NULL);

	// Destructeur
	virtual ~CDlgCumulativite();

	// Donn�es de bo�te de dialogue
	enum { IDD = IDD_CS_DLG_CUMULATIVITE };

	// D�finition des couleurs interface
	void SetColors();

	// Mise � jour s�lections supports
	bool UpdateSelSupports();

	// Mise � jour s�lections format supports
	bool UpdateSelFormatSupports();

	// Update grille r�sultat cumulativit� via new s�lection
	bool UpdateResultCum(JBool CalculResult);

	// Update des r�sultats au niveau grille cellule (ne touche pas aux headers)
	bool UpdateResultat(JBool CalculResult);

	// R�cup�re indice couleur pour courbe cumulativit�
	JUnt32 GetIdColorCourbe(JUnt32 NoRow, JUnt32 NoCol);

	// R�cup�re le nombre de courbe cumulation en cours
	JUnt32 GetNbCourbeCum();

	// Nb lignes ou colonnes pour couleur courbe cum
	JUnt32 GetNbRowColorCum();
	JUnt32 GetNbColColorCum();
	
	// Renvoi s�lection Nb Insertions
	JUnt32 GetNbInsertions();

	// Clipboard Cumulativite
	bool Clipboard();

	// Clipboard Graphique Cumulativite
	bool ClipboardGraphique();

	// Print Cumulativite
	bool PrintExcel();

	// Open (Excel) Cumulativite
	bool OpenExcel();

	// R�cup�ration s�lection Nb Insertions
	void SetNbInsertions(JUnt32 NbInsertions);

private:
	// SaveAsExcel Cumulativite
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
	JBool InitGridResultat_OPTIM();
	JBool InitGridResultat_OPTIM_OLD();

	// V�rifie si tous les calculs effectu�es
	JBool AuMoins1ResultatNonCalcule(CKeyResultCum KeyResultCum, JUnt32 MaxInsSupport);

	// Affichage ou pas certains supports
	JVoid UpdateAfficheSupports();

	// Affichage r�sultat cumulativit�
	JBool AfficheResultat();

	// Mise � jour des formats supports dans cumulativit�
	void UpdateFormatSupports();

	// Maximum insertions sur les supports trait�s
	JUnt32 MaxInsertionsAllSupports();

	// Mise � jour du tri courant
	JVoid MajTriCourant();
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV
	afx_msg LRESULT OnSplitHorz(WPARAM WParam, LPARAM LPARAM);
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

	// Barre de progression pour les calculs r�sultats cumulativit�
	CTextProgressCtrl m_ProgressResultCum;

	// Cadre englobant les objets cumulativit�
	CLabel m_CadreCumulativite;

	// Cadre du graphe cumulativit�
	CLabel m_CadreGrapheCum;

	// Titre pour informations analyse cumulativit�
	CLabel m_TitreCumulativite;

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

	// Update couleur courante pour graphe cumulativit�
	void UpdateVisuColorGraphe();

	// Update couleur graphe associ� aux supports
	void UpdateColorGraphSupports();

	// Couleurs sp�ciales de certaines cellules
	void UpdateColorSpe();

	// Nombre de couleur courbe en ligne
	JUnt32 m_NbLigColor;

	// Nombre de couleur courbe en colonne
	JUnt32 m_NbColColor;

	// Graphe cumulativit�
	JFCCourbe		m_GraphCum;

	// Interface graphe cumulativit�
	CATPGraphCrbCum m_IGraphCourbeCum;

	// Scroll horizontal du graphe cumulativit�
	JFCScroll		m_ScrollGraph;


public:

	// Position colonne invisible (Id Support)
	JUnt32 m_NoColIdSupport;

	// Position des colonnes pr�d�finis (S�l support, Libell� support, Libell� p�riodicit�)
	JUnt32 m_NoColSelSupport;
	JUnt32 m_NoColSupport;
	// JUnt32 m_NoColFormat;
	JUnt32 m_NoColColorGraphe;
	JUnt32 m_NoColPeriodicite;

	// Affiche barre de calcul
	void AffichBarreCalcul();

	// Export des r�sultats Cumulation d'audience
	void ExportResultat();

	// R�cup�ration de l'ent�te pour export cumulativit�
	void RecupEnteteExport(CString &Entete);

	// R�cup�ration r�sultat cumulativit� d�taill�e
	void RecupGrilleExport(CString &Entete);

	// R�cup�ration bloc des infos terrains / cibles
	void RecupBlocTerrainCible(CString &Entete);

	// R�cup�ration des infos groupe
	void RecupContenuGroupe(CString &Entete);

	// Update visualisation supports
	void UpdateVisuSupports();

	// Export du graphique cumulativit�
	void ExportGraphique();

	// Export des items
	BOOL ExportItems(long VSize, const TCHAR* pFilename); 

protected:
	// Membre split horizontal
	JFCHorzSplit  m_SplitHorz;

	// Grille r�sultat cumulativit�
	CCumGridWnd m_GridCumulativite;

	// Maximum de colonnes insertions (selon p�riodicit� supports s�lmectionn�s)
	JUnt32		m_MaxColIns;

	/////////////////////// UPDATE SPECIFIQUE FENETRE CUMULATIVITE ///////////////////
	// Update Entete Analyse (Nom de la source / terrain et date tarification)
	void UpdateTitreAnalyse();

	// Update R�sultat des Co�ts (suite � modif Format ou Tarif)
	void UpdateResultCout();

	// R�sultat cellule selon grandeur analyse demand�e
	/*
	CString ResultCellule(JUnt32 NoLig, JUnt32 NoCol, JFlt64 Tarif, JFlt64 VolLecture, JFlt64 Audience, JFlt64 AudienceProb1Ins, 
													  JFlt64 Audience1Ins, JFlt64 Effectif, JFlt64 NbCas, JInt32 NbInsertions);
	*/
	CString ResultCellule(JUnt32 NoLig, JUnt32 NoCol, JFlt64 Tarif, JInt32 NbInsertions, CResultCum &ResultCum);

public :
	// R�cup�re nombre de lignes header
	JUnt32 GetNbHeaderRow();

	// R�cup�re nombre de colonnes header
	JUnt32 GetNbHeaderCol();

	// Update du graphe
	void UpdateGraphe();

private  :
	// Limite maximum insertions selon p�riodicit�
	JUnt32 MaxInsGeneral;
	JUnt32 MaxInsMensuel;
	JUnt32 MaxInsBiMensuel;
	JUnt32 MaxInsHebdo;
	JUnt32 MaxInsBiHebdo;
	JUnt32 MaxInsQuotidien;

	// Gestion arr�t des calcums
	JBool m_StopCalcul;

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};
