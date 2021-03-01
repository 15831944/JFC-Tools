// JFCEdit.h : header file
// Created by: Alvaro Mendez - 07/17/2000
// http://www.codeproject.com/editctrl/validatingedit.asp
//


#pragma once

#include <afxwin.h>
#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// CJFCEdit window

// Class CJFCEdit is the base class for all the other JFC CEdit classes.  
// It provides some base functionality to set and get the text and change
// its text and background color.
//
class AFX_EXT_CLASS CJFCEdit : public CEdit
{
public:
	// Construction/destruction
	CJFCEdit();
	virtual ~CJFCEdit();

	// Operations
	void SetText(const CString& strText);
	CString GetText() const;
	CString GetTrimmedText() const;

	void SetBackgroundColor(COLORREF rgb);
	COLORREF GetBackgroundColor() const;

	void SetTextColor(COLORREF rgb);
	COLORREF GetTextColor() const;

	bool IsReadOnly() const;
	
protected:
	virtual void Redraw();
	virtual CString GetValidText() const;
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
	virtual bool ShouldEnter(TCHAR c) const;

protected:
	CBrush	m_brushBackground;
	COLORREF m_rgbText;

private:
	enum InternalFlags
	{
		None				= 0x0000,
		TextColorHasBeenSet = 0x0001
	};
	UINT m_uInternalFlags;

public:
	// Class SelectionSaver is used to save an edit box's current
	// selection and then restore it on destruction.
	class AFX_EXT_CLASS SelectionSaver
	{
	public:
		SelectionSaver(CEdit* pEdit);
		SelectionSaver(CEdit* pEdit, int nStart, int nEnd);
		~SelectionSaver();

		void MoveTo(int nStart, int nEnd);
		void MoveBy(int nStart, int nEnd);
		void MoveBy(int nPos);
		void operator+=(int nPos);

		int GetStart() const;
		int GetEnd() const;

		void Update();
		void Disable();

	protected:
		CEdit* m_pEdit;
		int m_nStart, m_nEnd;
	};

	// Class Behavior is an abstract base class used to define how an edit
	// box will behave when it is used.   Note that its virtual member functions start
	// with an underscore; this avoids naming conflicts when multiply inheriting.
	class AFX_EXT_CLASS Behavior
	{
	protected:
		Behavior(CJFCEdit* pEdit);
		virtual ~Behavior();

	public:
		void ModifyFlags(UINT uAdd, UINT uRemove);
		UINT GetFlags() const;

	public:
		virtual CString _GetValidText() const = 0;

		virtual void _OnChar(UINT uChar, UINT nRepCnt, UINT nFlags) = 0;
		virtual void _OnKeyDown(UINT uChar, UINT nRepCnt, UINT nFlags);
		virtual void _OnKillFocus(CWnd* pNewWnd);
		virtual LONG _OnPaste(UINT wParam, LONG lParam);

	protected:
		// Wrappers to allow access to protected members of CJFCEdit
		virtual LRESULT _Default();		
		virtual void _Redraw();
		virtual bool _ShouldEnter(TCHAR c) const;

	protected:
		CJFCEdit* m_pEdit;
		UINT m_uFlags;
	};
	friend class Behavior;

	// The AlphanumericBehavior class is used to allow entry of alphanumeric
	// characters.  It can be restricted in terms of what characters cannot 
	// be inputed as well as how many are allowed altogether.
	class AFX_EXT_CLASS AlphanumericBehavior : public Behavior
	{
	public:
		AlphanumericBehavior(CJFCEdit* pEdit, int nMaxChars = 0, const CString& strInvalidChars = _T("%'*\"+?><:\\"));

		// Operations
		void SetInvalidCharacters(const CString& strInvalidChars);
		const CString& GetInvalidCharacters() const;

		void SetMaxCharacters(int nMaxChars);
		int GetMaxCharacters() const;

	protected:
		virtual CString _GetValidText() const;
		virtual void _OnChar(UINT uChar, UINT nRepCnt, UINT nFlags);

	protected:
		int m_nMaxChars;
		CString m_strInvalidChars;
	};

