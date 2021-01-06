/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	IElem
//
//	Descriptif:
//	Classe de base des elements permet d'être inséré dans un JFCMultiListBox<T>
//
//	typedef :
//	IELEMMAP
//
//	Descriptif:
//	Map de IElem*
*/


#if !defined(AFX_IELEM_H__25676C4C_BF74_480B_8CE7_BE01BF61C532__INCLUDED_)
#define AFX_IELEM_H__25676C4C_BF74_480B_8CE7_BE01BF61C532__INCLUDED_

#pragma once
//#pragma warning( disable : 4251 )

class ILot;
class CTitre;
class CTitreApparie;
class CTitreApparente;
class CCouplageLibre;
class JCouplageFixe;
class ICouplage;
class CTitreLot;
class CTitreCouplageLibre;
class JTitreCouplage;
class JTitreCouplageFixe;
class ITitre;


class AFX_EXT_CLASS IElem  
{

public:
	virtual ~IElem();

	virtual JUnt32 GetID() const;
	virtual const JLabel & GetLabel() const;

//	virtual JUnt32 GetPeriodicite() const{ return 0; } ;

	virtual JBool IsTitre() const { return false; };
	virtual JBool IsTitreApparente() const { return false; };
	virtual JBool IsTitreApparie() const { return false; };
	virtual JBool IsTitreLot() const { return false; };
	virtual JBool IsTitreCouplage() const { return false; };
	virtual JBool IsTitreCouplageLibre() const { return false; };
	virtual JBool IsTitreCouplageFixe()const  { return false; };

	virtual JBool IsLot() const { return false; };
	virtual JBool IsCouplage() const { return false; };
	virtual JBool IsCouplageLibre() const { return false; };
	virtual JBool IsCouplageFixe() const { return false; };


	virtual ILot * AsLot() { return NULL;};
	virtual CTitre * AsTitre() { return NULL;};
	virtual ITitre * AsITitre() { return NULL;};
	virtual CTitreApparie * AsTitreApparie() { return NULL;};
	virtual CTitreApparente * AsTitreApparente() { return NULL;};
	virtual ICouplage * AsCouplage() { return NULL;};
	virtual CCouplageLibre * AsCouplageLibre() { return NULL;};
	virtual CCouplageLibre * AsCouplageLibre() const{ return NULL;};
	virtual JCouplageFixe * AsCouplageFixe() { return NULL;};
	virtual CTitreLot * AsTitreLot() { return NULL;};
	virtual JTitreCouplage * AsTitreCouplage() { return NULL;};
	virtual CTitreCouplageLibre * AsTitreCouplageLibre() { return NULL;};
	virtual JTitreCouplageFixe * AsTitreCouplageFixe() { return NULL;};

protected:
	IElem();
	IElem(JUnt32 id, const JChar* lbl);
	JUnt32 m_ID;
	JLabel m_Label;

};

typedef JMap <JUnt32, IElem*> IELEMMAP;


#endif // !defined(AFX_IELEM_H__25676C4C_BF74_480B_8CE7_BE01BF61C532__INCLUDED_)
