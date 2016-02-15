// FileUtils.cpp: implementation of the CFileUtils class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FileUtils.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileUtils::CFileUtils()
{

}

CFileUtils::~CFileUtils()
{

}

/*------------------------------------------------------------------------
 Procedure:     Date_Decode ID:1
 Purpose:       Décodage de la date : passer du nombre de jours depuis le
                01/01/1980 à une date au format JJMMAA. AA représente 19AA si
                AA est >= 80, 20AA autrement.
 Input:         La date codée.
 Output:        La date au format JJMMAA.
 Errors:
------------------------------------------------------------------------*/
long pascal CFileUtils::Date_Decode(short coddate)
{
	short jour, mois, an, nbja;
	short nbjm[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
	long r;

	an=80;
	nbja=366;
	while(coddate > nbja) {
		coddate -= nbja;
		an++;
		nbja=365;
		if(0==(an%4)) nbja=366;
	}
	if((0==(an%4))) nbjm[1] = 29;
	mois=0;
	while(coddate>nbjm[mois]) {
		coddate -= nbjm[mois];
		mois++;
	}
	mois++;
	jour=coddate;
	if (an>99) an-=100;
	r = 10000*(long) jour;
	r += 100*(long) mois;
	r += an;
	return(r);
}


/*------------------------------------------------------------------------
 Procedure:     Date_010180 ID:1
 Purpose:       Coder une date dans un format exploitable : le nombre de jour
                depuis le 01/01/1980, avec la possibilité de trouver
                immédiatement de trouver le jour de semaine avec le modulo 7,
                0 étant lundi jusqu'à 6 pour dimanche.
 Input:         La date au format JJMMAA, AA < 80 représentat les dates
                supérieures ou égales à 2000.
 Output:        La date codée.
 Errors:        0 si erreur.
------------------------------------------------------------------------*/
short pascal CFileUtils::Date_010180(long date)
{
	short jour, mois, an, r, a, nbja, m;
	short nbjm[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
	if (date == -1L) return(-1);
	r=0;
	an=(int)(date%100);
	if (an<80) an+=100;
	date=date/100;
	mois=(int)(date%100);
	jour=(int)(date/100);
	if((mois<1) || (mois>12)) return(0);
	if (0==(an%4)) nbjm[1] = 29;
	if ((jour<1) || (jour>nbjm[mois-1])) return(0);
	for(a=80; a!=an; a++) {
		nbja = 365;
		if(0==(a%4)) nbja = 366;
		r += nbja;
	}
	for(m=1; m<mois; m++)
		r += nbjm[m-1];
	r += jour;
	return(r);
}

/*------------------------------------------------------------------------
 Procedure:     SupBlancsFin ID:1
 Purpose:       Suppression des blancs de fin d'une chaîne de caractères.
 Input:
 Output:
 Errors:
------------------------------------------------------------------------*/
void CFileUtils::SupBlancsFin(LPSTR ch)
{
	for (short i=strlen(ch)-1; i>=0; i--)
		if (ch[i]!=' ') break;
	ch[i+1]=0;
}

/*------------------------------------------------------------------------
 Procedure:     SupprimeCARIAGE_LINE ID:1
 Purpose:       Termine une chaîne de caractères au premier caractre de
                contrôle rencontré.
 Input:
 Output:
 Errors:
------------------------------------------------------------------------*/
void CFileUtils::SupprimeCARIAGE_LINE(LPSTR txt)
{
	short l=strlen(txt);
	for(short i=l;i>=0;i--){
		if (txt[i]<32){
		    txt[i]=0;
			return;
		}
	}
}


/*------------------------------------------------------------------------
 Procedure:     MetZero ID:1
 Purpose:       Remplace, dans une chaîne de caractères, tous les caractères
                de contrôle (<32) par un \0. La chaîne est traitée sur une
                longueur donnée, et peut déjà contenir des \0.
 Input:
 Output:
 Errors:
------------------------------------------------------------------------*/
void CFileUtils::MetZero(LPSTR c, short t)
{
	for(short i=0;i<t;i++){
		if(c[i]<=32) c[i]=0;
	}
}

/*------------------------------------------------------------------------
 Procedure:     MetVzero ID:1
 Purpose:       Remplace, dans une chaîne de caractères, les ',' et les
                caractères de contrôle par un \0. La chaîne est de la longueur
                donnée, et peut déjà comprendre des \0.
 Input:
 Output:
 Errors:
------------------------------------------------------------------------*/
void CFileUtils::MetVzero(LPSTR c, short t)
{
	for(short i=0;i<t;i++){
//		if(c[i]<32) c[i]=0;
		if(c[i]==',') c[i]=0;
	}
}

/*------------------------------------------------------------------------
 Procedure:     ExtraireVChaine ID:1
 Purpose:       Extrait et positionne des zéros dans une chaîne et donne la
                position des champs. Rend 0 si OK.
                Traite la virgule comme un séparateur.
 Input:
 Output:
 Errors:
------------------------------------------------------------------------*/
short CFileUtils::ExtraireVChaine(short NbPos)
{
	short NrMot=0;
	m_Pos.RemoveAll();
	SupprimeCARIAGE_LINE(m_BuffLigne);
	MetVzero(m_BuffLigne,TAILLEBUFFLIGNE);
	for (short i=0;i<TAILLEBUFFLIGNE;i++){
		if((!i&&m_BuffLigne[i])||(i&&m_BuffLigne[i]&&!m_BuffLigne[i-1])){
			m_Pos.Add(i);
		}
		if(m_Pos.GetSize()>=NbPos)return(0);
	}
	return(1);
}

/*------------------------------------------------------------------------
 Procedure:     ExtraireChaine ID:1
 Purpose:       Extrait et positionne des zéros dans une chaîne et donne la
                position des champs. Rend 0 si OK.
                Ne traite pas la virgule comme un séparateur.
 Input:
 Output:
 Errors:
------------------------------------------------------------------------*/
short CFileUtils::ExtraireChaine(short NbPos)
{
	short NrMot=0;
	m_Pos.RemoveAll();
	SupprimeCARIAGE_LINE(m_BuffLigne);
//	MetZero(m_BuffLigne,t);
	for (short i=0;i<TAILLEBUFFLIGNE;i++){
		if((!i&&m_BuffLigne[i])||(i&&m_BuffLigne[i]&&!m_BuffLigne[i-1])){
			m_Pos.Add(i);
		}
		if(m_Pos.GetSize()>=NbPos)return(0);
	}
	return(1);
}

/*------------------------------------------------------------------------
 Procedure:     FormatteChaine ID:1
 Purpose:       Formate et positionne des zéros dans une chaîne en fonction
                des positions des champs. Retourne une erreur ou 0 si tout est
                OK.
 Input:
 Output:
 Errors:
------------------------------------------------------------------------*/
short CFileUtils::FormatteChaine(short NbPos)
{
	SupprimeCARIAGE_LINE(m_BuffLigne);
	// des 0 pour arrêter les champs
	for(short i=1;i<NbPos;i++)m_BuffLigne[m_Pos[i]-1]=0;
	// on réduit les champs
	for(i=0;i<NbPos;i++)SupBlancsFin(&m_BuffLigne[m_Pos[i]]);
	return(0);
}

/*------------------------------------------------------------------------
 Procedure:     LectureLigne ID:1
 Purpose:       Lit le fichier et découpe le buffer de ligne en champs.
                Retourne un code d'erreur.
 Input:
 Output:
 Errors:
------------------------------------------------------------------------*/
short CFileUtils::LectureLigne(short & NrLigne, CStdioFile * Fic,short NbPos)
{
	memset(m_BuffLigne,0,TAILLEBUFFLIGNE);
	if(!Fic->ReadString(m_BuffLigne,TAILLEBUFFLIGNE))	return(1);
	if(ExtraireChaine(NbPos)) return(1);
	NrLigne++;
	return(0);
}

/*------------------------------------------------------------------------
 Procedure:     LectureLigneV ID:1
 Purpose:       Lit le fichier et découpe le buffer de ligne en champs.
                Considere les virgules comme séparateur. Retourne un code
                d'erreur.
 Input:
 Output:
 Errors:
------------------------------------------------------------------------*/
short CFileUtils::LectureLigneV(short & NrLigne,CStdioFile * Fic, short NbPos)
{
	memset(m_BuffLigne,0,TAILLEBUFFLIGNE);
	if(!Fic->ReadString(m_BuffLigne,TAILLEBUFFLIGNE))	return(1);
	if(ExtraireVChaine(NbPos)) return(1);
	NrLigne++;
	return(0);
}

/*------------------------------------------------------------------------
 Procedure:     LireEnregistrement ID:1
 Purpose:       Lit un enregistrement simple formaté en champs. Retourne un
                code d'erreur.
 Input:
 Output:
 Errors:
------------------------------------------------------------------------*/
short CFileUtils::LireEnregistrement(short & NrLigne, CStdioFile * Fic,short NbPos)
{
	memset(m_BuffLigne,0,TAILLEBUFFLIGNE);
	if(!Fic->ReadString(m_BuffLigne,TAILLEBUFFLIGNE))	return(1);
	if(FormatteChaine(NbPos)) return(1);
	NrLigne++;
	return(0);
}

/*------------------------------------------------------------------------
 Procedure:     RechercheLigne ID:1
 Purpose:       Lit le fichier et se place à la position du caractère
                recherché. Retourne un code d'erreur.
 Input:
 Output:
 Errors:
------------------------------------------------------------------------*/
short CFileUtils::RechercheLigne(short & NrLigne,CString &Buf,CStdioFile * Fic, char Symbole)
{
	char fTrouve=0;
	while(!fTrouve){
		if(!Fic->ReadString(Buf))	return(1);
		NrLigne++;
		Buf.TrimLeft();
		Buf.TrimRight();
		if(Buf.IsEmpty())	continue;
		if(Buf[0]==';')	continue;
		if(Buf[0]==Symbole)	break;
	}
	return(0);
}

/*------------------------------------------------------------------------
 Procedure:     RechercheLigne ID:1
 Purpose:       Lit le champ de 'buf' avant 'sym1'. Positionne 'buf' apres 'sym2' de 'decal' caracteres
 Input:
 Output:
 Errors:
------------------------------------------------------------------------*/
CString CFileUtils::GetPtrPWOFic(CString & buf, char sym1, char sym2, short decal)
{
	CString str;
	str=buf.Left(buf.Find(sym1));
	buf=buf.Right(buf.GetLength()-buf.Find(sym2)-decal);
	return str;
}