	// The MaskedBehavior class is used to allow entry of numeric characters
	// based on a given mask containing '#' characters to hold digits.
	class AFX_EXT_CLASS MaskedBehavior : public Behavior
	{
	public:
		// Construction
		MaskedBehavior(CJFCEdit* pEdit, const CString& strMask = _T(""));

	public:
		// Operations
		void SetMask(const CString& strMask);
		const CString& GetMask() const;

		CString GetNumericText() const;
	
		// The Symbol class represents a character which may be added to the mask and then interpreted by the 
		// MaskedBehavior class to validate the input from the user and possibly convert it to something else.
		class AFX_EXT_CLASS Symbol
		{
		public:
			#ifndef _UNICODE
				typedef int (*ValidationFunction)(UINT);	// designed for functions such as _istdigit, _istalpha
				typedef UINT (*ConversionFunction)(UINT);	// designed for functions such as _totupper, _totlower
			#else
				typedef int (*ValidationFunction)(WCHAR);		
				typedef WCHAR (*ConversionFunction)(WCHAR);		
			#endif
			
			Symbol();
			Symbol(TCHAR cSymbol, ValidationFunction fnValidation, ConversionFunction fnConversion = NULL);
			virtual ~Symbol();

			virtual bool Validate(TCHAR c) const;
			virtual TCHAR Convert(TCHAR c) const;

			void Set(TCHAR cSymbol);
			TCHAR Get() const;
			operator TCHAR() const;

		protected:
			TCHAR m_cSymbol;
			ValidationFunction m_fnValidation;
			ConversionFunction m_fnConversion;
		};

		typedef CArray<Symbol, Symbol&> SymbolArray;

		SymbolArray& GetSymbolArray();

	protected:
		virtual CString _GetValidText() const;
		virtual void _OnChar(UINT uChar, UINT nRepCnt, UINT nFlags);
		virtual void _OnKeyDown(UINT uChar, UINT nRepCnt, UINT nFlags);

	protected:
		// Attributes
		CString m_strMask;
		SymbolArray m_arraySymbols;
	};

	// The NumericBehavior class is used to allow the entry of an actual numeric
	// value into the edit control.  It may be restricted by the number of digits
	// before or after the decimal point (if any).  If can also be set to use
	// commas to separate and group thousands.
	class AFX_EXT_CLASS NumericBehavior : public Behavior
	{
	public:
		// Construction
		NumericBehavior(CJFCEdit* pEdit, int nMaxWholeDigits = 9, int nMaxDecimalPlaces = 4);

	public:
		// Operations
		void SetDouble(double dText, bool bTrimTrailingZeros = true);
		double GetDouble() const;

		void SetInt(int nText);
		int GetInt() const;
		
		void SetMaxWholeDigits(int nMaxWholeDigits);
		int GetMaxWholeDigits() const;
		
		void SetMaxDecimalPlaces(int nMaxDecimalPlaces);
		int GetMaxDecimalPlaces() const;
		
		void AllowNegative(bool bAllowNegative = true);
		bool IsNegativeAllowed() const;
		
		void SetDigitsInGroup(int nDigitsInGroup);
		int GetDigitsInGroup() const;
		
		void SetSeparators(TCHAR cDecimal, TCHAR cGroup);
		void GetSeparators(TCHAR* pcDecimal, TCHAR* pcGroup) const;

		void SetPrefix(const CString& strPrefix);
		const CString& GetPrefix() const;

		void SetMask(const CString& strMask);
		CString GetMask() const;

		void SetRange(double dMin, double dMax);
		void GetRange(double* pdMin, double* pdMax) const;

		virtual bool IsValid() const;
		bool CheckIfValid(bool bShowErrorIfNotValid = true);

		enum Flags
		{
			None										= 0x0000,
			AddDecimalAfterMaxWholeDigits				= 0x1000,

			OnKillFocus_Beep_IfInvalid					= 0x0001,
			OnKillFocus_Beep_IfEmpty					= 0x0002,
			OnKillFocus_Beep							= 0x0003,
			OnKillFocus_SetValid_IfInvalid				= 0x0004,
			OnKillFocus_SetValid_IfEmpty				= 0x0008,
			OnKillFocus_SetValid						= 0x000C,
			OnKillFocus_SetFocus_IfInvalid				= 0x0010,
			OnKillFocus_SetFocus_IfEmpty				= 0x0020,
			OnKillFocus_SetFocus						= 0x0030,
			OnKillFocus_ShowMessage_IfInvalid			= 0x0050,
			OnKillFocus_ShowMessage_IfEmpty				= 0x00A0,
			OnKillFocus_ShowMessage						= 0x00F0,

