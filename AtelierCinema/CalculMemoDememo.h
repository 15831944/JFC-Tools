// CalculMemoDememo.h: interface for the CCalculMemoDememo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CALCULMEMODEMEMO_H__21522FED_D56C_4ECC_BD42_2B5019F60A7E__INCLUDED_)
#define AFX_CALCULMEMODEMEMO_H__21522FED_D56C_4ECC_BD42_2B5019F60A7E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCalculMemoDememo  
{
public:
	// Objet param�tre
	class CParametre
	{
	public:
		
		bool m_OptionCalcul_DememoPendantMemo; // Option de calcul pour tenir compte de la d�m�morisation pendant une phase de m�morisation

		double m_BasePopulation; // Population de la cible
		
		int m_NbSemaine;// De la campagne
		int m_NbSemaineCalcul;// De la courbe � calculer >= � m_NbSemaine
		int m_NbClasse; // nombre de classes de contacts

		int * m_NbChanceSem; // [semaine] Nb de chance maximum sur une semaine donn�e
		double ** m_NbContactBarre; // [semaine][classe] distribution des contacts de la barre (semaines exacte)

		int * m_NbChanceCum; // [semaine] Nb de chance maximum sur une semaine donn�e cumul�e
		double ** m_NbContactPlaque; // [semaine][classe] distribution des contacts de la plaque (cumul�)

		double * m_ValBeta;	//[classe] Courbe de r�ponse
		double	m_DureeBaseBeta; // dur�e de base pour la courbe de r�ponse donn�e
		double	m_DureeMaximum; // dur�e de maximum possible pour calculer le beta via dur�e
		double	* m_DureeMoy; // [semaine] dur�e moyenne du message sur une semaine donn�e

		bool * m_SelectCalend; // [m_NbSemaineCalcul], c'est pour calculer les points de m�mo; alias SemaineActive.
	};

	// Objet R�sultat
	class CResultat
	{
	public:
		CResultat()
		{
			m_MemoTot=NULL;
			m_CouvTot=NULL;
			m_DistTot=NULL;

		};
		void Initialise(int NbSemaine)
		{
			m_NbSemaine=NbSemaine;
			m_MemoTot= new double [NbSemaine];
			m_CouvTot= new double [NbSemaine];
			m_DistTot= new double [NbSemaine];
		};

		CResultat(const  CResultat & Data)
		{
			*this=Data;
		};

		CResultat & operator=(const  CResultat & Data)
		{

			m_NbIndivTouchesTot=Data.m_NbIndivTouchesTot;
			m_NbIndivMemoTot=Data.m_NbIndivMemoTot; 
			m_PtMemoCampTot=Data.m_PtMemoCampTot;	
			m_MemoMoy=Data.m_MemoMoy;
			m_NbSemaine=Data.m_NbSemaine;
			m_MemoTot= new double [m_NbSemaine];
			m_CouvTot= new double [m_NbSemaine];
			m_DistTot= new double [m_NbSemaine];

			for(int sem=0;sem<m_NbSemaine;sem++)
			{
				m_MemoTot[sem] = Data.m_MemoTot[sem];
				m_CouvTot[sem] = Data.m_CouvTot[sem];
				m_DistTot[sem] = Data.m_DistTot[sem];
			}
			return *this;
		};

		virtual ~CResultat()
		{
			if(m_MemoTot!=NULL)delete [] m_MemoTot;
			if(m_CouvTot!=NULL)delete [] m_CouvTot;
			if(m_DistTot!=NULL)delete [] m_DistTot;
		}



		double m_NbIndivTouchesTot;	// Nombre d'individus ayant �t� touch� au moins une fois
		double m_NbIndivMemoTot; 	// Nombre d'individus  ayant m�moris� au moins une fois
		double m_PtMemoCampTot;		// Points de m�mo, pour les semaine "actives"
		double m_MemoMoy;			// M�morisation moyenne pendant les semaines "actives"
		double m_RationMemo()
		{
			if(m_NbIndivMemoTot>0)return m_NbIndivMemoTot/m_NbIndivTouchesTot;
			return 0;
		};
		int m_NbSemaine;
		double * m_MemoTot; // Courbe de m�mo-d�m�mo
		double * m_CouvTot; // Courbe ?
		double * m_DistTot; // Courbe ?

	};


public:
	CCalculMemoDememo();
	virtual ~CCalculMemoDememo();
	CResultat  CCalculMemoDememo::CalculCourbeMemoDememo(CCalculMemoDememo::CParametre * ParamCalcul);

private: 


	// R�sultat du calcul m�mo-d�m�mo
	CResultat m_Resultat;

	// Param�tres pour la calcul
	CParametre * m_Parametre;

	class INDEX
	{
	public:
		// tout par NbSemaineCalcul
		char *s;
		char *z;
		char *b;
		char *v;
		char *w;
		char *p;

	} m_Index;
private:
	void MiseAJourIndex();
	char Borne(char *s,int deb,int max);
	double POWTEST(double x,double y);
	float BetaSuivantModele(float Beta0,float Duree);


};

#endif // !defined(AFX_CALCULMEMODEMEMO_H__21522FED_D56C_4ECC_BD42_2B5019F60A7E__INCLUDED_)
