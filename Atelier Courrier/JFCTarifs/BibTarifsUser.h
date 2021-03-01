// Classe bibliothèque de tous les tarifs users existants
#pragma once

class AFX_EXT_CLASS CBibTarifsUser
{
public :
	// Maximum de 9999 rep tarifs utilisateurs
	static	const JUnt32  MaxRepTarUser;
	static  const JString FicTarifUser;

public:
	// Constructeur
	CBibTarifsUser(void);

	// Destructeur
	virtual ~CBibTarifsUser(void);

	// Chargement bibliothèque tarifs utilisateur
	JBool LoadBibTarif();

	// Sauvegarde bibliothèque tarifs utilisateur
	JBool SaveBibTarif();

	// Pour supprimer un tarif
	JBool RemoveTarif(JUnt32 NoTarif);

	// Pour ajouter un tarif
	JBool AddTarif(JString NomTarif);

	// Pour renommer un tarif
	JBool RenameTarif(JString OldNomTarif, JString NewNomTarif); 

	// Pour copier un tarif
	JBool CopyTarif(JString OrgNomTarif, JString DstNomTarif);

	// Récupération dernier no repertoire tarif dispo
	JUnt32 GetFreeNoRepTarif();

private:

	// Map des tarifs user (clef = n° ident tarif, valeur = libellé tarif)
	JMap<JUnt32,JString> m_MapTarifUser;

	// Premier no libre pour No Répertoire disponible
	JUnt32 m_NoFreeBase;

};