			OnKillFocus_PadWithZerosBeforeDecimal		= 0x0100,
			OnKillFocus_PadWithZerosAfterDecimal		= 0x0200,
			OnKillFocus_DontPadWithZerosIfEmpty			= 0x0400,
			OnKillFocus_Max								= 0x0FFF
		};

	protected:
		virtual CString _GetValidText() const;
		virtual void _OnChar(UINT uChar, UINT nRepCnt, UINT nFlags);
		virtual void _OnKeyDown(UINT uChar, UINT nRepCnt, UINT nFlags);
		virtual void _OnKillFocus(CWnd* pNewWnd);

		int GetGroupSeparatorCount(const CString& strText) const;
		
		CString GetNumericText(const CString& strText, bool bConvertToNumericSigns = false) const;
		CString GetDoubleText(double dText, bool bTrimTrailingZeros = true) const;
		CString GetSeparatedText(const CString& strText) const;
		void AdjustSeparators(int nCurrentSeparatorCount);
		static void InsertZeros(CString* pStrText, int nPos, int nCount);

		virtual void ShowErrorMessage() const;
		void AdjustWithinRange();

	protected:
		// Attributes
		int m_nMaxWholeDigits;
		int m_nMaxDecimalPlaces;
		bool m_bAllowNegative;
		TCHAR m_cNegativeSign;
		TCHAR m_cDecimalPoint;
		TCHAR m_cGroupSeparator;
		int m_nDigitsInGroup;
		CString m_strPrefix;
		double m_dMin;
		double m_dMax;
	};

	// The DateBehavior class is used to allow the entry of date values.
	class AFX_EXT_CLASS DateBehavior : virtual public Behavior
	{
	public:
		// Construction
		DateBehavior(CJFCEdit* pEdit);

	public:
		// Operations
		void SetDate(int nYear, int nMonth, int nDay);
		void SetDate(const CTime& date);
		void SetDate(const COleDateTime& date);
		void SetDateToToday();

		CTime GetDate() const;
		COleDateTime GetOleDate() const;

		int GetYear() const;
		int GetMonth() const;
		int GetDay() const;
		void SetYear(int nYear);
		void SetMonth(int nMonth);
		void SetDay(int nDay);
		virtual bool IsValid() const;
		bool CheckIfValid(bool bShowErrorIfNotValid = true);

		void SetRange(const CTime& dateMin, const CTime& dateMax);
		void SetRange(const COleDateTime& dateMin, const COleDateTime& dateMax);
		void GetRange(CTime* pDateMin, CTime* pDateMax) const;
		void GetRange(COleDateTime* pDateMin, COleDateTime* pDateMax) const;
		void SetSeparator(TCHAR cSep);
		TCHAR GetSeparator() const;

		void ShowDayBeforeMonth(bool bDayBeforeMonth = true);
		bool IsDayShownBeforeMonth() const;

		enum Flags
		{
			None								= 0x0000,
			DayBeforeMonth						= 0x1000,

			OnKillFocus_Beep_IfInvalid			= 0x0001,
			OnKillFocus_Beep_IfEmpty			= 0x0002,
			OnKillFocus_Beep					= 0x0003,
			OnKillFocus_SetValid_IfInvalid		= 0x0004,
			OnKillFocus_SetValid_IfEmpty		= 0x0008,
			OnKillFocus_SetValid				= 0x000C,
			OnKillFocus_SetFocus_IfInvalid		= 0x0010,
			OnKillFocus_SetFocus_IfEmpty		= 0x0020,
			OnKillFocus_SetFocus				= 0x0030,
			OnKillFocus_ShowMessage_IfInvalid	= 0x0050,
			OnKillFocus_ShowMessage_IfEmpty		= 0x00A0,
			OnKillFocus_ShowMessage				= 0x00F0,
			OnKillFocus_Max						= 0x00FF
		};

