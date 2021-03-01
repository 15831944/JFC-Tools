// CCourbe.h: interface for the CCourbe class.
//
//////////////////////////////////////////////////////////////////////

#pragma once


class CCourbe : public JArray<JInt32>
{
public:
	CCourbe(void);
	CCourbe(JInt32 id, const JChar* lbl);
	~CCourbe(void);

	JInt32 GetId() const;
	const JLabel & GetLabel() const;
	void SetLabel(const JChar* lbl);

	virtual JArchive& Recv(JArchive & lhs, JInt32 Version);
	virtual JArchive& Send(JArchive & lhs, JInt32 Version) const;

private:
	JInt32 m_Id;
	JLabel m_Label;

};
