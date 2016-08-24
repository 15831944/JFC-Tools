// Cible.h: interface for the CCible class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CIBLE_H__BB9437A0_1510_11D4_989E_004095420987__INCLUDED_)
#define AFX_CIBLE_H__BB9437A0_1510_11D4_989E_004095420987__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRITERE
{
public:
	CRITERE & operator=(const CRITERE & Data){
		NrCritere=Data.NrCritere;
		Poids.Copy(Data.Poids);
		return *this;
	}
	short				NrCritere;
	CByteArray			Poids;
};
typedef CArray<CRITERE,CRITERE&> TabCritere;

class SEGMENT
{
public:
	SEGMENT & operator=(const SEGMENT & Data){
		PopPhy=Data.PopPhy;
		PopPon=Data.PopPon;
		NbCas=Data.NbCas;
		PctUni=Data.PctUni;
		Critere.Copy(Data.Critere);
		return *this;
	}
	
	double 			PopPhy;		// (PopPhysique)Population Physique
	double 			PopPon;		// (PopPonderer)Populations Ponderer
	long 			NbCas;		// (Cas)Nombre de cas
	double 			PctUni;		// (Pourcentage)Pourcentage

	TabCritere		Critere;
};

typedef CArray<SEGMENT,SEGMENT&> TabSegment;

class VECTCIBLE
{
	public:
	VECTCIBLE()
	{
		Vecteur=NULL;
	};

	virtual ~VECTCIBLE()
	{
		if(Vecteur) 
		{
			delete [] Vecteur;
			Vecteur = NULL;
		}
	};

	WORD	Cible;		// Numero de la cible
	double			*Vecteur;	// vecteur de long 64K
	double			PopPhy;
	double			PopPon;
	DWORD			NbCas;
	double			PctUni;
};


#define MAXLIBELLE 30
class CCible  
{
public:
	int GetIndexFromNumero(int Numero);
	void Init();

	// L'index des cibles
	CWordArray m_NumCible;
	CStringArray m_LibCible;
	WORD m_NextNumCible;

	// Le dico socio démo
	int m_NbCritere;
	CStringArray m_LibCritere;
	CWordArray m_NbClasse;
	CStringArray * m_LibClasse;

	// Le résultat d'une cible
	double m_PopPhy;
	double m_PopPon;
	double m_PctUni;
	long m_NbCas;


	CCible();
	virtual ~CCible();

	BOOL ChargerIndexCib();
	BOOL DeleteCible(short NumCible);
	BOOL RenommerCible(short NumCible,LPCSTR Nom);
	BOOL ChargerDico();
	BOOL ChargerCible(TabSegment & Segment, short NumCible);
	BOOL MemoriserCible(TabSegment & Segment,short NumCible);
	BOOL GetVectCible(VECTCIBLE *Vecteur);
	void CalculCible(TabSegment & Segment,double * Vecteur=NULL);
	BYTE* CCible::GetNewVecteurCritere(int nrcritere);
private:
	BOOL MemoriserIndexCib();
};

#endif // !defined(AFX_CIBLE_H__BB9437A0_1510_11D4_989E_004095420987__INCLUDED_)
