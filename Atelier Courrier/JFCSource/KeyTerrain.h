#pragma once

class AFX_EXT_CLASS CKeyTerrain
{
public:
	CKeyTerrain(void);
	CKeyTerrain(const JUnt32 IDSource, const JUnt32 IDTerrain);
	CKeyTerrain(const CKeyTerrain & Source);

	~CKeyTerrain(void);

	void Set (JUnt32 IDSource, JUnt32 IDTerrain);
	void SetSource (JUnt32 IDSource);
	void SetTerrain (JUnt32 IDTerrain);

	bool IsValid();

	// l'op�rateur pour recopier l'�l�ment
	CKeyTerrain & operator =(const CKeyTerrain & Source);

	// les op�rateurs pour comparer les �l�ments
	JBool operator <(const CKeyTerrain & Reference) const;
	JBool operator >(const CKeyTerrain & Reference) const;

	JUnt32 m_IDSource;
	JUnt32 m_IDTerrain;
};
