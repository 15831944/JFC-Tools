//
// Fichier: JFCEditDecimal.h
// Auteur:  Aurélien Rainone
//

// on débute l'inclusion conditionnelle
#ifndef JFCEDIT_DECIMAL
#define JFCEDIT_DECIMAL

/////////////////////////////////////////////////////////////////////////////
// JFCEditDecimal

class AFX_EXT_CLASS JFCEditDecimal : public CEdit
{
public:
	// le constructeur
	JFCEditDecimal();

	// le destructeur
	virtual ~JFCEditDecimal();

	////////////////////////////////////////////////////////////
	// les fonctions pour la gestion des nombres négatifs
	void SetNegativeNumbers (const bool bAcceptNegativeNumbers);

	const bool AcceptNegativeNumbers () const;

	////////////////////////////////////////////////////////////
	// les fonctions pour la gestion du nombre de décimales
	void SetNumberOfDecimals (const int iNumDecimals);

	const int GetNumberOfDecimals () const;

protected:
	// la fonction pour valider le text de l'EditBox avant de l'afficher
	virtual bool ValidateValue (char nChar);

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	DECLARE_MESSAGE_MAP()

protected:
	CFont m_FontS; // la fonte

protected:
	bool m_AcceptNegativeNumbers;	// saisie de nombres négatifs (inactif par défaut)
	int	 m_NumDecimals;				// nombre de décimales (3 par défaut)

public:
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
};

#endif // JFCEDIT_DECIMAL