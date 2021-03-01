#pragma once

class CATPMapAffHypo : public JMap<JUnt32,bool>
{
public:
	CATPMapAffHypo(void);
	~CATPMapAffHypo(void);

	void Init(JUnt32 nb, bool etat = false);
	void Set(JUnt32 id, bool etat = true);
	void ResetSet(JUnt32 id);

	JUnt32 GetNbAff();
	
	void Invert(JUnt32 id);

	bool GetState(JUnt32 id);


};