	protected:
		virtual CString _GetValidText() const;
		virtual void _OnChar(UINT uChar, UINT nRepCnt, UINT nFlags);
		virtual void _OnKeyDown(UINT uChar, UINT nRepCnt, UINT nFlags);
		virtual void _OnKillFocus(CWnd* pNewWnd);

	protected:
		// Helpers
		bool AdjustMaxMonthAndDay();
		bool AdjustMaxDay();

		int GetValidMonth() const;
		int GetMaxMonth() const;
		int GetMinMonth() const;
		int GetMonthStartPosition() const;
		TCHAR GetMaxMonthDigit(int nPos) const;
		TCHAR GetMinMonthDigit(int nPos) const;
		bool IsValidMonthDigit(TCHAR c, int nPos) const;
		bool IsValidMonth(int nMonth) const;

		int GetValidDay() const;
		int GetMaxDay() const;
		int GetMinDay() const;
		int GetDayStartPosition() const;
		TCHAR GetMaxDayDigit(int nPos) const;
		TCHAR GetMinDayDigit(int nPos) const;
		bool IsValidDayDigit(TCHAR c, int nPos) const;
		bool IsValidDay(int nDay) const;
		
		int GetValidYear() const;
		int GetYearStartPosition() const;
		TCHAR GetMaxYearDigit(int nPos) const;
		TCHAR GetMinYearDigit(int nPos, bool bValidYear = false) const;
		bool IsValidYearDigit(TCHAR c, int nPos) const;
		bool IsValidYear(int nYear) const;

		virtual bool IsValid(const COleDateTime& date, bool bDateOnly = true) const;
		virtual void ShowErrorMessage() const;
		CString GetFormattedDate(int nYear, int nMonth, int nDay) const;

	public:
		static bool IsLeapYear(int nYear);
		static CString GetString(int nValue, bool bTwoDigitWithLeadingZero = true);
		static int GetMaxDayOfMonth(int nMonth, int nYear);

	protected:
		// Attributes
		COleDateTime m_dateMin;
		COleDateTime m_dateMax;
		TCHAR m_cSep;
	};

	// The TimeBehavior class is used to allow the entry of time values.
	class AFX_EXT_CLASS TimeBehavior : virtual public Behavior
	{
	public:
		// Construction
		TimeBehavior(CJFCEdit* pEdit);

	public:
		// Operations
		void SetTime(int nHour, int nMinute, int nSecond = 0);
		void SetTime(const CTime& time);
		void SetTime(const COleDateTime& time);
		void SetTimeToNow();

		CTime GetTime() const;
		COleDateTime GetOleTime() const;

		int GetHour() const;
		int GetMinute() const;
		int GetSecond() const;
        CString GetAMPM() const;
		void SetHour(int nYear);
		void SetMinute(int nMonth);
		void SetSecond(int nDay);
		void SetAMPM(bool bAM);
		virtual bool IsValid() const;
		bool IsValid(bool bCheckRangeAlso) const;
		bool CheckIfValid(bool bShowErrorIfNotValid = true);

		void SetRange(const CTime& dateMin, const CTime& dateMax);
		void SetRange(const COleDateTime& dateMin, const COleDateTime& dateMax);
		void GetRange(CTime* pDateMin, CTime* pDateMax) const;
		void GetRange(COleDateTime* pDateMin, COleDateTime* pDateMax) const;
	
		void SetSeparator(TCHAR cSep);
		TCHAR GetSeparator() const;

		void Show24HourFormat(bool bShow24HourFormat = true);
		bool IsShowing24HourFormat() const;
		void ShowSeconds(bool bShowSeconds = true);
		bool IsShowingSeconds() const;
		void SetAMPMSymbols(const CString& strAM, const CString& strPM);
		void GetAMPMSymbols(CString* pStrAM, CString* pStrPM) const;
		
		enum Flags
		{
			None								= 0x0000,
			TwentyFourHourFormat				= 0x2000,
			WithSeconds							= 0x4000,

