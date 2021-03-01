/*
//	JFC Informatique et M�dia
//	2003
//
//	Auteur:
//	J�r�my DEVILERS
//
//
//	Classe:
//	JSegment
//
//	Descriptif:
//	Classe repr�santant un Segment de la table des segments
*/

#pragma once


class AFX_EXT_CLASS JSegment
{
public:
	/// les constructeurs
	JSegment();
	JSegment(const JSegment & Segment);

	/// la fonction pour tester l'�l�ment
	JBool IsValid() const;

	/// les fonctions pour s�rialiser l'�l�ment
	JVoid Send(JStream & Stream) const;
	JVoid Recv(JStream & Stream);

	/// l'op�rateur pour recopier l'�l�ment
	JSegment & operator =(const JSegment & Segment);

	/// la fonction pour r�initialiser l'�l�ment
	JVoid Reset();

	/// le destructeur
	~JSegment();

public:
	JInt32 m_IdSegment; ///< l'identifiant de la segment
	JLabel m_Libelle;  ///< le libell�
};
