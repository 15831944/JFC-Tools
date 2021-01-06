/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	JSegment
//
//	Descriptif:
//	Classe représantant un Segment de la table des segments
*/

#pragma once


class AFX_EXT_CLASS JSegment
{
public:
	/// les constructeurs
	JSegment();
	JSegment(const JSegment & Segment);

	/// la fonction pour tester l'élément
	JBool IsValid() const;

	/// les fonctions pour sérialiser l'élément
	JVoid Send(JStream & Stream) const;
	JVoid Recv(JStream & Stream);

	/// l'opérateur pour recopier l'élément
	JSegment & operator =(const JSegment & Segment);

	/// la fonction pour réinitialiser l'élément
	JVoid Reset();

	/// le destructeur
	~JSegment();

public:
	JInt32 m_IdSegment; ///< l'identifiant de la segment
	JLabel m_Libelle;  ///< le libellé
};