			OnKillFocus_Beep_IfInvalid			= 0x0001,
			OnKillFocus_Beep_IfEmpty			= 0x0002,
			OnKillFocus_Beep					= 0x0003,
			OnKillFocus_SetValid_IfInvalid		= 0x0004,
			OnKillFocus_SetValid_IfEmpty		= 0x0008,
			OnKillFocus_SetValid				= 0x000C,
			OnKillFocus_SetFocus_IfInvalid		= 0x0010,
			OnKillFocus_SetFocus_IfEmpty		= 0x0020,
			OnKillFocus_SetFocus				= 0x0030,
			OnKillFocus_ShowMessage_IfInvalid	= 0x0050,
			OnKillFocus_ShowMessage_IfEmpty		= 0x00A0,
			OnKillFocus_ShowMessage				= 0x00F0,
			OnKillFocus_Max						= 0x00FF
		};

	protected:
		virtual CString _GetValidText() const;
		virtual void _OnChar(UINT uChar, UINT nRepCnt, UINT nFlags);
		virtual void _OnKeyDown(UINT uChar, UINT nRepCnt, UINT nFlags);
		virtual void _OnKillFocus(CWnd* pNewWnd);

	protected:
		// Helpers
		int GetValidHour(bool b24HourFormat = false) const;
		int GetMaxHour(bool b24HourFormat = false) const;
		int GetMinHour(bool b24HourFormat = false) const;
		int GetHourStartPosition() const;
		TCHAR GetMaxHourDigit(int nPos) const;
		TCHAR GetMinHourDigit(int nPos) const;
		bool IsValidHourDigit(TCHAR c, int nPos) const;
		bool IsValidHour(int nHour, bool b24HourFormat = false) const;
		int ConvertTo24Hour(int nHour, const CString& strAMPM) const;
		int ConvertToAMPMHour(int nHour, CString* pStrAMPM = NULL) const;

		int GetValidMinute() const;
		int GetMaxMinute() const;
		int GetMinMinute() const;
		int GetMaxSecond() const;
		int GetMinSecond() const;
		int GetMinuteStartPosition() const;
		TCHAR GetMaxMinuteDigit(int nPos) const;
		TCHAR GetMinMinuteDigit(int nPos) const;
		bool IsValidMinuteDigit(TCHAR c, int nPos) const;
		bool IsValidMinute(int nMinute) const;
		
		int GetValidSecond() const;
		int GetSecondStartPosition() const;
		TCHAR GetMaxSecondDigit(int nPos) const;
		TCHAR GetMinSecondDigit(int nPos) const;
		bool IsValidSecondDigit(TCHAR c, int nPos) const;
		bool IsValidSecond(int nSecond) const;

		void ShowAMPM();
		bool ChangeAMPM(TCHAR c);
		CString GetValidAMPM() const;
		int GetAMPMStartPosition() const;
		bool IsValidAMPM(const CString& strAMPM) const;
		int GetAMPMPosition(const CString& strText) const;

		virtual bool IsValid(const COleDateTime& date, bool bDateOnly = true) const;
		virtual void ShowErrorMessage() const;
		CString GetFormattedTime(int nHour, int nMinute, int nSecond, const CString& strAMPM = _T("")) const;
		void AdjustWithinRange();
		
	protected:
		// Attributes
		COleDateTime m_timeMin;
		COleDateTime m_timeMax;
		TCHAR m_cSep;
		CString m_strAM;
		CString m_strPM;
		int m_nAMPMLength;
		int m_nHourStart;
	};

