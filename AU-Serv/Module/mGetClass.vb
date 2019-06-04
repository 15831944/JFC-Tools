Option Strict On
Option Explicit On

Module mGetClass
	Private Declare Function apiGetClassName Lib "user32"  Alias "GetClassNameA"(ByVal Hwnd As Integer, ByVal lpClassname As String, ByVal nMaxCount As Integer) As Integer
	Private Declare Function apiGetDesktopWindow Lib "user32"  Alias "GetDesktopWindow"() As Integer
	Private Declare Function apiGetWindow Lib "user32"  Alias "GetWindow"(ByVal Hwnd As Integer, ByVal wCmd As Integer) As Integer
	Private Declare Function apiGetWindowLong Lib "user32"  Alias "GetWindowLongA"(ByVal Hwnd As Integer, ByVal nIndex As Integer) As Integer
	Private Declare Function apiGetWindowText Lib "user32"  Alias "GetWindowTextA"(ByVal Hwnd As Integer, ByVal lpString As String, ByVal aint As Integer) As Integer
	
	Private Const mcGWCHILD As Short = 5
	Private Const mcGWHWNDNEXT As Short = 2
	Private Const mcGWLSTYLE As Short = (-16)
	Private Const mcWSVISIBLE As Integer = &H10000000
	Private Const mconMAXLEN As Short = 255

	Private Function fGetClassName(ByRef Hwnd As Integer) As String
		Dim strBuffer As String
		Dim intCount As Short
		
		strBuffer = New String(Chr(0), mconMAXLEN - 1)
        intCount = CShort(apiGetClassName(Hwnd, strBuffer, mconMAXLEN))
		If intCount > 0 Then
            fGetClassName = Left(strBuffer, intCount)
        Else
            fGetClassName = strBuffer
        End If
	End Function
	
	Private Function fGetCaption(ByRef Hwnd As Integer) As String
		Dim strBuffer As String
		Dim intCount As Short
		
		strBuffer = New String(Chr(0), mconMAXLEN - 1)
        intCount = CShort(apiGetWindowText(Hwnd, strBuffer, mconMAXLEN))
		If intCount > 0 Then
            fGetCaption = Left(strBuffer, intCount)
        Else
            fGetCaption = strBuffer
		End If
	End Function
	
	Public Function FindClass(ByRef TaskClassList As String) As Boolean
        Dim TaskMessage As String = ""
		
		Dim svFindTask As String
		Dim Task() As String
		Dim t As String
        Dim i As Integer
		
		Task = Split(TaskClassList, "|")
		ReDim Preserve Task(20)
		FindClass = False
		
		i = 0
		Do 
			t = Trim(Task(i))
			If t = "" Then Exit Do
			svFindTask = fEnumWindows(t)
			If svFindTask <> "" Then
                If TaskMessage = "" Then
                    TaskMessage = svFindTask
                Else
                    TaskMessage = TaskMessage + vbNewLine + svFindTask
                End If
				FindClass = True
			End If
			i = i + 1
		Loop 
		
		If FindClass Then
            TaskMessage = "Veulliez quitter l'application suivante:" & vbNewLine & vbNewLine + TaskMessage + vbNewLine + vbNewLine + "puis relancer l'application."
			MsgBox(TaskMessage, MsgBoxStyle.Exclamation)
		End If
		
	End Function
	
    Public Function fEnumWindows(ByVal NameClass As String) As String
        Dim lngx As Integer
        Dim lngStyle As Integer
        Dim strCaption As String

        lngx = apiGetDesktopWindow()
        'Return the first child to Desktop
        lngx = apiGetWindow(lngx, mcGWCHILD)

        Do While Not lngx = 0
            strCaption = fGetCaption(lngx)
            If Len(strCaption) > 0 Then
                lngStyle = apiGetWindowLong(lngx, mcGWLSTYLE)
                'enum visible windows only
                If CBool(lngStyle And mcWSVISIBLE) Then
                    If (UCase(NameClass) = UCase(fGetClassName(lngx))) Then
                        fEnumWindows = fGetCaption(lngx)
                        Exit Function
                    End If
                End If
            End If
            lngx = apiGetWindow(lngx, mcGWHWNDNEXT)
        Loop
        fEnumWindows = ""

    End Function

End Module