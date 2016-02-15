// Dictionary.cpp: implementation of the CDictionary class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Dictionary.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// Input : Must give full path and filename of the dictionary
// Output : Via the function GetDico()
CDictionary::CDictionary(LPCSTR FileName)
{
	LoadDico(FileName);
}

CDictionary::CDictionary()
{
	m_DicoText.RemoveAll(); // Remove all dictionary texts
	m_DicoIndice.RemoveAll(); // remove all indices
	m_DicoDefine.RemoveAll(); // remove all defines
}

CDictionary::~CDictionary()
{
	m_DicoText.RemoveAll();
	m_DicoIndice.RemoveAll();
	m_DicoDefine.RemoveAll();
}

// Function to get dictionary text using an indice
LPCSTR CDictionary::GetDico(WORD Indice)
{
	int x;

	if(!m_DicoIndice.GetSize()) return(ERR_DICO);

	for(x=0;x<m_DicoIndice.GetSize();x++)
	{
		if(Indice==m_DicoIndice.GetAt(x)) return(m_DicoText.GetAt(x));
	}
	return(ERR_INDICE);
}

// Function to get dictionary text using a Defined string
LPCSTR CDictionary::GetDico(LPCSTR Define)
{
	int x;

	if(!m_DicoDefine.GetSize()) return(ERR_DICO);
	for(x=0;x<m_DicoDefine.GetSize();x++)
	{
		if(m_DicoDefine.GetAt(x).Compare(Define)==0) return(m_DicoText.GetAt(x));
	}
	return(ERR_DEFINE);
}

void CDictionary::LoadDico(LPCSTR FileName)
{
	CFile	DicoFile(FileName,CFile::modeRead); // Open the Dictionary or throw exception
	CString Define,Temp;
	CString Chaine;
	long	FileLength=0;
	char	*pcBegin,*pcEnd,*pcCurrent,*pcEtranger; // byte pointer to read the file
	WORD	*pwIndice;


	FileLength=DicoFile.GetLength();// Get file size
	pcBegin= new char[FileLength]; // get enough memory for the entire file
	pcEnd=pcBegin+FileLength; // Pointeur to end of file
	DicoFile.Read((void*)pcBegin,FileLength); // read the entire file
	DicoFile.Close(); // close the dictionary file

	pcCurrent=pcBegin;
	pcCurrent+=SIZE_ENTETE;
	// Because this was a file designed by PWO its a bit of a shit to read, sorry!!!
	while(pcCurrent<pcEnd)
	{
		pwIndice=(WORD *)pcCurrent; // set pointer to indice
		pcCurrent+=SIZE_ETRANGER; // get offset to pointer to chaine etranger
		pcEtranger=(char *)*pcCurrent; // set pointer to etranger
		pcCurrent+=SIZE_END; // step pointer to first string Define
		Define=pcCurrent; // find length of string
		pcCurrent+=Define.GetLength()+1; // Step past Define
		if(pcEtranger)
		{
			Temp=pcCurrent; // Set chaine
			pcCurrent+=Temp.GetLength()+1; // Step over Chaine francaise
		}
		Chaine=pcCurrent;
		pcCurrent+=Chaine.GetLength()+1; // Step over Chaine

		m_DicoText.Add(Chaine); // add chaine to array
		m_DicoIndice.Add(*pwIndice); // set indice
		m_DicoDefine.Add(Define); // set define
	}
	delete pcBegin;
}

void CDictionary::NewDico(LPCSTR FileName)
{
	m_DicoText.RemoveAll(); // Remove all dictionary texts
	m_DicoIndice.RemoveAll(); // remove all indices
	m_DicoDefine.RemoveAll(); // remove all defines
	LoadDico(FileName); // Charger le dico demande
}

int CDictionary::GetNbElements()
{
	return(m_DicoText.GetSize());
}

CString CDictionary::GetElement(int Element)
{
	CString Enreg;

	Enreg.Format("%06hd %-30s %s",m_DicoIndice.GetAt(Element),
		m_DicoDefine.GetAt(Element),m_DicoText.GetAt(Element));
	return(Enreg);

}