	// The DateTimeBehavior class is used to allow the entry of date and time values.
	class AFX_EXT_CLASS DateTimeBehavior : public DateBehavior, 
							 public TimeBehavior
	{
	public:
		// Construction
		DateTimeBehavior(CJFCEdit* pEdit);

		void SetDateTime(int nYear, int nMonth, int nDay, int nHour, int nMinute, int nSecond = 0);
		void SetDateTime(const CTime& dt);
		void SetDateTime(const COleDateTime& dt);
		void SetToNow();

		CTime GetDateTime() const;
		COleDateTime GetOleDateTime() const;

		virtual bool IsValid() const;

		void SetRange(const CTime& dateMin, const CTime& dateMax);
		void SetRange(const COleDateTime& dateMin, const COleDateTime& dateMax);
		void GetRange(CTime* pDateMin, CTime* pDateMax) const;
		void GetRange(COleDateTime* pDateMin, COleDateTime* pDateMax) const;
	
		void SetSeparator(TCHAR cSep, bool bDate);
		TCHAR GetSeparator(bool bDate) const;

		void ModifyFlags(UINT uAdd, UINT uRemove);

		enum Flags
		{
			DateOnly							= 0x0100,
			TimeOnly							= 0x0200,

			OnKillFocus_Beep_IfInvalid			= 0x0001,
			OnKillFocus_Beep_IfEmpty			= 0x0002,
			OnKillFocus_Beep					= 0x0003,
			OnKillFocus_SetValid_IfInvalid		= 0x0004,
			OnKillFocus_SetValid_IfEmpty		= 0x0008,
			OnKillFocus_SetValid				= 0x000C,
			OnKillFocus_SetFocus_IfInvalid		= 0x0010,
			OnKillFocus_SetFocus_IfEmpty		= 0x0020,
			OnKillFocus_SetFocus				= 0x0030,
			OnKillFocus_ShowMessage_IfInvalid	= 0x0050,
			OnKillFocus_ShowMessage_IfEmpty		= 0x00A0,
			OnKillFocus_ShowMessage				= 0x00F0,
			OnKillFocus_Max						= 0x00FF
		};

	protected:
		virtual CString _GetValidText() const;
		virtual void _OnChar(UINT uChar, UINT nRepCnt, UINT nFlags);
		virtual void _OnKeyDown(UINT uChar, UINT nRepCnt, UINT nFlags);
		virtual void _OnKillFocus(CWnd* pNewWnd);

	protected:
		virtual bool IsValid(const COleDateTime& date, bool bDateOnly = true) const;
		virtual void ShowErrorMessage() const;
	};


	// Generated message map functions (for CJFCEdit)
protected:
	//{{AFX_MSG(CJFCEdit)
	//}}AFX_MSG
	afx_msg LONG OnCut(UINT wParam, LONG lParam);
	afx_msg LONG OnPaste(UINT wParam, LONG lParam);
	afx_msg LONG OnClear(UINT wParam, LONG lParam);
	afx_msg LONG OnSetText(UINT wParam, LONG lParam);

	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CJFCAlphanumericEdit window

// The CJFCAlphanumericEdit is a CJFCEdit control which supports the AlphanumericBehavior class.
//
class AFX_EXT_CLASS CJFCAlphanumericEdit : public CJFCEdit, 
                             public CJFCEdit::AlphanumericBehavior
{
public:
	// Construction
	CJFCAlphanumericEdit(int nMaxChars = 0, const CString& strInvalidChars = _T("%'*\"+?><:\\"));

protected:
	virtual CString GetValidText() const;

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJFCAlphanumericEdit)
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CJFCAlphanumericEdit)
	afx_msg void OnChar(UINT uChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CJFCMaskedEdit window

// The CJFCMaskedEdit is a CJFCEdit control which supports the MaskedBehavior class.
//
class AFX_EXT_CLASS CJFCMaskedEdit : public CJFCEdit, 
                       public CJFCEdit::MaskedBehavior
{
public:
	// Construction
	CJFCMaskedEdit(const CString& strMask = _T(""));

protected:
	virtual CString GetValidText() const;

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJFCMaskedEdit)
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CJFCMaskedEdit)
	afx_msg void OnChar(UINT uChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT uChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CJFCNumericEdit window

// The CJFCNumericEdit is a CJFCEdit control which supports the NumericBehavior class.
//
class AFX_EXT_CLASS CJFCNumericEdit : public CJFCEdit, 
                        public CJFCEdit::NumericBehavior
{
public:
	CJFCNumericEdit(int nMaxWholeDigits = 9, int nMaxDecimalPlaces = 4);

protected:
	virtual CString GetValidText() const;

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJFCNumericEdit)
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CJFCNumericEdit)
	afx_msg void OnChar(UINT uChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT uChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CJFCIntegerEdit window

// The CJFCNumericEdit is a CJFCEdit control which supports the NumericBehavior class
// restricted to only allow integer values.
//
class AFX_EXT_CLASS CJFCIntegerEdit : public CJFCNumericEdit
{
public:
	// Construction
	CJFCIntegerEdit(int nMaxWholeDigits = 9);

private:
	// Hidden members -- they don't make sense here
	void SetDouble(double dText, bool bTrimTrailingZeros = true);
	double GetDouble() const;
	void SetMaxDecimalPlaces(int nMaxDecimalPlaces);	// always 0

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJFCIntegerEdit)
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CJFCIntegerEdit)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CJFCCurrencyEdit window

