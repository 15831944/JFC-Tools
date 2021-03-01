#if !defined(AFX_JFCUPDATE_H__6828A135_9A44_4318_B09E_BEB365C12860__INCLUDED_)
#define AFX_JFCUPDATE_H__6828A135_9A44_4318_B09E_BEB365C12860__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS JFCUpdate  
{
public:
	// le constructeur copie
	JFCUpdate(long IdMessage);
	JFCUpdate(const JFCUpdate & Source);

	// les opérateurs pour recopier les messages
	JFCUpdate & operator =(long IdMessage);
	JFCUpdate & operator =(const JFCUpdate & Source);

	// les opérateurs pour comparer les messages
	bool operator ==(const JFCUpdate & Reference) const;
	bool operator !=(const JFCUpdate & Reference) const;

	// le destructeur
	~JFCUpdate();

private:
	// le constructeur
	JFCUpdate();

private:
	LONG m_IdMessage; // l'identifiant du message
};

#endif // !defined(AFX_JFCUPDATE_H__6828A135_9A44_4318_B09E_BEB365C12860__INCLUDED_)
