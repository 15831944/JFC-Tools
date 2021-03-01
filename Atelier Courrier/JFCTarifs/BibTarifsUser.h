// Classe biblioth�que de tous les tarifs users existants
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

	// Chargement biblioth�que tarifs utilisateur
	JBool LoadBibTarif();

	// Sauvegarde biblioth�que tarifs utilisateur
	JBool SaveBibTarif();

	// Pour supprimer un tarif
	JBool RemoveTarif(JUnt32 NoTarif);

	// Pour ajouter un tarif
	JBool AddTarif(JString NomTarif);

	// Pour renommer un tarif
	JBool RenameTarif(JString OldNomTarif, JString NewNomTarif); 

	// Pour copier un tarif
	JBool CopyTarif(JString OrgNomTarif, JString DstNomTarif);

	// R�cup�ration dernier no repertoire tarif dispo
	JUnt32 GetFreeNoRepTarif();

private:

	// Map des tarifs user (clef = n� ident tarif, valeur = libell� tarif)
	JMap<JUnt32,JString> m_MapTarifUser;

	// Premier no libre pour No R�pertoire disponible
	JUnt32 m_NoFreeBase;

};