// The CJFCNumericEdit is a CJFCEdit control which supports the NumericBehavior class
// modified to put the '$' character in front of the value and use commas to separate the thousands.
//
class AFX_EXT_CLASS CJFCCurrencyEdit : public CJFCNumericEdit
{
public:
	// Construction
	CJFCCurrencyEdit();

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJFCCurrencyEdit)
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CJFCCurrencyEdit)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CJFCDateEdit window

// The CJFCDateEdit is a CJFCEdit control which supports the DateBehavior class.
//
class AFX_EXT_CLASS CJFCDateEdit : public CJFCEdit, 
                     public CJFCEdit::DateBehavior
{
public:
	// Construction
	CJFCDateEdit();

protected:
	virtual CString GetValidText() const;

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJFCDateEdit)
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CJFCDateEdit)
	afx_msg void OnChar(UINT uChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT uChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG
	bool ShouldEnter(TCHAR c) const;

	afx_msg LONG OnPaste(UINT wParam, LONG lParam);

	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CJFCTimeEdit window

// The CJFCTimeEdit is a CJFCEdit control which supports the TimeBehavior class.
//
class AFX_EXT_CLASS CJFCTimeEdit : public CJFCEdit, 
                     public CJFCEdit::TimeBehavior
{
public:
	// Construction
	CJFCTimeEdit();

protected:
	virtual CString GetValidText() const;

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJFCTimeEdit)
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CJFCTimeEdit)
	afx_msg void OnChar(UINT uChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT uChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	afx_msg LONG OnPaste(UINT wParam, LONG lParam);

	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CJFCDateTimeEdit window

// The CJFCDateTimeEdit is a CJFCEdit control which supports the 
// DateBehavior and TimeBehavior classes.
//
class AFX_EXT_CLASS CJFCDateTimeEdit : public CJFCEdit, 
						 public CJFCEdit::DateTimeBehavior
{
public:
	// Construction
	CJFCDateTimeEdit();

protected:
	virtual CString GetValidText() const;

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJFCDateTimeEdit)
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CJFCDateTimeEdit)
	afx_msg void OnChar(UINT uChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT uChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	afx_msg LONG OnPaste(UINT wParam, LONG lParam);

	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CJFCMultiMaskedEdit window

// The CJFCMultiMaskedEdit class is a CJFCEdit control which can support the 
// AlphanumericBehavior, NumericBehavior, MaskedBehavior, DateBehavior, or 
// TimeBehavior behavior classes.  It uses the mask to determine the current behavior.
//
class AFX_EXT_CLASS CJFCMultiMaskedEdit : public CJFCEdit, 
                            public CJFCEdit::AlphanumericBehavior,
                            public CJFCEdit::NumericBehavior,
                            public CJFCEdit::MaskedBehavior,
                            public CJFCEdit::DateTimeBehavior
{
public:
	CJFCMultiMaskedEdit();

	const CString& GetMask() const;
	void SetMask(const CString& strMask);
	
protected:
	virtual CString GetValidText() const;

	// Attributes
	Behavior* m_pCurrentBehavior;

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJFCMultiMaskedEdit)
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CJFCMultiMaskedEdit)
	afx_msg void OnChar(UINT uChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT uChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	afx_msg LONG OnPaste(UINT wParam, LONG lParam);

	DECLARE_MESSAGE_MAP()
};


#define JFC_MIN_NUMBER			-1.7976931348623158e+308
#define JFC_MAX_NUMBER			 1.7976931348623158e+308
#define JFC_MIN_CTIME			CTime(1970, 1, 1, 0, 0, 0)
#define JFC_MAX_CTIME			CTime(2037, 12, 31, 23, 59, 59)
#define JFC_MIN_OLEDATETIME		COleDateTime(1900, 1, 1, 0, 0, 0)
#define JFC_MAX_OLEDATETIME		COleDateTime(9998, 12, 31, 23, 59, 59)
#define JFC_AM_SYMBOL			_T("AM")
#define JFC_PM_SYMBOL			_T("PM")


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.
