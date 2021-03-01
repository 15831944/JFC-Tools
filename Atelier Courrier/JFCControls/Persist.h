/*
//	JFC Informatique et Média
//	2005
//
//	Auteur:
//	Jérémy DEVILERS (Pris dans le livre "Atelier Visual C++ 6.0")
//
//
//	Classe:
//	CPersistentFrame
//
//	Descriptif:
//	Permet de mémoriser la position d'une fenetre
*/

// Persist.h

#ifndef _INSIDE_VISUAL_CPP_PERSISTENT_FRAME
#define _INSIDE_VISUAL_CPP_PERSISTENT_FRAME

class AFX_EXT_CLASS CPersistentFrame : public CFrameWnd
{ // mémorise son emplacement sur le bureau
    DECLARE_DYNAMIC(CPersistentFrame)
private:
    static const CRect s_rectDefault;
    static const char s_profileHeading[];
    static const char s_profileRect[];
    static const char s_profileIcon[];
    static const char s_profileMax[];
    static const char s_profileTool[];
    static const char s_profileStatus[];
    BOOL m_bFirstTime;
protected: // Create from serialization only
    CPersistentFrame();
    ~CPersistentFrame();
//{{AFX_VIRTUAL(CPersistentFrame)
    public:
    virtual void ActivateFrame(int nCmdShow = -1);
    protected:
    //}}AFX_VIRTUAL

    //{{AFX_MSG(CPersistentFrame)
    afx_msg void OnDestroy();
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

#endif // _INSIDE_VISUAL_CPP_PERSISTENT_FRAME
