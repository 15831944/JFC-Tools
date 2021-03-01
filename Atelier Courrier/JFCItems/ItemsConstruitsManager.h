#pragma once
class CItemsConstruitsBase;

class AFX_EXT_CLASS CItemsConstruitsManager
{
private:
	CItemsConstruitsManager(void);
	~CItemsConstruitsManager(void);

public:
	static CItemsConstruitsManager * GetInstance();
	static void Destroy();

public:

	// Positionnement du mode d'utilsation
	enum ItemsConstruitsMode{ModeUtilisateur, ModeAutomatique};
	void SetMode(ItemsConstruitsMode Mode)		{m_Mode=Mode;}
	const ItemsConstruitsMode GetMode()	const	{return m_Mode;}

	// Récup de l'instance correspond au mode
	CItemsConstruitsBase * GetItemsInstance();
 

private:
	ItemsConstruitsMode m_Mode;

	static CItemsConstruitsManager * m_pInstance;
};
