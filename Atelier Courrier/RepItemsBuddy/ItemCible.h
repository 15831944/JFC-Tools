
#pragma once


// Cible de la commande CItemCible

class CItemCible : public CCmdTarget
{
	DECLARE_DYNAMIC(CItemCible)

public:
	CItemCible();
	virtual ~CItemCible();

	virtual void OnFinalRelease();
	CString m_label;
	int m_ident;
protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	LONG GetIdent(void);

	enum 
	{
		dispidident = 2
	};
};


